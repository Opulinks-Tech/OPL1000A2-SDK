/**
 * @file
 * Sequential API External module
 *
 * @defgroup netconn Netconn API
 * @ingroup sequential_api
 * Thread-safe, to be called from non-TCPIP threads only.
 * TX/RX handling based on @ref netbuf (containing @ref pbuf)
 * to avoid copying data around.
 *
 * @defgroup netconn_common Common functions
 * @ingroup netconn
 * For use with TCP and UDP
 *
 * @defgroup netconn_tcp TCP only
 * @ingroup netconn
 * TCP only functions
 *
 * @defgroup netconn_udp UDP only
 * @ingroup netconn
 * UDP only functions
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
 */

/* This is the part of the API that is linked with
   the application */

#include "lwip/opt.h"

#if LWIP_NETCONN /* don't build if not configured for use in lwipopts.h */

#include "lwip/api.h"
#include "lwip/memp.h"

#include "lwip/ip.h"
#include "lwip/raw.h"
#include "lwip/udp.h"
#include "lwip/priv/api_msg.h"
#include "lwip/priv/tcp_priv.h"
#include "lwip/priv/tcpip_priv.h"

#include <string.h>

//err_t netconn_close_shutdown(struct netconn *conn, u8_t how);


RET_DATA netconn_close_shutdown_fp_t                 netconn_close_shutdown_adpt;
RET_DATA netconn_apimsg_fp_t                         netconn_apimsg_adpt;
RET_DATA netconn_recv_data_fp_t                      netconn_recv_data_adpt;

RET_DATA netconn_new_with_proto_and_callback_fp_t    netconn_new_with_proto_and_callback_adpt;
RET_DATA netconn_delete_fp_t                         netconn_delete_adpt;
RET_DATA netconn_getaddr_fp_t                        netconn_getaddr_adpt;

RET_DATA netconn_bind_fp_t                           netconn_bind_adpt;
RET_DATA netconn_connect_fp_t                        netconn_connect_adpt;
RET_DATA netconn_disconnect_fp_t                     netconn_disconnect_adpt;
RET_DATA netconn_listen_with_backlog_fp_t            netconn_listen_with_backlog_adpt;

RET_DATA netconn_accept_fp_t                         netconn_accept_adpt;
RET_DATA netconn_recv_fp_t                           netconn_recv_adpt;
RET_DATA netconn_recv_tcp_pbuf_fp_t                  netconn_recv_tcp_pbuf_adpt;
RET_DATA netconn_sendto_fp_t                         netconn_sendto_adpt;
RET_DATA netconn_send_fp_t                           netconn_send_adpt;
RET_DATA netconn_write_partly_fp_t                   netconn_write_partly_adpt;

RET_DATA netconn_close_fp_t                          netconn_close_adpt;
RET_DATA netconn_shutdown_fp_t                       netconn_shutdown_adpt;
#if LWIP_IGMP || (LWIP_IPV6 && LWIP_IPV6_MLD)
RET_DATA netconn_join_leave_group_fp_t               netconn_join_leave_group_adpt;
#endif /* LWIP_IGMP || (LWIP_IPV6 && LWIP_IPV6_MLD) */

#if LWIP_DNS
#if LWIP_IPV4 && LWIP_IPV6
RET_DATA netconn_gethostbyname_addrtype_fp_t         netconn_gethostbyname_addrtype_adpt;
#else
RET_DATA netconn_gethostbyname_fp_t                  netconn_gethostbyname_adpt;
#endif
#endif

#if LWIP_NETCONN_SEM_PER_THREAD
RET_DATA netconn_thread_init_fp_t                    netconn_thread_init_adpt;
RET_DATA netconn_thread_cleanup_fp_t                 netconn_thread_cleanup_adpt;
#endif

void lwip_load_interface_api_lib(void)
{
    /* Private static API fucntion used in api_lib.c*/
    netconn_close_shutdown_adpt                         = LWIP_ROMFN(netconn_close_shutdown);  
    netconn_apimsg_adpt                                 = LWIP_ROMFN(netconn_apimsg);   
    netconn_recv_data_adpt                              = LWIP_ROMFN(netconn_recv_data);

    /* Pub API fucntion used in api_lib.c*/
    netconn_new_with_proto_and_callback_adpt            = LWIP_ROMFN(netconn_new_with_proto_and_callback);
    netconn_delete_adpt                                 = LWIP_ROMFN(netconn_delete);
    netconn_getaddr_adpt                                = LWIP_ROMFN(netconn_getaddr);

    netconn_bind_adpt                                   = LWIP_ROMFN(netconn_bind);
    netconn_connect_adpt                                = LWIP_ROMFN(netconn_connect);
    netconn_disconnect_adpt                             = LWIP_ROMFN(netconn_disconnect);
    netconn_listen_with_backlog_adpt                    = LWIP_ROMFN(netconn_listen_with_backlog);

    netconn_accept_adpt                                 = LWIP_ROMFN(netconn_accept);
    netconn_recv_adpt                                   = LWIP_ROMFN(netconn_recv);
    netconn_recv_tcp_pbuf_adpt                          = LWIP_ROMFN(netconn_recv_tcp_pbuf);
    netconn_sendto_adpt                                 = LWIP_ROMFN(netconn_sendto);
    netconn_send_adpt                                   = LWIP_ROMFN(netconn_send);
    netconn_write_partly_adpt                           = LWIP_ROMFN(netconn_write_partly);

    netconn_close_adpt                                  = LWIP_ROMFN(netconn_close);
    netconn_shutdown_adpt                               = LWIP_ROMFN(netconn_shutdown);
#if LWIP_IGMP || (LWIP_IPV6 && LWIP_IPV6_MLD)
    netconn_join_leave_group_adpt                       = LWIP_ROMFN(netconn_join_leave_group);
#endif

#if LWIP_DNS
#if LWIP_IPV4 && LWIP_IPV6
    netconn_gethostbyname_addrtype_adpt                 = LWIP_ROMFN(netconn_gethostbyname_addrtype);
#else
    netconn_gethostbyname_adpt                          = LWIP_ROMFN(netconn_gethostbyname);
#endif
#endif

#if LWIP_NETCONN_SEM_PER_THREAD
    netconn_thread_init_adpt                            = LWIP_ROMFN(netconn_thread_init);
    netconn_thread_cleanup_adpt                         = LWIP_ROMFN(netconn_thread_cleanup);
#endif

}


/**
 * Call the lower part of a netconn_* function
 * This function is then running in the thread context
 * of tcpip_thread and has exclusive access to lwIP core code.
 *
 * @param fn function to call
 * @param apimsg a struct containing the function to call and its parameters
 * @return ERR_OK if the function was called, another err_t if not
 */
err_t
netconn_apimsg(tcpip_callback_fn fn, struct api_msg *apimsg)
{
    return netconn_apimsg_adpt(fn, apimsg);
}

/**
 * Create a new netconn (of a specific type) that has a callback function.
 * The corresponding pcb is also created.
 *
 * @param t the type of 'connection' to create (@see enum netconn_type)
 * @param proto the IP protocol for RAW IP pcbs
 * @param callback a function to call on status changes (RX available, TX'ed)
 * @return a newly allocated struct netconn or
 *         NULL on memory error
 */
struct netconn*
netconn_new_with_proto_and_callback(enum netconn_type t, u8_t proto, netconn_callback callback)
{
    return netconn_new_with_proto_and_callback_adpt(t, proto, callback);
}

/**
 * @ingroup netconn_common
 * Close a netconn 'connection' and free its resources.
 * UDP and RAW connection are completely closed, TCP pcbs might still be in a waitstate
 * after this returns.
 *
 * @param conn the netconn to delete
 * @return ERR_OK if the connection was deleted
 */
err_t
netconn_delete(struct netconn *conn)
{
    return netconn_delete_adpt(conn);
}

/**
 * Get the local or remote IP address and port of a netconn.
 * For RAW netconns, this returns the protocol instead of a port!
 *
 * @param conn the netconn to query
 * @param addr a pointer to which to save the IP address
 * @param port a pointer to which to save the port (or protocol for RAW)
 * @param local 1 to get the local IP address, 0 to get the remote one
 * @return ERR_CONN for invalid connections
 *         ERR_OK if the information was retrieved
 */
err_t
netconn_getaddr(struct netconn *conn, ip_addr_t *addr, u16_t *port, u8_t local)
{
    return netconn_getaddr_adpt(conn, addr, port, local);
}

/**
 * @ingroup netconn_common
 * Bind a netconn to a specific local IP address and port.
 * Binding one netconn twice might not always be checked correctly!
 *
 * @param conn the netconn to bind
 * @param addr the local IP address to bind the netconn to
 *             (use IP4_ADDR_ANY/IP6_ADDR_ANY to bind to all addresses)
 * @param port the local port to bind the netconn to (not used for RAW)
 * @return ERR_OK if bound, any other err_t on failure
 */
err_t
netconn_bind(struct netconn *conn, const ip_addr_t *addr, u16_t port)
{
    return netconn_bind_adpt(conn, addr, port);
}

/**
 * @ingroup netconn_common
 * Connect a netconn to a specific remote IP address and port.
 *
 * @param conn the netconn to connect
 * @param addr the remote IP address to connect to
 * @param port the remote port to connect to (no used for RAW)
 * @return ERR_OK if connected, return value of tcp_/udp_/raw_connect otherwise
 */
err_t
netconn_connect(struct netconn *conn, const ip_addr_t *addr, u16_t port)
{
    return netconn_connect_adpt(conn, addr, port);
}

/**
 * @ingroup netconn_udp
 * Disconnect a netconn from its current peer (only valid for UDP netconns).
 *
 * @param conn the netconn to disconnect
 * @return See @ref err_t
 */
err_t
netconn_disconnect(struct netconn *conn)
{
    return netconn_disconnect_adpt(conn);
}

/**
 * @ingroup netconn_tcp
 * Set a TCP netconn into listen mode
 *
 * @param conn the tcp netconn to set to listen mode
 * @param backlog the listen backlog, only used if TCP_LISTEN_BACKLOG==1
 * @return ERR_OK if the netconn was set to listen (UDP and RAW netconns
 *         don't return any error (yet?))
 */
err_t
netconn_listen_with_backlog(struct netconn *conn, u8_t backlog)
{
    return netconn_listen_with_backlog_adpt(conn, backlog);
}

/**
 * @ingroup netconn_tcp
 * Accept a new connection on a TCP listening netconn.
 *
 * @param conn the TCP listen netconn
 * @param new_conn pointer where the new connection is stored
 * @return ERR_OK if a new connection has been received or an error
 *                code otherwise
 */
err_t
netconn_accept(struct netconn *conn, struct netconn **new_conn)
{
    return netconn_accept_adpt(conn, new_conn);
}

/**
 * @ingroup netconn_common
 * Receive data: actual implementation that doesn't care whether pbuf or netbuf
 * is received
 *
 * @param conn the netconn from which to receive data
 * @param new_buf pointer where a new pbuf/netbuf is stored when received data
 * @return ERR_OK if data has been received, an error code otherwise (timeout,
 *                memory error or another error)
 */
err_t
netconn_recv_data(struct netconn *conn, void **new_buf)
{
    return netconn_recv_data_adpt(conn, new_buf);
}

/**
 * @ingroup netconn_tcp
 * Receive data (in form of a pbuf) from a TCP netconn
 *
 * @param conn the netconn from which to receive data
 * @param new_buf pointer where a new pbuf is stored when received data
 * @return ERR_OK if data has been received, an error code otherwise (timeout,
 *                memory error or another error)
 *         ERR_ARG if conn is not a TCP netconn
 */
err_t
netconn_recv_tcp_pbuf(struct netconn *conn, struct pbuf **new_buf)
{
    return netconn_recv_tcp_pbuf_adpt(conn, new_buf);
}

/**
 * @ingroup netconn_common
 * Receive data (in form of a netbuf containing a packet buffer) from a netconn
 *
 * @param conn the netconn from which to receive data
 * @param new_buf pointer where a new netbuf is stored when received data
 * @return ERR_OK if data has been received, an error code otherwise (timeout,
 *                memory error or another error)
 */
err_t
netconn_recv(struct netconn *conn, struct netbuf **new_buf)
{
    return netconn_recv_adpt(conn, new_buf);
}

/**
 * @ingroup netconn_udp
 * Send data (in form of a netbuf) to a specific remote IP address and port.
 * Only to be used for UDP and RAW netconns (not TCP).
 *
 * @param conn the netconn over which to send data
 * @param buf a netbuf containing the data to send
 * @param addr the remote IP address to which to send the data
 * @param port the remote port to which to send the data
 * @return ERR_OK if data was sent, any other err_t on error
 */
err_t
netconn_sendto(struct netconn *conn, struct netbuf *buf, const ip_addr_t *addr, u16_t port)
{
    return netconn_sendto_adpt(conn, buf, addr, port);
}

/**
 * @ingroup netconn_udp
 * Send data over a UDP or RAW netconn (that is already connected).
 *
 * @param conn the UDP or RAW netconn over which to send data
 * @param buf a netbuf containing the data to send
 * @return ERR_OK if data was sent, any other err_t on error
 */
err_t
netconn_send(struct netconn *conn, struct netbuf *buf)
{
    return netconn_send_adpt(conn, buf);
}

/**
 * @ingroup netconn_tcp
 * Send data over a TCP netconn.
 *
 * @param conn the TCP netconn over which to send data
 * @param dataptr pointer to the application buffer that contains the data to send
 * @param size size of the application data to send
 * @param apiflags combination of following flags :
 * - NETCONN_COPY: data will be copied into memory belonging to the stack
 * - NETCONN_MORE: for TCP connection, PSH flag will be set on last segment sent
 * - NETCONN_DONTBLOCK: only write the data if all data can be written at once
 * @param bytes_written pointer to a location that receives the number of written bytes
 * @return ERR_OK if data was sent, any other err_t on error
 */
err_t
netconn_write_partly(struct netconn *conn, const void *dataptr, size_t size,
                     u8_t apiflags, size_t *bytes_written)
{
    return netconn_write_partly_adpt(conn, dataptr, size, apiflags, bytes_written);
}

/**
 * @ingroup netconn_tcp
 * Close or shutdown a TCP netconn (doesn't delete it).
 *
 * @param conn the TCP netconn to close or shutdown
 * @param how fully close or only shutdown one side?
 * @return ERR_OK if the netconn was closed, any other err_t on error
 */
err_t
netconn_close_shutdown(struct netconn *conn, u8_t how)
{
    return netconn_close_shutdown_adpt(conn, how);
}

/**
 * @ingroup netconn_tcp
 * Close a TCP netconn (doesn't delete it).
 *
 * @param conn the TCP netconn to close
 * @return ERR_OK if the netconn was closed, any other err_t on error
 */
err_t
netconn_close(struct netconn *conn)
{
    /* shutting down both ends is the same as closing */
    return netconn_close_adpt(conn);
}

/**
 * @ingroup netconn_tcp
 * Shut down one or both sides of a TCP netconn (doesn't delete it).
 *
 * @param conn the TCP netconn to shut down
 * @param shut_rx shut down the RX side (no more read possible after this)
 * @param shut_tx shut down the TX side (no more write possible after this)
 * @return ERR_OK if the netconn was closed, any other err_t on error
 */
err_t
netconn_shutdown(struct netconn *conn, u8_t shut_rx, u8_t shut_tx)
{
    return netconn_shutdown_adpt(conn, shut_rx, shut_tx);
}

#if LWIP_IGMP || (LWIP_IPV6 && LWIP_IPV6_MLD)
/**
 * @ingroup netconn_udp
 * Join multicast groups for UDP netconns.
 *
 * @param conn the UDP netconn for which to change multicast addresses
 * @param multiaddr IP address of the multicast group to join or leave
 * @param netif_addr the IP address of the network interface on which to send
 *                  the igmp message
 * @param join_or_leave flag whether to send a join- or leave-message
 * @return ERR_OK if the action was taken, any err_t on error
 */
err_t
netconn_join_leave_group(struct netconn *conn,
                         const ip_addr_t *multiaddr,
                         const ip_addr_t *netif_addr,
                         enum netconn_igmp join_or_leave)
{
    return netconn_join_leave_group_adpt(conn, multiaddr, netif_addr, join_or_leave);
}
#endif /* LWIP_IGMP || (LWIP_IPV6 && LWIP_IPV6_MLD) */

#if LWIP_DNS
/**
 * @ingroup netconn_common
 * Execute a DNS query, only one IP address is returned
 *
 * @param name a string representation of the DNS host name to query
 * @param addr a preallocated ip_addr_t where to store the resolved IP address
 * @param dns_addrtype IP address type (IPv4 / IPv6)
 * @return ERR_OK: resolving succeeded
 *         ERR_MEM: memory error, try again later
 *         ERR_ARG: dns client not initialized or invalid hostname
 *         ERR_VAL: dns server response was invalid
 */
#if LWIP_IPV4 && LWIP_IPV6
err_t
netconn_gethostbyname_addrtype(const char *name, ip_addr_t *addr, u8_t dns_addrtype)
{
    return netconn_gethostbyname_addrtype_adpt(name, addr, dns_addrtype);
}

#else
err_t
netconn_gethostbyname(const char *name, ip_addr_t *addr)
{
    return netconn_gethostbyname_adpt(name, addr);
}
#endif
#endif /* LWIP_DNS*/

#if LWIP_NETCONN_SEM_PER_THREAD
void
netconn_thread_init(void)
{
    netconn_thread_init_adpt();
}

void
netconn_thread_cleanup(void)
{
    netconn_thread_cleanup_adpt();
}
#endif /* LWIP_NETCONN_SEM_PER_THREAD */

#endif /* LWIP_NETCONN */
