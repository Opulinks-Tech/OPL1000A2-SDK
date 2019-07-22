/**
 * @file
 * pbuf API
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

#ifndef LWIP_HDR_PBUF_IF_H
#define LWIP_HDR_PBUF_IF_H

#include "lwip/opt.h"
#include "lwip/err.h"

#if defined(LWIP_ROMBUILD)

#ifdef __cplusplus
extern "C" {
#endif

/** LWIP_SUPPORT_CUSTOM_PBUF==1: Custom pbufs behave much like their pbuf type
 * but they are allocated by external code (initialised by calling
 * pbuf_alloced_custom()) and when pbuf_free gives up their last reference, they
 * are freed by calling pbuf_custom->custom_free_function().
 * Currently, the pbuf_custom code is only needed for one specific configuration
 * of IP_FRAG, unless required by external driver/application code. */
#ifndef LWIP_SUPPORT_CUSTOM_PBUF
#define LWIP_SUPPORT_CUSTOM_PBUF ((IP_FRAG && !LWIP_NETIF_TX_SINGLE_PBUF) || (LWIP_IPV6 && LWIP_IPV6_FRAG))
#endif

/** Main packet buffer struct */
struct pbuf;

#if LWIP_SUPPORT_CUSTOM_PBUF
struct pbuf_custom;
#endif /* LWIP_SUPPORT_CUSTOM_PBUF */

/** Define this to 0 to prevent freeing ooseq pbufs when the PBUF_POOL is empty */
#ifndef PBUF_POOL_FREE_OOSEQ
#define PBUF_POOL_FREE_OOSEQ 1
#endif /* PBUF_POOL_FREE_OOSEQ */


/* private static API fucntion used in pbuf.c*/
void pbuf_free_ooseq(void);
void pbuf_free_ooseq_callback(void *arg);
void pbuf_pool_is_empty(void);
u8_t pbuf_header_impl(struct pbuf *p, s16_t header_size_increment, u8_t force);
const struct pbuf* pbuf_skip_const(const struct pbuf* in, u16_t in_offset, u16_t* out_offset);

typedef void (*pbuf_free_ooseq_fp_t)(void);
typedef void (*pbuf_free_ooseq_callback_fp_t)(void *arg);
typedef void (*pbuf_pool_is_empty_fp_t)(void);
typedef u8_t (*pbuf_header_impl_fp_t)(struct pbuf *p, s16_t header_size_increment, u8_t force);
typedef const struct pbuf* (*pbuf_skip_const_fp_t)(const struct pbuf* in, u16_t in_offset, u16_t* out_offset);

#if LWIP_TCP && TCP_QUEUE_OOSEQ &&  PBUF_POOL_FREE_OOSEQ
extern pbuf_free_ooseq_callback_fp_t       pbuf_free_ooseq_callback_adpt;
extern pbuf_pool_is_empty_fp_t             pbuf_pool_is_empty_adpt;
#endif
extern pbuf_header_impl_fp_t               pbuf_header_impl_adpt;
extern pbuf_skip_const_fp_t                pbuf_skip_const_adpt;


typedef struct pbuf *(*pbuf_alloc_fp_t)(pbuf_layer l, u16_t length, pbuf_type type);
#if LWIP_SUPPORT_CUSTOM_PBUF
typedef struct pbuf *(*pbuf_alloced_custom_fp_t)(pbuf_layer l, u16_t length, pbuf_type type,
                                 struct pbuf_custom *p, void *payload_mem,
                                 u16_t payload_mem_len);
#endif /* LWIP_SUPPORT_CUSTOM_PBUF */

typedef void   (*pbuf_realloc_fp_t)(struct pbuf *p, u16_t size);
//typedef u8_t   (*pbuf_header_fp_t)(struct pbuf *p, s16_t header_size);
typedef u8_t   (*pbuf_header_force_fp_t)(struct pbuf *p, s16_t header_size);
typedef void   (*pbuf_ref_fp_t)(struct pbuf *p);
typedef u8_t   (*pbuf_free_fp_t)(struct pbuf *p);
typedef u16_t  (*pbuf_clen_fp_t)(const struct pbuf *p);
typedef void   (*pbuf_cat_fp_t)(struct pbuf *head, struct pbuf *tail);
typedef void   (*pbuf_chain_fp_t)(struct pbuf *head, struct pbuf *tail);
typedef struct pbuf *(*pbuf_dechain_fp_t)(struct pbuf *p);
typedef err_t  (*pbuf_copy_fp_t)(struct pbuf *p_to, const struct pbuf *p_from);
typedef u16_t  (*pbuf_copy_partial_fp_t)(const struct pbuf *p, void *dataptr, u16_t len, u16_t offset);
typedef err_t  (*pbuf_take_fp_t)(struct pbuf *buf, const void *dataptr, u16_t len);
typedef err_t  (*pbuf_take_at_fp_t)(struct pbuf *buf, const void *dataptr, u16_t len, u16_t offset);
typedef struct pbuf *(*pbuf_skip_fp_t)(struct pbuf* in, u16_t in_offset, u16_t* out_offset);
typedef struct pbuf *(*pbuf_coalesce_fp_t)(struct pbuf *p, pbuf_layer layer);
#if LWIP_CHECKSUM_ON_COPY
typedef err_t  (*pbuf_fill_chksum_fp_t)(struct pbuf *p, u16_t start_offset, const void *dataptr,
                       u16_t len, u16_t *chksum);
#endif /* LWIP_CHECKSUM_ON_COPY */
#if LWIP_TCP && TCP_QUEUE_OOSEQ && LWIP_WND_SCALE
typedef void   (*pbuf_split_64k_fp_t)(struct pbuf *p, struct pbuf **rest);
#endif /* LWIP_TCP && TCP_QUEUE_OOSEQ && LWIP_WND_SCALE */

typedef u8_t   (*pbuf_get_at_fp_t)(const struct pbuf* p, u16_t offset);
typedef int    (*pbuf_try_get_at_fp_t)(const struct pbuf* p, u16_t offset);
typedef void   (*pbuf_put_at_fp_t)(struct pbuf* p, u16_t offset, u8_t data);
typedef u16_t  (*pbuf_memcmp_fp_t)(const struct pbuf* p, u16_t offset, const void* s2, u16_t n);
typedef u16_t  (*pbuf_memfind_fp_t)(const struct pbuf* p, const void* mem, u16_t mem_len, u16_t start_offset);
typedef u16_t  (*pbuf_strstr_fp_t)(const struct pbuf* p, const char* substr);



#if LWIP_TCP && TCP_QUEUE_OOSEQ && PBUF_POOL_FREE_OOSEQ
extern pbuf_free_ooseq_fp_t     pbuf_free_ooseq_adpt;
#endif /* LWIP_TCP && TCP_QUEUE_OOSEQ && NO_SYS && PBUF_POOL_FREE_OOSEQ*/
extern pbuf_alloc_fp_t          pbuf_alloc_adpt;
#if LWIP_SUPPORT_CUSTOM_PBUF
extern pbuf_alloced_custom_fp_t pbuf_alloced_custom_adpt;
#endif /* LWIP_SUPPORT_CUSTOM_PBUF */
extern pbuf_realloc_fp_t               pbuf_realloc_adpt;
//extern pbuf_header_fp_t                pbuf_header_adpt;
extern pbuf_header_force_fp_t          pbuf_header_force_adpt;
extern pbuf_ref_fp_t                   pbuf_ref_adpt;
extern pbuf_free_fp_t                  pbuf_free_adpt;
extern pbuf_clen_fp_t                  pbuf_clen_adpt;
extern pbuf_cat_fp_t                   pbuf_cat_adpt;
extern pbuf_chain_fp_t                 pbuf_chain_adpt;
extern pbuf_dechain_fp_t               pbuf_dechain_adpt;
extern pbuf_copy_fp_t                  pbuf_copy_adpt;
extern pbuf_copy_partial_fp_t          pbuf_copy_partial_adpt;
extern pbuf_take_fp_t                  pbuf_take_adpt;
extern pbuf_take_at_fp_t               pbuf_take_at_adpt;
extern pbuf_skip_fp_t                  pbuf_skip_adpt;
extern pbuf_coalesce_fp_t              pbuf_coalesce_adpt;
#if LWIP_CHECKSUM_ON_COPY
extern pbuf_fill_chksum_fp_t           pbuf_fill_chksum_adpt;

#endif /* LWIP_CHECKSUM_ON_COPY */
#if LWIP_TCP && TCP_QUEUE_OOSEQ && LWIP_WND_SCALE
extern pbuf_split_64k_fp_t             pbuf_split_64k_adpt;
#endif /* LWIP_TCP && TCP_QUEUE_OOSEQ && LWIP_WND_SCALE */

extern pbuf_get_at_fp_t                pbuf_get_at_adpt;
extern pbuf_try_get_at_fp_t            pbuf_try_get_at_adpt;
extern pbuf_put_at_fp_t                pbuf_put_at_adpt;
extern pbuf_memcmp_fp_t                pbuf_memcmp_adpt;
extern pbuf_memfind_fp_t               pbuf_memfind_adpt;
extern pbuf_strstr_fp_t                pbuf_strstr_adpt;


#ifdef __cplusplus
}
#endif

#endif /*#if defined(LWIP_ROMBUILD)*/

#endif /* LWIP_HDR_PBUF_IF_H */
