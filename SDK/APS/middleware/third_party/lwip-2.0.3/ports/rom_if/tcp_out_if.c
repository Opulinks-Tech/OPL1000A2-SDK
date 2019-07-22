/**
 * @file
 * Transmission Control Protocol, outgoing traffic
 *
 * The output functions of TCP.
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

#if defined(LWIP_ROMBUILD)

#if LWIP_TCP /* don't build if not configured for use in lwipopts.h */

#include "lwip/priv/tcp_priv.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/ip_addr.h"
#include "lwip/netif.h"
#include "lwip/inet_chksum.h"
#include "lwip/stats.h"
#include "lwip/ip6.h"
#include "lwip/ip6_addr.h"
#if LWIP_TCP_TIMESTAMPS
#include "lwip/sys.h"
#endif

#include "tcp_if.h"

#include <string.h>

void lwip_load_interface_tcp_out(void)
{
    tcp_output_alloc_header_adpt            = LWIP_ROMFN(tcp_output_alloc_header);
    tcp_create_segment_adpt                 = LWIP_ROMFN(tcp_create_segment);
    tcp_pbuf_prealloc_adpt                  = LWIP_ROMFN(tcp_pbuf_prealloc);
#if TCP_CHECKSUM_ON_COPY
    tcp_seg_add_chksum_adpt                 = LWIP_ROMFN(tcp_seg_add_chksum);
#endif
    tcp_write_checks_adpt                   = LWIP_ROMFN(tcp_write_checks);
#if LWIP_TCP_TIMESTAMPS
    tcp_build_timestamp_option_adpt         = LWIP_ROMFN(tcp_build_timestamp_option);
#endif
#if LWIP_WND_SCALE
    tcp_build_wnd_scale_option_adpt         = LWIP_ROMFN(tcp_build_wnd_scale_option);
#endif
    tcp_output_segment_adpt                 = LWIP_ROMFN(tcp_output_segment);
}


/* Forward declarations.*/
err_t tcp_output_segment(struct tcp_seg *seg, struct tcp_pcb *pcb, struct netif *netif);

/** Allocate a pbuf and create a tcphdr at p->payload, used for output
 * functions other than the default tcp_output -> tcp_output_segment
 * (e.g. tcp_send_empty_ack, etc.)
 *
 * @param pcb tcp pcb for which to send a packet (used to initialize tcp_hdr)
 * @param optlen length of header-options
 * @param datalen length of tcp data to reserve in pbuf
 * @param seqno_be seqno in network byte order (big-endian)
 * @return pbuf with p->payload being the tcp_hdr
 */
struct pbuf *
tcp_output_alloc_header(struct tcp_pcb *pcb, u16_t optlen, u16_t datalen,
                      u32_t seqno_be /* already in network byte order */)
{
    return tcp_output_alloc_header_adpt(pcb, optlen, datalen, seqno_be);
}

/**
 * Called by tcp_close() to send a segment including FIN flag but not data.
 *
 * @param pcb the tcp_pcb over which to send a segment
 * @return ERR_OK if sent, another err_t otherwise
 */
err_t
tcp_send_fin(struct tcp_pcb *pcb)
{
    return tcp_send_fin_adpt(pcb);
}

/**
 * Create a TCP segment with prefilled header.
 *
 * Called by tcp_write and tcp_enqueue_flags.
 *
 * @param pcb Protocol control block for the TCP connection.
 * @param p pbuf that is used to hold the TCP header.
 * @param flags TCP flags for header.
 * @param seqno TCP sequence number of this packet
 * @param optflags options to include in TCP header
 * @return a new tcp_seg pointing to p, or NULL.
 * The TCP header is filled in except ackno and wnd.
 * p is freed on failure.
 */
struct tcp_seg *
tcp_create_segment(struct tcp_pcb *pcb, struct pbuf *p, u8_t flags, u32_t seqno, u8_t optflags)
{
    return tcp_create_segment_adpt(pcb, p, flags, seqno, optflags);
}

/**
 * Allocate a PBUF_RAM pbuf, perhaps with extra space at the end.
 *
 * This function is like pbuf_alloc(layer, length, PBUF_RAM) except
 * there may be extra bytes available at the end.
 *
 * @param layer flag to define header size.
 * @param length size of the pbuf's payload.
 * @param max_length maximum usable size of payload+oversize.
 * @param oversize pointer to a u16_t that will receive the number of usable tail bytes.
 * @param pcb The TCP connection that will enqueue the pbuf.
 * @param apiflags API flags given to tcp_write.
 * @param first_seg true when this pbuf will be used in the first enqueued segment.
 */
#if TCP_OVERSIZE
struct pbuf *
tcp_pbuf_prealloc(pbuf_layer layer, u16_t length, u16_t max_length,
                  u16_t *oversize, struct tcp_pcb *pcb, u8_t apiflags,
                  u8_t first_seg)
{
    return tcp_pbuf_prealloc_adpt(layer, length, max_length, oversize, pcb, apiflags, first_seg);
}
#else /* TCP_OVERSIZE */
#define tcp_pbuf_prealloc(layer, length, mx, os, pcb, api, fst) pbuf_alloc((layer), (length), PBUF_RAM)
#endif /* TCP_OVERSIZE */

#if TCP_CHECKSUM_ON_COPY
/** Add a checksum of newly added data to the segment */
void
tcp_seg_add_chksum(u16_t chksum, u16_t len, u16_t *seg_chksum,
                   u8_t *seg_chksum_swapped)
{
    tcp_seg_add_chksum_adpt(chksum, len, seg_chksum, seg_chksum_swapped);
}
#endif /* TCP_CHECKSUM_ON_COPY */

/** Checks if tcp_write is allowed or not (checks state, snd_buf and snd_queuelen).
 *
 * @param pcb the tcp pcb to check for
 * @param len length of data to send (checked agains snd_buf)
 * @return ERR_OK if tcp_write is allowed to proceed, another err_t otherwise
 */
err_t
tcp_write_checks(struct tcp_pcb *pcb, u16_t len)
{
    return tcp_write_checks_adpt(pcb, len);
}

/**
 * @ingroup tcp_raw
 * Write data for sending (but does not send it immediately).
 *
 * It waits in the expectation of more data being sent soon (as
 * it can send them more efficiently by combining them together).
 * To prompt the system to send data now, call tcp_output() after
 * calling tcp_write().
 *
 * @param pcb Protocol control block for the TCP connection to enqueue data for.
 * @param arg Pointer to the data to be enqueued for sending.
 * @param len Data length in bytes
 * @param apiflags combination of following flags :
 * - TCP_WRITE_FLAG_COPY (0x01) data will be copied into memory belonging to the stack
 * - TCP_WRITE_FLAG_MORE (0x02) for TCP connection, PSH flag will not be set on last segment sent,
 * @return ERR_OK if enqueued, another err_t on error
 */

err_t
tcp_write(struct tcp_pcb *pcb, const void *arg, u16_t len, u8_t apiflags)
{
    return tcp_write_adpt(pcb, arg, len, apiflags);
}

/**
 * Enqueue TCP options for transmission.
 *
 * Called by tcp_connect(), tcp_listen_input(), and tcp_send_ctrl().
 *
 * @param pcb Protocol control block for the TCP connection.
 * @param flags TCP header flags to set in the outgoing segment.
 */
err_t
tcp_enqueue_flags(struct tcp_pcb *pcb, u8_t flags)
{
    return tcp_enqueue_flags_adpt(pcb, flags);
}


#if LWIP_TCP_TIMESTAMPS
/* Build a timestamp option (12 bytes long) at the specified options pointer)
 *
 * @param pcb tcp_pcb
 * @param opts option pointer where to store the timestamp option
 */
void
tcp_build_timestamp_option(struct tcp_pcb *pcb, u32_t *opts)
{
    tcp_build_timestamp_option_adpt(pcb, opts);
}
#endif

#if LWIP_WND_SCALE
/** Build a window scale option (3 bytes long) at the specified options pointer)
 *
 * @param opts option pointer where to store the window scale option
 */
void
tcp_build_wnd_scale_option(u32_t *opts)
{
    tcp_build_wnd_scale_option_adpt(opts);
}
#endif
/**
 * Send an ACK without data.
 *
 * @param pcb Protocol control block for the TCP connection to send the ACK
 */
err_t
tcp_send_empty_ack(struct tcp_pcb *pcb)
{
    return tcp_send_empty_ack_adpt(pcb);
}

/**
 * @ingroup tcp_raw
 * Find out what we can send and send it
 *
 * @param pcb Protocol control block for the TCP connection to send data
 * @return ERR_OK if data has been sent or nothing to send
 *         another err_t on error
 */
err_t
tcp_output(struct tcp_pcb *pcb)
{
    return tcp_output_adpt(pcb);
}

/**
 * Called by tcp_output() to actually send a TCP segment over IP.
 *
 * @param seg the tcp_seg to send
 * @param pcb the tcp_pcb for the TCP connection used to send the segment
 * @param netif the netif used to send the segment
 */
err_t
tcp_output_segment(struct tcp_seg *seg, struct tcp_pcb *pcb, struct netif *netif)
{
    return tcp_output_segment_adpt(seg, pcb, netif);
}

/**
 * Send a TCP RESET packet (empty segment with RST flag set) either to
 * abort a connection or to show that there is no matching local connection
 * for a received segment.
 *
 * Called by tcp_abort() (to abort a local connection), tcp_input() (if no
 * matching local pcb was found), tcp_listen_input() (if incoming segment
 * has ACK flag set) and tcp_process() (received segment in the wrong state)
 *
 * Since a RST segment is in most cases not sent for an active connection,
 * tcp_rst() has a number of arguments that are taken from a tcp_pcb for
 * most other segment output functions.
 *
 * @param seqno the sequence number to use for the outgoing segment
 * @param ackno the acknowledge number to use for the outgoing segment
 * @param local_ip the local IP address to send the segment from
 * @param remote_ip the remote IP address to send the segment to
 * @param local_port the local TCP port to send the segment from
 * @param remote_port the remote TCP port to send the segment to
 */
void
tcp_rst(u32_t seqno, u32_t ackno,
  const ip_addr_t *local_ip, const ip_addr_t *remote_ip,
  u16_t local_port, u16_t remote_port)
{
    tcp_rst_adpt(seqno, ackno, local_ip, remote_ip, local_port, remote_port);
}

/**
 * Requeue all unacked segments for retransmission
 *
 * Called by tcp_slowtmr() for slow retransmission.
 *
 * @param pcb the tcp_pcb for which to re-enqueue all unacked segments
 */
void
tcp_rexmit_rto(struct tcp_pcb *pcb)
{
    tcp_rexmit_rto_adpt(pcb);
}

/**
 * Requeue the first unacked segment for retransmission
 *
 * Called by tcp_receive() for fast retransmit.
 *
 * @param pcb the tcp_pcb for which to retransmit the first unacked segment
 */
void
tcp_rexmit(struct tcp_pcb *pcb)
{
    tcp_rexmit_adpt(pcb);
}


/**
 * Handle retransmission after three dupacks received
 *
 * @param pcb the tcp_pcb for which to retransmit the first unacked segment
 */
void
tcp_rexmit_fast(struct tcp_pcb *pcb)
{
    tcp_rexmit_fast_adpt(pcb);
}

/**
 * Send keepalive packets to keep a connection active although
 * no data is sent over it.
 *
 * Called by tcp_slowtmr()
 *
 * @param pcb the tcp_pcb for which to send a keepalive packet
 */
err_t
tcp_keepalive(struct tcp_pcb *pcb)
{
    return tcp_keepalive_adpt(pcb);
}

/**
 * Send persist timer zero-window probes to keep a connection active
 * when a window update is lost.
 *
 * Called by tcp_slowtmr()
 *
 * @param pcb the tcp_pcb for which to send a zero-window probe packet
 */
err_t
tcp_zero_window_probe(struct tcp_pcb *pcb)
{
    return tcp_zero_window_probe_adpt(pcb);
}

#endif /* LWIP_TCP */

#endif //#if defined(LWIP_ROMBUILD)

