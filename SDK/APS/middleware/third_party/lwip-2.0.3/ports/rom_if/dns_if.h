/**
 * @file
 * DNS API
 */

/**
 * lwip DNS resolver header file.

 * Author: Jim Pettinato
 *   April 2007

 * ported from uIP resolv.c Copyright (c) 2002-2003, Adam Dunkels.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef LWIP_HDR_DNS_IF_H
#define LWIP_HDR_DNS_IF_H

#include "lwip/opt.h"

#if defined(LWIP_ROMBUILD)

#if LWIP_DNS

#include "lwip/ip_addr.h"

#ifdef __cplusplus
extern "C" {
#endif


#if LWIP_IPV4 && LWIP_IPV6
#define LWIP_DNS_ADDRTYPE_IS_IPV6(t) (((t) == LWIP_DNS_ADDRTYPE_IPV6_IPV4) || ((t) == LWIP_DNS_ADDRTYPE_IPV6))
#define LWIP_DNS_ADDRTYPE_MATCH_IP(t, ip) (IP_IS_V6_VAL(ip) ? LWIP_DNS_ADDRTYPE_IS_IPV6(t) : (!LWIP_DNS_ADDRTYPE_IS_IPV6(t)))
#define LWIP_DNS_ADDRTYPE_ARG(x) , x
#define LWIP_DNS_ADDRTYPE_ARG_OR_ZERO(x) x
#define LWIP_DNS_SET_ADDRTYPE(x, y) do { x = y; } while(0)
#else
#if LWIP_IPV6
#define LWIP_DNS_ADDRTYPE_IS_IPV6(t) 1
#else
#define LWIP_DNS_ADDRTYPE_IS_IPV6(t) 0
#endif
#define LWIP_DNS_ADDRTYPE_MATCH_IP(t, ip) 1
#define LWIP_DNS_ADDRTYPE_ARG(x)
#define LWIP_DNS_ADDRTYPE_ARG_OR_ZERO(x) 0
#define LWIP_DNS_SET_ADDRTYPE(x, y)
#endif /* LWIP_IPV4 && LWIP_IPV6 */

#if LWIP_DNS_SUPPORT_MDNS_QUERIES
#define LWIP_DNS_ISMDNS_ARG(x) , x
#else
#define LWIP_DNS_ISMDNS_ARG(x)
#endif

#if DNS_LOCAL_HOSTLIST
void dns_init_local(void);
err_t dns_lookup_local(const char *hostname, ip_addr_t *addr LWIP_DNS_ADDRTYPE_ARG(u8_t dns_addrtype));
#endif
void dns_correct_response(u8_t idx, u32_t ttl);
void dns_recv(void *s, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);
void dns_check_entry(u8_t i);
void dns_check_entries(void);
void dns_call_found(u8_t idx, ip_addr_t* addr);
err_t dns_lookup(const char *name, ip_addr_t *addr LWIP_DNS_ADDRTYPE_ARG(u8_t dns_addrtype));
u16_t dns_compare_name(const char *query, struct pbuf* p, u16_t start_offset);
u16_t dns_skip_name(struct pbuf* p, u16_t query_idx);
err_t dns_send(u8_t idx);
u16_t dns_create_txid(void);
err_t dns_enqueue(const char *name, size_t hostnamelen, dns_found_callback found,
                  void *callback_arg LWIP_DNS_ADDRTYPE_ARG(u8_t dns_addrtype) LWIP_DNS_ISMDNS_ARG(u8_t is_mdns));

#if ((LWIP_DNS_SECURE & LWIP_DNS_SECURE_RAND_SRC_PORT) != 0)
struct udp_pcb* dns_alloc_random_port(void);
u8_t dns_alloc_pcb(void);
#endif /* ((LWIP_DNS_SECURE & LWIP_DNS_SECURE_RAND_SRC_PORT) != 0) */

#if DNS_LOCAL_HOSTLIST
typedef void  (*dns_init_local_fp_t)(void);
typedef err_t (*dns_lookup_local_fp_t)(const char *hostname, ip_addr_t *addr LWIP_DNS_ADDRTYPE_ARG(u8_t dns_addrtype));
#endif
typedef void  (*dns_correct_response_fp_t)(u8_t idx, u32_t ttl);
typedef void  (*dns_recv_fp_t)(void *s, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);
typedef void  (*dns_check_entries_fp_t)(void);
typedef void  (*dns_check_entry_fp_t)(u8_t i);
typedef void  (*dns_call_found_fp_t)(u8_t idx, ip_addr_t* addr);
typedef err_t (*dns_lookup_fp_t)(const char *name, ip_addr_t *addr LWIP_DNS_ADDRTYPE_ARG(u8_t dns_addrtype));
typedef u16_t (*dns_compare_name_fp_t)(const char *query, struct pbuf* p, u16_t start_offset);
typedef u16_t (*dns_skip_name_fp_t)(struct pbuf* p, u16_t query_idx);
typedef err_t (*dns_send_fp_t)(u8_t idx);
typedef u16_t (*dns_create_txid_fp_t)(void);
typedef err_t (*dns_enqueue_fp_t)(const char *name, size_t hostnamelen, dns_found_callback found,
                                    void *callback_arg LWIP_DNS_ADDRTYPE_ARG(u8_t dns_addrtype) LWIP_DNS_ISMDNS_ARG(u8_t is_mdns));


#if ((LWIP_DNS_SECURE & LWIP_DNS_SECURE_RAND_SRC_PORT) != 0)
typedef struct udp_pcb* (*dns_alloc_random_port_fp_t)(void);
typedef u8_t (*dns_alloc_pcb_fp_t)(void);
#endif /* ((LWIP_DNS_SECURE & LWIP_DNS_SECURE_RAND_SRC_PORT) != 0) */

#if DNS_LOCAL_HOSTLIST
extern dns_init_local_fp_t          dns_init_local_adpt;
extern dns_lookup_local_fp_t        dns_lookup_local_adpt;
#endif
extern dns_correct_response_fp_t    dns_correct_response_adpt;
extern dns_recv_fp_t                dns_recv_adpt;
extern dns_check_entry_fp_t         dns_check_entry_adpt;
extern dns_check_entries_fp_t       dns_check_entries_adpt;
extern dns_call_found_fp_t          dns_call_found_adpt;
extern dns_lookup_fp_t              dns_lookup_adpt;
extern dns_compare_name_fp_t        dns_compare_name_adpt;
extern dns_skip_name_fp_t           dns_skip_name_adpt;
extern dns_send_fp_t                dns_send_adpt;
extern dns_create_txid_fp_t         dns_create_txid_adpt;
extern dns_enqueue_fp_t             dns_enqueue_adpt;

#if ((LWIP_DNS_SECURE & LWIP_DNS_SECURE_RAND_SRC_PORT) != 0)
extern dns_alloc_random_port_fp_t   dns_alloc_random_port_adpt;
extern dns_alloc_pcb_fp_t           dns_alloc_pcb_adpt;
#endif /* ((LWIP_DNS_SECURE & LWIP_DNS_SECURE_RAND_SRC_PORT) != 0) */


/** Callback which is invoked when a hostname is found.
 * A function of this type must be implemented by the application using the DNS resolver.
 * @param name pointer to the name that was looked up.
 * @param ipaddr pointer to an ip_addr_t containing the IP address of the hostname,
 *        or NULL if the name could not be found (or on any other error).
 * @param callback_arg a user-specified callback argument passed to dns_gethostbyname
*/
typedef void (*dns_found_callback)(const char *name, const ip_addr_t *ipaddr, void *callback_arg);


/*Public API fucntion in dns.c*/
typedef void   (*dns_init_fp_t)(void);
typedef void   (*dns_tmr_fp_t)(void);
typedef void   (*dns_setserver_fp_t)(u8_t numdns, const ip_addr_t *dnsserver);
typedef const ip_addr_t* (*dns_getserver_fp_t)(u8_t numdns);
typedef err_t  (*dns_gethostbyname_fp_t)(const char *hostname, ip_addr_t *addr, dns_found_callback found, void *callback_arg);
typedef err_t  (*dns_gethostbyname_addrtype_fp_t)(const char *hostname, ip_addr_t *addr, dns_found_callback found, void *callback_arg, u8_t dns_addrtype);

#if DNS_LOCAL_HOSTLIST
typedef size_t (*dns_local_iterate_fp_t)(dns_found_callback iterator_fn, void *iterator_arg);
typedef err_t  (*dns_local_lookup_fp_t)(const char *hostname, ip_addr_t *addr, u8_t dns_addrtype);
#if DNS_LOCAL_HOSTLIST_IS_DYNAMIC
typedef int    (*dns_local_removehost_fp_t)(const char *hostname, const ip_addr_t *addr);
typedef err_t  (*dns_local_addhost_fp_t)(const char *hostname, const ip_addr_t *addr);
#endif /* DNS_LOCAL_HOSTLIST_IS_DYNAMIC */
#endif /* DNS_LOCAL_HOSTLIST */

extern dns_init_fp_t                        dns_init_adpt;
extern dns_tmr_fp_t                         dns_tmr_adpt;
extern dns_setserver_fp_t                   dns_setserver_adpt;
extern dns_getserver_fp_t                   dns_getserver_adpt;
extern dns_gethostbyname_fp_t               dns_gethostbyname_adpt;
extern dns_gethostbyname_addrtype_fp_t      dns_gethostbyname_addrtype_adpt;
#if DNS_LOCAL_HOSTLIST
extern dns_local_iterate_fp_t               dns_local_iterate_adpt;
extern dns_local_lookup_fp_t                dns_local_lookup_adpt;
#if DNS_LOCAL_HOSTLIST_IS_DYNAMIC
extern dns_local_removehost_fp_t            dns_local_removehost_adpt;
extern dns_local_addhost_fp_t               dns_local_addhost_adpt;
#endif /* DNS_LOCAL_HOSTLIST_IS_DYNAMIC */
#endif /* DNS_LOCAL_HOSTLIST */


#ifdef __cplusplus
}
#endif

#endif /* LWIP_DNS */

#endif //#if defined(LWIP_ROMBUILD)

#endif /* LWIP_HDR_DNS_IF_H */
