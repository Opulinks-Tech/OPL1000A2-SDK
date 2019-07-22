/**
 * @file
 * Dynamic memory manager
 *
 * This is a lightweight replacement for the standard C library malloc().
 *
 * If you want to use the standard C library malloc() instead, define
 * MEM_LIBC_MALLOC to 1 in your lwipopts.h
 *
 * To let mem_malloc() use pools (prevents fragmentation and is much faster than
 * a heap but might waste some memory), define MEM_USE_POOLS to 1, define
 * MEMP_USE_CUSTOM_POOLS to 1 and create a file "lwippools.h" that includes a list
 * of pools like this (more pools can be added between _START and _END):
 *
 * Define three pools with sizes 256, 512, and 1512 bytes
 * LWIP_MALLOC_MEMPOOL_START
 * LWIP_MALLOC_MEMPOOL(20, 256)
 * LWIP_MALLOC_MEMPOOL(10, 512)
 * LWIP_MALLOC_MEMPOOL(5, 1512)
 * LWIP_MALLOC_MEMPOOL_END
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
 *         Simon Goldschmidt
 *
 */

#include "lwip/opt.h"
#include "lwip/mem.h"
#include "lwip/def.h"
#include "lwip/sys.h"
#include "lwip/stats.h"
#include "lwip/err.h"

#include <string.h>

#if MEM_LIBC_MALLOC
#include <stdlib.h> /* for malloc()/free() */
#endif

#if !MEM_LIBC_MALLOC && !MEM_USE_POOLS
RET_DATA plug_holes_fp_t      plug_holes_adpt;
#endif

RET_DATA mem_init_fp_t        mem_init_adpt;
RET_DATA mem_trim_fp_t        mem_trim_adpt;
RET_DATA mem_malloc_fp_t      mem_malloc_adpt;
RET_DATA mem_calloc_fp_t      mem_calloc_adpt;
RET_DATA mem_free_fp_t        mem_free_adpt;

void lwip_load_interface_mem(void)
{
#if !MEM_LIBC_MALLOC && !MEM_USE_POOLS
    /** pointer to the heap (ram_heap): for alignment, ram is now a pointer instead of an array */
    ram = NULL;
    ram_end = NULL;
    lfree = NULL;

#if !NO_SYS
    mem_mutex = NULL;
#endif
#if LWIP_ALLOW_MEM_FREE_FROM_OTHER_CONTEXT
    mem_free_count = 0;
#endif

    plug_holes_adpt         = LWIP_ROMFN(plug_holes);
#endif
    mem_init_adpt           = LWIP_ROMFN(mem_init);
    mem_trim_adpt           = LWIP_ROMFN(mem_trim);
    mem_malloc_adpt         = LWIP_ROMFN(mem_malloc);
    mem_calloc_adpt         = LWIP_ROMFN(mem_calloc);
    mem_free_adpt           = LWIP_ROMFN(mem_free);
}

#if MEM_LIBC_MALLOC || MEM_USE_POOLS

/** mem_init is not used when using pools instead of a heap or using
 * C library malloc().
 */
void
mem_init(void)
{
    mem_init_adpt();
}

/** mem_trim is not used when using pools instead of a heap or using
 * C library malloc(): we can't free part of a pool element and the stack
 * support mem_trim() to return a different pointer
 */
void*
mem_trim(void *mem, mem_size_t size)
{
    return mem_trim_adpt(mem, size);
}
#endif /* MEM_LIBC_MALLOC || MEM_USE_POOLS */

#if MEM_LIBC_MALLOC
/* lwIP heap implemented using C library malloc() */

/**
 * Allocate a block of memory with a minimum of 'size' bytes.
 *
 * @param size is the minimum size of the requested block in bytes.
 * @return pointer to allocated memory or NULL if no free memory was found.
 *
 * Note that the returned value must always be aligned (as defined by MEM_ALIGNMENT).
 */
void *
mem_malloc(mem_size_t size)
{
    return mem_malloc_adpt(size);
}

/** Put memory back on the heap
 *
 * @param rmem is the pointer as returned by a previous call to mem_malloc()
 */
void
mem_free(void *rmem)
{
    mem_free_adpt(rmem);
}

#elif MEM_USE_POOLS

/* lwIP heap implemented with different sized pools */

/**
 * Allocate memory: determine the smallest pool that is big enough
 * to contain an element of 'size' and get an element from that pool.
 *
 * @param size the size in bytes of the memory needed
 * @return a pointer to the allocated memory or NULL if the pool is empty
 */
void *
mem_malloc(mem_size_t size)
{
    return mem_malloc_adpt(size);
}

/**
 * Free memory previously allocated by mem_malloc. Loads the pool number
 * and calls memp_free with that pool number to put the element back into
 * its pool
 *
 * @param rmem the memory element to free
 */
void
mem_free(void *rmem)
{
    mem_free_adpt(rmem);
}

#else /* MEM_USE_POOLS */
/* lwIP replacement for your libc malloc() */

/**
 * "Plug holes" by combining adjacent empty struct mems.
 * After this function is through, there should not exist
 * one empty struct mem pointing to another empty struct mem.
 *
 * @param mem this points to a struct mem which just has been freed
 * @internal this function is only called by mem_free() and mem_trim()
 *
 * This assumes access to the heap is protected by the calling function
 * already.
 */
void
plug_holes(struct mem *mem)
{
    plug_holes_adpt(mem);
}

/**
 * Zero the heap and initialize start, end and lowest-free
 */
void
mem_init(void)
{
    mem_init_adpt();
}

/**
 * Put a struct mem back on the heap
 *
 * @param rmem is the data portion of a struct mem as returned by a previous
 *             call to mem_malloc()
 */
void
mem_free(void *rmem)
{
    mem_free_adpt(rmem);
}

/**
 * Shrink memory returned by mem_malloc().
 *
 * @param rmem pointer to memory allocated by mem_malloc the is to be shrinked
 * @param newsize required size after shrinking (needs to be smaller than or
 *                equal to the previous size)
 * @return for compatibility reasons: is always == rmem, at the moment
 *         or NULL if newsize is > old size, in which case rmem is NOT touched
 *         or freed!
 */
void *
mem_trim(void *rmem, mem_size_t newsize)
{
    return mem_trim_adpt(rmem, newsize);
}

/**
 * Allocate a block of memory with a minimum of 'size' bytes.
 *
 * @param size is the minimum size of the requested block in bytes.
 * @return pointer to allocated memory or NULL if no free memory was found.
 *
 * Note that the returned value will always be aligned (as defined by MEM_ALIGNMENT).
 */
void *
mem_malloc(mem_size_t size)
{
    return mem_malloc_adpt(size);
}

#endif /* MEM_USE_POOLS */

#if MEM_LIBC_MALLOC && (!LWIP_STATS || !MEM_STATS)
void *
mem_calloc(mem_size_t count, mem_size_t size)
{
    return mem_calloc_adpt(count, size);
}

#else /* MEM_LIBC_MALLOC && (!LWIP_STATS || !MEM_STATS) */
/**
 * Contiguously allocates enough space for count objects that are size bytes
 * of memory each and returns a pointer to the allocated memory.
 *
 * The allocated memory is filled with bytes of value zero.
 *
 * @param count number of objects to allocate
 * @param size size of the objects to allocate
 * @return pointer to allocated memory / NULL pointer if there is an error
 */
void *
mem_calloc(mem_size_t count, mem_size_t size)
{
    return mem_calloc_adpt(count, size);
}
#endif /* MEM_LIBC_MALLOC && (!LWIP_STATS || !MEM_STATS) */
