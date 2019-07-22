/**
 * @file
 * ICMP API
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
#ifndef LWIP_HDR_ICMP_IF_H
#define LWIP_HDR_ICMP_IF_H

#include "lwip/opt.h"
#include "lwip/pbuf.h"
#include "lwip/ip_addr.h"
#include "lwip/netif.h"
#include "lwip/prot/icmp.h"

#if LWIP_IPV6 && LWIP_ICMP6
#include "lwip/icmp6.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(LWIP_ROMBUILD)

#if LWIP_IPV4 && LWIP_ICMP /* don't build if not configured for use in lwipopts.h */

void icmp_send_response(struct pbuf *p, u8_t type, u8_t code); //static private function


typedef void (*icmp_input_fp_t)(struct pbuf *p, struct netif *inp);
typedef void (*icmp_dest_unreach_fp_t)(struct pbuf *p, enum icmp_dur_type t);
typedef void (*icmp_time_exceeded_fp_t)(struct pbuf *p, enum icmp_te_type t);
typedef void (*icmp_send_response_fp_t)(struct pbuf *p, u8_t type, u8_t code);

extern icmp_input_fp_t icmp_input_adpt;
extern icmp_dest_unreach_fp_t icmp_dest_unreach_adpt;
extern icmp_time_exceeded_fp_t icmp_time_exceeded_adpt;
extern icmp_send_response_fp_t icmp_send_response_adpt;

#endif /* LWIP_IPV4 && LWIP_ICMP */

#endif //#if defined(LWIP_ROMBUILD)

#ifdef __cplusplus
}
#endif

#endif /* LWIP_HDR_ICMP_IF_H */
