/**
 * @file
 *
 * Neighbor discovery and stateless address autoconfiguration for IPv6.
 * Aims to be compliant with RFC 4861 (Neighbor discovery) and RFC 4862
 * (Address autoconfiguration).
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

#ifndef LWIP_HDR_ND6_IF_H
#define LWIP_HDR_ND6_IF_H

#include "lwip/opt.h"

#if defined(LWIP_ROMBUILD)

#if LWIP_IPV6  /* don't build if not configured for use in lwipopts.h */

#include "lwip/ip6_addr.h"
#include "lwip/err.h"

#ifdef __cplusplus
extern "C" {
#endif

struct pbuf;
struct netif;

struct nd6_q_entry;
struct nd6_neighbor_cache_entry;

/* Forward declarations. */
/* Private static API fucntion used in nd6.c*/
s8_t nd6_find_neighbor_cache_entry(const ip6_addr_t *ip6addr);
s8_t nd6_new_neighbor_cache_entry(void);
void nd6_free_neighbor_cache_entry(s8_t i);
s8_t nd6_find_destination_cache_entry(const ip6_addr_t *ip6addr);
s8_t nd6_new_destination_cache_entry(void);
s8_t nd6_is_prefix_in_netif(const ip6_addr_t *ip6addr, struct netif *netif);
s8_t nd6_select_router(const ip6_addr_t *ip6addr, struct netif *netif);
s8_t nd6_get_router(const ip6_addr_t *router_addr, struct netif *netif);
s8_t nd6_new_router(const ip6_addr_t *router_addr, struct netif *netif);
s8_t nd6_get_onlink_prefix(ip6_addr_t *prefix, struct netif *netif);
s8_t nd6_new_onlink_prefix(ip6_addr_t *prefix, struct netif *netif);
s8_t nd6_get_next_hop_entry(const ip6_addr_t *ip6addr, struct netif *netif);
err_t nd6_queue_packet(s8_t neighbor_index, struct pbuf *q);

void nd6_send_ns(struct netif *netif, const ip6_addr_t *target_addr, u8_t flags);
void nd6_send_na(struct netif *netif, const ip6_addr_t *target_addr, u8_t flags);
void nd6_send_neighbor_cache_probe(struct nd6_neighbor_cache_entry *entry, u8_t flags);
#if LWIP_IPV6_SEND_ROUTER_SOLICIT
err_t nd6_send_rs(struct netif *netif);
#endif /* LWIP_IPV6_SEND_ROUTER_SOLICIT */

#if LWIP_ND6_QUEUEING
void nd6_free_q(struct nd6_q_entry *q);
#endif /* LWIP_ND6_QUEUEING */
void nd6_send_q(s8_t i);

typedef s8_t (*nd6_find_neighbor_cache_entry_fp_t)(const ip6_addr_t *ip6addr);
typedef s8_t (*nd6_new_neighbor_cache_entry_fp_t)(void);
typedef void (*nd6_free_neighbor_cache_entry_fp_t)(s8_t i);
typedef s8_t (*nd6_find_destination_cache_entry_fp_t)(const ip6_addr_t *ip6addr);
typedef s8_t (*nd6_new_destination_cache_entry_fp_t)(void);
typedef s8_t (*nd6_is_prefix_in_netif_fp_t)(const ip6_addr_t *ip6addr, struct netif *netif);
typedef s8_t (*nd6_select_router_fp_t)(const ip6_addr_t *ip6addr, struct netif *netif);
typedef s8_t (*nd6_get_router_fp_t)(const ip6_addr_t *router_addr, struct netif *netif);
typedef s8_t (*nd6_new_router_fp_t)(const ip6_addr_t *router_addr, struct netif *netif);
typedef s8_t (*nd6_get_onlink_prefix_fp_t)(ip6_addr_t *prefix, struct netif *netif);
typedef s8_t (*nd6_new_onlink_prefix_fp_t)(ip6_addr_t *prefix, struct netif *netif);
typedef s8_t (*nd6_get_next_hop_entry_fp_t)(const ip6_addr_t *ip6addr, struct netif *netif);
typedef err_t (*nd6_queue_packet_fp_t)(s8_t neighbor_index, struct pbuf *q);

typedef void (*nd6_send_ns_fp_t)(struct netif *netif, const ip6_addr_t *target_addr, u8_t flags);
typedef void (*nd6_send_na_fp_t)(struct netif *netif, const ip6_addr_t *target_addr, u8_t flags);
typedef void (*nd6_send_neighbor_cache_probe_fp_t)(struct nd6_neighbor_cache_entry *entry, u8_t flags);
#if LWIP_IPV6_SEND_ROUTER_SOLICIT
typedef err_t (*nd6_send_rs_fp_t)(struct netif *netif);
#endif /* LWIP_IPV6_SEND_ROUTER_SOLICIT */

#if LWIP_ND6_QUEUEING
typedef void (*nd6_free_q_fp_t)(struct nd6_q_entry *q);
#endif /* LWIP_ND6_QUEUEING */
typedef void (*nd6_send_q_fp_t)(s8_t i);

extern nd6_find_neighbor_cache_entry_fp_t          nd6_find_neighbor_cache_entry_adpt;
extern nd6_new_neighbor_cache_entry_fp_t           nd6_new_neighbor_cache_entry_adpt;
extern nd6_free_neighbor_cache_entry_fp_t          nd6_free_neighbor_cache_entry_adpt;
extern nd6_find_destination_cache_entry_fp_t       nd6_find_destination_cache_entry_adpt;
extern nd6_new_destination_cache_entry_fp_t        nd6_new_destination_cache_entry_adpt;
extern nd6_is_prefix_in_netif_fp_t                 nd6_is_prefix_in_netif_adpt;
extern nd6_select_router_fp_t                      nd6_select_router_adpt;
extern nd6_get_router_fp_t                         nd6_get_router_adpt;
extern nd6_new_router_fp_t                         nd6_new_router_adpt;
extern nd6_get_onlink_prefix_fp_t                  nd6_get_onlink_prefix_adpt;
extern nd6_new_onlink_prefix_fp_t                  nd6_new_onlink_prefix_adpt;
extern nd6_get_next_hop_entry_fp_t                 nd6_get_next_hop_entry_adpt;
extern nd6_queue_packet_fp_t                       nd6_queue_packet_adpt;

extern nd6_send_ns_fp_t                            nd6_send_ns_adpt;
extern nd6_send_na_fp_t                            nd6_send_na_adpt;
extern nd6_send_neighbor_cache_probe_fp_t          nd6_send_neighbor_cache_probe_adpt;
#if LWIP_IPV6_SEND_ROUTER_SOLICIT
extern nd6_send_rs_fp_t                            nd6_send_rs_adpt;
#endif /* LWIP_IPV6_SEND_ROUTER_SOLICIT */

#if LWIP_ND6_QUEUEING
extern nd6_free_q_fp_t                             nd6_free_q_adpt;
#endif /* LWIP_ND6_QUEUEING */
extern nd6_send_q_fp_t                             nd6_send_q_adpt;


/* Public API fucntion used in nd6.c*/
typedef void (*nd6_tmr_fp_t)(void);
typedef void (*nd6_input_fp_t)(struct pbuf *p, struct netif *inp);
typedef void (*nd6_clear_destination_cache_fp_t)(void);
typedef struct netif *(*nd6_find_route_fp_t)(const ip6_addr_t *ip6addr);
typedef err_t (*nd6_get_next_hop_addr_or_queue_fp_t)(struct netif *netif, struct pbuf *q, const ip6_addr_t *ip6addr, const u8_t **hwaddrp);
typedef u16_t (*nd6_get_destination_mtu_fp_t)(const ip6_addr_t *ip6addr, struct netif *netif);
#if LWIP_ND6_TCP_REACHABILITY_HINTS
typedef void  (*nd6_reachability_hint_fp_t)(const ip6_addr_t *ip6addr);
#endif /* LWIP_ND6_TCP_REACHABILITY_HINTS */
typedef void  (*nd6_cleanup_netif_fp_t)(struct netif *netif);
#if LWIP_IPV6_MLD
typedef void  (*nd6_adjust_mld_membership_fp_t)(struct netif *netif, s8_t addr_idx, u8_t new_state);
#endif /* LWIP_IPV6_MLD */

extern nd6_tmr_fp_t                            nd6_tmr_adpt;
extern nd6_input_fp_t                          nd6_input_adpt;
extern nd6_clear_destination_cache_fp_t        nd6_clear_destination_cache_adpt;
extern nd6_find_route_fp_t                     nd6_find_route_adpt;
extern nd6_get_next_hop_addr_or_queue_fp_t     nd6_get_next_hop_addr_or_queue_adpt;
extern nd6_get_destination_mtu_fp_t            nd6_get_destination_mtu_adpt;
#if LWIP_ND6_TCP_REACHABILITY_HINTS
extern nd6_reachability_hint_fp_t              nd6_reachability_hint_adpt;
#endif
extern nd6_cleanup_netif_fp_t                  nd6_cleanup_netif_adpt;
#if LWIP_IPV6_MLD
extern nd6_adjust_mld_membership_fp_t          nd6_adjust_mld_membership_adpt;
#endif


#ifdef __cplusplus
}
#endif

#endif /* LWIP_IPV6 */

#endif /* #if defined(LWIP_ROMBUILD) */

#endif /* LWIP_HDR_ND6_IF_H */
