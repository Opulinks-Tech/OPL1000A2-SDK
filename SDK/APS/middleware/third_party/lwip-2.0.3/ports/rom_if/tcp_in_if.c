/**
 * @file
 * Transmission Control Protocol, incoming traffic
 *
 * The input processing functions of the TCP layer.
 *
 * These functions are generally called in the order (ip_input() ->)
 * tcp_input() -> * tcp_process() -> tcp_receive() (-> application).
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
#include "lwip/ip_addr.h"
#include "lwip/netif.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/inet_chksum.h"
#include "lwip/stats.h"
#include "lwip/ip6.h"
#include "lwip/ip6_addr.h"
#if LWIP_ND6_TCP_REACHABILITY_HINTS
#include "lwip/nd6.h"
#endif /* LWIP_ND6_TCP_REACHABILITY_HINTS */

#include <string.h>

void lwip_load_interface_tcp_in(void)
{
    /* Cold boot initialization for "zero_init" retention data */
    memset(&inseg, 0, sizeof(struct tcp_seg));
    tcphdr = NULL;
    tcphdr_optlen = 0;
    tcphdr_opt1len = 0;
    tcphdr_opt2 = NULL;
    tcp_optidx = 0;
    seqno = 0;
    ackno = 0;
    recv_acked = 0;
    tcplen = 0;
    flags = 0;
    recv_flags = 0;
    recv_data = NULL;
    tcp_input_pcb = NULL;


    tcp_process_adpt                    = LWIP_ROMFN(tcp_process);
    tcp_receive_adpt                    = LWIP_ROMFN(tcp_receive);
    tcp_parseopt_adpt                   = LWIP_ROMFN(tcp_parseopt);
    tcp_listen_input_adpt               = LWIP_ROMFN(tcp_listen_input);
    tcp_timewait_input_adpt             = LWIP_ROMFN(tcp_timewait_input);
    tcp_input_delayed_close_adpt        = LWIP_ROMFN(tcp_input_delayed_close);
#if TCP_QUEUE_OOSEQ
    tcp_oos_insert_segment_adpt         = LWIP_ROMFN(tcp_oos_insert_segment);
#endif
    tcp_getoptbyte_adpt                 = LWIP_ROMFN(tcp_getoptbyte);
}

/**
 * The initial input processing of TCP. It verifies the TCP header, demultiplexes
 * the segment between the PCBs and passes it on to tcp_process(), which implements
 * the TCP finite state machine. This function is called by the IP layer (in
 * ip_input()).
 *
 * @param p received TCP segment to process (p->payload pointing to the TCP header)
 * @param inp network interface on which this segment was received
 */
void
tcp_input(struct pbuf *p, struct netif *inp)
{
    tcp_input_adpt(p, inp);
}

/** Called from tcp_input to check for TF_CLOSED flag. This results in closing
 * and deallocating a pcb at the correct place to ensure noone references it
 * any more.
 * @returns 1 if the pcb has been closed and deallocated, 0 otherwise
 */
int
tcp_input_delayed_close(struct tcp_pcb *pcb)
{
    return tcp_input_delayed_close_adpt(pcb);
}

/**
 * Called by tcp_input() when a segment arrives for a listening
 * connection (from tcp_input()).
 *
 * @param pcb the tcp_pcb_listen for which a segment arrived
 *
 * @note the segment which arrived is saved in global variables, therefore only the pcb
 *       involved is passed as a parameter to this function
 */
void
tcp_listen_input(struct tcp_pcb_listen *pcb)
{
    tcp_listen_input_adpt(pcb);
}

/**
 * Called by tcp_input() when a segment arrives for a connection in
 * TIME_WAIT.
 *
 * @param pcb the tcp_pcb for which a segment arrived
 *
 * @note the segment which arrived is saved in global variables, therefore only the pcb
 *       involved is passed as a parameter to this function
 */
void
tcp_timewait_input(struct tcp_pcb *pcb)
{
    tcp_timewait_input_adpt(pcb);
}

/**
 * Implements the TCP state machine. Called by tcp_input. In some
 * states tcp_receive() is called to receive data. The tcp_seg
 * argument will be freed by the caller (tcp_input()) unless the
 * recv_data pointer in the pcb is set.
 *
 * @param pcb the tcp_pcb for which a segment arrived
 *
 * @note the segment which arrived is saved in global variables, therefore only the pcb
 *       involved is passed as a parameter to this function
 */
err_t
tcp_process(struct tcp_pcb *pcb)
{
    return tcp_process_adpt(pcb);
}

#if TCP_QUEUE_OOSEQ
/**
 * Insert segment into the list (segments covered with new one will be deleted)
 *
 * Called from tcp_receive()
 */
void
tcp_oos_insert_segment(struct tcp_seg *cseg, struct tcp_seg *next)
{
    tcp_oos_insert_segment_adpt(cseg, next);
}
#endif /* TCP_QUEUE_OOSEQ */

/**
 * Called by tcp_process. Checks if the given segment is an ACK for outstanding
 * data, and if so frees the memory of the buffered data. Next, it places the
 * segment on any of the receive queues (pcb->recved or pcb->ooseq). If the segment
 * is buffered, the pbuf is referenced by pbuf_ref so that it will not be freed until
 * it has been removed from the buffer.
 *
 * If the incoming segment constitutes an ACK for a segment that was used for RTT
 * estimation, the RTT is estimated here as well.
 *
 * Called from tcp_process().
 */
void
tcp_receive(struct tcp_pcb *pcb)
{
    tcp_receive_adpt(pcb);
}

u8_t
tcp_getoptbyte(void)
{
    return tcp_getoptbyte_adpt();
}

/**
 * Parses the options contained in the incoming segment.
 *
 * Called from tcp_listen_input() and tcp_process().
 * Currently, only the MSS option is supported!
 *
 * @param pcb the tcp_pcb for which a segment arrived
 */
 void
tcp_parseopt(struct tcp_pcb *pcb)
{
    tcp_parseopt_adpt(pcb);
}

void
tcp_trigger_input_pcb_close(void)
{
    tcp_trigger_input_pcb_close_adpt();
}

#endif /* LWIP_TCP */

#endif //#if defined(LWIP_ROMBUILD)

