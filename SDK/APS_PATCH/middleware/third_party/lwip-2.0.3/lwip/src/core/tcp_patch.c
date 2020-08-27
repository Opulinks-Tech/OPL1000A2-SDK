/******************************************************************************
*  Copyright 2019, Netlink Communication Corp.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Netlink Communication Corp. (C) 2019
******************************************************************************/

/**
 * @file
 * Transmission Control Protocol for IP
 * See also @ref tcp_raw
 *
 * @defgroup tcp_raw TCP
 * @ingroup callbackstyle_api
 * Transmission Control Protocol for IP\n
 * @see @ref raw_api and @ref netconn
 *
 * Common functions for the TCP implementation, such as functinos
 * for manipulating the data structures and the TCP timer functions. TCP functions
 * related to input and output is found in tcp_in.c and tcp_out.c respectively.\n
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
 *
 */

#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/tcp.h"
#include "lwip/priv/tcp_priv.h"
#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/ip6.h"
#include "lwip/ip6_addr.h"
#include "lwip/nd6.h"
#include "lwip/sys.h"

#include <string.h>
#include "tcp_if.h"
#include "tcp_patch.h"
#include "hal_tick.h"

/* As initial send MSS, we use TCP_MSS but limit it to 536. */
#if TCP_MSS > 536
#define INITIAL_MSS 536
#else
#define INITIAL_MSS TCP_MSS
#endif

#ifndef TCP_LOCAL_PORT_RANGE_START
/* From http://www.iana.org/assignments/port-numbers:
   "The Dynamic and/or Private Ports are those from 49152 through 65535" */
#define TCP_LOCAL_PORT_RANGE_START        0xc000
#define TCP_LOCAL_PORT_RANGE_END          0xffff
#define TCP_ENSURE_LOCAL_PORT_RANGE(port) ((u16_t)(((port) & ~TCP_LOCAL_PORT_RANGE_START) + TCP_LOCAL_PORT_RANGE_START))
#endif

#if LWIP_TCP_KEEPALIVE
#define TCP_KEEP_DUR(pcb)   ((pcb)->keep_cnt * (pcb)->keep_intvl)
#define TCP_KEEP_INTVL(pcb) ((pcb)->keep_intvl)
#else /* LWIP_TCP_KEEPALIVE */
#define TCP_KEEP_DUR(pcb)   TCP_MAXIDLE
#define TCP_KEEP_INTVL(pcb) TCP_KEEPINTVL_DEFAULT
#endif /* LWIP_TCP_KEEPALIVE */

#if OPL_LWIP
uint8_t g_memp_num_tcp_pcb = 0;
#endif

#if !OPL_LWIP
static const u8_t tcp_backoff[13] =
    { 1, 2, 3, 4, 5, 6, 7, 7, 7, 7, 7, 7, 7};
#endif

 /* Times per slowtmr hits */
static const u8_t tcp_persist_backoff[7] = { 3, 6, 12, 24, 48, 96, 120 };

extern u8_t tcp_timer_ctr;
extern u16_t tcp_port;

extern err_t unlock_netconn_state(void *arg, struct tcp_pcb *pcb);

static const char * const tcp_state_str[] = {
  "CLOSED",
  "LISTEN",
  "SYN_SENT",
  "SYN_RCVD",
  "ESTABLISHED",
  "FIN_WAIT_1",
  "FIN_WAIT_2",
  "CLOSE_WAIT",
  "CLOSING",
  "LAST_ACK",
  "TIME_WAIT"
};

static uint32_t sys_rand(void)
{
    uint32_t counter = Hal_Tick_Diff(0);
    uint32_t s;
    srand(counter);
    s = rand();
    return s;
}

/**
 * Called every 500 ms and implements the retransmission timer and the timer that
 * removes PCBs that have been in TIME-WAIT for enough time. It also increments
 * various timers such as the inactivity timer in each PCB.
 *
 * Automatically called from tcp_tmr().
 */
void
tcp_slowtmr_patch(void)
{
  struct tcp_pcb *pcb, *prev;
  tcpwnd_size_t eff_wnd;
  u8_t pcb_remove;      /* flag if a PCB should be removed */
  u8_t pcb_reset;       /* flag if a RST should be sent when removing */
  err_t err;

  err = ERR_OK;

  ++tcp_ticks;
  ++tcp_timer_ctr;

tcp_slowtmr_start:
  /* Steps through all of the active PCBs. */
  prev = NULL;
  pcb = tcp_active_pcbs;
  if (pcb == NULL) {
    LWIP_DEBUGF(TCP_DEBUG, ("tcp_slowtmr: no active pcbs\n"));
  }
  while (pcb != NULL) {
    LWIP_DEBUGF(TCP_DEBUG, ("tcp_slowtmr: processing active pcb\n"));
    LWIP_ASSERT("tcp_slowtmr: active pcb->state != CLOSED\n", pcb->state != CLOSED);
    LWIP_ASSERT("tcp_slowtmr: active pcb->state != LISTEN\n", pcb->state != LISTEN);
    LWIP_ASSERT("tcp_slowtmr: active pcb->state != TIME-WAIT\n", pcb->state != TIME_WAIT);
    if (pcb->last_timer == tcp_timer_ctr) {
      /* skip this pcb, we have already processed it */
      pcb = pcb->next;
      continue;
    }
    pcb->last_timer = tcp_timer_ctr;

    pcb_remove = 0;
    pcb_reset = 0;

    if (pcb->state == SYN_SENT && pcb->nrtx >= TCP_SYNMAXRTX) {
      ++pcb_remove;
      LWIP_DEBUGF(TCP_DEBUG, ("tcp_slowtmr: max SYN retries reached\n"));
    }
    else if (pcb->nrtx >= TCP_MAXRTX) {
      ++pcb_remove;
      LWIP_DEBUGF(TCP_DEBUG, ("tcp_slowtmr: max DATA retries reached\n"));
    } else {
      if (pcb->persist_backoff > 0) {
        /* If snd_wnd is zero, use persist timer to send 1 byte probes
         * instead of using the standard retransmission mechanism. */
        u8_t backoff_cnt = tcp_persist_backoff[pcb->persist_backoff-1];
        if (pcb->persist_cnt < backoff_cnt) {
          pcb->persist_cnt++;
        }
        if (pcb->persist_cnt >= backoff_cnt) {
          if (tcp_zero_window_probe(pcb) == ERR_OK) {
            pcb->persist_cnt = 0;
            if (pcb->persist_backoff < sizeof(tcp_persist_backoff)) {
              pcb->persist_backoff++;
            }
          }
        }
      } else {
        /* Increase the retransmission timer if it is running */
        if (pcb->rtime >= 0) {
          ++pcb->rtime;
        }

        if (pcb->unacked != NULL && pcb->rtime >= pcb->rto) {
          /* Time for a retransmission. */
          LWIP_DEBUGF(TCP_RTO_DEBUG, ("tcp_slowtmr: rtime %"S16_F
                                      " pcb->rto %"S16_F"\n",
                                      pcb->rtime, pcb->rto));

          /* Double retransmission time-out unless we are trying to
           * connect to somebody (i.e., we are in SYN_SENT). */
          if (pcb->state != SYN_SENT) {
#if OPL_LWIP
              pcb->rto = 3; // (6=3000 ms, 3=1500 ms)
#else
            u8_t backoff_idx = LWIP_MIN(pcb->nrtx, sizeof(tcp_backoff)-1);
            pcb->rto = ((pcb->sa >> 3) + pcb->sv) << tcp_backoff[backoff_idx];
#endif
          }

          /* Reset the retransmission timer. */
          pcb->rtime = 0;

          /* Reduce congestion window and ssthresh. */
          eff_wnd = LWIP_MIN(pcb->cwnd, pcb->snd_wnd);
          pcb->ssthresh = eff_wnd >> 1;
          if (pcb->ssthresh < (tcpwnd_size_t)(pcb->mss << 1)) {
            pcb->ssthresh = (pcb->mss << 1);
          }
          pcb->cwnd = pcb->mss;
          LWIP_DEBUGF(TCP_CWND_DEBUG, ("tcp_slowtmr: cwnd %"TCPWNDSIZE_F
                                       " ssthresh %"TCPWNDSIZE_F"\n",
                                       pcb->cwnd, pcb->ssthresh));

          /* The following needs to be called AFTER cwnd is set to one
             mss - STJ */
          tcp_rexmit_rto(pcb);
        }
      }
    }
#if OPL_LWIP
    /* Check if this PCB has stayed too long in FIN-WAIT-1 or FIN-WAIT-2 */
    if ((pcb->state == FIN_WAIT_1) || (pcb->state == FIN_WAIT_2)) {
#else
    /* Check if this PCB has stayed too long in FIN-WAIT-2 */
    if (pcb->state == FIN_WAIT_2) {
#endif
      /* If this PCB is in FIN_WAIT_2 because of SHUT_WR don't let it time out. */
      if (pcb->flags & TF_RXCLOSED) {
        /* PCB was fully closed (either through close() or SHUT_RDWR):
           normal FIN-WAIT timeout handling. */
        if ((u32_t)(tcp_ticks - pcb->tmr) >
            TCP_FIN_WAIT_TIMEOUT / TCP_SLOW_INTERVAL) {
          ++pcb_remove;
          LWIP_DEBUGF(TCP_DEBUG, ("tcp_slowtmr: removing pcb stuck in FIN-WAIT-2\n"));
        }
      }
    }

    /* Check if KEEPALIVE should be sent */
    if (ip_get_option(pcb, SOF_KEEPALIVE) &&
       ((pcb->state == ESTABLISHED) ||
        (pcb->state == CLOSE_WAIT))) {
      if ((u32_t)(tcp_ticks - pcb->tmr) >
         (pcb->keep_idle + TCP_KEEP_DUR(pcb)) / TCP_SLOW_INTERVAL)
      {
        LWIP_DEBUGF(TCP_DEBUG, ("tcp_slowtmr: KEEPALIVE timeout. Aborting connection to "));
        ip_addr_debug_print(TCP_DEBUG, &pcb->remote_ip);
        LWIP_DEBUGF(TCP_DEBUG, ("\n"));

        ++pcb_remove;
        ++pcb_reset;
      } else if ((u32_t)(tcp_ticks - pcb->tmr) >
                (pcb->keep_idle + pcb->keep_cnt_sent * TCP_KEEP_INTVL(pcb))
                / TCP_SLOW_INTERVAL)
      {
        err = tcp_keepalive(pcb);
        if (err == ERR_OK) {
          pcb->keep_cnt_sent++;
        }
      }
    }

    /* If this PCB has queued out of sequence data, but has been
       inactive for too long, will drop the data (it will eventually
       be retransmitted). */
#if TCP_QUEUE_OOSEQ
    if (pcb->ooseq != NULL &&
        (u32_t)tcp_ticks - pcb->tmr >= pcb->rto * TCP_OOSEQ_TIMEOUT) {
      tcp_segs_free(pcb->ooseq);
      pcb->ooseq = NULL;
      LWIP_DEBUGF(TCP_CWND_DEBUG, ("tcp_slowtmr: dropping OOSEQ queued data\n"));
    }
#endif /* TCP_QUEUE_OOSEQ */

    /* Check if this PCB has stayed too long in SYN-RCVD */
    if (pcb->state == SYN_RCVD) {
      if ((u32_t)(tcp_ticks - pcb->tmr) >
          TCP_SYN_RCVD_TIMEOUT / TCP_SLOW_INTERVAL) {
        ++pcb_remove;
        LWIP_DEBUGF(TCP_DEBUG, ("tcp_slowtmr: removing pcb stuck in SYN-RCVD\n"));
      }
    }

    /* Check if this PCB has stayed too long in LAST-ACK */
    if (pcb->state == LAST_ACK) {
      if ((u32_t)(tcp_ticks - pcb->tmr) > 2 * TCP_MSL / TCP_SLOW_INTERVAL) {
        ++pcb_remove;
        LWIP_DEBUGF(TCP_DEBUG, ("tcp_slowtmr: removing pcb stuck in LAST-ACK\n"));
      }
    }

    /* If the PCB should be removed, do it. */
    if (pcb_remove) {
      struct tcp_pcb *pcb2;
#if LWIP_CALLBACK_API
      tcp_err_fn err_fn = pcb->errf;
#endif /* LWIP_CALLBACK_API */
      void *err_arg;
      enum tcp_state last_state;
      tcp_pcb_purge(pcb);
      /* Remove PCB from tcp_active_pcbs list. */
      if (prev != NULL) {
        LWIP_ASSERT("tcp_slowtmr: middle tcp != tcp_active_pcbs", pcb != tcp_active_pcbs);
        prev->next = pcb->next;
      } else {
        /* This PCB was the first. */
        LWIP_ASSERT("tcp_slowtmr: first pcb == tcp_active_pcbs", tcp_active_pcbs == pcb);
        tcp_active_pcbs = pcb->next;
      }

      if (pcb_reset) {
        tcp_rst(pcb->snd_nxt, pcb->rcv_nxt, &pcb->local_ip, &pcb->remote_ip,
                 pcb->local_port, pcb->remote_port);
      }

      err_arg = pcb->callback_arg;
      last_state = pcb->state;
      pcb2 = pcb;
      pcb = pcb->next;
      memp_free(MEMP_TCP_PCB, pcb2);

      tcp_active_pcbs_changed = 0;
      TCP_EVENT_ERR(last_state, err_fn, err_arg, ERR_ABRT);
      if (tcp_active_pcbs_changed) {
        goto tcp_slowtmr_start;
      }
    } else {
      /* get the 'next' element now and work with 'prev' below (in case of abort) */
      prev = pcb;
      pcb = pcb->next;

      /* We check if we should poll the connection. */
      ++prev->polltmr;
      if (prev->polltmr >= prev->pollinterval) {
        prev->polltmr = 0;
        LWIP_DEBUGF(TCP_DEBUG, ("tcp_slowtmr: polling application\n"));
        tcp_active_pcbs_changed = 0;
        TCP_EVENT_POLL(prev, err);
        if (tcp_active_pcbs_changed) {
          goto tcp_slowtmr_start;
        }
        /* if err == ERR_ABRT, 'prev' is already deallocated */
        if (err == ERR_OK) {
          tcp_output(prev);
        }
      }
    }
  }


  /* Steps through all of the TIME-WAIT PCBs. */
  prev = NULL;
  pcb = tcp_tw_pcbs;
  while (pcb != NULL) {
    LWIP_ASSERT("tcp_slowtmr: TIME-WAIT pcb->state == TIME-WAIT", pcb->state == TIME_WAIT);
    pcb_remove = 0;

    /* Check if this PCB has stayed long enough in TIME-WAIT */
    if ((u32_t)(tcp_ticks - pcb->tmr) > 2 * TCP_MSL / TCP_SLOW_INTERVAL) {
      ++pcb_remove;
    }

    /* If the PCB should be removed, do it. */
    if (pcb_remove) {
      struct tcp_pcb *pcb2;
      tcp_pcb_purge(pcb);
      /* Remove PCB from tcp_tw_pcbs list. */
      if (prev != NULL) {
        LWIP_ASSERT("tcp_slowtmr: middle tcp != tcp_tw_pcbs", pcb != tcp_tw_pcbs);
        prev->next = pcb->next;
      } else {
        /* This PCB was the first. */
        LWIP_ASSERT("tcp_slowtmr: first pcb == tcp_tw_pcbs", tcp_tw_pcbs == pcb);
        tcp_tw_pcbs = pcb->next;
      }
      pcb2 = pcb;
      pcb = pcb->next;
      memp_free(MEMP_TCP_PCB, pcb2);
    } else {
      prev = pcb;
      pcb = pcb->next;
    }
  }
}

#if OPL_LWIP
typedef struct {
    u8_t time_wait;
    u8_t closing;
    u8_t fin_wait2;
    u8_t last_ack;
    u8_t fin_wait1;
    u8_t listen;
    u8_t bound;
    u8_t total;
}tcp_pcb_num_t;

static void tcp_pcb_num_cal(tcp_pcb_num_t *tcp_pcb_num)
{
    struct tcp_pcb_listen *listen;
    struct tcp_pcb *pcb;

    if (!tcp_pcb_num){
        return;
    }

    memset(tcp_pcb_num, 0, sizeof(*tcp_pcb_num));
    for(pcb = tcp_tw_pcbs; pcb != NULL; pcb = pcb->next) {
        tcp_pcb_num->total ++;
        tcp_pcb_num->time_wait ++;
    }

    for (pcb = tcp_active_pcbs; pcb != NULL; pcb = pcb->next){
        tcp_pcb_num->total ++;
        if (pcb->state == FIN_WAIT_2){
            tcp_pcb_num->fin_wait2 ++;
        } else if (pcb->state == LAST_ACK) {
            tcp_pcb_num->last_ack ++;
        } else if (pcb->state == CLOSING) {
            tcp_pcb_num->closing ++;
        } else if (pcb->state == FIN_WAIT_1){
            tcp_pcb_num->fin_wait1 ++;
        }
    }

    for (listen = tcp_listen_pcbs.listen_pcbs; listen != NULL; listen = listen->next){
        tcp_pcb_num->total ++;
        tcp_pcb_num->listen ++;
    }

    for (pcb = tcp_bound_pcbs; pcb != NULL; pcb = pcb->next){
        tcp_pcb_num->total ++;
        tcp_pcb_num->bound ++;
    }
}
#endif /* #if OPL_LWIP */

/**
 * Allocate a new tcp_pcb structure.
 *
 * @param prio priority for the new pcb
 * @return a new tcp_pcb that initially is in state CLOSED
 */
struct tcp_pcb *
tcp_alloc_patch(u8_t prio)
{
  struct tcp_pcb *pcb;

#if OPL_LWIP
    tcp_pcb_num_t tcp_pcb_num;

    tcp_pcb_num_cal(&tcp_pcb_num);

    if (tcp_pcb_num.total >= g_memp_num_tcp_pcb){
        if (tcp_pcb_num.time_wait > 0){
            tcp_kill_timewait();
        } else if (tcp_pcb_num.last_ack > 0){
            tcp_kill_state(LAST_ACK);
        } else if (tcp_pcb_num.closing > 0){
            tcp_kill_state(CLOSING);
        } else if (tcp_pcb_num.fin_wait2 > 0){
            tcp_kill_state(FIN_WAIT_2);
        } else if (tcp_pcb_num.fin_wait1 > 0){
            tcp_kill_state(FIN_WAIT_1);
        } else {
            tcp_kill_prio(prio);
        }
    }

    tcp_pcb_num_cal(&tcp_pcb_num);
    if (tcp_pcb_num.total >= MEMP_NUM_TCP_PCB){
        LWIP_DEBUGF(TCP_DEBUG, ("tcp_alloc: no available tcp pcb %d %d %d %d %d %d %d %d\n",
           tcp_pcb_num.total, tcp_pcb_num.time_wait, tcp_pcb_num.last_ack, tcp_pcb_num.closing,
           tcp_pcb_num.fin_wait2, tcp_pcb_num.fin_wait1, tcp_pcb_num.listen, tcp_pcb_num.bound));
        return NULL;
    }
#endif /* #if OPL_LWIP */

  pcb = (struct tcp_pcb *)memp_malloc(MEMP_TCP_PCB);
  if (pcb == NULL) {
    /* Try killing oldest connection in TIME-WAIT. */
    LWIP_DEBUGF(TCP_DEBUG, ("tcp_alloc: killing off oldest TIME-WAIT connection\n"));
    tcp_kill_timewait();
    /* Try to allocate a tcp_pcb again. */
    pcb = (struct tcp_pcb *)memp_malloc(MEMP_TCP_PCB);
    if (pcb == NULL) {
      /* Try killing oldest connection in LAST-ACK (these wouldn't go to TIME-WAIT). */
      LWIP_DEBUGF(TCP_DEBUG, ("tcp_alloc: killing off oldest LAST-ACK connection\n"));
      tcp_kill_state(LAST_ACK);
      /* Try to allocate a tcp_pcb again. */
      pcb = (struct tcp_pcb *)memp_malloc(MEMP_TCP_PCB);
      if (pcb == NULL) {
        /* Try killing oldest connection in CLOSING. */
        LWIP_DEBUGF(TCP_DEBUG, ("tcp_alloc: killing off oldest CLOSING connection\n"));
        tcp_kill_state(CLOSING);
        /* Try to allocate a tcp_pcb again. */
        pcb = (struct tcp_pcb *)memp_malloc(MEMP_TCP_PCB);
        if (pcb == NULL) {
          /* Try killing active connections with lower priority than the new one. */
          LWIP_DEBUGF(TCP_DEBUG, ("tcp_alloc: killing connection with prio lower than %d\n", prio));
          tcp_kill_prio(prio);
          /* Try to allocate a tcp_pcb again. */
          pcb = (struct tcp_pcb *)memp_malloc(MEMP_TCP_PCB);
          if (pcb != NULL) {
            /* adjust err stats: memp_malloc failed multiple times before */
            MEMP_STATS_DEC(err, MEMP_TCP_PCB);
          }
        }
        if (pcb != NULL) {
          /* adjust err stats: memp_malloc failed multiple times before */
          MEMP_STATS_DEC(err, MEMP_TCP_PCB);
        }
      }
      if (pcb != NULL) {
        /* adjust err stats: memp_malloc failed multiple times before */
        MEMP_STATS_DEC(err, MEMP_TCP_PCB);
      }
    }
    if (pcb != NULL) {
      /* adjust err stats: memp_malloc failed above */
      MEMP_STATS_DEC(err, MEMP_TCP_PCB);
    }
  }
  if (pcb != NULL) {
    /* zero out the whole pcb, so there is no need to initialize members to zero */
    memset(pcb, 0, sizeof(struct tcp_pcb));
    pcb->prio = prio;
    pcb->snd_buf = TCP_SND_BUF;

    /* Start with a window that does not need scaling. When window scaling is
       enabled and used, the window is enlarged when both sides agree on scaling. */
    pcb->rcv_wnd = pcb->rcv_ann_wnd = TCPWND_MIN16(TCP_WND);
    pcb->ttl = TCP_TTL;
    /* As initial send MSS, we use TCP_MSS but limit it to 536.
       The send MSS is updated when an MSS option is received. */
    pcb->mss = INITIAL_MSS;
#if OPL_LWIP
    pcb->rto = 1500 / TCP_SLOW_INTERVAL;
    pcb->sv = 1500 / TCP_SLOW_INTERVAL;
#else
    pcb->rto = 3000 / TCP_SLOW_INTERVAL;
    pcb->sv = 3000 / TCP_SLOW_INTERVAL;
#endif
    pcb->rtime = -1;
    pcb->cwnd = 1;
    pcb->tmr = tcp_ticks;
    pcb->last_timer = tcp_timer_ctr;

    /* RFC 5681 recommends setting ssthresh abritrarily high and gives an example
    of using the largest advertised receive window.  We've seen complications with
    receiving TCPs that use window scaling and/or window auto-tuning where the
    initial advertised window is very small and then grows rapidly once the
    connection is established. To avoid these complications, we set ssthresh to the
    largest effective cwnd (amount of in-flight data) that the sender can have. */
    pcb->ssthresh = TCP_SND_BUF;

#if LWIP_CALLBACK_API
    pcb->recv = tcp_recv_null;
#endif /* LWIP_CALLBACK_API */

    /* Init KEEPALIVE timer */
    pcb->keep_idle  = TCP_KEEPIDLE_DEFAULT;

#if LWIP_TCP_KEEPALIVE
    pcb->keep_intvl = TCP_KEEPINTVL_DEFAULT;
    pcb->keep_cnt   = TCP_KEEPCNT_DEFAULT;
#endif /* LWIP_TCP_KEEPALIVE */
  }
  return pcb;
}

/**
 * Kills the oldest connection that is in specific state.
 * Called from tcp_alloc() for LAST_ACK and CLOSING if no more connections are available.
 */
static void
tcp_kill_state_patch(enum tcp_state state)
{
  struct tcp_pcb *pcb, *inactive;
  u32_t inactivity;

#if !OPL_LWIP
  LWIP_ASSERT("invalid state", (state == CLOSING) || (state == LAST_ACK));
#endif

  inactivity = 0;
  inactive = NULL;
  /* Go through the list of active pcbs and get the oldest pcb that is in state
     CLOSING/LAST_ACK. */
  for (pcb = tcp_active_pcbs; pcb != NULL; pcb = pcb->next) {
    if (pcb->state == state) {
      if ((u32_t)(tcp_ticks - pcb->tmr) >= inactivity) {
        inactivity = tcp_ticks - pcb->tmr;
        inactive = pcb;
      }
    }
  }
  if (inactive != NULL) {
    LWIP_DEBUGF(TCP_DEBUG, ("tcp_kill_closing: killing oldest %s PCB %p (%"S32_F")\n",
           tcp_state_str[state], (void *)inactive, inactivity));
    /* Don't send a RST, since no data is lost. */
    tcp_abandon(inactive, 0);
  }
}

void tcp_init_patch(void)
{
#if LWIP_RANDOMIZE_INITIAL_LOCAL_PORTS && defined(LWIP_RAND)
  tcp_port = TCP_ENSURE_LOCAL_PORT_RANGE(sys_rand());
#endif /* LWIP_RANDOMIZE_INITIAL_LOCAL_PORTS && defined(LWIP_RAND) */
}

/**
 * Allocate a new local TCP port.
 *
 * @return a new (free) local TCP port number
 */
static u16_t tcp_new_port_patch(void)
{
  u8_t i;
  u16_t n = 0;
  struct tcp_pcb *pcb;

again:

#if OPL_RANDOM_TCP_PORT
  tcp_port = sys_rand() % (TCP_LOCAL_PORT_RANGE_END - TCP_LOCAL_PORT_RANGE_START);
  tcp_port += TCP_LOCAL_PORT_RANGE_START;
#else
  if (tcp_port++ == TCP_LOCAL_PORT_RANGE_END) {
    tcp_port = TCP_LOCAL_PORT_RANGE_START;
  }
#endif
  LWIP_DEBUGF(TCP_DEBUG, ("tcp_new_port: %d\r\n", tcp_port));

  /* Check all PCB lists. */
  for (i = 0; i < NUM_TCP_PCB_LISTS; i++) {
    for (pcb = *tcp_pcb_lists[i]; pcb != NULL; pcb = pcb->next) {
      if (pcb->local_port == tcp_port) {
        if (++n > (TCP_LOCAL_PORT_RANGE_END - TCP_LOCAL_PORT_RANGE_START)) {
          return 0;
        }
        goto again;
      }
    }
  }
  return tcp_port;
}


#if OPL_LWIP
int tcp_kill_pcb_check_threshold_set(uint8_t num)
{
    if (num == 0 || num > MEMP_NUM_TCP_PCB) {
        g_memp_num_tcp_pcb = MEMP_NUM_TCP_PCB;
    }
    else {
        g_memp_num_tcp_pcb = num;
    }
    return 0;
}
#endif

void lwip_load_interface_tcp_patch(void)
{
#if OPL_LWIP
    g_memp_num_tcp_pcb = MEMP_NUM_TCP_PCB;
#endif
    tcp_init_adpt       = tcp_init_patch;
    tcp_slowtmr_adpt    = tcp_slowtmr_patch;
    tcp_alloc_adpt      = tcp_alloc_patch;
    tcp_new_port_adpt   = tcp_new_port_patch;
    tcp_kill_state_adpt = tcp_kill_state_patch;
}
