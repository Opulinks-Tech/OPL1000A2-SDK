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

#include "network_config.h"
#include "lwip_helper.h"
#include "wlannetif.h"
#include "wifi_mac_task.h"

extern struct netif netif;

/**
  * @brief  wifi connect failed will call this callback function. set lwip status in this function
  * @retval None
  */
int32_t wifi_station_connect_failed_event_handler(void *arg)
{
    LWIP_UNUSED_ARG(arg);
    netif_set_link_down(&netif);
    if(dhcp_config_init() == STA_IP_MODE_DHCP) {
        netif_set_addr(&netif, IP4_ADDR_ANY4, IP4_ADDR_ANY4, IP4_ADDR_ANY4);
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

/*-------------------------------------------------------------------------------------
 * Interface assignment
 *------------------------------------------------------------------------------------*/
void lwip_load_interface_lwip_helper_patch(void)
{
    lwip_tcpip_init  = lwip_tcpip_init_patch;
}
