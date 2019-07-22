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

#ifndef __NETWORK_CONFIG_H__
#define __NETWORK_CONFIG_H__

#include <stdint.h>
#include <stdio.h>
#include <string.h>


#include "lwip/sockets.h"
#include "lwip/ip4_addr.h"
#include "lwip/inet.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "lwip/dhcp.h"
#include "lwip_helper.h"

/* Rom patch interface */
typedef int32_t (*tcpip_config_init_fp_t)(lwip_tcpip_config_t *tcpip_config);
typedef int32_t (*dhcp_config_init_fp_t)(void);
typedef void (*tcpip_config_dhcp_arp_check_init_fp_t) (void);

/* Export interface funtion pointer */
extern tcpip_config_init_fp_t   tcpip_config_init;
extern dhcp_config_init_fp_t    dhcp_config_init;
extern tcpip_config_dhcp_arp_check_init_fp_t tcpip_config_dhcp_arp_check_init;

#define USE_DHCP       1

#define STA_IPADDR    ("0.0.0.0")
#define STA_NETMASK   ("0.0.0.0")
#define STA_GATEWAY   ("0.0.0.0")

#endif  /*__WIFI_LWIP_DEFAULT_CONFIG_H__*/
