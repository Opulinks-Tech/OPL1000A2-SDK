/**
 * @file
 * raw API (to be used from TCPIP thread)\n
 * See also @ref raw_raw
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
#ifndef LWIP_HDR_RAW_IF_H
#define LWIP_HDR_RAW_IF_H

#include "lwip/opt.h"

#if defined(LWIP_ROMBUILD)

#if LWIP_RAW /* don't build if not configured for use in lwipopts.h */

#include "lwip/pbuf.h"
#include "lwip/def.h"
#include "lwip/ip.h"
#include "lwip/ip_addr.h"
#include "lwip/ip6_addr.h"

#ifdef __cplusplus
extern "C" {
#endif


/** Function prototype for raw pcb receive callback functions.
 * @param arg user supplied argument (raw_pcb.recv_arg)
 * @param pcb the raw_pcb which received data
 * @param p the packet buffer that was received
 * @param addr the remote IP address from which the packet was received
 * @return 1 if the packet was 'eaten' (aka. deleted),
 *         0 if the packet lives on
 * If returning 1, the callback is responsible for freeing the pbuf
 * if it's not used any more.
 */
typedef u8_t (*raw_recv_fn)(void *arg, struct raw_pcb *pcb, struct pbuf *p,
    const ip_addr_t *addr);

/** the RAW protocol control block */
struct raw_pcb;

/* Private static API fucntion used in raw.c*/
u8_t raw_input_match(struct raw_pcb *pcb, u8_t broadcast);
typedef u8_t (*raw_input_match_fp_t)(struct raw_pcb *pcb, u8_t broadcast);

extern raw_input_match_fp_t raw_input_match_adpt;


/* The following functions is the application layer interface to the
   RAW code. */
typedef struct raw_pcb * (*raw_new_fp_t)        (u8_t proto);
typedef struct raw_pcb * (*raw_new_ip_type_fp_t)(u8_t type, u8_t proto);
typedef void             (*raw_remove_fp_t)     (struct raw_pcb *pcb);
typedef err_t            (*raw_bind_fp_t)       (struct raw_pcb *pcb, const ip_addr_t *ipaddr);
typedef err_t            (*raw_connect_fp_t)    (struct raw_pcb *pcb, const ip_addr_t *ipaddr);
typedef err_t            (*raw_sendto_fp_t)     (struct raw_pcb *pcb, struct pbuf *p, const ip_addr_t *ipaddr);
typedef err_t            (*raw_send_fp_t)       (struct raw_pcb *pcb, struct pbuf *p);
typedef void             (*raw_recv_fp_t)       (struct raw_pcb *pcb, raw_recv_fn recv, void *recv_arg);

/* The following functions are the lower layer interface to RAW. */
typedef u8_t             (*raw_input_fp_t)      (struct pbuf *p, struct netif *inp);
//#define raw_init() /* Compatibility define, no init needed. */

typedef void (*raw_netif_ip_addr_changed_fp_t)(const ip_addr_t* old_addr, const ip_addr_t* new_addr);

/* for compatibility with older implementation */
//#define raw_new_ip6(proto) raw_new_ip_type(IPADDR_TYPE_V6, proto)

extern raw_new_fp_t                     raw_new_adpt;
extern raw_new_ip_type_fp_t             raw_new_ip_type_adpt;
extern raw_remove_fp_t                  raw_remove_adpt;
extern raw_bind_fp_t                    raw_bind_adpt;
extern raw_connect_fp_t                 raw_connect_adpt;
extern raw_sendto_fp_t                  raw_sendto_adpt;
extern raw_send_fp_t                    raw_send_adpt;
extern raw_recv_fp_t                    raw_recv_adpt;
extern raw_input_fp_t                   raw_input_adpt;
extern raw_netif_ip_addr_changed_fp_t   raw_netif_ip_addr_changed_adpt;

#ifdef __cplusplus
}
#endif

#endif /* LWIP_RAW */

#endif /* #if defined(LWIP_ROMBUILD)*/
#endif /* LWIP_HDR_RAW_IF_H */
