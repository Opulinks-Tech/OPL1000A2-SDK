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

