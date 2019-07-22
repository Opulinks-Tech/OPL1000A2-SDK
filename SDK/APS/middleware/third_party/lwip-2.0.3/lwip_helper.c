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
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "lwip/tcpip.h"
#include "lwip/netif.h"
#include "lwip/stats.h"
#include "lwip/dhcp.h"

#include "wlannetif.h"
#include "lwip_helper.h"
#include "network_config.h"
#include "event_loop.h"
#include "wifi_api.h"
#include "wifi_mac_task.h"
#include "ps.h"


LWIP_RETDATA sys_sem_t wifi_connected;
LWIP_RETDATA sys_sem_t ip_ready;

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
//I0060 0001137_add_beg
#define IPADDR_TIMEOUT            ((u32_t)0x0000FEA9UL)
#define IP4_ADDR_ISTIMEOUT(addr1) (((*(addr1)).addr & IPADDR_TIMEOUT) == IPADDR_TIMEOUT)
//I0060 0001137_add_end

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/
/* NETIF data */
LWIP_RETDATA struct netif netif;
LWIP_RETDATA bool tcpip_inited;

extern LWIP_RETDATA int wakeup_event_timeouts;

/*****************************************************************************
 * Private functions declarations
 ****************************************************************************/
//static void tcpip_init_done( void* arg );
//static void ip_ready_callback(struct netif *netif);

//static int32_t wifi_station_connected_event_handler(void);
//static int32_t wifi_station_disconnected_event_handler(void);

//I0060 0001137_add_beg
static uint8_t def_dhcp_retry = 0;
static uint8_t dhcp_retry = 0;
//I0060 0001137_add_end

/*****************************************************************************
 * Private functions
 ****************************************************************************/
static void lwip_check_timeouts(PS_WAKEUP_TYPE wake_type)
{
    /* set wakeup timeout = BLE maximum adv interval (10s) + 500ms */
    wakeup_event_timeouts = 10*1000 + 500;
    tcpip_check_timeouts();
    return;
}

/**
  * @brief  wifi connected will call this callback function. set lwip status in this function
  * @retval None
  */
static int32_t wifi_station_connected_event_handler_impl(void *arg)
{
    LWIP_UNUSED_ARG(arg);
    netif_set_link_up(&netif);
    sys_sem_signal(&wifi_connected);

    /* reset to initial timeout value (1000 ms) */
    wakeup_event_timeouts = 1000;

    printf("wifi connected\r\n");
    return 0;
}


/**
  * @brief  wifi disconnected will call this callback function. set lwip status in this function
  * @retval None
  */
static int32_t wifi_station_disconnected_event_handler_impl(void *arg)
{
    LWIP_UNUSED_ARG(arg);
    netif_set_link_down(&netif);
    if(dhcp_config_init() == STA_IP_MODE_DHCP) {
        netif_set_addr(&netif, IP4_ADDR_ANY4, IP4_ADDR_ANY4, IP4_ADDR_ANY4);
    }
    printf("wifi disconnected\r\n");
    return 1;
}

static void tcpip_init_done_cb_impl(void* arg)
{
	/* Tell main thread TCP/IP init is done */
	*(s32_t *)arg = 1;
}

static void ip_ready_callback_impl(struct netif *netif)
{
    event_msg_t msg = {0};
#if 1 //I0060 0001137_add_beg
    if (!ip4_addr_isany(netif_ip4_addr(netif))) {
        sys_sem_signal(&ip_ready);
        char ipaddrstr[16] = {0};
        if (IP4_ADDR_ISTIMEOUT(netif_ip4_addr(netif))) { //DHCP timeout
            dhcp_retry++;
            
            ipaddr_ntoa_r(&netif->ip_addr, ipaddrstr, sizeof(ipaddrstr));
            printf("DHCP got timeout IP:%s\r\n", ipaddrstr);
            
            // 1. Retry DHCP mechanism
            if (dhcp_retry <= def_dhcp_retry) {
                printf("DHCP got timeout IP, retry %d\r\n", dhcp_retry);
#if LWIP_DHCP
                dhcp_start(netif);
#endif
            }
            // 2. Wifi disconnect
            else {
                printf("DHCP got Failed\r\n");
#if LWIP_DHCP
                dhcp_release(netif);
                dhcp_stop(netif);
#endif
                wifi_connection_disconnect_ap();
                dhcp_retry = 0;
            }
        }
        else {
            dhcp_retry = 0;
            ipaddr_ntoa_r(&netif->ip_addr, ipaddrstr, sizeof(ipaddrstr));
            printf("DHCP got IP:%s\r\n", ipaddrstr);
            msg.event = WIFI_EVENT_STA_GOT_IP;
            msg.length = 0;
            msg.param = NULL;
            wifi_event_loop_send(&msg);
        }
    }
#else
    if (!ip4_addr_isany(netif_ip4_addr(netif))) {
        char ipaddrstr[16] = {0};
        if (NULL != ipaddr_ntoa_r(&netif->ip_addr, ipaddrstr, sizeof(ipaddrstr))) {
        printf("DHCP got IP:%s\r\n", ipaddrstr);
        sys_sem_signal(&ip_ready);

        msg.event = WIFI_EVENT_STA_GOT_IP;
        msg.length = 0;
        msg.param = NULL;
        wifi_event_loop_send(&msg);

        } else {
        printf("DHCP got Failed\r\n");

        }
    }
#endif //I0060 0001137_add_end
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/
void lwip_tcpip_init_impl(void)
{
	ip4_addr_t ipaddr, netmask, gw;
	volatile s32_t tcpipdone = 0;
    lwip_tcpip_config_t tcpip_config = {{0}, {0}, {0}};

    tcpip_init(tcpip_init_done_cb, (void *) &tcpipdone);
	while (!tcpipdone) {
		sys_msleep(1);
	}

	LWIP_DEBUGF(NETIF_DEBUG, ("LwIP TCPIP thread is initialized.. \n"));

    if (0 != tcpip_config_init(&tcpip_config)) {
        LWIP_DEBUGF(NETIF_DEBUG, ("tcpip config init fail \n"));
        return;
    }

    ipaddr = tcpip_config.sta_ip;
    netmask = tcpip_config.sta_mask;
    gw = tcpip_config.sta_gw;

	/* Add netif interface for OPL1000, wlan interface */
	if (!netif_add(&netif, &ipaddr, &netmask, &gw, NULL, ethernetif_init, tcpip_input)) {
		LWIP_ASSERT("Net interface failed to initialize\r\n", 0);
	}
	netif_set_default(&netif);

#ifdef WIFI_RX_DATA_USE_IPC
    wifi_mac_register_rxcb(&netif, lwip_wlan_sta_input);
#endif

#if LWIP_IPV6
    netif_create_ip6_linklocal_address(&netif, 1);
    netif.ip6_autoconfig_enabled = 1;
    LWIP_DEBUGF(NETIF_DEBUG, ("ip6 linklocal address: "));
    ip6_addr_debug_print(NETIF_DEBUG, netif_ip6_addr(&netif, 0));
#endif
	netif_set_up(&netif);

    //netif_set_link_callback(&netif, lwip_netif_link_irq);
    //netif_set_status_callback(&netif, lwip_netif_status_irq);
}

void lwip_network_init_impl(uint8_t opmode)
{
    LWIP_UNUSED_ARG(opmode);

    if (tcpip_inited == false) {
        tcpip_inited = true;

        /* Initialize the LwIP system.  */
        LWIP_DEBUGF(LWIP_DBG_ON, ("Initialising LwIP " "2.0.0" "\n"));

        sys_sem_new(&wifi_connected, 0);

        if(dhcp_config_init() == STA_IP_MODE_DHCP) {
            sys_sem_new(&ip_ready, 0);
        }

        lwip_tcpip_init();

        /* register check timeouts callback for Smart Sleep */
        ps_set_wakeup_callback_internal(lwip_check_timeouts);
    }
}

void lwip_net_start_impl(uint8_t opmode)
{
    //struct netif *sta_if;
    //struct netif *ap_if;
    LWIP_UNUSED_ARG(opmode);

    switch(opmode) {
        case WIFI_MODE_STA_ONLY:
            //ToDO: register wifi connection event
            //wifi_connection_register_event_handler(WIFI_EVENT_IOT_PORT_SECURE, wifi_station_connected_event_handler);
            //wifi_connection_register_event_handler(WIFI_EVENT_IOT_DISCONNECTED, wifi_station_disconnected_event_handler);
            if(dhcp_config_init() == STA_IP_MODE_DHCP) {
                //sta_if = netif_find_by_type(NETIF_TYPE_STA);
                netif_set_default(&netif);
                netif_set_link_up(&netif);
                netif_set_status_callback(&netif, ip_ready_callback);
                #if LWIP_DHCP
                dhcp_start(&netif);
                #endif
            }
            else
            {
                netif_set_link_up(&netif);
            }
            break;
        case WIFI_MODE_AP_ONLY: {
            /*
            dhcpd_settings_t dhcpd_settings = {{0},{0},{0},{0},{0},{0},{0}};
            strcpy((char *)dhcpd_settings.dhcpd_server_address, AP_IPADDR);
            strcpy((char *)dhcpd_settings.dhcpd_netmask, AP_NETMASK);
            strcpy((char *)dhcpd_settings.dhcpd_gateway, AP_GATEWAY);
            strcpy((char *)dhcpd_settings.dhcpd_primary_dns, PRIMARY_DNS);
            strcpy((char *)dhcpd_settings.dhcpd_secondary_dns, SECONDARY_DNS);
            strcpy((char *)dhcpd_settings.dhcpd_ip_pool_start, IP_POOL_START);
            strcpy((char *)dhcpd_settings.dhcpd_ip_pool_end, IP_POOL_END);
            ap_if = netif_find_by_type(NETIF_TYPE_AP);
            netif_set_default(ap_if);
            netif_set_link_up(ap_if);
            dhcpd_start(&dhcpd_settings);
            */
            break;
        }
    }


}

void lwip_net_stop_impl(uint8_t opmode)
{
    //struct netif *sta_if;
    //struct netif *ap_if;

    //sta_if = netif_find_by_type(NETIF_TYPE_STA);
    //ap_if = netif_find_by_type(NETIF_TYPE_AP);
    switch (opmode) {
        case WIFI_MODE_AP_ONLY:
            /*
            dhcpd_stop();
            netif_set_link_down(ap_if);
            break;*/
        case WIFI_MODE_STA_ONLY:
            netif_set_status_callback(&netif, NULL);
            if(dhcp_config_init() == STA_IP_MODE_DHCP) {
                #if LWIP_DHCP
                dhcp_release(&netif);
                dhcp_stop(&netif);
                #endif
            }
            netif_set_link_down(&netif);
            break;
    }
}

void lwip_net_ready_impl(void)
{
    sys_arch_sem_wait(&wifi_connected, 0);
    if(dhcp_config_init() == STA_IP_MODE_DHCP) {
        sys_arch_sem_wait(&ip_ready, 0);
    }
}

int lwip_get_ip_info_impl(char *ifname)
{
    struct netif *iface = netif_find(ifname);

    if (!iface && !netif_is_up(iface)) {
        return -1;
    }

    printf("\ninterface: %s\n", ifname);

#if LWIP_DHCP
    if (dhcp_supplied_address(iface)) {
        struct dhcp *d = ((struct dhcp*)(iface)->client_data[LWIP_NETIF_CLIENT_DATA_INDEX_DHCP]);
        printf("ip mode : DHCP\n");
        printf("  ip      %s\n", ip4addr_ntoa(&d->offered_ip_addr));
        printf("  netmask %s\n", ip4addr_ntoa(&d->offered_sn_mask));
        printf("  gateway %s\n", ip4addr_ntoa(&d->offered_gw_addr));
    }
    else
#endif
    {
        printf("ip mode : STATIC\n");
        printf("ip addr : %s\n", ipaddr_ntoa(&iface->ip_addr));
        printf("netmask : %s\n", ipaddr_ntoa(&iface->netmask));
        printf("gateway : %s\n", ipaddr_ntoa(&iface->gw));
    }

    return 0;
}

int lwip_wlan_sta_input_impl(void *buffer, uint16_t len, void *arg)
{
    wlanif_input(&netif, buffer, len, arg);
    return ERR_OK;
}

int lwip_get_netif_impl(struct netif *iface)
{
    if (iface == NULL) return -1;

    memset(iface, 0, sizeof(netif));

    struct netif *pnetif = netif_find("st1");

    if (!netif_is_up(pnetif)) {
        return -1;
    }

    memcpy(iface, pnetif, sizeof(netif));

    return 0;
}

/*-------------------------------------------------------------------------------------
 * Definitions of interface function pointer
 *------------------------------------------------------------------------------------*/
RET_DATA lwip_network_init_fp_t   lwip_network_init;
RET_DATA lwip_net_start_fp_t      lwip_net_start;
RET_DATA lwip_net_stop_fp_t       lwip_net_stop;
RET_DATA lwip_net_ready_fp_t      lwip_net_ready;
RET_DATA lwip_wlan_sta_input_fp_t lwip_wlan_sta_input;
RET_DATA lwip_tcpip_init_fp_t     lwip_tcpip_init;
RET_DATA tcpip_init_done_cb_fp_t  tcpip_init_done_cb;
RET_DATA lwip_get_ip_info_fp_t    lwip_get_ip_info;
RET_DATA ip_ready_callback_fp_t   ip_ready_callback;
RET_DATA wifi_station_connected_event_handler_fp_t    wifi_station_connected_event_handler;
RET_DATA wifi_station_disconnected_event_handler_fp_t wifi_station_disconnected_event_handler;
RET_DATA lwip_get_netif_fp_t      lwip_get_netif;

/*-------------------------------------------------------------------------------------
 * Interface assignment
 *------------------------------------------------------------------------------------*/
void lwip_load_interface_lwip_helper(void)
{
    /* Cold boot initialization for "zero_init" retention data */
    memset(&netif, 0, sizeof(struct netif));
    memset(&lwip_stats, 0, sizeof(lwip_stats));

    tcpip_inited = false;
    def_dhcp_retry = 1; //I0060 0001137
    
    lwip_network_init   = lwip_network_init_impl;
    lwip_net_start      = lwip_net_start_impl;
    lwip_net_stop       = lwip_net_stop_impl;
    lwip_net_ready      = lwip_net_ready_impl;
    lwip_wlan_sta_input = lwip_wlan_sta_input_impl;
    lwip_tcpip_init     = lwip_tcpip_init_impl;
    tcpip_init_done_cb  = tcpip_init_done_cb_impl;
    lwip_get_ip_info    = lwip_get_ip_info_impl;
    ip_ready_callback   = ip_ready_callback_impl;

    wifi_station_connected_event_handler    = wifi_station_connected_event_handler_impl;
    wifi_station_disconnected_event_handler = wifi_station_disconnected_event_handler_impl;
    lwip_get_netif      = lwip_get_netif_impl;
}

