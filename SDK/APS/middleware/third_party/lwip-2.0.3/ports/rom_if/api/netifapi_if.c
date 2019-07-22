/**
 * @file
 * Network Interface Sequential API module
 *
 * @defgroup netifapi NETIF API
 * @ingroup sequential_api
 * Thread-safe functions to be called from non-TCPIP threads
 * 
 * @defgroup netifapi_netif NETIF related
 * @ingroup netifapi
 * To be called from non-TCPIP threads 
 */

/*
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
 */

#include "lwip/opt.h"

#if LWIP_NETIF_API /* don't build if not configured for use in lwipopts.h */

#include "lwip/netifapi.h"
#include "lwip/memp.h"
#include "lwip/priv/tcpip_priv.h"

/* Private API fucntion used in netifapi.c*/
RET_DATA netifapi_do_netif_add_fp_t       netifapi_do_netif_add_adpt;
RET_DATA netifapi_do_netif_set_addr_fp_t  netifapi_do_netif_set_addr_adpt;
RET_DATA netifapi_do_netif_common_fp_t    netifapi_do_netif_common_adpt;

/* Public API fucntion used in netifapi.c*/
RET_DATA netifapi_netif_add_fp_t          netifapi_netif_add_adpt;
#if LWIP_IPV4
RET_DATA netifapi_netif_set_addr_fp_t     netifapi_netif_set_addr_adpt;
#endif
RET_DATA netifapi_netif_common_fp_t       netifapi_netif_common_adpt;


void lwip_load_interface_netifapi(void)
{
    netifapi_do_netif_add_fp_t          = LWIP_ROMFN(netifapi_do_netif_add);
    netifapi_do_netif_set_addr_fp_t     = LWIP_ROMFN(netifapi_do_netif_set_addr);
    netifapi_do_netif_common_fp_t       = LWIP_ROMFN(netifapi_do_netif_common);

    netifapi_netif_add_fp_t             = LWIP_ROMFN(netifapi_netif_add);
#if LWIP_IPV4
    netifapi_netif_set_addr_fp_t        = LWIP_ROMFN(netifapi_netif_set_addr);
#endif
    netifapi_netif_common_fp_t          = LWIP_ROMFN(netifapi_netif_common);          
}


/**
 * Call netif_add() inside the tcpip_thread context.
 */
err_t
netifapi_do_netif_add(struct tcpip_api_call_data *m)
{
    return netifapi_do_netif_add_adpt(m);
}

#if LWIP_IPV4
/**
 * Call netif_set_addr() inside the tcpip_thread context.
 */
err_t
netifapi_do_netif_set_addr(struct tcpip_api_call_data *m)
{
    return netifapi_do_netif_set_addr_adpt(m);
}
#endif /* LWIP_IPV4 */

/**
 * Call the "errtfunc" (or the "voidfunc" if "errtfunc" is NULL) inside the
 * tcpip_thread context.
 */
err_t
netifapi_do_netif_common(struct tcpip_api_call_data *m)
{
    return netifapi_do_netif_set_addr_adpt(m);
}

/**
 * @ingroup netifapi_netif
 * Call netif_add() in a thread-safe way by running that function inside the
 * tcpip_thread context.
 *
 * @note for params @see netif_add()
 */
err_t
netifapi_netif_add(struct netif *netif,
#if LWIP_IPV4
                   const ip4_addr_t *ipaddr, const ip4_addr_t *netmask, const ip4_addr_t *gw,
#endif /* LWIP_IPV4 */
                   void *state, netif_init_fn init, netif_input_fn input)
{
    return netifapi_netif_add_adpt(netif,
    #if LWIP_IPV4
                                   ipaddr, netmask, gw,
    #endif                                    
                                   state, init, input);
}


#if LWIP_IPV4
/**
 * @ingroup netifapi_netif
 * Call netif_set_addr() in a thread-safe way by running that function inside the
 * tcpip_thread context.
 *
 * @note for params @see netif_set_addr()
 */
err_t
netifapi_netif_set_addr(struct netif *netif,
                        const ip4_addr_t *ipaddr,
                        const ip4_addr_t *netmask,
                        const ip4_addr_t *gw)
{
    return netifapi_netif_set_addr_adpt(netif, ipaddr, netmask, gw);
}
#endif /* LWIP_IPV4 */

/**
 * call the "errtfunc" (or the "voidfunc" if "errtfunc" is NULL) in a thread-safe
 * way by running that function inside the tcpip_thread context.
 *
 * @note use only for functions where there is only "netif" parameter.
 */
err_t
netifapi_netif_common(struct netif *netif, netifapi_void_fn voidfunc,
                       netifapi_errt_fn errtfunc)
{
    return netifapi_netif_common_adpt(netif, voidfunc, errtfunc);
}

#endif /* LWIP_NETIF_API */
