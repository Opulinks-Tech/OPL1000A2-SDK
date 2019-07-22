/**
 * @file
 * DHCP client API
 */

/*
 * Copyright (c) 2001-2004 Leon Woestenberg <leon.woestenberg@gmx.net>
 * Copyright (c) 2001-2004 Axon Digital Design B.V., The Netherlands.
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
 * Author: Leon Woestenberg <leon.woestenberg@gmx.net>
 *
 */
#ifndef LWIP_HDR_DHCP_IF_H
#define LWIP_HDR_DHCP_IF_H

#include "lwip/opt.h"

#if defined(LWIP_ROMBUILD)

#if LWIP_DHCP /* don't build if not configured for use in lwipopts.h */

#include "lwip/netif.h"
#include "lwip/udp.h"

#ifdef __cplusplus
extern "C" {
#endif

struct dhcp;


/* Private API fucntion used in dhcp.c*/
err_t dhcp_inc_pcb_refcount(void);
void  dhcp_dec_pcb_refcount(void);
void  dhcp_handle_offer(struct netif *netif);
void  dhcp_handle_ack(struct netif *netif);
void  dhcp_handle_nak(struct netif *netif);

/* DHCP client state machine functions */
err_t dhcp_discover(struct netif *netif);
err_t dhcp_select(struct netif *netif);
void  dhcp_bind(struct netif *netif);
#if DHCP_DOES_ARP_CHECK
err_t dhcp_decline(struct netif *netif);
void  dhcp_check(struct netif *netif);
#endif /* DHCP_DOES_ARP_CHECK */
err_t dhcp_rebind(struct netif *netif);
err_t dhcp_reboot(struct netif *netif);
void  dhcp_set_state(struct dhcp *dhcp, u8_t new_state);
err_t dhcp_parse_reply(struct dhcp *dhcp, struct pbuf *p);


/* receive, unfold, parse and free incoming messages */
void dhcp_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);

/* set the DHCP timers */
void dhcp_timeout(struct netif *netif);
void dhcp_t1_timeout(struct netif *netif);
void dhcp_t2_timeout(struct netif *netif);

/* build outgoing messages */
/* create a DHCP message, fill in common headers */
err_t dhcp_create_msg(struct netif *netif, struct dhcp *dhcp, u8_t message_type);
/* free a DHCP request */
void dhcp_delete_msg(struct dhcp *dhcp);
/* add a DHCP option (type, then length in bytes) */
void dhcp_option(struct dhcp *dhcp, u8_t option_type, u8_t option_len);
/* add option values */
void dhcp_option_byte(struct dhcp *dhcp, u8_t value);
void dhcp_option_short(struct dhcp *dhcp, u16_t value);
void dhcp_option_long(struct dhcp *dhcp, u32_t value);
#if LWIP_NETIF_HOSTNAME
void dhcp_option_hostname(struct dhcp *dhcp, struct netif *netif);
#endif /* LWIP_NETIF_HOSTNAME */
/* always add the DHCP options trailer to end and pad */
void dhcp_option_trailer(struct dhcp *dhcp);

typedef err_t (*dhcp_inc_pcb_refcount_fp_t)(void);
typedef void  (*dhcp_dec_pcb_refcount_fp_t)(void);
typedef err_t (*dhcp_discover_fp_t)(struct netif *netif);
typedef err_t (*dhcp_select_fp_t)(struct netif *netif);
typedef void  (*dhcp_bind_fp_t)(struct netif *netif);
#if DHCP_DOES_ARP_CHECK
typedef err_t (*dhcp_decline_fp_t)(struct netif *netif);
typedef void  (*dhcp_check_fp_t)(struct netif *netif);

#endif /* DHCP_DOES_ARP_CHECK */
typedef err_t (*dhcp_rebind_fp_t)(struct netif *netif);
typedef err_t (*dhcp_reboot_fp_t)(struct netif *netif);
typedef void  (*dhcp_set_state_fp_t)(struct dhcp *dhcp, u8_t new_state);
typedef void  (*dhcp_handle_offer_fp_t)(struct netif *netif);
typedef void  (*dhcp_handle_ack_fp_t)(struct netif *netif);
typedef void  (*dhcp_handle_nak_fp_t)(struct netif *netif);
typedef err_t (*dhcp_parse_reply_fp_t)(struct dhcp *dhcp, struct pbuf *p);

/* receive, unfold, parse and free incoming messages */
typedef void (*dhcp_recv_fp_t)(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);

/* set the DHCP timers */
typedef void (*dhcp_timeout_fp_t)(struct netif *netif);
typedef void (*dhcp_t1_timeout_fp_t)(struct netif *netif);
typedef void (*dhcp_t2_timeout_fp_t)(struct netif *netif);

/* build outgoing messages */
/* create a DHCP message, fill in common headers */
typedef err_t (*dhcp_create_msg_fp_t)(struct netif *netif, struct dhcp *dhcp, u8_t message_type);
/* free a DHCP request */
typedef void (*dhcp_delete_msg_fp_t)(struct dhcp *dhcp);
/* add a DHCP option (type, then length in bytes) */
typedef void (*dhcp_option_fp_t)(struct dhcp *dhcp, u8_t option_type, u8_t option_len);
/* add option values */
typedef void (*dhcp_option_byte_fp_t)(struct dhcp *dhcp, u8_t value);
typedef void (*dhcp_option_short_fp_t)(struct dhcp *dhcp, u16_t value);
typedef void (*dhcp_option_long_fp_t)(struct dhcp *dhcp, u32_t value);
#if LWIP_NETIF_HOSTNAME
typedef void (*dhcp_option_hostname_fp_t)(struct dhcp *dhcp, struct netif *netif);
#endif /* LWIP_NETIF_HOSTNAME */
/* always add the DHCP options trailer to end and pad */
typedef void (*dhcp_option_trailer_fp_t)(struct dhcp *dhcp);

extern dhcp_inc_pcb_refcount_fp_t   dhcp_inc_pcb_refcount_adpt;
extern dhcp_dec_pcb_refcount_fp_t   dhcp_dec_pcb_refcount_adpt;
extern dhcp_discover_fp_t           dhcp_discover_adpt;
extern dhcp_select_fp_t             dhcp_select_adpt;
extern dhcp_bind_fp_t               dhcp_bind_adpt;
#if DHCP_DOES_ARP_CHECK
extern dhcp_decline_fp_t            dhcp_decline_adpt;
extern dhcp_check_fp_t              dhcp_check_adpt;
#endif
extern dhcp_rebind_fp_t             dhcp_rebind_adpt;
extern dhcp_reboot_fp_t             dhcp_reboot_adpt;
extern dhcp_set_state_fp_t          dhcp_set_state_adpt;
extern dhcp_handle_offer_fp_t       dhcp_handle_offer_adpt;
extern dhcp_handle_ack_fp_t         dhcp_handle_ack_adpt;
extern dhcp_handle_nak_fp_t         dhcp_handle_nak_adpt;
extern dhcp_parse_reply_fp_t        dhcp_parse_reply_adpt;

extern dhcp_recv_fp_t               dhcp_recv_adpt;

extern dhcp_timeout_fp_t            dhcp_timeout_adpt;
extern dhcp_t1_timeout_fp_t         dhcp_t1_timeout_adpt;
extern dhcp_t2_timeout_fp_t         dhcp_t2_timeout_adpt;

extern dhcp_create_msg_fp_t         dhcp_create_msg_adpt;
extern dhcp_delete_msg_fp_t         dhcp_delete_msg_adpt;
extern dhcp_option_fp_t             dhcp_option_adpt;

extern dhcp_option_byte_fp_t        dhcp_option_byte_adpt;
extern dhcp_option_short_fp_t       dhcp_option_short_adpt;
extern dhcp_option_long_fp_t        dhcp_option_long_adpt;
#if LWIP_NETIF_HOSTNAME
extern dhcp_option_hostname_fp_t    dhcp_option_hostname_adpt;
#endif
extern dhcp_option_trailer_fp_t     dhcp_option_trailer_adpt;

/*Public API fucntion */
typedef void (*dhcp_set_struct_fp_t)(struct netif *netif, struct dhcp *dhcp);
/** Remove a struct dhcp previously set to the netif using dhcp_set_struct() */
#define dhcp_remove_struct(netif) netif_set_client_data(netif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP, NULL)
typedef void (*dhcp_cleanup_fp_t)(struct netif *netif);
typedef void (*dhcp_set_cb_fp_t)(struct netif *netif, void (*cb)(struct netif*));
typedef err_t (*dhcp_start_fp_t)(struct netif *netif);
typedef err_t (*dhcp_renew_fp_t)(struct netif *netif);
typedef err_t (*dhcp_release_fp_t)(struct netif *netif);
typedef void (*dhcp_stop_fp_t)(struct netif *netif);
typedef void (*dhcp_inform_fp_t)(struct netif *netif);
typedef void (*dhcp_network_changed_fp_t)(struct netif *netif);
#if DHCP_DOES_ARP_CHECK
typedef void (*dhcp_arp_reply_fp_t)(struct netif *netif, const ip4_addr_t *addr);
#endif
typedef u8_t (*dhcp_supplied_address_fp_t)(const struct netif *netif);
/* to be called every minute */
typedef void (*dhcp_coarse_tmr_fp_t)(void);
/* to be called every half second */
typedef void (*dhcp_fine_tmr_fp_t)(void);

extern dhcp_set_struct_fp_t        dhcp_set_struct_adpt;
extern dhcp_cleanup_fp_t           dhcp_cleanup_adpt;
extern dhcp_start_fp_t             dhcp_start_adpt;
extern dhcp_set_cb_fp_t            dhcp_set_cb_adpt;
extern dhcp_renew_fp_t             dhcp_renew_adpt;
extern dhcp_release_fp_t           dhcp_release_adpt;
extern dhcp_stop_fp_t              dhcp_stop_adpt;
extern dhcp_inform_fp_t            dhcp_inform_adpt;
extern dhcp_network_changed_fp_t   dhcp_network_changed_adpt;
#if DHCP_DOES_ARP_CHECK
extern dhcp_arp_reply_fp_t         dhcp_arp_reply_adpt;
#endif
extern dhcp_supplied_address_fp_t  dhcp_supplied_address_adpt;
extern dhcp_coarse_tmr_fp_t        dhcp_coarse_tmr_adpt;
extern dhcp_fine_tmr_fp_t          dhcp_fine_tmr_adpt;



#if LWIP_DHCP_GET_NTP_SRV
/** This function must exist, in other to add offered NTP servers to
 * the NTP (or SNTP) engine.
 * See LWIP_DHCP_MAX_NTP_SERVERS */
extern void dhcp_set_ntp_servers(u8_t num_ntp_servers, const ip4_addr_t* ntp_server_addrs);
#endif /* LWIP_DHCP_GET_NTP_SRV */

#define netif_dhcp_data(netif) ((struct dhcp*)netif_get_client_data(netif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP))

#ifdef __cplusplus
}
#endif

#endif /* LWIP_DHCP */

#endif //#if defined(LWIP_ROMBUILD)


#endif /*LWIP_HDR_DHCP_IF_H*/
