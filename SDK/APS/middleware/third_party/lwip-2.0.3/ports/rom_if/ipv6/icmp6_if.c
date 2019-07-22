/**
 * @file
 *
 * IPv6 version of ICMP, as per RFC 4443.
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

#include "lwip/opt.h"

#if LWIP_ICMP6 && LWIP_IPV6 /* don't build if not configured for use in lwipopts.h */

#include "lwip/icmp6.h"
#include "lwip/prot/icmp6.h"
#include "lwip/ip6.h"
#include "lwip/ip6_addr.h"
#include "lwip/inet_chksum.h"
#include "lwip/pbuf.h"
#include "lwip/netif.h"
#include "lwip/nd6.h"
#include "lwip/mld6.h"
#include "lwip/ip.h"
#include "lwip/stats.h"

#include <string.h>


RET_DATA icmp6_send_response_fp_t     icmp6_send_response_adpt;
RET_DATA icmp6_input_fp_t             icmp6_input_adpt;
RET_DATA icmp6_dest_unreach_fp_t      icmp6_dest_unreach_adpt;
RET_DATA icmp6_packet_too_big_fp_t    icmp6_packet_too_big_adpt;
RET_DATA icmp6_time_exceeded_fp_t     icmp6_time_exceeded_adpt;
RET_DATA icmp6_param_problem_fp_t     icmp6_param_problem_adpt;

void lwip_load_interface_icmp6(void)
{
    icmp6_send_response_adpt    = LWIP_ROMFN(icmp6_send_response);
    icmp6_input_adpt            = LWIP_ROMFN(icmp6_input);
    icmp6_dest_unreach_adpt     = LWIP_ROMFN(icmp6_dest_unreach);
    icmp6_packet_too_big_adpt   = LWIP_ROMFN(icmp6_packet_too_big);
    icmp6_time_exceeded_adpt    = LWIP_ROMFN(icmp6_time_exceeded);
    icmp6_param_problem_adpt    = LWIP_ROMFN(icmp6_param_problem);
}


/**
 * Process an input ICMPv6 message. Called by ip6_input.
 *
 * Will generate a reply for echo requests. Other messages are forwarded
 * to nd6_input, or mld6_input.
 *
 * @param p the mld packet, p->payload pointing to the icmpv6 header
 * @param inp the netif on which this packet was received
 */
void
icmp6_input(struct pbuf *p, struct netif *inp)
{
    icmp6_input_adpt(p, inp);
}


/**
 * Send an icmpv6 'destination unreachable' packet.
 *
 * @param p the input packet for which the 'unreachable' should be sent,
 *          p->payload pointing to the IPv6 header
 * @param c ICMPv6 code for the unreachable type
 */
void
icmp6_dest_unreach(struct pbuf *p, enum icmp6_dur_code c)
{
    icmp6_dest_unreach_adpt(p, c);
}

/**
 * Send an icmpv6 'packet too big' packet.
 *
 * @param p the input packet for which the 'packet too big' should be sent,
 *          p->payload pointing to the IPv6 header
 * @param mtu the maximum mtu that we can accept
 */
void
icmp6_packet_too_big(struct pbuf *p, u32_t mtu)
{
    icmp6_packet_too_big_adpt(p, mtu);
}

/**
 * Send an icmpv6 'time exceeded' packet.
 *
 * @param p the input packet for which the 'unreachable' should be sent,
 *          p->payload pointing to the IPv6 header
 * @param c ICMPv6 code for the time exceeded type
 */
void
icmp6_time_exceeded(struct pbuf *p, enum icmp6_te_code c)
{
    icmp6_time_exceeded_adpt(p, c);
}

/**
 * Send an icmpv6 'parameter problem' packet.
 *
 * @param p the input packet for which the 'param problem' should be sent,
 *          p->payload pointing to the IP header
 * @param c ICMPv6 code for the param problem type
 * @param pointer the pointer to the byte where the parameter is found
 */
void
icmp6_param_problem(struct pbuf *p, enum icmp6_pp_code c, u32_t pointer)
{
    icmp6_param_problem_adpt(p, c, pointer);
}

/**
 * Send an ICMPv6 packet in response to an incoming packet.
 *
 * @param p the input packet for which the response should be sent,
 *          p->payload pointing to the IPv6 header
 * @param code Code of the ICMPv6 header
 * @param data Additional 32-bit parameter in the ICMPv6 header
 * @param type Type of the ICMPv6 header
 */
void
icmp6_send_response(struct pbuf *p, u8_t code, u32_t data, u8_t type)
{
    icmp6_send_response_adpt(p, code, data, type);
}

#endif /* LWIP_ICMP6 && LWIP_IPV6 */
