/******************************************************************************
*  Copyright 2019, Netlink Communication Corp.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Netlink Communication Corp. (C) 2019
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "lwip/tcpip.h"
#include "lwip/netif.h"
#include "lwip/stats.h"
#include "lwip/dhcp.h"

#include "network_config.h"
#include "lwip_helper.h"
#include "wlannetif.h"
#include "wifi_mac_task.h"
#include "ps.h"
#include "event_loop.h"
#include "controller_wifi.h"
#include "wifi_api.h"

#define IPADDR_TIMEOUT            ((u32_t)0x0000FEA9UL)
#define IP4_ADDR_ISTIMEOUT(addr1) (((*(addr1)).addr & IPADDR_TIMEOUT) == IPADDR_TIMEOUT)

static uint8_t def_dhcp_retry = 1;
static uint8_t dhcp_retry = 0;

extern struct netif netif;
extern bool tcpip_inited;
extern sys_sem_t wifi_connected;
extern sys_sem_t ip_ready;
extern int wakeup_event_timeouts;

extern u32_t last_wakeup_time;

static void lwip_check_timeouts(PS_WAKEUP_TYPE wake_type)
{
    /* set wakeup timeout = BLE maximum adv interval (10s) + 500ms */
    wakeup_event_timeouts = 10*1000 + 500;
    last_wakeup_time = sys_now();
    tcpip_check_timeouts();
    return;
}

/**
  * @brief  wifi connect failed will call this callback function. set lwip status in this function
  * @retval None
  */
int32_t wifi_station_connect_failed_event_handler(void *arg)
{
    LWIP_UNUSED_ARG(arg);
    if(dhcp_config_init() == STA_IP_MODE_DHCP) {
        lwip_net_stop(WIFI_MODE_STA_ONLY);
    }
    printf("[lwip_helper] wifi connect failed\r\n");
    return 1;
}

void lwip_tcpip_init_patch(void)
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

    netif_set_up(&netif);

    //netif_set_link_callback(&netif, lwip_netif_link_irq);
    //netif_set_status_callback(&netif, lwip_netif_status_irq);
}

void lwip_network_init_patch(uint8_t opmode)
{
    LWIP_UNUSED_ARG(opmode);

    if (tcpip_inited == false) {
        tcpip_inited = true;

        /* Initialize the LwIP system.  */
        sys_sem_new(&wifi_connected, 0);

        if(dhcp_config_init() == STA_IP_MODE_DHCP) {
            sys_sem_new(&ip_ready, 0);
        }

        lwip_tcpip_init();

        /* register check timeouts callback for Smart Sleep */
        ps_set_wakeup_callback_internal(lwip_check_timeouts);
    }
}

void lwip_net_start_patch(uint8_t opmode)
{
    switch(opmode) {
        case WIFI_MODE_STA_ONLY:

            if(dhcp_config_init() == STA_IP_MODE_DHCP) {
                //sta_if = netif_find_by_type(NETIF_TYPE_STA);
                netif_set_default(&netif);
                netif_set_status_callback(&netif, ip_ready_callback);
                #if LWIP_DHCP
                tcpip_callback_with_block((tcpip_callback_fn)dhcp_start, &netif, 1);
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

void lwip_net_stop_patch(uint8_t opmode)
{
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
                tcpip_callback_with_block((tcpip_callback_fn)dhcp_release, &netif, 1);
                tcpip_callback_with_block((tcpip_callback_fn)dhcp_stop, &netif, 1);
                #endif
            }
            else
            {
              netif_set_link_down(&netif);
            }
            break;
    }
}

static void ip_ready_callback_patch(struct netif *netif)
{
    event_msg_t msg = {0};
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
            /* Enter the PS POLL */
            CtrlWifi_PsStateForce(STA_PS_NONE, 0);
            
            dhcp_retry = 0;
            ipaddr_ntoa_r(&netif->ip_addr, ipaddrstr, sizeof(ipaddrstr));
            printf("DHCP got IP:%s\r\n", ipaddrstr);
            msg.event = WIFI_EVENT_STA_GOT_IP;
            msg.length = 0;
            msg.param = NULL;
            wifi_event_loop_send(&msg);
        }
    }
}

static int32_t wifi_station_connected_event_handler_patch(void *arg)
{
    LWIP_UNUSED_ARG(arg);
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
static int32_t wifi_station_disconnected_event_handler_patch(void *arg)
{
    LWIP_UNUSED_ARG(arg);
    if(dhcp_config_init() == STA_IP_MODE_DHCP) {
        lwip_net_stop(WIFI_MODE_STA_ONLY);
    }
    printf("wifi disconnected\r\n");
    return 1;
}

/*-------------------------------------------------------------------------------------
 * Interface assignment
 *------------------------------------------------------------------------------------*/
void lwip_load_interface_lwip_helper_patch(void)
{
    lwip_tcpip_init  = lwip_tcpip_init_patch;
    lwip_network_init = lwip_network_init_patch;
    lwip_net_start   = lwip_net_start_patch;
    lwip_net_stop    = lwip_net_stop_patch;
    ip_ready_callback = ip_ready_callback_patch;
    wifi_station_connected_event_handler    = wifi_station_connected_event_handler_patch;
    wifi_station_disconnected_event_handler = wifi_station_disconnected_event_handler_patch;

}
