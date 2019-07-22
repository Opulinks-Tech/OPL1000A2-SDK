/**
 * @file
 * Transmission Control Protocol for IP
 * See also @ref tcp_raw
 *
 * @defgroup tcp_raw TCP
 * @ingroup callbackstyle_api
 * Transmission Control Protocol for IP\n
 * @see @ref raw_api and @ref netconn
 *
 * Common functions for the TCP implementation, such as functinos
 * for manipulating the data structures and the TCP timer functions. TCP functions
 * related to input and output is found in tcp_in.c and tcp_out.c respectively.\n
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

#if defined(LWIP_ROMBUILD)

#if LWIP_TCP /* don't build if not configured for use in lwipopts.h */

#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/tcp.h"
#include "lwip/priv/tcp_priv.h"
#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/ip6.h"
#include "lwip/ip6_addr.h"
#include "lwip/nd6.h"

#include <string.h>

#ifdef LWIP_HOOK_FILENAME
#include LWIP_HOOK_FILENAME
#endif


/** External function (implemented in tcp_out.c) */
extern err_t LWIP_ROMFN(tcp_write)(struct tcp_pcb *pcb, const void *dataptr, u16_t len, u8_t apiflags);
extern err_t LWIP_ROMFN(tcp_output)(struct tcp_pcb *pcb);
extern err_t LWIP_ROMFN(tcp_send_empty_ack)(struct tcp_pcb *pcb);
extern void  LWIP_ROMFN(tcp_rexmit)(struct tcp_pcb *pcb);
extern void  LWIP_ROMFN(tcp_rexmit_rto)(struct tcp_pcb *pcb);
extern void  LWIP_ROMFN(tcp_rexmit_fast)(struct tcp_pcb *pcb);
extern void  LWIP_ROMFN(tcp_rst)(u32_t seqno, u32_t ackno, const ip_addr_t *local_ip, const ip_addr_t *remote_ip, u16_t local_port, u16_t remote_port);
extern err_t LWIP_ROMFN(tcp_send_fin)(struct tcp_pcb *pcb);
extern err_t LWIP_ROMFN(tcp_enqueue_flags)(struct tcp_pcb *pcb, u8_t flags);
extern err_t LWIP_ROMFN(tcp_keepalive)(struct tcp_pcb *pcb);
extern err_t LWIP_ROMFN(tcp_zero_window_probe)(struct tcp_pcb *pcb);

/** External function (implemented in tcp_in.c) */
extern void  LWIP_ROMFN(tcp_input)(struct pbuf *p, struct netif *inp);
extern void  LWIP_ROMFN(tcp_trigger_input_pcb_close)(void);

/** External function (implemented in tcp_in_if.c tcp_out_if.c) */
extern void lwip_load_interface_tcp_in(void);
extern void lwip_load_interface_tcp_out(void);


/*-------------------------------------------------------------------------------------
 * Definitions of interface function pointer
 *------------------------------------------------------------------------------------*/

/* Private static API fucntion used in tcp_in.c*/
RET_DATA tcp_process_fp_t               tcp_process_adpt;
RET_DATA tcp_receive_fp_t               tcp_receive_adpt;
RET_DATA tcp_parseopt_fp_t              tcp_parseopt_adpt;
RET_DATA tcp_listen_input_fp_t          tcp_listen_input_adpt;
RET_DATA tcp_timewait_input_fp_t        tcp_timewait_input_adpt;
RET_DATA tcp_input_delayed_close_fp_t   tcp_input_delayed_close_adpt;
RET_DATA tcp_oos_insert_segment_fp_t    tcp_oos_insert_segment_adpt;
RET_DATA tcp_getoptbyte_fp_t            tcp_getoptbyte_adpt;


RET_DATA tcp_output_alloc_header_fp_t       tcp_output_alloc_header_adpt;
RET_DATA tcp_create_segment_fp_t            tcp_create_segment_adpt;
RET_DATA tcp_pbuf_prealloc_fp_t             tcp_pbuf_prealloc_adpt;
#if TCP_CHECKSUM_ON_COPY
RET_DATA tcp_seg_add_chksum_fp_t            tcp_seg_add_chksum_adpt;
#endif
RET_DATA tcp_write_checks_fp_t              tcp_write_checks_adpt;
#if LWIP_TCP_TIMESTAMPS
RET_DATA tcp_build_timestamp_option_fp_t    tcp_build_timestamp_option_adpt;
#endif
#if LWIP_WND_SCALE
RET_DATA tcp_build_wnd_scale_option_fp_t    tcp_build_wnd_scale_option_adpt;
#endif
RET_DATA tcp_output_segment_fp_t            tcp_output_segment_adpt;


/* Private static API fucntion used in tcp.c*/
#if LWIP_CALLBACK_API || TCP_LISTEN_BACKLOG
RET_DATA tcp_remove_listener_fp_t       tcp_remove_listener_adpt;
#endif
RET_DATA tcp_listen_closed_fp_t         tcp_listen_closed_adpt;
RET_DATA tcp_close_shutdown_fp_t        tcp_close_shutdown_adpt;
RET_DATA tcp_close_shutdown_fin_fp_t    tcp_close_shutdown_fin_adpt;
RET_DATA tcp_new_port_fp_t              tcp_new_port_adpt;
RET_DATA tcp_kill_prio_fp_t             tcp_kill_prio_adpt;
RET_DATA tcp_kill_state_fp_t            tcp_kill_state_adpt;
RET_DATA tcp_kill_timewait_fp_t         tcp_kill_timewait_adpt;
RET_DATA tcp_netif_ip_addr_changed_pcblist_fp_t tcp_netif_ip_addr_changed_pcblist_adpt;

/* Functions for interfacing with TCP: */
/* Take form tcp_prv.h, implement in tcp.c */
RET_DATA tcp_init_fp_t                  tcp_init_adpt;
RET_DATA tcp_tmr_fp_t                   tcp_tmr_adpt;
RET_DATA tcp_slowtmr_fp_t               tcp_slowtmr_adpt;
RET_DATA tcp_fasttmr_fp_t               tcp_fasttmr_adpt;
RET_DATA tcp_txnow_fp_t                 tcp_txnow_adpt;
RET_DATA tcp_input_fp_t                 tcp_input_adpt;
RET_DATA tcp_alloc_fp_t                 tcp_alloc_adpt;
RET_DATA tcp_abandon_fp_t               tcp_abandon_adpt;
RET_DATA tcp_send_empty_ack_fp_t        tcp_send_empty_ack_adpt;
RET_DATA tcp_rexmit_fp_t                tcp_rexmit_adpt;
RET_DATA tcp_rexmit_rto_fp_t            tcp_rexmit_rto_adpt;
RET_DATA tcp_rexmit_fast_fp_t           tcp_rexmit_fast_adpt;
RET_DATA tcp_update_rcv_ann_wnd_fp_t    tcp_update_rcv_ann_wnd_adpt;
RET_DATA tcp_process_refused_data_fp_t  tcp_process_refused_data_adpt;
RET_DATA tcp_pcb_purge_fp_t             tcp_pcb_purge_adpt;
RET_DATA tcp_pcb_remove_fp_t            tcp_pcb_remove_adpt;
RET_DATA tcp_segs_free_fp_t             tcp_segs_free_adpt;
RET_DATA tcp_seg_free_fp_t              tcp_seg_free_adpt;
#if TCP_QUEUE_OOSEQ
RET_DATA tcp_seg_copy_fp_t              tcp_seg_copy_adpt;
#endif

RET_DATA tcp_rst_fp_t                   tcp_rst_adpt;
RET_DATA tcp_send_fin_fp_t              tcp_send_fin_adpt;
RET_DATA tcp_enqueue_flags_fp_t         tcp_enqueue_flags_adpt;
//RET_DATA tcp_rexmit_seg_fp_t            tcp_rexmit_seg_adpt;
RET_DATA tcp_next_iss_fp_t              tcp_next_iss_adpt;
RET_DATA tcp_keepalive_fp_t             tcp_keepalive_adpt;
RET_DATA tcp_zero_window_probe_fp_t     tcp_zero_window_probe_adpt;
RET_DATA tcp_trigger_input_pcb_close_fp_t   tcp_trigger_input_pcb_close_adpt;

#if TCP_CALCULATE_EFF_SEND_MSS
RET_DATA tcp_eff_send_mss_impl_fp_t     tcp_eff_send_mss_impl_adpt;
#endif
#if LWIP_CALLBACK_API
RET_DATA tcp_recv_null_fp_t             tcp_recv_null_adpt;
RET_DATA tcp_accept_null_fp_t           tcp_accept_null_adpt;
#endif

RET_DATA tcp_netif_ip_addr_changed_fp_t tcp_netif_ip_addr_changed_adpt;


RET_DATA tcp_new_fp_t                   tcp_new_adpt;
RET_DATA tcp_new_ip_type_fp_t           tcp_new_ip_type_adpt;
RET_DATA tcp_arg_fp_t                   tcp_arg_adpt;
#if LWIP_CALLBACK_API
RET_DATA tcp_recv_fp_t                  tcp_recv_adpt;
RET_DATA tcp_sent_fp_t                  tcp_sent_adpt;
RET_DATA tcp_err_fp_t                   tcp_err_adpt;
RET_DATA tcp_accept_fp_t                tcp_accept_adpt;
#endif
RET_DATA tcp_poll_fp_t                  tcp_poll_adpt;


#if TCP_LISTEN_BACKLOG
RET_DATA tcp_backlog_delayed_fp_t       tcp_backlog_delayed_adpt;
RET_DATA tcp_backlog_accepted_fp_t      tcp_backlog_accepted_adpt;
#endif  /* TCP_LISTEN_BACKLOG */

RET_DATA tcp_recved_fp_t                tcp_recved_adpt;
RET_DATA tcp_bind_fp_t                  tcp_bind_adpt;
RET_DATA tcp_connect_fp_t               tcp_connect_adpt;
RET_DATA tcp_abort_fp_t                 tcp_abort_adpt;
RET_DATA tcp_close_fp_t                 tcp_close_adpt;
RET_DATA tcp_shutdown_fp_t              tcp_shutdown_adpt;
RET_DATA tcp_write_fp_t                 tcp_write_adpt;
RET_DATA tcp_setprio_fp_t               tcp_setprio_adpt;
RET_DATA tcp_output_fp_t                tcp_output_adpt;
RET_DATA tcp_listen_with_backlog_and_err_fp_t    tcp_listen_with_backlog_and_err_adpt;
RET_DATA tcp_listen_with_backlog_fp_t            tcp_listen_with_backlog_adpt;

/*-------------------------------------------------------------------------------------
 * Interface assignment
 *------------------------------------------------------------------------------------*/
void lwip_load_interface_tcp(void)
{

    /* Cold boot initialization for "zero_init" retention data */
    /* last local TCP port */
    tcp_port = TCP_LOCAL_PORT_RANGE_START;
    /* Incremented every coarse grained timer shot (typically every 500 ms). */
    tcp_ticks = 0;
    iss = 6510;

    /* The TCP PCB lists. */
    tcp_bound_pcbs = NULL;
    memset((void*)&tcp_listen_pcbs, 0, sizeof(tcp_listen_pcbs));
    tcp_active_pcbs = NULL;
    tcp_tw_pcbs = NULL;
    tcp_active_pcbs_changed = 0;
    tcp_timer = 0;
    tcp_timer_ctr = 0;


    lwip_load_interface_tcp_in(); //static private func in tcp_in.c

    lwip_load_interface_tcp_out();

#if LWIP_CALLBACK_API || TCP_LISTEN_BACKLOG
    tcp_remove_listener_adpt                    = LWIP_ROMFN(tcp_remove_listener);
#endif
    tcp_listen_closed_adpt                      = LWIP_ROMFN(tcp_listen_closed);
    tcp_close_shutdown_adpt                     = LWIP_ROMFN(tcp_close_shutdown);
    tcp_close_shutdown_fin_adpt                 = LWIP_ROMFN(tcp_close_shutdown_fin);
    tcp_new_port_adpt                           = LWIP_ROMFN(tcp_new_port);
    tcp_kill_prio_adpt                          = LWIP_ROMFN(tcp_kill_prio);
    tcp_kill_state_adpt                         = LWIP_ROMFN(tcp_kill_state);
    tcp_kill_timewait_adpt                      = LWIP_ROMFN(tcp_kill_timewait);
    tcp_netif_ip_addr_changed_pcblist_adpt      = LWIP_ROMFN(tcp_netif_ip_addr_changed_pcblist);

    /* Functions for interfacing with TCP:Take form tcp_prv.h */
    /* implemetation in tcp.c, tcp_in.c, tcp_out */
    tcp_init_adpt                               = LWIP_ROMFN(tcp_init);
    tcp_tmr_adpt                                = LWIP_ROMFN(tcp_tmr);
    tcp_slowtmr_adpt                            = LWIP_ROMFN(tcp_slowtmr);
    tcp_fasttmr_adpt                            = LWIP_ROMFN(tcp_fasttmr);
    tcp_txnow_adpt                              = LWIP_ROMFN(tcp_txnow);
    tcp_input_adpt                              = LWIP_ROMFN(tcp_input);
    tcp_alloc_adpt                              = LWIP_ROMFN(tcp_alloc);
    tcp_abandon_adpt                            = LWIP_ROMFN(tcp_abandon);
    tcp_send_empty_ack_adpt                     = LWIP_ROMFN(tcp_send_empty_ack);
    tcp_rexmit_adpt                             = LWIP_ROMFN(tcp_rexmit);
    tcp_rexmit_rto_adpt                         = LWIP_ROMFN(tcp_rexmit_rto);
    tcp_rexmit_fast_adpt                        = LWIP_ROMFN(tcp_rexmit_fast);
    tcp_update_rcv_ann_wnd_adpt                 = LWIP_ROMFN(tcp_update_rcv_ann_wnd);
    tcp_process_refused_data_adpt               = LWIP_ROMFN(tcp_process_refused_data);
    tcp_pcb_purge_adpt                          = LWIP_ROMFN(tcp_pcb_purge);
    tcp_pcb_remove_adpt                         = LWIP_ROMFN(tcp_pcb_remove);
    tcp_segs_free_adpt                          = LWIP_ROMFN(tcp_segs_free);
    tcp_seg_free_adpt                           = LWIP_ROMFN(tcp_seg_free);
#if TCP_QUEUE_OOSEQ
    tcp_seg_copy_adpt                           = LWIP_ROMFN(tcp_seg_copy);
#endif

    tcp_rst_adpt                                = LWIP_ROMFN(tcp_rst);
    tcp_send_fin_adpt                           = LWIP_ROMFN(tcp_send_fin);
    tcp_enqueue_flags_adpt                      = LWIP_ROMFN(tcp_enqueue_flags);
    //tcp_rexmit_seg_adpt                         = LWIP_ROMFN(tcp_rexmit_seg);
    tcp_next_iss_adpt                           = LWIP_ROMFN(tcp_next_iss);
    tcp_keepalive_adpt                          = LWIP_ROMFN(tcp_keepalive);
    tcp_zero_window_probe_adpt                  = LWIP_ROMFN(tcp_zero_window_probe);
    tcp_trigger_input_pcb_close_adpt            = LWIP_ROMFN(tcp_trigger_input_pcb_close);
#if TCP_CALCULATE_EFF_SEND_MSS
    tcp_eff_send_mss_impl_adpt                  = LWIP_ROMFN(tcp_eff_send_mss_impl);
#endif
#if LWIP_CALLBACK_API
    tcp_recv_null_adpt                          = LWIP_ROMFN(tcp_recv_null);
    tcp_accept_null_adpt                        = LWIP_ROMFN(tcp_accept_null);
#endif

    tcp_netif_ip_addr_changed_adpt              = LWIP_ROMFN(tcp_netif_ip_addr_changed);


    /* Application program's interface: */
    tcp_new_adpt                                = LWIP_ROMFN(tcp_new);
    tcp_new_ip_type_adpt                        = LWIP_ROMFN(tcp_new_ip_type);
#if LWIP_CALLBACK_API
    tcp_arg_adpt                                = LWIP_ROMFN(tcp_arg);
    tcp_recv_adpt                               = LWIP_ROMFN(tcp_recv);
    tcp_sent_adpt                               = LWIP_ROMFN(tcp_sent);
    tcp_err_adpt                                = LWIP_ROMFN(tcp_err);
    tcp_accept_adpt                             = LWIP_ROMFN(tcp_accept);
#endif
    tcp_poll_adpt                               = LWIP_ROMFN(tcp_poll);
#if TCP_LISTEN_BACKLOG
    tcp_backlog_delayed_adpt                    = LWIP_ROMFN(tcp_backlog_delayed);
    tcp_backlog_accepted_adpt                   = LWIP_ROMFN(tcp_backlog_accepted);
#endif  /* TCP_LISTEN_BACKLOG */

    tcp_recved_adpt                             = LWIP_ROMFN(tcp_recved);
    tcp_bind_adpt                               = LWIP_ROMFN(tcp_bind);
    tcp_connect_adpt                            = LWIP_ROMFN(tcp_connect);
    tcp_abort_adpt                              = LWIP_ROMFN(tcp_abort);
    tcp_close_adpt                              = LWIP_ROMFN(tcp_close);
    tcp_shutdown_adpt                           = LWIP_ROMFN(tcp_shutdown);
    tcp_write_adpt                              = LWIP_ROMFN(tcp_write);
    tcp_setprio_adpt                            = LWIP_ROMFN(tcp_setprio);
    tcp_output_adpt                             = LWIP_ROMFN(tcp_output);
    tcp_listen_with_backlog_and_err_adpt        = LWIP_ROMFN(tcp_listen_with_backlog_and_err);
    tcp_listen_with_backlog_adpt                = LWIP_ROMFN(tcp_listen_with_backlog);
}



/**
 * Initialize this module.
 */
void
tcp_init(void)
{
    tcp_init_adpt();
}

/**
 * Called periodically to dispatch TCP timers.
 */
void
tcp_tmr(void)
{
    tcp_tmr_adpt();
}

#if LWIP_CALLBACK_API || TCP_LISTEN_BACKLOG
/** Called when a listen pcb is closed. Iterates one pcb list and removes the
 * closed listener pcb from pcb->listener if matching.
 */
void
tcp_remove_listener(struct tcp_pcb *list, struct tcp_pcb_listen *lpcb)
{
    tcp_remove_listener_adpt(list, lpcb);
}
#endif

/** Called when a listen pcb is closed. Iterates all pcb lists and removes the
 * closed listener pcb from pcb->listener if matching.
 */
void
tcp_listen_closed(struct tcp_pcb *pcb)
{
    tcp_listen_closed_adpt(pcb);
}


#if TCP_LISTEN_BACKLOG
/** @ingroup tcp_raw
 * Delay accepting a connection in respect to the listen backlog:
 * the number of outstanding connections is increased until
 * tcp_backlog_accepted() is called.
 *
 * ATTENTION: the caller is responsible for calling tcp_backlog_accepted()
 * or else the backlog feature will get out of sync!
 *
 * @param pcb the connection pcb which is not fully accepted yet
 */
void
tcp_backlog_delayed(struct tcp_pcb* pcb)
{
    tcp_backlog_delayed_adpt(pcb);
}

/** @ingroup tcp_raw
 * A delayed-accept a connection is accepted (or closed/aborted): decreases
 * the number of outstanding connections after calling tcp_backlog_delayed().
 *
 * ATTENTION: the caller is responsible for calling tcp_backlog_accepted()
 * or else the backlog feature will get out of sync!
 *
 * @param pcb the connection pcb which is now fully accepted (or closed/aborted)
 */
void
tcp_backlog_accepted(struct tcp_pcb* pcb)
{
    tcp_backlog_accepted_adpt(pcb);
}
#endif /* TCP_LISTEN_BACKLOG */

/**
 * Closes the TX side of a connection held by the PCB.
 * For tcp_close(), a RST is sent if the application didn't receive all data
 * (tcp_recved() not called for all data passed to recv callback).
 *
 * Listening pcbs are freed and may not be referenced any more.
 * Connection pcbs are freed if not yet connected and may not be referenced
 * any more. If a connection is established (at least SYN received or in
 * a closing state), the connection is closed, and put in a closing state.
 * The pcb is then automatically freed in tcp_slowtmr(). It is therefore
 * unsafe to reference it.
 *
 * @param pcb the tcp_pcb to close
 * @return ERR_OK if connection has been closed
 *         another err_t if closing failed and pcb is not freed
 */
err_t
tcp_close_shutdown(struct tcp_pcb *pcb, u8_t rst_on_unacked_data)
{
    return tcp_close_shutdown_adpt(pcb, rst_on_unacked_data);
}

err_t
tcp_close_shutdown_fin(struct tcp_pcb *pcb)
{
    return tcp_close_shutdown_fin_adpt(pcb);
}

/**
 * @ingroup tcp_raw
 * Closes the connection held by the PCB.
 *
 * Listening pcbs are freed and may not be referenced any more.
 * Connection pcbs are freed if not yet connected and may not be referenced
 * any more. If a connection is established (at least SYN received or in
 * a closing state), the connection is closed, and put in a closing state.
 * The pcb is then automatically freed in tcp_slowtmr(). It is therefore
 * unsafe to reference it (unless an error is returned).
 *
 * @param pcb the tcp_pcb to close
 * @return ERR_OK if connection has been closed
 *         another err_t if closing failed and pcb is not freed
 */
err_t
tcp_close(struct tcp_pcb *pcb)
{
    return tcp_close_adpt(pcb);
}

/**
 * @ingroup tcp_raw
 * Causes all or part of a full-duplex connection of this PCB to be shut down.
 * This doesn't deallocate the PCB unless shutting down both sides!
 * Shutting down both sides is the same as calling tcp_close, so if it succeds
 * (i.e. returns ER_OK), the PCB must not be referenced any more!
 *
 * @param pcb PCB to shutdown
 * @param shut_rx shut down receive side if this is != 0
 * @param shut_tx shut down send side if this is != 0
 * @return ERR_OK if shutdown succeeded (or the PCB has already been shut down)
 *         another err_t on error.
 */
err_t
tcp_shutdown(struct tcp_pcb *pcb, int shut_rx, int shut_tx)
{
    return tcp_shutdown_adpt(pcb, shut_rx, shut_tx);
}

/**
 * Abandons a connection and optionally sends a RST to the remote
 * host.  Deletes the local protocol control block. This is done when
 * a connection is killed because of shortage of memory.
 *
 * @param pcb the tcp_pcb to abort
 * @param reset boolean to indicate whether a reset should be sent
 */
void
tcp_abandon(struct tcp_pcb *pcb, int reset)
{
    tcp_abandon_adpt(pcb, reset);
}

/**
 * @ingroup tcp_raw
 * Aborts the connection by sending a RST (reset) segment to the remote
 * host. The pcb is deallocated. This function never fails.
 *
 * ATTENTION: When calling this from one of the TCP callbacks, make
 * sure you always return ERR_ABRT (and never return ERR_ABRT otherwise
 * or you will risk accessing deallocated memory or memory leaks!
 *
 * @param pcb the tcp pcb to abort
 */
void
tcp_abort(struct tcp_pcb *pcb)
{
    tcp_abort_adpt(pcb);
}

/**
 * @ingroup tcp_raw
 * Binds the connection to a local port number and IP address. If the
 * IP address is not given (i.e., ipaddr == NULL), the IP address of
 * the outgoing network interface is used instead.
 *
 * @param pcb the tcp_pcb to bind (no check is done whether this pcb is
 *        already bound!)
 * @param ipaddr the local ip address to bind to (use IP4_ADDR_ANY to bind
 *        to any local address
 * @param port the local port to bind to
 * @return ERR_USE if the port is already in use
 *         ERR_VAL if bind failed because the PCB is not in a valid state
 *         ERR_OK if bound
 */
err_t
tcp_bind(struct tcp_pcb *pcb, const ip_addr_t *ipaddr, u16_t port)
{
    return tcp_bind_adpt(pcb, ipaddr, port);
}

#if LWIP_CALLBACK_API
/**
 * Default accept callback if no accept callback is specified by the user.
 */
err_t
tcp_accept_null(void *arg, struct tcp_pcb *pcb, err_t err)
{
    return tcp_accept_null_adpt(arg, pcb, err);
}
#endif /* LWIP_CALLBACK_API */

/**
 * @ingroup tcp_raw
 * Set the state of the connection to be LISTEN, which means that it
 * is able to accept incoming connections. The protocol control block
 * is reallocated in order to consume less memory. Setting the
 * connection to LISTEN is an irreversible process.
 *
 * @param pcb the original tcp_pcb
 * @param backlog the incoming connections queue limit
 * @return tcp_pcb used for listening, consumes less memory.
 *
 * @note The original tcp_pcb is freed. This function therefore has to be
 *       called like this:
 *             tpcb = tcp_listen_with_backlog(tpcb, backlog);
 */
struct tcp_pcb *
tcp_listen_with_backlog(struct tcp_pcb *pcb, u8_t backlog)
{
    return tcp_listen_with_backlog_adpt(pcb, backlog);
}

/**
 * @ingroup tcp_raw
 * Set the state of the connection to be LISTEN, which means that it
 * is able to accept incoming connections. The protocol control block
 * is reallocated in order to consume less memory. Setting the
 * connection to LISTEN is an irreversible process.
 *
 * @param pcb the original tcp_pcb
 * @param backlog the incoming connections queue limit
 * @param err when NULL is returned, this contains the error reason
 * @return tcp_pcb used for listening, consumes less memory.
 *
 * @note The original tcp_pcb is freed. This function therefore has to be
 *       called like this:
 *             tpcb = tcp_listen_with_backlog_and_err(tpcb, backlog, &err);
 */
struct tcp_pcb *
tcp_listen_with_backlog_and_err(struct tcp_pcb *pcb, u8_t backlog, err_t *err)
{
    return tcp_listen_with_backlog_and_err_adpt(pcb, backlog, err);
}

/**
 * Update the state that tracks the available window space to advertise.
 *
 * Returns how much extra window would be advertised if we sent an
 * update now.
 */
u32_t
tcp_update_rcv_ann_wnd(struct tcp_pcb *pcb)
{
    return tcp_update_rcv_ann_wnd_adpt(pcb);
}

/**
 * @ingroup tcp_raw
 * This function should be called by the application when it has
 * processed the data. The purpose is to advertise a larger window
 * when the data has been processed.
 *
 * @param pcb the tcp_pcb for which data is read
 * @param len the amount of bytes that have been read by the application
 */
void
tcp_recved(struct tcp_pcb *pcb, u16_t len)
{
    tcp_recved_adpt(pcb, len);
}

/**
 * Allocate a new local TCP port.
 *
 * @return a new (free) local TCP port number
 */
u16_t
tcp_new_port(void)
{
    return tcp_new_port_adpt();
}

/**
 * @ingroup tcp_raw
 * Connects to another host. The function given as the "connected"
 * argument will be called when the connection has been established.
 *
 * @param pcb the tcp_pcb used to establish the connection
 * @param ipaddr the remote ip address to connect to
 * @param port the remote tcp port to connect to
 * @param connected callback function to call when connected (on error,
                    the err calback will be called)
 * @return ERR_VAL if invalid arguments are given
 *         ERR_OK if connect request has been sent
 *         other err_t values if connect request couldn't be sent
 */
err_t
tcp_connect(struct tcp_pcb *pcb, const ip_addr_t *ipaddr, u16_t port,
      tcp_connected_fn connected)
{
    return tcp_connect_adpt(pcb, ipaddr, port, connected);
}

/**
 * Called every 500 ms and implements the retransmission timer and the timer that
 * removes PCBs that have been in TIME-WAIT for enough time. It also increments
 * various timers such as the inactivity timer in each PCB.
 *
 * Automatically called from tcp_tmr().
 */
void
tcp_slowtmr(void)
{
    tcp_slowtmr_adpt();
}

/**
 * Is called every TCP_FAST_INTERVAL (250 ms) and process data previously
 * "refused" by upper layer (application) and sends delayed ACKs.
 *
 * Automatically called from tcp_tmr().
 */
void
tcp_fasttmr(void)
{
    tcp_fasttmr_adpt();
}

/** Call tcp_output for all active pcbs that have TF_NAGLEMEMERR set */
void
tcp_txnow(void)
{
    tcp_txnow_adpt();
}

/** Pass pcb->refused_data to the recv callback */
err_t
tcp_process_refused_data(struct tcp_pcb *pcb)
{
    return tcp_process_refused_data_adpt(pcb);
}

/**
 * Deallocates a list of TCP segments (tcp_seg structures).
 *
 * @param seg tcp_seg list of TCP segments to free
 */
void
tcp_segs_free(struct tcp_seg *seg)
{
    tcp_segs_free_adpt(seg);
}

/**
 * Frees a TCP segment (tcp_seg structure).
 *
 * @param seg single tcp_seg to free
 */
void
tcp_seg_free(struct tcp_seg *seg)
{
    tcp_seg_free_adpt(seg);
}


/**
 * Sets the priority of a connection.
 *
 * @param pcb the tcp_pcb to manipulate
 * @param prio new priority
 */
void
tcp_setprio(struct tcp_pcb *pcb, u8_t prio)
{
    tcp_setprio_adpt(pcb, prio);
}

#if TCP_QUEUE_OOSEQ
/**
 * Returns a copy of the given TCP segment.
 * The pbuf and data are not copied, only the pointers
 *
 * @param seg the old tcp_seg
 * @return a copy of seg
 */
struct tcp_seg *
tcp_seg_copy(struct tcp_seg *seg)
{
    return tcp_seg_copy_adpt(seg);
}
#endif /* TCP_QUEUE_OOSEQ */

#if LWIP_CALLBACK_API
/**
 * Default receive callback that is called if the user didn't register
 * a recv callback for the pcb.
 */
err_t
tcp_recv_null(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
    return tcp_recv_null_adpt(arg, pcb, p, err);
}
#endif /* LWIP_CALLBACK_API */

/**
 * Kills the oldest active connection that has the same or lower priority than
 * 'prio'.
 *
 * @param prio minimum priority
 */
void
tcp_kill_prio(u8_t prio)
{
    tcp_kill_prio_adpt(prio);
}

/**
 * Kills the oldest connection that is in specific state.
 * Called from tcp_alloc() for LAST_ACK and CLOSING if no more connections are available.
 */
void
tcp_kill_state(enum tcp_state state)
{
    tcp_kill_state_adpt(state);
}

/**
 * Kills the oldest connection that is in TIME_WAIT state.
 * Called from tcp_alloc() if no more connections are available.
 */
void
tcp_kill_timewait(void)
{
    tcp_kill_timewait_adpt();
}


/**
 * Allocate a new tcp_pcb structure.
 *
 * @param prio priority for the new pcb
 * @return a new tcp_pcb that initially is in state CLOSED
 */
struct tcp_pcb *
tcp_alloc(u8_t prio)
{
    return tcp_alloc_adpt(prio);
}

/**
 * @ingroup tcp_raw
 * Creates a new TCP protocol control block but doesn't place it on
 * any of the TCP PCB lists.
 * The pcb is not put on any list until binding using tcp_bind().
 *
 * @internal: Maybe there should be a idle TCP PCB list where these
 * PCBs are put on. Port reservation using tcp_bind() is implemented but
 * allocated pcbs that are not bound can't be killed automatically if wanting
 * to allocate a pcb with higher prio (@see tcp_kill_prio())
 *
 * @return a new tcp_pcb that initially is in state CLOSED
 */
struct tcp_pcb *
tcp_new(void)
{
  return tcp_new_adpt();
}

/**
 * @ingroup tcp_raw
 * Creates a new TCP protocol control block but doesn't
 * place it on any of the TCP PCB lists.
 * The pcb is not put on any list until binding using tcp_bind().
 *
 * @param type IP address type, see @ref lwip_ip_addr_type definitions.
 * If you want to listen to IPv4 and IPv6 (dual-stack) connections,
 * supply @ref IPADDR_TYPE_ANY as argument and bind to @ref IP_ANY_TYPE.
 * @return a new tcp_pcb that initially is in state CLOSED
 */
struct tcp_pcb *
tcp_new_ip_type(u8_t type)
{
    return tcp_new_ip_type_adpt(type);
}

/**
 * @ingroup tcp_raw
 * Used to specify the argument that should be passed callback
 * functions.
 *
 * @param pcb tcp_pcb to set the callback argument
 * @param arg void pointer argument to pass to callback functions
 */
void
tcp_arg(struct tcp_pcb *pcb, void *arg)
{
    tcp_arg_adpt(pcb, arg);
}
#if LWIP_CALLBACK_API

/**
 * @ingroup tcp_raw
 * Used to specify the function that should be called when a TCP
 * connection receives data.
 *
 * @param pcb tcp_pcb to set the recv callback
 * @param recv callback function to call for this pcb when data is received
 */
void
tcp_recv(struct tcp_pcb *pcb, tcp_recv_fn recv)
{
    tcp_recv_adpt(pcb, recv);
}

/**
 * @ingroup tcp_raw
 * Used to specify the function that should be called when TCP data
 * has been successfully delivered to the remote host.
 *
 * @param pcb tcp_pcb to set the sent callback
 * @param sent callback function to call for this pcb when data is successfully sent
 */
void
tcp_sent(struct tcp_pcb *pcb, tcp_sent_fn sent)
{
    tcp_sent_adpt(pcb, sent);
}

/**
 * @ingroup tcp_raw
 * Used to specify the function that should be called when a fatal error
 * has occurred on the connection.
 *
 * @note The corresponding pcb is already freed when this callback is called!
 *
 * @param pcb tcp_pcb to set the err callback
 * @param err callback function to call for this pcb when a fatal error
 *        has occurred on the connection
 */
void
tcp_err(struct tcp_pcb *pcb, tcp_err_fn err)
{
    tcp_err_adpt(pcb, err);
}

/**
 * @ingroup tcp_raw
 * Used for specifying the function that should be called when a
 * LISTENing connection has been connected to another host.
 *
 * @param pcb tcp_pcb to set the accept callback
 * @param accept callback function to call for this pcb when LISTENing
 *        connection has been connected to another host
 */
void
tcp_accept(struct tcp_pcb *pcb, tcp_accept_fn accept)
{
    tcp_accept_adpt(pcb, accept);
}
#endif /* LWIP_CALLBACK_API */


/**
 * @ingroup tcp_raw
 * Used to specify the function that should be called periodically
 * from TCP. The interval is specified in terms of the TCP coarse
 * timer interval, which is called twice a second.
 *
 */
void
tcp_poll(struct tcp_pcb *pcb, tcp_poll_fn poll, u8_t interval)
{
    tcp_poll_adpt(pcb, poll, interval);
}

/**
 * Purges a TCP PCB. Removes any buffered data and frees the buffer memory
 * (pcb->ooseq, pcb->unsent and pcb->unacked are freed).
 *
 * @param pcb tcp_pcb to purge. The pcb itself is not deallocated!
 */
void
tcp_pcb_purge(struct tcp_pcb *pcb)
{
    tcp_pcb_purge_adpt(pcb);
}

/**
 * Purges the PCB and removes it from a PCB list. Any delayed ACKs are sent first.
 *
 * @param pcblist PCB list to purge.
 * @param pcb tcp_pcb to purge. The pcb itself is NOT deallocated!
 */
void
tcp_pcb_remove(struct tcp_pcb **pcblist, struct tcp_pcb *pcb)
{
    tcp_pcb_remove_adpt(pcblist, pcb);
}

/**
 * Calculates a new initial sequence number for new connections.
 *
 * @return u32_t pseudo random sequence number
 */
u32_t
tcp_next_iss(struct tcp_pcb *pcb)
{
    return tcp_next_iss_adpt(pcb);
}

#if TCP_CALCULATE_EFF_SEND_MSS
/**
 * Calculates the effective send mss that can be used for a specific IP address
 * by using ip_route to determine the netif used to send to the address and
 * calculating the minimum of TCP_MSS and that netif's mtu (if set).
 */
u16_t
tcp_eff_send_mss_impl(u16_t sendmss, const ip_addr_t *dest
#if LWIP_IPV6 || LWIP_IPV4_SRC_ROUTING
                     , const ip_addr_t *src
#endif /* LWIP_IPV6 || LWIP_IPV4_SRC_ROUTING */
                     )
{
    return tcp_eff_send_mss_impl_adpt(sendmss, dest
#if LWIP_IPV6 || LWIP_IPV4_SRC_ROUTING
                     , src
#endif /* LWIP_IPV6 || LWIP_IPV4_SRC_ROUTING */
                    );
}
#endif /* TCP_CALCULATE_EFF_SEND_MSS */


/** Helper function for tcp_netif_ip_addr_changed() that iterates a pcb list */
void
tcp_netif_ip_addr_changed_pcblist(const ip_addr_t* old_addr, struct tcp_pcb* pcb_list)
{
    tcp_netif_ip_addr_changed_pcblist_adpt(old_addr, pcb_list);
}

/** This function is called from netif.c when address is changed or netif is removed
 *
 * @param old_addr IP address of the netif before change
 * @param new_addr IP address of the netif after change or NULL if netif has been removed
 */
void
tcp_netif_ip_addr_changed(const ip_addr_t* old_addr, const ip_addr_t* new_addr)
{
    tcp_netif_ip_addr_changed_adpt(old_addr, new_addr);
}

#if 0
const char*
tcp_debug_state_str(enum tcp_state s)
{
  return tcp_state_str[s];
}
#endif

#if TCP_DEBUG || TCP_INPUT_DEBUG || TCP_OUTPUT_DEBUG
/**
 * Print a tcp header for debugging purposes.
 *
 * @param tcphdr pointer to a struct tcp_hdr
 */
#if 0
void
tcp_debug_print(struct tcp_hdr *tcphdr)
{
  LWIP_DEBUGF(TCP_DEBUG, ("TCP header:\n"));
  LWIP_DEBUGF(TCP_DEBUG, ("+-------------------------------+\n"));
  LWIP_DEBUGF(TCP_DEBUG, ("|    %5"U16_F"      |    %5"U16_F"      | (src port, dest port)\n",
         lwip_ntohs(tcphdr->src), lwip_ntohs(tcphdr->dest)));
  LWIP_DEBUGF(TCP_DEBUG, ("+-------------------------------+\n"));
  LWIP_DEBUGF(TCP_DEBUG, ("|           %010"U32_F"          | (seq no)\n",
          lwip_ntohl(tcphdr->seqno)));
  LWIP_DEBUGF(TCP_DEBUG, ("+-------------------------------+\n"));
  LWIP_DEBUGF(TCP_DEBUG, ("|           %010"U32_F"          | (ack no)\n",
         lwip_ntohl(tcphdr->ackno)));
  LWIP_DEBUGF(TCP_DEBUG, ("+-------------------------------+\n"));
  LWIP_DEBUGF(TCP_DEBUG, ("| %2"U16_F" |   |%"U16_F"%"U16_F"%"U16_F"%"U16_F"%"U16_F"%"U16_F"|     %5"U16_F"     | (hdrlen, flags (",
       TCPH_HDRLEN(tcphdr),
         (u16_t)(TCPH_FLAGS(tcphdr) >> 5 & 1),
         (u16_t)(TCPH_FLAGS(tcphdr) >> 4 & 1),
         (u16_t)(TCPH_FLAGS(tcphdr) >> 3 & 1),
         (u16_t)(TCPH_FLAGS(tcphdr) >> 2 & 1),
         (u16_t)(TCPH_FLAGS(tcphdr) >> 1 & 1),
         (u16_t)(TCPH_FLAGS(tcphdr)      & 1),
         lwip_ntohs(tcphdr->wnd)));
  tcp_debug_print_flags(TCPH_FLAGS(tcphdr));
  LWIP_DEBUGF(TCP_DEBUG, ("), win)\n"));
  LWIP_DEBUGF(TCP_DEBUG, ("+-------------------------------+\n"));
  LWIP_DEBUGF(TCP_DEBUG, ("|    0x%04"X16_F"     |     %5"U16_F"     | (chksum, urgp)\n",
         lwip_ntohs(tcphdr->chksum), lwip_ntohs(tcphdr->urgp)));
  LWIP_DEBUGF(TCP_DEBUG, ("+-------------------------------+\n"));
}
#endif

/**
 * Print a tcp state for debugging purposes.
 *
 * @param s enum tcp_state to print
 */
#if 0
void
tcp_debug_print_state(enum tcp_state s)
{
  LWIP_DEBUGF(TCP_DEBUG, ("State: %s\n", tcp_state_str[s]));
}
#endif

/**
 * Print tcp flags for debugging purposes.
 *
 * @param flags tcp flags, all active flags are printed
 */
#if 0
void
tcp_debug_print_flags(u8_t flags)
{
  if (flags & TCP_FIN) {
    LWIP_DEBUGF(TCP_DEBUG, ("FIN "));
  }
  if (flags & TCP_SYN) {
    LWIP_DEBUGF(TCP_DEBUG, ("SYN "));
  }
  if (flags & TCP_RST) {
    LWIP_DEBUGF(TCP_DEBUG, ("RST "));
  }
  if (flags & TCP_PSH) {
    LWIP_DEBUGF(TCP_DEBUG, ("PSH "));
  }
  if (flags & TCP_ACK) {
    LWIP_DEBUGF(TCP_DEBUG, ("ACK "));
  }
  if (flags & TCP_URG) {
    LWIP_DEBUGF(TCP_DEBUG, ("URG "));
  }
  if (flags & TCP_ECE) {
    LWIP_DEBUGF(TCP_DEBUG, ("ECE "));
  }
  if (flags & TCP_CWR) {
    LWIP_DEBUGF(TCP_DEBUG, ("CWR "));
  }
  LWIP_DEBUGF(TCP_DEBUG, ("\n"));
}
#endif

/**
 * Print all tcp_pcbs in every list for debugging purposes.
 */
#if 0
void
tcp_debug_print_pcbs(void)
{
  struct tcp_pcb *pcb;
  struct tcp_pcb_listen *pcbl;

  LWIP_DEBUGF(TCP_DEBUG, ("Active PCB states:\n"));
  for (pcb = tcp_active_pcbs; pcb != NULL; pcb = pcb->next) {
    LWIP_DEBUGF(TCP_DEBUG, ("Local port %"U16_F", foreign port %"U16_F" snd_nxt %"U32_F" rcv_nxt %"U32_F" ",
                       pcb->local_port, pcb->remote_port,
                       pcb->snd_nxt, pcb->rcv_nxt));
    tcp_debug_print_state(pcb->state);
  }

  LWIP_DEBUGF(TCP_DEBUG, ("Listen PCB states:\n"));
  for (pcbl = tcp_listen_pcbs.listen_pcbs; pcbl != NULL; pcbl = pcbl->next) {
    LWIP_DEBUGF(TCP_DEBUG, ("Local port %"U16_F" ", pcbl->local_port));
    tcp_debug_print_state(pcbl->state);
  }

  LWIP_DEBUGF(TCP_DEBUG, ("TIME-WAIT PCB states:\n"));
  for (pcb = tcp_tw_pcbs; pcb != NULL; pcb = pcb->next) {
    LWIP_DEBUGF(TCP_DEBUG, ("Local port %"U16_F", foreign port %"U16_F" snd_nxt %"U32_F" rcv_nxt %"U32_F" ",
                       pcb->local_port, pcb->remote_port,
                       pcb->snd_nxt, pcb->rcv_nxt));
    tcp_debug_print_state(pcb->state);
  }
}

/**
 * Check state consistency of the tcp_pcb lists.
 */
s16_t
tcp_pcbs_sane(void)
{
  struct tcp_pcb *pcb;
  for (pcb = tcp_active_pcbs; pcb != NULL; pcb = pcb->next) {
    LWIP_ASSERT("tcp_pcbs_sane: active pcb->state != CLOSED", pcb->state != CLOSED);
    LWIP_ASSERT("tcp_pcbs_sane: active pcb->state != LISTEN", pcb->state != LISTEN);
    LWIP_ASSERT("tcp_pcbs_sane: active pcb->state != TIME-WAIT", pcb->state != TIME_WAIT);
  }
  for (pcb = tcp_tw_pcbs; pcb != NULL; pcb = pcb->next) {
    LWIP_ASSERT("tcp_pcbs_sane: tw pcb->state == TIME-WAIT", pcb->state == TIME_WAIT);
  }
  return 1;
}
#endif
#endif /* TCP_DEBUG */

#endif /* LWIP_TCP */

#endif //#if defined(LWIP_ROMBUILD)

