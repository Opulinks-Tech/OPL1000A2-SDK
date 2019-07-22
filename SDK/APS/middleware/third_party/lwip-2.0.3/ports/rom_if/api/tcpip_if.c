/**
 * @file
 * Sequential API Main thread module
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

#if !NO_SYS /* don't build if not configured for use in lwipopts.h */

#include "lwip/priv/tcpip_priv.h"
#include "lwip/sys.h"
#include "lwip/memp.h"
#include "lwip/mem.h"
#include "lwip/init.h"
#include "lwip/ip.h"
#include "lwip/pbuf.h"
#include "lwip/etharp.h"
#include "netif/ethernet.h"
#include "sys_os_config.h"


RET_DATA tcpip_thread_fp_t                      tcpip_thread_adpt;
RET_DATA pbuf_free_int_fp_t                     pbuf_free_int_adpt;

RET_DATA tcpip_send_msg_wait_sem_fp_t           tcpip_send_msg_wait_sem_adpt;
RET_DATA tcpip_api_call_fp_t                    tcpip_api_call_adpt;
RET_DATA tcpip_init_fp_t                        tcpip_init_adpt;
RET_DATA tcpip_inpkt_fp_t                       tcpip_inpkt_adpt;
RET_DATA tcpip_input_fp_t                       tcpip_input_adpt;
RET_DATA tcpip_callback_with_block_fp_t         tcpip_callback_with_block_adpt;
RET_DATA tcpip_callbackmsg_new_fp_t             tcpip_callbackmsg_new_adpt;
RET_DATA tcpip_callbackmsg_delete_fp_t          tcpip_callbackmsg_delete_adpt;
RET_DATA tcpip_trycallback_fp_t                 tcpip_trycallback_adpt;
RET_DATA pbuf_free_callback_fp_t                pbuf_free_callback_adpt;
RET_DATA mem_free_callback_fp_t                 mem_free_callback_adpt;

#if LWIP_TCPIP_TIMEOUT && LWIP_TIMERS
RET_DATA tcpip_timeout_fp_t                     tcpip_timeout_adpt;
RET_DATA tcpip_untimeout_fp_t                   tcpip_untimeout_adpt;
#endif /* LWIP_TCPIP_TIMEOUT && LWIP_TIMERS */

RET_DATA tcpip_deactivate_tcpip_timeouts_fp_t   tcpip_deactivate_tcpip_timeouts_adpt;
RET_DATA tcpip_activate_tcpip_timeouts_fp_t     tcpip_activate_tcpip_timeouts_adpt;
RET_DATA tcpip_check_timeouts_fp_t              tcpip_check_timeouts_adpt;

void lwip_load_interface_tcpip(void)
{
    /* Cold boot initialization for "zero_init" retention data */
    tcpip_init_done = NULL;
    tcpip_init_done_arg = NULL;
    mbox = NULL;

    tcpip_thread_stacksize = OS_TASK_STACK_SIZE_TCPIP;
    tcpip_thread_prio = OS_TASK_PRIORITY_TCPIP;

#if LWIP_TCPIP_CORE_LOCKING
    /** The global semaphore to lock the stack. */
    lock_tcpip_core = NULL;
#endif /* LWIP_TCPIP_CORE_LOCKING */

    tcpip_thread_adpt                   = LWIP_ROMFN(tcpip_thread);
    pbuf_free_int_adpt                  = LWIP_ROMFN(pbuf_free_int);

    tcpip_send_msg_wait_sem_adpt        = LWIP_ROMFN(tcpip_send_msg_wait_sem);
    tcpip_api_call_adpt                 = LWIP_ROMFN(tcpip_api_call);

    tcpip_init_adpt                     = LWIP_ROMFN(tcpip_init);
    tcpip_inpkt_adpt                    = LWIP_ROMFN(tcpip_inpkt);
    tcpip_input_adpt                    = LWIP_ROMFN(tcpip_input);
    tcpip_callback_with_block_adpt      = LWIP_ROMFN(tcpip_callback_with_block);
    tcpip_callbackmsg_new_adpt          = LWIP_ROMFN(tcpip_callbackmsg_new);
    tcpip_callbackmsg_delete_adpt       = LWIP_ROMFN(tcpip_callbackmsg_delete);
    tcpip_trycallback_adpt              = LWIP_ROMFN(tcpip_trycallback);
    pbuf_free_callback_adpt             = LWIP_ROMFN(pbuf_free_callback);
    mem_free_callback_adpt              = LWIP_ROMFN(mem_free_callback);

#if LWIP_TCPIP_TIMEOUT && LWIP_TIMERS
    tcpip_timeout_adpt                  = LWIP_ROMFN(tcpip_timeout);
    tcpip_untimeout_adpt                = LWIP_ROMFN(tcpip_untimeout);
/* OPULINKS_CHANGES */
    tcpip_deactivate_tcpip_timeouts_adpt= LWIP_ROMFN(tcpip_deactivate_tcpip_timeouts);
    tcpip_activate_tcpip_timeouts_adpt  = LWIP_ROMFN(tcpip_activate_tcpip_timeouts);
    tcpip_check_timeouts_adpt           = LWIP_ROMFN(tcpip_check_timeouts);
/* OPULINKS_CHANGES */
#endif /* LWIP_TCPIP_TIMEOUT && LWIP_TIMERS */
}


/**
 * The main lwIP thread. This thread has exclusive access to lwIP core functions
 * (unless access to them is not locked). Other threads communicate with this
 * thread using message boxes.
 *
 * It also starts all the timers to make sure they are running in the right
 * thread context.
 *
 * @param arg unused argument
 */
void
tcpip_thread(void *arg)
{
    tcpip_thread_adpt(arg);
}

/**
 * Pass a received packet to tcpip_thread for input processing
 *
 * @param p the received packet
 * @param inp the network interface on which the packet was received
 * @param input_fn input function to call
 */
err_t
tcpip_inpkt(struct pbuf *p, struct netif *inp, netif_input_fn input_fn)
{
    return tcpip_inpkt_adpt(p, inp, input_fn);
}

/**
 * @ingroup lwip_os
 * Pass a received packet to tcpip_thread for input processing with
 * ethernet_input or ip_input. Don't call directly, pass to netif_add()
 * and call netif->input().
 *
 * @param p the received packet, p->payload pointing to the Ethernet header or
 *          to an IP header (if inp doesn't have NETIF_FLAG_ETHARP or
 *          NETIF_FLAG_ETHERNET flags)
 * @param inp the network interface on which the packet was received
 */
err_t
tcpip_input(struct pbuf *p, struct netif *inp)
{
    return tcpip_input_adpt(p, inp);
}

/**
 * Call a specific function in the thread context of
 * tcpip_thread for easy access synchronization.
 * A function called in that way may access lwIP core code
 * without fearing concurrent access.
 *
 * @param function the function to call
 * @param ctx parameter passed to f
 * @param block 1 to block until the request is posted, 0 to non-blocking mode
 * @return ERR_OK if the function was called, another err_t if not
 */
err_t
tcpip_callback_with_block(tcpip_callback_fn function, void *ctx, u8_t block)
{
    return tcpip_callback_with_block_adpt(function, ctx, block);
}

#if LWIP_TCPIP_TIMEOUT && LWIP_TIMERS
/**
 * call sys_timeout in tcpip_thread
 *
 * @param msecs time in milliseconds for timeout
 * @param h function to be called on timeout
 * @param arg argument to pass to timeout function h
 * @return ERR_MEM on memory error, ERR_OK otherwise
 */
err_t
tcpip_timeout(u32_t msecs, sys_timeout_handler h, void *arg)
{
    return tcpip_timeout_adpt(msecs, h, arg);
}

/**
 * call sys_untimeout in tcpip_thread
 *
 * @param h function to be called on timeout
 * @param arg argument to pass to timeout function h
 * @return ERR_MEM on memory error, ERR_OK otherwise
 */
err_t
tcpip_untimeout(sys_timeout_handler h, void *arg)
{
    return tcpip_untimeout_adpt(h, arg);
}
#endif /* LWIP_TCPIP_TIMEOUT && LWIP_TIMERS */

/**
 * @return ERR_MEM on memory error, ERR_OK otherwise
 */
err_t
tcpip_deactivate_tcpip_timeouts(void)
{
    return tcpip_deactivate_tcpip_timeouts_adpt();
}

/**
 * @return ERR_MEM on memory error, ERR_OK otherwise
 */
err_t
tcpip_activate_tcpip_timeouts(void)
{
    return tcpip_activate_tcpip_timeouts_adpt();
}

err_t
tcpip_check_timeouts(void)
{
    return tcpip_check_timeouts_adpt();
}


/**
 * Sends a message to TCPIP thread to call a function. Caller thread blocks on
 * on a provided semaphore, which ist NOT automatically signalled by TCPIP thread,
 * this has to be done by the user.
 * It is recommended to use LWIP_TCPIP_CORE_LOCKING since this is the way
 * with least runtime overhead.
 *
 * @param fn function to be called from TCPIP thread
 * @param apimsg argument to API function
 * @param sem semaphore to wait on
 * @return ERR_OK if the function was called, another err_t if not
 */
err_t
tcpip_send_msg_wait_sem(tcpip_callback_fn fn, void *apimsg, sys_sem_t* sem)
{
    return tcpip_send_msg_wait_sem_adpt(fn, apimsg, sem);
}

/**
 * Synchronously calls function in TCPIP thread and waits for its completion.
 * It is recommended to use LWIP_TCPIP_CORE_LOCKING (preferred) or
 * LWIP_NETCONN_SEM_PER_THREAD.
 * If not, a semaphore is created and destroyed on every call which is usually
 * an expensive/slow operation.
 * @param fn Function to call
 * @param call Call parameters
 * @return Return value from tcpip_api_call_fn
 */
err_t
tcpip_api_call(tcpip_api_call_fn fn, struct tcpip_api_call_data *call)
{
    return tcpip_api_call_adpt(fn, call);
}

/**
 * Allocate a structure for a static callback message and initialize it.
 * This is intended to be used to send "static" messages from interrupt context.
 *
 * @param function the function to call
 * @param ctx parameter passed to function
 * @return a struct pointer to pass to tcpip_trycallback().
 */
struct tcpip_callback_msg*
tcpip_callbackmsg_new(tcpip_callback_fn function, void *ctx)
{
    return tcpip_callbackmsg_new_adpt(function, ctx);
}

/**
 * Free a callback message allocated by tcpip_callbackmsg_new().
 *
 * @param msg the message to free
 */
void
tcpip_callbackmsg_delete(struct tcpip_callback_msg* msg)
{
    tcpip_callbackmsg_delete_adpt(msg);
}

/**
 * Try to post a callback-message to the tcpip_thread mbox
 * This is intended to be used to send "static" messages from interrupt context.
 *
 * @param msg pointer to the message to post
 * @return sys_mbox_trypost() return code
 */
err_t
tcpip_trycallback(struct tcpip_callback_msg* msg)
{
    return tcpip_trycallback_adpt(msg);
}

/**
 * @ingroup lwip_os
 * Initialize this module:
 * - initialize all sub modules
 * - start the tcpip_thread
 *
 * @param initfunc a function to call when tcpip_thread is running and finished initializing
 * @param arg argument to pass to initfunc
 */
void
tcpip_init(tcpip_init_done_fn initfunc, void *arg)
{
    tcpip_init_adpt(initfunc, arg);
}

/**
 * Simple callback function used with tcpip_callback to free a pbuf
 * (pbuf_free has a wrong signature for tcpip_callback)
 *
 * @param p The pbuf (chain) to be dereferenced.
 */
void
pbuf_free_int(void *p)
{
    pbuf_free_int_adpt(p);
}

/**
 * A simple wrapper function that allows you to free a pbuf from interrupt context.
 *
 * @param p The pbuf (chain) to be dereferenced.
 * @return ERR_OK if callback could be enqueued, an err_t if not
 */
err_t
pbuf_free_callback(struct pbuf *p)
{
    return pbuf_free_callback_adpt(p);
}

/**
 * A simple wrapper function that allows you to free heap memory from
 * interrupt context.
 *
 * @param m the heap memory to free
 * @return ERR_OK if callback could be enqueued, an err_t if not
 */
err_t
mem_free_callback(void *m)
{
    return mem_free_callback_adpt(m);
}

#endif /* !NO_SYS */
