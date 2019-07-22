/**
 * @file
 * Multicast listener discovery
 *
 * @defgroup mld6 MLD6
 * @ingroup ip6
 * Multicast listener discovery for IPv6. Aims to be compliant with RFC 2710.
 * No support for MLDv2.\n
 * To be called from TCPIP thread
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

/* Based on igmp.c implementation of igmp v2 protocol */

#include "lwip/opt.h"

#if LWIP_IPV6 && LWIP_IPV6_MLD  /* don't build if not configured for use in lwipopts.h */

#include "lwip/mld6.h"
#include "lwip/prot/mld6.h"
#include "lwip/icmp6.h"
#include "lwip/ip6.h"
#include "lwip/ip6_addr.h"
#include "lwip/ip.h"
#include "lwip/inet_chksum.h"
#include "lwip/pbuf.h"
#include "lwip/netif.h"
#include "lwip/memp.h"
#include "lwip/stats.h"

#include <string.h>


RET_DATA mld6_new_group_fp_t              mld6_new_group_adpt;
RET_DATA mld6_remove_group_fp_t           mld6_remove_group_adpt;
RET_DATA mld6_delayed_report_fp_t         mld6_delayed_report_adpt;
RET_DATA mld6_send_fp_t                   mld6_send_adpt;

RET_DATA mld6_stop_fp_t                   mld6_stop_adpt;
RET_DATA mld6_report_groups_fp_t          mld6_report_groups_adpt;
RET_DATA mld6_tmr_fp_t                    mld6_tmr_adpt;
RET_DATA mld6_lookfor_group_fp_t          mld6_lookfor_group_adpt;
RET_DATA mld6_input_fp_t                  mld6_input_adpt;
RET_DATA mld6_joingroup_fp_t              mld6_joingroup_adpt;
RET_DATA mld6_joingroup_netif_fp_t        mld6_joingroup_netif_adpt;
RET_DATA mld6_leavegroup_fp_t             mld6_leavegroup_adpt;
RET_DATA mld6_leavegroup_netif_fp_t       mld6_leavegroup_netif_adpt;


void lwip_load_interface_mld6(void)
{
    //private
    mld6_new_group_adpt           = LWIP_ROMFN(mld6_new_group);
    mld6_remove_group_adpt        = LWIP_ROMFN(mld6_remove_group);
    mld6_delayed_report_adpt      = LWIP_ROMFN(mld6_delayed_report);
    mld6_send_adpt                = LWIP_ROMFN(mld6_send);
    //public
    mld6_stop_adpt                = LWIP_ROMFN(mld6_stop);
    mld6_report_groups_adpt       = LWIP_ROMFN(mld6_report_groups);
    mld6_tmr_adpt                 = LWIP_ROMFN(mld6_tmr);
    mld6_lookfor_group_adpt       = LWIP_ROMFN(mld6_lookfor_group);
    mld6_input_adpt               = LWIP_ROMFN(mld6_input);
    mld6_joingroup_adpt           = LWIP_ROMFN(mld6_joingroup);
    mld6_joingroup_netif_adpt     = LWIP_ROMFN(mld6_joingroup_netif);
    mld6_leavegroup_adpt          = LWIP_ROMFN(mld6_leavegroup);
    mld6_leavegroup_netif_adpt    = LWIP_ROMFN(mld6_leavegroup_netif);
}

/**
 * Stop MLD processing on interface
 *
 * @param netif network interface on which stop MLD processing
 */
err_t
mld6_stop(struct netif *netif)
{
    return mld6_stop_adpt(netif);
}

/**
 * Report MLD memberships for this interface
 *
 * @param netif network interface on which report MLD memberships
 */
void
mld6_report_groups(struct netif *netif)
{
    mld6_report_groups_adpt(netif);
}

/**
 * Search for a group that is joined on a netif
 *
 * @param ifp the network interface for which to look
 * @param addr the group ipv6 address to search for
 * @return a struct mld_group* if the group has been found,
 *         NULL if the group wasn't found.
 */
struct mld_group *
mld6_lookfor_group(struct netif *ifp, const ip6_addr_t *addr)
{
    return mld6_lookfor_group_adpt(ifp, addr);
}


/**
 * create a new group
 *
 * @param ifp the network interface for which to create
 * @param addr the new group ipv6
 * @return a struct mld_group*,
 *         NULL on memory error.
 */
struct mld_group *
mld6_new_group(struct netif *ifp, const ip6_addr_t *addr)
{
    return mld6_new_group_adpt(ifp, addr);
}

/**
 * Remove a group from the mld_group_list, but do not free it yet
 *
 * @param group the group to remove
 * @return ERR_OK if group was removed from the list, an err_t otherwise
 */
err_t
mld6_remove_group(struct netif *netif, struct mld_group *group)
{
    return mld6_remove_group_adpt(netif, group);
}


/**
 * Process an input MLD message. Called by icmp6_input.
 *
 * @param p the mld packet, p->payload pointing to the icmpv6 header
 * @param inp the netif on which this packet was received
 */
void
mld6_input(struct pbuf *p, struct netif *inp)
{
    mld6_input_adpt(p, inp);
}

/**
 * @ingroup mld6
 * Join a group on a network interface.
 *
 * @param srcaddr ipv6 address of the network interface which should
 *                join a new group. If IP6_ADDR_ANY, join on all netifs
 * @param groupaddr the ipv6 address of the group to join
 * @return ERR_OK if group was joined on the netif(s), an err_t otherwise
 */
err_t
mld6_joingroup(const ip6_addr_t *srcaddr, const ip6_addr_t *groupaddr)
{
    return mld6_joingroup_adpt(srcaddr, groupaddr);
}

/**
 * @ingroup mld6
 * Join a group on a network interface.
 *
 * @param netif the network interface which should join a new group.
 * @param groupaddr the ipv6 address of the group to join
 * @return ERR_OK if group was joined on the netif, an err_t otherwise
 */
err_t
mld6_joingroup_netif(struct netif *netif, const ip6_addr_t *groupaddr)
{
    return mld6_joingroup_netif_adpt(netif, groupaddr);
}

/**
 * @ingroup mld6
 * Leave a group on a network interface.
 *
 * @param srcaddr ipv6 address of the network interface which should
 *                leave the group. If IP6_ISANY, leave on all netifs
 * @param groupaddr the ipv6 address of the group to leave
 * @return ERR_OK if group was left on the netif(s), an err_t otherwise
 */
err_t
mld6_leavegroup(const ip6_addr_t *srcaddr, const ip6_addr_t *groupaddr)
{
    return mld6_leavegroup_adpt(srcaddr, groupaddr);
}

/**
 * @ingroup mld6
 * Leave a group on a network interface.
 *
 * @param netif the network interface which should leave the group.
 * @param groupaddr the ipv6 address of the group to leave
 * @return ERR_OK if group was left on the netif, an err_t otherwise
 */
err_t
mld6_leavegroup_netif(struct netif *netif, const ip6_addr_t *groupaddr)
{
    return mld6_leavegroup_netif_adpt(netif, groupaddr);
}


/**
 * Periodic timer for mld processing. Must be called every
 * MLD6_TMR_INTERVAL milliseconds (100).
 *
 * When a delaying member expires, a membership report is sent.
 */
void
mld6_tmr(void)
{
    mld6_tmr_adpt();
}

/**
 * Schedule a delayed membership report for a group
 *
 * @param group the mld_group for which "delaying" membership report
 *              should be sent
 * @param maxresp the max resp delay provided in the query
 */
void
mld6_delayed_report(struct mld_group *group, u16_t maxresp)
{
    mld6_delayed_report_adpt(group, maxresp);
}

/**
 * Send a MLD message (report or done).
 *
 * An IPv6 hop-by-hop options header with a router alert option
 * is prepended.
 *
 * @param group the group to report or quit
 * @param type ICMP6_TYPE_MLR (report) or ICMP6_TYPE_MLD (done)
 */
void
mld6_send(struct netif *netif, struct mld_group *group, u8_t type)
{
    mld6_send_adpt(netif, group, type);
}

#endif /* LWIP_IPV6 */
