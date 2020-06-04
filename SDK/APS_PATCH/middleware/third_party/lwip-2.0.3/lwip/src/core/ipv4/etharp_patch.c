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

#if LWIP_ARP || LWIP_ETHERNET

#include "lwip/etharp.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "lwip/dhcp.h"
#include "lwip/autoip.h"
#include "netif/ethernet.h"

#include "wifi_mac_common.h"

#include <string.h>

#ifdef LWIP_HOOK_FILENAME
#include LWIP_HOOK_FILENAME
#endif

#if defined(LWIP_ROMBUILD)
#include "ipv4/etharp_if.h"
#endif

#if LWIP_IPV4 && LWIP_ARP /* don't build if not configured for use in lwipopts.h */

/** Re-request a used ARP entry 1 minute before it would expire to prevent
 *  breaking a steadily used connection because the ARP entry timed out. */
#define ARP_AGE_REREQUEST_USED_UNICAST   (ARP_MAXAGE - 30)
#define ARP_AGE_REREQUEST_USED_BROADCAST (ARP_MAXAGE - 15)

/** the time an ARP entry stays pending after first request,
 *  for ARP_TMR_INTERVAL = 1000, this is
 *  10 seconds.
 *
 *  @internal Keep this number at least 2, otherwise it might
 *  run out instantly if the timeout occurs directly after a request.
 */
#define ARP_MAXPENDING 5

/** ARP states */
enum etharp_state {
  ETHARP_STATE_EMPTY = 0,
  ETHARP_STATE_PENDING,
  ETHARP_STATE_STABLE,
  ETHARP_STATE_STABLE_REREQUESTING_1,
  ETHARP_STATE_STABLE_REREQUESTING_2
#if ETHARP_SUPPORT_STATIC_ENTRIES
  ,ETHARP_STATE_STATIC
#endif /* ETHARP_SUPPORT_STATIC_ENTRIES */
};

struct etharp_entry {
#if ARP_QUEUEING
  /** Pointer to queue of pending outgoing packets on this ARP entry. */
  struct etharp_q_entry *q;
#else /* ARP_QUEUEING */
  /** Pointer to a single pending outgoing packet on this ARP entry. */
  struct pbuf *q;
#endif /* ARP_QUEUEING */
  ip4_addr_t ipaddr;
  struct netif *netif;
  struct eth_addr ethaddr;
  u16_t ctime;
  u8_t state;
};

extern struct etharp_entry arp_table[ARP_TABLE_SIZE];

#if !LWIP_NETIF_HWADDRHINT
extern u8_t etharp_cached_entry;
#endif /* !LWIP_NETIF_HWADDRHINT */

#define LWIP_AUTO_ARP_TIMEOUT               90 // seconds
#define LWIP_AUTO_ARP_BROADCAST_ADV_TIME    15 // seconds
#define LWIP_AUTO_ARP_UNICAST_ADV_TIME      30 // seconds

extern u8_t g_lwip_auto_arp_enable;
extern u32_t g_lwip_auto_arp_timeout;
extern u8_t g_lwip_one_shot_arp_enable;

extern lwip_auto_arp_enable_fp_t lwip_auto_arp_enable;
extern lwip_one_shot_arp_enable_fp_t lwip_one_shot_arp_enable;

extern int dhcp_does_arp_check_flag;

/** Try hard to create a new entry - we want the IP address to appear in
    the cache (even if this means removing an active entry or so). */
#define ETHARP_FLAG_TRY_HARD     1
#define ETHARP_FLAG_FIND_ONLY    2
#if ETHARP_SUPPORT_STATIC_ENTRIES
#define ETHARP_FLAG_STATIC_ENTRY 4
#endif /* ETHARP_SUPPORT_STATIC_ENTRIES */

#if LWIP_NETIF_HWADDRHINT
#define ETHARP_SET_HINT(netif, hint)  if (((netif) != NULL) && ((netif)->addr_hint != NULL))  \
                                      *((netif)->addr_hint) = (hint);
#else /* LWIP_NETIF_HWADDRHINT */
#define ETHARP_SET_HINT(netif, hint)  (etharp_cached_entry = (hint))
#endif /* LWIP_NETIF_HWADDRHINT */

extern struct eth_addr dhcp_svr_hwaddr;


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
void etharp_input_patch(struct pbuf *p, struct netif *netif)
{
  struct etharp_hdr *hdr;
  /* these are aligned properly, whereas the ARP header fields might not be */
  ip4_addr_t sipaddr, dipaddr;
  u8_t for_us;

  LWIP_ERROR("netif != NULL", (netif != NULL), return;);

  hdr = (struct etharp_hdr *)p->payload;

  /* RFC 826 "Packet Reception": */
  if ((hdr->hwtype != PP_HTONS(HWTYPE_ETHERNET)) ||
      (hdr->hwlen != ETH_HWADDR_LEN) ||
      (hdr->protolen != sizeof(ip4_addr_t)) ||
      (hdr->proto != PP_HTONS(ETHTYPE_IP)))  {
    LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_WARNING,
      ("etharp_input: packet dropped, wrong hw type, hwlen, proto, protolen or ethernet type (%"U16_F"/%"U16_F"/%"U16_F"/%"U16_F")\n",
      hdr->hwtype, (u16_t)hdr->hwlen, hdr->proto, (u16_t)hdr->protolen));
    ETHARP_STATS_INC(etharp.proterr);
    ETHARP_STATS_INC(etharp.drop);
    pbuf_free(p);
    return;
  }
  ETHARP_STATS_INC(etharp.recv);

#if LWIP_AUTOIP
  /* We have to check if a host already has configured our random
   * created link local address and continuously check if there is
   * a host with this IP-address so we can detect collisions */
  autoip_arp_reply(netif, hdr);
#endif /* LWIP_AUTOIP */

  /* Copy struct ip4_addr2 to aligned ip4_addr, to support compilers without
   * structure packing (not using structure copy which breaks strict-aliasing rules). */
  IPADDR2_COPY(&sipaddr, &hdr->sipaddr);
  IPADDR2_COPY(&dipaddr, &hdr->dipaddr);

  /* this interface is not configured? */
  if (ip4_addr_isany_val(*netif_ip4_addr(netif))) {
    for_us = 0;
  } else {
    /* ARP packet directed to us? */
    for_us = (u8_t)ip4_addr_cmp(&dipaddr, netif_ip4_addr(netif));
  }

  /* ARP message directed to us?
      -> add IP address in ARP cache; assume requester wants to talk to us,
         can result in directly sending the queued packets for this host.
     ARP message not directed to us?
      ->  update the source IP address in the cache, if present */

  LWIP_DEBUGF(ETHARP_DEBUG, ("etharp_input: gateway: %"U16_F".%"U16_F".%"U16_F".%"U16_F"\n",
    ip4_addr1_16(netif_ip4_gw(netif)),
    ip4_addr2_16(netif_ip4_gw(netif)),
    ip4_addr3_16(netif_ip4_gw(netif)),
    ip4_addr4_16(netif_ip4_gw(netif))));

  LWIP_DEBUGF(ETHARP_DEBUG, ("etharp_input: src ipaddr:%"U16_F".%"U16_F".%"U16_F".%"U16_F" - src hwaddr: %02"X16_F":%02"X16_F":%02"X16_F":%02"X16_F":%02"X16_F":%02"X16_F"\n",
  ip4_addr1_16(&sipaddr), ip4_addr2_16(&sipaddr), ip4_addr3_16(&sipaddr), ip4_addr4_16(&sipaddr),
  (u16_t)(hdr->shwaddr.addr[0]), (u16_t)hdr->shwaddr.addr[1], (u16_t)hdr->shwaddr.addr[2],
  (u16_t)hdr->shwaddr.addr[3], (u16_t)hdr->shwaddr.addr[4], (u16_t)hdr->shwaddr.addr[5]));
#if 0
  LWIP_DEBUGF(ETHARP_DEBUG, ("etharp_input: dst ipaddr:%"U16_F".%"U16_F".%"U16_F".%"U16_F" - dst hwaddr: %02"X16_F":%02"X16_F":%02"X16_F":%02"X16_F":%02"X16_F":%02"X16_F"\n",
  ip4_addr1_16(&dipaddr), ip4_addr2_16(&dipaddr), ip4_addr3_16(&dipaddr), ip4_addr4_16(&dipaddr),
  (u16_t)(hdr->dhwaddr.addr[0]), (u16_t)hdr->dhwaddr.addr[1], (u16_t)hdr->dhwaddr.addr[2],
  (u16_t)hdr->dhwaddr.addr[3], (u16_t)hdr->dhwaddr.addr[4], (u16_t)hdr->dhwaddr.addr[5]));
#endif
#if OPL_LWIP_ARP
  if ( (memcmp(netif_ip4_gw(netif), &sipaddr, sizeof(ip4_addr_t)) == 0) &&
       (memcmp(&dhcp_svr_hwaddr, &hdr->shwaddr, ETH_HWADDR_LEN-1)!=0) )
  {
    // check if source IP address is gateway IP and it's hwaddr is not gateway addr
    // if yes, we filter duplicated gateway IP which have different hw address when updating lwip ARP entry
    LWIP_DEBUGF (ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_input: Duplicate gateway IP with differnet hwaddr\n"));
  }
  else
#endif
  {
    etharp_update_arp_entry_adpt(netif, &sipaddr, &(hdr->shwaddr),
         for_us ? ETHARP_FLAG_TRY_HARD : ETHARP_FLAG_FIND_ONLY);
  }

  /* now act on the message itself */
  switch (hdr->opcode) {
  /* ARP request? */
  case PP_HTONS(ARP_REQUEST):
    /* ARP request. If it asked for our address, we send out a
     * reply. In any case, we time-stamp any existing ARP entry,
     * and possibly send out an IP packet that was queued on it. */

    LWIP_DEBUGF (ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_input: incoming ARP request\n"));
    /* ARP request for our address? */
    if (for_us) {
      /* send ARP response */
      etharp_raw_adpt(netif,
                 (struct eth_addr *)netif->hwaddr, &hdr->shwaddr,
                 (struct eth_addr *)netif->hwaddr, netif_ip4_addr(netif),
                 &hdr->shwaddr, &sipaddr,
                 ARP_REPLY);
    /* we are not configured? */
    } else if (ip4_addr_isany_val(*netif_ip4_addr(netif))) {
      /* { for_us == 0 and netif->ip_addr.addr == 0 } */
      LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_input: we are unconfigured, ARP request ignored.\n"));
    /* request was not directed to us */
    } else {
      /* { for_us == 0 and netif->ip_addr.addr != 0 } */
      LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_input: ARP request was not for us.\n"));
    }
    break;
  case PP_HTONS(ARP_REPLY):
    /* ARP reply. We already updated the ARP cache earlier. */
    LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_input: incoming ARP reply\n"));
#if (LWIP_DHCP && DHCP_DOES_ARP_CHECK)
    /* DHCP wants to know about ARP replies from any host with an
     * IP address also offered to us by the DHCP server. We do not
     * want to take a duplicate IP address on a single network.
     * @todo How should we handle redundant (fail-over) interfaces? */
    if (dhcp_does_arp_check_flag == 1) {
        dhcp_arp_reply(netif, &sipaddr);
    }
#endif /* (LWIP_DHCP && DHCP_DOES_ARP_CHECK) */
    break;
  default:
    LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_input: ARP unknown opcode type %"S16_F"\n", lwip_htons(hdr->opcode)));
    ETHARP_STATS_INC(etharp.err);
    break;
  }
  /* free ARP packet */
  pbuf_free(p);
}


void lwip_load_interface_etharp_patch(void)
{
    etharp_input_adpt  =  etharp_input_patch;
}

#endif /* LWIP_IPV4 && LWIP_ARP */

#endif /* LWIP_ARP || LWIP_ETHERNET */
