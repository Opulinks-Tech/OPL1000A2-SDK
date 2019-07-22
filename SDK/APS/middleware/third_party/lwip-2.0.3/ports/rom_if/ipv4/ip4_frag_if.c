/**
 * @file
 * This is the IPv4 packet segmentation and reassembly implementation.
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
 * Author: Jani Monoses <jani@iv.ro>
 *         Simon Goldschmidt
 * original reassembly code by Adam Dunkels <adam@sics.se>
 *
 */

#include "lwip/opt.h"

#if LWIP_IPV4

#include "lwip/ip4_frag.h"
#include "lwip/def.h"
#include "lwip/inet_chksum.h"
#include "lwip/netif.h"
#include "lwip/stats.h"
#include "lwip/icmp.h"

#include <string.h>

#if IP_REASSEMBLY
RET_DATA ip_reass_free_complete_datagram_fp_t                 ip_reass_free_complete_datagram_adpt;
RET_DATA ip_reass_remove_oldest_datagram_fp_t                 ip_reass_remove_oldest_datagram_adpt;
RET_DATA ip_reass_dequeue_datagram_fp_t                       ip_reass_dequeue_datagram_adpt;
RET_DATA ip_reass_chain_frag_into_datagram_and_validate_fp_t  ip_reass_chain_frag_into_datagram_and_validate_adpt;
RET_DATA ip_reass_enqueue_new_datagram_fp_t                   ip_reass_enqueue_new_datagram_adpt;
#endif

#if IP_FRAG
#if !LWIP_NETIF_TX_SINGLE_PBUF
RET_DATA ip_frag_free_pbuf_custom_ref_fp_t                    ip_frag_free_pbuf_custom_ref_adpt;
RET_DATA ipfrag_free_pbuf_custom_fp_t                         ipfrag_free_pbuf_custom_adpt;
RET_DATA ip_frag_alloc_pbuf_custom_ref_fp_t                   ip_frag_alloc_pbuf_custom_ref_adpt;
#endif /* LWIP_PBUF_CUSTOM_REF_DEFINED */
#endif /* IP_FRAG */

#if IP_REASSEMBLY
//RET_DATA ip_reass_init_fp_t                                   ip_reass_init_adpt;
RET_DATA ip_reass_tmr_fp_t                                    ip_reass_tmr_adpt;
RET_DATA ip4_reass_fp_t                                       ip4_reass_adpt;
#endif
#if IP_FRAG
RET_DATA ip4_frag_fp_t                                        ip4_frag_adpt;
#endif


void lwip_load_interface_ip4_frag(void)
{
    reassdatagrams = NULL;
    ip_reass_pbufcount = 0;

#if IP_REASSEMBLY
    ip_reass_free_complete_datagram_adpt                    = LWIP_ROMFN(ip_reass_free_complete_datagram);
    ip_reass_remove_oldest_datagram_adpt                    = LWIP_ROMFN(ip_reass_remove_oldest_datagram);
    ip_reass_dequeue_datagram_adpt                          = LWIP_ROMFN(ip_reass_dequeue_datagram);
    ip_reass_chain_frag_into_datagram_and_validate_adpt     = LWIP_ROMFN(ip_reass_chain_frag_into_datagram_and_validate);
    ip_reass_enqueue_new_datagram_adpt                      = LWIP_ROMFN(ip_reass_enqueue_new_datagram);
#endif

#if IP_FRAG
#if !LWIP_NETIF_TX_SINGLE_PBUF
    ip_frag_free_pbuf_custom_ref_adpt                       = LWIP_ROMFN(ip_frag_free_pbuf_custom_ref);
    ipfrag_free_pbuf_custom_adpt                            = LWIP_ROMFN(ipfrag_free_pbuf_custom);
    ip_frag_alloc_pbuf_custom_ref_adpt                      = LWIP_ROMFN(ip_frag_alloc_pbuf_custom_ref);
#endif /* LWIP_PBUF_CUSTOM_REF_DEFINED */
#endif /* IP_FRAG */

#if IP_REASSEMBLY
    //ip_reass_init_adpt                                      = LWIP_ROMFN(ip_reass_init);
    ip_reass_tmr_adpt                                       = LWIP_ROMFN(ip_reass_tmr);
    ip4_reass_adpt                                          = LWIP_ROMFN(ip4_reass);
#endif
#if IP_FRAG
    ip4_frag_adpt                                           = LWIP_ROMFN(ip4_frag);
#endif
}


#if IP_REASSEMBLY


/** Set to 0 to prevent freeing the oldest datagram when the reassembly buffer is
 * full (IP_REASS_MAX_PBUFS pbufs are enqueued). The code gets a little smaller.
 * Datagrams will be freed by timeout only. Especially useful when MEMP_NUM_REASSDATA
 * is set to 1, so one datagram can be reassembled at a time, only. */
#ifndef IP_REASS_FREE_OLDEST
#define IP_REASS_FREE_OLDEST 1
#endif /* IP_REASS_FREE_OLDEST */




/**
 * Reassembly timer base function
 * for both NO_SYS == 0 and 1 (!).
 *
 * Should be called every 1000 msec (defined by IP_TMR_INTERVAL).
 */
void
ip_reass_tmr(void)
{
    ip_reass_tmr_adpt();
}

/**
 * Free a datagram (struct ip_reassdata) and all its pbufs.
 * Updates the total count of enqueued pbufs (ip_reass_pbufcount),
 * SNMP counters and sends an ICMP time exceeded packet.
 *
 * @param ipr datagram to free
 * @param prev the previous datagram in the linked list
 * @return the number of pbufs freed
 */
int
ip_reass_free_complete_datagram(struct ip_reassdata *ipr, struct ip_reassdata *prev)
{
    return ip_reass_free_complete_datagram_adpt(ipr, prev);
}

#if IP_REASS_FREE_OLDEST
/**
 * Free the oldest datagram to make room for enqueueing new fragments.
 * The datagram 'fraghdr' belongs to is not freed!
 *
 * @param fraghdr IP header of the current fragment
 * @param pbufs_needed number of pbufs needed to enqueue
 *        (used for freeing other datagrams if not enough space)
 * @return the number of pbufs freed
 */
int
ip_reass_remove_oldest_datagram(struct ip_hdr *fraghdr, int pbufs_needed)
{
    return ip_reass_remove_oldest_datagram_adpt(fraghdr, pbufs_needed);
}
#endif /* IP_REASS_FREE_OLDEST */

/**
 * Enqueues a new fragment into the fragment queue
 * @param fraghdr points to the new fragments IP hdr
 * @param clen number of pbufs needed to enqueue (used for freeing other datagrams if not enough space)
 * @return A pointer to the queue location into which the fragment was enqueued
 */
struct ip_reassdata*
ip_reass_enqueue_new_datagram(struct ip_hdr *fraghdr, int clen)
{
    return ip_reass_enqueue_new_datagram_adpt(fraghdr, clen);
}

/**
 * Dequeues a datagram from the datagram queue. Doesn't deallocate the pbufs.
 * @param ipr points to the queue entry to dequeue
 */
void
ip_reass_dequeue_datagram(struct ip_reassdata *ipr, struct ip_reassdata *prev)
{
    ip_reass_dequeue_datagram_adpt(ipr, prev);
}

/**
 * Chain a new pbuf into the pbuf list that composes the datagram.  The pbuf list
 * will grow over time as  new pbufs are rx.
 * Also checks that the datagram passes basic continuity checks (if the last
 * fragment was received at least once).
 * @param ipr points to the reassembly state
 * @param new_p points to the pbuf for the current fragment
 * @param is_last is 1 if this pbuf has MF==0 (ipr->flags not updated yet)
 * @return see IP_REASS_VALIDATE_* defines
 */
int
ip_reass_chain_frag_into_datagram_and_validate(struct ip_reassdata *ipr, struct pbuf *new_p, int is_last)
{
    return ip_reass_chain_frag_into_datagram_and_validate_adpt(ipr, new_p, is_last);
}

/**
 * Reassembles incoming IP fragments into an IP datagram.
 *
 * @param p points to a pbuf chain of the fragment
 * @return NULL if reassembly is incomplete, ? otherwise
 */
struct pbuf *
ip4_reass(struct pbuf *p)
{
    return ip4_reass_adpt(p);
}
#endif /* IP_REASSEMBLY */

#if IP_FRAG
#if !LWIP_NETIF_TX_SINGLE_PBUF
/** Allocate a new struct pbuf_custom_ref */
struct pbuf_custom_ref*
ip_frag_alloc_pbuf_custom_ref(void)
{
    return ip_frag_alloc_pbuf_custom_ref_adpt();
}

/** Free a struct pbuf_custom_ref */
void
ip_frag_free_pbuf_custom_ref(struct pbuf_custom_ref* p)
{
    ip_frag_free_pbuf_custom_ref_adpt(p);
}

/** Free-callback function to free a 'struct pbuf_custom_ref', called by
 * pbuf_free. */
void
ipfrag_free_pbuf_custom(struct pbuf *p)
{
    ipfrag_free_pbuf_custom_adpt(p);
}
#endif /* !LWIP_NETIF_TX_SINGLE_PBUF */

/**
 * Fragment an IP datagram if too large for the netif.
 *
 * Chop the datagram in MTU sized chunks and send them in order
 * by pointing PBUF_REFs into p.
 *
 * @param p ip packet to send
 * @param netif the netif on which to send
 * @param dest destination ip address to which to send
 *
 * @return ERR_OK if sent successfully, err_t otherwise
 */
err_t
ip4_frag(struct pbuf *p, struct netif *netif, const ip4_addr_t *dest)
{
    return ip4_frag_adpt(p, netif, dest);
}
#endif /* IP_FRAG */

#endif /* LWIP_IPV4 */
