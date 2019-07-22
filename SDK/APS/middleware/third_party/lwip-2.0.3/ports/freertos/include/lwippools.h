/* OPTIONAL: Pools to replace heap allocation
 * Optional: Pools can be used instead of the heap for mem_malloc. If
 * so, these should be defined here, in increasing order according to
 * the pool element size.
 *
 * LWIP_MALLOC_MEMPOOL(number_elements, element_size)
 */

/* @file lwippools.h
 * Define three pools with sizes 256, 512, and 1512 bytes */
LWIP_MALLOC_MEMPOOL_START
LWIP_MALLOC_MEMPOOL(5, 256)
LWIP_MALLOC_MEMPOOL(5, 512)
LWIP_MALLOC_MEMPOOL(5, 2000)
LWIP_MALLOC_MEMPOOL_END
