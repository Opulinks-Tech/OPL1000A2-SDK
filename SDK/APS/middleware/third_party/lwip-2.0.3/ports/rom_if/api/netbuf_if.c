/**
 * @file
 * Network buffer management
 *
 * @defgroup netbuf Network buffers
 * @ingroup netconn
 * Network buffer descriptor for @ref netconn. Based on @ref pbuf internally
 * to avoid copying data around.\n
 * Buffers must not be shared accross multiple threads, all functions except
 * netbuf_new() and netbuf_delete() are not thread-safe.
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

#if LWIP_NETCONN /* don't build if not configured for use in lwipopts.h */

#include "lwip/netbuf.h"
#include "lwip/memp.h"

#include <string.h>


RET_DATA netbuf_new_fp_t         netbuf_new_adpt;
RET_DATA netbuf_delete_fp_t      netbuf_delete_adpt;
RET_DATA netbuf_alloc_fp_t       netbuf_alloc_adpt;
RET_DATA netbuf_free_fp_t        netbuf_free_adpt;
RET_DATA netbuf_ref_fp_t         netbuf_ref_adpt;
RET_DATA netbuf_chain_fp_t       netbuf_chain_adpt;
RET_DATA netbuf_data_fp_t        netbuf_data_adpt;
RET_DATA netbuf_next_fp_t        netbuf_next_adpt;
RET_DATA netbuf_first_fp_t       netbuf_first_adpt;

void lwip_load_interface_netbuf(void)
{
    netbuf_new_adpt         = LWIP_ROMFN(netbuf_new);
    netbuf_delete_adpt      = LWIP_ROMFN(netbuf_delete);
    netbuf_alloc_adpt       = LWIP_ROMFN(netbuf_alloc);
    netbuf_free_adpt        = LWIP_ROMFN(netbuf_free);
    netbuf_ref_adpt         = LWIP_ROMFN(netbuf_ref);
    netbuf_chain_adpt       = LWIP_ROMFN(netbuf_chain);
    netbuf_data_adpt        = LWIP_ROMFN(netbuf_data);
    netbuf_next_adpt        = LWIP_ROMFN(netbuf_next);
    netbuf_first_adpt       = LWIP_ROMFN(netbuf_first);
}

/**
 * @ingroup netbuf
 * Create (allocate) and initialize a new netbuf.
 * The netbuf doesn't yet contain a packet buffer!
 *
 * @return a pointer to a new netbuf
 *         NULL on lack of memory
 */
struct
netbuf *netbuf_new(void)
{
    return netbuf_new_adpt();
}

/**
 * @ingroup netbuf
 * Deallocate a netbuf allocated by netbuf_new().
 *
 * @param buf pointer to a netbuf allocated by netbuf_new()
 */
void
netbuf_delete(struct netbuf *buf)
{
    netbuf_delete_adpt(buf);
}

/**
 * @ingroup netbuf
 * Allocate memory for a packet buffer for a given netbuf.
 *
 * @param buf the netbuf for which to allocate a packet buffer
 * @param size the size of the packet buffer to allocate
 * @return pointer to the allocated memory
 *         NULL if no memory could be allocated
 */
void *
netbuf_alloc(struct netbuf *buf, u16_t size)
{
    return netbuf_alloc_adpt(buf, size);
}

/**
 * @ingroup netbuf
 * Free the packet buffer included in a netbuf
 *
 * @param buf pointer to the netbuf which contains the packet buffer to free
 */
void
netbuf_free(struct netbuf *buf)
{
    netbuf_free_adpt(buf);
}

/**
 * @ingroup netbuf
 * Let a netbuf reference existing (non-volatile) data.
 *
 * @param buf netbuf which should reference the data
 * @param dataptr pointer to the data to reference
 * @param size size of the data
 * @return ERR_OK if data is referenced
 *         ERR_MEM if data couldn't be referenced due to lack of memory
 */
err_t
netbuf_ref(struct netbuf *buf, const void *dataptr, u16_t size)
{
    return netbuf_ref_adpt(buf, dataptr, size);
}

/**
 * @ingroup netbuf
 * Chain one netbuf to another (@see pbuf_chain)
 *
 * @param head the first netbuf
 * @param tail netbuf to chain after head, freed by this function, may not be reference after returning
 */
void
netbuf_chain(struct netbuf *head, struct netbuf *tail)
{
    netbuf_chain_adpt(head, tail);
}

/**
 * @ingroup netbuf
 * Get the data pointer and length of the data inside a netbuf.
 *
 * @param buf netbuf to get the data from
 * @param dataptr pointer to a void pointer where to store the data pointer
 * @param len pointer to an u16_t where the length of the data is stored
 * @return ERR_OK if the information was retrieved,
 *         ERR_BUF on error.
 */
err_t
netbuf_data(struct netbuf *buf, void **dataptr, u16_t *len)
{
    return netbuf_data_adpt(buf, dataptr, len);
}

/**
 * @ingroup netbuf
 * Move the current data pointer of a packet buffer contained in a netbuf
 * to the next part.
 * The packet buffer itself is not modified.
 *
 * @param buf the netbuf to modify
 * @return -1 if there is no next part
 *         1  if moved to the next part but now there is no next part
 *         0  if moved to the next part and there are still more parts
 */
s8_t
netbuf_next(struct netbuf *buf)
{
    return netbuf_next_adpt(buf);
}

/**
 * @ingroup netbuf
 * Move the current data pointer of a packet buffer contained in a netbuf
 * to the beginning of the packet.
 * The packet buffer itself is not modified.
 *
 * @param buf the netbuf to modify
 */
void
netbuf_first(struct netbuf *buf)
{
    netbuf_first_adpt(buf);
}

#endif /* LWIP_NETCONN */
