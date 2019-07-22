/**
 * @file
 * Address Resolution Protocol module for IP over Ethernet
 *
 * Functionally, ARP is divided into two parts. The first maps an IP address
 * to a physical address when sending a packet, and the second part answers
 * requests from other machines for our physical address.
 *
 * This implementation complies with RFC 826 (Ethernet ARP). It supports
 * Gratuitious ARP from RFC3220 (IP Mobility Support for IPv4) section 4.6
 * if an interface calls etharp_gratuitous(our_netif) upon address change.
 */

/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * Copyright (c) 2003-2004 Leon Woestenberg <leon.woestenberg@axon.tv>
 * Copyright (c) 2003-2004 Axon Digital Design B.V., The Netherlands.
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
 */

#include "lwip/opt.h"

#if defined(LWIP_ROMBUILD)
#include "ipv4/etharp_if.h"


#if LWIP_ARP || LWIP_ETHERNET

#include "lwip/etharp.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "lwip/dhcp.h"
#include "lwip/autoip.h"
#include "netif/ethernet.h"

#include <string.h>

#if LWIP_IPV4 && LWIP_ARP /* don't build if not configured for use in lwipopts.h */

#if ARP_QUEUEING
RET_DATA free_etharp_q_fp_t                 free_etharp_q_adpt;
#endif /* ARP_QUEUEING */
RET_DATA etharp_free_entry_fp_t             etharp_free_entry_adpt;
RET_DATA etharp_find_entry_fp_t             etharp_find_entry_adpt;
RET_DATA etharp_update_arp_entry_fp_t       etharp_update_arp_entry_adpt;
RET_DATA etharp_output_to_arp_index_fp_t    etharp_output_to_arp_index_adpt;
RET_DATA etharp_raw_fp_t                    etharp_raw_adpt;
RET_DATA etharp_request_dst_fp_t            etharp_request_dst_adpt;

RET_DATA etharp_tmr_fp_t                    etharp_tmr_adpt;
RET_DATA etharp_find_addr_fp_t              etharp_find_addr_adpt;
RET_DATA etharp_get_entry_fp_t              etharp_get_entry_adpt;
RET_DATA etharp_output_fp_t                 etharp_output_adpt;
RET_DATA etharp_query_fp_t                  etharp_query_adpt;
RET_DATA etharp_request_fp_t                etharp_request_adpt;
RET_DATA etharp_cleanup_netif_fp_t          etharp_cleanup_netif_adpt;

#if ETHARP_SUPPORT_STATIC_ENTRIES
RET_DATA etharp_add_static_entry_fp_t       etharp_add_static_entry_adpt;
RET_DATA etharp_remove_static_entry_fp_t    etharp_remove_static_entry_adpt;
#endif /* ETHARP_SUPPORT_STATIC_ENTRIES */

RET_DATA etharp_input_fp_t                  etharp_input_adpt;



void lwip_load_interface_etharp(void)
{
    memset(&arp_table, 0, sizeof(arp_table));
#if !LWIP_NETIF_HWADDRHINT
    etharp_cached_entry = 0;
#endif /* !LWIP_NETIF_HWADDRHINT */


#if ARP_QUEUEING
    free_etharp_q_adpt      = LWIP_ROMFN(free_etharp_q);
#endif
    etharp_free_entry_adpt  = LWIP_ROMFN(etharp_free_entry);
    etharp_find_entry_adpt  = LWIP_ROMFN(etharp_find_entry);
    etharp_update_arp_entry_adpt = LWIP_ROMFN(etharp_update_arp_entry);
    etharp_output_to_arp_index_adpt = LWIP_ROMFN(etharp_output_to_arp_index);
    etharp_raw_adpt         = LWIP_ROMFN(etharp_raw);
    etharp_request_dst_adpt = LWIP_ROMFN(etharp_request_dst);

    etharp_tmr_adpt         = LWIP_ROMFN(etharp_tmr);
    etharp_find_addr_adpt   = LWIP_ROMFN(etharp_find_addr);
    etharp_get_entry_adpt   = LWIP_ROMFN(etharp_get_entry);
    etharp_output_adpt      = LWIP_ROMFN(etharp_output);
    etharp_query_adpt       = LWIP_ROMFN(etharp_query);
    etharp_request_adpt       = LWIP_ROMFN(etharp_request);
    etharp_cleanup_netif_adpt = LWIP_ROMFN(etharp_cleanup_netif);

#if ETHARP_SUPPORT_STATIC_ENTRIES
    etharp_add_static_entry_adpt    = LWIP_ROMFN(etharp_add_static_entry);
    etharp_remove_static_entry_adpt = LWIP_ROMFN(etharp_remove_static_entry);
#endif /* ETHARP_SUPPORT_STATIC_ENTRIES */

    etharp_input_adpt = LWIP_ROMFN(etharp_input);

    // auto-arp and one-shot-arp to prevent continuous HTTP post failure
    g_lwip_one_shot_arp_enable = 0;
    g_lwip_auto_arp_enable = 0;
    g_lwip_auto_arp_timeout = LWIP_AUTO_ARP_TIMEOUT;

    lwip_auto_arp_enable     = LWIP_ROMFN(lwip_auto_arp_enable);
    lwip_one_shot_arp_enable = LWIP_ROMFN(lwip_one_shot_arp_enable);
}


#if ARP_QUEUEING
/**
 * Free a complete queue of etharp entries
 *
 * @param q a qeueue of etharp_q_entry's to free
 */
static void
free_etharp_q(struct etharp_q_entry *q)
{
    free_etharp_q_adpt(q);
}
#else /* ARP_QUEUEING */

/** Compatibility define: free the queued pbuf */
#define free_etharp_q(q) pbuf_free(q)

#endif /* ARP_QUEUEING */

/** Clean up ARP table entries */
static void
etharp_free_entry(int i)
{
    etharp_free_entry_adpt(i);
}

/**
 * Clears expired entries in the ARP table.
 *
 * This function should be called every ARP_TMR_INTERVAL milliseconds (1 second),
 * in order to expire entries in the ARP table.
 */
void
etharp_tmr(void)
{
    etharp_tmr_adpt();
}

/**
 * Search the ARP table for a matching or new entry.
 *
 * If an IP address is given, return a pending or stable ARP entry that matches
 * the address. If no match is found, create a new entry with this address set,
 * but in state ETHARP_EMPTY. The caller must check and possibly change the
 * state of the returned entry.
 *
 * If ipaddr is NULL, return a initialized new entry in state ETHARP_EMPTY.
 *
 * In all cases, attempt to create new entries from an empty entry. If no
 * empty entries are available and ETHARP_FLAG_TRY_HARD flag is set, recycle
 * old entries. Heuristic choose the least important entry for recycling.
 *
 * @param ipaddr IP address to find in ARP cache, or to add if not found.
 * @param flags See @ref etharp_state
 * @param netif netif related to this address (used for NETIF_HWADDRHINT)
 *
 * @return The ARP entry index that matched or is created, ERR_MEM if no
 * entry is found or could be recycled.
 */
static s8_t
etharp_find_entry(const ip4_addr_t *ipaddr, u8_t flags, struct netif* netif)
{
    return etharp_find_entry_adpt(ipaddr, flags, netif);
}

/**
 * Update (or insert) a IP/MAC address pair in the ARP cache.
 *
 * If a pending entry is resolved, any queued packets will be sent
 * at this point.
 *
 * @param netif netif related to this entry (used for NETIF_ADDRHINT)
 * @param ipaddr IP address of the inserted ARP entry.
 * @param ethaddr Ethernet address of the inserted ARP entry.
 * @param flags See @ref etharp_state
 *
 * @return
 * - ERR_OK Successfully updated ARP cache.
 * - ERR_MEM If we could not add a new ARP entry when ETHARP_FLAG_TRY_HARD was set.
 * - ERR_ARG Non-unicast address given, those will not appear in ARP cache.
 *
 * @see pbuf_free()
 */
static err_t
etharp_update_arp_entry(struct netif *netif, const ip4_addr_t *ipaddr, struct eth_addr *ethaddr, u8_t flags)
{
    return etharp_update_arp_entry_adpt(netif, ipaddr, ethaddr, flags);
}

#if ETHARP_SUPPORT_STATIC_ENTRIES
/** Add a new static entry to the ARP table. If an entry exists for the
 * specified IP address, this entry is overwritten.
 * If packets are queued for the specified IP address, they are sent out.
 *
 * @param ipaddr IP address for the new static entry
 * @param ethaddr ethernet address for the new static entry
 * @return See return values of etharp_add_static_entry
 */
err_t
etharp_add_static_entry(const ip4_addr_t *ipaddr, struct eth_addr *ethaddr)
{
    return etharp_add_static_entry_adpt(ipaddr, ethaddr);
}

/** Remove a static entry from the ARP table previously added with a call to
 * etharp_add_static_entry.
 *
 * @param ipaddr IP address of the static entry to remove
 * @return ERR_OK: entry removed
 *         ERR_MEM: entry wasn't found
 *         ERR_ARG: entry wasn't a static entry but a dynamic one
 */
err_t
etharp_remove_static_entry(const ip4_addr_t *ipaddr)
{
    return etharp_remove_static_entry_adpt(ipaddr);
}
#endif /* ETHARP_SUPPORT_STATIC_ENTRIES */

/**
 * Remove all ARP table entries of the specified netif.
 *
 * @param netif points to a network interface
 */
void
etharp_cleanup_netif(struct netif *netif)
{
    etharp_cleanup_netif_adpt(netif);
}

/**
 * Finds (stable) ethernet/IP address pair from ARP table
 * using interface and IP address index.
 * @note the addresses in the ARP table are in network order!
 *
 * @param netif points to interface index
 * @param ipaddr points to the (network order) IP address index
 * @param eth_ret points to return pointer
 * @param ip_ret points to return pointer
 * @return table index if found, -1 otherwise
 */
s8_t
etharp_find_addr(struct netif *netif, const ip4_addr_t *ipaddr,
         struct eth_addr **eth_ret, const ip4_addr_t **ip_ret)
{
    return etharp_find_addr_adpt(netif, ipaddr, eth_ret, ip_ret);
}

/**
 * Possibility to iterate over stable ARP table entries
 *
 * @param i entry number, 0 to ARP_TABLE_SIZE
 * @param ipaddr return value: IP address
 * @param netif return value: points to interface
 * @param eth_ret return value: ETH address
 * @return 1 on valid index, 0 otherwise
 */
u8_t
etharp_get_entry(u8_t i, ip4_addr_t **ipaddr, struct netif **netif, struct eth_addr **eth_ret)
{
    return etharp_get_entry_adpt(i, ipaddr, netif, eth_ret);
}

/**
 * Responds to ARP requests to us. Upon ARP replies to us, add entry to cache
 * send out queued IP packets. Updates cache with snooped address pairs.
 *
 * Should be called for incoming ARP packets. The pbuf in the argument
 * is freed by this function.
 *
 * @param p The ARP packet that arrived on netif. Is freed by this function.
 * @param netif The lwIP network interface on which the ARP packet pbuf arrived.
 *
 * @see pbuf_free()
 */
void
etharp_input(struct pbuf *p, struct netif *netif)
{
    etharp_input_adpt(p, netif);
}

/** Just a small helper function that sends a pbuf to an ethernet address
 * in the arp_table specified by the index 'arp_idx'.
 */
static err_t
etharp_output_to_arp_index(struct netif *netif, struct pbuf *q, u8_t arp_idx)
{
    return etharp_output_to_arp_index_adpt(netif, q, arp_idx);
}

/**
 * Resolve and fill-in Ethernet address header for outgoing IP packet.
 *
 * For IP multicast and broadcast, corresponding Ethernet addresses
 * are selected and the packet is transmitted on the link.
 *
 * For unicast addresses, the packet is submitted to etharp_query(). In
 * case the IP address is outside the local network, the IP address of
 * the gateway is used.
 *
 * @param netif The lwIP network interface which the IP packet will be sent on.
 * @param q The pbuf(s) containing the IP packet to be sent.
 * @param ipaddr The IP address of the packet destination.
 *
 * @return
 * - ERR_RTE No route to destination (no gateway to external networks),
 * or the return type of either etharp_query() or ethernet_output().
 */
err_t
etharp_output(struct netif *netif, struct pbuf *q, const ip4_addr_t *ipaddr)
{
    return etharp_output_adpt(netif, q, ipaddr);
}

/**
 * Send an ARP request for the given IP address and/or queue a packet.
 *
 * If the IP address was not yet in the cache, a pending ARP cache entry
 * is added and an ARP request is sent for the given address. The packet
 * is queued on this entry.
 *
 * If the IP address was already pending in the cache, a new ARP request
 * is sent for the given address. The packet is queued on this entry.
 *
 * If the IP address was already stable in the cache, and a packet is
 * given, it is directly sent and no ARP request is sent out.
 *
 * If the IP address was already stable in the cache, and no packet is
 * given, an ARP request is sent out.
 *
 * @param netif The lwIP network interface on which ipaddr
 * must be queried for.
 * @param ipaddr The IP address to be resolved.
 * @param q If non-NULL, a pbuf that must be delivered to the IP address.
 * q is not freed by this function.
 *
 * @note q must only be ONE packet, not a packet queue!
 *
 * @return
 * - ERR_BUF Could not make room for Ethernet header.
 * - ERR_MEM Hardware address unknown, and no more ARP entries available
 *   to query for address or queue the packet.
 * - ERR_MEM Could not queue packet due to memory shortage.
 * - ERR_RTE No route to destination (no gateway to external networks).
 * - ERR_ARG Non-unicast address given, those will not appear in ARP cache.
 *
 */
err_t
etharp_query(struct netif *netif, const ip4_addr_t *ipaddr, struct pbuf *q)
{
    return etharp_query_adpt(netif, ipaddr, q);
}

/**
 * Send a raw ARP packet (opcode and all addresses can be modified)
 *
 * @param netif the lwip network interface on which to send the ARP packet
 * @param ethsrc_addr the source MAC address for the ethernet header
 * @param ethdst_addr the destination MAC address for the ethernet header
 * @param hwsrc_addr the source MAC address for the ARP protocol header
 * @param ipsrc_addr the source IP address for the ARP protocol header
 * @param hwdst_addr the destination MAC address for the ARP protocol header
 * @param ipdst_addr the destination IP address for the ARP protocol header
 * @param opcode the type of the ARP packet
 * @return ERR_OK if the ARP packet has been sent
 *         ERR_MEM if the ARP packet couldn't be allocated
 *         any other err_t on failure
 */
static err_t
etharp_raw(struct netif *netif, const struct eth_addr *ethsrc_addr,
           const struct eth_addr *ethdst_addr,
           const struct eth_addr *hwsrc_addr, const ip4_addr_t *ipsrc_addr,
           const struct eth_addr *hwdst_addr, const ip4_addr_t *ipdst_addr,
           const u16_t opcode)
{
    return etharp_raw_adpt(netif, ethsrc_addr, ethdst_addr,
                        hwsrc_addr, ipsrc_addr,
                        hwdst_addr, ipdst_addr, opcode);
}

/**
 * Send an ARP request packet asking for ipaddr to a specific eth address.
 * Used to send unicast request to refresh the ARP table just before an entry
 * times out
 *
 * @param netif the lwip network interface on which to send the request
 * @param ipaddr the IP address for which to ask
 * @param hw_dst_addr the ethernet address to send this packet to
 * @return ERR_OK if the request has been sent
 *         ERR_MEM if the ARP packet couldn't be allocated
 *         any other err_t on failure
 */
static err_t
etharp_request_dst(struct netif *netif, const ip4_addr_t *ipaddr, const struct eth_addr* hw_dst_addr)
{
    return etharp_request_dst_adpt(netif, ipaddr, hw_dst_addr);
}

/**
 * Send an ARP request packet asking for ipaddr.
 *
 * @param netif the lwip network interface on which to send the request
 * @param ipaddr the IP address for which to ask
 * @return ERR_OK if the request has been sent
 *         ERR_MEM if the ARP packet couldn't be allocated
 *         any other err_t on failure
 */
err_t
etharp_request(struct netif *netif, const ip4_addr_t *ipaddr)
{
    return etharp_request_adpt(netif, ipaddr);
}
#endif /* LWIP_IPV4 && LWIP_ARP */

#endif /* LWIP_ARP || LWIP_ETHERNET */

#endif //#if defined(LWIP_ROMBUILD)


