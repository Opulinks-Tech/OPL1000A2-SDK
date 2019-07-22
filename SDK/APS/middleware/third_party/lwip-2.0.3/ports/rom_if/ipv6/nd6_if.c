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

#include "lwip/opt.h"

#if LWIP_IPV6  /* don't build if not configured for use in lwipopts.h */

#include "lwip/nd6.h"
#include "lwip/priv/nd6_priv.h"
#include "lwip/prot/nd6.h"
#include "lwip/prot/icmp6.h"
#include "lwip/pbuf.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/ip6.h"
#include "lwip/ip6_addr.h"
#include "lwip/inet_chksum.h"
#include "lwip/netif.h"
#include "lwip/icmp6.h"
#include "lwip/mld6.h"
#include "lwip/ip.h"
#include "lwip/stats.h"
#include "lwip/dns.h"

#include <string.h>

#ifdef LWIP_HOOK_FILENAME
#include LWIP_HOOK_FILENAME
#endif

#if LWIP_IPV6_DUP_DETECT_ATTEMPTS > IP6_ADDR_TENTATIVE_COUNT_MASK
#error LWIP_IPV6_DUP_DETECT_ATTEMPTS > IP6_ADDR_TENTATIVE_COUNT_MASK
#endif

RET_DATA nd6_find_neighbor_cache_entry_fp_t       nd6_find_neighbor_cache_entry_adpt;
RET_DATA nd6_new_neighbor_cache_entry_fp_t        nd6_new_neighbor_cache_entry_adpt;
RET_DATA nd6_free_neighbor_cache_entry_fp_t       nd6_free_neighbor_cache_entry_adpt;
RET_DATA nd6_find_destination_cache_entry_fp_t    nd6_find_destination_cache_entry_adpt;
RET_DATA nd6_new_destination_cache_entry_fp_t     nd6_new_destination_cache_entry_adpt;
RET_DATA nd6_is_prefix_in_netif_fp_t              nd6_is_prefix_in_netif_adpt;
RET_DATA nd6_select_router_fp_t                   nd6_select_router_adpt;
RET_DATA nd6_get_router_fp_t                      nd6_get_router_adpt;
RET_DATA nd6_new_router_fp_t                      nd6_new_router_adpt;
RET_DATA nd6_get_onlink_prefix_fp_t               nd6_get_onlink_prefix_adpt;
RET_DATA nd6_new_onlink_prefix_fp_t               nd6_new_onlink_prefix_adpt;
RET_DATA nd6_get_next_hop_entry_fp_t              nd6_get_next_hop_entry_adpt;
RET_DATA nd6_queue_packet_fp_t                    nd6_queue_packet_adpt;

RET_DATA nd6_send_ns_fp_t                         nd6_send_ns_adpt;
RET_DATA nd6_send_na_fp_t                         nd6_send_na_adpt;
RET_DATA nd6_send_neighbor_cache_probe_fp_t       nd6_send_neighbor_cache_probe_adpt;
#if LWIP_IPV6_SEND_ROUTER_SOLICIT
RET_DATA nd6_send_rs_fp_t                         nd6_send_rs_adpt;
#endif /* LWIP_IPV6_SEND_ROUTER_SOLICIT */

#if LWIP_ND6_QUEUEING
RET_DATA nd6_free_q_fp_t                          nd6_free_q_adpt;
#endif /* LWIP_ND6_QUEUEING */
RET_DATA nd6_send_q_fp_t                          nd6_send_q_adpt;


RET_DATA nd6_tmr_fp_t                            nd6_tmr_adpt;
RET_DATA nd6_input_fp_t                          nd6_input_adpt;
RET_DATA nd6_clear_destination_cache_fp_t        nd6_clear_destination_cache_adpt;
RET_DATA nd6_find_route_fp_t                     nd6_find_route_adpt;
RET_DATA nd6_get_next_hop_addr_or_queue_fp_t     nd6_get_next_hop_addr_or_queue_adpt;
RET_DATA nd6_get_destination_mtu_fp_t            nd6_get_destination_mtu_adpt;
#if LWIP_ND6_TCP_REACHABILITY_HINTS
RET_DATA nd6_reachability_hint_fp_t              nd6_reachability_hint_adpt;
#endif
RET_DATA nd6_cleanup_netif_fp_t                  nd6_cleanup_netif_adpt;
#if LWIP_IPV6_MLD
RET_DATA nd6_adjust_mld_membership_fp_t          nd6_adjust_mld_membership_adpt;
#endif

void lwip_load_interface_nd6(void)
{
#if LWIP_IPV6
    /* Router tables. */
    memset(neighbor_cache, 0, sizeof(neighbor_cache));
    memset(destination_cache, 0, sizeof(destination_cache));
    memset(prefix_list, 0, sizeof(prefix_list));
    memset(default_router_list, 0, sizeof(default_router_list));

    /* Default values, can be updated by a RA message. */
    reachable_time = LWIP_ND6_REACHABLE_TIME;
    retrans_timer = LWIP_ND6_RETRANS_TIMER;

    /* Index for cache entries. */
    nd6_cached_neighbor_index = 0;
    nd6_cached_destination_index = 0;

    /* Multicast address holder. */
    ip6_addr_set_zero(&multicast_address);

    /* Static buffer to parse RA packet options (size of a prefix option, biggest option) */
    memset(&nd6_ra_buffer, 0, sizeof(nd6_ra_buffer));
    last_router = 0;
#endif

    nd6_find_neighbor_cache_entry_adpt           = LWIP_ROMFN(nd6_find_neighbor_cache_entry);
    nd6_new_neighbor_cache_entry_adpt            = LWIP_ROMFN(nd6_new_neighbor_cache_entry);
    nd6_free_neighbor_cache_entry_adpt           = LWIP_ROMFN(nd6_free_neighbor_cache_entry);
    nd6_find_destination_cache_entry_adpt        = LWIP_ROMFN(nd6_find_destination_cache_entry);
    nd6_new_destination_cache_entry_adpt         = LWIP_ROMFN(nd6_new_destination_cache_entry);
    nd6_is_prefix_in_netif_adpt                  = LWIP_ROMFN(nd6_is_prefix_in_netif);
    nd6_select_router_adpt                       = LWIP_ROMFN(nd6_select_router);
    nd6_get_router_adpt                          = LWIP_ROMFN(nd6_get_router);
    nd6_new_router_adpt                          = LWIP_ROMFN(nd6_new_router);
    nd6_get_onlink_prefix_adpt                   = LWIP_ROMFN(nd6_get_onlink_prefix);
    nd6_new_onlink_prefix_adpt                   = LWIP_ROMFN(nd6_new_onlink_prefix);
    nd6_get_next_hop_entry_adpt                  = LWIP_ROMFN(nd6_get_next_hop_entry);
    nd6_queue_packet_adpt                        = LWIP_ROMFN(nd6_queue_packet);

    nd6_send_ns_adpt                             = LWIP_ROMFN(nd6_send_ns);
    nd6_send_na_adpt                             = LWIP_ROMFN(nd6_send_na);
    nd6_send_neighbor_cache_probe_adpt           = LWIP_ROMFN(nd6_send_neighbor_cache_probe);
#if LWIP_IPV6_SEND_ROUTER_SOLICIT
    nd6_send_rs_adpt                             = LWIP_ROMFN(nd6_send_rs);
#endif /* LWIP_IPV6_SEND_ROUTER_SOLICIT */

#if LWIP_ND6_QUEUEING
    nd6_free_q_adpt                              = LWIP_ROMFN(nd6_free_q);
#endif /* LWIP_ND6_QUEUEING */
    nd6_send_q_adpt                              = LWIP_ROMFN(nd6_send_q);

    nd6_tmr_adpt                                 = LWIP_ROMFN(nd6_tmr);
    nd6_input_adpt                               = LWIP_ROMFN(nd6_input);
    nd6_clear_destination_cache_adpt             = LWIP_ROMFN(nd6_clear_destination_cache);
    nd6_find_route_adpt                          = LWIP_ROMFN(nd6_find_route);
    nd6_get_next_hop_addr_or_queue_adpt          = LWIP_ROMFN(nd6_get_next_hop_addr_or_queue);
    nd6_get_destination_mtu_adpt                 = LWIP_ROMFN(nd6_get_destination_mtu);
#if LWIP_ND6_TCP_REACHABILITY_HINTS
    nd6_reachability_hint_adpt                   = LWIP_ROMFN(nd6_reachability_hint);
#endif
    nd6_cleanup_netif_adpt                       = LWIP_ROMFN(nd6_cleanup_netif);
#if LWIP_IPV6_MLD
    nd6_adjust_mld_membership_adpt               = LWIP_ROMFN(nd6_adjust_mld_membership);
#endif
}

/**
 * Process an incoming neighbor discovery message
 *
 * @param p the nd packet, p->payload pointing to the icmpv6 header
 * @param inp the netif on which this packet was received
 */
void
nd6_input(struct pbuf *p, struct netif *inp)
{
    nd6_input_adpt(p, inp);
}


/**
 * Periodic timer for Neighbor discovery functions:
 *
 * - Update neighbor reachability states
 * - Update destination cache entries age
 * - Update invalidation timers of default routers and on-link prefixes
 * - Perform duplicate address detection (DAD) for our addresses
 * - Send router solicitations
 */
void
nd6_tmr(void)
{
    nd6_tmr_adpt();
}

/** Send a neighbor solicitation message for a specific neighbor cache entry
 *
 * @param entry the neightbor cache entry for wich to send the message
 * @param flags one of ND6_SEND_FLAG_*
 */
static void
nd6_send_neighbor_cache_probe(struct nd6_neighbor_cache_entry *entry, u8_t flags)
{
    nd6_send_neighbor_cache_probe_adpt(entry, flags);
}

/**
 * Send a neighbor solicitation message
 *
 * @param netif the netif on which to send the message
 * @param target_addr the IPv6 target address for the ND message
 * @param flags one of ND6_SEND_FLAG_*
 */
void
nd6_send_ns(struct netif *netif, const ip6_addr_t *target_addr, u8_t flags)
{
    nd6_send_ns_adpt(netif, target_addr, flags);
}

/**
 * Send a neighbor advertisement message
 *
 * @param netif the netif on which to send the message
 * @param target_addr the IPv6 target address for the ND message
 * @param flags one of ND6_SEND_FLAG_*
 */
void
nd6_send_na(struct netif *netif, const ip6_addr_t *target_addr, u8_t flags)
{
    nd6_send_na_adpt(netif, target_addr, flags);
}

#if LWIP_IPV6_SEND_ROUTER_SOLICIT
/**
 * Send a router solicitation message
 *
 * @param netif the netif on which to send the message
 */
err_t
nd6_send_rs(struct netif *netif)
{
    return nd6_send_rs_adpt(netif);
}
#endif /* LWIP_IPV6_SEND_ROUTER_SOLICIT */

/**
 * Search for a neighbor cache entry
 *
 * @param ip6addr the IPv6 address of the neighbor
 * @return The neighbor cache entry index that matched, -1 if no
 * entry is found
 */
s8_t
nd6_find_neighbor_cache_entry(const ip6_addr_t *ip6addr)
{
    return nd6_find_neighbor_cache_entry_adpt(ip6addr);
}

/**
 * Create a new neighbor cache entry.
 *
 * If no unused entry is found, will try to recycle an old entry
 * according to ad-hoc "age" heuristic.
 *
 * @return The neighbor cache entry index that was created, -1 if no
 * entry could be created
 */
s8_t
nd6_new_neighbor_cache_entry(void)
{
    return nd6_new_neighbor_cache_entry_adpt();
}

/**
 * Will free any resources associated with a neighbor cache
 * entry, and will mark it as unused.
 *
 * @param i the neighbor cache entry index to free
 */
void
nd6_free_neighbor_cache_entry(s8_t i)
{
    nd6_free_neighbor_cache_entry_adpt(i);
}

/**
 * Search for a destination cache entry
 *
 * @param ip6addr the IPv6 address of the destination
 * @return The destination cache entry index that matched, -1 if no
 * entry is found
 */
s8_t
nd6_find_destination_cache_entry(const ip6_addr_t *ip6addr)
{
    return nd6_find_destination_cache_entry_adpt(ip6addr);
}

/**
 * Create a new destination cache entry. If no unused entry is found,
 * will recycle oldest entry.
 *
 * @return The destination cache entry index that was created, -1 if no
 * entry was created
 */
static s8_t
nd6_new_destination_cache_entry(void)
{
    return nd6_new_destination_cache_entry_adpt();
}

/**
 * Clear the destination cache.
 *
 * This operation may be necessary for consistency in the light of changing
 * local addresses and/or use of the gateway hook.
 */
void
nd6_clear_destination_cache(void)
{
    nd6_clear_destination_cache_adpt();
}

/**
 * Determine whether an address matches an on-link prefix.
 *
 * @param ip6addr the IPv6 address to match
 * @return 1 if the address is on-link, 0 otherwise
 */
s8_t
nd6_is_prefix_in_netif(const ip6_addr_t *ip6addr, struct netif *netif)
{
    return nd6_is_prefix_in_netif_adpt(ip6addr, netif);
}

/**
 * Select a default router for a destination.
 *
 * @param ip6addr the destination address
 * @param netif the netif for the outgoing packet, if known
 * @return the default router entry index, or -1 if no suitable
 *         router is found
 */
s8_t
nd6_select_router(const ip6_addr_t *ip6addr, struct netif *netif)
{
    return nd6_select_router_adpt(ip6addr, netif);
}

/**
 * Find a router-announced route to the given destination.
 *
 * The caller is responsible for checking whether the returned netif, if any,
 * is in a suitable state (up, link up) to be used for packet transmission.
 *
 * @param ip6addr the destination IPv6 address
 * @return the netif to use for the destination, or NULL if none found
 */
struct netif *
nd6_find_route(const ip6_addr_t *ip6addr)
{
    return nd6_find_route_adpt(ip6addr);
}

/**
 * Find an entry for a default router.
 *
 * @param router_addr the IPv6 address of the router
 * @param netif the netif on which the router is found, if known
 * @return the index of the router entry, or -1 if not found
 */
s8_t
nd6_get_router(const ip6_addr_t *router_addr, struct netif *netif)
{
    return nd6_get_router_adpt(router_addr, netif);
}

/**
 * Create a new entry for a default router.
 *
 * @param router_addr the IPv6 address of the router
 * @param netif the netif on which the router is connected, if known
 * @return the index on the router table, or -1 if could not be created
 */
s8_t
nd6_new_router(const ip6_addr_t *router_addr, struct netif *netif)
{
    return nd6_new_router_adpt(router_addr, netif);
}

/**
 * Find the cached entry for an on-link prefix.
 *
 * @param prefix the IPv6 prefix that is on-link
 * @param netif the netif on which the prefix is on-link
 * @return the index on the prefix table, or -1 if not found
 */
s8_t
nd6_get_onlink_prefix(ip6_addr_t *prefix, struct netif *netif)
{
    return nd6_get_onlink_prefix_adpt(prefix, netif);
}

/**
 * Creates a new entry for an on-link prefix.
 *
 * @param prefix the IPv6 prefix that is on-link
 * @param netif the netif on which the prefix is on-link
 * @return the index on the prefix table, or -1 if not created
 */
s8_t
nd6_new_onlink_prefix(ip6_addr_t *prefix, struct netif *netif)
{
    return nd6_new_onlink_prefix_adpt(prefix, netif);
}

/**
 * Determine the next hop for a destination. Will determine if the
 * destination is on-link, else a suitable on-link router is selected.
 *
 * The last entry index is cached for fast entry search.
 *
 * @param ip6addr the destination address
 * @param netif the netif on which the packet will be sent
 * @return the neighbor cache entry for the next hop, ERR_RTE if no
 *         suitable next hop was found, ERR_MEM if no cache entry
 *         could be created
 */
s8_t
nd6_get_next_hop_entry(const ip6_addr_t *ip6addr, struct netif *netif)
{
    return nd6_get_next_hop_entry_adpt(ip6addr, netif);
}

/**
 * Queue a packet for a neighbor.
 *
 * @param neighbor_index the index in the neighbor cache table
 * @param q packet to be queued
 * @return ERR_OK if succeeded, ERR_MEM if out of memory
 */
err_t
nd6_queue_packet(s8_t neighbor_index, struct pbuf *q)
{
    return nd6_queue_packet_adpt(neighbor_index, q);
}

#if LWIP_ND6_QUEUEING
/**
 * Free a complete queue of nd6 q entries
 *
 * @param q a queue of nd6_q_entry to free
 */
void
nd6_free_q(struct nd6_q_entry *q)
{
    nd6_free_q_adpt(q);
}
#endif /* LWIP_ND6_QUEUEING */

/**
 * Send queued packets for a neighbor
 *
 * @param i the neighbor to send packets to
 */
void
nd6_send_q(s8_t i)
{
    nd6_send_q_adpt(i);
}

/**
 * A packet is to be transmitted to a specific IPv6 destination on a specific
 * interface. Check if we can find the hardware address of the next hop to use
 * for the packet. If so, give the hardware address to the caller, which should
 * use it to send the packet right away. Otherwise, enqueue the packet for
 * later transmission while looking up the hardware address, if possible.
 *
 * As such, this function returns one of three different possible results:
 *
 * - ERR_OK with a non-NULL 'hwaddrp': the caller should send the packet now.
 * - ERR_OK with a NULL 'hwaddrp': the packet has been enqueued for later.
 * - not ERR_OK: something went wrong; forward the error upward in the stack.
 *
 * @param netif The lwIP network interface on which the IP packet will be sent.
 * @param q The pbuf(s) containing the IP packet to be sent.
 * @param ip6addr The destination IPv6 address of the packet.
 * @param hwaddrp On success, filled with a pointer to a HW address or NULL (meaning
 *        the packet has been queued).
 * @return
 * - ERR_OK on success, ERR_RTE if no route was found for the packet,
 * or ERR_MEM if low memory conditions prohibit sending the packet at all.
 */
err_t
nd6_get_next_hop_addr_or_queue(struct netif *netif, struct pbuf *q, const ip6_addr_t *ip6addr, const u8_t **hwaddrp)
{
    return nd6_get_next_hop_addr_or_queue_adpt(netif, q, ip6addr, hwaddrp);
}


/**
 * Get the Path MTU for a destination.
 *
 * @param ip6addr the destination address
 * @param netif the netif on which the packet will be sent
 * @return the Path MTU, if known, or the netif default MTU
 */
u16_t
nd6_get_destination_mtu(const ip6_addr_t *ip6addr, struct netif *netif)
{
    return nd6_get_destination_mtu_adpt(ip6addr, netif);
}


#if LWIP_ND6_TCP_REACHABILITY_HINTS
/**
 * Provide the Neighbor discovery process with a hint that a
 * destination is reachable. Called by tcp_receive when ACKs are
 * received or sent (as per RFC). This is useful to avoid sending
 * NS messages every 30 seconds.
 *
 * @param ip6addr the destination address which is know to be reachable
 *                by an upper layer protocol (TCP)
 */
void
nd6_reachability_hint(const ip6_addr_t *ip6addr)
{
    nd6_reachability_hint_adpt(ip6addr);
}
#endif /* LWIP_ND6_TCP_REACHABILITY_HINTS */

/**
 * Remove all prefix, neighbor_cache and router entries of the specified netif.
 *
 * @param netif points to a network interface
 */
void
nd6_cleanup_netif(struct netif *netif)
{
    nd6_cleanup_netif_adpt(netif);
}

#if LWIP_IPV6_MLD
/**
 * The state of a local IPv6 address entry is about to change. If needed, join
 * or leave the solicited-node multicast group for the address.
 *
 * @param netif The netif that owns the address.
 * @param addr_idx The index of the address.
 * @param new_state The new (IP6_ADDR_) state for the address.
 */
void
nd6_adjust_mld_membership(struct netif *netif, s8_t addr_idx, u8_t new_state)
{
    nd6_adjust_mld_membership_adpt(netif, addr_idx, new_state);
}
#endif /* LWIP_IPV6_MLD */

#endif /* LWIP_IPV6 */
