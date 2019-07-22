/**
 * @file
 * AutoIP Automatic LinkLocal IP Configuration
 *
 * This is a AutoIP implementation for the lwIP TCP/IP stack. It aims to conform
 * with RFC 3927.
 *
 * @defgroup autoip AUTOIP
 * @ingroup ip4
 * AUTOIP related functions
 * USAGE:
 *
 * define @ref LWIP_AUTOIP 1 in your lwipopts.h
 * Options:
 * AUTOIP_TMR_INTERVAL msecs,
 *   I recommend a value of 100. The value must divide 1000 with a remainder almost 0.
 *   Possible values are 1000, 500, 333, 250, 200, 166, 142, 125, 111, 100 ....
 *
 * Without DHCP:
 * - Call autoip_start() after netif_add().
 *
 * With DHCP:
 * - define @ref LWIP_DHCP_AUTOIP_COOP 1 in your lwipopts.h.
 * - Configure your DHCP Client.
 *
 * @see netifapi_autoip
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
 */

#include "lwip/opt.h"

#if LWIP_IPV4 && LWIP_AUTOIP /* don't build if not configured for use in lwipopts.h */

#include "lwip/mem.h"
/* #include "lwip/udp.h" */
#include "lwip/ip_addr.h"
#include "lwip/netif.h"
#include "lwip/autoip.h"
#include "lwip/etharp.h"
#include "lwip/prot/autoip.h"

#include <string.h>

/*-------------------------------------------------------------------------------------
 * Definitions of interface function pointer
 *------------------------------------------------------------------------------------*/
RET_DATA autoip_start_probing_fp_t           autoip_start_probing_adpt;
RET_DATA autoip_restart_fp_t                 autoip_restart_adpt;
RET_DATA autoip_handle_arp_conflict_fp_t     autoip_handle_arp_conflict_adpt;
RET_DATA autoip_create_addr_fp_t             autoip_create_addr_adpt;
RET_DATA autoip_arp_probe_fp_t               autoip_arp_probe_adpt;
RET_DATA autoip_arp_announce_fp_t            autoip_arp_announce_adpt;
RET_DATA autoip_bind_fp_t                    autoip_bind_adpt;

RET_DATA autoip_set_struct_fp_t              autoip_set_struct_adpt;
RET_DATA autoip_start_fp_t                   autoip_start_adpt;
RET_DATA autoip_stop_fp_t                    autoip_stop_adpt;
RET_DATA autoip_arp_reply_fp_t               autoip_arp_reply_adpt;
RET_DATA autoip_tmr_fp_t                     autoip_tmr_adpt;
RET_DATA autoip_network_changed_fp_t         autoip_network_changed_adpt;
RET_DATA autoip_supplied_address_fp_t        autoip_supplied_address_adpt;
RET_DATA autoip_accept_packet_fp_t           autoip_accept_packet_adpt;

/*-------------------------------------------------------------------------------------
 * Interface assignment
 *------------------------------------------------------------------------------------*/
void lwip_load_interface_autoip(void)
{
    autoip_start_probing_adpt           = LWIP_ROMFN(autoip_start_probing);
    autoip_restart_adpt                 = LWIP_ROMFN(autoip_restart);
    autoip_handle_arp_conflict_adpt     = LWIP_ROMFN(autoip_handle_arp_conflict);
    autoip_create_addr_adpt             = LWIP_ROMFN(autoip_create_addr);
    autoip_arp_probe_adpt               = LWIP_ROMFN(autoip_arp_probe);
    autoip_arp_announce_adpt            = LWIP_ROMFN(autoip_arp_announce);
    autoip_bind_adpt                    = LWIP_ROMFN(autoip_bind);

    autoip_set_struct_adpt              = LWIP_ROMFN(autoip_set_struct);
    autoip_start_adpt                   = LWIP_ROMFN(autoip_start);
    autoip_stop_adpt                    = LWIP_ROMFN(autoip_stop);
    autoip_arp_reply_adpt               = LWIP_ROMFN(autoip_arp_reply);
    autoip_tmr_adpt                     = LWIP_ROMFN(autoip_tmr);
    autoip_network_changed_adpt         = LWIP_ROMFN(autoip_network_changed);
    autoip_supplied_address_adpt        = LWIP_ROMFN(autoip_supplied_address);
    autoip_accept_packet_adpt           = LWIP_ROMFN(autoip_accept_packet);
}

/**
 * @ingroup autoip
 * Set a statically allocated struct autoip to work with.
 * Using this prevents autoip_start to allocate it using mem_malloc.
 *
 * @param netif the netif for which to set the struct autoip
 * @param autoip (uninitialised) autoip struct allocated by the application
 */
void
autoip_set_struct(struct netif *netif, struct autoip *autoip)
{
    autoip_set_struct_adpt(netif, autoip);
}


/** Restart AutoIP client and check the next address (conflict detected)
 *
 * @param netif The netif under AutoIP control
 */
void
autoip_restart(struct netif *netif)
{
    autoip_restart_adpt(netif);
}

/**
 * Handle a IP address conflict after an ARP conflict detection
 */
void
autoip_handle_arp_conflict(struct netif *netif)
{
    autoip_handle_arp_conflict_adpt(netif);
}

/**
 * Create an IP-Address out of range 169.254.1.0 to 169.254.254.255
 *
 * @param netif network interface on which create the IP-Address
 * @param ipaddr ip address to initialize
 */
void
autoip_create_addr(struct netif *netif, ip4_addr_t *ipaddr)
{
    autoip_create_addr_adpt(netif, ipaddr);
}

/**
 * Sends an ARP probe from a network interface
 *
 * @param netif network interface used to send the probe
 */
err_t
autoip_arp_probe(struct netif *netif)
{
    return autoip_arp_probe_adpt(netif);
}

/**
 * Sends an ARP announce from a network interface
 *
 * @param netif network interface used to send the announce
 */
err_t
autoip_arp_announce(struct netif *netif)
{
    return autoip_arp_announce_adpt(netif);
}

/**
 * Configure interface for use with current LL IP-Address
 *
 * @param netif network interface to configure with current LL IP-Address
 */
err_t
autoip_bind(struct netif *netif)
{
    return autoip_bind_adpt(netif);
}

/**
 * @ingroup autoip
 * Start AutoIP client
 *
 * @param netif network interface on which start the AutoIP client
 */
err_t
autoip_start(struct netif *netif)
{
    return autoip_start_adpt(netif);
}

void
autoip_start_probing(struct netif *netif)
{
    autoip_start_probing_adpt(netif);
}

/**
 * Handle a possible change in the network configuration.
 *
 * If there is an AutoIP address configured, take the interface down
 * and begin probing with the same address.
 */
void
autoip_network_changed(struct netif *netif)
{
    autoip_network_changed_adpt(netif);
}

/**
 * @ingroup autoip
 * Stop AutoIP client
 *
 * @param netif network interface on which stop the AutoIP client
 */
err_t
autoip_stop(struct netif *netif)
{
    return autoip_stop_adpt(netif);
}

/**
 * Has to be called in loop every AUTOIP_TMR_INTERVAL milliseconds
 */
void
autoip_tmr(void)
{
    autoip_tmr_adpt();
}

/**
 * Handles every incoming ARP Packet, called by etharp_input().
 *
 * @param netif network interface to use for autoip processing
 * @param hdr Incoming ARP packet
 */
void
autoip_arp_reply(struct netif *netif, struct etharp_hdr *hdr)
{
    autoip_arp_reply_adpt(netif, hdr);
}

/** check if AutoIP supplied netif->ip_addr
 *
 * @param netif the netif to check
 * @return 1 if AutoIP supplied netif->ip_addr (state BOUND or ANNOUNCING),
 *         0 otherwise
 */
u8_t
autoip_supplied_address(const struct netif *netif)
{
    return autoip_supplied_address_adpt(netif);
}

u8_t
autoip_accept_packet(struct netif *netif, const ip4_addr_t *addr)
{
    return autoip_accept_packet_adpt(netif, addr);
}

#endif /* LWIP_IPV4 && LWIP_AUTOIP */
