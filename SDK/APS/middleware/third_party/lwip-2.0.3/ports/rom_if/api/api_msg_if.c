/**
 * @file
 * Sequential API Internal module
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

#include "lwip/opt.h"

#if LWIP_NETCONN /* don't build if not configured for use in lwipopts.h */

#include "lwip/priv/api_msg.h"

#include "lwip/ip.h"
#include "lwip/ip_addr.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include "lwip/raw.h"

#include "lwip/memp.h"
#include "lwip/igmp.h"
#include "lwip/dns.h"
#include "lwip/mld6.h"
#include "lwip/priv/tcpip_priv.h"

#include <string.h>


#if LWIP_TCP
#if LWIP_TCPIP_CORE_LOCKING
#define WRITE_DELAYED         , 1
#define WRITE_DELAYED_PARAM   , u8_t delayed
#define WRITE_DELAYED_ARG     , delayed
#else /* LWIP_TCPIP_CORE_LOCKING */
#define WRITE_DELAYED
#define WRITE_DELAYED_PARAM
#define WRITE_DELAYED_ARG
#endif /* LWIP_TCPIP_CORE_LOCKING */
#endif

#if LWIP_RAW
RET_DATA recv_raw_fp_t                                recv_raw_adpt;
#endif /* LWIP_RAW*/
#if LWIP_UDP
RET_DATA recv_udp_fp_t                                recv_udp_adpt;
#endif /* LWIP_UDP */
#if LWIP_TCP
RET_DATA recv_tcp_fp_t                                recv_tcp_adpt;
RET_DATA poll_tcp_fp_t                                poll_tcp_adpt;
RET_DATA sent_tcp_fp_t                                sent_tcp_adpt;
RET_DATA err_tcp_fp_t                                 err_tcp_adpt;
RET_DATA setup_tcp_fp_t                               setup_tcp_adpt;
RET_DATA accept_function_fp_t                         accept_function_adpt;
#endif /* LWIP_TCP */
RET_DATA pcb_new_fp_t                                 pcb_new_adpt;
RET_DATA netconn_drain_fp_t                           netconn_drain_adpt;
#if LWIP_TCP
RET_DATA lwip_netconn_do_close_internal_fp_t          lwip_netconn_do_close_internal_adpt;
RET_DATA lwip_netconn_do_connected_fp_t               lwip_netconn_do_connected_adpt;
RET_DATA lwip_netconn_do_writemore_fp_t               lwip_netconn_do_writemore_adpt;
#endif /* LWIP_TCP */
#if LWIP_DNS
RET_DATA lwip_netconn_do_dns_found_fp_t               lwip_netconn_do_dns_found_adpt;
#endif


RET_DATA lwip_netconn_do_newconn_fp_t                 lwip_netconn_do_newconn_adpt;
RET_DATA lwip_netconn_do_delconn_fp_t                 lwip_netconn_do_delconn_adpt;
RET_DATA lwip_netconn_do_bind_fp_t                    lwip_netconn_do_bind_adpt;
RET_DATA lwip_netconn_do_connect_fp_t                 lwip_netconn_do_connect_adpt;
RET_DATA lwip_netconn_do_disconnect_fp_t              lwip_netconn_do_disconnect_adpt;
RET_DATA lwip_netconn_do_listen_fp_t                  lwip_netconn_do_listen_adpt;
RET_DATA lwip_netconn_do_send_fp_t                    lwip_netconn_do_send_adpt;
RET_DATA lwip_netconn_do_recv_fp_t                    lwip_netconn_do_recv_adpt;
#if TCP_LISTEN_BACKLOG
RET_DATA lwip_netconn_do_accepted_fp_t                lwip_netconn_do_accepted_adpt;
#endif /* TCP_LISTEN_BACKLOG */
RET_DATA lwip_netconn_do_write_fp_t                   lwip_netconn_do_write_adpt;
RET_DATA lwip_netconn_do_getaddr_fp_t                 lwip_netconn_do_getaddr_adpt;
RET_DATA lwip_netconn_do_close_fp_t                   lwip_netconn_do_close_adpt;
//RET_DATA lwip_netconn_do_shutdown_fp_t                lwip_netconn_do_shutdown_adpt;
#if LWIP_IGMP || (LWIP_IPV6 && LWIP_IPV6_MLD)
RET_DATA lwip_netconn_do_join_leave_group_fp_t        lwip_netconn_do_join_leave_group_adpt;
#endif
#if LWIP_DNS
RET_DATA lwip_netconn_do_gethostbyname_fp_t           lwip_netconn_do_gethostbyname_adpt;
#endif /* LWIP_DNS */

RET_DATA netconn_alloc_fp_t                           netconn_alloc_adpt;
RET_DATA netconn_free_fp_t                            netconn_free_adpt;


void lwip_load_interface_api_msg(void)
{
    netconn_aborted = 0;

#if LWIP_RAW
    recv_raw_adpt                                   = LWIP_ROMFN(recv_raw);
#endif /* LWIP_RAW*/
#if LWIP_UDP
    recv_udp_adpt                                   = LWIP_ROMFN(recv_udp);
#endif /* LWIP_UDP */
#if LWIP_TCP
    recv_tcp_adpt                                   = LWIP_ROMFN(recv_tcp);
    poll_tcp_adpt                                   = LWIP_ROMFN(poll_tcp);
    sent_tcp_adpt                                   = LWIP_ROMFN(sent_tcp);
    err_tcp_adpt                                    = LWIP_ROMFN(err_tcp);
    setup_tcp_adpt                                  = LWIP_ROMFN(setup_tcp);
    accept_function_adpt                            = LWIP_ROMFN(accept_function);
#endif /* LWIP_TCP */
    pcb_new_adpt                                    = LWIP_ROMFN(pcb_new);
    netconn_drain_adpt                              = LWIP_ROMFN(netconn_drain);
#if LWIP_TCP
    lwip_netconn_do_close_internal_adpt             = LWIP_ROMFN(lwip_netconn_do_close_internal);
    lwip_netconn_do_connected_adpt                  = LWIP_ROMFN(lwip_netconn_do_connected);
    lwip_netconn_do_writemore_adpt                  = LWIP_ROMFN(lwip_netconn_do_writemore);
#endif /* LWIP_TCP */
#if LWIP_DNS
     lwip_netconn_do_dns_found_adpt                 = LWIP_ROMFN(lwip_netconn_do_dns_found);
#endif

    lwip_netconn_do_newconn_adpt                    = LWIP_ROMFN(lwip_netconn_do_newconn);
    lwip_netconn_do_delconn_adpt                    = LWIP_ROMFN(lwip_netconn_do_delconn);
    lwip_netconn_do_bind_adpt                       = LWIP_ROMFN(lwip_netconn_do_bind);
    lwip_netconn_do_connect_adpt                    = LWIP_ROMFN(lwip_netconn_do_connect);
    lwip_netconn_do_disconnect_adpt                 = LWIP_ROMFN(lwip_netconn_do_disconnect);
    lwip_netconn_do_listen_adpt                     = LWIP_ROMFN(lwip_netconn_do_listen);
    lwip_netconn_do_send_adpt                       = LWIP_ROMFN(lwip_netconn_do_send);
    lwip_netconn_do_recv_adpt                       = LWIP_ROMFN(lwip_netconn_do_recv);
#if TCP_LISTEN_BACKLOG
    lwip_netconn_do_accepted_adpt                   = LWIP_ROMFN(lwip_netconn_do_accepted);
#endif /* TCP_LISTEN_BACKLOG */
    lwip_netconn_do_write_adpt                      = LWIP_ROMFN(lwip_netconn_do_write);
    lwip_netconn_do_getaddr_adpt                    = LWIP_ROMFN(lwip_netconn_do_getaddr);
    lwip_netconn_do_close_adpt                      = LWIP_ROMFN(lwip_netconn_do_close);
    //lwip_netconn_do_shutdown_adpt                   = LWIP_ROMFN(lwip_netconn_do_shutdown);
#if LWIP_IGMP || (LWIP_IPV6 && LWIP_IPV6_MLD)
    lwip_netconn_do_join_leave_group_adpt           = LWIP_ROMFN(lwip_netconn_do_join_leave_group);
#endif
#if LWIP_DNS
    lwip_netconn_do_gethostbyname_adpt              = LWIP_ROMFN(lwip_netconn_do_gethostbyname);
#endif /* LWIP_DNS */

    netconn_alloc_adpt                              = LWIP_ROMFN(netconn_alloc);
    netconn_free_adpt                               = LWIP_ROMFN(netconn_free);
}



#if LWIP_RAW
/**
 * Receive callback function for RAW netconns.
 * Doesn't 'eat' the packet, only copies it and sends it to
 * conn->recvmbox
 *
 * @see raw.h (struct raw_pcb.recv) for parameters and return value
 */
u8_t
recv_raw(void *arg, struct raw_pcb *pcb, struct pbuf *p,
    const ip_addr_t *addr)
{
    return recv_raw_adpt(arg, pcb, p, addr);
}
#endif /* LWIP_RAW*/

#if LWIP_UDP
/**
 * Receive callback function for UDP netconns.
 * Posts the packet to conn->recvmbox or deletes it on memory error.
 *
 * @see udp.h (struct udp_pcb.recv) for parameters
 */
void
recv_udp(void *arg, struct udp_pcb *pcb, struct pbuf *p,
   const ip_addr_t *addr, u16_t port)
{
    recv_udp_adpt(arg, pcb, p, addr, port);
}
#endif /* LWIP_UDP */

#if LWIP_TCP
/**
 * Receive callback function for TCP netconns.
 * Posts the packet to conn->recvmbox, but doesn't delete it on errors.
 *
 * @see tcp.h (struct tcp_pcb.recv) for parameters and return value
 */
err_t
recv_tcp(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
    return recv_tcp_adpt(arg, pcb, p, err);
}

/**
 * Poll callback function for TCP netconns.
 * Wakes up an application thread that waits for a connection to close
 * or data to be sent. The application thread then takes the
 * appropriate action to go on.
 *
 * Signals the conn->sem.
 * netconn_close waits for conn->sem if closing failed.
 *
 * @see tcp.h (struct tcp_pcb.poll) for parameters and return value
 */
err_t
poll_tcp(void *arg, struct tcp_pcb *pcb)
{
    return poll_tcp_adpt(arg, pcb);
}

/**
 * Sent callback function for TCP netconns.
 * Signals the conn->sem and calls API_EVENT.
 * netconn_write waits for conn->sem if send buffer is low.
 *
 * @see tcp.h (struct tcp_pcb.sent) for parameters and return value
 */
err_t
sent_tcp(void *arg, struct tcp_pcb *pcb, u16_t len)
{
    return sent_tcp_adpt(arg, pcb, len);
}

/**
 * Error callback function for TCP netconns.
 * Signals conn->sem, posts to all conn mboxes and calls API_EVENT.
 * The application thread has then to decide what to do.
 *
 * @see tcp.h (struct tcp_pcb.err) for parameters
 */
void
err_tcp(void *arg, err_t err)
{
    err_tcp_adpt(arg, err);
}

/**
 * Setup a tcp_pcb with the correct callback function pointers
 * and their arguments.
 *
 * @param conn the TCP netconn to setup
 */
void
setup_tcp(struct netconn *conn)
{
    setup_tcp_adpt(conn);
}

/**
 * Accept callback function for TCP netconns.
 * Allocates a new netconn and posts that to conn->acceptmbox.
 *
 * @see tcp.h (struct tcp_pcb_listen.accept) for parameters and return value
 */
err_t
accept_function(void *arg, struct tcp_pcb *newpcb, err_t err)
{
    return accept_function_adpt(arg, newpcb, err);
}
#endif /* LWIP_TCP */

/**
 * Create a new pcb of a specific type.
 * Called from lwip_netconn_do_newconn().
 *
 * @param msg the api_msg_msg describing the connection type
 */
void
pcb_new(struct api_msg *msg)
{
    pcb_new_adpt(msg);
}

/**
 * Create a new pcb of a specific type inside a netconn.
 * Called from netconn_new_with_proto_and_callback.
 *
 * @param m the api_msg_msg describing the connection type
 */
void
lwip_netconn_do_newconn(void *m)
{
    lwip_netconn_do_newconn_adpt(m);
}

/**
 * Create a new netconn (of a specific type) that has a callback function.
 * The corresponding pcb is NOT created!
 *
 * @param t the type of 'connection' to create (@see enum netconn_type)
 * @param callback a function to call on status changes (RX available, TX'ed)
 * @return a newly allocated struct netconn or
 *         NULL on memory error
 */
struct netconn*
netconn_alloc(enum netconn_type t, netconn_callback callback)
{
    return netconn_alloc_adpt(t, callback);
}

/**
 * Delete a netconn and all its resources.
 * The pcb is NOT freed (since we might not be in the right thread context do this).
 *
 * @param conn the netconn to free
 */
void
netconn_free(struct netconn *conn)
{
    netconn_free_adpt(conn);
}

/**
 * Delete rcvmbox and acceptmbox of a netconn and free the left-over data in
 * these mboxes
 *
 * @param conn the netconn to free
 * @bytes_drained bytes drained from recvmbox
 * @accepts_drained pending connections drained from acceptmbox
 */
void
netconn_drain(struct netconn *conn)
{
    netconn_drain_adpt(conn);
}

#if LWIP_TCP
/**
 * Internal helper function to close a TCP netconn: since this sometimes
 * doesn't work at the first attempt, this function is called from multiple
 * places.
 *
 * @param conn the TCP netconn to close
 */
err_t
lwip_netconn_do_close_internal(struct netconn *conn  WRITE_DELAYED_PARAM)
{
    return lwip_netconn_do_close_internal_adpt(conn WRITE_DELAYED_ARG);
}
#endif /* LWIP_TCP */

/**
 * Delete the pcb inside a netconn.
 * Called from netconn_delete.
 *
 * @param m the api_msg_msg pointing to the connection
 */
void
lwip_netconn_do_delconn(void *m)
{
    lwip_netconn_do_delconn_adpt(m);
}

/**
 * Bind a pcb contained in a netconn
 * Called from netconn_bind.
 *
 * @param m the api_msg_msg pointing to the connection and containing
 *          the IP address and port to bind to
 */
void
lwip_netconn_do_bind(void *m)
{
    lwip_netconn_do_bind_adpt(m);
}

#if LWIP_TCP
/**
 * TCP callback function if a connection (opened by tcp_connect/lwip_netconn_do_connect) has
 * been established (or reset by the remote host).
 *
 * @see tcp.h (struct tcp_pcb.connected) for parameters and return values
 */
err_t
lwip_netconn_do_connected(void *arg, struct tcp_pcb *pcb, err_t err)
{
    return lwip_netconn_do_connected_adpt(arg, pcb, err);
}
#endif /* LWIP_TCP */

/**
 * Connect a pcb contained inside a netconn
 * Called from netconn_connect.
 *
 * @param m the api_msg_msg pointing to the connection and containing
 *          the IP address and port to connect to
 */
void
lwip_netconn_do_connect(void *m)
{
    lwip_netconn_do_connect_adpt(m);
}

/**
 * Disconnect a pcb contained inside a netconn
 * Only used for UDP netconns.
 * Called from netconn_disconnect.
 *
 * @param m the api_msg_msg pointing to the connection to disconnect
 */
void
lwip_netconn_do_disconnect(void *m)
{
    lwip_netconn_do_disconnect_adpt(m);
}

#if LWIP_TCP
/**
 * Set a TCP pcb contained in a netconn into listen mode
 * Called from netconn_listen.
 *
 * @param m the api_msg_msg pointing to the connection
 */
void
lwip_netconn_do_listen(void *m)
{
    lwip_netconn_do_listen_adpt(m);
}
#endif /* LWIP_TCP */

/**
 * Send some data on a RAW or UDP pcb contained in a netconn
 * Called from netconn_send
 *
 * @param m the api_msg_msg pointing to the connection
 */
void
lwip_netconn_do_send(void *m)
{
    lwip_netconn_do_send_adpt(m);
}

#if LWIP_TCP
/**
 * Indicate data has been received from a TCP pcb contained in a netconn
 * Called from netconn_recv
 *
 * @param m the api_msg_msg pointing to the connection
 */
void
lwip_netconn_do_recv(void *m)
{
    lwip_netconn_do_recv_adpt(m);
}

#if TCP_LISTEN_BACKLOG
/** Indicate that a TCP pcb has been accepted
 * Called from netconn_accept
 *
 * @param m the api_msg_msg pointing to the connection
 */
void
lwip_netconn_do_accepted(void *m)
{
    lwip_netconn_do_accepted_adpt(m);
}
#endif /* TCP_LISTEN_BACKLOG */

/**
 * See if more data needs to be written from a previous call to netconn_write.
 * Called initially from lwip_netconn_do_write. If the first call can't send all data
 * (because of low memory or empty send-buffer), this function is called again
 * from sent_tcp() or poll_tcp() to send more data. If all data is sent, the
 * blocking application thread (waiting in netconn_write) is released.
 *
 * @param conn netconn (that is currently in state NETCONN_WRITE) to process
 * @return ERR_OK
 *         ERR_MEM if LWIP_TCPIP_CORE_LOCKING=1 and sending hasn't yet finished
 */
err_t
lwip_netconn_do_writemore(struct netconn *conn  WRITE_DELAYED_PARAM)
{
    return lwip_netconn_do_writemore_adpt(conn  WRITE_DELAYED_ARG);
}
#endif /* LWIP_TCP */

/**
 * Send some data on a TCP pcb contained in a netconn
 * Called from netconn_write
 *
 * @param m the api_msg_msg pointing to the connection
 */
void
lwip_netconn_do_write(void *m)
{
    lwip_netconn_do_write_adpt(m);
}

/**
 * Return a connection's local or remote address
 * Called from netconn_getaddr
 *
 * @param m the api_msg_msg pointing to the connection
 */
void
lwip_netconn_do_getaddr(void *m)
{
    lwip_netconn_do_getaddr_adpt(m);
}

/**
 * Close or half-shutdown a TCP pcb contained in a netconn
 * Called from netconn_close
 * In contrast to closing sockets, the netconn is not deallocated.
 *
 * @param m the api_msg_msg pointing to the connection
 */
void
lwip_netconn_do_close(void *m)
{
    lwip_netconn_do_close_adpt(m);
}

#if LWIP_IGMP || (LWIP_IPV6 && LWIP_IPV6_MLD)
/**
 * Join multicast groups for UDP netconns.
 * Called from netconn_join_leave_group
 *
 * @param m the api_msg_msg pointing to the connection
 */
void
lwip_netconn_do_join_leave_group(void *m)
{
    lwip_netconn_do_join_leave_group_adpt(m);
}
#endif /* LWIP_IGMP || (LWIP_IPV6 && LWIP_IPV6_MLD) */

#if LWIP_DNS
/**
 * Callback function that is called when DNS name is resolved
 * (or on timeout). A waiting application thread is waked up by
 * signaling the semaphore.
 */
void
lwip_netconn_do_dns_found(const char *name, const ip_addr_t *ipaddr, void *arg)
{
    lwip_netconn_do_dns_found_adpt(name, ipaddr, arg);
}

/**
 * Execute a DNS query
 * Called from netconn_gethostbyname
 *
 * @param arg the dns_api_msg pointing to the query
 */
void
lwip_netconn_do_gethostbyname(void *arg)
{
    lwip_netconn_do_gethostbyname_adpt(arg);
}
#endif /* LWIP_DNS */

#endif /* LWIP_NETCONN */
