/**
 * @file
 * netbuf API (for netconn API)
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
#ifndef LWIP_HDR_NETBUF_IF_H
#define LWIP_HDR_NETBUF_IF_H

#include "lwip/opt.h"

#if defined(LWIP_ROMBUILD)


#if LWIP_NETCONN || LWIP_SOCKET /* don't build if not configured for use in lwipopts.h */
/* Note: Netconn API is always available when sockets are enabled -
 * sockets are implemented on top of them */

#include "lwip/pbuf.h"
#include "lwip/ip_addr.h"
#include "lwip/ip6_addr.h"

#ifdef __cplusplus
extern "C" {
#endif

struct netbuf;

/* Network buffer functions: */
typedef struct netbuf * (*netbuf_new_fp_t)(void);
typedef void  (*netbuf_delete_fp_t)  (struct netbuf *buf);
typedef void *(*netbuf_alloc_fp_t) (struct netbuf *buf, u16_t size);
typedef void  (*netbuf_free_fp_t)    (struct netbuf *buf);
typedef err_t (*netbuf_ref_fp_t)     (struct netbuf *buf, const void *dataptr, u16_t size);
typedef void  (*netbuf_chain_fp_t)   (struct netbuf *head, struct netbuf *tail);
typedef err_t (*netbuf_data_fp_t)    (struct netbuf *buf, void **dataptr, u16_t *len);
typedef s8_t  (*netbuf_next_fp_t)    (struct netbuf *buf);
typedef void  (*netbuf_first_fp_t)   (struct netbuf *buf);

extern netbuf_new_fp_t         netbuf_new_adpt;
extern netbuf_delete_fp_t      netbuf_delete_adpt;
extern netbuf_alloc_fp_t       netbuf_alloc_adpt;
extern netbuf_free_fp_t        netbuf_free_adpt;
extern netbuf_ref_fp_t         netbuf_ref_adpt;
extern netbuf_chain_fp_t       netbuf_chain_adpt;
extern netbuf_data_fp_t        netbuf_data_adpt;
extern netbuf_next_fp_t        netbuf_next_adpt;
extern netbuf_first_fp_t       netbuf_first_adpt;

#ifdef __cplusplus
}
#endif

#endif /* LWIP_NETCONN || LWIP_SOCKET */

#endif /*#if defined(LWIP_ROMBUILD)*/

#endif /* LWIP_HDR_NETBUF_IF_H */
