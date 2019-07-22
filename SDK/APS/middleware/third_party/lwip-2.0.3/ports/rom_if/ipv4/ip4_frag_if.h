/**
 * @file
 * IP fragmentation/reassembly
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
 * Author: Jani Monoses <jani@iv.ro>
 *
 */

#ifndef LWIP_HDR_IP4_FRAG_IF_H
#define LWIP_HDR_IP4_FRAG_IF_H

#include "lwip/opt.h"
#include "lwip/err.h"
#include "lwip/pbuf.h"
#include "lwip/netif.h"
#include "lwip/ip_addr.h"
#include "lwip/ip.h"

#if defined(LWIP_ROMBUILD)

#if LWIP_IPV4

#ifdef __cplusplus
extern "C" {
#endif


struct ip_reassdata;
struct pbuf_custom_ref;
struct ip_hdr;

/* Private static API fucntion used in ip4_frag.c*/
int  ip_reass_free_complete_datagram(struct ip_reassdata *ipr, struct ip_reassdata *prev);
int  ip_reass_remove_oldest_datagram(struct ip_hdr *fraghdr, int pbufs_needed);
void ip_reass_dequeue_datagram(struct ip_reassdata *ipr, struct ip_reassdata *prev);
int  ip_reass_chain_frag_into_datagram_and_validate(struct ip_reassdata *ipr, struct pbuf *new_p, int is_last);
struct ip_reassdata*    ip_reass_enqueue_new_datagram(struct ip_hdr *fraghdr, int clen);

#if IP_FRAG
#if !LWIP_NETIF_TX_SINGLE_PBUF
void ip_frag_free_pbuf_custom_ref(struct pbuf_custom_ref* p);
void ipfrag_free_pbuf_custom(struct pbuf *p);
struct pbuf_custom_ref* ip_frag_alloc_pbuf_custom_ref(void);
#endif /* !LWIP_NETIF_TX_SINGLE_PBUF */
#endif


typedef int  (*ip_reass_free_complete_datagram_fp_t)(struct ip_reassdata *ipr, struct ip_reassdata *prev);
typedef int  (*ip_reass_remove_oldest_datagram_fp_t)(struct ip_hdr *fraghdr, int pbufs_needed);
typedef void (*ip_reass_dequeue_datagram_fp_t)(struct ip_reassdata *ipr, struct ip_reassdata *prev);
typedef int  (*ip_reass_chain_frag_into_datagram_and_validate_fp_t)(struct ip_reassdata *ipr, struct pbuf *new_p, int is_last);
typedef struct ip_reassdata*    (*ip_reass_enqueue_new_datagram_fp_t)(struct ip_hdr *fraghdr, int clen);

#if IP_FRAG
#if !LWIP_NETIF_TX_SINGLE_PBUF
typedef void (*ip_frag_free_pbuf_custom_ref_fp_t)(struct pbuf_custom_ref* p);
typedef void (*ipfrag_free_pbuf_custom_fp_t)(struct pbuf *p);
typedef struct pbuf_custom_ref* (*ip_frag_alloc_pbuf_custom_ref_fp_t)(void);
#endif /* !LWIP_NETIF_TX_SINGLE_PBUF */
#endif


extern ip_reass_free_complete_datagram_fp_t                    ip_reass_free_complete_datagram_adpt;
extern ip_reass_remove_oldest_datagram_fp_t                    ip_reass_remove_oldest_datagram_adpt;
extern ip_reass_dequeue_datagram_fp_t                          ip_reass_dequeue_datagram_adpt;
extern ip_reass_chain_frag_into_datagram_and_validate_fp_t     ip_reass_chain_frag_into_datagram_and_validate_adpt;
extern ip_reass_enqueue_new_datagram_fp_t                      ip_reass_enqueue_new_datagram_adpt;

#if IP_FRAG
#if !LWIP_NETIF_TX_SINGLE_PBUF
extern ip_frag_free_pbuf_custom_ref_fp_t                       ip_frag_free_pbuf_custom_ref_adpt;
extern ipfrag_free_pbuf_custom_fp_t                            ipfrag_free_pbuf_custom_adpt;
extern ip_frag_alloc_pbuf_custom_ref_fp_t                      ip_frag_alloc_pbuf_custom_ref_adpt;
#endif /* !LWIP_NETIF_TX_SINGLE_PBUF */
#endif /* IP_FRAG */


/* Public API fucntion used in ip4_frag.c*/
#if IP_REASSEMBLY
void ip_reass_init(void);
void ip_reass_tmr(void);
struct pbuf * ip4_reass(struct pbuf *p);
#endif /* IP_REASSEMBLY */

#if IP_FRAG
err_t ip4_frag(struct pbuf *p, struct netif *netif, const ip4_addr_t *dest);
#endif /* IP_FRAG */


#if IP_REASSEMBLY
typedef void (*ip_reass_init_fp_t)(void);
typedef void (*ip_reass_tmr_fp_t)(void);
typedef struct pbuf * (*ip4_reass_fp_t)(struct pbuf *p);
#endif /* IP_REASSEMBLY */

#if IP_FRAG
typedef err_t (*ip4_frag_fp_t)(struct pbuf *p, struct netif *netif, const ip4_addr_t *dest);
#endif /* IP_FRAG */

#if IP_REASSEMBLY
//extern ip_reass_init_fp_t      ip_reass_init_adpt;
extern ip_reass_tmr_fp_t       ip_reass_tmr_adpt;
extern ip4_reass_fp_t          ip4_reass_adpt;
#endif

#if IP_FRAG
extern ip4_frag_fp_t           ip4_frag_adpt;
#endif

#ifdef __cplusplus
}
#endif

#endif /* LWIP_IPV4 */

#endif /* #if defined(LWIP_ROMBUILD) */

#endif /* LWIP_HDR_IP4_FRAG_IF_H */
