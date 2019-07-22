/**
 * @file
 * TCP API (to be used from TCPIP thread)\n
 * See also @ref tcp_raw
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
#ifndef LWIP_HDR_TCP_IF_H
#define LWIP_HDR_TCP_IF_H

#include "lwip/opt.h"

#if defined(LWIP_ROMBUILD)


#if LWIP_TCP /* don't build if not configured for use in lwipopts.h */

#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/ip.h"
#include "lwip/icmp.h"
#include "lwip/err.h"
#include "lwip/ip6.h"
#include "lwip/ip6_addr.h"
#include "lwip/err.h"

#ifdef __cplusplus
extern "C" {
#endif

struct tcp_pcb;
struct tcp_seg;
struct tcp_pcb_listen;

/* Private static API fucntion used in tcp.c*/
void  tcp_remove_listener(struct tcp_pcb *list, struct tcp_pcb_listen *lpcb);
void  tcp_listen_closed(struct tcp_pcb *pcb);
err_t tcp_close_shutdown(struct tcp_pcb *pcb, u8_t rst_on_unacked_data);
err_t tcp_close_shutdown_fin(struct tcp_pcb *pcb);
u16_t tcp_new_port(void);
void  tcp_kill_prio(u8_t prio);
void  tcp_kill_state(enum tcp_state state);
void  tcp_kill_timewait(void);
err_t tcp_accept_null(void *arg, struct tcp_pcb *pcb, err_t err);
void  tcp_netif_ip_addr_changed_pcblist(const ip_addr_t* old_addr, struct tcp_pcb* pcb_list);


#if LWIP_CALLBACK_API || TCP_LISTEN_BACKLOG
typedef void  (*tcp_remove_listener_fp_t)(struct tcp_pcb *list, struct tcp_pcb_listen *lpcb);
#endif
typedef void  (*tcp_listen_closed_fp_t)(struct tcp_pcb *pcb);
typedef err_t (*tcp_close_shutdown_fp_t)(struct tcp_pcb *pcb, u8_t rst_on_unacked_data);
typedef err_t (*tcp_close_shutdown_fin_fp_t)(struct tcp_pcb *pcb);
typedef u16_t (*tcp_new_port_fp_t)(void);
typedef void  (*tcp_kill_prio_fp_t)(u8_t prio);
typedef void  (*tcp_kill_state_fp_t)(enum tcp_state state);
typedef void  (*tcp_kill_timewait_fp_t)(void);
#if LWIP_CALLBACK_API
typedef err_t (*tcp_accept_null_fp_t)(void *arg, struct tcp_pcb *pcb, err_t err);
typedef err_t (*tcp_recv_null_fp_t)(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err);
#endif
typedef void  (*tcp_netif_ip_addr_changed_pcblist_fp_t)(const ip_addr_t* old_addr, struct tcp_pcb* pcb_list);

#if LWIP_CALLBACK_API || TCP_LISTEN_BACKLOG
extern tcp_remove_listener_fp_t         tcp_remove_listener_adpt;
#endif
extern tcp_listen_closed_fp_t           tcp_listen_closed_adpt;
extern tcp_close_shutdown_fp_t          tcp_close_shutdown_adpt;
extern tcp_close_shutdown_fin_fp_t      tcp_close_shutdown_fin_adpt;;
extern tcp_new_port_fp_t                tcp_new_port_adpt;
extern tcp_kill_prio_fp_t               tcp_kill_prio_adpt;
extern tcp_kill_state_fp_t              tcp_kill_state_adpt;
extern tcp_kill_timewait_fp_t           tcp_kill_timewait_adpt;
#if LWIP_CALLBACK_API
extern tcp_accept_null_fp_t             tcp_accept_null_adpt;
extern tcp_recv_null_fp_t               tcp_recv_null_adpt;
#endif
extern tcp_netif_ip_addr_changed_pcblist_fp_t tcp_netif_ip_addr_changed_pcblist_adpt;

/* Private static API fucntion used in tcp_in.c*/
err_t tcp_process(struct tcp_pcb *pcb);
void  tcp_receive(struct tcp_pcb *pcb);
void  tcp_parseopt(struct tcp_pcb *pcb);
void  tcp_listen_input(struct tcp_pcb_listen *pcb);
void  tcp_timewait_input(struct tcp_pcb *pcb);
int   tcp_input_delayed_close(struct tcp_pcb *pcb);
void  tcp_oos_insert_segment(struct tcp_seg *cseg, struct tcp_seg *next);
u8_t  tcp_getoptbyte(void);

typedef err_t (*tcp_process_fp_t)(struct tcp_pcb *pcb);
typedef void  (*tcp_receive_fp_t)(struct tcp_pcb *pcb);
typedef void  (*tcp_parseopt_fp_t)(struct tcp_pcb *pcb);
typedef void  (*tcp_listen_input_fp_t)(struct tcp_pcb_listen *pcb);
typedef void  (*tcp_timewait_input_fp_t)(struct tcp_pcb *pcb);
typedef int   (*tcp_input_delayed_close_fp_t)(struct tcp_pcb *pcb);
typedef void  (*tcp_oos_insert_segment_fp_t)(struct tcp_seg *cseg, struct tcp_seg *next);
typedef u8_t  (*tcp_getoptbyte_fp_t)(void);

extern tcp_process_fp_t                 tcp_process_adpt;
extern tcp_receive_fp_t                 tcp_receive_adpt;
extern tcp_parseopt_fp_t                tcp_parseopt_adpt;
extern tcp_listen_input_fp_t            tcp_listen_input_adpt;
extern tcp_timewait_input_fp_t          tcp_timewait_input_adpt;
extern tcp_input_delayed_close_fp_t     tcp_input_delayed_close_adpt;
extern tcp_oos_insert_segment_fp_t      tcp_oos_insert_segment_adpt;
extern tcp_getoptbyte_fp_t              tcp_getoptbyte_adpt;


/* Private static API fucntion used in tcp_out.c*/
struct pbuf * tcp_output_alloc_header(struct tcp_pcb *pcb, u16_t optlen, u16_t datalen, u32_t seqno_be);
struct tcp_seg * tcp_create_segment(struct tcp_pcb *pcb, struct pbuf *p, u8_t flags, u32_t seqno, u8_t optflags);
struct pbuf * tcp_pbuf_prealloc(pbuf_layer layer, u16_t length, u16_t max_length, u16_t *oversize,
                                 struct tcp_pcb *pcb, u8_t apiflags, u8_t first_seg);
void  tcp_seg_add_chksum(u16_t chksum, u16_t len, u16_t *seg_chksum, u8_t *seg_chksum_swapped);
err_t tcp_write_checks(struct tcp_pcb *pcb, u16_t len);
void  tcp_build_timestamp_option(struct tcp_pcb *pcb, u32_t *opts);
void  tcp_build_wnd_scale_option(u32_t *opts);
err_t tcp_output_segment(struct tcp_seg *seg, struct tcp_pcb *pcb, struct netif *netif);

typedef struct pbuf * (*tcp_output_alloc_header_fp_t)(struct tcp_pcb *pcb, u16_t optlen, u16_t datalen, u32_t seqno_be);
typedef struct tcp_seg * (*tcp_create_segment_fp_t)(struct tcp_pcb *pcb, struct pbuf *p, u8_t flags, u32_t seqno, u8_t optflags);
typedef struct pbuf * (*tcp_pbuf_prealloc_fp_t)(pbuf_layer layer, u16_t length, u16_t max_length, u16_t *oversize,
                                   struct tcp_pcb *pcb, u8_t apiflags, u8_t first_seg);
typedef void  (*tcp_seg_add_chksum_fp_t)(u16_t chksum, u16_t len, u16_t *seg_chksum, u8_t *seg_chksum_swapped);
typedef err_t (*tcp_write_checks_fp_t)(struct tcp_pcb *pcb, u16_t len);
typedef void  (*tcp_build_timestamp_option_fp_t)(struct tcp_pcb *pcb, u32_t *opts);
typedef void  (*tcp_build_wnd_scale_option_fp_t)(u32_t *opts);
typedef err_t (*tcp_output_segment_fp_t)(struct tcp_seg *seg, struct tcp_pcb *pcb, struct netif *netif);

extern tcp_output_alloc_header_fp_t      tcp_output_alloc_header_adpt;
extern tcp_create_segment_fp_t           tcp_create_segment_adpt;
extern tcp_pbuf_prealloc_fp_t            tcp_pbuf_prealloc_adpt;
extern tcp_seg_add_chksum_fp_t           tcp_seg_add_chksum_adpt;
extern tcp_write_checks_fp_t             tcp_write_checks_adpt;
extern tcp_build_timestamp_option_fp_t   tcp_build_timestamp_option_adpt;
extern tcp_build_wnd_scale_option_fp_t   tcp_build_wnd_scale_option_adpt;
extern tcp_output_segment_fp_t           tcp_output_segment_adpt;



/* Functions for interfacing with TCP: */
/* Take form tcp_prv.h */

/* Lower layer interface to TCP: */
typedef void (*tcp_init_fp_t)(void); /* Initialize this module. */
typedef void (*tcp_tmr_fp_t)(void);  /* Must be called every
                                         TCP_TMR_INTERVAL
                                         ms. (Typically 250 ms). */
/* It is also possible to call these two functions at the right
   intervals (instead of calling tcp_tmr()). */
typedef void (*tcp_slowtmr_fp_t)(void);
typedef void (*tcp_fasttmr_fp_t)(void);

/* Call this from a netif driver (watch out for threading issues!) that has
   returned a memory error on transmit and now has free buffers to send more.
   This iterates all active pcbs that had an error and tries to call
   tcp_output, so use this with care as it might slow down the system. */
typedef void  (*tcp_txnow_fp_t)(void);

/* Only used by IP to pass a TCP segment to TCP: */
typedef void  (*tcp_input_fp_t)(struct pbuf *p, struct netif *inp);
/* Used within the TCP code only: */
typedef struct tcp_pcb * (*tcp_alloc_fp_t)(u8_t prio);
typedef void  (*tcp_abandon_fp_t)(struct tcp_pcb *pcb, int reset);
typedef err_t (*tcp_send_empty_ack_fp_t)(struct tcp_pcb *pcb);
typedef void  (*tcp_rexmit_fp_t)(struct tcp_pcb *pcb);
typedef void  (*tcp_rexmit_rto_fp_t)(struct tcp_pcb *pcb);
typedef void  (*tcp_rexmit_fast_fp_t)(struct tcp_pcb *pcb);
typedef u32_t (*tcp_update_rcv_ann_wnd_fp_t)(struct tcp_pcb *pcb);
typedef err_t (*tcp_process_refused_data_fp_t)(struct tcp_pcb *pcb);
typedef void  (*tcp_pcb_purge_fp_t)(struct tcp_pcb *pcb);
typedef void  (*tcp_pcb_remove_fp_t)(struct tcp_pcb **pcblist, struct tcp_pcb *pcb);
typedef void  (*tcp_segs_free_fp_t)(struct tcp_seg *seg);
typedef void  (*tcp_seg_free_fp_t)(struct tcp_seg *seg);
typedef struct tcp_seg *(*tcp_seg_copy_fp_t)(struct tcp_seg *seg);
typedef void  (*tcp_rst_fp_t)(u32_t seqno, u32_t ackno,const ip_addr_t *local_ip, const ip_addr_t *remote_ip, u16_t local_port, u16_t remote_port);
typedef err_t (*tcp_send_fin_fp_t)(struct tcp_pcb *pcb);
typedef err_t (*tcp_enqueue_flags_fp_t)(struct tcp_pcb *pcb, u8_t flags);
//typedef void  (*tcp_rexmit_seg_fp_t)(struct tcp_pcb *pcb, struct tcp_seg *seg);
typedef u32_t (*tcp_next_iss_fp_t)(struct tcp_pcb *pcb);
typedef err_t (*tcp_keepalive_fp_t)(struct tcp_pcb *pcb);
typedef err_t (*tcp_zero_window_probe_fp_t)(struct tcp_pcb *pcb);
typedef void  (*tcp_trigger_input_pcb_close_fp_t)(void);

#if TCP_CALCULATE_EFF_SEND_MSS
typedef u16_t (*tcp_eff_send_mss_impl_fp_t)(u16_t sendmss, const ip_addr_t *dest
#if LWIP_IPV6 || LWIP_IPV4_SRC_ROUTING
                           , const ip_addr_t *src
#endif /* LWIP_IPV6 || LWIP_IPV4_SRC_ROUTING */
                           );
#endif /* TCP_CALCULATE_EFF_SEND_MSS */


#if LWIP_CALLBACK_API
typedef err_t (*tcp_recv_null_fp_t)(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err);
#endif /* LWIP_CALLBACK_API */

typedef void (*tcp_netif_ip_addr_changed_fp_t)(const ip_addr_t* old_addr, const ip_addr_t* new_addr);

extern tcp_init_fp_t                       tcp_init_adpt;
extern tcp_tmr_fp_t                        tcp_tmr_adpt;
extern tcp_slowtmr_fp_t                    tcp_slowtmr_adpt;
extern tcp_fasttmr_fp_t                    tcp_fasttmr_adpt;
extern tcp_txnow_fp_t                      tcp_txnow_adpt;
extern tcp_input_fp_t                      tcp_input_adpt;
extern tcp_alloc_fp_t                      tcp_alloc_adpt;
extern tcp_abandon_fp_t                    tcp_abandon_adpt;
extern tcp_send_empty_ack_fp_t             tcp_send_empty_ack_adpt;
extern tcp_rexmit_fp_t                     tcp_rexmit_adpt;
extern tcp_rexmit_rto_fp_t                 tcp_rexmit_rto_adpt;
extern tcp_rexmit_fast_fp_t                tcp_rexmit_fast_adpt;
extern tcp_update_rcv_ann_wnd_fp_t         tcp_update_rcv_ann_wnd_adpt;
extern tcp_process_refused_data_fp_t       tcp_process_refused_data_adpt;
extern tcp_pcb_purge_fp_t                  tcp_pcb_purge_adpt;
extern tcp_pcb_remove_fp_t                 tcp_pcb_remove_adpt;
extern tcp_segs_free_fp_t                  tcp_segs_free_adpt;
extern tcp_seg_free_fp_t                   tcp_seg_free_adpt;
extern tcp_seg_copy_fp_t                   tcp_seg_copy_adpt;
extern tcp_rst_fp_t                        tcp_rst_adpt;
extern tcp_send_fin_fp_t                   tcp_send_fin_adpt;
extern tcp_enqueue_flags_fp_t              tcp_enqueue_flags_adpt;
//extern tcp_rexmit_seg_fp_t                 tcp_rexmit_seg_adpt;
extern tcp_next_iss_fp_t                   tcp_next_iss_adpt;
extern tcp_next_iss_fp_t                   tcp_next_iss_adpt;
extern tcp_keepalive_fp_t                  tcp_keepalive_adpt;
extern tcp_zero_window_probe_fp_t          tcp_zero_window_probe_adpt;
extern tcp_trigger_input_pcb_close_fp_t    tcp_trigger_input_pcb_close_adpt;

#if TCP_CALCULATE_EFF_SEND_MSS
extern tcp_eff_send_mss_impl_fp_t          tcp_eff_send_mss_impl_adpt;
#endif
#if LWIP_CALLBACK_API
extern tcp_recv_null_fp_t                  tcp_recv_null_adpt;
#endif

extern tcp_netif_ip_addr_changed_fp_t      tcp_netif_ip_addr_changed_adpt;



/** Function prototype for tcp accept callback functions. Called when a new
 * connection can be accepted on a listening pcb.
 *
 * @param arg Additional argument to pass to the callback function (@see tcp_arg())
 * @param newpcb The new connection pcb
 * @param err An error code if there has been an error accepting.
 *            Only return ERR_ABRT if you have called tcp_abort from within the
 *            callback function!
 */
typedef err_t (*tcp_accept_fn)(void *arg, struct tcp_pcb *newpcb, err_t err);

/** Function prototype for tcp receive callback functions. Called when data has
 * been received.
 *
 * @param arg Additional argument to pass to the callback function (@see tcp_arg())
 * @param tpcb The connection pcb which received data
 * @param p The received data (or NULL when the connection has been closed!)
 * @param err An error code if there has been an error receiving
 *            Only return ERR_ABRT if you have called tcp_abort from within the
 *            callback function!
 */
typedef err_t (*tcp_recv_fn)(void *arg, struct tcp_pcb *tpcb,
                             struct pbuf *p, err_t err);

/** Function prototype for tcp sent callback functions. Called when sent data has
 * been acknowledged by the remote side. Use it to free corresponding resources.
 * This also means that the pcb has now space available to send new data.
 *
 * @param arg Additional argument to pass to the callback function (@see tcp_arg())
 * @param tpcb The connection pcb for which data has been acknowledged
 * @param len The amount of bytes acknowledged
 * @return ERR_OK: try to send some data by calling tcp_output
 *            Only return ERR_ABRT if you have called tcp_abort from within the
 *            callback function!
 */
typedef err_t (*tcp_sent_fn)(void *arg, struct tcp_pcb *tpcb,
                              u16_t len);

/** Function prototype for tcp poll callback functions. Called periodically as
 * specified by @see tcp_poll.
 *
 * @param arg Additional argument to pass to the callback function (@see tcp_arg())
 * @param tpcb tcp pcb
 * @return ERR_OK: try to send some data by calling tcp_output
 *            Only return ERR_ABRT if you have called tcp_abort from within the
 *            callback function!
 */
typedef err_t (*tcp_poll_fn)(void *arg, struct tcp_pcb *tpcb);

/** Function prototype for tcp error callback functions. Called when the pcb
 * receives a RST or is unexpectedly closed for any other reason.
 *
 * @note The corresponding pcb is already freed when this callback is called!
 *
 * @param arg Additional argument to pass to the callback function (@see tcp_arg())
 * @param err Error code to indicate why the pcb has been closed
 *            ERR_ABRT: aborted through tcp_abort or by a TCP timer
 *            ERR_RST: the connection was reset by the remote host
 */
typedef void  (*tcp_err_fn)(void *arg, err_t err);

/** Function prototype for tcp connected callback functions. Called when a pcb
 * is connected to the remote side after initiating a connection attempt by
 * calling tcp_connect().
 *
 * @param arg Additional argument to pass to the callback function (@see tcp_arg())
 * @param tpcb The connection pcb which is connected
 * @param err An unused error code, always ERR_OK currently ;-) @todo!
 *            Only return ERR_ABRT if you have called tcp_abort from within the
 *            callback function!
 *
 * @note When a connection attempt fails, the error callback is currently called!
 */
typedef err_t (*tcp_connected_fn)(void *arg, struct tcp_pcb *tpcb, err_t err);





/** the TCP protocol control block for listening pcbs */
struct tcp_pcb_listen;
/** the TCP protocol control block */
struct tcp_pcb;

/* Application program's interface: */
typedef struct tcp_pcb * (*tcp_new_fp_t)(void);
typedef struct tcp_pcb * (*tcp_new_ip_type_fp_t)(u8_t type);

typedef void (*tcp_arg_fp_t)(struct tcp_pcb *pcb, void *arg);
#if LWIP_CALLBACK_API
typedef void (*tcp_recv_fp_t)(struct tcp_pcb *pcb, tcp_recv_fn recv);
typedef void (*tcp_sent_fp_t)(struct tcp_pcb *pcb, tcp_sent_fn sent);
typedef void (*tcp_err_fp_t)(struct tcp_pcb *pcb, tcp_err_fn err);
typedef void (*tcp_accept_fp_t)(struct tcp_pcb *pcb, tcp_accept_fn accept);
#endif /* LWIP_CALLBACK_API */
typedef void (*tcp_poll_fp_t)(struct tcp_pcb *pcb, tcp_poll_fn poll, u8_t interval);


extern tcp_new_fp_t         tcp_new_adpt;
extern tcp_new_ip_type_fp_t tcp_new_ip_type_adpt;
extern tcp_arg_fp_t         tcp_arg_adpt;
#if LWIP_CALLBACK_API
extern tcp_recv_fp_t        tcp_recv_adpt;
extern tcp_sent_fp_t        tcp_sent_adpt;
extern tcp_err_fp_t         tcp_err_adpt;
extern tcp_accept_fp_t      tcp_accept_adpt;
#endif
extern tcp_poll_fp_t        tcp_poll_adpt;



#if TCP_LISTEN_BACKLOG
typedef void (*tcp_backlog_delayed_fp_t)(struct tcp_pcb* pcb);
typedef void (*tcp_backlog_accepted_fp_t)(struct tcp_pcb* pcb);
extern tcp_backlog_delayed_fp_t tcp_backlog_delayed_adpt;
extern tcp_backlog_accepted_fp_t tcp_backlog_accepted_adpt;
#else  /* TCP_LISTEN_BACKLOG */
#define         tcp_backlog_set(pcb, new_backlog)
#define         tcp_backlog_delayed(pcb)
#define         tcp_backlog_accepted(pcb)
#endif /* TCP_LISTEN_BACKLOG */

typedef struct tcp_pcb * (*tcp_listen_with_backlog_and_err_fp_t)(struct tcp_pcb *pcb, u8_t backlog, err_t *err);
typedef struct tcp_pcb * (*tcp_listen_with_backlog_fp_t)(struct tcp_pcb *pcb, u8_t backlog);
typedef void  (*tcp_recved_fp_t)(struct tcp_pcb *pcb, u16_t len);
typedef err_t (*tcp_bind_fp_t)(struct tcp_pcb *pcb, const ip_addr_t *ipaddr, u16_t port);
typedef err_t (*tcp_connect_fp_t)(struct tcp_pcb *pcb, const ip_addr_t *ipaddr, u16_t port, tcp_connected_fn connected);
typedef void  (*tcp_abort_fp_t)(struct tcp_pcb *pcb);
typedef err_t (*tcp_close_fp_t)(struct tcp_pcb *pcb);
typedef err_t (*tcp_shutdown_fp_t)(struct tcp_pcb *pcb, int shut_rx, int shut_tx);
typedef err_t (*tcp_write_fp_t)(struct tcp_pcb *pcb, const void *dataptr, u16_t len, u8_t apiflags);
typedef void  (*tcp_setprio_fp_t)(struct tcp_pcb *pcb, u8_t prio);
typedef err_t (*tcp_output_fp_t)(struct tcp_pcb *pcb);

typedef const char* (*tcp_debug_state_str_fp_t)(enum tcp_state s);

extern tcp_listen_with_backlog_and_err_fp_t    tcp_listen_with_backlog_and_err_adpt;
extern tcp_listen_with_backlog_fp_t            tcp_listen_with_backlog_adpt;
extern tcp_recved_fp_t                         tcp_recved_adpt;
extern tcp_bind_fp_t                           tcp_bind_adpt;
extern tcp_connect_fp_t                        tcp_connect_adpt;
extern tcp_abort_fp_t                          tcp_abort_adpt;
extern tcp_close_fp_t                          tcp_close_adpt;
extern tcp_shutdown_fp_t                       tcp_shutdown_adpt;
extern tcp_write_fp_t                          tcp_write_adpt;
extern tcp_setprio_fp_t                        tcp_setprio_adpt;
extern tcp_output_fp_t                         tcp_output_adpt;


#ifdef __cplusplus
}
#endif

#endif /* LWIP_TCP */

#endif /* defined(LWIP_ROMBUILD) */


#endif /* LWIP_HDR_TCP_IF_H */
