/**
 * @file
 * Functions to sync with TCPIP thread
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
#ifndef LWIP_HDR_TCPIP_IF_H
#define LWIP_HDR_TCPIP_IF_H

#include "lwip/opt.h"

#if defined(LWIP_ROMBUILD)

#if !NO_SYS /* don't build if not configured for use in lwipopts.h */

#include "lwip/err.h"
#include "lwip/timeouts.h"
#include "lwip/netif.h"

#ifdef __cplusplus
extern "C" {
#endif

struct pbuf;
struct netif;

/** Function prototype for the init_done function passed to tcpip_init */
typedef void (*tcpip_init_done_fn)(void *arg);
/** Function prototype for functions passed to tcpip_callback() */
typedef void (*tcpip_callback_fn)(void *ctx);

/* Forward declarations */
struct tcpip_callback_msg;

/* Private static API fucntion used in tcpip.c*/
void tcpip_thread(void *arg);
void pbuf_free_int(void *p);

typedef void (*tcpip_thread_fp_t)(void *arg);
typedef void (*pbuf_free_int_fp_t)(void *p);

extern tcpip_thread_fp_t       tcpip_thread_adpt;
extern pbuf_free_int_fp_t      pbuf_free_int_adpt;


/* Public API fucntion in tcpip.c*/
/* declaration in tcpip_prv.h */
typedef err_t (*tcpip_send_msg_wait_sem_fp_t)(tcpip_callback_fn fn, void *apimsg, sys_sem_t* sem);
typedef err_t (*tcpip_api_call_fp_t)(tcpip_api_call_fn fn, struct tcpip_api_call_data *call);


/* declaration in tcpip.h */
typedef void   (*tcpip_init_fp_t)(tcpip_init_done_fn tcpip_init_done, void *arg);
typedef err_t  (*tcpip_inpkt_fp_t)(struct pbuf *p, struct netif *inp, netif_input_fn input_fn);
typedef err_t  (*tcpip_input_fp_t)(struct pbuf *p, struct netif *inp);
typedef err_t  (*tcpip_callback_with_block_fp_t)(tcpip_callback_fn function, void *ctx, u8_t block);
typedef struct tcpip_callback_msg* (*tcpip_callbackmsg_new_fp_t)(tcpip_callback_fn function, void *ctx);
typedef void   (*tcpip_callbackmsg_delete_fp_t)(struct tcpip_callback_msg* msg);
typedef err_t  (*tcpip_trycallback_fp_t)(struct tcpip_callback_msg* msg);

/* free pbufs or heap memory from another context without blocking */
typedef err_t  (*pbuf_free_callback_fp_t)(struct pbuf *p);
typedef err_t  (*mem_free_callback_fp_t)(void *m);

#if LWIP_TCPIP_TIMEOUT && LWIP_TIMERS
typedef err_t  (*tcpip_timeout_fp_t)(u32_t msecs, sys_timeout_handler h, void *arg);
typedef err_t  (*tcpip_untimeout_fp_t)(sys_timeout_handler h, void *arg);
#endif /* LWIP_TCPIP_TIMEOUT && LWIP_TIMERS */
typedef err_t  (*tcpip_deactivate_tcpip_timeouts_fp_t)(void);
typedef err_t  (*tcpip_activate_tcpip_timeouts_fp_t)(void);
typedef err_t  (*tcpip_check_timeouts_fp_t)(void);

extern tcpip_send_msg_wait_sem_fp_t               tcpip_send_msg_wait_sem_adpt;
extern tcpip_api_call_fp_t                        tcpip_api_call_adpt;
extern tcpip_init_fp_t                            tcpip_init_adpt;
extern tcpip_inpkt_fp_t                           tcpip_inpkt_adpt;
extern tcpip_input_fp_t                           tcpip_input_adpt;
extern tcpip_callback_with_block_fp_t             tcpip_callback_with_block_adpt;
extern tcpip_callbackmsg_new_fp_t                 tcpip_callbackmsg_new_adpt;
extern tcpip_callbackmsg_delete_fp_t              tcpip_callbackmsg_delete_adpt;
extern tcpip_trycallback_fp_t                     tcpip_trycallback_adpt;
extern pbuf_free_callback_fp_t                    pbuf_free_callback_adpt;
extern mem_free_callback_fp_t                     mem_free_callback_adpt;

#if LWIP_TCPIP_TIMEOUT && LWIP_TIMERS
extern tcpip_timeout_fp_t                         tcpip_timeout_adpt;
extern tcpip_untimeout_fp_t                       tcpip_untimeout_adpt;
#endif /* LWIP_TCPIP_TIMEOUT && LWIP_TIMERS */
extern tcpip_deactivate_tcpip_timeouts_fp_t       tcpip_deactivate_tcpip_timeouts_adpt;
extern tcpip_activate_tcpip_timeouts_fp_t         tcpip_activate_tcpip_timeouts_adpt;
extern tcpip_check_timeouts_fp_t                  tcpip_check_timeouts_adpt;

#ifdef __cplusplus
}
#endif

#endif /* !NO_SYS */

#endif /*#if defined(LWIP_ROMBUILD)*/

#endif /* LWIP_HDR_TCPIP_IF_H */
