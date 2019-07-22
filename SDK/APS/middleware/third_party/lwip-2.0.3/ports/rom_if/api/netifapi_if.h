/**
 * @file
 * netif API (to be used from non-TCPIP threads)
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
#ifndef LWIP_HDR_NETIFAPI_IF_H
#define LWIP_HDR_NETIFAPI_IF_H

#include "lwip/opt.h"

#if defined(LWIP_ROMBUILD)

#if LWIP_NETIF_API /* don't build if not configured for use in lwipopts.h */

#include "lwip/sys.h"
#include "lwip/netif.h"
#include "lwip/dhcp.h"
#include "lwip/autoip.h"
#include "lwip/priv/tcpip_priv.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*netifapi_void_fn)(struct netif *netif);
typedef err_t (*netifapi_errt_fn)(struct netif *netif);


/* Private static API fucntion used in api_lib.c*/
err_t netifapi_do_netif_add(struct tcpip_api_call_data *m);
err_t netifapi_do_netif_set_addr(struct tcpip_api_call_data *m);
err_t netifapi_do_netif_common(struct tcpip_api_call_data *m);

typedef err_t (*netifapi_do_netif_add_fp_t)(struct tcpip_api_call_data *m);
typedef err_t (*netifapi_do_netif_set_addr_fp_t)(struct tcpip_api_call_data *m);
typedef err_t (*netifapi_do_netif_common_fp_t)(struct tcpip_api_call_data *m);

extern netifapi_do_netif_add_fp_t       netifapi_do_netif_add_adpt;
extern netifapi_do_netif_set_addr_fp_t  netifapi_do_netif_set_addr_adpt;
extern netifapi_do_netif_common_fp_t    netifapi_do_netif_common_adpt;


/* API for application */
typedef err_t (*netifapi_netif_add_fp_t)(struct netif *netif,
#if LWIP_IPV4
                         const ip4_addr_t *ipaddr, const ip4_addr_t *netmask, const ip4_addr_t *gw,
#endif /* LWIP_IPV4 */
                         void *state, netif_init_fn init, netif_input_fn input);

#if LWIP_IPV4
typedef err_t (*netifapi_netif_set_addr_fp_t)(struct netif *netif, const ip4_addr_t *ipaddr,
                              const ip4_addr_t *netmask, const ip4_addr_t *gw);
#endif /* LWIP_IPV4*/

typedef err_t (*netifapi_netif_common_fp_t)(struct netif *netif, netifapi_void_fn voidfunc, netifapi_errt_fn errtfunc);


extern netifapi_netif_add_fp_t          netifapi_netif_add_adpt;
#if LWIP_IPV4
extern netifapi_netif_set_addr_fp_t     netifapi_netif_set_addr_adpt;
#endif
extern netifapi_netif_common_fp_t       netifapi_netif_common_adpt;


#ifdef __cplusplus
}
#endif

#endif /* LWIP_NETIF_API */

#endif /* #if defined(LWIP_ROMBUILD) */

#endif /* LWIP_HDR_NETIFAPI_IF_H */
