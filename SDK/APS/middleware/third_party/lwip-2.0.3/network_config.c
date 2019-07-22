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
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "network_config.h"
#include "sys_common_ctrl.h"

extern int dhcp_does_arp_check_flag;

// TODO:Get DHCP, IP configuration from flash

int32_t dhcp_config_init_impl(void)
{
    //Read "DHCP ARP check" for the first time in Sys_ServiceInit
    //tcpip_config_dhcp_arp_check_init();
    return (USE_DHCP == 0) ? STA_IP_MODE_STATIC : STA_IP_MODE_DHCP;
}

int32_t tcpip_config_init_impl(lwip_tcpip_config_t *tcpip_config)
{
    /* Static IP assignment */
    ip4addr_aton(STA_IPADDR, &(tcpip_config->sta_ip));
    ip4addr_aton(STA_NETMASK, &tcpip_config->sta_mask);
    ip4addr_aton(STA_GATEWAY, &tcpip_config->sta_gw);

    return 0;
}

void tcpip_config_dhcp_arp_check_init_impl(void)
{
    dhcp_does_arp_check_flag = get_dhcp_arp_check_from_fim();
}

/*-------------------------------------------------------------------------------------
 * Definitions of interface function pointer
 *------------------------------------------------------------------------------------*/
RET_DATA tcpip_config_init_fp_t tcpip_config_init;
RET_DATA dhcp_config_init_fp_t  dhcp_config_init;
RET_DATA tcpip_config_dhcp_arp_check_init_fp_t tcpip_config_dhcp_arp_check_init;

/*-------------------------------------------------------------------------------------
 * Interface assignment
 *------------------------------------------------------------------------------------*/
void lwip_load_interface_network_config(void)
{
    tcpip_config_init = tcpip_config_init_impl;
    dhcp_config_init  = dhcp_config_init_impl;
    tcpip_config_dhcp_arp_check_init = tcpip_config_dhcp_arp_check_init_impl;
}

