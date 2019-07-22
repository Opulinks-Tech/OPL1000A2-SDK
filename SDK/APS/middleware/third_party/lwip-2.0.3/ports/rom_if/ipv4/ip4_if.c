/**
 * @file
 * This is the IPv4 layer implementation for incoming and outgoing IP traffic.
 *
 * @see ip_frag.c
 *
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
#include "ip4_if.h"

#if LWIP_IPV4


#if defined(LWIP_ROMBUILD)
RET_DATA ip4_route_fp_t             ip4_route_adpt;
RET_DATA ip4_input_fp_t             ip4_input_adpt;
RET_DATA ip4_output_fp_t            ip4_output_adpt;
RET_DATA ip4_output_if_fp_t         ip4_output_if_adpt;
RET_DATA ip4_output_if_src_fp_t     ip4_output_if_src_adpt;
#if LWIP_NETIF_HWADDRHINT
RET_DATA ip4_output_hinted_fp_t     ip4_output_hinted_adpt;
#endif
#if IP_OPTIONS_SEND
RET_DATA ip4_output_if_opt_fp_t     ip4_output_if_opt_adpt;
RET_DATA ip4_output_if_opt_src_fp_t ip4_output_if_opt_src_adpt;
#endif
#if LWIP_MULTICAST_TX_OPTIONS
RET_DATA ip4_set_default_multicast_netif_fp_t ip4_set_default_multicast_netif_adpt;
#endif

#if IP_DEBUG
RET_DATA ip4_debug_print_fp_t       ip4_debug_print_adpt;
#endif

void lwip_load_interface_ip4(void)
{
    /* Cold boot initialization for "zero_init" retention data */
    ip_id = 0;
#if LWIP_MULTICAST_TX_OPTIONS
    ip4_default_multicast_netif = NULL;
#endif

    ip4_route_adpt          = LWIP_ROMFN(ip4_route);
    ip4_input_adpt          = LWIP_ROMFN(ip4_input);
    ip4_output_adpt         = LWIP_ROMFN(ip4_output);
    ip4_output_if_adpt      = LWIP_ROMFN(ip4_output_if);
    ip4_output_if_src_adpt  = LWIP_ROMFN(ip4_output_if_src);
#if LWIP_NETIF_HWADDRHINT
    ip4_output_hinted_adpt  = LWIP_ROMFN(ip4_output_hinted);
#endif
#if IP_OPTIONS_SEND
    ip4_output_if_opt_adpt      = LWIP_ROMFN(ip4_output_if_opt);
    ip4_output_if_opt_src_adpt  = LWIP_ROMFN(ip4_output_if_opt_src);
#endif
#if LWIP_MULTICAST_TX_OPTIONS
    ip4_set_default_multicast_netif_adpt = LWIP_ROMFN(ip4_set_default_multicast_netif);
#endif

#if IP_DEBUG
    ip4_debug_print_adpt    = LWIP_ROMFN(ip4_debug_print);
#endif
}


#if LWIP_MULTICAST_TX_OPTIONS
/**
 * @ingroup ip4
 * Set a default netif for IPv4 multicast. */
void
ip4_set_default_multicast_netif(struct netif* default_multicast_netif)
{
    ip4_set_default_multicast_netif_adpt(default_multicast_netif);
}
#endif /* LWIP_MULTICAST_TX_OPTIONS */

#ifdef LWIP_HOOK_IP4_ROUTE_SRC
/**
 * Source based IPv4 routing must be fully implemented in
 * LWIP_HOOK_IP4_ROUTE_SRC(). This function only provides he parameters.
 */
struct netif *
ip4_route_src(const ip4_addr_t *dest, const ip4_addr_t *src)
{
    return ip4_route_src_adpt(dest, src);
}
#endif /* LWIP_HOOK_IP4_ROUTE_SRC */

/**
 * Finds the appropriate network interface for a given IP address. It
 * searches the list of network interfaces linearly. A match is found
 * if the masked IP address of the network interface equals the masked
 * IP address given to the function.
 *
 * @param dest the destination IP address for which to find the route
 * @return the netif on which to send to reach dest
 */
struct netif *
ip4_route(const ip4_addr_t *dest)
{
    return ip4_route_adpt(dest);
}

#if IP_FORWARD
/**
 * Determine whether an IP address is in a reserved set of addresses
 * that may not be forwarded, or whether datagrams to that destination
 * may be forwarded.
 * @param p the packet to forward
 * @return 1: can forward 0: discard
 */
static int
ip4_canforward(struct pbuf *p)
{
    return ip4_canforward_adpt(p);
}

/**
 * Forwards an IP packet. It finds an appropriate route for the
 * packet, decrements the TTL value of the packet, adjusts the
 * checksum and outputs the packet on the appropriate interface.
 *
 * @param p the packet to forward (p->payload points to IP header)
 * @param iphdr the IP header of the input packet
 * @param inp the netif on which this packet was received
 */
static void
ip4_forward(struct pbuf *p, struct ip_hdr *iphdr, struct netif *inp)
{
    ip4_forward_adpt(p, iphdr, inp);
}
#endif /* IP_FORWARD */

/**
 * This function is called by the network interface device driver when
 * an IP packet is received. The function does the basic checks of the
 * IP header such as packet size being at least larger than the header
 * size etc. If the packet was not destined for us, the packet is
 * forwarded (using ip_forward). The IP checksum is always checked.
 *
 * Finally, the packet is sent to the upper layer protocol input function.
 *
 * @param p the received IP packet (p->payload points to IP header)
 * @param inp the netif on which this packet was received
 * @return ERR_OK if the packet was processed (could return ERR_* if it wasn't
 *         processed, but currently always returns ERR_OK)
 */
err_t
ip4_input(struct pbuf *p, struct netif *inp)
{
    return ip4_input_adpt(p, inp);
}

/**
 * Sends an IP packet on a network interface. This function constructs
 * the IP header and calculates the IP header checksum. If the source
 * IP address is NULL, the IP address of the outgoing network
 * interface is filled in as source address.
 * If the destination IP address is LWIP_IP_HDRINCL, p is assumed to already
 * include an IP header and p->payload points to it instead of the data.
 *
 * @param p the packet to send (p->payload points to the data, e.g. next
            protocol header; if dest == LWIP_IP_HDRINCL, p already includes an
            IP header and p->payload points to that IP header)
 * @param src the source IP address to send from (if src == IP4_ADDR_ANY, the
 *         IP  address of the netif used to send is used as source address)
 * @param dest the destination IP address to send the packet to
 * @param ttl the TTL value to be set in the IP header
 * @param tos the TOS value to be set in the IP header
 * @param proto the PROTOCOL to be set in the IP header
 * @param netif the netif on which to send this packet
 * @return ERR_OK if the packet was sent OK
 *         ERR_BUF if p doesn't have enough space for IP/LINK headers
 *         returns errors returned by netif->output
 *
 * @note ip_id: RFC791 "some host may be able to simply use
 *  unique identifiers independent of destination"
 */
err_t
ip4_output_if(struct pbuf *p, const ip4_addr_t *src, const ip4_addr_t *dest,
             u8_t ttl, u8_t tos,
             u8_t proto, struct netif *netif)
{
  return ip4_output_if_adpt(p, src, dest, ttl, tos, proto, netif);
}

/**
 * Same as ip_output_if() but with the possibility to include IP options:
 *
 * @ param ip_options pointer to the IP options, copied into the IP header
 * @ param optlen length of ip_options
 */
#if IP_OPTIONS_SEND
err_t
ip4_output_if_opt(struct pbuf *p, const ip4_addr_t *src, const ip4_addr_t *dest,
       u8_t ttl, u8_t tos, u8_t proto, struct netif *netif, void *ip_options,
       u16_t optlen)
{
    return ip4_output_if_opt_adpt(p, src, dest, ttl, tos, proto, netif, ip_options, optlen);
}
#endif /* IP_OPTIONS_SEND */
/**
 * Same as ip_output_if() but 'src' address is not replaced by netif address
 * when it is 'any'.
 */
err_t
ip4_output_if_src(struct pbuf *p, const ip4_addr_t *src, const ip4_addr_t *dest,
             u8_t ttl, u8_t tos,
             u8_t proto, struct netif *netif)
{
  return ip4_output_if_src_adpt(p, src, dest, ttl, tos, proto, netif);
}

/**
 * Same as ip_output_if_opt() but 'src' address is not replaced by netif address
 * when it is 'any'.
 */
#if IP_OPTIONS_SEND
err_t
ip4_output_if_opt_src(struct pbuf *p, const ip4_addr_t *src, const ip4_addr_t *dest,
       u8_t ttl, u8_t tos, u8_t proto, struct netif *netif, void *ip_options,
       u16_t optlen)
{
    return ip4_output_if_opt_src_adpt(p, src, dest, ttl, tos, proto, netif, ip_options, optlen);
}
#endif /* IP_OPTIONS_SEND */
/**
 * Simple interface to ip_output_if. It finds the outgoing network
 * interface and calls upon ip_output_if to do the actual work.
 *
 * @param p the packet to send (p->payload points to the data, e.g. next
            protocol header; if dest == LWIP_IP_HDRINCL, p already includes an
            IP header and p->payload points to that IP header)
 * @param src the source IP address to send from (if src == IP4_ADDR_ANY, the
 *         IP  address of the netif used to send is used as source address)
 * @param dest the destination IP address to send the packet to
 * @param ttl the TTL value to be set in the IP header
 * @param tos the TOS value to be set in the IP header
 * @param proto the PROTOCOL to be set in the IP header
 *
 * @return ERR_RTE if no route is found
 *         see ip_output_if() for more return values
 */
err_t
ip4_output(struct pbuf *p, const ip4_addr_t *src, const ip4_addr_t *dest,
          u8_t ttl, u8_t tos, u8_t proto)
{
    return ip4_output_adpt(p, src, dest, ttl, tos, proto);
}

#if LWIP_NETIF_HWADDRHINT
/** Like ip_output, but takes and addr_hint pointer that is passed on to netif->addr_hint
 *  before calling ip_output_if.
 *
 * @param p the packet to send (p->payload points to the data, e.g. next
            protocol header; if dest == LWIP_IP_HDRINCL, p already includes an
            IP header and p->payload points to that IP header)
 * @param src the source IP address to send from (if src == IP4_ADDR_ANY, the
 *         IP  address of the netif used to send is used as source address)
 * @param dest the destination IP address to send the packet to
 * @param ttl the TTL value to be set in the IP header
 * @param tos the TOS value to be set in the IP header
 * @param proto the PROTOCOL to be set in the IP header
 * @param addr_hint address hint pointer set to netif->addr_hint before
 *        calling ip_output_if()
 *
 * @return ERR_RTE if no route is found
 *         see ip_output_if() for more return values
 */
err_t
ip4_output_hinted(struct pbuf *p, const ip4_addr_t *src, const ip4_addr_t *dest,
          u8_t ttl, u8_t tos, u8_t proto, u8_t *addr_hint)
{
    return ip4_output_hinted_adpt(p, src, dest, ttl, tos, proto, addr_hint);
}
#endif /* LWIP_NETIF_HWADDRHINT*/

#if IP_DEBUG
/* Print an IP header by using LWIP_DEBUGF
 * @param p an IP packet, p->payload pointing to the IP header
 */
void
ip4_debug_print(struct pbuf *p)
{
    ip4_debug_print_adpt(p);
}
#endif /* IP_DEBUG */


#endif //#defined(LWIP_ROMBUILD)


#endif /* LWIP_IPV4 */
