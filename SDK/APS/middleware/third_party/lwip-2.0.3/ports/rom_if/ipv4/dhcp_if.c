/**
 * @file
 * Dynamic Host Configuration Protocol client
 *
 * @defgroup dhcp4 DHCPv4
 * @ingroup ip4
 * DHCP (IPv4) related functions
 * This is a DHCP client for the lwIP TCP/IP stack. It aims to conform
 * with RFC 2131 and RFC 2132.
 *
 * @todo:
 * - Support for interfaces other than Ethernet (SLIP, PPP, ...)
 *
 * Options:
 * @ref DHCP_COARSE_TIMER_SECS (recommended 60 which is a minute)
 * @ref DHCP_FINE_TIMER_MSECS (recommended 500 which equals TCP coarse timer)
 *
 * dhcp_start() starts a DHCP client instance which
 * configures the interface by obtaining an IP address lease and maintaining it.
 *
 * Use dhcp_release() to end the lease and use dhcp_stop()
 * to remove the DHCP client.
 *
 * @see netifapi_dhcp4
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
 * The Swedish Institute of Computer Science and Adam Dunkels
 * are specifically granted permission to redistribute this
 * source code.
 *
 * Author: Leon Woestenberg <leon.woestenberg@gmx.net>
 *
 */

#include "lwip/opt.h"

#if defined(LWIP_ROMBUILD)

#if LWIP_IPV4 && LWIP_DHCP /* don't build if not configured for use in lwipopts.h */

#include "lwip/stats.h"
#include "lwip/mem.h"
#include "lwip/udp.h"
#include "lwip/ip_addr.h"
#include "lwip/netif.h"
#include "lwip/def.h"
#include "lwip/dhcp.h"
#include "lwip/autoip.h"
#include "lwip/dns.h"
#include "lwip/etharp.h"
#include "lwip/prot/dhcp.h"

#include <string.h>


RET_DATA dhcp_inc_pcb_refcount_fp_t   dhcp_inc_pcb_refcount_adpt;
RET_DATA dhcp_dec_pcb_refcount_fp_t   dhcp_dec_pcb_refcount_adpt;
RET_DATA dhcp_discover_fp_t           dhcp_discover_adpt;
RET_DATA dhcp_select_fp_t             dhcp_select_adpt;
RET_DATA dhcp_bind_fp_t               dhcp_bind_adpt;
#if DHCP_DOES_ARP_CHECK
RET_DATA dhcp_decline_fp_t            dhcp_decline_adpt;
RET_DATA dhcp_check_fp_t              dhcp_check_adpt;
#endif
RET_DATA dhcp_rebind_fp_t             dhcp_rebind_adpt;
RET_DATA dhcp_reboot_fp_t             dhcp_reboot_adpt;
RET_DATA dhcp_set_state_fp_t          dhcp_set_state_adpt;
RET_DATA dhcp_handle_offer_fp_t       dhcp_handle_offer_adpt;
RET_DATA dhcp_handle_ack_fp_t         dhcp_handle_ack_adpt;
RET_DATA dhcp_handle_nak_fp_t         dhcp_handle_nak_adpt;
RET_DATA dhcp_parse_reply_fp_t        dhcp_parse_reply_adpt;
RET_DATA dhcp_recv_fp_t               dhcp_recv_adpt;

RET_DATA dhcp_timeout_fp_t            dhcp_timeout_adpt;
RET_DATA dhcp_t1_timeout_fp_t         dhcp_t1_timeout_adpt;
RET_DATA dhcp_t2_timeout_fp_t         dhcp_t2_timeout_adpt;

RET_DATA dhcp_create_msg_fp_t         dhcp_create_msg_adpt;
RET_DATA dhcp_delete_msg_fp_t         dhcp_delete_msg_adpt;
RET_DATA dhcp_option_fp_t             dhcp_option_adpt;

RET_DATA dhcp_option_byte_fp_t        dhcp_option_byte_adpt;
RET_DATA dhcp_option_short_fp_t       dhcp_option_short_adpt;
RET_DATA dhcp_option_long_fp_t        dhcp_option_long_adpt;
#if LWIP_NETIF_HOSTNAME
RET_DATA dhcp_option_hostname_fp_t    dhcp_option_hostname_adpt;
#endif
RET_DATA dhcp_option_trailer_fp_t     dhcp_option_trailer_adpt;


RET_DATA dhcp_set_struct_fp_t         dhcp_set_struct_adpt;
RET_DATA dhcp_cleanup_fp_t            dhcp_cleanup_adpt;
RET_DATA dhcp_set_cb_fp_t             dhcp_set_cb_adpt;
RET_DATA dhcp_start_fp_t              dhcp_start_adpt;
RET_DATA dhcp_renew_fp_t              dhcp_renew_adpt;
RET_DATA dhcp_release_fp_t            dhcp_release_adpt;
RET_DATA dhcp_stop_fp_t               dhcp_stop_adpt;
RET_DATA dhcp_inform_fp_t             dhcp_inform_adpt;
RET_DATA dhcp_network_changed_fp_t    dhcp_network_changed_adpt;
#if DHCP_DOES_ARP_CHECK
RET_DATA dhcp_arp_reply_fp_t          dhcp_arp_reply_adpt;
#endif
RET_DATA dhcp_supplied_address_fp_t   dhcp_supplied_address_adpt;
RET_DATA dhcp_coarse_tmr_fp_t         dhcp_coarse_tmr_adpt;
RET_DATA dhcp_fine_tmr_fp_t           dhcp_fine_tmr_adpt;


void lwip_load_interface_dhcp(void)
{
    /* Cold boot initialization for "zero_init" retention data */
    memset(dhcp_rx_options_val, 0, sizeof(dhcp_rx_options_val));
    memset(dhcp_rx_options_given, 0, sizeof(dhcp_rx_options_given));
    dhcp_pcb = NULL;
    dhcp_pcb_refcount = 0;
    dhcp_does_arp_check_flag = 1;

    dhcp_inc_pcb_refcount_adpt   = LWIP_ROMFN(dhcp_inc_pcb_refcount);
    dhcp_dec_pcb_refcount_adpt   = LWIP_ROMFN(dhcp_dec_pcb_refcount);
    dhcp_discover_adpt           = LWIP_ROMFN(dhcp_discover);
    dhcp_select_adpt             = LWIP_ROMFN(dhcp_select);
    dhcp_bind_adpt               = LWIP_ROMFN(dhcp_bind);
#if DHCP_DOES_ARP_CHECK
    dhcp_decline_adpt            = LWIP_ROMFN(dhcp_decline);
    dhcp_check_adpt              = LWIP_ROMFN(dhcp_check);
#endif
    dhcp_rebind_adpt             = LWIP_ROMFN(dhcp_rebind);
    dhcp_reboot_adpt             = LWIP_ROMFN(dhcp_reboot);
    dhcp_set_state_adpt          = LWIP_ROMFN(dhcp_set_state);
    dhcp_handle_offer_adpt       = LWIP_ROMFN(dhcp_handle_offer);
    dhcp_handle_ack_adpt         = LWIP_ROMFN(dhcp_handle_ack);
    dhcp_handle_nak_adpt         = LWIP_ROMFN(dhcp_handle_nak);
    dhcp_parse_reply_adpt        = LWIP_ROMFN(dhcp_parse_reply);

    dhcp_recv_adpt               = LWIP_ROMFN(dhcp_recv);

    dhcp_timeout_adpt            = LWIP_ROMFN(dhcp_timeout);
    dhcp_t1_timeout_adpt         = LWIP_ROMFN(dhcp_t1_timeout);
    dhcp_t2_timeout_adpt         = LWIP_ROMFN(dhcp_t2_timeout);

    dhcp_create_msg_adpt         = LWIP_ROMFN(dhcp_create_msg);
    dhcp_delete_msg_adpt         = LWIP_ROMFN(dhcp_delete_msg);
    dhcp_option_adpt             = LWIP_ROMFN(dhcp_option);

    dhcp_option_byte_adpt        = LWIP_ROMFN(dhcp_option_byte);
    dhcp_option_short_adpt       = LWIP_ROMFN(dhcp_option_short);
    dhcp_option_long_adpt        = LWIP_ROMFN(dhcp_option_long);
#if LWIP_NETIF_HOSTNAME
    dhcp_option_hostname_adpt    = LWIP_ROMFN(dhcp_option_hostname);
#endif
    dhcp_option_trailer_adpt     = LWIP_ROMFN(dhcp_option_trailer);


    dhcp_set_struct_adpt         = LWIP_ROMFN(dhcp_set_struct);
    dhcp_cleanup_adpt            = LWIP_ROMFN(dhcp_cleanup);
    dhcp_set_cb_adpt             = LWIP_ROMFN(dhcp_set_cb);
    dhcp_start_adpt              = LWIP_ROMFN(dhcp_start);
    dhcp_renew_adpt              = LWIP_ROMFN(dhcp_renew);
    dhcp_release_adpt            = LWIP_ROMFN(dhcp_release);
    dhcp_stop_adpt               = LWIP_ROMFN(dhcp_stop);
    dhcp_inform_adpt             = LWIP_ROMFN(dhcp_inform);
    dhcp_network_changed_adpt    = LWIP_ROMFN(dhcp_network_changed);
#if DHCP_DOES_ARP_CHECK
    dhcp_arp_reply_adpt          = LWIP_ROMFN(dhcp_arp_reply);
#endif
    dhcp_supplied_address_adpt   = LWIP_ROMFN(dhcp_supplied_address);
    dhcp_coarse_tmr_adpt         = LWIP_ROMFN(dhcp_coarse_tmr);
    dhcp_fine_tmr_adpt           = LWIP_ROMFN(dhcp_fine_tmr);
}


/** Ensure DHCP PCB is allocated and bound */
err_t
dhcp_inc_pcb_refcount(void)
{
    return dhcp_inc_pcb_refcount_adpt();
}

/** Free DHCP PCB if the last netif stops using it */
void
dhcp_dec_pcb_refcount(void)
{
    dhcp_dec_pcb_refcount_adpt();
}

/**
 * Back-off the DHCP client (because of a received NAK response).
 *
 * Back-off the DHCP client because of a received NAK. Receiving a
 * NAK means the client asked for something non-sensible, for
 * example when it tries to renew a lease obtained on another network.
 *
 * We clear any existing set IP address and restart DHCP negotiation
 * afresh (as per RFC2131 3.2.3).
 *
 * @param netif the netif under DHCP control
 */
void
dhcp_handle_nak(struct netif *netif)
{
    dhcp_handle_nak_adpt(netif);
}

#if DHCP_DOES_ARP_CHECK
/**
 * Checks if the offered IP address is already in use.
 *
 * It does so by sending an ARP request for the offered address and
 * entering CHECKING state. If no ARP reply is received within a small
 * interval, the address is assumed to be free for use by us.
 *
 * @param netif the netif under DHCP control
 */
void
dhcp_check(struct netif *netif)
{
    dhcp_check_adpt(netif);
}
#endif /* DHCP_DOES_ARP_CHECK */

/**
 * Remember the configuration offered by a DHCP server.
 *
 * @param netif the netif under DHCP control
 */
void
dhcp_handle_offer(struct netif *netif)
{
    dhcp_handle_offer_adpt(netif);
}

/**
 * Select a DHCP server offer out of all offers.
 *
 * Simply select the first offer received.
 *
 * @param netif the netif under DHCP control
 * @return lwIP specific error (see error.h)
 */
err_t
dhcp_select(struct netif *netif)
{
    return dhcp_select_adpt(netif);
}

/**
 * The DHCP timer that checks for lease renewal/rebind timeouts.
 * Must be called once a minute (see @ref DHCP_COARSE_TIMER_SECS).
 */
void
dhcp_coarse_tmr(void)
{
    dhcp_coarse_tmr_adpt();
}

/**
 * DHCP transaction timeout handling (this function must be called every 500ms,
 * see @ref DHCP_FINE_TIMER_MSECS).
 *
 * A DHCP server is expected to respond within a short period of time.
 * This timer checks whether an outstanding DHCP request is timed out.
 */
void
dhcp_fine_tmr(void)
{
    dhcp_fine_tmr_adpt();
}

/**
 * A DHCP negotiation transaction, or ARP request, has timed out.
 *
 * The timer that was started with the DHCP or ARP request has
 * timed out, indicating no response was received in time.
 *
 * @param netif the netif under DHCP control
 */
void
dhcp_timeout(struct netif *netif)
{
    dhcp_timeout_adpt(netif);
}

/**
 * The renewal period has timed out.
 *
 * @param netif the netif under DHCP control
 */
void
dhcp_t1_timeout(struct netif *netif)
{
    dhcp_t1_timeout_adpt(netif);
}

/**
 * The rebind period has timed out.
 *
 * @param netif the netif under DHCP control
 */
void
dhcp_t2_timeout(struct netif *netif)
{
    dhcp_t2_timeout_adpt(netif);
}

/**
 * Handle a DHCP ACK packet
 *
 * @param netif the netif under DHCP control
 */
void
dhcp_handle_ack(struct netif *netif)
{
    dhcp_handle_ack_adpt(netif);
}

/**
 * @ingroup dhcp4
 * Set a statically allocated struct dhcp to work with.
 * Using this prevents dhcp_start to allocate it using mem_malloc.
 *
 * @param netif the netif for which to set the struct dhcp
 * @param dhcp (uninitialised) dhcp struct allocated by the application
 */
void
dhcp_set_struct(struct netif *netif, struct dhcp *dhcp)
{
    dhcp_set_struct_adpt(netif, dhcp);
}

/**
 * @ingroup dhcp4
 * Removes a struct dhcp from a netif.
 *
 * ATTENTION: Only use this when not using dhcp_set_struct() to allocate the
 *            struct dhcp since the memory is passed back to the heap.
 *
 * @param netif the netif from which to remove the struct dhcp
 */
void dhcp_cleanup(struct netif *netif)
{
    dhcp_cleanup_adpt(netif);
}

/** Set callback for dhcp, reserved parameter for future use.
 *
 * @param netif the netif from which to remove the struct dhcp
 * @param cb    callback for dhcp
 */
void dhcp_set_cb(struct netif *netif, void (*cb)(struct netif*))
{
    dhcp_set_cb_adpt(netif, cb);
}

/**
 * @ingroup dhcp4
 * Start DHCP negotiation for a network interface.
 *
 * If no DHCP client instance was attached to this interface,
 * a new client is created first. If a DHCP client instance
 * was already present, it restarts negotiation.
 *
 * @param netif The lwIP network interface
 * @return lwIP error code
 * - ERR_OK - No error
 * - ERR_MEM - Out of memory
 */
err_t
dhcp_start(struct netif *netif)
{
    return dhcp_start_adpt(netif);
}

/**
 * @ingroup dhcp4
 * Inform a DHCP server of our manual configuration.
 *
 * This informs DHCP servers of our fixed IP address configuration
 * by sending an INFORM message. It does not involve DHCP address
 * configuration, it is just here to be nice to the network.
 *
 * @param netif The lwIP network interface
 */
void
dhcp_inform(struct netif *netif)
{
    dhcp_inform_adpt(netif);
}

/** Handle a possible change in the network configuration.
 *
 * This enters the REBOOTING state to verify that the currently bound
 * address is still valid.
 */
void
dhcp_network_changed(struct netif *netif)
{
    dhcp_network_changed_adpt(netif);
}

#if DHCP_DOES_ARP_CHECK
/**
 * Match an ARP reply with the offered IP address:
 * check whether the offered IP address is not in use using ARP
 *
 * @param netif the network interface on which the reply was received
 * @param addr The IP address we received a reply from
 */
void
dhcp_arp_reply(struct netif *netif, const ip4_addr_t *addr)
{
    dhcp_arp_reply_adpt(netif, addr);
}

/**
 * Decline an offered lease.
 *
 * Tell the DHCP server we do not accept the offered address.
 * One reason to decline the lease is when we find out the address
 * is already in use by another host (through ARP).
 *
 * @param netif the netif under DHCP control
 */
err_t
dhcp_decline(struct netif *netif)
{
    return dhcp_decline_adpt(netif);
}
#endif /* DHCP_DOES_ARP_CHECK */


/**
 * Start the DHCP process, discover a DHCP server.
 *
 * @param netif the netif under DHCP control
 */
err_t
dhcp_discover(struct netif *netif)
{
    return dhcp_discover_adpt(netif);
}


/**
 * Bind the interface to the offered IP address.
 *
 * @param netif network interface to bind to the offered address
 */
void
dhcp_bind(struct netif *netif)
{
    dhcp_bind_adpt(netif);
}

/**
 * @ingroup dhcp4
 * Renew an existing DHCP lease at the involved DHCP server.
 *
 * @param netif network interface which must renew its lease
 */
err_t
dhcp_renew(struct netif *netif)
{
    return dhcp_renew_adpt(netif);
}

/**
 * Rebind with a DHCP server for an existing DHCP lease.
 *
 * @param netif network interface which must rebind with a DHCP server
 */
err_t
dhcp_rebind(struct netif *netif)
{
    return dhcp_rebind_adpt(netif);
}

/**
 * Enter REBOOTING state to verify an existing lease
 *
 * @param netif network interface which must reboot
 */
err_t
dhcp_reboot(struct netif *netif)
{
    return dhcp_reboot_adpt(netif);
}


/**
 * @ingroup dhcp4
 * Release a DHCP lease (usually called before @ref dhcp_stop).
 *
 * @param netif network interface which must release its lease
 */
err_t
dhcp_release(struct netif *netif)
{
    return dhcp_release_adpt(netif);
}

/**
 * @ingroup dhcp4
 * Remove the DHCP client from the interface.
 *
 * @param netif The network interface to stop DHCP on
 */
void
dhcp_stop(struct netif *netif)
{
    dhcp_stop_adpt(netif);
}

/*
 * Set the DHCP state of a DHCP client.
 *
 * If the state changed, reset the number of tries.
 */
static void
dhcp_set_state(struct dhcp *dhcp, u8_t new_state)
{
    dhcp_set_state_adpt(dhcp, new_state);
}

/*
 * Concatenate an option type and length field to the outgoing
 * DHCP message.
 *
 */
void
dhcp_option(struct dhcp *dhcp, u8_t option_type, u8_t option_len)
{
    dhcp_option_adpt(dhcp, option_type, option_len);
}
/*
 * Concatenate a single byte to the outgoing DHCP message.
 *
 */
void
dhcp_option_byte(struct dhcp *dhcp, u8_t value)
{
    dhcp_option_byte_adpt(dhcp, value);
}

void
dhcp_option_short(struct dhcp *dhcp, u16_t value)
{
    dhcp_option_short_adpt(dhcp, value);
}

static void
dhcp_option_long(struct dhcp *dhcp, u32_t value)
{
    dhcp_option_long_adpt(dhcp, value);
}

#if LWIP_NETIF_HOSTNAME
void
dhcp_option_hostname(struct dhcp *dhcp, struct netif *netif)
{
    dhcp_option_hostname_adpt(dhcp, netif);

}
#endif /* LWIP_NETIF_HOSTNAME */

/**
 * Extract the DHCP message and the DHCP options.
 *
 * Extract the DHCP message and the DHCP options, each into a contiguous
 * piece of memory. As a DHCP message is variable sized by its options,
 * and also allows overriding some fields for options, the easy approach
 * is to first unfold the options into a contiguous piece of memory, and
 * use that further on.
 *
 */
err_t
dhcp_parse_reply(struct dhcp *dhcp, struct pbuf *p)
{
    return dhcp_parse_reply_adpt(dhcp, p);
}

/**
 * If an incoming DHCP message is in response to us, then trigger the state machine
 */
void
dhcp_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    dhcp_recv_adpt(arg, pcb, p, addr, port);
}

/**
 * Create a DHCP request, fill in common headers
 *
 * @param netif the netif under DHCP control
 * @param dhcp dhcp control struct
 * @param message_type message type of the request
 */
err_t
dhcp_create_msg(struct netif *netif, struct dhcp *dhcp, u8_t message_type)
{
    return dhcp_create_msg_adpt(netif, dhcp, message_type);
}

/**
 * Free previously allocated memory used to send a DHCP request.
 *
 * @param dhcp the dhcp struct to free the request from
 */
void
dhcp_delete_msg(struct dhcp *dhcp)
{
    dhcp_delete_msg_adpt(dhcp);
}

/**
 * Add a DHCP message trailer
 *
 * Adds the END option to the DHCP message, and if
 * necessary, up to three padding bytes.
 *
 * @param dhcp DHCP state structure
 */
void
dhcp_option_trailer(struct dhcp *dhcp)
{
    dhcp_option_trailer_adpt(dhcp);
}

/** check if DHCP supplied netif->ip_addr
 *
 * @param netif the netif to check
 * @return 1 if DHCP supplied netif->ip_addr (states BOUND or RENEWING),
 *         0 otherwise
 */
u8_t
dhcp_supplied_address(const struct netif *netif)
{
    return dhcp_supplied_address_adpt(netif);
}

#endif /* LWIP_IPV4 && LWIP_DHCP */

#endif //#if defined(LWIP_ROMBUILD)

