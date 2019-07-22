/**
 * @file
 * Packet buffer management
 */

/**
 * @defgroup pbuf Packet buffers (PBUF)
 * @ingroup infrastructure
 *
 * Packets are built from the pbuf data structure. It supports dynamic
 * memory allocation for packet contents or can reference externally
 * managed packet contents both in RAM and ROM. Quick allocation for
 * incoming packets is provided through pools with fixed sized pbufs.
 *
 * A packet may span over multiple pbufs, chained as a singly linked
 * list. This is called a "pbuf chain".
 *
 * Multiple packets may be queued, also using this singly linked list.
 * This is called a "packet queue".
 *
 * So, a packet queue consists of one or more pbuf chains, each of
 * which consist of one or more pbufs. CURRENTLY, PACKET QUEUES ARE
 * NOT SUPPORTED!!! Use helper structs to queue multiple packets.
 *
 * The differences between a pbuf chain and a packet queue are very
 * precise but subtle.
 *
 * The last pbuf of a packet has a ->tot_len field that equals the
 * ->len field. It can be found by traversing the list. If the last
 * pbuf of a packet has a ->next field other than NULL, more packets
 * are on the queue.
 *
 * Therefore, looping through a pbuf of a single packet, has an
 * loop end condition (tot_len == p->len), NOT (next == NULL).
 *
 * Example of custom pbuf usage for zero-copy RX:
  @code{.c}
typedef struct my_custom_pbuf
{
   struct pbuf_custom p;
   void* dma_descriptor;
} my_custom_pbuf_t;

LWIP_MEMPOOL_DECLARE(RX_POOL, 10, sizeof(my_custom_pbuf_t), "Zero-copy RX PBUF pool");

void my_pbuf_free_custom(void* p)
{
  my_custom_pbuf_t* my_puf = (my_custom_pbuf_t*)p;

  LOCK_INTERRUPTS();
  free_rx_dma_descriptor(my_pbuf->dma_descriptor);
  LWIP_MEMPOOL_FREE(RX_POOL, my_pbuf);
  UNLOCK_INTERRUPTS();
}

void eth_rx_irq()
{
  dma_descriptor*   dma_desc = get_RX_DMA_descriptor_from_ethernet();
  my_custom_pbuf_t* my_pbuf  = (my_custom_pbuf_t*)LWIP_MEMPOOL_ALLOC(RX_POOL);

  my_pbuf->p.custom_free_function = my_pbuf_free_custom;
  my_pbuf->dma_descriptor         = dma_desc;

  invalidate_cpu_cache(dma_desc->rx_data, dma_desc->rx_length);

  struct pbuf* p = pbuf_alloced_custom(PBUF_RAW,
     dma_desc->rx_length,
     PBUF_REF,
     &my_pbuf->p,
     dma_desc->rx_data,
     dma_desc->max_buffer_size);

  if(netif->input(p, netif) != ERR_OK) {
    pbuf_free(p);
  }
}
  @endcode
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

#include "lwip/stats.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/pbuf.h"
#include "lwip/sys.h"
#if LWIP_TCP && TCP_QUEUE_OOSEQ
#include "lwip/priv/tcp_priv.h"
#endif
#if LWIP_CHECKSUM_ON_COPY
#include "lwip/inet_chksum.h"
#endif

#include <string.h>


#if LWIP_TCP && TCP_QUEUE_OOSEQ && PBUF_POOL_FREE_OOSEQ
// private static api
RET_DATA pbuf_free_ooseq_fp_t           pbuf_free_ooseq_adpt;
RET_DATA pbuf_free_ooseq_callback_fp_t  pbuf_free_ooseq_callback_adpt;
RET_DATA pbuf_pool_is_empty_fp_t        pbuf_pool_is_empty_adpt;
#endif /* LWIP_TCP && TCP_QUEUE_OOSEQ && NO_SYS && PBUF_POOL_FREE_OOSEQ*/
RET_DATA pbuf_header_impl_fp_t          pbuf_header_impl_adpt;
RET_DATA pbuf_skip_const_fp_t           pbuf_skip_const_adpt;

RET_DATA pbuf_alloc_fp_t                pbuf_alloc_adpt;
#if LWIP_SUPPORT_CUSTOM_PBUF
RET_DATA pbuf_alloced_custom_fp_t       pbuf_alloced_custom_adpt;
#endif /* LWIP_SUPPORT_CUSTOM_PBUF */
RET_DATA pbuf_realloc_fp_t              pbuf_realloc_adpt;
//RET_DATA pbuf_header_fp_t                pbuf_header_adpt;
RET_DATA pbuf_header_force_fp_t         pbuf_header_force_adpt;
RET_DATA pbuf_ref_fp_t                  pbuf_ref_adpt;
RET_DATA pbuf_free_fp_t                 pbuf_free_adpt;
RET_DATA pbuf_clen_fp_t                 pbuf_clen_adpt;
RET_DATA pbuf_cat_fp_t                  pbuf_cat_adpt;
RET_DATA pbuf_chain_fp_t                pbuf_chain_adpt;
RET_DATA pbuf_dechain_fp_t              pbuf_dechain_adpt;
RET_DATA pbuf_copy_fp_t                 pbuf_copy_adpt;
RET_DATA pbuf_copy_partial_fp_t         pbuf_copy_partial_adpt;
RET_DATA pbuf_take_fp_t                 pbuf_take_adpt;
RET_DATA pbuf_take_at_fp_t              pbuf_take_at_adpt;
RET_DATA pbuf_skip_fp_t                 pbuf_skip_adpt;
RET_DATA pbuf_coalesce_fp_t             pbuf_coalesce_adpt;
#if LWIP_CHECKSUM_ON_COPY
RET_DATA pbuf_fill_chksum_fp_t          pbuf_fill_chksum_adpt;

#endif /* LWIP_CHECKSUM_ON_COPY */
#if LWIP_TCP && TCP_QUEUE_OOSEQ && LWIP_WND_SCALE
RET_DATA pbuf_split_64k_fp_t             pbuf_split_64k_adpt;
#endif /* LWIP_TCP && TCP_QUEUE_OOSEQ && LWIP_WND_SCALE */

RET_DATA pbuf_get_at_fp_t               pbuf_get_at_adpt;
RET_DATA pbuf_try_get_at_fp_t           pbuf_try_get_at_adpt;
RET_DATA pbuf_put_at_fp_t               pbuf_put_at_adpt;
RET_DATA pbuf_memcmp_fp_t               pbuf_memcmp_adpt;
RET_DATA pbuf_memfind_fp_t              pbuf_memfind_adpt;
RET_DATA pbuf_strstr_fp_t               pbuf_strstr_adpt;

void lwip_load_interface_pbuf(void)
{
#if LWIP_TCP && TCP_QUEUE_OOSEQ && PBUF_POOL_FREE_OOSEQ
    // private static api
    pbuf_free_ooseq_adpt                              = LWIP_ROMFN(pbuf_free_ooseq);
    pbuf_free_ooseq_callback_adpt                     = LWIP_ROMFN(pbuf_free_ooseq_callback);
    pbuf_pool_is_empty_adpt                           = LWIP_ROMFN(pbuf_pool_is_empty);
#endif
    pbuf_header_impl_adpt                             = LWIP_ROMFN(pbuf_header_impl);
    pbuf_skip_const_adpt                              = LWIP_ROMFN(pbuf_skip_const);

    pbuf_alloc_adpt                                   = LWIP_ROMFN(pbuf_alloc);
#if LWIP_SUPPORT_CUSTOM_PBUF
    pbuf_alloced_custom_adpt                          = LWIP_ROMFN(pbuf_alloced_custom);
#endif
    pbuf_realloc_adpt                                 = LWIP_ROMFN(pbuf_realloc);
    //pbuf_header_adpt                                //= LWIP_ROMFN(pbuf_header);
    pbuf_header_force_adpt                            = LWIP_ROMFN(pbuf_header_force);
    pbuf_ref_adpt                                     = LWIP_ROMFN(pbuf_ref);
    pbuf_free_adpt                                    = LWIP_ROMFN(pbuf_free);
    pbuf_clen_adpt                                    = LWIP_ROMFN(pbuf_clen);
    pbuf_cat_adpt                                     = LWIP_ROMFN(pbuf_cat);
    pbuf_chain_adpt                                   = LWIP_ROMFN(pbuf_chain);
    pbuf_dechain_adpt                                 = LWIP_ROMFN(pbuf_dechain);
    pbuf_copy_adpt                                    = LWIP_ROMFN(pbuf_copy);
    pbuf_copy_partial_adpt                            = LWIP_ROMFN(pbuf_copy_partial);
    pbuf_take_adpt                                    = LWIP_ROMFN(pbuf_take);
    pbuf_take_at_adpt                                 = LWIP_ROMFN(pbuf_take_at);
    pbuf_skip_adpt                                    = LWIP_ROMFN(pbuf_skip);
    pbuf_coalesce_adpt                                = LWIP_ROMFN(pbuf_coalesce);
#if LWIP_CHECKSUM_ON_COPY
    pbuf_fill_chksum_adpt                              = LWIP_ROMFN(pbuf_fill_chksum);
#endif /* LWIP_CHECKSUM_ON_COPY */
#if LWIP_TCP && TCP_QUEUE_OOSEQ && LWIP_WND_SCALE
    pbuf_split_64k_adpt                                = LWIP_ROMFN(pbuf_split_64k);
#endif /* LWIP_TCP && TCP_QUEUE_OOSEQ && LWIP_WND_SCALE */

    pbuf_get_at_adpt                                   = LWIP_ROMFN(pbuf_get_at);
    pbuf_try_get_at_adpt                               = LWIP_ROMFN(pbuf_try_get_at);
    pbuf_put_at_adpt                                   = LWIP_ROMFN(pbuf_put_at);
    pbuf_memcmp_adpt                                   = LWIP_ROMFN(pbuf_memcmp);
    pbuf_memfind_adpt                                  = LWIP_ROMFN(pbuf_memfind);
    pbuf_strstr_adpt                                   = LWIP_ROMFN(pbuf_strstr);
}


#if !LWIP_TCP || !TCP_QUEUE_OOSEQ || !PBUF_POOL_FREE_OOSEQ
//#define PBUF_POOL_IS_EMPTY()
#else /* !LWIP_TCP || !TCP_QUEUE_OOSEQ || !PBUF_POOL_FREE_OOSEQ */

//#define PBUF_POOL_IS_EMPTY() pbuf_pool_is_empty()

/**
 * Attempt to reclaim some memory from queued out-of-sequence TCP segments
 * if we run out of pool pbufs. It's better to give priority to new packets
 * if we're running out.
 *
 * This must be done in the correct thread context therefore this function
 * can only be used with NO_SYS=0 and through tcpip_callback.
 */
void
pbuf_free_ooseq(void)
{
    pbuf_free_ooseq_adpt();
}

#if !NO_SYS
/**
 * Just a callback function for tcpip_callback() that calls pbuf_free_ooseq().
 */
void
pbuf_free_ooseq_callback(void *arg)
{
    pbuf_free_ooseq_callback_adpt(arg);
}
#endif /* !NO_SYS */

/** Queue a call to pbuf_free_ooseq if not already queued. */
void
pbuf_pool_is_empty(void)
{
    pbuf_pool_is_empty_adpt();
}
#endif /* !LWIP_TCP || !TCP_QUEUE_OOSEQ || !PBUF_POOL_FREE_OOSEQ */

/**
 * @ingroup pbuf
 * Allocates a pbuf of the given type (possibly a chain for PBUF_POOL type).
 *
 * The actual memory allocated for the pbuf is determined by the
 * layer at which the pbuf is allocated and the requested size
 * (from the size parameter).
 *
 * @param layer flag to define header size
 * @param length size of the pbuf's payload
 * @param type this parameter decides how and where the pbuf
 * should be allocated as follows:
 *
 * - PBUF_RAM: buffer memory for pbuf is allocated as one large
 *             chunk. This includes protocol headers as well.
 * - PBUF_ROM: no buffer memory is allocated for the pbuf, even for
 *             protocol headers. Additional headers must be prepended
 *             by allocating another pbuf and chain in to the front of
 *             the ROM pbuf. It is assumed that the memory used is really
 *             similar to ROM in that it is immutable and will not be
 *             changed. Memory which is dynamic should generally not
 *             be attached to PBUF_ROM pbufs. Use PBUF_REF instead.
 * - PBUF_REF: no buffer memory is allocated for the pbuf, even for
 *             protocol headers. It is assumed that the pbuf is only
 *             being used in a single thread. If the pbuf gets queued,
 *             then pbuf_take should be called to copy the buffer.
 * - PBUF_POOL: the pbuf is allocated as a pbuf chain, with pbufs from
 *              the pbuf pool that is allocated during pbuf_init().
 *
 * @return the allocated pbuf. If multiple pbufs where allocated, this
 * is the first pbuf of a pbuf chain.
 */
struct pbuf *
pbuf_alloc(pbuf_layer layer, u16_t length, pbuf_type type)
{
    return pbuf_alloc_adpt(layer, length, type);
}

#if LWIP_SUPPORT_CUSTOM_PBUF
/**
 * @ingroup pbuf
 * Initialize a custom pbuf (already allocated).
 *
 * @param l flag to define header size
 * @param length size of the pbuf's payload
 * @param type type of the pbuf (only used to treat the pbuf accordingly, as
 *        this function allocates no memory)
 * @param p pointer to the custom pbuf to initialize (already allocated)
 * @param payload_mem pointer to the buffer that is used for payload and headers,
 *        must be at least big enough to hold 'length' plus the header size,
 *        may be NULL if set later.
 *        ATTENTION: The caller is responsible for correct alignment of this buffer!!
 * @param payload_mem_len the size of the 'payload_mem' buffer, must be at least
 *        big enough to hold 'length' plus the header size
 */
struct pbuf*
pbuf_alloced_custom(pbuf_layer l, u16_t length, pbuf_type type, struct pbuf_custom *p,
                    void *payload_mem, u16_t payload_mem_len)
{
    return pbuf_alloced_custom_adpt(l, length, type, p, payload_mem, payload_mem_len);
}
#endif /* LWIP_SUPPORT_CUSTOM_PBUF */

/**
 * @ingroup pbuf
 * Shrink a pbuf chain to a desired length.
 *
 * @param p pbuf to shrink.
 * @param new_len desired new length of pbuf chain
 *
 * Depending on the desired length, the first few pbufs in a chain might
 * be skipped and left unchanged. The new last pbuf in the chain will be
 * resized, and any remaining pbufs will be freed.
 *
 * @note If the pbuf is ROM/REF, only the ->tot_len and ->len fields are adjusted.
 * @note May not be called on a packet queue.
 *
 * @note Despite its name, pbuf_realloc cannot grow the size of a pbuf (chain).
 */
void
pbuf_realloc(struct pbuf *p, u16_t new_len)
{
    pbuf_realloc_adpt(p, new_len);
}

/**
 * Adjusts the payload pointer to hide or reveal headers in the payload.
 * @see pbuf_header.
 *
 * @param p pbuf to change the header size.
 * @param header_size_increment Number of bytes to increment header size.
 * @param force Allow 'header_size_increment > 0' for PBUF_REF/PBUF_ROM types
 *
 * @return non-zero on failure, zero on success.
 *
 */
u8_t
pbuf_header_impl(struct pbuf *p, s16_t header_size_increment, u8_t force)
{
    return pbuf_header_impl_adpt(p, header_size_increment, force);
}

/**
 * Adjusts the payload pointer to hide or reveal headers in the payload.
 *
 * Adjusts the ->payload pointer so that space for a header
 * (dis)appears in the pbuf payload.
 *
 * The ->payload, ->tot_len and ->len fields are adjusted.
 *
 * @param p pbuf to change the header size.
 * @param header_size_increment Number of bytes to increment header size which
 * increases the size of the pbuf. New space is on the front.
 * (Using a negative value decreases the header size.)
 * If hdr_size_inc is 0, this function does nothing and returns successful.
 *
 * PBUF_ROM and PBUF_REF type buffers cannot have their sizes increased, so
 * the call will fail. A check is made that the increase in header size does
 * not move the payload pointer in front of the start of the buffer.
 * @return non-zero on failure, zero on success.
 *
 */
#if 0
u8_t
pbuf_header(struct pbuf *p, s16_t header_size_increment)
{
   return pbuf_header_impl(p, header_size_increment, 0);
}
#endif
/**
 * Same as pbuf_header but does not check if 'header_size > 0' is allowed.
 * This is used internally only, to allow PBUF_REF for RX.
 */
u8_t
pbuf_header_force(struct pbuf *p, s16_t header_size_increment)
{
   return pbuf_header_impl(p, header_size_increment, 1);
}

/**
 * @ingroup pbuf
 * Dereference a pbuf chain or queue and deallocate any no-longer-used
 * pbufs at the head of this chain or queue.
 *
 * Decrements the pbuf reference count. If it reaches zero, the pbuf is
 * deallocated.
 *
 * For a pbuf chain, this is repeated for each pbuf in the chain,
 * up to the first pbuf which has a non-zero reference count after
 * decrementing. So, when all reference counts are one, the whole
 * chain is free'd.
 *
 * @param p The pbuf (chain) to be dereferenced.
 *
 * @return the number of pbufs that were de-allocated
 * from the head of the chain.
 *
 * @note MUST NOT be called on a packet queue (Not verified to work yet).
 * @note the reference counter of a pbuf equals the number of pointers
 * that refer to the pbuf (or into the pbuf).
 *
 * @internal examples:
 *
 * Assuming existing chains a->b->c with the following reference
 * counts, calling pbuf_free(a) results in:
 *
 * 1->2->3 becomes ...1->3
 * 3->3->3 becomes 2->3->3
 * 1->1->2 becomes ......1
 * 2->1->1 becomes 1->1->1
 * 1->1->1 becomes .......
 *
 */
u8_t
pbuf_free(struct pbuf *p)
{
    return pbuf_free_adpt(p);
}

/**
 * Count number of pbufs in a chain
 *
 * @param p first pbuf of chain
 * @return the number of pbufs in a chain
 */
u16_t
pbuf_clen(const struct pbuf *p)
{
    return pbuf_clen_adpt(p);
}

/**
 * @ingroup pbuf
 * Increment the reference count of the pbuf.
 *
 * @param p pbuf to increase reference counter of
 *
 */
void
pbuf_ref(struct pbuf *p)
{
    pbuf_ref_adpt(p);
}

/**
 * @ingroup pbuf
 * Concatenate two pbufs (each may be a pbuf chain) and take over
 * the caller's reference of the tail pbuf.
 *
 * @note The caller MAY NOT reference the tail pbuf afterwards.
 * Use pbuf_chain() for that purpose.
 *
 * @see pbuf_chain()
 */
void
pbuf_cat(struct pbuf *h, struct pbuf *t)
{
    pbuf_cat_adpt(h, t);
}

/**
 * @ingroup pbuf
 * Chain two pbufs (or pbuf chains) together.
 *
 * The caller MUST call pbuf_free(t) once it has stopped
 * using it. Use pbuf_cat() instead if you no longer use t.
 *
 * @param h head pbuf (chain)
 * @param t tail pbuf (chain)
 * @note The pbufs MUST belong to the same packet.
 * @note MAY NOT be called on a packet queue.
 *
 * The ->tot_len fields of all pbufs of the head chain are adjusted.
 * The ->next field of the last pbuf of the head chain is adjusted.
 * The ->ref field of the first pbuf of the tail chain is adjusted.
 *
 */
void
pbuf_chain(struct pbuf *h, struct pbuf *t)
{
    pbuf_chain_adpt(h, t);
}

/**
 * Dechains the first pbuf from its succeeding pbufs in the chain.
 *
 * Makes p->tot_len field equal to p->len.
 * @param p pbuf to dechain
 * @return remainder of the pbuf chain, or NULL if it was de-allocated.
 * @note May not be called on a packet queue.
 */
struct pbuf *
pbuf_dechain(struct pbuf *p)
{
    return pbuf_dechain_adpt(p);
}

/**
 * @ingroup pbuf
 * Create PBUF_RAM copies of pbufs.
 *
 * Used to queue packets on behalf of the lwIP stack, such as
 * ARP based queueing.
 *
 * @note You MUST explicitly use p = pbuf_take(p);
 *
 * @note Only one packet is copied, no packet queue!
 *
 * @param p_to pbuf destination of the copy
 * @param p_from pbuf source of the copy
 *
 * @return ERR_OK if pbuf was copied
 *         ERR_ARG if one of the pbufs is NULL or p_to is not big
 *                 enough to hold p_from
 */
err_t
pbuf_copy(struct pbuf *p_to, const struct pbuf *p_from)
{
    return pbuf_copy_adpt(p_to, p_from);
}

/**
 * @ingroup pbuf
 * Copy (part of) the contents of a packet buffer
 * to an application supplied buffer.
 *
 * @param buf the pbuf from which to copy data
 * @param dataptr the application supplied buffer
 * @param len length of data to copy (dataptr must be big enough). No more
 * than buf->tot_len will be copied, irrespective of len
 * @param offset offset into the packet buffer from where to begin copying len bytes
 * @return the number of bytes copied, or 0 on failure
 */
u16_t
pbuf_copy_partial(const struct pbuf *buf, void *dataptr, u16_t len, u16_t offset)
{
    return pbuf_copy_partial_adpt(buf, dataptr, len, offset);
}

#if LWIP_TCP && TCP_QUEUE_OOSEQ && LWIP_WND_SCALE
/**
 * This method modifies a 'pbuf chain', so that its total length is
 * smaller than 64K. The remainder of the original pbuf chain is stored
 * in *rest.
 * This function never creates new pbufs, but splits an existing chain
 * in two parts. The tot_len of the modified packet queue will likely be
 * smaller than 64K.
 * 'packet queues' are not supported by this function.
 *
 * @param p the pbuf queue to be split
 * @param rest pointer to store the remainder (after the first 64K)
 */
void pbuf_split_64k(struct pbuf *p, struct pbuf **rest)
{
    pbuf_split_64k_adpt(p, rest);
}
#endif /* LWIP_TCP && TCP_QUEUE_OOSEQ && LWIP_WND_SCALE */

/* Actual implementation of pbuf_skip() but returning const pointer... */
const struct pbuf*
pbuf_skip_const(const struct pbuf* in, u16_t in_offset, u16_t* out_offset)
{
    return pbuf_skip_const_adpt(in, in_offset, out_offset);
}

/**
 * @ingroup pbuf
 * Skip a number of bytes at the start of a pbuf
 *
 * @param in input pbuf
 * @param in_offset offset to skip
 * @param out_offset resulting offset in the returned pbuf
 * @return the pbuf in the queue where the offset is
 */
struct pbuf*
pbuf_skip(struct pbuf* in, u16_t in_offset, u16_t* out_offset)
{
    return pbuf_skip_adpt(in, in_offset, out_offset);
}

/**
 * @ingroup pbuf
 * Copy application supplied data into a pbuf.
 * This function can only be used to copy the equivalent of buf->tot_len data.
 *
 * @param buf pbuf to fill with data
 * @param dataptr application supplied data buffer
 * @param len length of the application supplied data buffer
 *
 * @return ERR_OK if successful, ERR_MEM if the pbuf is not big enough
 */
err_t
pbuf_take(struct pbuf *buf, const void *dataptr, u16_t len)
{
    return pbuf_take_adpt(buf, dataptr, len);
}

/**
 * @ingroup pbuf
 * Same as pbuf_take() but puts data at an offset
 *
 * @param buf pbuf to fill with data
 * @param dataptr application supplied data buffer
 * @param len length of the application supplied data buffer
 * @param offset offset in pbuf where to copy dataptr to
 *
 * @return ERR_OK if successful, ERR_MEM if the pbuf is not big enough
 */
err_t
pbuf_take_at(struct pbuf *buf, const void *dataptr, u16_t len, u16_t offset)
{
    return pbuf_take_at_adpt(buf, dataptr, len, offset);
}

/**
 * @ingroup pbuf
 * Creates a single pbuf out of a queue of pbufs.
 *
 * @remark: Either the source pbuf 'p' is freed by this function or the original
 *          pbuf 'p' is returned, therefore the caller has to check the result!
 *
 * @param p the source pbuf
 * @param layer pbuf_layer of the new pbuf
 *
 * @return a new, single pbuf (p->next is NULL)
 *         or the old pbuf if allocation fails
 */
struct pbuf*
pbuf_coalesce(struct pbuf *p, pbuf_layer layer)
{
    return pbuf_coalesce_adpt(p, layer);
}

#if LWIP_CHECKSUM_ON_COPY
/**
 * Copies data into a single pbuf (*not* into a pbuf queue!) and updates
 * the checksum while copying
 *
 * @param p the pbuf to copy data into
 * @param start_offset offset of p->payload where to copy the data to
 * @param dataptr data to copy into the pbuf
 * @param len length of data to copy into the pbuf
 * @param chksum pointer to the checksum which is updated
 * @return ERR_OK if successful, another error if the data does not fit
 *         within the (first) pbuf (no pbuf queues!)
 */
err_t
pbuf_fill_chksum(struct pbuf *p, u16_t start_offset, const void *dataptr,
                 u16_t len, u16_t *chksum)
{
    return pbuf_fill_chksum_adpt(p, start_offset, dataptr, len, chksum);
}
#endif /* LWIP_CHECKSUM_ON_COPY */

/**
 * @ingroup pbuf
 * Get one byte from the specified position in a pbuf
 * WARNING: returns zero for offset >= p->tot_len
 *
 * @param p pbuf to parse
 * @param offset offset into p of the byte to return
 * @return byte at an offset into p OR ZERO IF 'offset' >= p->tot_len
 */
u8_t
pbuf_get_at(const struct pbuf* p, u16_t offset)
{
    return pbuf_get_at_adpt(p, offset);
}

/**
 * @ingroup pbuf
 * Get one byte from the specified position in a pbuf
 *
 * @param p pbuf to parse
 * @param offset offset into p of the byte to return
 * @return byte at an offset into p [0..0xFF] OR negative if 'offset' >= p->tot_len
 */
int
pbuf_try_get_at(const struct pbuf* p, u16_t offset)
{
    return pbuf_try_get_at_adpt(p, offset);
}

/**
 * @ingroup pbuf
 * Put one byte to the specified position in a pbuf
 * WARNING: silently ignores offset >= p->tot_len
 *
 * @param p pbuf to fill
 * @param offset offset into p of the byte to write
 * @param data byte to write at an offset into p
 */
void
pbuf_put_at(struct pbuf* p, u16_t offset, u8_t data)
{
    pbuf_put_at_adpt(p, offset, data);
}

/**
 * @ingroup pbuf
 * Compare pbuf contents at specified offset with memory s2, both of length n
 *
 * @param p pbuf to compare
 * @param offset offset into p at which to start comparing
 * @param s2 buffer to compare
 * @param n length of buffer to compare
 * @return zero if equal, nonzero otherwise
 *         (0xffff if p is too short, diffoffset+1 otherwise)
 */
u16_t
pbuf_memcmp(const struct pbuf* p, u16_t offset, const void* s2, u16_t n)
{
    return pbuf_memcmp_adpt(p, offset, s2, n);
}

/**
 * @ingroup pbuf
 * Find occurrence of mem (with length mem_len) in pbuf p, starting at offset
 * start_offset.
 *
 * @param p pbuf to search, maximum length is 0xFFFE since 0xFFFF is used as
 *        return value 'not found'
 * @param mem search for the contents of this buffer
 * @param mem_len length of 'mem'
 * @param start_offset offset into p at which to start searching
 * @return 0xFFFF if substr was not found in p or the index where it was found
 */
u16_t
pbuf_memfind(const struct pbuf* p, const void* mem, u16_t mem_len, u16_t start_offset)
{
    return pbuf_memfind_adpt(p, mem, mem_len, start_offset);
}

/**
 * Find occurrence of substr with length substr_len in pbuf p, start at offset
 * start_offset
 * WARNING: in contrast to strstr(), this one does not stop at the first \0 in
 * the pbuf/source string!
 *
 * @param p pbuf to search, maximum length is 0xFFFE since 0xFFFF is used as
 *        return value 'not found'
 * @param substr string to search for in p, maximum length is 0xFFFE
 * @return 0xFFFF if substr was not found in p or the index where it was found
 */
u16_t
pbuf_strstr(const struct pbuf* p, const char* substr)
{
    return pbuf_strstr_adpt(p, substr);
}
