/**
 * @file
 * IPv4 API
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
#ifndef LWIP_HDR_IP4_IF_H
#define LWIP_HDR_IP4_IF_H

#include "lwip/opt.h"

#if LWIP_IPV4

#include "lwip/def.h"
#include "lwip/pbuf.h"
#include "lwip/ip4_addr.h"
#include "lwip/err.h"
#include "lwip/netif.h"
#include "lwip/prot/ip4.h"

#if defined(LWIP_ROMBUILD)
typedef struct netif *(*ip4_route_fp_t)(const ip4_addr_t *dest);
typedef err_t (*ip4_input_fp_t)(struct pbuf *p, struct netif *inp);
typedef err_t (*ip4_output_fp_t)(struct pbuf *p, const ip4_addr_t *src, const ip4_addr_t *dest, u8_t ttl, u8_t tos, u8_t proto);
typedef err_t (*ip4_output_if_fp_t)(struct pbuf *p, const ip4_addr_t *src, const ip4_addr_t *dest, u8_t ttl, u8_t tos, u8_t proto, struct netif *netif);
typedef err_t (*ip4_output_if_src_fp_t)(struct pbuf *p, const ip4_addr_t *src, const ip4_addr_t *dest, u8_t ttl, u8_t tos, u8_t proto, struct netif *netif);

#if LWIP_NETIF_HWADDRHINT
typedef err_t (*ip4_output_hinted_fp_t)(struct pbuf *p, const ip4_addr_t *src, const ip4_addr_t *dest, u8_t ttl, u8_t tos, u8_t proto, u8_t *addr_hint);
#endif /* LWIP_NETIF_HWADDRHINT */
#if IP_OPTIONS_SEND
typedef err_t (*ip4_output_if_opt_fp_t)(struct pbuf *p, const ip4_addr_t *src, const ip4_addr_t *dest,
       u8_t ttl, u8_t tos, u8_t proto, struct netif *netif, void *ip_options,
       u16_t optlen);
typedef err_t (*ip4_output_if_opt_src_fp_t)(struct pbuf *p, const ip4_addr_t *src, const ip4_addr_t *dest,
       u8_t ttl, u8_t tos, u8_t proto, struct netif *netif, void *ip_options,
       u16_t optlen);
#endif /* IP_OPTIONS_SEND */

#if LWIP_MULTICAST_TX_OPTIONS
typedef void (*ip4_set_default_multicast_netif_fp_t)(struct netif* default_multicast_netif);
#endif

#if IP_DEBUG
typedef void (*ip4_debug_print_fp_t)(struct pbuf *p);
#endif /* IP_DEBUG */


extern ip4_route_fp_t ip4_route_adpt;
extern ip4_input_fp_t ip4_input_adpt;
extern ip4_output_fp_t ip4_output_adpt;
extern ip4_output_if_fp_t ip4_output_if_adpt;
extern ip4_output_if_src_fp_t ip4_output_if_src_adpt;
#if LWIP_NETIF_HWADDRHINT
extern ip4_output_hinted_fp_t ip4_output_hinted_adpt;
#endif       
#if IP_OPTIONS_SEND
extern ip4_output_if_opt_fp_t ip4_output_if_opt_adpt;
extern ip4_output_if_opt_src_fp_t ip4_output_if_opt_src_adpt;
#endif
#if LWIP_MULTICAST_TX_OPTIONS
extern ip4_set_default_multicast_netif_fp_t ip4_set_default_multicast_netif_adpt;
#endif

#if IP_DEBUG
extern ip4_debug_print_fp_t ip4_debug_print_adpt;
#endif

#endif //#if defined(OPLROMBUILD)

#ifdef __cplusplus
}
#endif

#endif /* LWIP_IPV4 */

#endif /* LWIP_HDR_IP4_IF_H */


