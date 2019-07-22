/**
 * @file
 *
 * IPv6 fragmentation and reassembly.
 */

/*
 * Copyright (c) 2010 Inico Technologies Ltd.
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
 * Author: Ivan Delamer <delamer@inicotech.com>
 *
 *
 * Please coordinate changes and requests with Ivan Delamer
 * <delamer@inicotech.com>
 */
#ifndef LWIP_HDR_IP6_FRAG_IF_H
#define LWIP_HDR_IP6_FRAG_IF_H

#include "lwip/opt.h"

#if defined(LWIP_ROMBUILD)

#include "lwip/pbuf.h"
#include "lwip/ip6_addr.h"
#include "lwip/ip6.h"
#include "lwip/netif.h"

#ifdef __cplusplus
extern "C" {
#endif

struct ip6_reassdata;
struct pbuf_custom_ref;

/* Private static API fucntion used in ip6_frag.c*/
#if LWIP_IPV6 && LWIP_IPV6_REASS
/* Forward declarations. */
void ip6_reass_free_complete_datagram(struct ip6_reassdata *ipr);
void ip6_reass_remove_oldest_datagram(struct ip6_reassdata *ipr, int pbufs_needed);
#endif /* LWIP_IPV6 && LWIP_IPV6_REASS */

#if LWIP_IPV6 && LWIP_IPV6_FRAG

#if !LWIP_NETIF_TX_SINGLE_PBUF
struct pbuf_custom_ref* ip6_frag_alloc_pbuf_custom_ref(void);
void ip6_frag_free_pbuf_custom_ref(struct pbuf_custom_ref* p);
void ip6_frag_free_pbuf_custom(struct pbuf *p);
#endif
#endif


#if LWIP_IPV6 && LWIP_IPV6_REASS
/* Forward declarations. */
typedef void (*ip6_reass_free_complete_datagram_fp_t)(struct ip6_reassdata *ipr);
extern ip6_reass_free_complete_datagram_fp_t ip6_reass_free_complete_datagram_adpt;
typedef void (*ip6_reass_remove_oldest_datagram_fp_t)(struct ip6_reassdata *ipr, int pbufs_needed);
extern ip6_reass_remove_oldest_datagram_fp_t ip6_reass_remove_oldest_datagram_adpt;
#endif /* LWIP_IPV6 && LWIP_IPV6_REASS */

#if LWIP_IPV6 && LWIP_IPV6_FRAG

#if !LWIP_NETIF_TX_SINGLE_PBUF
typedef struct pbuf_custom_ref* (*ip6_frag_alloc_pbuf_custom_ref_fp_t)(void);
typedef void (*ip6_frag_free_pbuf_custom_ref_fp_t)(struct pbuf_custom_ref* p);
typedef void (*ip6_frag_free_pbuf_custom_fp_t)(struct pbuf *p);

extern ip6_frag_alloc_pbuf_custom_ref_fp_t  ip6_frag_alloc_pbuf_custom_ref_adpt;
extern ip6_frag_free_pbuf_custom_ref_fp_t   ip6_frag_free_pbuf_custom_ref_adpt;
extern ip6_frag_free_pbuf_custom_fp_t       ip6_frag_free_pbuf_custom_adpt;
#endif
#endif



#if LWIP_IPV6 && LWIP_IPV6_REASS  /* don't build if not configured for use in lwipopts.h */
typedef void (*ip6_reass_tmr_fp_t)(void);
typedef struct pbuf *(*ip6_reass_fp_t)(struct pbuf *p);

extern ip6_reass_tmr_fp_t       ip6_reass_tmr_adpt;
extern  ip6_reass_fp_t          ip6_reass_adpt;
#endif /* LWIP_IPV6 && LWIP_IPV6_REASS */

#if LWIP_IPV6 && LWIP_IPV6_FRAG  /* don't build if not configured for use in lwipopts.h */
typedef err_t (*ip6_frag_fp_t)(struct pbuf *p, struct netif *netif, const ip6_addr_t *dest);
extern  ip6_frag_fp_t           ip6_frag_adpt;
#endif /* LWIP_IPV6 && LWIP_IPV6_FRAG */


#ifdef __cplusplus
}
#endif

#endif /* #if defined(LWIP_ROMBUILD) */

#endif /* LWIP_HDR_IP6_FRAG_IF_H */
