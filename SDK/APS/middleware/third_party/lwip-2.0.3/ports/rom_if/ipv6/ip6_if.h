/**
 * @file
 *
 * IPv6 layer.
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
#ifndef LWIP_HDR_IP6_IF_H
#define LWIP_HDR_IP6_IF_H

#include "lwip/opt.h"

#if defined(LWIP_ROMBUILD)

#if LWIP_IPV6  /* don't build if not configured for use in lwipopts.h */

#include "lwip/ip6_addr.h"
#include "lwip/prot/ip6.h"
#include "lwip/def.h"
#include "lwip/pbuf.h"
#include "lwip/netif.h"

#include "lwip/err.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Private static API fucntion used in ip6.c*/
void ip6_forward(struct pbuf *p, struct ip6_hdr *iphdr, struct netif *inp);

typedef void (*ip6_forward_fp_t)(struct pbuf *p, struct ip6_hdr *iphdr, struct netif *inp);



typedef struct netif *(*ip6_route_fp_t)(const ip6_addr_t *src, const ip6_addr_t *dest);
typedef const ip_addr_t *(*ip6_select_source_address_fp_t)(struct netif *netif, const ip6_addr_t * dest);
typedef err_t (*ip6_input_fp_t)(struct pbuf *p, struct netif *inp);
typedef err_t (*ip6_output_fp_t)(struct pbuf *p, const ip6_addr_t *src, const ip6_addr_t *dest, u8_t hl, u8_t tc, u8_t nexth);
typedef err_t (*ip6_output_if_fp_t)(struct pbuf *p, const ip6_addr_t *src, const ip6_addr_t *dest, u8_t hl, u8_t tc, u8_t nexth, struct netif *netif);
typedef err_t (*ip6_output_if_src_fp_t)(struct pbuf *p, const ip6_addr_t *src, const ip6_addr_t *dest, u8_t hl, u8_t tc, u8_t nexth, struct netif *netif);
#if LWIP_NETIF_HWADDRHINT
typedef err_t (*ip6_output_hinted_fp_t)(struct pbuf *p, const ip6_addr_t *src, const ip6_addr_t *dest, u8_t hl, u8_t tc, u8_t nexth, u8_t *addr_hint);
#endif /* LWIP_NETIF_HWADDRHINT */
#if LWIP_IPV6_MLD
typedef err_t (*ip6_options_add_hbh_ra_fp_t)(struct pbuf * p, u8_t nexth, u8_t value);
#endif /* LWIP_IPV6_MLD */

#if IP6_DEBUG
typedef void (*ip6_debug_print_fp_t)(struct pbuf *p);
#endif

extern ip6_forward_fp_t                     ip6_forward_adpt_t;
extern ip6_route_fp_t                       ip6_route_adpt;
extern ip6_select_source_address_fp_t       ip6_select_source_address_adpt;
extern ip6_input_fp_t                       ip6_input_adpt;
extern ip6_output_fp_t                      ip6_output_adpt;
extern ip6_output_if_fp_t                   ip6_output_if_adpt;
extern ip6_output_if_src_fp_t               ip6_output_if_src_adpt;
#if LWIP_NETIF_HWADDRHINT
extern ip6_output_hinted_fp_t               ip6_output_hinted_adpt;
#endif /* LWIP_NETIF_HWADDRHINT */
#if LWIP_IPV6_MLD
extern ip6_options_add_hbh_ra_fp_t          ip6_options_add_hbh_ra_adpt;
#endif /* LWIP_IPV6_MLD */

#if IP6_DEBUG
extern ip6_debug_print_fp_t                 ip6_debug_print_adpt;
#endif

#ifdef __cplusplus
}
#endif

#endif /* LWIP_IPV6 */

#endif /*#if defined(LWIP_ROMBUILD) */
#endif /* LWIP_HDR_IP6_IF_H */
