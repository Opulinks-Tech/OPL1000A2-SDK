/**
 * @file
 * netif API (to be used from TCPIP thread)
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
#ifndef LWIP_HDR_NETIF_IF_H
#define LWIP_HDR_NETIF_IF_H

#include "lwip/opt.h"

#if defined(LWIP_ROMBUILD)

#define ENABLE_LOOPBACK (LWIP_NETIF_LOOPBACK || LWIP_HAVE_LOOPIF)

#include "lwip/err.h"

#include "lwip/ip_addr.h"

#include "lwip/def.h"
#include "lwip/pbuf.h"
#include "lwip/stats.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Throughout this file, IP addresses are expected to be in
 * the same byte order as in IP_PCB. */

/** Generic data structure used for all lwIP network interfaces.
 *  The following fields should be filled in by the initialization
 *  function for the device driver: hwaddr_len, hwaddr[], mtu, flags */
struct netif;

/** Function prototype for netif init functions. Set up flags and output/linkoutput
 * callback functions in this function.
 *
 * @param netif The netif to initialize
 */
typedef err_t (*netif_init_fn)(struct netif *netif);
/** Function prototype for netif->input functions. This function is saved as 'input'
 * callback function in the netif struct. Call it when a packet has been received.
 *
 * @param p The received packet, copied into a pbuf
 * @param inp The netif which received the packet
 */
typedef err_t (*netif_input_fn)(struct pbuf *p, struct netif *inp);

#if LWIP_IPV4
/** Function prototype for netif->output functions. Called by lwIP when a packet
 * shall be sent. For ethernet netif, set this to 'etharp_output' and set
 * 'linkoutput'.
 *
 * @param netif The netif which shall send a packet
 * @param p The packet to send (p->payload points to IP header)
 * @param ipaddr The IP address to which the packet shall be sent
 */
typedef err_t (*netif_output_fn)(struct netif *netif, struct pbuf *p,
       const ip4_addr_t *ipaddr);
#endif /* LWIP_IPV4*/

#if LWIP_IPV6
/** Function prototype for netif->output_ip6 functions. Called by lwIP when a packet
 * shall be sent. For ethernet netif, set this to 'ethip6_output' and set
 * 'linkoutput'.
 *
 * @param netif The netif which shall send a packet
 * @param p The packet to send (p->payload points to IP header)
 * @param ipaddr The IPv6 address to which the packet shall be sent
 */
typedef err_t (*netif_output_ip6_fn)(struct netif *netif, struct pbuf *p,
       const ip6_addr_t *ipaddr);
#endif /* LWIP_IPV6 */

/** Function prototype for netif->linkoutput functions. Only used for ethernet
 * netifs. This function is called by ARP when a packet shall be sent.
 *
 * @param netif The netif which shall send a packet
 * @param p The packet to send (raw ethernet packet)
 */
typedef err_t (*netif_linkoutput_fn)(struct netif *netif, struct pbuf *p);
/** Function prototype for netif status- or link-callback functions. */
typedef void (*netif_status_callback_fn)(struct netif *netif);
/** Function prototype for netif address change-callback functions. */
typedef void (*netif_ipchange_callback_fn)( struct netif *netif, const ip4_addr_t *new_ip );

#if LWIP_DHCP || LWIP_AUTOIP || LWIP_IGMP || LWIP_IPV6_MLD || (LWIP_NUM_NETIF_CLIENT_DATA > 0)
u8_t netif_alloc_client_data_id(void);
#endif /* LWIP_DHCP || LWIP_AUTOIP || (LWIP_NUM_NETIF_CLIENT_DATA > 0) */


/** The list of network interfaces. */
extern struct netif *netif_list;
/** The default network interface. */
extern struct netif *netif_default;

/* Private static API fucntion used in netif.c*/
void netif_issue_reports(struct netif* netif, u8_t report_type);
#if LWIP_IPV6
err_t netif_null_output_ip6(struct netif *netif, struct pbuf *p, const ip6_addr_t *ipaddr);
#endif /* LWIP_IPV6 */

#if LWIP_HAVE_LOOPIF
#if LWIP_IPV4
err_t netif_loop_output_ipv4(struct netif *netif, struct pbuf *p, const ip4_addr_t* addr);
#endif
#if LWIP_IPV6
err_t netif_loop_output_ipv6(struct netif *netif, struct pbuf *p, const ip6_addr_t* addr);
#endif

err_t netif_loopif_init(struct netif *netif);
#endif


typedef void  (*netif_issue_reports_fp_t)(struct netif* netif, u8_t report_type);
#if LWIP_IPV6
typedef err_t (*netif_null_output_ip6_fp_t)(struct netif *netif, struct pbuf *p, const ip6_addr_t *ipaddr);
#endif /* LWIP_IPV6 */

#if LWIP_HAVE_LOOPIF
#if LWIP_IPV4
typedef err_t (*netif_loop_output_ipv4_fp_t)(struct netif *netif, struct pbuf *p, const ip4_addr_t* addr);
#endif
#if LWIP_IPV6
typedef err_t (*netif_loop_output_ipv6_fp_t)(struct netif *netif, struct pbuf *p, const ip6_addr_t* addr);
#endif

typedef err_t (*netif_loopif_init_fp_t)(struct netif *netif);
#endif

extern netif_issue_reports_fp_t            netif_issue_reports_adpt;
#if LWIP_IPV6
extern netif_null_output_ip6_fp_t          netif_null_output_ip6_adpt;
#endif
#if LWIP_HAVE_LOOPIF
#if LWIP_IPV4
extern netif_loop_output_ipv4_fp_t         netif_loop_output_ipv4_adpt;
#endif
#if LWIP_IPV6
extern netif_loop_output_ipv6_fp_t         netif_loop_output_ipv6_adpt;
#endif
extern netif_loopif_init_fp_t              netif_loopif_init_adpt;
#endif

/* Public API fucntion used in netif.c*/

#if LWIP_DHCP || LWIP_AUTOIP || LWIP_IGMP || LWIP_IPV6_MLD || (LWIP_NUM_NETIF_CLIENT_DATA > 0)
typedef u8_t (*netif_alloc_client_data_id_fp_t)(void);
#endif

typedef void (*netif_init_fp_t)(void);
typedef struct netif *(*netif_add_fp_t)(struct netif *netif,
#if LWIP_IPV4
                        const ip4_addr_t *ipaddr, const ip4_addr_t *netmask, const ip4_addr_t *gw,
#endif /* LWIP_IPV4 */
                        void *state, netif_init_fn init, netif_input_fn input);
#if LWIP_IPV4
typedef void (*netif_set_addr_fp_t)(struct netif *netif, const ip4_addr_t *ipaddr, const ip4_addr_t *netmask,
                    const ip4_addr_t *gw);
#endif /* LWIP_IPV4 */

typedef void (*netif_remove_fp_t)(struct netif * netif);
typedef struct netif *(*netif_find_fp_t)(const char *name);
typedef void (*netif_set_default_fp_t)(struct netif *netif);

#if LWIP_IPV4
typedef void (*netif_set_ipaddr_fp_t)(struct netif *netif, const ip4_addr_t *ipaddr);
typedef void (*netif_set_netmask_fp_t)(struct netif *netif, const ip4_addr_t *netmask);
typedef void (*netif_set_gw_fp_t)(struct netif *netif, const ip4_addr_t *gw);

#endif /* LWIP_IPV4 */

typedef void (*netif_set_up_fp_t)(struct netif *netif);
typedef void (*netif_set_down_fp_t)(struct netif *netif);


/** @ingroup netif
 * Ask if an interface is up
 */
#if LWIP_NETIF_STATUS_CALLBACK
typedef void (*netif_set_status_callback_fp_t)(struct netif *netif, netif_status_callback_fn status_callback);
#endif /* LWIP_NETIF_STATUS_CALLBACK */
#if LWIP_NETIF_IP_CHANGE_CALLBACK
typedef void (*netif_set_ipchange_callback_fp_t)(struct netif *netif, netif_ipchange_callback_fn ipchange_callback);
#endif
#if LWIP_NETIF_REMOVE_CALLBACK
typedef void (*netif_set_remove_callback_fp_t)(struct netif *netif, netif_status_callback_fn remove_callback);
#endif /* LWIP_NETIF_REMOVE_CALLBACK */

typedef void (*netif_set_link_up_fp_t)(struct netif *netif);
typedef void (*netif_set_link_down_fp_t)(struct netif *netif);

#if LWIP_NETIF_LINK_CALLBACK
typedef void (*netif_set_link_callback_fp_t)(struct netif *netif, netif_status_callback_fn link_callback);
#endif /* LWIP_NETIF_LINK_CALLBACK */

#if ENABLE_LOOPBACK
typedef err_t (*netif_loop_output_fp_t)(struct netif *netif, struct pbuf *p);
typedef void  (*netif_poll_fp_t)(struct netif *netif);
#if !LWIP_NETIF_LOOPBACK_MULTITHREADING
typedef void  (*netif_poll_all_fp_t)(void);
#endif /* !LWIP_NETIF_LOOPBACK_MULTITHREADING */
#endif /* ENABLE_LOOPBACK */

typedef err_t (*netif_input_fp_t)(struct pbuf *p, struct netif *inp);

#if LWIP_IPV6
typedef void  (*netif_ip6_addr_set_fp_t)(struct netif *netif, s8_t addr_idx, const ip6_addr_t *addr6);
typedef void  (*netif_ip6_addr_set_parts_fp_t)(struct netif *netif, s8_t addr_idx, u32_t i0, u32_t i1, u32_t i2, u32_t i3);
typedef void  (*netif_ip6_addr_set_state_fp_t)(struct netif* netif, s8_t addr_idx, u8_t state);
typedef s8_t  (*netif_get_ip6_addr_match_fp_t)(struct netif *netif, const ip6_addr_t *ip6addr);
typedef void  (*netif_create_ip6_linklocal_address_fp_t)(struct netif *netif, u8_t from_mac_48bit);
typedef err_t (*netif_add_ip6_address_fp_t)(struct netif *netif, const ip6_addr_t *ip6addr, s8_t *chosen_idx);
#endif /* LWIP_IPV6 */

#if LWIP_DHCP || LWIP_AUTOIP || LWIP_IGMP || LWIP_IPV6_MLD || (LWIP_NUM_NETIF_CLIENT_DATA > 0)
extern netif_alloc_client_data_id_fp_t           netif_alloc_client_data_id_adpt;
#endif
extern netif_init_fp_t                           netif_init_adpt;
extern netif_add_fp_t                            netif_add_adpt;
#if LWIP_IPV4
extern netif_set_addr_fp_t                       netif_set_addr_adpt;
#endif
extern netif_remove_fp_t                         netif_remove_adpt;
extern netif_find_fp_t                           netif_find_adpt;
extern netif_set_default_fp_t                    netif_set_default_adpt;
#if LWIP_IPV4
extern netif_set_ipaddr_fp_t                     netif_set_ipaddr_adpt;
extern netif_set_netmask_fp_t                    netif_set_netmask_adpt;
extern netif_set_gw_fp_t                         netif_set_gw_adpt;
#endif
extern netif_set_up_fp_t                         netif_set_up_adpt;
extern netif_set_down_fp_t                       netif_set_down_adpt;
#if LWIP_NETIF_STATUS_CALLBACK
extern netif_set_status_callback_fp_t            netif_set_status_callback_adpt;
#endif  /* LWIP_NETIF_LINK_CALLBACK */
#if LWIP_NETIF_REMOVE_CALLBACK
extern netif_set_remove_callback_fp_t            netif_set_remove_callback_adpt;
#endif /* LWIP_NETIF_REMOVE_CALLBACK */
extern netif_set_link_up_fp_t                    netif_set_link_up_adpt;
extern netif_set_link_down_fp_t                  netif_set_link_down_adpt;
#if LWIP_NETIF_LINK_CALLBACK
extern netif_set_link_callback_fp_t              netif_set_link_callback_adpt;
#endif /* LWIP_NETIF_LINK_CALLBACK */

#if ENABLE_LOOPBACK
extern netif_loop_output_fp_t                    netif_loop_output_adpt;
extern netif_poll_fp_t                           netif_poll_adpt;
#if !LWIP_NETIF_LOOPBACK_MULTITHREADING
extern netif_poll_all_fp_t                       netif_poll_all_adpt;
#endif /* !LWIP_NETIF_LOOPBACK_MULTITHREADING */
#endif /* ENABLE_LOOPBACK */
extern netif_input_fp_t                          netif_input_adpt;
#if LWIP_IPV6
extern netif_ip6_addr_set_fp_t                   netif_ip6_addr_set_adpt;
extern netif_ip6_addr_set_parts_fp_t             netif_ip6_addr_set_parts_adpt;
extern netif_ip6_addr_set_state_fp_t             netif_ip6_addr_set_state_adpt;
extern netif_get_ip6_addr_match_fp_t             netif_get_ip6_addr_match_adpt;
extern netif_create_ip6_linklocal_address_fp_t   netif_create_ip6_linklocal_address_adpt;
extern netif_add_ip6_address_fp_t                netif_add_ip6_address_adpt;
#endif



#ifdef __cplusplus
}
#endif

#endif /* #if defined(LWIP_ROMBUILD) */

#endif /* LWIP_HDR_NETIF_IF_H */
