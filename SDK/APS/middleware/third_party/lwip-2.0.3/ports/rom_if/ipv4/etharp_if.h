/**
 * @file
 * Ethernet output function - handles OUTGOING ethernet level traffic, implements
 * ARP resolving.
 * To be used in most low-level netif implementations
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
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

#ifndef LWIP_HDR_NETIF_ETHARP_IF_H
#define LWIP_HDR_NETIF_ETHARP_IF_H

#include "lwip/opt.h"

#if defined(LWIP_ROMBUILD)


#if LWIP_ARP || LWIP_ETHERNET /* don't build if not configured for use in lwipopts.h */

#include "lwip/pbuf.h"
#include "lwip/ip4_addr.h"
#include "lwip/netif.h"
#include "lwip/ip4.h"
#include "lwip/prot/ethernet.h"

#ifdef __cplusplus
extern "C" {
#endif

#if LWIP_IPV4 && LWIP_ARP /* don't build if not configured for use in lwipopts.h */

#include "lwip/prot/etharp.h"

#if 0
/** 1 seconds period */
#define ARP_TMR_INTERVAL 1000

#if ARP_QUEUEING
/** struct for queueing outgoing packets for unknown address
  * defined here to be accessed by memp.h
  */
struct etharp_q_entry {
  struct etharp_q_entry *next;
  struct pbuf *p;
};
#endif /* ARP_QUEUEING */
#endif

//#define etharp_init() /* Compatibility define, no init needed. */


/* static api fucntion */
void  free_etharp_q(struct etharp_q_entry *q);
void  etharp_free_entry(int i);
s8_t  etharp_find_entry(const ip4_addr_t *ipaddr, u8_t flags, struct netif* netif);
err_t etharp_update_arp_entry(struct netif *netif, const ip4_addr_t *ipaddr, struct eth_addr *ethaddr, u8_t flags);
err_t etharp_output_to_arp_index(struct netif *netif, struct pbuf *q, u8_t arp_idx);
err_t etharp_raw(struct netif *netif, const struct eth_addr *ethsrc_addr,
           const struct eth_addr *ethdst_addr,
           const struct eth_addr *hwsrc_addr, const ip4_addr_t *ipsrc_addr,
           const struct eth_addr *hwdst_addr, const ip4_addr_t *ipdst_addr,
           const u16_t opcode);
err_t etharp_request_dst(struct netif *netif, const ip4_addr_t *ipaddr, const struct eth_addr* hw_dst_addr);


#if ARP_QUEUEING
/**
 * Free a complete queue of etharp entries
 *
 * @param q a qeueue of etharp_q_entry's to free
 */
typedef void (*free_etharp_q_fp_t)(struct etharp_q_entry *q);
extern free_etharp_q_fp_t free_etharp_q_adpt;
#endif /* ARP_QUEUEING */

typedef void  (*etharp_free_entry_fp_t)(int i);
typedef s8_t  (*etharp_find_entry_fp_t)(const ip4_addr_t *ipaddr, u8_t flags, struct netif* netif);
typedef err_t (*etharp_update_arp_entry_fp_t)(struct netif *netif, const ip4_addr_t *ipaddr, struct eth_addr *ethaddr, u8_t flags);
typedef err_t (*etharp_output_to_arp_index_fp_t)(struct netif *netif, struct pbuf *q, u8_t arp_idx);
typedef err_t (*etharp_raw_fp_t)(struct netif *netif, const struct eth_addr *ethsrc_addr,
           const struct eth_addr *ethdst_addr,
           const struct eth_addr *hwsrc_addr, const ip4_addr_t *ipsrc_addr,
           const struct eth_addr *hwdst_addr, const ip4_addr_t *ipdst_addr,
           const u16_t opcode);
typedef err_t (*etharp_request_dst_fp_t)(struct netif *netif, const ip4_addr_t *ipaddr, const struct eth_addr* hw_dst_addr);


extern etharp_free_entry_fp_t           etharp_free_entry_adpt;
extern etharp_find_entry_fp_t           etharp_find_entry_adpt;
extern etharp_update_arp_entry_fp_t     etharp_update_arp_entry_adpt;
extern etharp_output_to_arp_index_fp_t  etharp_output_to_arp_index_adpt;
extern etharp_raw_fp_t                  etharp_raw_adpt;
extern etharp_request_dst_fp_t          etharp_request_dst_adpt;

/*pub api fucntion */
typedef void  (*etharp_tmr_fp_t)(void);
typedef s8_t  (*etharp_find_addr_fp_t)(struct netif *netif, const ip4_addr_t *ipaddr,
         struct eth_addr **eth_ret, const ip4_addr_t **ip_ret);
typedef u8_t  (*etharp_get_entry_fp_t)(u8_t i, ip4_addr_t **ipaddr, struct netif **netif, struct eth_addr **eth_ret);
typedef err_t (*etharp_output_fp_t)(struct netif *netif, struct pbuf *q, const ip4_addr_t *ipaddr);
typedef err_t (*etharp_query_fp_t)(struct netif *netif, const ip4_addr_t *ipaddr, struct pbuf *q);
typedef err_t (*etharp_request_fp_t)(struct netif *netif, const ip4_addr_t *ipaddr);
typedef void  (*etharp_cleanup_netif_fp_t)(struct netif *netif);

extern etharp_tmr_fp_t                  etharp_tmr_adpt;
extern etharp_find_addr_fp_t            etharp_find_addr_adpt;
extern etharp_get_entry_fp_t            etharp_get_entry_adpt;
extern etharp_output_fp_t               etharp_output_adpt;
extern etharp_query_fp_t                etharp_query_adpt;
extern etharp_request_fp_t              etharp_request_adpt;
extern etharp_cleanup_netif_fp_t        etharp_cleanup_netif_adpt;

#if ETHARP_SUPPORT_STATIC_ENTRIES
typedef err_t (*etharp_add_static_entry_fp_t)(const ip4_addr_t *ipaddr, struct eth_addr *ethaddr);
typedef err_t (*etharp_remove_static_entry_fp_t)(const ip4_addr_t *ipaddr);

extern etharp_add_static_entry_fp_t     etharp_add_static_entry_adpt;
extern etharp_remove_static_entry_fp_t  etharp_remove_static_entry_adpt;
#endif /* ETHARP_SUPPORT_STATIC_ENTRIES */

#endif /* LWIP_IPV4 && LWIP_ARP */

typedef void (*etharp_input_fp_t)(struct pbuf *p, struct netif *netif);
extern etharp_input_fp_t etharp_input_adpt;


#ifdef __cplusplus
}
#endif

#endif /* LWIP_ARP || LWIP_ETHERNET */

#endif //#if defined(LWIP_ROMBUILD)

#endif /* LWIP_HDR_NETIF_ETHARP_IF_H */
