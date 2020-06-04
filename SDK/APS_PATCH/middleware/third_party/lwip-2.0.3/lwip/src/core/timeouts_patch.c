/**
 * @file
 * Stack-internal timers implementation.
 * This file includes timer callbacks for stack-internal timers as well as
 * functions to set up or stop timers and check for expired timers.
 *
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *         Simon Goldschmidt
 *
 */

#include "lwip/opt.h"

#include "lwip/timeouts.h"
#include "lwip/priv/tcp_priv.h"

#include "lwip/def.h"
#include "lwip/memp.h"
#include "lwip/priv/tcpip_priv.h"

#include "lwip/ip4_frag.h"
#include "lwip/etharp.h"
#include "lwip/dhcp.h"
#include "lwip/autoip.h"
#include "lwip/igmp.h"
#include "lwip/dns.h"
#include "lwip/nd6.h"
#include "lwip/ip6_frag.h"
#include "lwip/mld6.h"
#include "lwip/sys.h"
#include "lwip/pbuf.h"


#if defined(LWIP_ROMBUILD)
#include "timeouts_if.h"
#endif

#include "ps.h"
#include "cmsis_os.h"

#if LWIP_DEBUG_TIMERNAMES
#define HANDLER(x) x, #x
#else /* LWIP_DEBUG_TIMERNAMES */
#define HANDLER(x) x
#endif /* LWIP_DEBUG_TIMERNAMES */


#define LWIP_MAX_TIMEOUT  0x7fffffff
#define LWIP_UINT32_MAX   0xffffffff

/* Check if timer's expiry time is greater than time and care about u32_t wraparounds */
#define TIME_LESS_THAN(t, compare_to) ( (((u32_t)((t)-(compare_to))) > LWIP_MAX_TIMEOUT) ? 1 : 0 )

#if 0
/** This array contains all stack-internal cyclic timers. To get the number of
 * timers, use LWIP_ARRAYSIZE() */
const struct lwip_cyclic_timer lwip_cyclic_timers[] = {
#if LWIP_TCP
  /* The TCP timer is a special case: it does not have to run always and
     is triggered to start from TCP using tcp_timer_needed() */
  {TCP_TMR_INTERVAL, HANDLER(tcp_tmr)},
#endif /* LWIP_TCP */
#if LWIP_IPV4
#if IP_REASSEMBLY
  {IP_TMR_INTERVAL, HANDLER(ip_reass_tmr)},
#endif /* IP_REASSEMBLY */
#if LWIP_ARP
  {ARP_TMR_INTERVAL, HANDLER(etharp_tmr)},
#endif /* LWIP_ARP */
#if LWIP_DHCP
  {DHCP_COARSE_TIMER_MSECS, HANDLER(dhcp_coarse_tmr)},
  {DHCP_FINE_TIMER_MSECS, HANDLER(dhcp_fine_tmr)},
#endif /* LWIP_DHCP */
#if LWIP_AUTOIP
  {AUTOIP_TMR_INTERVAL, HANDLER(autoip_tmr)},
#endif /* LWIP_AUTOIP */
#if LWIP_IGMP
  {IGMP_TMR_INTERVAL, HANDLER(igmp_tmr)},
#endif /* LWIP_IGMP */
#endif /* LWIP_IPV4 */
#if LWIP_DNS
  {DNS_TMR_INTERVAL, HANDLER(dns_tmr)},
#endif /* LWIP_DNS */
#if LWIP_IPV6
  {ND6_TMR_INTERVAL, HANDLER(nd6_tmr)},
#if LWIP_IPV6_REASS
  {IP6_REASS_TMR_INTERVAL, HANDLER(ip6_reass_tmr)},
#endif /* LWIP_IPV6_REASS */
#if LWIP_IPV6_MLD
  {MLD6_TMR_INTERVAL, HANDLER(mld6_tmr)},
#endif /* LWIP_IPV6_MLD */
#endif /* LWIP_IPV6 */
};

const int lwip_num_cyclic_timers = LWIP_ARRAYSIZE(lwip_cyclic_timers);
#endif

#if LWIP_TIMERS && !LWIP_TIMERS_CUSTOM

/** The one and only timeout list */
extern LWIP_RETDATA struct sys_timeo *next_timeout;
extern LWIP_RETDATA u32_t timeouts_last_time;

extern LWIP_RETDATA int timer_suspend_state;
extern LWIP_RETDATA u32_t current_timeout_due_time;
extern LWIP_RETDATA int wakeup_event_timeouts;

#define PS_SMART_SLEEP          1
#define LWIP_USE_NEW_TIMER      1
#define LWIP_DEBUG_SEELPTIME    0

u32_t last_wakeup_time = 0;

#define LWIP_MAX_WAKEUP_TIMEOUT (10*1000 + 500)

#if LWIP_USE_NEW_TIMER

/**
 * Wait (forever) for a message to arrive in an mbox.
 * While waiting, timeouts are processed.
 *
 * @param mbox the mbox to fetch the message from
 * @param msg the place to store the message
 */
void sys_timeouts_mbox_fetch_patch(sys_mbox_t *mbox, void **msg)
{
  u32_t sleeptime, res;
#if LWIP_DEBUG_SEELPTIME
  int s_time, e_time;
#endif
again:
  //LWIP_ASSERT_CORE_LOCKED();

#if PS_SMART_SLEEP
  if (ps_is_smart_sleep_enabled())
  {
      UNLOCK_TCPIP_CORE();
#if LWIP_DEBUG_SEELPTIME
      s_time= osKernelSysTick();
#endif
      res = sys_arch_mbox_fetch(mbox, msg, wakeup_event_timeouts);
#if LWIP_DEBUG_SEELPTIME
      e_time= osKernelSysTick();
      printf("real sleeptime = %d\r\n",e_time - s_time);
#endif
      //prvResetNextTaskUnblockTime();
      LOCK_TCPIP_CORE();
      if (res == SYS_ARCH_TIMEOUT) {
        sys_check_timeouts();
        wakeup_event_timeouts = 100;
        goto again;
      } else {
        // 1. snapshot last wakeup time in wakeup callback fucntion
        // 2. check wakeup_timeouts= sys_now() - last wakeup time
        // 3. if (wakeup_timeouts > 10.5s) wakeup_event_timeouts = 100ms
        if ((sys_now() - last_wakeup_time) > LWIP_MAX_WAKEUP_TIMEOUT)
        {
            printf("wakeup events timeout from last_wakeup_time =%d\r\n", last_wakeup_time);
            wakeup_event_timeouts = 100;
            return;
        }
      }
  }
  else
#endif
  {
    sleeptime = sys_timeouts_sleeptime();

    LWIP_DEBUGF(TIMERS_DEBUG, ("sys_timeout: sleeptime=%"U32_F"\n", sleeptime));

    if (sleeptime == SYS_TIMEOUTS_SLEEPTIME_INFINITE) {
      UNLOCK_TCPIP_CORE();
      sys_arch_mbox_fetch(mbox, msg, 0);
      LOCK_TCPIP_CORE();
      return;
    } else if (sleeptime == 0) {
      sys_check_timeouts();
      /* We try again to fetch a message from the mbox. */
      goto again;
    }

#if LWIP_DEBUG_SEELPTIME
    s_time= osKernelSysTick();
#endif
    UNLOCK_TCPIP_CORE();
    res = sys_arch_mbox_fetch(mbox, msg, sleeptime);
    LOCK_TCPIP_CORE();
#if LWIP_DEBUG_SEELPTIME
    e_time= osKernelSysTick();
#endif
#if LWIP_DEBUG_TIMERNAMES && LWIP_DEBUG_SEELPTIME
    printf("real sleeptime = %d\r\n",e_time - s_time);
#endif
    if (res == SYS_ARCH_TIMEOUT) {
      /* If a SYS_ARCH_TIMEOUT value is returned, a timeout occurred
         before a message could be fetched. */
      sys_check_timeouts();
      /* We try again to fetch a message from the mbox. */
      goto again;
    }
  }
}

#else /* LWIP_USE_NEW_TIMER */

#endif /* LWIP_USE_NEW_TIMER */

void lwip_load_interface_timeouts_patch(void)
{
    sys_timeouts_mbox_fetch_adpt  =  sys_timeouts_mbox_fetch_patch;
}

#else /* LWIP_TIMERS && !LWIP_TIMERS_CUSTOM */
/* Satisfy the TCP code which calls this function */
#endif /* LWIP_TIMERS && !LWIP_TIMERS_CUSTOM */


