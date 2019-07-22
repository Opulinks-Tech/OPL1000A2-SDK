/**
 * @file
 * ICMP - Internet Control Message Protocol
 *
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

/* Some ICMP messages should be passed to the transport protocols. This
   is not implemented. */

#if LWIP_IPV4 && LWIP_ICMP /* don't build if not configured for use in lwipopts.h */

#if defined(LWIP_ROMBUILD)

#include "lwip/icmp.h"
#include "lwip/inet_chksum.h"
#include "lwip/ip.h"
#include "lwip/def.h"
#include "lwip/stats.h"

#include <string.h>

#include "ipv4/icmp_if.h"

#if LWIP_IPV4 && LWIP_ICMP
RET_DATA icmp_input_fp_t         icmp_input_adpt;
RET_DATA icmp_dest_unreach_fp_t  icmp_dest_unreach_adpt;
#if IP_FORWARD || IP_REASSEMBLY
RET_DATA icmp_time_exceeded_fp_t icmp_time_exceeded_adpt;
#endif
RET_DATA icmp_send_response_fp_t icmp_send_response_adpt;
#endif

void lwip_load_interface_icmp(void)
{
    icmp_input_adpt         = LWIP_ROMFN(icmp_input);
    icmp_dest_unreach_adpt  = LWIP_ROMFN(icmp_dest_unreach);
#if IP_FORWARD || IP_REASSEMBLY
    icmp_time_exceeded_adpt = LWIP_ROMFN(icmp_time_exceeded);
#endif
    icmp_send_response_adpt = LWIP_ROMFN(icmp_send_response);
}

/**
 * Processes ICMP input packets, called from ip_input().
 *
 * Currently only processes icmp echo requests and sends
 * out the echo response.
 *
 * @param p the icmp echo request packet, p->payload pointing to the icmp header
 * @param inp the netif on which this packet was received
 */
void
icmp_input(struct pbuf *p, struct netif *inp)
{
    icmp_input_adpt(p, inp);
}

/**
 * Send an icmp 'destination unreachable' packet, called from ip_input() if
 * the transport layer protocol is unknown and from udp_input() if the local
 * port is not bound.
 *
 * @param p the input packet for which the 'unreachable' should be sent,
 *          p->payload pointing to the IP header
 * @param t type of the 'unreachable' packet
 */
void
icmp_dest_unreach(struct pbuf *p, enum icmp_dur_type t)
{
    icmp_dest_unreach_adpt(p, t);
}

#if IP_FORWARD || IP_REASSEMBLY
/**
 * Send a 'time exceeded' packet, called from ip_forward() if TTL is 0.
 *
 * @param p the input packet for which the 'time exceeded' should be sent,
 *          p->payload pointing to the IP header
 * @param t type of the 'time exceeded' packet
 */
void
icmp_time_exceeded(struct pbuf *p, enum icmp_te_type t)
{
    icmp_time_exceeded_adpt(p, t);
}

#endif /* IP_FORWARD || IP_REASSEMBLY */

/**
 * Send an icmp packet in response to an incoming packet.
 *
 * @param p the input packet for which the 'unreachable' should be sent,
 *          p->payload pointing to the IP header
 * @param type Type of the ICMP header
 * @param code Code of the ICMP header
 */
void
icmp_send_response(struct pbuf *p, u8_t type, u8_t code)
{
    icmp_send_response_adpt(p, type, code);
}

#endif //#if defined(LWIP_ROMBUILD)

#endif /* LWIP_IPV4 && LWIP_ICMP */
