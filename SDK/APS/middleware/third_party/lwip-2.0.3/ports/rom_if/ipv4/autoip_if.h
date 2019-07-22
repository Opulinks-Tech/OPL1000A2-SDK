/**
 * @file
 *
 * AutoIP Automatic LinkLocal IP Configuration
 */

/*
 *
 * Copyright (c) 2007 Dominik Spies <kontakt@dspies.de>
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
 * Author: Dominik Spies <kontakt@dspies.de>
 *
 * This is a AutoIP implementation for the lwIP TCP/IP stack. It aims to conform
 * with RFC 3927.
 *
 */

#ifndef LWIP_HDR_AUTOIP_IF_H
#define LWIP_HDR_AUTOIP_IF_H

#include "lwip/opt.h"

#if LWIP_IPV4 && LWIP_AUTOIP /* don't build if not configured for use in lwipopts.h */

#include "lwip/netif.h"
/* #include "lwip/udp.h" */
#include "lwip/etharp.h"

#ifdef __cplusplus
extern "C" {
#endif


struct autoip;

/* Private API fucntion used in autoip.c*/
void  autoip_start_probing(struct netif *netif);
void  autoip_restart(struct netif *netif);
void  autoip_handle_arp_conflict(struct netif *netif);
void  autoip_create_addr(struct netif *netif, ip4_addr_t *ipaddr);
err_t autoip_arp_probe(struct netif *netif);
err_t autoip_arp_announce(struct netif *netif);
err_t autoip_bind(struct netif *netif);

/* Rom patch interface */
typedef void  (*autoip_start_probing_fp_t)(struct netif *netif);
typedef void  (*autoip_restart_fp_t)(struct netif *netif);
typedef void  (*autoip_handle_arp_conflict_fp_t)(struct netif *netif);
typedef void  (*autoip_create_addr_fp_t)(struct netif *netif, ip4_addr_t *ipaddr);
typedef err_t (*autoip_arp_probe_fp_t)(struct netif *netif);
typedef err_t (*autoip_arp_announce_fp_t)(struct netif *netif);
typedef err_t (*autoip_bind_fp_t)(struct netif *netif);

/* Export interface funtion pointer */
extern autoip_start_probing_fp_t           autoip_start_probing_adpt;
extern autoip_restart_fp_t                 autoip_restart_adpt;
extern autoip_handle_arp_conflict_fp_t     autoip_handle_arp_conflict_adpt;
extern autoip_create_addr_fp_t             autoip_create_addr_adpt;
extern autoip_arp_probe_fp_t               autoip_arp_probe_adpt;
extern autoip_arp_announce_fp_t            autoip_arp_announce_adpt;
extern autoip_bind_fp_t                    autoip_bind_adpt;


/* Public API fucntion used in autoip.c*/
/* Rom patch interface */
typedef void  (*autoip_set_struct_fp_t)(struct netif *netif, struct autoip *autoip);
typedef err_t (*autoip_start_fp_t)(struct netif *netif);
typedef err_t (*autoip_stop_fp_t)(struct netif *netif);
typedef void  (*autoip_arp_reply_fp_t)(struct netif *netif, struct etharp_hdr *hdr);
typedef void  (*autoip_tmr_fp_t)(void);
typedef void  (*autoip_network_changed_fp_t)(struct netif *netif);
typedef u8_t  (*autoip_supplied_address_fp_t)(const struct netif *netif);
typedef u8_t  (*autoip_accept_packet_fp_t)(struct netif *netif, const ip4_addr_t *addr);

/* Export interface funtion pointer */
extern autoip_set_struct_fp_t              autoip_set_struct_adpt;
extern autoip_start_fp_t                   autoip_start_adpt;
extern autoip_stop_fp_t                    autoip_stop_adpt;
extern autoip_arp_reply_fp_t               autoip_arp_reply_adpt;
extern autoip_tmr_fp_t                     autoip_tmr_adpt;
extern autoip_network_changed_fp_t         autoip_network_changed_adpt;
extern autoip_supplied_address_fp_t        autoip_supplied_address_adpt;
extern autoip_accept_packet_fp_t           autoip_accept_packet_adpt;



#ifdef __cplusplus
}
#endif

#endif /* LWIP_IPV4 && LWIP_AUTOIP */

#endif /* LWIP_HDR_AUTOIP_IF_H */
