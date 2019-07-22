/**
 * @file
 * Sockets BSD-Like API module
 *
 * @defgroup socket Socket API
 * @ingroup sequential_api
 * BSD-style socket API.\n
 * Thread-safe, to be called from non-TCPIP threads only.\n
 * Can be activated by defining @ref LWIP_SOCKET to 1.\n
 * Header is in posix/sys/socket.h\b
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
 * Improved by Marc Boucher <marc@mbsi.ca> and David Haas <dhaas@alum.rpi.edu>
 *
 */

#include "lwip/opt.h"

#if LWIP_SOCKET /* don't build if not configured for use in lwipopts.h */

#include "lwip/sockets.h"
#include "lwip/api.h"
#include "lwip/sys.h"
#include "lwip/igmp.h"
#include "lwip/inet.h"
#include "lwip/tcp.h"
#include "lwip/raw.h"
#include "lwip/udp.h"
#include "lwip/memp.h"
#include "lwip/pbuf.h"
#include "lwip/priv/tcpip_priv.h"
#if LWIP_CHECKSUM_ON_COPY
#include "lwip/inet_chksum.h"
#endif

#include <string.h>

/* Private static API fucntion used in sockets.c*/
RET_DATA sockaddr_to_ipaddr_port_fp_t                   sockaddr_to_ipaddr_port_adpt;
#if LWIP_IGMP
RET_DATA lwip_socket_register_membership_fp_t           lwip_socket_register_membership_adpt;
RET_DATA lwip_socket_unregister_membership_fp_t         lwip_socket_unregister_membership_adpt;
RET_DATA lwip_socket_drop_registered_memberships_fp_t   lwip_socket_drop_registered_memberships_adpt;
#endif

#if !LWIP_TCPIP_CORE_LOCKING
RET_DATA lwip_getsockopt_callback_fp_t                  lwip_getsockopt_callback_adpt;
RET_DATA lwip_setsockopt_callback_fp_t                  lwip_setsockopt_callback_adpt;
#endif
RET_DATA lwip_getsockopt_implement_fp_t                 lwip_getsockopt_implement_adpt;
RET_DATA lwip_setsockopt_implement_fp_t                 lwip_setsockopt_implement_adpt;

RET_DATA get_socket_fp_t                                get_socket_adpt;
RET_DATA tryget_socket_fp_t                             tryget_socket_adpt;
RET_DATA alloc_socket_fp_t                              alloc_socket_adpt;
RET_DATA free_socket_fp_t                               free_socket_adpt;
RET_DATA lwip_selscan_fp_t                              lwip_selscan_adpt;
RET_DATA event_callback_fp_t                            event_callback_adpt;
RET_DATA lwip_getaddrname_fp_t                          lwip_getaddrname_adpt;


/* Public API fucntion used in sockets.c*/
RET_DATA lwip_socket_thread_init_fp_t                   lwip_socket_thread_init_adpt;
RET_DATA lwip_socket_thread_cleanup_fp_t                lwip_socket_thread_cleanup_adpt;

RET_DATA lwip_accept_fp_t                               lwip_accept_adpt;
RET_DATA lwip_bind_fp_t                                 lwip_bind_adpt;
RET_DATA lwip_shutdown_fp_t                             lwip_shutdown_adpt;
RET_DATA lwip_getpeername_fp_t                          lwip_getpeername_adpt;
RET_DATA lwip_getsockname_fp_t                          lwip_getsockname_adpt;
RET_DATA lwip_getsockopt_fp_t                           lwip_getsockopt_adpt;
RET_DATA lwip_setsockopt_fp_t                           lwip_setsockopt_adpt;
RET_DATA lwip_close_fp_t                                lwip_close_adpt;
RET_DATA lwip_connect_fp_t                              lwip_connect_adpt;
RET_DATA lwip_listen_fp_t                               lwip_listen_adpt;
RET_DATA lwip_recv_fp_t                                 lwip_recv_adpt;
RET_DATA lwip_read_fp_t                                 lwip_read_adpt;
RET_DATA lwip_recvfrom_fp_t                             lwip_recvfrom_adpt;
RET_DATA lwip_send_fp_t                                 lwip_send_adpt;
RET_DATA lwip_sendmsg_fp_t                              lwip_sendmsg_adpt;
RET_DATA lwip_sendto_fp_t                               lwip_sendto_adpt;
RET_DATA lwip_socket_fp_t                               lwip_socket_adpt;
RET_DATA lwip_write_fp_t                                lwip_write_adpt;
RET_DATA lwip_writev_fp_t                               lwip_writev_adpt;
RET_DATA lwip_select_fp_t                               lwip_select_adpt;
RET_DATA lwip_ioctl_fp_t                                lwip_ioctl_adpt;
RET_DATA lwip_fcntl_fp_t                                lwip_fcntl_adpt;


void lwip_load_interface_sockets(void)
{
#if LWIP_IGMP
    memset(socket_ipv4_multicast_memberships, 0, sizeof(struct lwip_socket_multicast_pair) * LWIP_SOCKET_MAX_MEMBERSHIPS);
#endif
    memset(sockets, 0, sizeof(struct lwip_sock)*NUM_SOCKETS);

    select_cb_list = NULL;
    select_cb_ctr = 0;

    /* Private static API fucntion used in sockets.c*/
#if LWIP_IPV4 && LWIP_IPV6
    sockaddr_to_ipaddr_port_adpt                    = LWIP_ROMFN(sockaddr_to_ipaddr_port);
#endif

#if LWIP_IGMP
    lwip_socket_register_membership_adpt            = LWIP_ROMFN(lwip_socket_register_membership);
    lwip_socket_unregister_membership_adpt          = LWIP_ROMFN(lwip_socket_unregister_membership);
    lwip_socket_drop_registered_memberships_adpt    = LWIP_ROMFN(lwip_socket_drop_registered_memberships);
#endif

#if !LWIP_TCPIP_CORE_LOCKING
    lwip_getsockopt_callback_adpt                   = LWIP_ROMFN(lwip_getsockopt_callback);
    lwip_setsockopt_callback_adpt                   = LWIP_ROMFN(lwip_setsockopt_callback);
#endif
    lwip_getsockopt_implement_adpt                  = LWIP_ROMFN(lwip_getsockopt_implement);
    lwip_setsockopt_implement_adpt                  = LWIP_ROMFN(lwip_setsockopt_implement);

    get_socket_adpt                                 = LWIP_ROMFN(get_socket);
    tryget_socket_adpt                              = LWIP_ROMFN(tryget_socket);
    alloc_socket_adpt                               = LWIP_ROMFN(alloc_socket);
    free_socket_adpt                                = LWIP_ROMFN(free_socket);
    lwip_selscan_adpt                               = LWIP_ROMFN(lwip_selscan);
    event_callback_adpt                             = LWIP_ROMFN(event_callback);
    lwip_getaddrname_adpt                           = LWIP_ROMFN(lwip_getaddrname);

    /* Public API fucntion */
    lwip_socket_thread_init_adpt                    = LWIP_ROMFN(lwip_socket_thread_init);
    lwip_socket_thread_cleanup_adpt                 = LWIP_ROMFN(lwip_socket_thread_cleanup);
    lwip_accept_adpt                                = LWIP_ROMFN(lwip_accept);
    lwip_bind_adpt                                  = LWIP_ROMFN(lwip_bind);
    lwip_shutdown_adpt                              = LWIP_ROMFN(lwip_shutdown);
    lwip_getpeername_adpt                           = LWIP_ROMFN(lwip_getpeername);
    lwip_getsockname_adpt                           = LWIP_ROMFN(lwip_getsockname);
    lwip_getsockopt_adpt                            = LWIP_ROMFN(lwip_getsockopt);
    lwip_setsockopt_adpt                            = LWIP_ROMFN(lwip_setsockopt);
    lwip_close_adpt                                 = LWIP_ROMFN(lwip_close);
    lwip_connect_adpt                               = LWIP_ROMFN(lwip_connect);
    lwip_listen_adpt                                = LWIP_ROMFN(lwip_listen);
    lwip_recv_adpt                                  = LWIP_ROMFN(lwip_recv);
    lwip_read_adpt                                  = LWIP_ROMFN(lwip_read);
    lwip_recvfrom_adpt                              = LWIP_ROMFN(lwip_recvfrom);
    lwip_send_adpt                                  = LWIP_ROMFN(lwip_send);
    lwip_sendmsg_adpt                               = LWIP_ROMFN(lwip_sendmsg);
    lwip_sendto_adpt                                = LWIP_ROMFN(lwip_sendto);
    lwip_socket_adpt                                = LWIP_ROMFN(lwip_socket);
    lwip_write_adpt                                 = LWIP_ROMFN(lwip_write);
    lwip_writev_adpt                                = LWIP_ROMFN(lwip_writev);
    lwip_select_adpt                                = LWIP_ROMFN(lwip_select);
    lwip_ioctl_adpt                                 = LWIP_ROMFN(lwip_ioctl);
    lwip_fcntl_adpt                                 = LWIP_ROMFN(lwip_fcntl);
}


#if LWIP_IPV4 && LWIP_IPV6
void
sockaddr_to_ipaddr_port(const struct sockaddr* sockaddr, ip_addr_t* ipaddr, u16_t* port)
{
    sockaddr_to_ipaddr_port_adpt(sockaddr, ipaddr, port);
}
#endif /* LWIP_IPV4 && LWIP_IPV6 */

/** LWIP_NETCONN_SEM_PER_THREAD==1: initialize thread-local semaphore */
void
lwip_socket_thread_init(void)
{
    lwip_socket_thread_init_adpt();
}

/** LWIP_NETCONN_SEM_PER_THREAD==1: destroy thread-local semaphore */
void
lwip_socket_thread_cleanup(void)
{
    lwip_socket_thread_cleanup_adpt();
}

/**
 * Map a externally used socket index to the internal socket representation.
 *
 * @param s externally used socket index
 * @return struct lwip_sock for the socket or NULL if not found
 */
struct lwip_sock *
get_socket(int s)
{
    return get_socket_adpt(s);
}

/**
 * Same as get_socket but doesn't set errno
 *
 * @param s externally used socket index
 * @return struct lwip_sock for the socket or NULL if not found
 */
struct lwip_sock *
tryget_socket(int s)
{
    return tryget_socket_adpt(s);
}

/**
 * Allocate a new socket for a given netconn.
 *
 * @param newconn the netconn for which to allocate a socket
 * @param accepted 1 if socket has been created by accept(),
 *                 0 if socket has been created by socket()
 * @return the index of the new socket; -1 on error
 */
int
alloc_socket(struct netconn *newconn, int accepted)
{
    return alloc_socket_adpt(newconn, accepted);
}

/** Free a socket. The socket's netconn must have been
 * delete before!
 *
 * @param sock the socket to free
 * @param is_tcp != 0 for TCP sockets, used to free lastdata
 */
void
free_socket(struct lwip_sock *sock, int is_tcp)
{
    free_socket_adpt(sock, is_tcp);
}

/* Below this, the well-known socket functions are implemented.
 * Use google.com or opengroup.org to get a good description :-)
 *
 * Exceptions are documented!
 */

int
lwip_accept(int s, struct sockaddr *addr, socklen_t *addrlen)
{
    return lwip_accept_adpt(s, addr, addrlen);
}

int
lwip_bind(int s, const struct sockaddr *name, socklen_t namelen)
{
    return lwip_bind_adpt(s, name, namelen);
}

int
lwip_close(int s)
{
    return lwip_close_adpt(s);
}

int
lwip_connect(int s, const struct sockaddr *name, socklen_t namelen)
{
    return lwip_connect_adpt(s, name, namelen);
}

/**
 * Set a socket into listen mode.
 * The socket may not have been used for another connection previously.
 *
 * @param s the socket to set to listening mode
 * @param backlog (ATTENTION: needs TCP_LISTEN_BACKLOG=1)
 * @return 0 on success, non-zero on failure
 */
int
lwip_listen(int s, int backlog)
{
    return lwip_listen_adpt(s, backlog);
}

int
lwip_recvfrom(int s, void *mem, size_t len, int flags,
              struct sockaddr *from, socklen_t *fromlen)
{
    return lwip_recvfrom_adpt(s, mem, len, flags, from, fromlen);
}

int
lwip_read(int s, void *mem, size_t len)
{
    return lwip_read_adpt(s, mem, len);
}

int
lwip_recv(int s, void *mem, size_t len, int flags)
{
    return lwip_recv_adpt(s, mem, len, flags);
}

int
lwip_send(int s, const void *data, size_t size, int flags)
{
    return lwip_send_adpt(s, data, size, flags);
}

int
lwip_sendmsg(int s, const struct msghdr *msg, int flags)
{
    return lwip_sendmsg_adpt(s, msg, flags);
}

int
lwip_sendto(int s, const void *data, size_t size, int flags,
       const struct sockaddr *to, socklen_t tolen)
{
    return lwip_sendto_adpt(s, data, size, flags, to, tolen);
}


int
lwip_socket(int domain, int type, int protocol)
{
    return lwip_socket_adpt(domain, type, protocol);
}

int
lwip_write(int s, const void *data, size_t size)
{
    return lwip_write_adpt(s, data, size);
}

int
lwip_writev(int s, const struct iovec *iov, int iovcnt)
{
    return lwip_writev_adpt(s, iov, iovcnt);
}

/**
 * Go through the readset and writeset lists and see which socket of the sockets
 * set in the sets has events. On return, readset, writeset and exceptset have
 * the sockets enabled that had events.
 *
 * @param maxfdp1 the highest socket index in the sets
 * @param readset_in    set of sockets to check for read events
 * @param writeset_in   set of sockets to check for write events
 * @param exceptset_in  set of sockets to check for error events
 * @param readset_out   set of sockets that had read events
 * @param writeset_out  set of sockets that had write events
 * @param exceptset_out set os sockets that had error events
 * @return number of sockets that had events (read/write/exception) (>= 0)
 */
int
lwip_selscan(int maxfdp1, fd_set *readset_in, fd_set *writeset_in, fd_set *exceptset_in,
             fd_set *readset_out, fd_set *writeset_out, fd_set *exceptset_out)
{
    return lwip_selscan_adpt(maxfdp1, readset_in, writeset_in, exceptset_in, readset_out, writeset_out, exceptset_out);
}

int
lwip_select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset,
            struct timeval *timeout)
{
    return lwip_select_adpt(maxfdp1, readset, writeset, exceptset, timeout);
}

/**
 * Callback registered in the netconn layer for each socket-netconn.
 * Processes recvevent (data available) and wakes up tasks waiting for select.
 */
void
event_callback(struct netconn *conn, enum netconn_evt evt, u16_t len)
{
    event_callback_adpt(conn, evt, len);
}

/**
 * Close one end of a full-duplex connection.
 */
int
lwip_shutdown(int s, int how)
{
    return lwip_shutdown_adpt(s, how);
}

int
lwip_getaddrname(int s, struct sockaddr *name, socklen_t *namelen, u8_t local)
{
    return lwip_getaddrname_adpt(s, name, namelen, local);
}

int
lwip_getpeername(int s, struct sockaddr *name, socklen_t *namelen)
{
    return lwip_getpeername_adpt(s, name, namelen);
}

int
lwip_getsockname(int s, struct sockaddr *name, socklen_t *namelen)
{
    return lwip_getsockname_adpt(s, name, namelen);
}

int
lwip_getsockopt(int s, int level, int optname, void *optval, socklen_t *optlen)
{
    return lwip_getsockopt_adpt(s, level, optname, optval, optlen);
}

#if !LWIP_TCPIP_CORE_LOCKING
/** lwip_getsockopt_callback: only used without CORE_LOCKING
 * to get into the tcpip_thread
 */
void
lwip_getsockopt_callback(void *arg)
{
    lwip_getsockopt_callback_adpt(arg);
}
#endif  /* LWIP_TCPIP_CORE_LOCKING */

/** lwip_getsockopt_impl: the actual implementation of getsockopt:
 * same argument as lwip_getsockopt, either called directly or through callback
 */
u8_t
lwip_getsockopt_implement(int s, int level, int optname, void *optval, socklen_t *optlen)
{
    return lwip_getsockopt_implement_adpt(s, level, optname, optval, optlen);
}

int
lwip_setsockopt(int s, int level, int optname, const void *optval, socklen_t optlen)
{
    return lwip_setsockopt_adpt(s, level, optname, optval, optlen);
}

#if !LWIP_TCPIP_CORE_LOCKING
/** lwip_setsockopt_callback: only used without CORE_LOCKING
 * to get into the tcpip_thread
 */
void
lwip_setsockopt_callback(void *arg)
{
    lwip_setsockopt_callback_adpt(arg);
}
#endif  /* LWIP_TCPIP_CORE_LOCKING */

/** lwip_setsockopt_impl: the actual implementation of setsockopt:
 * same argument as lwip_setsockopt, either called directly or through callback
 */
u8_t
lwip_setsockopt_implement(int s, int level, int optname, const void *optval, socklen_t optlen)
{
    return lwip_setsockopt_implement_adpt(s, level, optname, optval, optlen);
}

int
lwip_ioctl(int s, long cmd, void *argp)
{
    return lwip_ioctl_adpt(s, cmd, argp);
}

/** A minimal implementation of fcntl.
 * Currently only the commands F_GETFL and F_SETFL are implemented.
 * Only the flag O_NONBLOCK is implemented.
 */
int
lwip_fcntl(int s, int cmd, int val)
{
    return lwip_fcntl_adpt(s, cmd, val);
}

#if LWIP_IGMP
/** Register a new IGMP membership. On socket close, the membership is dropped automatically.
 *
 * ATTENTION: this function is called from tcpip_thread (or under CORE_LOCK).
 *
 * @return 1 on success, 0 on failure
 */
int
lwip_socket_register_membership(int s, const ip4_addr_t *if_addr, const ip4_addr_t *multi_addr)
{
    return lwip_socket_register_membership_adpt(s, if_addr, multi_addr);
}

/** Unregister a previously registered membership. This prevents dropping the membership
 * on socket close.
 *
 * ATTENTION: this function is called from tcpip_thread (or under CORE_LOCK).
 */
void
lwip_socket_unregister_membership(int s, const ip4_addr_t *if_addr, const ip4_addr_t *multi_addr)
{
    lwip_socket_unregister_membership_adpt(s, if_addr, multi_addr);
}

/** Drop all memberships of a socket that were not dropped explicitly via setsockopt.
 *
 * ATTENTION: this function is NOT called from tcpip_thread (or under CORE_LOCK).
 */
void
lwip_socket_drop_registered_memberships(int s)
{
    lwip_socket_drop_registered_memberships_adpt(s);
}
#endif /* LWIP_IGMP */
#endif /* LWIP_SOCKET */


