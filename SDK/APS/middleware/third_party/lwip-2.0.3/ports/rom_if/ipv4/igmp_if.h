/**
 * @file
 * IGMP API
 */

/*
 * Copyright (c) 2002 CITEL Technologies Ltd.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of CITEL Technologies Ltd nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY CITEL TECHNOLOGIES AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL CITEL TECHNOLOGIES OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is a contribution to the lwIP TCP/IP stack.
 * The Swedish Institute of Computer Science and Adam Dunkels
 * are specifically granted permission to redistribute this
 * source code.
*/

#ifndef LWIP_HDR_IGMP_IF_H
#define LWIP_HDR_IGMP_IF_H

#include "lwip/opt.h"
#include "lwip/ip_addr.h"
#include "lwip/netif.h"
#include "lwip/pbuf.h"

#if defined(LWIP_ROMBUILD)

#if LWIP_IPV4 && LWIP_IGMP /* don't build if not configured for use in lwipopts.h */

#ifdef __cplusplus
extern "C" {
#endif


/**
 * igmp group structure - there is
 * a list of groups for each interface
 * these should really be linked from the interface, but
 * if we keep them separate we will not affect the lwip original code
 * too much
 *
 * There will be a group for the all systems group address but this
 * will not run the state machine as it is used to kick off reports
 * from all the other groups
 */
struct igmp_group;

/*  Prototypes */

/* Private API fucntion used in igmp.c*/
struct igmp_group *igmp_lookup_group(struct netif *ifp, const ip4_addr_t *addr);
err_t  igmp_remove_group(struct netif* netif, struct igmp_group *group);
void   igmp_timeout(struct netif *netif, struct igmp_group *group);
void   igmp_start_timer(struct igmp_group *group, u8_t max_time);
void   igmp_delaying_member(struct igmp_group *group, u8_t maxresp);
err_t  igmp_ip_output_if(struct pbuf *p, const ip4_addr_t *src, const ip4_addr_t *dest, struct netif *netif);
void   igmp_send(struct netif *netif, struct igmp_group *group, u8_t type);

typedef struct igmp_group *(*igmp_lookup_group_fp_t)(struct netif *ifp, const ip4_addr_t *addr);
typedef err_t  (*igmp_remove_group_fp_t)(struct netif* netif, struct igmp_group *group);
typedef void   (*igmp_timeout_fp_t)(struct netif *netif, struct igmp_group *group);
typedef void   (*igmp_start_timer_fp_t)(struct igmp_group *group, u8_t max_time);
typedef void   (*igmp_delaying_member_fp_t)(struct igmp_group *group, u8_t maxresp);
typedef err_t  (*igmp_ip_output_if_fp_t)(struct pbuf *p, const ip4_addr_t *src, const ip4_addr_t *dest, struct netif *netif);
typedef void   (*igmp_send_fp_t)(struct netif *netif, struct igmp_group *group, u8_t type);

extern igmp_lookup_group_fp_t       igmp_lookup_group_adpt;
extern igmp_remove_group_fp_t       igmp_remove_group_adpt;
extern igmp_timeout_fp_t            igmp_timeout_adpt;
extern igmp_start_timer_fp_t        igmp_start_timer_adpt;
extern igmp_delaying_member_fp_t    igmp_delaying_member_adpt;
extern igmp_ip_output_if_fp_t       igmp_ip_output_if_adpt;
extern igmp_send_fp_t               igmp_send_adpt;


/*Public API fucntion */
typedef void   (*igmp_init_fp_t)(void);
typedef err_t  (*igmp_start_fp_t)(struct netif *netif);
typedef err_t  (*igmp_stop_fp_t)(struct netif *netif);
typedef void   (*igmp_report_groups_fp_t)(struct netif *netif);
typedef struct igmp_group *(*igmp_lookfor_group_fp_t)(struct netif *ifp, const ip4_addr_t *addr);
typedef void   (*igmp_input_fp_t)(struct pbuf *p, struct netif *inp, const ip4_addr_t *dest);
typedef err_t  (*igmp_joingroup_fp_t)(const ip4_addr_t *ifaddr, const ip4_addr_t *groupaddr);
typedef err_t  (*igmp_joingroup_netif_fp_t)(struct netif *netif, const ip4_addr_t *groupaddr);
typedef err_t  (*igmp_leavegroup_fp_t)(const ip4_addr_t *ifaddr, const ip4_addr_t *groupaddr);
typedef err_t  (*igmp_leavegroup_netif_fp_t)(struct netif *netif, const ip4_addr_t *groupaddr);
typedef void   (*igmp_tmr_fp_t)(void);

extern igmp_init_fp_t               igmp_init_adpt;
extern igmp_start_fp_t              igmp_start_adpt;
extern igmp_stop_fp_t               igmp_stop_adpt;
extern igmp_report_groups_fp_t      igmp_report_groups_adpt;
extern igmp_lookfor_group_fp_t      igmp_lookfor_group_adpt;
extern igmp_input_fp_t              igmp_input_adpt;
extern igmp_joingroup_fp_t          igmp_joingroup_adpt;
extern igmp_joingroup_netif_fp_t    igmp_joingroup_netif_adpt;
extern igmp_leavegroup_fp_t         igmp_leavegroup_adpt;
extern igmp_leavegroup_netif_fp_t   igmp_leavegroup_netif_adpt;
extern igmp_tmr_fp_t                igmp_tmr_adpt;

/** @ingroup igmp
 * Get list head of IGMP groups for netif.
 * Note: The allsystems group IP is contained in the list as first entry.
 * @see @ref netif_set_igmp_mac_filter()
 */
#define netif_igmp_data(netif) ((struct igmp_group *)netif_get_client_data(netif, LWIP_NETIF_CLIENT_DATA_INDEX_IGMP))

#ifdef __cplusplus
}
#endif

#endif /* LWIP_IPV4 && LWIP_IGMP */

#endif //#if defined(LWIP_ROMBUILD)


#endif /* LWIP_HDR_IGMP_IF_H */
