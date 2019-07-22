/**
 * @file
 * DNS - host name to IP address resolver.
 *
 * @defgroup dns DNS
 * @ingroup callbackstyle_api
 *
 * Implements a DNS host name to IP address resolver.
 *
 * The lwIP DNS resolver functions are used to lookup a host name and
 * map it to a numerical IP address. It maintains a list of resolved
 * hostnames that can be queried with the dns_lookup() function.
 * New hostnames can be resolved using the dns_query() function.
 *
 * The lwIP version of the resolver also adds a non-blocking version of
 * gethostbyname() that will work with a raw API application. This function
 * checks for an IP address string first and converts it if it is valid.
 * gethostbyname() then does a dns_lookup() to see if the name is
 * already in the table. If so, the IP is returned. If not, a query is
 * issued and the function returns with a ERR_INPROGRESS status. The app
 * using the dns client must then go into a waiting state.
 *
 * Once a hostname has been resolved (or found to be non-existent),
 * the resolver code calls a specified callback function (which
 * must be implemented by the module that uses the resolver).
 *
 * Multicast DNS queries are supported for names ending on ".local".
 * However, only "One-Shot Multicast DNS Queries" are supported (RFC 6762
 * chapter 5.1), this is not a fully compliant implementation of continuous
 * mDNS querying!
 *
 * All functions must be called from TCPIP thread.
 *
 * @see @ref netconn_common for thread-safe access.
 */

/*
 * Port to lwIP from uIP
 * by Jim Pettinato April 2007
 *
 * security fixes and more by Simon Goldschmidt
 *
 * uIP version Copyright (c) 2002-2003, Adam Dunkels.
 * All rights reserved.
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

/*-----------------------------------------------------------------------------
 * RFC 1035 - Domain names - implementation and specification
 * RFC 2181 - Clarifications to the DNS Specification
 *----------------------------------------------------------------------------*/

/** @todo: define good default values (rfc compliance) */
/** @todo: improve answer parsing, more checkings... */
/** @todo: check RFC1035 - 7.3. Processing responses */
/** @todo: one-shot mDNS: dual-stack fallback to another IP version */

/*-----------------------------------------------------------------------------
 * Includes
 *----------------------------------------------------------------------------*/
#include "lwip/opt.h"

#if defined(LWIP_ROMBUILD)


#if LWIP_DNS /* don't build if not configured for use in lwipopts.h */

#include "lwip/def.h"
#include "lwip/udp.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/dns.h"
#include "lwip/prot/dns.h"

#include <string.h>

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
RET_DATA  dns_init_local_fp_t               dns_init_local_adpt;
RET_DATA  dns_lookup_local_fp_t             dns_lookup_local_adpt;
#endif
RET_DATA  dns_correct_response_fp_t         dns_correct_response_adpt;
RET_DATA  dns_recv_fp_t                     dns_recv_adpt;
RET_DATA  dns_check_entry_fp_t              dns_check_entry_adpt;
RET_DATA  dns_check_entries_fp_t            dns_check_entries_adpt;
RET_DATA  dns_call_found_fp_t               dns_call_found_adpt;
RET_DATA  dns_lookup_fp_t                   dns_lookup_adpt;
RET_DATA  dns_compare_name_fp_t             dns_compare_name_adpt;
RET_DATA  dns_skip_name_fp_t                dns_skip_name_adpt;
RET_DATA  dns_send_fp_t                     dns_send_adpt;
RET_DATA  dns_create_txid_fp_t              dns_create_txid_adpt;
RET_DATA  dns_enqueue_fp_t                  dns_enqueue_adpt;
#if ((LWIP_DNS_SECURE & LWIP_DNS_SECURE_RAND_SRC_PORT) != 0)
RET_DATA  dns_alloc_random_port_fp_t        dns_alloc_random_port_adpt;
RET_DATA  dns_alloc_pcb_fp_t                dns_alloc_pcb_adpt;
#endif /* ((LWIP_DNS_SECURE & LWIP_DNS_SECURE_RAND_SRC_PORT) != 0) */

RET_DATA  dns_init_fp_t                     dns_init_adpt;
RET_DATA  dns_tmr_fp_t                      dns_tmr_adpt;
RET_DATA  dns_setserver_fp_t                dns_setserver_adpt;
RET_DATA  dns_getserver_fp_t                dns_getserver_adpt;
RET_DATA  dns_gethostbyname_fp_t            dns_gethostbyname_adpt;
RET_DATA  dns_gethostbyname_addrtype_fp_t   dns_gethostbyname_addrtype_adpt;
#if DNS_LOCAL_HOSTLIST
RET_DATA  dns_local_iterate_fp_t            dns_local_iterate_adpt;
RET_DATA  dns_local_lookup_fp_t             dns_local_lookup_adpt;
#if DNS_LOCAL_HOSTLIST_IS_DYNAMIC
RET_DATA  dns_local_removehost_fp_t         dns_local_removehost_adpt;
RET_DATA  dns_local_addhost_fp_t            dns_local_addhost_adpt;
#endif /* DNS_LOCAL_HOSTLIST_IS_DYNAMIC */
#endif /* DNS_LOCAL_HOSTLIST */


void lwip_load_interface_dns(void)
{
    /* DNS variables */
    memset(&dns_pcbs, 0, sizeof(dns_pcbs));
#if ((LWIP_DNS_SECURE & LWIP_DNS_SECURE_RAND_SRC_PORT) != 0)
    dns_last_pcb_idx = 0;
#endif
    dns_seqno = 0;
    memset(&dns_table, 0, sizeof(struct dns_table_entry)*DNS_TABLE_SIZE);
    memset(&dns_requests, 0, sizeof(struct dns_req_entry)*DNS_MAX_REQUESTS);
    memset(&dns_servers, 0, sizeof(ip_addr_t)* DNS_MAX_SERVERS);

#if DNS_LOCAL_HOSTLIST
    dns_init_local_adpt                 = LWIP_ROMFN(dns_init_local);
    dns_lookup_local_adpt               = LWIP_ROMFN(dns_lookup_local);
#endif
    dns_correct_response_adpt           = LWIP_ROMFN(dns_correct_response);
    dns_recv_adpt                       = LWIP_ROMFN(dns_recv);
    dns_check_entry_adpt                = LWIP_ROMFN(dns_check_entry);
    dns_check_entries_adpt              = LWIP_ROMFN(dns_check_entries);
    dns_call_found_adpt                 = LWIP_ROMFN(dns_call_found);
    dns_lookup_adpt                     = LWIP_ROMFN(dns_lookup);
    dns_compare_name_adpt               = LWIP_ROMFN(dns_compare_name);
    dns_skip_name_adpt                  = LWIP_ROMFN(dns_skip_name);
    dns_send_adpt                       = LWIP_ROMFN(dns_send);
    dns_create_txid_adpt                = LWIP_ROMFN(dns_create_txid);
    dns_enqueue_adpt                    = LWIP_ROMFN(dns_enqueue);
#if ((LWIP_DNS_SECURE & LWIP_DNS_SECURE_RAND_SRC_PORT) != 0)
    dns_alloc_random_port_adpt          = LWIP_ROMFN(dns_alloc_random_port);
    dns_alloc_pcb_adpt                  = LWIP_ROMFN(dns_alloc_pcb);
#endif

    dns_init_adpt                       = LWIP_ROMFN(dns_init);
    dns_tmr_adpt                        = LWIP_ROMFN(dns_tmr);
    dns_setserver_adpt                  = LWIP_ROMFN(dns_setserver);
    dns_getserver_adpt                  = LWIP_ROMFN(dns_getserver);
    dns_gethostbyname_adpt              = LWIP_ROMFN(dns_gethostbyname);
    dns_gethostbyname_addrtype_adpt     = LWIP_ROMFN(dns_gethostbyname_addrtype);
#if DNS_LOCAL_HOSTLIST
    dns_local_iterate_adpt              = LWIP_ROMFN(dns_local_iterate);
    dns_local_lookup_adpt               = LWIP_ROMFN(dns_local_lookup);
#if DNS_LOCAL_HOSTLIST_IS_DYNAMIC
    dns_local_removehost_adpt           = LWIP_ROMFN(dns_local_removehost);
    dns_local_addhost_adpt              = LWIP_ROMFN(dns_local_addhost);
#endif
#endif
}


/**
 * Initialize the resolver: set up the UDP pcb and configure the default server
 * (if DNS_SERVER_ADDRESS is set).
 */
void
dns_init(void)
{
    dns_init_adpt();
}

/**
 * @ingroup dns
 * Initialize one of the DNS servers.
 *
 * @param numdns the index of the DNS server to set must be < DNS_MAX_SERVERS
 * @param dnsserver IP address of the DNS server to set
 */
void
dns_setserver(u8_t numdns, const ip_addr_t *dnsserver)
{
    dns_setserver_adpt(numdns, dnsserver);
}

/**
 * @ingroup dns
 * Obtain one of the currently configured DNS server.
 *
 * @param numdns the index of the DNS server
 * @return IP address of the indexed DNS server or "ip_addr_any" if the DNS
 *         server has not been configured.
 */
const ip_addr_t*
dns_getserver(u8_t numdns)
{
    return dns_getserver_adpt(numdns);
}

/**
 * The DNS resolver client timer - handle retries and timeouts and should
 * be called every DNS_TMR_INTERVAL milliseconds (every second by default).
 */
void
dns_tmr(void)
{
    dns_tmr_adpt();
}

#if DNS_LOCAL_HOSTLIST
void
dns_init_local(void)
{
    dns_init_local_adpt();
}

/**
 * @ingroup dns
 * Iterate the local host-list for a hostname.
 *
 * @param iterator_fn a function that is called for every entry in the local host-list
 * @param iterator_arg 3rd argument passed to iterator_fn
 * @return the number of entries in the local host-list
 */
size_t
dns_local_iterate(dns_found_callback iterator_fn, void *iterator_arg)
{
    return dns_local_iterate_adpt(iterator_fn, iterator_arg);
}

/**
 * @ingroup dns
 * Scans the local host-list for a hostname.
 *
 * @param hostname Hostname to look for in the local host-list
 * @param addr the first IP address for the hostname in the local host-list or
 *         IPADDR_NONE if not found.
 * @param dns_addrtype - LWIP_DNS_ADDRTYPE_IPV4_IPV6: try to resolve IPv4 (ATTENTION: no fallback here!)
 *                     - LWIP_DNS_ADDRTYPE_IPV6_IPV4: try to resolve IPv6 (ATTENTION: no fallback here!)
 *                     - LWIP_DNS_ADDRTYPE_IPV4: try to resolve IPv4 only
 *                     - LWIP_DNS_ADDRTYPE_IPV6: try to resolve IPv6 only
 * @return ERR_OK if found, ERR_ARG if not found
 */
err_t
dns_local_lookup(const char *hostname, ip_addr_t *addr, u8_t dns_addrtype)
{
    return dns_local_lookup_adpt(hostname, addr, dns_addrtype);
}

/* Internal implementation for dns_local_lookup and dns_lookup */
err_t
dns_lookup_local(const char *hostname, ip_addr_t *addr LWIP_DNS_ADDRTYPE_ARG(u8_t dns_addrtype))
{
    return dns_lookup_local_adpt(hostname, addr LWIP_DNS_ADDRTYPE_ARG(dns_addrtype));
}

#if DNS_LOCAL_HOSTLIST_IS_DYNAMIC
/**
 * @ingroup dns
 * Remove all entries from the local host-list for a specific hostname
 * and/or IP address
 *
 * @param hostname hostname for which entries shall be removed from the local
 *                 host-list
 * @param addr address for which entries shall be removed from the local host-list
 * @return the number of removed entries
 */
int
dns_local_removehost(const char *hostname, const ip_addr_t *addr)
{
    return dns_local_removehost_adpt(hostname, addr);
}

/**
 * @ingroup dns
 * Add a hostname/IP address pair to the local host-list.
 * Duplicates are not checked.
 *
 * @param hostname hostname of the new entry
 * @param addr IP address of the new entry
 * @return ERR_OK if succeeded or ERR_MEM on memory error
 */
err_t
dns_local_addhost(const char *hostname, const ip_addr_t *addr)
{
    return dns_local_addhost_adpt(hostname, addr);
}
#endif /* DNS_LOCAL_HOSTLIST_IS_DYNAMIC*/
#endif /* DNS_LOCAL_HOSTLIST */

/**
 * @ingroup dns
 * Look up a hostname in the array of known hostnames.
 *
 * @note This function only looks in the internal array of known
 * hostnames, it does not send out a query for the hostname if none
 * was found. The function dns_enqueue() can be used to send a query
 * for a hostname.
 *
 * @param name the hostname to look up
 * @param addr the hostname's IP address, as u32_t (instead of ip_addr_t to
 *         better check for failure: != IPADDR_NONE) or IPADDR_NONE if the hostname
 *         was not found in the cached dns_table.
 * @return ERR_OK if found, ERR_ARG if not found
 */
err_t
dns_lookup(const char *name, ip_addr_t *addr LWIP_DNS_ADDRTYPE_ARG(u8_t dns_addrtype))
{
    return dns_lookup_adpt(name, addr LWIP_DNS_ADDRTYPE_ARG(dns_addrtype));
}

/**
 * Compare the "dotted" name "query" with the encoded name "response"
 * to make sure an answer from the DNS server matches the current dns_table
 * entry (otherwise, answers might arrive late for hostname not on the list
 * any more).
 *
 * @param query hostname (not encoded) from the dns_table
 * @param p pbuf containing the encoded hostname in the DNS response
 * @param start_offset offset into p where the name starts
 * @return 0xFFFF: names differ, other: names equal -> offset behind name
 */
u16_t
dns_compare_name(const char *query, struct pbuf* p, u16_t start_offset)
{
    return dns_compare_name_adpt(query, p, start_offset);
}

/**
 * Walk through a compact encoded DNS name and return the end of the name.
 *
 * @param p pbuf containing the name
 * @param query_idx start index into p pointing to encoded DNS name in the DNS server response
 * @return index to end of the name
 */
u16_t
dns_skip_name(struct pbuf* p, u16_t query_idx)
{
    return dns_skip_name_adpt(p, query_idx);
}

/**
 * Send a DNS query packet.
 *
 * @param idx the DNS table entry index for which to send a request
 * @return ERR_OK if packet is sent; an err_t indicating the problem otherwise
 */
err_t
dns_send(u8_t idx)
{
    return dns_send_adpt(idx);
}

#if ((LWIP_DNS_SECURE & LWIP_DNS_SECURE_RAND_SRC_PORT) != 0)
struct udp_pcb*
dns_alloc_random_port(void)
{
    return dns_alloc_random_port_adpt();
}

/**
 * dns_alloc_pcb() - allocates a new pcb (or reuses an existing one) to be used
 * for sending a request
 *
 * @return an index into dns_pcbs
 */
u8_t
dns_alloc_pcb(void)
{
    return dns_alloc_pcb_adpt();
}
#endif /* ((LWIP_DNS_SECURE & LWIP_DNS_SECURE_RAND_SRC_PORT) != 0) */

/**
 * dns_call_found() - call the found callback and check if there are duplicate
 * entries for the given hostname. If there are any, their found callback will
 * be called and they will be removed.
 *
 * @param idx dns table index of the entry that is resolved or removed
 * @param addr IP address for the hostname (or NULL on error or memory shortage)
 */
void
dns_call_found(u8_t idx, ip_addr_t* addr)
{
    dns_call_found_adpt(idx, addr);
}

/* Create a query transmission ID that is unique for all outstanding queries */
u16_t
dns_create_txid(void)
{
    return dns_create_txid_adpt();
}

/**
 * dns_check_entry() - see if entry has not yet been queried and, if so, sends out a query.
 * Check an entry in the dns_table:
 * - send out query for new entries
 * - retry old pending entries on timeout (also with different servers)
 * - remove completed entries from the table if their TTL has expired
 *
 * @param i index of the dns_table entry to check
 */
void
dns_check_entry(u8_t i)
{
    dns_check_entry_adpt(i);
}

/**
 * Call dns_check_entry for each entry in dns_table - check all entries.
 */
void
dns_check_entries(void)
{
    dns_check_entries_adpt();
}

/**
 * Save TTL and call dns_call_found for correct response.
 */
void
dns_correct_response(u8_t idx, u32_t ttl)
{
    dns_correct_response_adpt(idx, ttl);
}
/**
 * Receive input function for DNS response packets arriving for the dns UDP pcb.
 */
void
dns_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    dns_recv_adpt(arg, pcb, p, addr, port);
}

/**
 * Queues a new hostname to resolve and sends out a DNS query for that hostname
 *
 * @param name the hostname that is to be queried
 * @param hostnamelen length of the hostname
 * @param found a callback function to be called on success, failure or timeout
 * @param callback_arg argument to pass to the callback function
 * @return err_t return code.
 */
err_t
dns_enqueue(const char *name, size_t hostnamelen, dns_found_callback found,
            void *callback_arg LWIP_DNS_ADDRTYPE_ARG(u8_t dns_addrtype) LWIP_DNS_ISMDNS_ARG(u8_t is_mdns))
{
    return dns_enqueue_adpt(name, hostnamelen, found, callback_arg LWIP_DNS_ADDRTYPE_ARG(dns_addrtype) LWIP_DNS_ISMDNS_ARG(is_mdns));
}

/**
 * @ingroup dns
 * Resolve a hostname (string) into an IP address.
 * NON-BLOCKING callback version for use with raw API!!!
 *
 * Returns immediately with one of err_t return codes:
 * - ERR_OK if hostname is a valid IP address string or the host
 *   name is already in the local names table.
 * - ERR_INPROGRESS enqueue a request to be sent to the DNS server
 *   for resolution if no errors are present.
 * - ERR_ARG: dns client not initialized or invalid hostname
 *
 * @param hostname the hostname that is to be queried
 * @param addr pointer to a ip_addr_t where to store the address if it is already
 *             cached in the dns_table (only valid if ERR_OK is returned!)
 * @param found a callback function to be called on success, failure or timeout (only if
 *              ERR_INPROGRESS is returned!)
 * @param callback_arg argument to pass to the callback function
 * @return a err_t return code.
 */
err_t
dns_gethostbyname(const char *hostname, ip_addr_t *addr, dns_found_callback found,
                  void *callback_arg)
{
    return dns_gethostbyname_adpt(hostname, addr, found, callback_arg);
}

/**
 * @ingroup dns
 * Like dns_gethostbyname, but returned address type can be controlled:
 * @param hostname the hostname that is to be queried
 * @param addr pointer to a ip_addr_t where to store the address if it is already
 *             cached in the dns_table (only valid if ERR_OK is returned!)
 * @param found a callback function to be called on success, failure or timeout (only if
 *              ERR_INPROGRESS is returned!)
 * @param callback_arg argument to pass to the callback function
 * @param dns_addrtype - LWIP_DNS_ADDRTYPE_IPV4_IPV6: try to resolve IPv4 first, try IPv6 if IPv4 fails only
 *                     - LWIP_DNS_ADDRTYPE_IPV6_IPV4: try to resolve IPv6 first, try IPv4 if IPv6 fails only
 *                     - LWIP_DNS_ADDRTYPE_IPV4: try to resolve IPv4 only
 *                     - LWIP_DNS_ADDRTYPE_IPV6: try to resolve IPv6 only
 */
err_t
dns_gethostbyname_addrtype(const char *hostname, ip_addr_t *addr, dns_found_callback found,
                           void *callback_arg, u8_t dns_addrtype)
{
    return dns_gethostbyname_addrtype_adpt(hostname, addr, found, callback_arg, dns_addrtype);
}

#endif /* LWIP_DNS */

#endif //#if defined(LWIP_ROMBUILD)

