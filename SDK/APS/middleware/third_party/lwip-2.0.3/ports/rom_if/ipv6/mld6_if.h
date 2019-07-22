/**
 * @file
 *
 * Multicast listener discovery for IPv6. Aims to be compliant with RFC 2710.
 * No support for MLDv2.
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

#ifndef LWIP_HDR_MLD6_IF_H
#define LWIP_HDR_MLD6_IF_H

#include "lwip/opt.h"

#if defined(LWIP_ROMBUILD)

#if LWIP_IPV6_MLD && LWIP_IPV6  /* don't build if not configured for use in lwipopts.h */

#include "lwip/pbuf.h"
#include "lwip/netif.h"

#ifdef __cplusplus
extern "C" {
#endif

/** MLD group */
struct mld_group;


/* Forward declarations. */
/* Private static API fucntion used in mld6.c*/
struct mld_group *mld6_new_group(struct netif *ifp, const ip6_addr_t *addr);
err_t mld6_remove_group(struct netif *netif, struct mld_group *group);
void  mld6_delayed_report(struct mld_group *group, u16_t maxresp);
void  mld6_send(struct netif *netif, struct mld_group *group, u8_t type);

typedef struct mld_group *(*mld6_new_group_fp_t)(struct netif *ifp, const ip6_addr_t *addr);
typedef err_t (*mld6_remove_group_fp_t)(struct netif *netif, struct mld_group *group);
typedef void  (*mld6_delayed_report_fp_t)(struct mld_group *group, u16_t maxresp);
typedef void  (*mld6_send_fp_t)(struct netif *netif, struct mld_group *group, u8_t type);

extern mld6_new_group_fp_t              mld6_new_group_adpt;
extern mld6_remove_group_fp_t           mld6_remove_group_adpt;
extern mld6_delayed_report_fp_t         mld6_delayed_report_adpt;
extern mld6_send_fp_t                   mld6_send_adpt;



/* Publick API fucntion used in mld6.c*/
typedef err_t  (*mld6_stop_fp_t)(struct netif *netif);
typedef void   (*mld6_report_groups_fp_t)(struct netif *netif);
typedef void   (*mld6_tmr_fp_t)(void);
typedef struct mld_group *(*mld6_lookfor_group_fp_t)(struct netif *ifp, const ip6_addr_t *addr);
typedef void   (*mld6_input_fp_t)(struct pbuf *p, struct netif *inp);
typedef err_t  (*mld6_joingroup_fp_t)(const ip6_addr_t *srcaddr, const ip6_addr_t *groupaddr);
typedef err_t  (*mld6_joingroup_netif_fp_t)(struct netif *netif, const ip6_addr_t *groupaddr);
typedef err_t  (*mld6_leavegroup_fp_t)(const ip6_addr_t *srcaddr, const ip6_addr_t *groupaddr);
typedef err_t  (*mld6_leavegroup_netif_fp_t)(struct netif *netif, const ip6_addr_t *groupaddr);

extern mld6_stop_fp_t                   mld6_stop_adpt;
extern mld6_report_groups_fp_t          mld6_report_groups_adpt;
extern mld6_tmr_fp_t                    mld6_tmr_adpt;
extern mld6_lookfor_group_fp_t          mld6_lookfor_group_adpt;
extern mld6_input_fp_t                  mld6_input_adpt;
extern mld6_joingroup_fp_t              mld6_joingroup_adpt;
extern mld6_joingroup_netif_fp_t        mld6_joingroup_netif_adpt;
extern mld6_leavegroup_fp_t             mld6_leavegroup_adpt;
extern mld6_leavegroup_netif_fp_t       mld6_leavegroup_netif_adpt;

#ifdef __cplusplus
}
#endif

#endif /* LWIP_IPV6_MLD && LWIP_IPV6 */

#endif /* #if defined(LWIP_ROMBUILD) */

#endif /* LWIP_HDR_MLD6_IF_H */
