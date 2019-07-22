/**
 * @file
 * IGMP - Internet Group Management Protocol
 *
 * @defgroup igmp IGMP
 * @ingroup ip4
 * To be called from TCPIP thread
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

/*-------------------------------------------------------------
Note 1)
Although the rfc requires V1 AND V2 capability
we will only support v2 since now V1 is very old (August 1989)
V1 can be added if required

a debug print and statistic have been implemented to
show this up.
-------------------------------------------------------------
-------------------------------------------------------------
Note 2)
A query for a specific group address (as opposed to ALLHOSTS)
has now been implemented as I am unsure if it is required

a debug print and statistic have been implemented to
show this up.
-------------------------------------------------------------
-------------------------------------------------------------
Note 3)
The router alert rfc 2113 is implemented in outgoing packets
but not checked rigorously incoming
-------------------------------------------------------------
Steve Reynolds
------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * RFC 988  - Host extensions for IP multicasting                         - V0
 * RFC 1054 - Host extensions for IP multicasting                         -
 * RFC 1112 - Host extensions for IP multicasting                         - V1
 * RFC 2236 - Internet Group Management Protocol, Version 2               - V2  <- this code is based on this RFC (it's the "de facto" standard)
 * RFC 3376 - Internet Group Management Protocol, Version 3               - V3
 * RFC 4604 - Using Internet Group Management Protocol Version 3...       - V3+
 * RFC 2113 - IP Router Alert Option                                      -
 *----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Includes
 *----------------------------------------------------------------------------*/

#include "lwip/opt.h"

#if defined(LWIP_ROMBUILD)

#if LWIP_IPV4 && LWIP_IGMP /* don't build if not configured for use in lwipopts.h */

#include "lwip/igmp.h"
#include "lwip/debug.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/ip.h"
#include "lwip/inet_chksum.h"
#include "lwip/netif.h"
#include "lwip/stats.h"
#include "lwip/prot/igmp.h"

#include "string.h"

/*
static struct igmp_group *igmp_lookup_group(struct netif *ifp, const ip4_addr_t *addr);
static err_t  igmp_remove_group(struct netif* netif, struct igmp_group *group);
static void   igmp_timeout(struct netif *netif, struct igmp_group *group);
static void   igmp_start_timer(struct igmp_group *group, u8_t max_time);
static void   igmp_delaying_member(struct igmp_group *group, u8_t maxresp);
static err_t  igmp_ip_output_if(struct pbuf *p, const ip4_addr_t *src, const ip4_addr_t *dest, struct netif *netif);
static void   igmp_send(struct netif *netif, struct igmp_group *group, u8_t type);

static ip4_addr_t     allsystems;
static ip4_addr_t     allrouters;

*/

RET_DATA igmp_lookup_group_fp_t      igmp_lookup_group_adpt;
RET_DATA igmp_remove_group_fp_t      igmp_remove_group_adpt;
RET_DATA igmp_timeout_fp_t           igmp_timeout_adpt;
RET_DATA igmp_start_timer_fp_t       igmp_start_timer_adpt;
RET_DATA igmp_delaying_member_fp_t   igmp_delaying_member_adpt;
RET_DATA igmp_ip_output_if_fp_t      igmp_ip_output_if_adpt;
RET_DATA igmp_send_fp_t              igmp_send_adpt;

RET_DATA igmp_init_fp_t              igmp_init_adpt;
RET_DATA igmp_start_fp_t             igmp_start_adpt;
RET_DATA igmp_stop_fp_t              igmp_stop_adpt;
RET_DATA igmp_report_groups_fp_t     igmp_report_groups_adpt;
RET_DATA igmp_lookfor_group_fp_t     igmp_lookfor_group_adpt;
RET_DATA igmp_input_fp_t             igmp_input_adpt;
RET_DATA igmp_joingroup_fp_t         igmp_joingroup_adpt;
RET_DATA igmp_joingroup_netif_fp_t   igmp_joingroup_netif_adpt;
RET_DATA igmp_leavegroup_fp_t        igmp_leavegroup_adpt;
RET_DATA igmp_leavegroup_netif_fp_t  igmp_leavegroup_netif_adpt;
RET_DATA igmp_tmr_fp_t               igmp_tmr_adpt;


void lwip_load_interface_igmp(void)
{
    ip4_addr_set_zero(&allsystems);
    ip4_addr_set_zero(&allrouters);

    igmp_lookup_group_adpt          = LWIP_ROMFN(igmp_lookup_group);
    igmp_remove_group_adpt          = LWIP_ROMFN(igmp_remove_group);
    igmp_timeout_adpt               = LWIP_ROMFN(igmp_timeout);
    igmp_start_timer_adpt           = LWIP_ROMFN(igmp_start_timer);
    igmp_delaying_member_adpt       = LWIP_ROMFN(igmp_delaying_member);
    igmp_ip_output_if_adpt          = LWIP_ROMFN(igmp_ip_output_if);
    igmp_send_adpt                  = LWIP_ROMFN(igmp_send);

    igmp_init_adpt                  = LWIP_ROMFN(igmp_init);
    igmp_start_adpt                 = LWIP_ROMFN(igmp_start);
    igmp_stop_adpt                  = LWIP_ROMFN(igmp_stop);
    igmp_report_groups_adpt         = LWIP_ROMFN(igmp_report_groups);
    igmp_lookfor_group_adpt         = LWIP_ROMFN(igmp_lookfor_group);
    igmp_input_adpt                 = LWIP_ROMFN(igmp_input);
    igmp_joingroup_adpt             = LWIP_ROMFN(igmp_joingroup);
    igmp_joingroup_netif_adpt       = LWIP_ROMFN(igmp_joingroup_netif);
    igmp_leavegroup_adpt            = LWIP_ROMFN(igmp_leavegroup);
    igmp_leavegroup_netif_adpt      = LWIP_ROMFN(igmp_leavegroup_netif);
    igmp_tmr_adpt                   = LWIP_ROMFN(igmp_tmr);
}


/**
 * Initialize the IGMP module
 */
void
igmp_init(void)
{
    igmp_init_adpt();
}

/**
 * Start IGMP processing on interface
 *
 * @param netif network interface on which start IGMP processing
 */
err_t
igmp_start(struct netif *netif)
{
    return igmp_start_adpt(netif);
}

/**
 * Stop IGMP processing on interface
 *
 * @param netif network interface on which stop IGMP processing
 */
err_t
igmp_stop(struct netif *netif)
{
    return igmp_stop_adpt(netif);
}

/**
 * Report IGMP memberships for this interface
 *
 * @param netif network interface on which report IGMP memberships
 */
void
igmp_report_groups(struct netif *netif)
{
    igmp_report_groups_adpt(netif);
}

/**
 * Search for a group in the global igmp_group_list
 *
 * @param ifp the network interface for which to look
 * @param addr the group ip address to search for
 * @return a struct igmp_group* if the group has been found,
 *         NULL if the group wasn't found.
 */
struct igmp_group *
igmp_lookfor_group(struct netif *ifp, const ip4_addr_t *addr)
{
    return igmp_lookfor_group_adpt(ifp, addr);
}

/**
 * Search for a specific igmp group and create a new one if not found-
 *
 * @param ifp the network interface for which to look
 * @param addr the group ip address to search
 * @return a struct igmp_group*,
 *         NULL on memory error.
 */
static struct igmp_group *
igmp_lookup_group(struct netif *ifp, const ip4_addr_t *addr)
{
    return igmp_lookup_group_adpt(ifp, addr);
}

/**
 * Remove a group in the global igmp_group_list, but don't free it yet
 *
 * @param group the group to remove from the global igmp_group_list
 * @return ERR_OK if group was removed from the list, an err_t otherwise
 */
static err_t
igmp_remove_group(struct netif* netif, struct igmp_group *group)
{
    return igmp_remove_group_adpt(netif, group);
}

/**
 * Called from ip_input() if a new IGMP packet is received.
 *
 * @param p received igmp packet, p->payload pointing to the igmp header
 * @param inp network interface on which the packet was received
 * @param dest destination ip address of the igmp packet
 */
void
igmp_input(struct pbuf *p, struct netif *inp, const ip4_addr_t *dest)
{
    igmp_input_adpt(p, inp, dest);
}

/**
 * @ingroup igmp
 * Join a group on one network interface.
 *
 * @param ifaddr ip address of the network interface which should join a new group
 * @param groupaddr the ip address of the group which to join
 * @return ERR_OK if group was joined on the netif(s), an err_t otherwise
 */
err_t
igmp_joingroup(const ip4_addr_t *ifaddr, const ip4_addr_t *groupaddr)
{
    return igmp_joingroup_adpt(ifaddr, groupaddr);
}

/**
 * @ingroup igmp
 * Join a group on one network interface.
 *
 * @param netif the network interface which should join a new group
 * @param groupaddr the ip address of the group which to join
 * @return ERR_OK if group was joined on the netif, an err_t otherwise
 */
err_t
igmp_joingroup_netif(struct netif *netif, const ip4_addr_t *groupaddr)
{
    return igmp_joingroup_netif_adpt(netif, groupaddr);
}

/**
 * @ingroup igmp
 * Leave a group on one network interface.
 *
 * @param ifaddr ip address of the network interface which should leave a group
 * @param groupaddr the ip address of the group which to leave
 * @return ERR_OK if group was left on the netif(s), an err_t otherwise
 */
err_t
igmp_leavegroup(const ip4_addr_t *ifaddr, const ip4_addr_t *groupaddr)
{
    return igmp_leavegroup_adpt(ifaddr, groupaddr);
}

/**
 * @ingroup igmp
 * Leave a group on one network interface.
 *
 * @param netif the network interface which should leave a group
 * @param groupaddr the ip address of the group which to leave
 * @return ERR_OK if group was left on the netif, an err_t otherwise
 */
err_t
igmp_leavegroup_netif(struct netif *netif, const ip4_addr_t *groupaddr)
{
    return igmp_leavegroup_netif_adpt(netif, groupaddr);
}

/**
 * The igmp timer function (both for NO_SYS=1 and =0)
 * Should be called every IGMP_TMR_INTERVAL milliseconds (100 ms is default).
 */
void
igmp_tmr(void)
{
    igmp_tmr_adpt();
}

/**
 * Called if a timeout for one group is reached.
 * Sends a report for this group.
 *
 * @param group an igmp_group for which a timeout is reached
 */
void
igmp_timeout(struct netif *netif, struct igmp_group *group)
{
    igmp_timeout_adpt(netif, group);
}

/**
 * Start a timer for an igmp group
 *
 * @param group the igmp_group for which to start a timer
 * @param max_time the time in multiples of IGMP_TMR_INTERVAL (decrease with
 *        every call to igmp_tmr())
 */
void
igmp_start_timer(struct igmp_group *group, u8_t max_time)
{
    igmp_start_timer_adpt(group, max_time);
}

/**
 * Delaying membership report for a group if necessary
 *
 * @param group the igmp_group for which "delaying" membership report
 * @param maxresp query delay
 */
void
igmp_delaying_member(struct igmp_group *group, u8_t maxresp)
{
    igmp_delaying_member_adpt(group, maxresp);
}


/**
 * Sends an IP packet on a network interface. This function constructs the IP header
 * and calculates the IP header checksum. If the source IP address is NULL,
 * the IP address of the outgoing network interface is filled in as source address.
 *
 * @param p the packet to send (p->payload points to the data, e.g. next
            protocol header; if dest == LWIP_IP_HDRINCL, p already includes an
            IP header and p->payload points to that IP header)
 * @param src the source IP address to send from (if src == IP4_ADDR_ANY, the
 *         IP  address of the netif used to send is used as source address)
 * @param dest the destination IP address to send the packet to
 * @param netif the netif on which to send this packet
 * @return ERR_OK if the packet was sent OK
 *         ERR_BUF if p doesn't have enough space for IP/LINK headers
 *         returns errors returned by netif->output
 */
err_t
igmp_ip_output_if(struct pbuf *p, const ip4_addr_t *src, const ip4_addr_t *dest, struct netif *netif)
{
    return igmp_ip_output_if_adpt(p, src, dest, netif);
}

/**
 * Send an igmp packet to a specific group.
 *
 * @param group the group to which to send the packet
 * @param type the type of igmp packet to send
 */
void
igmp_send(struct netif *netif, struct igmp_group *group, u8_t type)
{
    igmp_send_adpt(netif, group, type);
}

#endif /* LWIP_IPV4 && LWIP_IGMP */

#endif /* #if defined(LWIP_ROMBUILD)*/
