/**
 * @file
 * netconn API (to be used from non-TCPIP threads)
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
#ifndef LWIP_HDR_API__IF_H
#define LWIP_HDR_API__IF_H

#include "lwip/opt.h"

#if defined(LWIP_ROMBUILD)

#if LWIP_NETCONN || LWIP_SOCKET /* don't build if not configured for use in lwipopts.h */
/* Note: Netconn API is always available when sockets are enabled -
 * sockets are implemented on top of them */

#include "lwip/arch.h"
#include "lwip/netbuf.h"
#include "lwip/sys.h"
#include "lwip/ip_addr.h"
#include "lwip/err.h"

#ifdef __cplusplus
extern "C" {
#endif

/* forward-declare some structs to avoid to include their headers */
struct ip_pcb;
struct tcp_pcb;
struct udp_pcb;
struct raw_pcb;
struct netconn;
struct api_msg;


/* Private static API fucntion used in api_lib.c*/
err_t netconn_close_shutdown(struct netconn *conn, u8_t how);
err_t netconn_apimsg(tcpip_callback_fn fn, struct api_msg *apimsg);
err_t netconn_recv_data(struct netconn *conn, void **new_buf);

typedef err_t (*netconn_close_shutdown_fp_t)(struct netconn *conn, u8_t how);
typedef err_t (*netconn_apimsg_fp_t)(tcpip_callback_fn fn, struct api_msg *apimsg);
typedef err_t (*netconn_recv_data_fp_t)(struct netconn *conn, void **new_buf);

extern netconn_close_shutdown_fp_t     netconn_close_shutdown_adpt;
extern netconn_apimsg_fp_t             netconn_apimsg_adpt;
extern netconn_recv_data_fp_t          netconn_recv_data_adpt;


/** A callback prototype to inform about events for a netconn */
typedef void (* netconn_callback)(struct netconn *, enum netconn_evt, u16_t len);

/** A netconn descriptor */
struct netconn;

/* Network connection functions: */

/** @ingroup netconn_common
 * Create new netconn connection
 * @param t @ref netconn_type */
#define netconn_new(t)                  netconn_new_with_proto_and_callback(t, 0, NULL)
#define netconn_new_with_callback(t, c) netconn_new_with_proto_and_callback(t, 0, c)
typedef struct netconn *(*netconn_new_with_proto_and_callback_fp_t)(enum netconn_type t, u8_t proto,
                                             netconn_callback callback);

/** Get the type of a netconn (as enum netconn_type). */
#define netconn_type(conn) (conn->type)


typedef err_t   (*netconn_delete_fp_t)(struct netconn *conn);
typedef err_t   (*netconn_getaddr_fp_t)(struct netconn *conn, ip_addr_t *addr, u16_t *port, u8_t local);

/** @ingroup netconn_common */
#define netconn_peer(c,i,p) netconn_getaddr(c,i,p,0)
/** @ingroup netconn_common */
#define netconn_addr(c,i,p) netconn_getaddr(c,i,p,1)

typedef err_t   (*netconn_bind_fp_t)(struct netconn *conn, const ip_addr_t *addr, u16_t port);
typedef err_t   (*netconn_connect_fp_t)(struct netconn *conn, const ip_addr_t *addr, u16_t port);
typedef err_t   (*netconn_disconnect_fp_t)(struct netconn *conn);
typedef err_t   (*netconn_listen_with_backlog_fp_t)(struct netconn *conn, u8_t backlog);

/** @ingroup netconn_tcp */
#define netconn_listen(conn) netconn_listen_with_backlog(conn, TCP_DEFAULT_LISTEN_BACKLOG)

typedef err_t   (*netconn_accept_fp_t)(struct netconn *conn, struct netconn **new_conn);
typedef err_t   (*netconn_recv_fp_t)(struct netconn *conn, struct netbuf **new_buf);
typedef err_t   (*netconn_recv_tcp_pbuf_fp_t)(struct netconn *conn, struct pbuf **new_buf);
typedef err_t   (*netconn_sendto_fp_t)(struct netconn *conn, struct netbuf *buf,
                             const ip_addr_t *addr, u16_t port);
typedef err_t   (*netconn_send_fp_t)(struct netconn *conn, struct netbuf *buf);
typedef err_t   (*netconn_write_partly_fp_t)(struct netconn *conn, const void *dataptr, size_t size,
                             u8_t apiflags, size_t *bytes_written);
/** @ingroup netconn_tcp */
#define netconn_write(conn, dataptr, size, apiflags) \
          netconn_write_partly(conn, dataptr, size, apiflags, NULL)

typedef err_t   (*netconn_close_fp_t)(struct netconn *conn);
typedef err_t   (*netconn_shutdown_fp_t)(struct netconn *conn, u8_t shut_rx, u8_t shut_tx);

#if LWIP_IGMP || (LWIP_IPV6 && LWIP_IPV6_MLD)
typedef err_t   (*netconn_join_leave_group_fp_t)(struct netconn *conn, const ip_addr_t *multiaddr,
                             const ip_addr_t *netif_addr, enum netconn_igmp join_or_leave);
#endif /* LWIP_IGMP || (LWIP_IPV6 && LWIP_IPV6_MLD) */


#if LWIP_DNS

#if LWIP_IPV4                   && LWIP_IPV6
typedef err_t(*netconn_gethostbyname_addrtype_fp_t) (const char * name, ip_addr_t * addr, u8_t dns_addrtype);

#define netconn_gethostbyname(name, addr) netconn_gethostbyname_addrtype(name, addr, NETCONN_DNS_DEFAULT)

#else /* LWIP_IPV4 && LWIP_IPV6 */

typedef err_t(*netconn_gethostbyname_fp_t) (const char * name, ip_addr_t * addr);

#define netconn_gethostbyname_addrtype(name, addr, dns_addrtype) netconn_gethostbyname(name, addr)
#endif /* LWIP_IPV4 && LWIP_IPV6 */

#endif /* LWIP_DNS */



extern netconn_new_with_proto_and_callback_fp_t    netconn_new_with_proto_and_callback_adpt;
extern netconn_delete_fp_t                         netconn_delete_adpt;
extern netconn_getaddr_fp_t                        netconn_getaddr_adpt;

extern netconn_bind_fp_t                           netconn_bind_adpt;
extern netconn_connect_fp_t                        netconn_connect_adpt;
extern netconn_disconnect_fp_t                     netconn_disconnect_adpt;
extern netconn_listen_with_backlog_fp_t            netconn_listen_with_backlog_adpt;

extern netconn_accept_fp_t                         netconn_accept_adpt;
extern netconn_recv_fp_t                           netconn_recv_adpt;
extern netconn_recv_tcp_pbuf_fp_t                  netconn_recv_tcp_pbuf_adpt;
extern netconn_sendto_fp_t                         netconn_sendto_adpt;
extern netconn_send_fp_t                           netconn_send_adpt;
extern netconn_write_partly_fp_t                   netconn_write_partly_adpt;

extern netconn_close_fp_t                          netconn_close_adpt;
extern netconn_shutdown_fp_t                       netconn_shutdown_adpt;
#if LWIP_IGMP || (LWIP_IPV6 && LWIP_IPV6_MLD)
extern netconn_join_leave_group_fp_t               netconn_join_leave_group_adpt;
#endif /* LWIP_IGMP || (LWIP_IPV6 && LWIP_IPV6_MLD) */

#if LWIP_DNS
#if LWIP_IPV4 && LWIP_IPV6
extern netconn_gethostbyname_addrtype_fp_t         netconn_gethostbyname_addrtype_adpt;
#else
extern netconn_gethostbyname_fp_t                  netconn_gethostbyname_adpt;
#endif
#endif

#if LWIP_NETCONN_SEM_PER_THREAD
typedef void (*netconn_thread_init_fp_t)(void);
typedef void (*netconn_thread_cleanup_fp_t)(void);
extern netconn_thread_init_fp_t                    netconn_thread_init_adpt;
extern netconn_thread_cleanup_fp_t                 netconn_thread_cleanup_adpt;

#else /* LWIP_NETCONN_SEM_PER_THREAD */
#define netconn_thread_init()
#define netconn_thread_cleanup()
#endif /* LWIP_NETCONN_SEM_PER_THREAD */

#ifdef __cplusplus
}
#endif

#endif /* LWIP_NETCONN || LWIP_SOCKET */

#endif /* #if defined(LWIP_ROMBUILD) */


#endif /* LWIP_HDR_API__IF_H */
