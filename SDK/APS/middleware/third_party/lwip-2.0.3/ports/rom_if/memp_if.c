/**
 * @file
 * Dynamic pool memory manager
 *
 * lwIP has dedicated pools for many structures (netconn, protocol control blocks,
 * packet buffers, ...). All these pools are managed here.
 *
 * @defgroup mempool Memory pools
 * @ingroup infrastructure
 * Custom memory pools

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

#include "lwip/memp.h"
#include "lwip/sys.h"
#include "lwip/stats.h"

#include <string.h>

/* Make sure we include everything we need for size calculation required by memp_std.h */
#include "lwip/pbuf.h"
#include "lwip/raw.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include "lwip/priv/tcp_priv.h"
#include "lwip/ip4_frag.h"
#include "lwip/netbuf.h"
#include "lwip/api.h"
#include "lwip/priv/tcpip_priv.h"
#include "lwip/priv/api_msg.h"
#include "lwip/sockets.h"
#include "lwip/netifapi.h"
#include "lwip/etharp.h"
#include "lwip/igmp.h"
#include "lwip/timeouts.h"
/* needed by default MEMP_NUM_SYS_TIMEOUT */
#include "netif/ppp/ppp_opts.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
#include "lwip/priv/nd6_priv.h"
#include "lwip/ip6_frag.h"
#include "lwip/mld6.h"

#ifdef LWIP_HOOK_FILENAME
#include LWIP_HOOK_FILENAME
#endif

/* Private static API fucntion used in memp.c*/
#if MEMP_SANITY_CHECK && !MEMP_MEM_MALLOC
RET_DATA memp_sanity_fp_t                             memp_sanity_adpt;
#endif
#if MEMP_OVERFLOW_CHECK
RET_DATA memp_overflow_check_element_overflow_fp_t    memp_overflow_check_element_overflow_adpt;
RET_DATA memp_overflow_check_element_underflow_fp_t   memp_overflow_check_element_underflow_adpt;
RET_DATA memp_overflow_init_element_fp_t              memp_overflow_init_element_adpt;
#if MEMP_OVERFLOW_CHECK >= 2
RET_DATA memp_overflow_check_all_fp_t                 memp_overflow_check_all_adpt;
#endif /* MEMP_OVERFLOW_CHECK >= 2 */
#endif /* MEMP_OVERFLOW_CHECK */
#if !MEMP_OVERFLOW_CHECK
RET_DATA do_memp_malloc_pool_fp_t                     do_memp_malloc_pool_adpt;
#else
RET_DATA do_memp_malloc_pool_fn_fp_t                  do_memp_malloc_pool_fn_adpt;
#endif
RET_DATA do_memp_free_pool_fp_t                       do_memp_free_pool_adpt;



/* Private API fucntion used in memp.c*/
RET_DATA memp_init_pool_fp_t      memp_init_pool_adpt;
#if MEMP_OVERFLOW_CHECK
RET_DATA memp_malloc_pool_fn_fp_t memp_malloc_pool_fn_adpt;
#else
RET_DATA memp_malloc_pool_fp_t    memp_malloc_pool_adpt;
#endif
RET_DATA memp_free_pool_fp_t      memp_free_pool_adpt;

/* Public API fucntion used in memp.c*/
RET_DATA memp_init_fp_t           memp_init_adpt;
#if MEMP_OVERFLOW_CHECK
RET_DATA memp_malloc_fn_fp_t      memp_malloc_fn_adpt;
#else
RET_DATA memp_malloc_fp_t         memp_malloc_adpt;
#endif
RET_DATA memp_free_fp_t           memp_free_adpt;


void lwip_load_interface_memp(void)
{
    memset(&memp_stats_RAW_PCB         , 0, sizeof(memp_stats_RAW_PCB));
    memset(&memp_stats_UDP_PCB         , 0, sizeof(memp_stats_UDP_PCB));
    memset(&memp_stats_TCP_PCB         , 0, sizeof(memp_stats_TCP_PCB));
    memset(&memp_stats_TCP_PCB_LISTEN  , 0, sizeof(memp_stats_TCP_PCB_LISTEN));
    memset(&memp_stats_TCP_SEG         , 0, sizeof(memp_stats_TCP_SEG));
    memset(&memp_stats_REASSDATA       , 0, sizeof(memp_stats_REASSDATA));
    memset(&memp_stats_NETBUF          , 0, sizeof(memp_stats_NETBUF));
    memset(&memp_stats_NETCONN         , 0, sizeof(memp_stats_NETCONN));
    memset(&memp_stats_TCPIP_MSG_API   , 0, sizeof(memp_stats_TCPIP_MSG_API));
    memset(&memp_stats_TCPIP_MSG_INPKT , 0, sizeof(memp_stats_TCPIP_MSG_INPKT));
    memset(&memp_stats_ARP_QUEUE       , 0, sizeof(memp_stats_ARP_QUEUE));
    memset(&memp_stats_IGMP_GROUP      , 0, sizeof(memp_stats_ARP_QUEUE));
    memset(&memp_stats_SYS_TIMEOUT     , 0, sizeof(memp_stats_SYS_TIMEOUT));
    memset(&memp_stats_NETDB           , 0, sizeof(memp_stats_NETDB));
    memset(&memp_stats_ND6_QUEUE       , 0, sizeof(memp_stats_ND6_QUEUE));
    memset(&memp_stats_IP6_REASSDATA   , 0, sizeof(memp_stats_IP6_REASSDATA));
    memset(&memp_stats_MLD6_GROUP      , 0, sizeof(memp_stats_MLD6_GROUP));
    memset(&memp_stats_PBUF            , 0, sizeof(memp_stats_PBUF));
    memset(&memp_stats_PBUF_POOL       , 0, sizeof(memp_stats_PBUF_POOL));

    memp_tab_RAW_PCB            = NULL;
    memp_tab_UDP_PCB            = NULL;
    memp_tab_TCP_PCB            = NULL;
    memp_tab_TCP_PCB_LISTEN     = NULL;
    memp_tab_TCP_SEG            = NULL;
    memp_tab_REASSDATA          = NULL;
    memp_tab_NETBUF             = NULL;
    memp_tab_NETCONN            = NULL;
    memp_tab_TCPIP_MSG_API      = NULL;
    memp_tab_TCPIP_MSG_INPKT    = NULL;
    memp_tab_ARP_QUEUE          = NULL;
    memp_tab_IGMP_GROUP         = NULL;
    memp_tab_SYS_TIMEOUT        = NULL;
    memp_tab_NETDB              = NULL;
    memp_tab_ND6_QUEUE          = NULL;
    memp_tab_IP6_REASSDATA      = NULL;
    memp_tab_MLD6_GROUP         = NULL;
    memp_tab_PBUF               = NULL;
    memp_tab_PBUF_POOL          = NULL;

    /* Private static API fucntion used in memp.c*/
#if MEMP_SANITY_CHECK && !MEMP_MEM_MALLOC
    memp_sanity_adpt = LWIP_ROMFN(memp_sanity);
#endif
#if MEMP_OVERFLOW_CHECK
    memp_overflow_check_element_overflow_adpt  = LWIP_ROMFN(memp_overflow_check_element_overflow);
    memp_overflow_check_element_underflow_adpt = LWIP_ROMFN(memp_overflow_check_element_underflow);
    memp_overflow_init_element_adpt            = LWIP_ROMFN(memp_overflow_init_element);
#if MEMP_OVERFLOW_CHECK >= 2
    memp_overflow_check_all_adpt    = LWIP_ROMFN(memp_overflow_check_all);
#endif /* MEMP_OVERFLOW_CHECK >= 2 */
#endif /* MEMP_OVERFLOW_CHECK */
#if !MEMP_OVERFLOW_CHECK
    do_memp_malloc_pool_adpt        = LWIP_ROMFN(do_memp_malloc_pool);
#else
    do_memp_malloc_pool_fn_adpt     = LWIP_ROMFN(do_memp_malloc_pool_fn);
#endif
    do_memp_free_pool_adpt          = LWIP_ROMFN(do_memp_free_pool);



    /* Private API fucntion used in memp.c*/
    memp_init_pool_adpt             = LWIP_ROMFN(memp_init_pool);
#if MEMP_OVERFLOW_CHECK
    memp_malloc_pool_fn_adpt        = LWIP_ROMFN(memp_malloc_pool_fn);
#else
    memp_malloc_pool_adpt           = LWIP_ROMFN(memp_malloc_pool);
#endif
    memp_free_pool_adpt             = LWIP_ROMFN(memp_free_pool);

    /* Public API fucntion used in memp.c*/
    memp_init_adpt                  = LWIP_ROMFN(memp_init);
#if MEMP_OVERFLOW_CHECK
    memp_malloc_fn_adpt             = LWIP_ROMFN(memp_malloc_fn);
#else
    memp_malloc_adpt                = LWIP_ROMFN(memp_malloc);
#endif
    memp_free_adpt                  = LWIP_ROMFN(memp_free);
}


#if MEMP_SANITY_CHECK && !MEMP_MEM_MALLOC
/**
 * Check that memp-lists don't form a circle, using "Floyd's cycle-finding algorithm".
 */
int
memp_sanity(const struct memp_desc *desc)
{
    return memp_sanity_adpt(desc);
}
#endif /* MEMP_SANITY_CHECK && !MEMP_MEM_MALLOC */

#if MEMP_OVERFLOW_CHECK
/**
 * Check if a memp element was victim of an overflow
 * (e.g. the restricted area after it has been altered)
 *
 * @param p the memp element to check
 * @param desc the pool p comes from
 */
void
memp_overflow_check_element_overflow(struct memp *p, const struct memp_desc *desc)
{
    memp_overflow_check_element_overflow_adpt(p, desc);
}

/**
 * Check if a memp element was victim of an underflow
 * (e.g. the restricted area before it has been altered)
 *
 * @param p the memp element to check
 * @param desc the pool p comes from
 */
void
memp_overflow_check_element_underflow(struct memp *p, const struct memp_desc *desc)
{
    memp_overflow_check_element_underflow_adpt(p, desc);
}

/**
 * Initialize the restricted area of on memp element.
 */
void
memp_overflow_init_element(struct memp *p, const struct memp_desc *desc)
{
    memp_overflow_init_element_adpt(p, desc);
}

#if MEMP_OVERFLOW_CHECK >= 2
/**
 * Do an overflow check for all elements in every pool.
 *
 * @see memp_overflow_check_element for a description of the check
 */
void
memp_overflow_check_all(void)
{
    memp_overflow_check_all_adpt();
}
#endif /* MEMP_OVERFLOW_CHECK >= 2 */
#endif /* MEMP_OVERFLOW_CHECK */

/**
 * Initialize custom memory pool.
 * Related functions: memp_malloc_pool, memp_free_pool
 *
 * @param desc pool to initialize
 */
void
memp_init_pool(const struct memp_desc *desc)
{
    memp_init_pool_adpt(desc);
}

/**
 * Initializes lwIP built-in pools.
 * Related functions: memp_malloc, memp_free
 *
 * Carves out memp_memory into linked lists for each pool-type.
 */
void
memp_init(void)
{
    memp_init_adpt();
}

void*
#if !MEMP_OVERFLOW_CHECK
do_memp_malloc_pool(const struct memp_desc *desc)
{
    return do_memp_malloc_pool_adpt(desc);
}
#else
do_memp_malloc_pool_fn(const struct memp_desc *desc, const char* file, const int line)
{
    return do_memp_malloc_pool_fn_adpt(desc, file, line);
}
#endif
/**
 * Get an element from a custom pool.
 *
 * @param desc the pool to get an element from
 *
 * @return a pointer to the allocated memory or a NULL pointer on error
 */
void *
#if !MEMP_OVERFLOW_CHECK
memp_malloc_pool(const struct memp_desc *desc)
{
    return memp_malloc_pool_adpt(desc);
}
#else
memp_malloc_pool_fn(const struct memp_desc *desc, const char* file, const int line)
{
    return memp_malloc_pool_fn_adpt(desc, file, line);
}
#endif
/**
 * Get an element from a specific pool.
 *
 * @param type the pool to get an element from
 *
 * @return a pointer to the allocated memory or a NULL pointer on error
 */
void *
#if !MEMP_OVERFLOW_CHECK
memp_malloc(memp_t type)
{
    return memp_malloc_adpt(type);
}
#else
memp_malloc_fn(memp_t type, const char* file, const int line)
{
    return memp_malloc_fn_adpt(type, file, line);
}
#endif

void
do_memp_free_pool(const struct memp_desc* desc, void *mem)
{
    do_memp_free_pool_adpt(desc, mem);
}

/**
 * Put a custom pool element back into its pool.
 *
 * @param desc the pool where to put mem
 * @param mem the memp element to free
 */
void
memp_free_pool(const struct memp_desc* desc, void *mem)
{
    memp_free_pool_adpt(desc, mem);
}

/**
 * Put an element back into its pool.
 *
 * @param type the pool where to put mem
 * @param mem the memp element to free
 */
void
memp_free(memp_t type, void *mem)
{
    memp_free_adpt(type, mem);
}

