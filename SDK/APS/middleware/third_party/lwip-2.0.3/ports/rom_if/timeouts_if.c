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

#if LWIP_DEBUG_TIMERNAMES
#define HANDLER(x) x, #x
#else /* LWIP_DEBUG_TIMERNAMES */
#define HANDLER(x) x
#endif /* LWIP_DEBUG_TIMERNAMES */

#if LWIP_TIMERS && !LWIP_TIMERS_CUSTOM

#if LWIP_TCP
RET_DATA tcpip_tcp_timer_fp_t               tcpip_tcp_timer_adpt;
RET_DATA tcp_timer_needed_fp_t              tcp_timer_needed_adpt;

#endif

#if LWIP_USE_NEW_TIMER
RET_DATA sys_timeout_abs_fp_t               sys_timeout_abs_adpt;
#endif

RET_DATA cyclic_timer_fp_t                  cyclic_timer_adpt;
RET_DATA sys_check_timeouts_fp_t            sys_check_timeouts_adpt;
RET_DATA sys_timeouts_sleeptime_fp_t        sys_timeouts_sleeptime_adpt;

RET_DATA sys_timeouts_init_fp_t             sys_timeouts_init_adpt;
#if LWIP_DEBUG_TIMERNAMES
RET_DATA sys_timeout_debug_fp_t             sys_timeout_debug_adpt;
#else /* LWIP_DEBUG_TIMERNAMES */
RET_DATA sys_timeout_fp_t                   sys_timeout_adpt;
#endif /* LWIP_DEBUG_TIMERNAMES */

RET_DATA sys_untimeout_fp_t                 sys_untimeout_adpt;
RET_DATA sys_restart_timeouts_fp_t          sys_restart_timeouts_adpt;
#if NO_SYS
RET_DATA sys_check_timeouts_fp_t            sys_check_timeouts_adpt;
RET_DATA sys_timeouts_sleeptime_fp_t        sys_timeouts_sleeptime_adpt;
#else /* NO_SYS */
RET_DATA sys_timeouts_mbox_fetch_fp_t       sys_timeouts_mbox_fetch_adpt;
#endif /* NO_SYS */

RET_DATA sys_timeouts_suspend_fp_t          sys_timeouts_suspend_adpt;
RET_DATA sys_timeouts_resume_fp_t           sys_timeouts_resume_adpt;
RET_DATA sys_timeouts_check_fp_t            sys_timeouts_check_adpt;


void lwip_load_interface_timeouts(void)
{
#if LWIP_TIMERS && !LWIP_TIMERS_CUSTOM
    /** The one and only timeout list */
    next_timeout = NULL;
    timeouts_last_time = 0;
    current_timeout_due_time = 0;

    /** Add for power saving smart sleep */
    wakeup_event_timeouts = 1000;

    /** Add for suspend, resume timeouts state */
    timer_suspend_state = 0;
#if LWIP_TCP
    /** global variable that shows if the tcp timer is currently scheduled or not */
    tcpip_tcp_timer_active = 0;
#endif
#endif

#if LWIP_USE_NEW_TIMER
    sys_timeout_abs_adpt                    = LWIP_ROMFN(sys_timeout_abs);
#endif

#if LWIP_TCP
    tcpip_tcp_timer_adpt                    = LWIP_ROMFN(tcpip_tcp_timer);
    tcp_timer_needed_adpt                   = LWIP_ROMFN(tcp_timer_needed);
#endif
    cyclic_timer_adpt                       = LWIP_ROMFN(cyclic_timer);
    sys_check_timeouts_adpt                 = LWIP_ROMFN(sys_check_timeouts);
    sys_timeouts_sleeptime_adpt             = LWIP_ROMFN(sys_timeouts_sleeptime);

    sys_timeouts_init_adpt                  = LWIP_ROMFN(sys_timeouts_init);
#if LWIP_DEBUG_TIMERNAMES
    sys_timeout_debug_adpt                  = LWIP_ROMFN(sys_timeout_debug);
#else /* LWIP_DEBUG_TIMERNAMES */
    sys_timeout_adpt                        = LWIP_ROMFN(sys_timeout);
#endif /* LWIP_DEBUG_TIMERNAMES */

    sys_untimeout_adpt                      = LWIP_ROMFN(sys_untimeout);
    sys_restart_timeouts_adpt               = LWIP_ROMFN(sys_restart_timeouts);
#if NO_SYS
    sys_check_timeouts_adpt                 = LWIP_ROMFN(sys_check_timeouts);
    sys_timeouts_sleeptime_adpt             = LWIP_ROMFN(sys_timeouts_sleeptime);
#else /* NO_SYS */
    sys_timeouts_mbox_fetch_adpt            = LWIP_ROMFN(sys_timeouts_mbox_fetch);
#endif /* NO_SYS */

    sys_timeouts_suspend_adpt               = LWIP_ROMFN(sys_timeouts_suspend);
    sys_timeouts_resume_adpt                = LWIP_ROMFN(sys_timeouts_resume);
    sys_timeouts_check_adpt                 = LWIP_ROMFN(sys_timeouts_check);
}


#if LWIP_TCP

/**
 * Timer callback function that calls tcp_tmr() and reschedules itself.
 *
 * @param arg unused argument
 */
void
tcpip_tcp_timer(void *arg)
{
    tcpip_tcp_timer_adpt(arg);
}

/**
 * Called from TCP_REG when registering a new PCB:
 * the reason is to have the TCP timer only running when
 * there are active (or time-wait) PCBs.
 */
void
tcp_timer_needed(void)
{
    tcp_timer_needed_adpt();
}
#endif /* LWIP_TCP */



#if LWIP_USE_NEW_TIMER
#if LWIP_DEBUG_TIMERNAMES
void sys_timeout_abs(u32_t abs_time, sys_timeout_handler handler, void *arg, const char *handler_name)
{
    sys_timeout_abs_adpt(abs_time, handler, arg, handler_name);
}
#else /* LWIP_DEBUG_TIMERNAMES */
void sys_timeout_abs(u32_t abs_time, sys_timeout_handler handler, void *arg)
{
    sys_timeout_abs_adpt(abs_time, handler, arg);
}
#endif
#endif
/**
 * Timer callback function that calls cyclic_timer() and reschedules itself.
 *
 * @param arg unused argument
 */
void
cyclic_timer(void *arg)
{
    cyclic_timer_adpt(arg);
}

/** Initialize this module */
void sys_timeouts_init(void)
{
    sys_timeouts_init_adpt();
}

/**
 * Create a one-shot timer (aka timeout). Timeouts are processed in the
 * following cases:
 * - while waiting for a message using sys_timeouts_mbox_fetch()
 * - by calling sys_check_timeouts() (NO_SYS==1 only)
 *
 * @param msecs time in milliseconds after that the timer should expire
 * @param handler callback function to call when msecs have elapsed
 * @param arg argument to pass to the callback function
 */
#if LWIP_DEBUG_TIMERNAMES
void
sys_timeout_debug(u32_t msecs, sys_timeout_handler handler, void *arg, const char* handler_name)
{
    sys_timeout_debug_adpt(msecs, handler, arg, handler_name);
}
#else /* LWIP_DEBUG_TIMERNAMES */
void
sys_timeout(u32_t msecs, sys_timeout_handler handler, void *arg)
{
    sys_timeout_adpt(msecs, handler, arg);
}
#endif /* LWIP_DEBUG_TIMERNAMES */

/**
 * Go through timeout list (for this task only) and remove the first matching
 * entry (subsequent entries remain untouched), even though the timeout has not
 * triggered yet.
 *
 * @param handler callback function that would be called by the timeout
 * @param arg callback argument that would be passed to handler
*/
void
sys_untimeout(sys_timeout_handler handler, void *arg)
{
    sys_untimeout_adpt(handler, arg);
}

/**
 * @ingroup lwip_nosys
 * Handle timeouts for NO_SYS==1 (i.e. without using
 * tcpip_thread/sys_timeouts_mbox_fetch(). Uses sys_now() to call timeout
 * handler functions when timeouts expire.
 *
 * Must be called periodically from your main loop.
 */
#if !NO_SYS && !defined __DOXYGEN__

#endif /* !NO_SYS */
void
sys_check_timeouts(void)
{
    sys_check_timeouts_adpt();
}

/** Set back the timestamp of the last call to sys_check_timeouts()
 * This is necessary if sys_check_timeouts() hasn't been called for a long
 * time (e.g. while saving energy) to prevent all timer functions of that
 * period being called.
 */
void
sys_restart_timeouts(void)
{
    sys_restart_timeouts_adpt();
}

/** Return the time left before the next timeout is due. If no timeouts are
 * enqueued, returns 0xffffffff
 */
#if !NO_SYS

#endif /* !NO_SYS */
u32_t
sys_timeouts_sleeptime(void)
{
    return sys_timeouts_sleeptime_adpt();
}


#if !NO_SYS

/**
 * Wait (forever) for a message to arrive in an mbox.
 * While waiting, timeouts are processed.
 *
 * @param mbox the mbox to fetch the message from
 * @param msg the place to store the message
 */
void
sys_timeouts_mbox_fetch(sys_mbox_t *mbox, void **msg)
{
    sys_timeouts_mbox_fetch_adpt(mbox, msg);
}


#endif /* NO_SYS */


void
sys_timeouts_suspend(void *arg)
{
    sys_timeouts_suspend_adpt(arg);
}

void
sys_timeouts_resume(void *arg)
{
    sys_timeouts_resume_adpt(arg);
}

void
sys_timeouts_check(void *arg)
{
    sys_timeouts_check_adpt(arg);
}
#else /* LWIP_TIMERS && !LWIP_TIMERS_CUSTOM */
/* Satisfy the TCP code which calls this function */
void
tcp_timer_needed(void)
{
}
#endif /* LWIP_TIMERS && !LWIP_TIMERS_CUSTOM */
