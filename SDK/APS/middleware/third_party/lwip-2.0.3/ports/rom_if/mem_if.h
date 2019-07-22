/**
 * @file
 * Heap API
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
#ifndef LWIP_HDR_MEM_IF_H
#define LWIP_HDR_MEM_IF_H

#include "lwip/opt.h"

#if defined(LWIP_ROMBUILD)

#ifdef __cplusplus
extern "C" {
#endif

struct mem;
    
void plug_holes(struct mem *mem);
typedef void (*plug_holes_fp_t)(struct mem *mem);

extern plug_holes_fp_t      plug_holes_adpt;


typedef void  (*mem_init_fp_t)(void);
typedef void *(*mem_trim_fp_t)(void *mem, mem_size_t size);
typedef void *(*mem_malloc_fp_t)(mem_size_t size);
typedef void *(*mem_calloc_fp_t)(mem_size_t count, mem_size_t size);
typedef void  (*mem_free_fp_t)(void *mem);

extern mem_init_fp_t        mem_init_adpt;
extern mem_trim_fp_t        mem_trim_adpt;
extern mem_malloc_fp_t      mem_malloc_adpt;
extern mem_calloc_fp_t      mem_calloc_adpt;
extern mem_free_fp_t        mem_free_adpt;

#ifdef __cplusplus
}
#endif

#endif /*#if defined(LWIP_ROMBUILD)*/

#endif /* LWIP_HDR_MEM_IF_H */
