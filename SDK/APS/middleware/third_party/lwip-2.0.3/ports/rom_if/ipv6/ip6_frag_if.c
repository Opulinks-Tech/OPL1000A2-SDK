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

#include "lwip/opt.h"
#include "lwip/ip6_frag.h"
#include "lwip/ip6.h"
#include "lwip/icmp6.h"
#include "lwip/nd6.h"
#include "lwip/ip.h"

#include "lwip/pbuf.h"
#include "lwip/memp.h"
#include "lwip/stats.h"

#include <string.h>

#if LWIP_IPV6 && LWIP_IPV6_REASS

#ifndef IP_REASS_FREE_OLDEST
#define IP_REASS_FREE_OLDEST 1
#endif /* IP_REASS_FREE_OLDEST */

RET_DATA ip6_reass_free_complete_datagram_fp_t  ip6_reass_free_complete_datagram_adpt;
#if IP_REASS_FREE_OLDEST
RET_DATA ip6_reass_remove_oldest_datagram_fp_t  ip6_reass_remove_oldest_datagram_adpt;
#endif /* IP_REASS_FREE_OLDEST */
#endif /* LWIP_IPV6 && LWIP_IPV6_REASS */

#if LWIP_IPV6 && LWIP_IPV6_FRAG
#if !LWIP_NETIF_TX_SINGLE_PBUF
RET_DATA ip6_frag_alloc_pbuf_custom_ref_fp_t    ip6_frag_alloc_pbuf_custom_ref_adpt;
RET_DATA ip6_frag_free_pbuf_custom_ref_fp_t     ip6_frag_free_pbuf_custom_ref_adpt;
RET_DATA ip6_frag_free_pbuf_custom_fp_t         ip6_frag_free_pbuf_custom_adpt;
#endif
#endif

#if LWIP_IPV6 && LWIP_IPV6_REASS
RET_DATA ip6_reass_tmr_fp_t                     ip6_reass_tmr_adpt;
RET_DATA  ip6_reass_fp_t                        ip6_reass_adpt;
#endif /* LWIP_IPV6 && LWIP_IPV6_REASS */

#if LWIP_IPV6 && LWIP_IPV6_FRAG
RET_DATA  ip6_frag_fp_t                         ip6_frag_adpt;
#endif /* LWIP_IPV6 && LWIP_IPV6_FRAG */


#if LWIP_IPV6
void lwip_load_interface_ip6_frag(void)
{
    ip6_reassdatagrams = NULL;
    ip6_reass_pbufcount = 0;


#if LWIP_IPV6 && LWIP_IPV6_REASS
    ip6_reass_free_complete_datagram_adpt       = LWIP_ROMFN(ip6_reass_free_complete_datagram);
#if IP_REASS_FREE_OLDEST
    ip6_reass_remove_oldest_datagram_adpt       = LWIP_ROMFN(ip6_reass_remove_oldest_datagram);
#endif /* IP_REASS_FREE_OLDEST */
#endif /* LWIP_IPV6 && LWIP_IPV6_REASS */

#if LWIP_IPV6 && LWIP_IPV6_FRAG
#if !LWIP_NETIF_TX_SINGLE_PBUF
    ip6_frag_alloc_pbuf_custom_ref_adpt         = LWIP_ROMFN(ip6_frag_alloc_pbuf_custom_ref);
    ip6_frag_free_pbuf_custom_ref_adpt          = LWIP_ROMFN(ip6_frag_free_pbuf_custom_ref);
    ip6_frag_free_pbuf_custom_adpt              = LWIP_ROMFN(ip6_frag_free_pbuf_custom);
#endif
#endif

#if LWIP_IPV6 && LWIP_IPV6_REASS
    ip6_reass_tmr_adpt                          = LWIP_ROMFN(ip6_reass_tmr);
    ip6_reass_adpt                              = LWIP_ROMFN(ip6_reass);
#endif /* LWIP_IPV6 && LWIP_IPV6_REASS */

#if LWIP_IPV6 && LWIP_IPV6_FRAG
    ip6_frag_adpt                               = LWIP_ROMFN(ip6_frag);
#endif /* LWIP_IPV6 && LWIP_IPV6_FRAG */
}
#endif



#if LWIP_IPV6 && LWIP_IPV6_REASS  /* don't build if not configured for use in lwipopts.h */

/* Forward declarations. */
//static void ip6_reass_free_complete_datagram(struct ip6_reassdata *ipr);
#if IP_REASS_FREE_OLDEST
//static void ip6_reass_remove_oldest_datagram(struct ip6_reassdata *ipr, int pbufs_needed);
#endif /* IP_REASS_FREE_OLDEST */

void
ip6_reass_tmr(void)
{
    ip6_reass_tmr_adpt();
}

/**
 * Free a datagram (struct ip6_reassdata) and all its pbufs.
 * Updates the total count of enqueued pbufs (ip6_reass_pbufcount),
 * sends an ICMP time exceeded packet.
 *
 * @param ipr datagram to free
 */
void
ip6_reass_free_complete_datagram(struct ip6_reassdata *ipr)
{
    ip6_reass_free_complete_datagram_adpt(ipr);
}

#if IP_REASS_FREE_OLDEST
/**
 * Free the oldest datagram to make room for enqueueing new fragments.
 * The datagram ipr is not freed!
 *
 * @param ipr ip6_reassdata for the current fragment
 * @param pbufs_needed number of pbufs needed to enqueue
 *        (used for freeing other datagrams if not enough space)
 */
void
ip6_reass_remove_oldest_datagram(struct ip6_reassdata *ipr, int pbufs_needed)
{
    ip6_reass_remove_oldest_datagram_adpt(ipr, pbufs_needed);
}
#endif /* IP_REASS_FREE_OLDEST */

/**
 * Reassembles incoming IPv6 fragments into an IPv6 datagram.
 *
 * @param p points to the IPv6 Fragment Header
 * @return NULL if reassembly is incomplete, pbuf pointing to
 *         IPv6 Header if reassembly is complete
 */
struct pbuf *
ip6_reass(struct pbuf *p)
{
    return ip6_reass_adpt(p);
}
#endif /* LWIP_IPV6 && LWIP_IPV6_REASS */

#if LWIP_IPV6 && LWIP_IPV6_FRAG

#if !LWIP_NETIF_TX_SINGLE_PBUF
/** Allocate a new struct pbuf_custom_ref */
struct pbuf_custom_ref*
ip6_frag_alloc_pbuf_custom_ref(void)
{
    return ip6_frag_alloc_pbuf_custom_ref_adpt()  ;
}

/** Free a struct pbuf_custom_ref */
void
ip6_frag_free_pbuf_custom_ref(struct pbuf_custom_ref* p)
{
    ip6_frag_free_pbuf_custom_ref_adpt(p);
}

/** Free-callback function to free a 'struct pbuf_custom_ref', called by
 * pbuf_free. */
void
ip6_frag_free_pbuf_custom(struct pbuf *p)
{
    ip6_frag_free_pbuf_custom_adpt(p);
}
#endif /* !LWIP_NETIF_TX_SINGLE_PBUF */

/**
 * Fragment an IPv6 datagram if too large for the netif or path MTU.
 *
 * Chop the datagram in MTU sized chunks and send them in order
 * by pointing PBUF_REFs into p
 *
 * @param p ipv6 packet to send
 * @param netif the netif on which to send
 * @param dest destination ipv6 address to which to send
 *
 * @return ERR_OK if sent successfully, err_t otherwise
 */
err_t
ip6_frag(struct pbuf *p, struct netif *netif, const ip6_addr_t *dest)
{
    return ip6_frag_adpt(p, netif, dest);
}

#endif /* LWIP_IPV6 && LWIP_IPV6_FRAG */
