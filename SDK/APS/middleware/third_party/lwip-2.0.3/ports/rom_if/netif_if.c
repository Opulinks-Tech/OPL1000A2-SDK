/**
 * @file
 * lwIP network interface abstraction
 *
 * @defgroup netif Network interface (NETIF)
 * @ingroup callbackstyle_api
 *
 * @defgroup netif_ip4 IPv4 address handling
 * @ingroup netif
 *
 * @defgroup netif_ip6 IPv6 address handling
 * @ingroup netif
 *
 * @defgroup netif_cd Client data handling
 * Store data (void*) on a netif for application usage.
 * @see @ref LWIP_NUM_NETIF_CLIENT_DATA
 * @ingroup netif
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
 */

#include "lwip/opt.h"

#include <string.h>

#include "lwip/def.h"
#include "lwip/ip_addr.h"
#include "lwip/ip6_addr.h"
#include "lwip/netif.h"
#include "lwip/priv/tcp_priv.h"
#include "lwip/udp.h"
#include "lwip/raw.h"
#include "lwip/snmp.h"
#include "lwip/igmp.h"
#include "lwip/etharp.h"
#include "lwip/stats.h"
#include "lwip/sys.h"
#include "lwip/ip.h"
#if ENABLE_LOOPBACK
#if LWIP_NETIF_LOOPBACK_MULTITHREADING
#include "lwip/tcpip.h"
#endif /* LWIP_NETIF_LOOPBACK_MULTITHREADING */
#endif /* ENABLE_LOOPBACK */

#include "netif/ethernet.h"

#if LWIP_AUTOIP
#include "lwip/autoip.h"
#endif /* LWIP_AUTOIP */
#if LWIP_DHCP
#include "lwip/dhcp.h"
#endif /* LWIP_DHCP */
#if LWIP_IPV6_DHCP6
#include "lwip/dhcp6.h"
#endif /* LWIP_IPV6_DHCP6 */
#if LWIP_IPV6_MLD
#include "lwip/mld6.h"
#endif /* LWIP_IPV6_MLD */
#if LWIP_IPV6
#include "lwip/nd6.h"
#endif



RET_DATA netif_issue_reports_fp_t                   netif_issue_reports_adpt;
#if LWIP_IPV6
RET_DATA netif_null_output_ip6_fp_t                 netif_null_output_ip6_adpt;
#endif
#if LWIP_HAVE_LOOPIF
#if LWIP_IPV4
RET_DATA netif_loop_output_ipv4_fp_t                netif_loop_output_ipv4_adpt;
#endif
#if LWIP_IPV6
RET_DATA netif_loop_output_ipv6_fp_t                netif_loop_output_ipv6_adpt;
#endif
RET_DATA netif_loopif_init_fp_t                     netif_loopif_init_adpt;
#endif

#if LWIP_DHCP || LWIP_AUTOIP || LWIP_IGMP || LWIP_IPV6_MLD || (LWIP_NUM_NETIF_CLIENT_DATA > 0)
RET_DATA netif_alloc_client_data_id_fp_t            netif_alloc_client_data_id_adpt;
#endif
RET_DATA netif_init_fp_t                            netif_init_adpt;
RET_DATA netif_add_fp_t                             netif_add_adpt;
#if LWIP_IPV4
RET_DATA netif_set_addr_fp_t                        netif_set_addr_adpt;
#endif
RET_DATA netif_remove_fp_t                          netif_remove_adpt;
RET_DATA netif_find_fp_t                            netif_find_adpt;
RET_DATA netif_set_default_fp_t                     netif_set_default_adpt;
#if LWIP_IPV4
RET_DATA netif_set_ipaddr_fp_t                      netif_set_ipaddr_adpt;
RET_DATA netif_set_netmask_fp_t                     netif_set_netmask_adpt;
RET_DATA netif_set_gw_fp_t                          netif_set_gw_adpt;
#endif
RET_DATA netif_set_up_fp_t                          netif_set_up_adpt;
RET_DATA netif_set_down_fp_t                        netif_set_down_adpt;
#if LWIP_NETIF_STATUS_CALLBACK
RET_DATA netif_set_status_callback_fp_t             netif_set_status_callback_adpt;
#endif
#if LWIP_NETIF_IP_CHANGE_CALLBACK
RET_DATA netif_set_ipchange_callback_fp_t           netif_set_ipchange_callback_adpt;
#endif

#if LWIP_NETIF_REMOVE_CALLBACK
RET_DATA netif_set_remove_callback_fp_t             netif_set_remove_callback_adpt;
#endif
RET_DATA netif_set_link_up_fp_t                     netif_set_link_up_adpt;
RET_DATA netif_set_link_down_fp_t                   netif_set_link_down_adpt;
#if LWIP_NETIF_LINK_CALLBACK
RET_DATA netif_set_link_callback_fp_t               netif_set_link_callback_adpt;
#endif

#if ENABLE_LOOPBACK
RET_DATA netif_loop_output_fp_t                     netif_loop_output_adpt;
RET_DATA netif_poll_fp_t                            netif_poll_adpt;
#if !LWIP_NETIF_LOOPBACK_MULTITHREADING
RET_DATA netif_poll_all_fp_t                        netif_poll_all_adpt;
#endif /* !LWIP_NETIF_LOOPBACK_MULTITHREADING */
#endif /* ENABLE_LOOPBACK */
RET_DATA netif_input_fp_t                           netif_input_adpt;

#if LWIP_IPV6
RET_DATA netif_ip6_addr_set_fp_t                    netif_ip6_addr_set_adpt;
RET_DATA netif_ip6_addr_set_parts_fp_t              netif_ip6_addr_set_parts_adpt;
RET_DATA netif_ip6_addr_set_state_fp_t              netif_ip6_addr_set_state_adpt;
RET_DATA netif_get_ip6_addr_match_fp_t              netif_get_ip6_addr_match_adpt;
RET_DATA netif_create_ip6_linklocal_address_fp_t    netif_create_ip6_linklocal_address_adpt;
RET_DATA netif_add_ip6_address_fp_t                 netif_add_ip6_address_adpt;
#endif

void lwip_load_interface_netif(void)
{
    /* Cold boot initialization for "zero_init" retention data */
    netif_list = NULL;
    netif_default = NULL;
    netif_num = 0;
#if LWIP_NUM_NETIF_CLIENT_DATA > 0
    netif_client_id = 0;
#endif
    memset(&loop_netif, 0, sizeof(struct netif));


    netif_issue_reports_adpt                    = LWIP_ROMFN(netif_issue_reports);
#if LWIP_IPV6
    netif_null_output_ip6_adpt                  = LWIP_ROMFN(netif_null_output_ip6);
#endif
#if LWIP_HAVE_LOOPIF
#if LWIP_IPV4
    netif_loop_output_ipv4_adpt                 = LWIP_ROMFN(netif_loop_output_ipv4);
#endif
#if LWIP_IPV6
    netif_loop_output_ipv6_adpt                 = LWIP_ROMFN(netif_loop_output_ipv6);
#endif
    netif_loopif_init_adpt                      = LWIP_ROMFN(netif_loopif_init);
#endif

#if LWIP_DHCP || LWIP_AUTOIP || LWIP_IGMP || LWIP_IPV6_MLD || (LWIP_NUM_NETIF_CLIENT_DATA > 0)
    netif_alloc_client_data_id_adpt             = LWIP_ROMFN(netif_alloc_client_data_id);
#endif
    netif_init_adpt                             = LWIP_ROMFN(netif_init);
    netif_add_adpt                              = LWIP_ROMFN(netif_add);
#if LWIP_IPV4
    netif_set_addr_adpt                         = LWIP_ROMFN(netif_set_addr);
#endif
    netif_remove_adpt                           = LWIP_ROMFN(netif_remove);
    netif_find_adpt                             = LWIP_ROMFN(netif_find);
    netif_set_default_adpt                      = LWIP_ROMFN(netif_set_default);
#if LWIP_IPV4
    netif_set_ipaddr_adpt                       = LWIP_ROMFN(netif_set_ipaddr);
    netif_set_netmask_adpt                      = LWIP_ROMFN(netif_set_netmask);
    netif_set_gw_adpt                           = LWIP_ROMFN(netif_set_gw);
#endif
    netif_set_up_adpt                           = LWIP_ROMFN(netif_set_up);
    netif_set_down_adpt                         = LWIP_ROMFN(netif_set_down);
#if LWIP_NETIF_STATUS_CALLBACK
    netif_set_status_callback_adpt              = LWIP_ROMFN(netif_set_status_callback);
#endif
#if LWIP_NETIF_IP_CHANGE_CALLBACK
    netif_set_ipchange_callback_adpt            = LWIP_ROMFN(netif_set_ipchange_callback);
#endif
#if LWIP_NETIF_REMOVE_CALLBACK
    netif_set_remove_callback_adpt              = LWIP_ROMFN(netif_set_remove_callback);
#endif
    netif_set_link_up_adpt                      = LWIP_ROMFN(netif_set_link_up);
    netif_set_link_down_adpt                    = LWIP_ROMFN(netif_set_link_down);
#if LWIP_NETIF_LINK_CALLBACK
    netif_set_link_callback_adpt                = LWIP_ROMFN(netif_set_link_callback);
#endif

#if ENABLE_LOOPBACK
    netif_loop_output_adpt                      = LWIP_ROMFN(netif_loop_output);
    netif_poll_adpt                             = LWIP_ROMFN(netif_poll);
#if !LWIP_NETIF_LOOPBACK_MULTITHREADING
    netif_poll_all_adpt                         = LWIP_ROMFN(netif_poll_all);
#endif /* !LWIP_NETIF_LOOPBACK_MULTITHREADING */
#endif /* ENABLE_LOOPBACK */
    netif_input_adpt                            = LWIP_ROMFN(netif_input);

#if LWIP_IPV6
    netif_ip6_addr_set_adpt                     = LWIP_ROMFN(netif_ip6_addr_set);
    netif_ip6_addr_set_parts_adpt               = LWIP_ROMFN(netif_ip6_addr_set_parts);
    netif_ip6_addr_set_state_adpt               = LWIP_ROMFN(netif_ip6_addr_set_state);
    netif_get_ip6_addr_match_adpt               = LWIP_ROMFN(netif_get_ip6_addr_match);
    netif_create_ip6_linklocal_address_adpt     = LWIP_ROMFN(netif_create_ip6_linklocal_address);
    netif_add_ip6_address_adpt                  = LWIP_ROMFN(netif_add_ip6_address);
#endif
}


#if LWIP_HAVE_LOOPIF
/**
 * Initialize a lwip network interface structure for a loopback interface
 *
 * @param netif the lwip network interface structure for this loopif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 */
static err_t
netif_loopif_init(struct netif *netif)
{
    return netif_loopif_init_adpt(netif);
}
#endif /* LWIP_HAVE_LOOPIF */

void
netif_init(void)
{
    netif_init_adpt();
}

/**
 * @ingroup lwip_nosys
 * Forwards a received packet for input processing with
 * ethernet_input() or ip_input() depending on netif flags.
 * Don't call directly, pass to netif_add() and call
 * netif->input().
 * Only works if the netif driver correctly sets
 * NETIF_FLAG_ETHARP and/or NETIF_FLAG_ETHERNET flag!
 */
err_t
netif_input(struct pbuf *p, struct netif *inp)
{
    return netif_input_adpt(p, inp);
}

/**
 * @ingroup netif
 * Add a network interface to the list of lwIP netifs.
 *
 * @param netif a pre-allocated netif structure
 * @param ipaddr IP address for the new netif
 * @param netmask network mask for the new netif
 * @param gw default gateway IP address for the new netif
 * @param state opaque data passed to the new netif
 * @param init callback function that initializes the interface
 * @param input callback function that is called to pass
 * ingress packets up in the protocol layer stack.\n
 * It is recommended to use a function that passes the input directly
 * to the stack (netif_input(), NO_SYS=1 mode) or via sending a
 * message to TCPIP thread (tcpip_input(), NO_SYS=0 mode).\n
 * These functions use netif flags NETIF_FLAG_ETHARP and NETIF_FLAG_ETHERNET
 * to decide whether to forward to ethernet_input() or ip_input().
 * In other words, the functions only work when the netif
 * driver is implemented correctly!\n
 * Most members of struct netif should be be initialized by the
 * netif init function = netif driver (init parameter of this function).\n
 * IPv6: Don't forget to call netif_create_ip6_linklocal_address() after
 * setting the MAC address in struct netif.hwaddr
 * (IPv6 requires a link-local address).
 *
 * @return netif, or NULL if failed.
 */
struct netif *
netif_add(struct netif *netif,
#if LWIP_IPV4
          const ip4_addr_t *ipaddr, const ip4_addr_t *netmask, const ip4_addr_t *gw,
#endif /* LWIP_IPV4 */
          void *state, netif_init_fn init, netif_input_fn input)
{
    return netif_add_adpt(netif,
#if LWIP_IPV4
                         ipaddr, netmask, gw,
#endif
                         state, init, input);
}

#if LWIP_IPV4
/**
 * @ingroup netif_ip4
 * Change IP address configuration for a network interface (including netmask
 * and default gateway).
 *
 * @param netif the network interface to change
 * @param ipaddr the new IP address
 * @param netmask the new netmask
 * @param gw the new default gateway
 */
void
netif_set_addr(struct netif *netif, const ip4_addr_t *ipaddr, const ip4_addr_t *netmask,
    const ip4_addr_t *gw)
{
    netif_set_addr_adpt(netif, ipaddr, netmask, gw);

}
#endif /* LWIP_IPV4*/

/**
 * @ingroup netif
 * Remove a network interface from the list of lwIP netifs.
 *
 * @param netif the network interface to remove
 */
void
netif_remove(struct netif *netif)
{
    netif_remove_adpt(netif);
}

/**
 * @ingroup netif
 * Find a network interface by searching for its name
 *
 * @param name the name of the netif (like netif->name) plus concatenated number
 * in ascii representation (e.g. 'en0')
 */
struct netif *
netif_find(const char *name)
{
    return netif_find_adpt(name);
}

#if LWIP_IPV4
/**
 * @ingroup netif_ip4
 * Change the IP address of a network interface
 *
 * @param netif the network interface to change
 * @param ipaddr the new IP address
 *
 * @note call netif_set_addr() if you also want to change netmask and
 * default gateway
 */
void
netif_set_ipaddr(struct netif *netif, const ip4_addr_t *ipaddr)
{
    netif_set_ipaddr_adpt(netif, ipaddr);
}

/**
 * @ingroup netif_ip4
 * Change the default gateway for a network interface
 *
 * @param netif the network interface to change
 * @param gw the new default gateway
 *
 * @note call netif_set_addr() if you also want to change ip address and netmask
 */
void
netif_set_gw(struct netif *netif, const ip4_addr_t *gw)
{
    netif_set_gw_adpt(netif, gw);
}

/**
 * @ingroup netif_ip4
 * Change the netmask of a network interface
 *
 * @param netif the network interface to change
 * @param netmask the new netmask
 *
 * @note call netif_set_addr() if you also want to change ip address and
 * default gateway
 */
void
netif_set_netmask(struct netif *netif, const ip4_addr_t *netmask)
{
    netif_set_netmask_adpt(netif, netmask);
}
#endif /* LWIP_IPV4 */

/**
 * @ingroup netif
 * Set a network interface as the default network interface
 * (used to output all packets for which no specific route is found)
 *
 * @param netif the default network interface
 */
void
netif_set_default(struct netif *netif)
{
    netif_set_default_adpt(netif);
}

/**
 * @ingroup netif
 * Bring an interface up, available for processing
 * traffic.
 */
void
netif_set_up(struct netif *netif)
{
    netif_set_up_adpt(netif);
}

/** Send ARP/IGMP/MLD/RS events, e.g. on link-up/netif-up or addr-change
 */
void
netif_issue_reports(struct netif* netif, u8_t report_type)
{
    netif_issue_reports_adpt(netif, report_type);
}

/**
 * @ingroup netif
 * Bring an interface down, disabling any traffic processing.
 */
void
netif_set_down(struct netif *netif)
{
    netif_set_down_adpt(netif);
}

#if LWIP_NETIF_STATUS_CALLBACK
/**
 * @ingroup netif
 * Set callback to be called when interface is brought up/down or address is changed while up
 */
void
netif_set_status_callback(struct netif *netif, netif_status_callback_fn status_callback)
{
    netif_set_status_callback_adpt(netif, status_callback);
}
#endif /* LWIP_NETIF_STATUS_CALLBACK */

#if LWIP_NETIF_IP_CHANGE_CALLBACK
/**
 * @ingroup netif
 * Set callback to be called when interface changes address
 */
void netif_set_ipchange_callback(struct netif *netif, netif_ipchange_callback_fn ipchange_callback)
{
    netif_set_ipchange_callback_adpt(netif, ipchange_callback);
}
#endif /* LWIP_NETIF_IP_CHANGE_CALLBACK */

#if LWIP_NETIF_REMOVE_CALLBACK
/**
 * @ingroup netif
 * Set callback to be called when the interface has been removed
 */
void
netif_set_remove_callback(struct netif *netif, netif_status_callback_fn remove_callback)
{
    netif_set_remove_callback_adpt(netif, remove_callback);
}
#endif /* LWIP_NETIF_REMOVE_CALLBACK */

/**
 * @ingroup netif
 * Called by a driver when its link goes up
 */
void
netif_set_link_up(struct netif *netif)
{
    netif_set_link_up_adpt(netif);
}

/**
 * @ingroup netif
 * Called by a driver when its link goes down
 */
void
netif_set_link_down(struct netif *netif )
{
    netif_set_link_down_adpt(netif);
}

#if LWIP_NETIF_LINK_CALLBACK
/**
 * @ingroup netif
 * Set callback to be called when link is brought up/down
 */
void
netif_set_link_callback(struct netif *netif, netif_status_callback_fn link_callback)
{
    netif_set_link_callback_adpt(netif, link_callback);
}
#endif /* LWIP_NETIF_LINK_CALLBACK */

#if ENABLE_LOOPBACK
/**
 * @ingroup netif
 * Send an IP packet to be received on the same netif (loopif-like).
 * The pbuf is simply copied and handed back to netif->input.
 * In multithreaded mode, this is done directly since netif->input must put
 * the packet on a queue.
 * In callback mode, the packet is put on an internal queue and is fed to
 * netif->input by netif_poll().
 *
 * @param netif the lwip network interface structure
 * @param p the (IP) packet to 'send'
 * @return ERR_OK if the packet has been sent
 *         ERR_MEM if the pbuf used to copy the packet couldn't be allocated
 */
err_t
netif_loop_output(struct netif *netif, struct pbuf *p)
{
    return netif_loop_output_adpt(netif, p);
}

#if LWIP_HAVE_LOOPIF
#if LWIP_IPV4
err_t
netif_loop_output_ipv4(struct netif *netif, struct pbuf *p, const ip4_addr_t* addr)
{
    return netif_loop_output_ipv4_adpt(netif, p, addr);
}
#endif /* LWIP_IPV4 */

#if LWIP_IPV6
err_t
netif_loop_output_ipv6(struct netif *netif, struct pbuf *p, const ip6_addr_t* addr)
{
    return netif_loop_output_ipv6_adpt(netif, p, addr);
}
#endif /* LWIP_IPV6 */
#endif /* LWIP_HAVE_LOOPIF */


/**
 * Call netif_poll() in the main loop of your application. This is to prevent
 * reentering non-reentrant functions like tcp_input(). Packets passed to
 * netif_loop_output() are put on a list that is passed to netif->input() by
 * netif_poll().
 */
void
netif_poll(struct netif *netif)
{
    netif_poll_adpt(netif);
}

#if !LWIP_NETIF_LOOPBACK_MULTITHREADING
/**
 * Calls netif_poll() for every netif on the netif_list.
 */
void
netif_poll_all(void)
{
    netif_poll_all_adpt();
}
#endif /* !LWIP_NETIF_LOOPBACK_MULTITHREADING */
#endif /* ENABLE_LOOPBACK */

#if LWIP_NUM_NETIF_CLIENT_DATA > 0
/**
 * @ingroup netif_cd
 * Allocate an index to store data in client_data member of struct netif.
 * Returned value is an index in mentioned array.
 * @see LWIP_NUM_NETIF_CLIENT_DATA
 */
u8_t
netif_alloc_client_data_id(void)
{
    return netif_alloc_client_data_id_adpt();
}
#endif

#if LWIP_IPV6
/**
 * @ingroup netif_ip6
 * Change an IPv6 address of a network interface
 *
 * @param netif the network interface to change
 * @param addr_idx index of the IPv6 address
 * @param addr6 the new IPv6 address
 *
 * @note call netif_ip6_addr_set_state() to set the address valid/temptative
 */
void
netif_ip6_addr_set(struct netif *netif, s8_t addr_idx, const ip6_addr_t *addr6)
{
    netif_ip6_addr_set_adpt(netif, addr_idx, addr6);
}

/*
 * Change an IPv6 address of a network interface (internal version taking 4 * u32_t)
 *
 * @param netif the network interface to change
 * @param addr_idx index of the IPv6 address
 * @param i0 word0 of the new IPv6 address
 * @param i1 word1 of the new IPv6 address
 * @param i2 word2 of the new IPv6 address
 * @param i3 word3 of the new IPv6 address
 */
void
netif_ip6_addr_set_parts(struct netif *netif, s8_t addr_idx, u32_t i0, u32_t i1, u32_t i2, u32_t i3)
{
    netif_ip6_addr_set_parts_adpt(netif, addr_idx, i0, i1, i2, i3);
}

/**
 * @ingroup netif_ip6
 * Change the state of an IPv6 address of a network interface
 * (INVALID, TEMPTATIVE, PREFERRED, DEPRECATED, where TEMPTATIVE
 * includes the number of checks done, see ip6_addr.h)
 *
 * @param netif the network interface to change
 * @param addr_idx index of the IPv6 address
 * @param state the new IPv6 address state
 */
void
netif_ip6_addr_set_state(struct netif* netif, s8_t addr_idx, u8_t state)
{
    netif_ip6_addr_set_state_adpt(netif, addr_idx, state);
}

/**
 * Checks if a specific address is assigned to the netif and returns its
 * index.
 *
 * @param netif the netif to check
 * @param ip6addr the IPv6 address to find
 * @return >= 0: address found, this is its index
 *         -1: address not found on this netif
 */
s8_t
netif_get_ip6_addr_match(struct netif *netif, const ip6_addr_t *ip6addr)
{
    return netif_get_ip6_addr_match_adpt(netif, ip6addr);
}

/**
 * @ingroup netif_ip6
 * Create a link-local IPv6 address on a netif (stored in slot 0)
 *
 * @param netif the netif to create the address on
 * @param from_mac_48bit if != 0, assume hwadr is a 48-bit MAC address (std conversion)
 *                       if == 0, use hwaddr directly as interface ID
 */
void
netif_create_ip6_linklocal_address(struct netif *netif, u8_t from_mac_48bit)
{
    netif_create_ip6_linklocal_address_adpt(netif, from_mac_48bit);
}

/**
 * @ingroup netif_ip6
 * This function allows for the easy addition of a new IPv6 address to an interface.
 * It takes care of finding an empty slot and then sets the address tentative
 * (to make sure that all the subsequent processing happens).
 *
 * @param netif netif to add the address on
 * @param ip6addr address to add
 * @param chosen_idx if != NULL, the chosen IPv6 address index will be stored here
 */
err_t
netif_add_ip6_address(struct netif *netif, const ip6_addr_t *ip6addr, s8_t *chosen_idx)
{
    return netif_add_ip6_address_adpt(netif, ip6addr, chosen_idx);
}

/** Dummy IPv6 output function for netifs not supporting IPv6
 */
err_t
netif_null_output_ip6(struct netif *netif, struct pbuf *p, const ip6_addr_t *ipaddr)
{
    return netif_null_output_ip6_adpt(netif, p, ipaddr);
}
#endif /* LWIP_IPV6 */
