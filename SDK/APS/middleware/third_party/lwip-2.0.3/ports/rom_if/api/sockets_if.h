/**
 * @file
 * Socket API (to be used from non-TCPIP threads)
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


#ifndef LWIP_HDR_SOCKETS_IF_H
#define LWIP_HDR_SOCKETS_IF_H

#include "lwip/opt.h"

#if defined(LWIP_ROMBUILD)

#if LWIP_SOCKET /* don't build if not configured for use in lwipopts.h */

#include "lwip/ip_addr.h"
#include "lwip/err.h"
#include "lwip/inet.h"
#include "lwip/errno.h"

#ifdef __cplusplus
extern "C" {
#endif

/* If your port already typedef's socklen_t, define SOCKLEN_T_DEFINED
   to prevent this code from redefining it. */
#if !defined(socklen_t) && !defined(SOCKLEN_T_DEFINED)
typedef u32_t socklen_t;
#endif

struct sockaddr;

struct lwip_sock;
/* FD_SET used for lwip_select */
struct fd_set;
struct timeval;
struct netconn;

/* Private static API fucntion used in sockets.c*/
void sockaddr_to_ipaddr_port(const struct sockaddr * sockaddr, ip_addr_t * ipaddr, u16_t * port);

#if LWIP_IGMP
int  lwip_socket_register_membership(int s, const ip4_addr_t * if_addr, const ip4_addr_t * multi_addr);
void lwip_socket_unregister_membership(int s, const ip4_addr_t * if_addr, const ip4_addr_t * multi_addr);
void lwip_socket_drop_registered_memberships(int s);
#endif

#if !LWIP_TCPIP_CORE_LOCKING
void lwip_getsockopt_callback(void * arg);
void lwip_setsockopt_callback(void * arg);
#endif
u8_t lwip_getsockopt_implement(int s, int level, int optname, void * optval, socklen_t * optlen);
u8_t lwip_setsockopt_implement(int s, int level, int optname, const void * optval, socklen_t optlen);

struct lwip_sock *get_socket(int s);
struct lwip_sock *tryget_socket(int s);
int  alloc_socket(struct netconn * newconn, int accepted);
void free_socket(struct lwip_sock * sock, int is_tcp);
int lwip_selscan(int maxfdp1, struct fd_set *readset_in, struct fd_set *writeset_in, struct fd_set *exceptset_in, struct fd_set *readset_out, struct fd_set *writeset_out, struct fd_set *exceptset_out);
void event_callback(struct netconn * conn, enum netconn_evt evt, u16_t len);
int  lwip_getaddrname(int s, struct sockaddr * name, socklen_t * namelen, u8_t local);

typedef void (*sockaddr_to_ipaddr_port_fp_t)(const struct sockaddr * sockaddr, ip_addr_t * ipaddr, u16_t * port);

#if LWIP_IGMP
typedef int  (*lwip_socket_register_membership_fp_t)(int s, const ip4_addr_t * if_addr, const ip4_addr_t * multi_addr);
typedef void (*lwip_socket_unregister_membership_fp_t)(int s, const ip4_addr_t * if_addr, const ip4_addr_t * multi_addr);
typedef void (*lwip_socket_drop_registered_memberships_fp_t)(int s);
#endif

#if !LWIP_TCPIP_CORE_LOCKING
typedef void (*lwip_getsockopt_callback_fp_t)(void * arg);
typedef void (*lwip_setsockopt_callback_fp_t)(void * arg);
#endif
typedef u8_t (*lwip_getsockopt_implement_fp_t)(int s, int level, int optname, void * optval, socklen_t * optlen);
typedef u8_t (*lwip_setsockopt_implement_fp_t)(int s, int level, int optname, const void * optval, socklen_t optlen);

typedef struct lwip_sock *(*get_socket_fp_t)(int s);
typedef struct lwip_sock *(*tryget_socket_fp_t)(int s);
typedef int  (*alloc_socket_fp_t)(struct netconn * newconn, int accepted);
typedef void (*free_socket_fp_t)(struct lwip_sock * sock, int is_tcp);
typedef int  (*lwip_selscan_fp_t)(int maxfdp1, struct fd_set *readset_in, struct fd_set *writeset_in, struct fd_set *exceptset_in, struct fd_set *readset_out, struct fd_set *writeset_out, struct fd_set *exceptset_out);
typedef void (*event_callback_fp_t)(struct netconn * conn, enum netconn_evt evt, u16_t len);
typedef int  (*lwip_getaddrname_fp_t)(int s, struct sockaddr * name, socklen_t * namelen, u8_t local);


extern sockaddr_to_ipaddr_port_fp_t                    sockaddr_to_ipaddr_port_adpt;
#if LWIP_IGMP
extern lwip_socket_register_membership_fp_t            lwip_socket_register_membership_adpt;
extern lwip_socket_unregister_membership_fp_t          lwip_socket_unregister_membership_adpt;
extern lwip_socket_drop_registered_memberships_fp_t    lwip_socket_drop_registered_memberships_adpt;
#endif

#if !LWIP_TCPIP_CORE_LOCKING
extern lwip_getsockopt_callback_fp_t                   lwip_getsockopt_callback_adpt;
extern lwip_setsockopt_callback_fp_t                   lwip_setsockopt_callback_adpt;
#endif
extern lwip_getsockopt_implement_fp_t                  lwip_getsockopt_implement_adpt;
extern lwip_setsockopt_implement_fp_t                  lwip_setsockopt_implement_adpt;

extern get_socket_fp_t                                 get_socket_adpt;
extern tryget_socket_fp_t                              tryget_socket_adpt;
extern alloc_socket_fp_t                               alloc_socket_adpt;
extern free_socket_fp_t                                free_socket_adpt;
extern lwip_selscan_fp_t                               lwip_selscan_adpt;
extern event_callback_fp_t                             event_callback_adpt;
extern lwip_getaddrname_fp_t                           lwip_getaddrname_adpt;


/* Public API fucntion */
#define lwip_socket_init() /* Compatibility define, no init needed. */
typedef void (*lwip_socket_thread_init_fp_t)(void); /* LWIP_NETCONN_SEM_PER_THREAD==1: initialize thread-local semaphore */
typedef void (*lwip_socket_thread_cleanup_fp_t)(void); /* LWIP_NETCONN_SEM_PER_THREAD==1: destroy thread-local semaphore */

typedef int (*lwip_accept_fp_t)(int s, struct sockaddr *addr, socklen_t *addrlen);
typedef int (*lwip_bind_fp_t)(int s, const struct sockaddr *name, socklen_t namelen);
typedef int (*lwip_shutdown_fp_t)(int s, int how);
typedef int (*lwip_getpeername_fp_t)(int s, struct sockaddr *name, socklen_t *namelen);
typedef int (*lwip_getsockname_fp_t)(int s, struct sockaddr *name, socklen_t *namelen);
typedef int (*lwip_getsockopt_fp_t)(int s, int level, int optname, void *optval, socklen_t *optlen);
typedef int (*lwip_setsockopt_fp_t)(int s, int level, int optname, const void *optval, socklen_t optlen);
typedef int (*lwip_close_fp_t)(int s);
typedef int (*lwip_connect_fp_t)(int s, const struct sockaddr *name, socklen_t namelen);
typedef int (*lwip_listen_fp_t)(int s, int backlog);
typedef int (*lwip_recv_fp_t)(int s, void *mem, size_t len, int flags);
typedef int (*lwip_read_fp_t)(int s, void *mem, size_t len);
typedef int (*lwip_recvfrom_fp_t)(int s, void *mem, size_t len, int flags,      struct sockaddr *from, socklen_t *fromlen);
typedef int (*lwip_send_fp_t)(int s, const void *dataptr, size_t size, int flags);
typedef int (*lwip_sendmsg_fp_t)(int s, const struct msghdr *message, int flags);
typedef int (*lwip_sendto_fp_t)(int s, const void *dataptr, size_t size, int flags, const struct sockaddr *to, socklen_t tolen);
typedef int (*lwip_socket_fp_t)(int domain, int type, int protocol);
typedef int (*lwip_write_fp_t)(int s, const void *dataptr, size_t size);
typedef int (*lwip_writev_fp_t)(int s, const struct iovec *iov, int iovcnt);
typedef int (*lwip_select_fp_t)(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset,struct timeval *timeout);
typedef int (*lwip_ioctl_fp_t)(int s, long cmd, void *argp);
typedef int (*lwip_fcntl_fp_t)(int s, int cmd, int val);

extern lwip_socket_thread_init_fp_t     lwip_socket_thread_init_adpt;
extern lwip_socket_thread_cleanup_fp_t  lwip_socket_thread_cleanup_adpt;

extern lwip_accept_fp_t                 lwip_accept_adpt;
extern lwip_bind_fp_t                   lwip_bind_adpt;
extern lwip_shutdown_fp_t               lwip_shutdown_adpt;
extern lwip_getpeername_fp_t            lwip_getpeername_adpt;
extern lwip_getsockname_fp_t            lwip_getsockname_adpt;
extern lwip_getsockopt_fp_t             lwip_getsockopt_adpt;
extern lwip_setsockopt_fp_t             lwip_setsockopt_adpt;
extern lwip_close_fp_t                  lwip_close_adpt;
extern lwip_connect_fp_t                lwip_connect_adpt;
extern lwip_listen_fp_t                 lwip_listen_adpt;
extern lwip_recv_fp_t                   lwip_recv_adpt;
extern lwip_read_fp_t                   lwip_read_adpt;
extern lwip_recvfrom_fp_t               lwip_recvfrom_adpt;
extern lwip_send_fp_t                   lwip_send_adpt;
extern lwip_sendmsg_fp_t                lwip_sendmsg_adpt;
extern lwip_sendto_fp_t                 lwip_sendto_adpt;
extern lwip_socket_fp_t                 lwip_socket_adpt;
extern lwip_write_fp_t                  lwip_write_adpt;
extern lwip_writev_fp_t                 lwip_writev_adpt;
extern lwip_select_fp_t                 lwip_select_adpt;
extern lwip_ioctl_fp_t                  lwip_ioctl_adpt;
extern lwip_fcntl_fp_t                  lwip_fcntl_adpt;


#ifdef __cplusplus
}
#endif

#endif /* LWIP_SOCKET */

#endif /*#if defined(LWIP_ROMBUILD) */

#endif /* LWIP_HDR_SOCKETS_If_H */
