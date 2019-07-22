/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

#include "lwip/opt.h"

/* user api related */
extern void lwip_load_interface_api_lib(void);
extern void lwip_load_interface_netifapi(void);
extern void lwip_load_interface_netdb(void);
extern void lwip_load_interface_sockets(void);
extern void lwip_load_interface_netbuf(void);
extern void lwip_load_interface_api_msg(void);
extern void lwip_load_interface_tcpip(void);
/* ipv4 related */
#include "lwip_ipv4_jmptbl.h"

/* ipv6 related */
extern void lwip_load_interface_icmp6(void);
extern void lwip_load_interface_ethip6(void);
extern void lwip_load_interface_ip6(void);
extern void lwip_load_interface_mld6(void);
extern void lwip_load_interface_nd6(void);
extern void lwip_load_interface_ip6_frag(void);

/* common */
extern void lwip_load_interface_tcp(void);
extern void lwip_load_interface_udp(void);
extern void lwip_load_interface_dns(void);
extern void lwip_load_interface_raw(void);
extern void lwip_load_interface_netif(void);
extern void lwip_load_interface_ip(void);
extern void lwip_load_interface_pbuf(void);
extern void lwip_load_interface_timeouts(void);
extern void lwip_load_interface_mem(void);
extern void lwip_load_interface_memp(void);

/* network interface */
extern void lwip_load_interface_ethernet(void);

/* porting layer */
extern void lwip_load_interface_sys_arch(void);
extern void lwip_load_interface_wlannetif(void);
extern void lwip_load_interface_lwip_helper(void);
extern void lwip_load_interface_network_config(void);

/* application && cli */
extern void lwip_load_interface_cli(void);
extern void lwip_load_interface_socket_app(void);

void lwip_module_interface_init(void)
{
#if defined(LWIP_ROMBUILD)

    /* ipv4 related */
#if LWIP_IGMP
    lwip_load_interface_icmp();
#endif
#if LWIP_IGMP
    lwip_load_interface_igmp();
#endif

#if LWIP_IPV4
    lwip_load_interface_ip4();
#endif

    lwip_load_interface_ip4_frag();

    lwip_load_interface_etharp();

#if LWIP_IPV4 && LWIP_DHCP
    lwip_load_interface_dhcp();
#endif

#if LWIP_IPV4 && LWIP_AUTOIP
    lwip_load_interface_autoip();
#endif

    /* ipv6 related */
#if LWIP_ICMP6 && LWIP_IPV6
    lwip_load_interface_icmp6();
#endif

#if LWIP_IPV6 && LWIP_ETHERNET
    lwip_load_interface_ethip6();
#endif

#if LWIP_IPV6
    lwip_load_interface_ip6();
    lwip_load_interface_ip6_frag();
#endif

#if LWIP_IPV6 && LWIP_IPV6_MLD
    lwip_load_interface_mld6();
#endif

#if LWIP_IPV6
    lwip_load_interface_nd6();
#endif

    /* common */
    lwip_load_interface_mem();
    lwip_load_interface_memp();
    lwip_load_interface_pbuf();
    lwip_load_interface_timeouts();

    lwip_load_interface_tcp();

#if LWIP_UDP
    lwip_load_interface_udp();
#endif

#if LWIP_DNS
    lwip_load_interface_dns();
#endif

#if LWIP_RAW
    lwip_load_interface_raw();
#endif

    lwip_load_interface_netif();

#if LWIP_IPV4 && LWIP_IPV6
    lwip_load_interface_ip();
#endif

    /* user api related */
    lwip_load_interface_api_lib();

#if LWIP_SOCKET
    lwip_load_interface_sockets();
#endif

#if LWIP_DNS && LWIP_SOCKET
    lwip_load_interface_netdb();
#endif

#if LWIP_NETIF_API
    lwip_load_interface_netifapi();
#endif

#if LWIP_NETCONN
    lwip_load_interface_netbuf();
#endif

#if LWIP_NETCONN
    lwip_load_interface_api_msg();
#endif

    lwip_load_interface_tcpip();

    /* network interface */
#if LWIP_ARP || LWIP_ETHERNET
    lwip_load_interface_ethernet();
#endif

    /* porting layer */
    lwip_load_interface_sys_arch();

    lwip_load_interface_wlannetif();

#endif /* if defined(LWIP_ROMBUILD) */

    lwip_load_interface_lwip_helper();
    lwip_load_interface_network_config();

    lwip_load_interface_cli();
    lwip_load_interface_socket_app();
}
