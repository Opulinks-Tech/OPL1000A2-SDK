/**
 * @file
 * Timer implementations
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
#ifndef LWIP_HDR_TIMEOUTS_IF_H
#define LWIP_HDR_TIMEOUTS_IF_H

#include "lwip/opt.h"
#include "lwip/err.h"
#if !NO_SYS
#include "lwip/sys.h"
#endif

#if defined(LWIP_ROMBUILD)

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LWIP_DEBUG_TIMERNAMES
#ifdef LWIP_DEBUG
#define LWIP_DEBUG_TIMERNAMES SYS_DEBUG
#else /* LWIP_DEBUG */
#define LWIP_DEBUG_TIMERNAMES 0
#endif /* LWIP_DEBUG*/
#endif

#if LWIP_TIMERS

#if LWIP_DEBUG_TIMERNAMES
void sys_timeout_abs(u32_t abs_time, sys_timeout_handler handler, void *arg, const char *handler_name);
#else /* LWIP_DEBUG_TIMERNAMES */
void sys_timeout_abs(u32_t abs_time, sys_timeout_handler handler, void *arg);
#endif

void tcpip_tcp_timer(void *arg);
void cyclic_timer(void *arg);
void sys_check_timeouts(void);
u32_t sys_timeouts_sleeptime(void);

#if LWIP_DEBUG_TIMERNAMES
typedef void (*sys_timeout_abs_fp_t)(u32_t abs_time, sys_timeout_handler handler, void *arg, const char *handler_name);
#else /* LWIP_DEBUG_TIMERNAMES */
typedef void (*sys_timeout_abs_fp_t)(u32_t abs_time, sys_timeout_handler handler, void *arg);
#endif
typedef void (*tcpip_tcp_timer_fp_t)(void *arg);
typedef void (*tcp_timer_needed_fp_t)(void);
typedef void (*cyclic_timer_fp_t)(void *arg);
typedef void (*sys_check_timeouts_fp_t)(void);
typedef u32_t (*sys_timeouts_sleeptime_fp_t)(void);

extern sys_timeout_abs_fp_t            sys_timeout_abs_adpt;

#if LWIP_TCP
extern tcpip_tcp_timer_fp_t            tcpip_tcp_timer_adpt;
extern tcp_timer_needed_fp_t           tcp_timer_needed_adpt;
#endif
extern cyclic_timer_fp_t               cyclic_timer_adpt;
extern sys_check_timeouts_fp_t         sys_check_timeouts_adpt;
extern sys_timeouts_sleeptime_fp_t     sys_timeouts_sleeptime_adpt;



/** Function prototype for a timeout callback function. Register such a function
 * using sys_timeout().
 *
 * @param arg Additional argument to pass to the function - set up by sys_timeout()
 */
typedef void (* sys_timeout_handler)(void *arg);

typedef void (*sys_timeouts_init_fp_t)(void);
#if LWIP_DEBUG_TIMERNAMES
typedef void (*sys_timeout_debug_fp_t)(u32_t msecs, sys_timeout_handler handler, void *arg, const char* handler_name);
#else /* LWIP_DEBUG_TIMERNAMES */
typedef void (*sys_timeout_fp_t)(u32_t msecs, sys_timeout_handler handler, void *arg);
#endif /* LWIP_DEBUG_TIMERNAMES */

typedef void (*sys_untimeout_fp_t)(sys_timeout_handler handler, void *arg);
typedef void (*sys_restart_timeouts_fp_t)(void);
#if NO_SYS
typedef void (*sys_check_timeouts_fp_t)(void);
typedef u32_t (*sys_timeouts_sleeptime_fp_t)(void);
#else /* NO_SYS */
typedef void (*sys_timeouts_mbox_fetch_fp_t)(sys_mbox_t *mbox, void **msg);
#endif /* NO_SYS */

typedef void (*sys_timeouts_suspend_fp_t)(void *arg);
typedef void (*sys_timeouts_resume_fp_t)(void *arg);
typedef void (*sys_timeouts_check_fp_t)(void *arg);

extern sys_timeouts_init_fp_t              sys_timeouts_init_adpt;
#if LWIP_DEBUG_TIMERNAMES
extern sys_timeout_debug_fp_t              sys_timeout_debug_adpt;
#else /* LWIP_DEBUG_TIMERNAMES */
extern sys_timeout_fp_t                    sys_timeout_adpt;
#endif /* LWIP_DEBUG_TIMERNAMES */

extern sys_untimeout_fp_t                  sys_untimeout_adpt;
extern sys_restart_timeouts_fp_t           sys_restart_timeouts_adpt;
#if NO_SYS
extern sys_check_timeouts_fp_t             sys_check_timeouts_adpt;
extern sys_timeouts_sleeptime_fp_t         sys_timeouts_sleeptime_adpt;
#else /* NO_SYS */
extern sys_timeouts_mbox_fetch_fp_t        sys_timeouts_mbox_fetch_adpt;
#endif /* NO_SYS */

extern sys_timeouts_suspend_fp_t           sys_timeouts_suspend_adpt;
extern sys_timeouts_resume_fp_t            sys_timeouts_resume_adpt;
extern sys_timeouts_check_fp_t             sys_timeouts_check_adpt;

#endif /* LWIP_TIMERS */

#ifdef __cplusplus
}
#endif

#endif /*#if defined(LWIP_ROMBUILD)*/

#endif /* LWIP_HDR_TIMEOUTS_IF_H */
