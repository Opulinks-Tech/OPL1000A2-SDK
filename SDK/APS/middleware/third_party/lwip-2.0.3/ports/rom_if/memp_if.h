/**
 * @file
 * Memory pool API
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

#ifndef LWIP_HDR_MEMP_IF_H
#define LWIP_HDR_MEMP_IF_H

#include "lwip/opt.h"

#if defined(LWIP_ROMBUILD)

#ifdef __cplusplus
extern "C" {
#endif

struct memp_desc;

/* Private static API fucntion used in memp.c*/
#if MEMP_SANITY_CHECK && !MEMP_MEM_MALLOC
int  memp_sanity(const struct memp_desc *desc);
#endif

#if MEMP_OVERFLOW_CHECK
void memp_overflow_check_element_overflow(struct memp *p, const struct memp_desc *desc);
void memp_overflow_check_element_underflow(struct memp *p, const struct memp_desc *desc);
void memp_overflow_init_element(struct memp *p, const struct memp_desc *desc);
#if MEMP_OVERFLOW_CHECK >= 2
void memp_overflow_check_all(void);
#endif /* MEMP_OVERFLOW_CHECK >= 2 */
#endif /* MEMP_OVERFLOW_CHECK */

#if !MEMP_OVERFLOW_CHECK
void *do_memp_malloc_pool(const struct memp_desc *desc);
#else
void *do_memp_malloc_pool_fn(const struct memp_desc *desc, const char* file, const int line);
#endif
void  do_memp_free_pool(const struct memp_desc* desc, void *mem);



#if MEMP_SANITY_CHECK && !MEMP_MEM_MALLOC
typedef int  (*memp_sanity_fp_t)(const struct memp_desc *desc);
#endif

#if MEMP_OVERFLOW_CHECK
typedef void (*memp_overflow_check_element_overflow_fp_t)(struct memp *p, const struct memp_desc *desc);
typedef void (*memp_overflow_check_element_underflow_fp_t)(struct memp *p, const struct memp_desc *desc);
typedef void (*memp_overflow_init_element_fp_t)(struct memp *p, const struct memp_desc *desc);

#if MEMP_OVERFLOW_CHECK >= 2
typedef void (*memp_overflow_check_all_fp_t)(void);
#endif /* MEMP_OVERFLOW_CHECK >= 2 */
#endif /* MEMP_OVERFLOW_CHECK */


#if !MEMP_OVERFLOW_CHECK
typedef void *(*do_memp_malloc_pool_fp_t)(const struct memp_desc *desc);
#else
typedef void *(*do_memp_malloc_pool_fn_fp_t)(const struct memp_desc *desc, const char* file, const int line);
#endif
typedef void  (*do_memp_free_pool_fp_t)(const struct memp_desc* desc, void *mem);


#if MEMP_SANITY_CHECK && !MEMP_MEM_MALLOC
extern memp_sanity_fp_t                             memp_sanity_adpt;
#endif
#if MEMP_OVERFLOW_CHECK
extern memp_overflow_check_element_overflow_fp_t    memp_overflow_check_element_overflow_adpt;
extern memp_overflow_check_element_underflow_fp_t   memp_overflow_check_element_underflow_adpt;
extern memp_overflow_init_element_fp_t              memp_overflow_init_element_adpt;
#if MEMP_OVERFLOW_CHECK >= 2
extern memp_overflow_check_all_fp_t                 memp_overflow_check_all_adpt;
#endif /* MEMP_OVERFLOW_CHECK >= 2 */
#endif /* MEMP_OVERFLOW_CHECK */

#if !MEMP_OVERFLOW_CHECK
extern do_memp_malloc_pool_fp_t                     do_memp_malloc_pool_adpt;
#else
extern do_memp_malloc_pool_fn_fp_t                  do_memp_malloc_pool_fn_adpt;
#endif
extern do_memp_free_pool_fp_t                       do_memp_free_pool_adpt;


/* Private API fucntion used in memp.c, declare in memp_priv.h*/
typedef void (*memp_init_pool_fp_t)(const struct memp_desc *desc);
#if MEMP_OVERFLOW_CHECK
typedef void *(*memp_malloc_pool_fn_fp_t)(const struct memp_desc* desc, const char* file, const int line);
#else
typedef void *(*memp_malloc_pool_fp_t)(const struct memp_desc *desc);
#endif
typedef void  (*memp_free_pool_fp_t)(const struct memp_desc* desc, void *mem);

extern memp_init_pool_fp_t      memp_init_pool_adpt;
#if MEMP_OVERFLOW_CHECK
extern memp_malloc_pool_fn_fp_t memp_malloc_pool_fn_adpt;
#else
extern memp_malloc_pool_fp_t    memp_malloc_pool_adpt;
#endif
extern memp_free_pool_fp_t      memp_free_pool_adpt;


/* Public API fucntion used in memp.c*/
typedef void  (*memp_init_fp_t)(void);
#if MEMP_OVERFLOW_CHECK
typedef void *(*memp_malloc_fn_fp_t)(memp_t type, const char* file, const int line);
#else
typedef void *(*memp_malloc_fp_t)(memp_t type);
#endif
typedef void  (*memp_free_fp_t)(memp_t type, void *mem);

extern memp_init_fp_t           memp_init_adpt;
#if MEMP_OVERFLOW_CHECK
extern memp_malloc_fn_fp_t      memp_malloc_fn_adpt;
#else
extern memp_malloc_fp_t         memp_malloc_adpt;
#endif
extern memp_free_fp_t           memp_free_adpt;

#ifdef __cplusplus
}
#endif

#endif /*#if defined(LWIP_ROMBUILD)*/

#endif /* LWIP_HDR_MEMP_IF_H */
