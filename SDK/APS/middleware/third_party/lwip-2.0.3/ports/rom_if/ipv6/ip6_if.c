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

#include "lwip/opt.h"

#if LWIP_IPV6  /* don't build if not configured for use in lwipopts.h */

#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/netif.h"
#include "lwip/ip.h"
#include "lwip/ip6.h"
#include "lwip/ip6_addr.h"
#include "lwip/ip6_frag.h"
#include "lwip/icmp6.h"
#include "lwip/raw.h"
#include "lwip/udp.h"
#include "lwip/priv/tcp_priv.h"
#include "lwip/dhcp6.h"
#include "lwip/nd6.h"
#include "lwip/mld6.h"
#include "lwip/debug.h"
#include "lwip/stats.h"

#ifdef LWIP_HOOK_FILENAME
#include LWIP_HOOK_FILENAME
#endif

#if LWIP_IPV6_FORWARD
RET_DATA ip6_forward_fp_t                  ip6_forward_adpt_t;
#endif
RET_DATA ip6_route_fp_t                    ip6_route_adpt;
RET_DATA ip6_select_source_address_fp_t    ip6_select_source_address_adpt;
RET_DATA ip6_input_fp_t                    ip6_input_adpt;
RET_DATA ip6_output_fp_t                   ip6_output_adpt;
RET_DATA ip6_output_if_fp_t                ip6_output_if_adpt;
RET_DATA ip6_output_if_src_fp_t            ip6_output_if_src_adpt;
#if LWIP_NETIF_HWADDRHINT
RET_DATA ip6_output_hinted_fp_t            ip6_output_hinted_adpt;
#endif /* LWIP_NETIF_HWADDRHINT */
#if LWIP_IPV6_MLD
RET_DATA ip6_options_add_hbh_ra_fp_t       ip6_options_add_hbh_ra_adpt;
#endif /* LWIP_IPV6_MLD */
#if IP6_DEBUG
RET_DATA ip6_debug_print_fp_t              ip6_debug_print_adpt;
#endif


void lwip_load_interface_ip6(void)
{
#if LWIP_IPV6_FORWARD
    ip6_forward_adpt_t                  = LWIP_ROMFN(ip6_forward);
#endif
    ip6_route_adpt                      = LWIP_ROMFN(ip6_route);
    ip6_select_source_address_adpt      = LWIP_ROMFN(ip6_select_source_address);
    ip6_input_adpt                      = LWIP_ROMFN(ip6_input);
    ip6_output_adpt                     = LWIP_ROMFN(ip6_output);
    ip6_output_if_adpt                  = LWIP_ROMFN(ip6_output_if);
    ip6_output_if_src_adpt              = LWIP_ROMFN(ip6_output_if_src);
#if LWIP_NETIF_HWADDRHINT
    ip6_output_hinted_adpt              = LWIP_ROMFN(ip6_output_hinted);
#endif /* LWIP_NETIF_HWADDRHINT */
#if LWIP_IPV6_MLD
    ip6_options_add_hbh_ra_adpt         = LWIP_ROMFN(ip6_options_add_hbh_ra);
#endif /* LWIP_IPV6_MLD */
#if IP6_DEBUG
    ip6_debug_print_adpt                = LWIP_ROMFN(ip6_debug_print);
#endif
}



/**
 * Finds the appropriate network interface for a given IPv6 address. It tries to select
 * a netif following a sequence of heuristics:
 * 1) if there is only 1 netif, return it
 * 2) if the destination is a link-local address, try to match the src address to a netif.
 *    this is a tricky case because with multiple netifs, link-local addresses only have
 *    meaning within a particular subnet/link.
 * 3) tries to match the destination subnet to a configured address
 * 4) tries to find a router
 * 5) tries to match the source address to the netif
 * 6) returns the default netif, if configured
 *
 * @param src the source IPv6 address, if known
 * @param dest the destination IPv6 address for which to find the route
 * @return the netif on which to send to reach dest
 */
struct netif *
ip6_route(const ip6_addr_t *src, const ip6_addr_t *dest)
{
    return ip6_route_adpt(src, dest);
}

/**
 * @ingroup ip6
 * Select the best IPv6 source address for a given destination
 * IPv6 address. Loosely follows RFC 3484. "Strong host" behavior
 * is assumed.
 *
 * @param netif the netif on which to send a packet
 * @param dest the destination we are trying to reach
 * @return the most suitable source address to use, or NULL if no suitable
 *         source address is found
 */
const ip_addr_t *
ip6_select_source_address(struct netif *netif, const ip6_addr_t *dest)
{
    return ip6_select_source_address_adpt(netif, dest);
}

#if LWIP_IPV6_FORWARD
/**
 * Forwards an IPv6 packet. It finds an appropriate route for the
 * packet, decrements the HL value of the packet, and outputs
 * the packet on the appropriate interface.
 *
 * @param p the packet to forward (p->payload points to IP header)
 * @param iphdr the IPv6 header of the input packet
 * @param inp the netif on which this packet was received
 */
void
ip6_forward(struct pbuf *p, struct ip6_hdr *iphdr, struct netif *inp)
{
    ip6_forward_adpt(p, iphdr, inp);
}
#endif /* LWIP_IPV6_FORWARD */

/**
 * This function is called by the network interface device driver when
 * an IPv6 packet is received. The function does the basic checks of the
 * IP header such as packet size being at least larger than the header
 * size etc. If the packet was not destined for us, the packet is
 * forwarded (using ip6_forward).
 *
 * Finally, the packet is sent to the upper layer protocol input function.
 *
 * @param p the received IPv6 packet (p->payload points to IPv6 header)
 * @param inp the netif on which this packet was received
 * @return ERR_OK if the packet was processed (could return ERR_* if it wasn't
 *         processed, but currently always returns ERR_OK)
 */
err_t
ip6_input(struct pbuf *p, struct netif *inp)
{
    return ip6_input_adpt(p, inp);
}


/**
 * Sends an IPv6 packet on a network interface. This function constructs
 * the IPv6 header. If the source IPv6 address is NULL, the IPv6 "ANY" address is
 * used as source (usually during network startup). If the source IPv6 address it
 * IP6_ADDR_ANY, the most appropriate IPv6 address of the outgoing network
 * interface is filled in as source address. If the destination IPv6 address is
 * LWIP_IP_HDRINCL, p is assumed to already include an IPv6 header and
 * p->payload points to it instead of the data.
 *
 * @param p the packet to send (p->payload points to the data, e.g. next
            protocol header; if dest == LWIP_IP_HDRINCL, p already includes an
            IPv6 header and p->payload points to that IPv6 header)
 * @param src the source IPv6 address to send from (if src == IP6_ADDR_ANY, an
 *         IP address of the netif is selected and used as source address.
 *         if src == NULL, IP6_ADDR_ANY is used as source)
 * @param dest the destination IPv6 address to send the packet to
 * @param hl the Hop Limit value to be set in the IPv6 header
 * @param tc the Traffic Class value to be set in the IPv6 header
 * @param nexth the Next Header to be set in the IPv6 header
 * @param netif the netif on which to send this packet
 * @return ERR_OK if the packet was sent OK
 *         ERR_BUF if p doesn't have enough space for IPv6/LINK headers
 *         returns errors returned by netif->output
 */
err_t
ip6_output_if(struct pbuf *p, const ip6_addr_t *src, const ip6_addr_t *dest,
             u8_t hl, u8_t tc,
             u8_t nexth, struct netif *netif)
{
    return ip6_output_if_adpt(p, src, dest, hl, tc, nexth, netif);
}

/**
 * Same as ip6_output_if() but 'src' address is not replaced by netif address
 * when it is 'any'.
 */
err_t
ip6_output_if_src(struct pbuf *p, const ip6_addr_t *src, const ip6_addr_t *dest,
             u8_t hl, u8_t tc,
             u8_t nexth, struct netif *netif)
{
    return ip6_output_if_src_adpt(p, src, dest, hl, tc, nexth, netif);
}

/**
 * Simple interface to ip6_output_if. It finds the outgoing network
 * interface and calls upon ip6_output_if to do the actual work.
 *
 * @param p the packet to send (p->payload points to the data, e.g. next
            protocol header; if dest == LWIP_IP_HDRINCL, p already includes an
            IPv6 header and p->payload points to that IPv6 header)
 * @param src the source IPv6 address to send from (if src == IP6_ADDR_ANY, an
 *         IP address of the netif is selected and used as source address.
 *         if src == NULL, IP6_ADDR_ANY is used as source)
 * @param dest the destination IPv6 address to send the packet to
 * @param hl the Hop Limit value to be set in the IPv6 header
 * @param tc the Traffic Class value to be set in the IPv6 header
 * @param nexth the Next Header to be set in the IPv6 header
 *
 * @return ERR_RTE if no route is found
 *         see ip_output_if() for more return values
 */
err_t
ip6_output(struct pbuf *p, const ip6_addr_t *src, const ip6_addr_t *dest,
          u8_t hl, u8_t tc, u8_t nexth)
{
    return ip6_output_adpt(p, src ,dest, hl ,tc, nexth);
}


#if LWIP_NETIF_HWADDRHINT
/** Like ip6_output, but takes and addr_hint pointer that is passed on to netif->addr_hint
 *  before calling ip6_output_if.
 *
 * @param p the packet to send (p->payload points to the data, e.g. next
            protocol header; if dest == LWIP_IP_HDRINCL, p already includes an
            IPv6 header and p->payload points to that IPv6 header)
 * @param src the source IPv6 address to send from (if src == IP6_ADDR_ANY, an
 *         IP address of the netif is selected and used as source address.
 *         if src == NULL, IP6_ADDR_ANY is used as source)
 * @param dest the destination IPv6 address to send the packet to
 * @param hl the Hop Limit value to be set in the IPv6 header
 * @param tc the Traffic Class value to be set in the IPv6 header
 * @param nexth the Next Header to be set in the IPv6 header
 * @param addr_hint address hint pointer set to netif->addr_hint before
 *        calling ip_output_if()
 *
 * @return ERR_RTE if no route is found
 *         see ip_output_if() for more return values
 */
err_t
ip6_output_hinted(struct pbuf *p, const ip6_addr_t *src, const ip6_addr_t *dest,
          u8_t hl, u8_t tc, u8_t nexth, u8_t *addr_hint)
{
    return ip6_output_hinted_adpt(p, src , dest, hl, tc, nexth, addr_hint);
}
#endif /* LWIP_NETIF_HWADDRHINT*/

#if LWIP_IPV6_MLD
/**
 * Add a hop-by-hop options header with a router alert option and padding.
 *
 * Used by MLD when sending a Multicast listener report/done message.
 *
 * @param p the packet to which we will prepend the options header
 * @param nexth the next header protocol number (e.g. IP6_NEXTH_ICMP6)
 * @param value the value of the router alert option data (e.g. IP6_ROUTER_ALERT_VALUE_MLD)
 * @return ERR_OK if hop-by-hop header was added, ERR_* otherwise
 */
err_t
ip6_options_add_hbh_ra(struct pbuf *p, u8_t nexth, u8_t value)
{
    return ip6_options_add_hbh_ra_adpt(p, nexth, value);
}
#endif /* LWIP_IPV6_MLD */

#if IP6_DEBUG
/* Print an IPv6 header by using LWIP_DEBUGF
 * @param p an IPv6 packet, p->payload pointing to the IPv6 header
 */
void
ip6_debug_print(struct pbuf *p)
{
    ip6_debug_print_adpt(p);
}
#endif /* IP6_DEBUG */

#endif /* LWIP_IPV6 */
