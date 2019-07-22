/**
 * @file
 * User Datagram Protocol module\n
 * The code for the User Datagram Protocol UDP & UDPLite (RFC 3828).\n
 * See also @ref udp_raw
 *
 * @defgroup udp_raw UDP
 * @ingroup callbackstyle_api
 * User Datagram Protocol module\n
 * @see @ref raw_api and @ref netconn
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

/* @todo Check the use of '(struct udp_pcb).chksum_len_rx'!
 */

#include "lwip/opt.h"

#if defined(LWIP_ROMBUILD)

#if LWIP_UDP /* don't build if not configured for use in lwipopts.h */

#include "lwip/udp.h"
#include "lwip/def.h"
#include "lwip/memp.h"
#include "lwip/inet_chksum.h"
#include "lwip/ip_addr.h"
#include "lwip/ip6.h"
#include "lwip/ip6_addr.h"
#include "lwip/netif.h"
#include "lwip/icmp.h"
#include "lwip/icmp6.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "lwip/dhcp.h"

#include <string.h>

/*-------------------------------------------------------------------------------------
 * Definitions of interface function pointer
 *------------------------------------------------------------------------------------*/
RET_DATA udp_new_port_fp_t              udp_new_port_adpt;
RET_DATA udp_input_local_match_fp_t     udp_input_local_match_adpt;
RET_DATA udp_new_fp_t                   udp_new_adpt;
RET_DATA udp_new_ip_type_fp_t           udp_new_ip_type_adpt;
RET_DATA udp_remove_fp_t                udp_remove_adpt;
RET_DATA udp_bind_fp_t                  udp_bind_adpt;
RET_DATA udp_connect_fp_t               udp_connect_adpt;
RET_DATA udp_disconnect_fp_t            udp_disconnect_adpt;
RET_DATA udp_recv_fp_t                  udp_recv_adpt;
RET_DATA udp_sendto_if_fp_t             udp_sendto_if_adpt;
RET_DATA udp_sendto_if_src_fp_t         udp_sendto_if_src_adpt;
RET_DATA udp_sendto_fp_t                udp_sendto_adpt;
RET_DATA udp_send_fp_t                  udp_send_adpt;
#if LWIP_CHECKSUM_ON_COPY && CHECKSUM_GEN_UDP
RET_DATA udp_sendto_if_chksum_fp_t      udp_sendto_if_chksum_adpt;
RET_DATA udp_sendto_chksum_fp_t         udp_sendto_chksum_adpt;
RET_DATA udp_send_chksum_fp_t           udp_send_chksum_adpt;
RET_DATA udp_sendto_if_src_chksum_fp_t  udp_sendto_if_src_chksum_adpt;
#endif /* LWIP_CHECKSUM_ON_COPY && CHECKSUM_GEN_UDP */

RET_DATA udp_input_fp_t                 udp_input_adpt;
RET_DATA udp_init_fp_t                  udp_init_adpt;

#if UDP_DEBUG
RET_DATA udp_debug_print_fp_t           udp_debug_print_adpt;
#endif
RET_DATA udp_netif_ip_addr_changed_fp_t udp_netif_ip_addr_changed_adpt;



/*-------------------------------------------------------------------------------------
 * Interface assignment
 *------------------------------------------------------------------------------------*/
void lwip_load_interface_udp(void)
{
    /* last local UDP port */
    udp_port = UDP_LOCAL_PORT_RANGE_START;

    /* The list of UDP PCBs */
    udp_pcbs = NULL;


    //private interface
    udp_new_port_adpt                   = LWIP_ROMFN(udp_new_port);
    udp_input_local_match_adpt          = LWIP_ROMFN(udp_input_local_match);

    //public interface
    udp_new_adpt                        = LWIP_ROMFN(udp_new);
    udp_new_ip_type_adpt                = LWIP_ROMFN(udp_new_ip_type);
    udp_remove_adpt                     = LWIP_ROMFN(udp_remove);
    udp_bind_adpt                       = LWIP_ROMFN(udp_bind);
    udp_connect_adpt                    = LWIP_ROMFN(udp_connect);
    udp_disconnect_adpt                 = LWIP_ROMFN(udp_disconnect);
    udp_recv_adpt                       = LWIP_ROMFN(udp_recv);
    udp_sendto_if_adpt                  = LWIP_ROMFN(udp_sendto_if);
    udp_sendto_if_src_adpt              = LWIP_ROMFN(udp_sendto_if_src);
    udp_sendto_adpt                     = LWIP_ROMFN(udp_sendto);
    udp_send_adpt                       = LWIP_ROMFN(udp_send);
#if LWIP_CHECKSUM_ON_COPY && CHECKSUM_GEN_UDP
    udp_sendto_if_chksum_adpt           = LWIP_ROMFN(udp_sendto_if_chksum);
    udp_sendto_chksum_adpt              = LWIP_ROMFN(udp_sendto_chksum);
    udp_send_chksum_adpt                = LWIP_ROMFN(udp_send_chksum);
    udp_sendto_if_src_chksum_adpt       = LWIP_ROMFN(udp_sendto_if_src_chksum);
#endif /* LWIP_CHECKSUM_ON_COPY && CHECKSUM_GEN_UDP */

    udp_input_adpt                      = LWIP_ROMFN(udp_input);
    udp_init_adpt                       = LWIP_ROMFN(udp_init);

#if UDP_DEBUG
    udp_debug_print_adpt                = LWIP_ROMFN(udp_debug_print);
#endif
    udp_netif_ip_addr_changed_adpt      = LWIP_ROMFN(udp_netif_ip_addr_changed);
}


/**
 * Initialize this module.
 */
void
udp_init(void)
{
    udp_init_adpt();
}

/**
 * Allocate a new local UDP port.
 *
 * @return a new (free) local UDP port number
 */
u16_t
udp_new_port(void)
{
    return udp_new_port_adpt();
}

/** Common code to see if the current input packet matches the pcb
 * (current input packet is accessed via ip(4/6)_current_* macros)
 *
 * @param pcb pcb to check
 * @param inp network interface on which the datagram was received (only used for IPv4)
 * @param broadcast 1 if his is an IPv4 broadcast (global or subnet-only), 0 otherwise (only used for IPv4)
 * @return 1 on match, 0 otherwise
 */
u8_t
udp_input_local_match(struct udp_pcb *pcb, struct netif *inp, u8_t broadcast)
{
    return udp_input_local_match_adpt(pcb, inp, broadcast);
}

/**
 * Process an incoming UDP datagram.
 *
 * Given an incoming UDP datagram (as a chain of pbufs) this function
 * finds a corresponding UDP PCB and hands over the pbuf to the pcbs
 * recv function. If no pcb is found or the datagram is incorrect, the
 * pbuf is freed.
 *
 * @param p pbuf to be demultiplexed to a UDP PCB (p->payload pointing to the UDP header)
 * @param inp network interface on which the datagram was received.
 *
 */
void
udp_input(struct pbuf *p, struct netif *inp)
{
    udp_input_adpt(p, inp);
}

/**
 * @ingroup udp_raw
 * Send data using UDP.
 *
 * @param pcb UDP PCB used to send the data.
 * @param p chain of pbuf's to be sent.
 *
 * The datagram will be sent to the current remote_ip & remote_port
 * stored in pcb. If the pcb is not bound to a port, it will
 * automatically be bound to a random port.
 *
 * @return lwIP error code.
 * - ERR_OK. Successful. No error occurred.
 * - ERR_MEM. Out of memory.
 * - ERR_RTE. Could not find route to destination address.
 * - ERR_VAL. No PCB or PCB is dual-stack
 * - More errors could be returned by lower protocol layers.
 *
 * @see udp_disconnect() udp_sendto()
 */
err_t
udp_send(struct udp_pcb *pcb, struct pbuf *p)
{
    return udp_send_adpt(pcb, p);
}

#if LWIP_CHECKSUM_ON_COPY && CHECKSUM_GEN_UDP
/** @ingroup udp_raw
 * Same as udp_send() but with checksum
 */
err_t
udp_send_chksum(struct udp_pcb *pcb, struct pbuf *p,
                u8_t have_chksum, u16_t chksum)
{
    return udp_send_chksum_adpt(pcb, p, have_chksum, chksum);
}
#endif /* LWIP_CHECKSUM_ON_COPY && CHECKSUM_GEN_UDP */

/**
 * @ingroup udp_raw
 * Send data to a specified address using UDP.
 *
 * @param pcb UDP PCB used to send the data.
 * @param p chain of pbuf's to be sent.
 * @param dst_ip Destination IP address.
 * @param dst_port Destination UDP port.
 *
 * dst_ip & dst_port are expected to be in the same byte order as in the pcb.
 *
 * If the PCB already has a remote address association, it will
 * be restored after the data is sent.
 *
 * @return lwIP error code (@see udp_send for possible error codes)
 *
 * @see udp_disconnect() udp_send()
 */
err_t
udp_sendto(struct udp_pcb *pcb, struct pbuf *p,
  const ip_addr_t *dst_ip, u16_t dst_port)
{
    return udp_sendto_adpt(pcb, p, dst_ip, dst_port);
}

#if LWIP_CHECKSUM_ON_COPY && CHECKSUM_GEN_UDP
/** @ingroup udp_raw
 * Same as udp_sendto(), but with checksum */
err_t
udp_sendto_chksum(struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *dst_ip,
                  u16_t dst_port, u8_t have_chksum, u16_t chksum)
{
    return udp_sendto_chksum_adpt(pcb, p, dst_ip, dst_port, have_chksum, chksum);
}
#endif /* LWIP_CHECKSUM_ON_COPY && CHECKSUM_GEN_UDP */

/**
 * @ingroup udp_raw
 * Send data to a specified address using UDP.
 * The netif used for sending can be specified.
 *
 * This function exists mainly for DHCP, to be able to send UDP packets
 * on a netif that is still down.
 *
 * @param pcb UDP PCB used to send the data.
 * @param p chain of pbuf's to be sent.
 * @param dst_ip Destination IP address.
 * @param dst_port Destination UDP port.
 * @param netif the netif used for sending.
 *
 * dst_ip & dst_port are expected to be in the same byte order as in the pcb.
 *
 * @return lwIP error code (@see udp_send for possible error codes)
 *
 * @see udp_disconnect() udp_send()
 */
err_t
udp_sendto_if(struct udp_pcb *pcb, struct pbuf *p,
  const ip_addr_t *dst_ip, u16_t dst_port, struct netif *netif)
{
    return udp_sendto_if_adpt(pcb, p, dst_ip, dst_port, netif);
}

#if LWIP_CHECKSUM_ON_COPY && CHECKSUM_GEN_UDP
/** Same as udp_sendto_if(), but with checksum */
err_t
udp_sendto_if_chksum(struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *dst_ip,
                     u16_t dst_port, struct netif *netif, u8_t have_chksum,
                     u16_t chksum)
{
    return udp_sendto_if_chksum_adpt(pcb, p, dst_ip, dst_port, netif, have_chksum, chksum);
}
#endif /* LWIP_CHECKSUM_ON_COPY && CHECKSUM_GEN_UDP */

/** @ingroup udp_raw
 * Same as @ref udp_sendto_if, but with source address */
err_t
udp_sendto_if_src(struct udp_pcb *pcb, struct pbuf *p,
  const ip_addr_t *dst_ip, u16_t dst_port, struct netif *netif, const ip_addr_t *src_ip)
{
    return udp_sendto_if_src_adpt(pcb, p, dst_ip, dst_port, netif, src_ip);
}


#if LWIP_CHECKSUM_ON_COPY && CHECKSUM_GEN_UDP
/** Same as udp_sendto_if_src(), but with checksum */
err_t
udp_sendto_if_src_chksum(struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *dst_ip,
                     u16_t dst_port, struct netif *netif, u8_t have_chksum,
                     u16_t chksum, const ip_addr_t *src_ip)
{
    return udp_sendto_if_src_chksum_adpt(pcb, p, dst_ip, dst_port, netif, have_chksum, chksum, src_ip);
}
#endif /* LWIP_CHECKSUM_ON_COPY && CHECKSUM_GEN_UDP */

/**
 * @ingroup udp_raw
 * Bind an UDP PCB.
 *
 * @param pcb UDP PCB to be bound with a local address ipaddr and port.
 * @param ipaddr local IP address to bind with. Use IP4_ADDR_ANY to
 * bind to all local interfaces.
 * @param port local UDP port to bind with. Use 0 to automatically bind
 * to a random port between UDP_LOCAL_PORT_RANGE_START and
 * UDP_LOCAL_PORT_RANGE_END.
 *
 * ipaddr & port are expected to be in the same byte order as in the pcb.
 *
 * @return lwIP error code.
 * - ERR_OK. Successful. No error occurred.
 * - ERR_USE. The specified ipaddr and port are already bound to by
 * another UDP PCB.
 *
 * @see udp_disconnect()
 */
err_t
udp_bind(struct udp_pcb *pcb, const ip_addr_t *ipaddr, u16_t port)
{
    return udp_bind_adpt(pcb, ipaddr, port);
}

/**
 * @ingroup udp_raw
 * Connect an UDP PCB.
 *
 * This will associate the UDP PCB with the remote address.
 *
 * @param pcb UDP PCB to be connected with remote address ipaddr and port.
 * @param ipaddr remote IP address to connect with.
 * @param port remote UDP port to connect with.
 *
 * @return lwIP error code
 *
 * ipaddr & port are expected to be in the same byte order as in the pcb.
 *
 * The udp pcb is bound to a random local port if not already bound.
 *
 * @see udp_disconnect()
 */
err_t
udp_connect(struct udp_pcb *pcb, const ip_addr_t *ipaddr, u16_t port)
{
    return udp_connect_adpt(pcb, ipaddr, port);
}

/**
 * @ingroup udp_raw
 * Disconnect a UDP PCB
 *
 * @param pcb the udp pcb to disconnect.
 */
void
udp_disconnect(struct udp_pcb *pcb)
{
    udp_disconnect_adpt(pcb);
}

/**
 * @ingroup udp_raw
 * Set a receive callback for a UDP PCB
 *
 * This callback will be called when receiving a datagram for the pcb.
 *
 * @param pcb the pcb for which to set the recv callback
 * @param recv function pointer of the callback function
 * @param recv_arg additional argument to pass to the callback function
 */
void
udp_recv(struct udp_pcb *pcb, udp_recv_fn recv, void *recv_arg)
{
    udp_recv_adpt(pcb, recv, recv_arg);
}

/**
 * @ingroup udp_raw
 * Remove an UDP PCB.
 *
 * @param pcb UDP PCB to be removed. The PCB is removed from the list of
 * UDP PCB's and the data structure is freed from memory.
 *
 * @see udp_new()
 */
void
udp_remove(struct udp_pcb *pcb)
{
    udp_remove_adpt(pcb);
}

/**
 * @ingroup udp_raw
 * Create a UDP PCB.
 *
 * @return The UDP PCB which was created. NULL if the PCB data structure
 * could not be allocated.
 *
 * @see udp_remove()
 */
struct udp_pcb *
udp_new(void)
{
    return udp_new_adpt();
}

/**
 * @ingroup udp_raw
 * Create a UDP PCB for specific IP type.
 *
 * @param type IP address type, see @ref lwip_ip_addr_type definitions.
 * If you want to listen to IPv4 and IPv6 (dual-stack) packets,
 * supply @ref IPADDR_TYPE_ANY as argument and bind to @ref IP_ANY_TYPE.
 * @return The UDP PCB which was created. NULL if the PCB data structure
 * could not be allocated.
 *
 * @see udp_remove()
 */
struct udp_pcb *
udp_new_ip_type(u8_t type)
{
    return udp_new_ip_type_adpt(type);
}

/** This function is called from netif.c when address is changed
 *
 * @param old_addr IP address of the netif before change
 * @param new_addr IP address of the netif after change
 */
void udp_netif_ip_addr_changed(const ip_addr_t* old_addr, const ip_addr_t* new_addr)
{
    udp_netif_ip_addr_changed_adpt(old_addr, new_addr);
}

#if UDP_DEBUG
/**
 * Print UDP header information for debug purposes.
 *
 * @param udphdr pointer to the udp header in memory.
 */
void
udp_debug_print(struct udp_hdr *udphdr)
{
    udp_debug_print_adpt(udphdr);
}
#endif /* UDP_DEBUG */

#endif /* LWIP_UDP */

#endif /* #if defined(LWIP_ROMBUILD) */

