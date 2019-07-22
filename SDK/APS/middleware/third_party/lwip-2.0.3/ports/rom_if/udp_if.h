/**
 * @file
 * UDP API (to be used from TCPIP thread)\n
 * See also @ref udp_raw
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
#ifndef LWIP_HDR_UDP_IF_H
#define LWIP_HDR_UDP_IF_H

#include "lwip/opt.h"

#if defined(LWIP_ROMBUILD)


#if LWIP_UDP /* don't build if not configured for use in lwipopts.h */

#include "lwip/pbuf.h"
#include "lwip/netif.h"
#include "lwip/ip_addr.h"
#include "lwip/ip.h"
#include "lwip/ip6_addr.h"
#include "lwip/prot/udp.h"

#ifdef __cplusplus
extern "C" {
#endif

struct udp_pcb;

/* Private API fucntion used in udp.c*/
u16_t udp_new_port(void);
u8_t  udp_input_local_match(struct udp_pcb *pcb, struct netif *inp, u8_t broadcast);

typedef u16_t (*udp_new_port_fp_t)(void);
typedef u8_t  (*udp_input_local_match_fp_t)(struct udp_pcb *pcb, struct netif *inp, u8_t broadcast);

extern udp_new_port_fp_t            udp_new_port_adpt;
extern udp_input_local_match_fp_t   udp_input_local_match_adpt;


/** Function prototype for udp pcb receive callback functions
 * addr and port are in same byte order as in the pcb
 * The callback is responsible for freeing the pbuf
 * if it's not used any more.
 *
 * ATTENTION: Be aware that 'addr' might point into the pbuf 'p' so freeing this pbuf
 *            can make 'addr' invalid, too.
 *
 * @param arg user supplied argument (udp_pcb.recv_arg)
 * @param pcb the udp_pcb which received data
 * @param p the packet buffer that was received
 * @param addr the remote IP address from which the packet was received
 * @param port the remote port from which the packet was received
 */
typedef void (*udp_recv_fn)(void *arg, struct udp_pcb *pcb, struct pbuf *p,
    const ip_addr_t *addr, u16_t port);


/* udp_pcbs export for external reference (e.g. SNMP agent) */
extern struct udp_pcb *udp_pcbs;

/* The following functions is the application layer interface to the
   UDP code. */

/*Public API fucntion */
typedef struct udp_pcb * (*udp_new_fp_t)(void);
typedef struct udp_pcb * (*udp_new_ip_type_fp_t)(u8_t type);
typedef void             (*udp_remove_fp_t)(struct udp_pcb *pcb);
typedef err_t            (*udp_bind_fp_t)(struct udp_pcb *pcb, const ip_addr_t *ipaddr, u16_t port);
typedef err_t            (*udp_connect_fp_t)(struct udp_pcb *pcb, const ip_addr_t *ipaddr, u16_t port);
typedef void             (*udp_disconnect_fp_t)(struct udp_pcb *pcb);
typedef void             (*udp_recv_fp_t)(struct udp_pcb *pcb, udp_recv_fn recv, void *recv_arg);
typedef err_t            (*udp_sendto_if_fp_t)(struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *dst_ip, u16_t dst_port, struct netif *netif);
typedef err_t            (*udp_sendto_if_src_fp_t)(struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *dst_ip, u16_t dst_port, struct netif *netif, const ip_addr_t *src_ip);
typedef err_t            (*udp_sendto_fp_t)(struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *dst_ip, u16_t dst_port);
typedef err_t            (*udp_send_fp_t)(struct udp_pcb *pcb, struct pbuf *p);

#if LWIP_CHECKSUM_ON_COPY && CHECKSUM_GEN_UDP
typedef err_t            (*udp_sendto_if_chksum_fp_t)(struct udp_pcb *pcb, struct pbuf *p,
                                 const ip_addr_t *dst_ip, u16_t dst_port,
                                 struct netif *netif, u8_t have_chksum,
                                 u16_t chksum);
typedef err_t            (*udp_sendto_chksum_fp_t)(struct udp_pcb *pcb, struct pbuf *p,
                                 const ip_addr_t *dst_ip, u16_t dst_port,
                                 u8_t have_chksum, u16_t chksum);
typedef err_t            (*udp_send_chksum_fp_t)(struct udp_pcb *pcb, struct pbuf *p,
                                 u8_t have_chksum, u16_t chksum);
typedef err_t            (*udp_sendto_if_src_chksum_fp_t)(struct udp_pcb *pcb, struct pbuf *p,
                                 const ip_addr_t *dst_ip, u16_t dst_port, struct netif *netif,
                                 u8_t have_chksum, u16_t chksum, const ip_addr_t *src_ip);
#endif /* LWIP_CHECKSUM_ON_COPY && CHECKSUM_GEN_UDP */


extern udp_new_fp_t                     udp_new_adpt;
extern udp_new_ip_type_fp_t             udp_new_ip_type_adpt;
extern udp_remove_fp_t                  udp_remove_adpt;
extern udp_bind_fp_t                    udp_bind_adpt;
extern udp_connect_fp_t                 udp_connect_adpt;
extern udp_disconnect_fp_t              udp_disconnect_adpt;
extern udp_recv_fp_t                    udp_recv_adpt;
extern udp_sendto_if_fp_t               udp_sendto_if_adpt;
extern udp_sendto_if_src_fp_t           udp_sendto_if_src_adpt;
extern udp_sendto_fp_t                  udp_sendto_adpt;
extern udp_send_fp_t                    udp_send_adpt;
#if LWIP_CHECKSUM_ON_COPY && CHECKSUM_GEN_UDP
extern udp_sendto_if_chksum_fp_t        udp_sendto_if_chksum_adpt;
extern udp_sendto_chksum_fp_t           udp_sendto_chksum_adpt;
extern udp_send_chksum_fp_t             udp_send_chksum_adpt;
extern udp_sendto_if_src_chksum_fp_t    udp_sendto_if_src_chksum_adpt;
#endif /* LWIP_CHECKSUM_ON_COPY && CHECKSUM_GEN_UDP */


/* The following functions are the lower layer interface to UDP. */
typedef void             (*udp_input_fp_t)(struct pbuf *p, struct netif *inp);
typedef void             (*udp_init_fp_t)(void);

extern udp_input_fp_t udp_input_adpt;
extern udp_init_fp_t udp_init_adpt;

#if UDP_DEBUG
typedef void (*udp_debug_print_fp_t)(struct udp_hdr *udphdr);
extern udp_debug_print_fp_t udp_debug_print_adpt;
#else
#define udp_debug_print(udphdr)
#endif

typedef void (*udp_netif_ip_addr_changed_fp_t)(const ip_addr_t* old_addr, const ip_addr_t* new_addr);
extern udp_netif_ip_addr_changed_fp_t udp_netif_ip_addr_changed_adpt;

#ifdef __cplusplus
}
#endif

#endif /* LWIP_UDP */

#endif /* #define LWIP_ROMBUILD */

#endif /* LWIP_HDR_UDP_IF_H */
