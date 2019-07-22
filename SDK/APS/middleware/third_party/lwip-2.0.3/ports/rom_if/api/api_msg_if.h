/**
 * @file
 * netconn API lwIP internal implementations (do not use in application code)
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
#ifndef LWIP_HDR_API_MSG_IF_H
#define LWIP_HDR_API_MSG_IF_H

#include "lwip/opt.h"

#if defined(LWIP_ROMBUILD)

#if LWIP_NETCONN || LWIP_SOCKET /* don't build if not configured for use in lwipopts.h */
/* Note: Netconn API is always available when sockets are enabled -
 * sockets are implemented on top of them */

#include "lwip/arch.h"
#include "lwip/ip_addr.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "lwip/igmp.h"
#include "lwip/api.h"
#include "lwip/priv/tcpip_priv.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Private static API fucntion used in api_msg.c*/
#if LWIP_RAW
u8_t  recv_raw(void *arg, struct raw_pcb *pcb, struct pbuf *p, const ip_addr_t *addr);
#endif /* LWIP_RAW*/

#if LWIP_UDP
void  recv_udp(void *arg, struct udp_pcb *pcb, struct pbuf *p,const ip_addr_t *addr, u16_t port);
#endif /* LWIP_UDP */

#if LWIP_TCP
err_t recv_tcp(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err);
err_t poll_tcp(void *arg, struct tcp_pcb *pcb);
err_t sent_tcp(void *arg, struct tcp_pcb *pcb, u16_t len);
void  err_tcp(void *arg, err_t err);
void  setup_tcp(struct netconn *conn);
err_t accept_function(void *arg, struct tcp_pcb *newpcb, err_t err);
#endif /* LWIP_TCP */
void  pcb_new(struct api_msg *msg);
void  netconn_drain(struct netconn *conn);

#if LWIP_TCP
err_t lwip_netconn_do_close_internal(struct netconn *conn  WRITE_DELAYED_PARAM);
err_t lwip_netconn_do_connected(void *arg, struct tcp_pcb *pcb, err_t err);
err_t lwip_netconn_do_writemore(struct netconn *conn  WRITE_DELAYED_PARAM);
#endif /* LWIP_TCP */

#if LWIP_DNS
void  lwip_netconn_do_dns_found(const char *name, const ip_addr_t *ipaddr, void *arg);
#endif /* LWIP_DNS */



/* Private static API fucntion used in api_msg.c*/
#if LWIP_RAW
typedef u8_t  (*recv_raw_fp_t)(void *arg, struct raw_pcb *pcb, struct pbuf *p, const ip_addr_t *addr);
#endif /* LWIP_RAW*/

#if LWIP_UDP
typedef void  (*recv_udp_fp_t)(void *arg, struct udp_pcb *pcb, struct pbuf *p,const ip_addr_t *addr, u16_t port);
#endif /* LWIP_UDP */

#if LWIP_TCP
typedef err_t (*recv_tcp_fp_t)(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err);
typedef err_t (*poll_tcp_fp_t)(void *arg, struct tcp_pcb *pcb);
typedef err_t (*sent_tcp_fp_t)(void *arg, struct tcp_pcb *pcb, u16_t len);
typedef void  (*err_tcp_fp_t)(void *arg, err_t err);
typedef void  (*setup_tcp_fp_t)(struct netconn *conn);
typedef err_t (*accept_function_fp_t)(void *arg, struct tcp_pcb *newpcb, err_t err);
#endif /* LWIP_TCP */

typedef void  (*pcb_new_fp_t)(struct api_msg *msg);
typedef void  (*netconn_drain_fp_t)(struct netconn *conn);

#if LWIP_TCP
typedef err_t (*lwip_netconn_do_close_internal_fp_t)(struct netconn *conn  WRITE_DELAYED_PARAM);
typedef err_t (*lwip_netconn_do_connected_fp_t)(void *arg, struct tcp_pcb *pcb, err_t err);
typedef err_t (*lwip_netconn_do_writemore_fp_t)(struct netconn *conn  WRITE_DELAYED_PARAM);
#endif /* LWIP_TCP */

#if LWIP_DNS
typedef void  (*lwip_netconn_do_dns_found_fp_t)(const char *name, const ip_addr_t *ipaddr, void *arg);
#endif /* LWIP_DNS */


#if LWIP_RAW
extern recv_raw_fp_t                       recv_raw_adpt;
#endif /* LWIP_RAW*/
#if LWIP_UDP
extern recv_udp_fp_t                       recv_udp_adpt;
#endif /* LWIP_UDP */
#if LWIP_TCP
extern recv_tcp_fp_t                       recv_tcp_adpt;
extern poll_tcp_fp_t                       poll_tcp_adpt;
extern sent_tcp_fp_t                       sent_tcp_adpt;
extern err_tcp_fp_t                        err_tcp_adpt;
extern setup_tcp_fp_t                      setup_tcp_adpt;
extern accept_function_fp_t                accept_function_adpt;
#endif /* LWIP_TCP */
extern pcb_new_fp_t                        pcb_new_adpt;
extern netconn_drain_fp_t                  netconn_drain_adpt;
#if LWIP_TCP
extern lwip_netconn_do_close_internal_fp_t lwip_netconn_do_close_internal_adpt;
extern lwip_netconn_do_connected_fp_t      lwip_netconn_do_connected_adpt;
extern lwip_netconn_do_writemore_fp_t      lwip_netconn_do_writemore_adpt;
#endif /* LWIP_TCP */
#if LWIP_DNS
extern lwip_netconn_do_dns_found_fp_t      lwip_netconn_do_dns_found_adpt;
#endif



/* Public API fucntion used in api_msg.c*/
typedef void (*lwip_netconn_do_newconn_fp_t)         (void *m);
typedef void (*lwip_netconn_do_delconn_fp_t)         (void *m);
typedef void (*lwip_netconn_do_bind_fp_t)            (void *m);
typedef void (*lwip_netconn_do_connect_fp_t)         (void *m);
typedef void (*lwip_netconn_do_disconnect_fp_t)      (void *m);
typedef void (*lwip_netconn_do_listen_fp_t)          (void *m);
typedef void (*lwip_netconn_do_send_fp_t)            (void *m);
typedef void (*lwip_netconn_do_recv_fp_t)            (void *m);
#if TCP_LISTEN_BACKLOG
typedef void (*lwip_netconn_do_accepted_fp_t)        (void *m);
#endif /* TCP_LISTEN_BACKLOG */
typedef void (*lwip_netconn_do_write_fp_t)           (void *m);
typedef void (*lwip_netconn_do_getaddr_fp_t)         (void *m);
typedef void (*lwip_netconn_do_close_fp_t)           (void *m);
typedef void (*lwip_netconn_do_shutdown_fp_t)        (void *m);
#if LWIP_IGMP || (LWIP_IPV6 && LWIP_IPV6_MLD)
typedef void (*lwip_netconn_do_join_leave_group_fp_t)(void *m);
#endif /* LWIP_IGMP || (LWIP_IPV6 && LWIP_IPV6_MLD) */

#if LWIP_DNS
typedef void (*lwip_netconn_do_gethostbyname_fp_t)(void *arg);
#endif /* LWIP_DNS */

typedef struct netconn* (*netconn_alloc_fp_t)(enum netconn_type t, netconn_callback callback);
typedef void (*netconn_free_fp_t)(struct netconn *conn);


extern lwip_netconn_do_newconn_fp_t                       lwip_netconn_do_newconn_adpt;
extern lwip_netconn_do_delconn_fp_t                       lwip_netconn_do_delconn_adpt;
extern lwip_netconn_do_bind_fp_t                          lwip_netconn_do_bind_adpt;
extern lwip_netconn_do_connect_fp_t                       lwip_netconn_do_connect_adpt;
extern lwip_netconn_do_disconnect_fp_t                    lwip_netconn_do_disconnect_adpt;
extern lwip_netconn_do_listen_fp_t                        lwip_netconn_do_listen_adpt;
extern lwip_netconn_do_send_fp_t                          lwip_netconn_do_send_adpt;
extern lwip_netconn_do_recv_fp_t                          lwip_netconn_do_recv_adpt;
#if TCP_LISTEN_BACKLOG
extern lwip_netconn_do_accepted_fp_t                      lwip_netconn_do_accepted_adpt;
#endif /* TCP_LISTEN_BACKLOG */
extern lwip_netconn_do_write_fp_t                         lwip_netconn_do_write_adpt;
extern lwip_netconn_do_getaddr_fp_t                       lwip_netconn_do_getaddr_adpt;
extern lwip_netconn_do_close_fp_t                         lwip_netconn_do_close_adpt;
extern lwip_netconn_do_shutdown_fp_t                      lwip_netconn_do_shutdown_adpt;
#if LWIP_IGMP || (LWIP_IPV6 && LWIP_IPV6_MLD)
extern lwip_netconn_do_join_leave_group_fp_t              lwip_netconn_do_join_leave_group_adpt;
#endif
#if LWIP_DNS
extern lwip_netconn_do_gethostbyname_fp_t                 lwip_netconn_do_gethostbyname_adpt;
#endif /* LWIP_DNS */

extern netconn_alloc_fp_t                                 netconn_alloc_adpt;
extern netconn_free_fp_t                                  netconn_free_adpt;



#ifdef __cplusplus
}
#endif

#endif /* LWIP_NETCONN || LWIP_SOCKET */

#endif /*#if defined(LWIP_ROMBUILD)*/
#endif /* LWIP_HDR_API_MSG_IF_H */
