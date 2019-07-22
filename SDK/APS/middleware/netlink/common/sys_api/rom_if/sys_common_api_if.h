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

#ifndef _SYS_COMMON_API_IF_H_
#define _SYS_COMMON_API_IF_H_

#include <stdint.h>
#include <stdbool.h>
#include "sys_common_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
 *                    Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *               Global Variables
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/
void sys_common_api_func_init(void);
int mac_addr_get_config_source_impl(mac_iface_t iface, mac_source_type_t *type);
int mac_addr_set_config_source_impl(mac_iface_t iface, mac_source_type_t type);
int sys_get_config_rf_power_level_impl(uint8_t *level);
int sys_set_config_rf_power_level_impl(uint8_t level);
int tcp_get_config_dhcp_arp_check_impl(uint8_t *mode);
int tcp_set_config_dhcp_arp_check_impl(uint8_t mode);
int ble_set_config_bd_addr_impl(uint8_t *bd_addr);
int ble_get_config_bd_addr_impl(uint8_t *bd_addr);

typedef int (*mac_addr_get_config_source_fp_t)(mac_iface_t iface, mac_source_type_t *type);
typedef int (*mac_addr_set_config_source_fp_t)(mac_iface_t iface, mac_source_type_t type);
typedef int (*sys_get_config_rf_power_level_fp_t) (uint8_t *level);
typedef int (*sys_set_config_rf_power_level_fp_t) (uint8_t level);
typedef int (*tcp_get_config_dhcp_arp_check_fp_t) (uint8_t *mode);
typedef int (*tcp_set_config_dhcp_arp_check_fp_t) (uint8_t mode);
typedef int (*ble_set_config_bd_addr_fp_t) (uint8_t *bd_addr);
typedef int (*ble_get_config_bd_addr_fp_t) (uint8_t *bd_addr);

/* Export interface funtion pointer */
extern mac_addr_get_config_source_fp_t mac_addr_get_config_source_api;
extern mac_addr_set_config_source_fp_t mac_addr_set_config_source_api;
extern sys_get_config_rf_power_level_fp_t sys_get_config_rf_power_level_api;
extern sys_set_config_rf_power_level_fp_t sys_set_config_rf_power_level_api;
extern tcp_get_config_dhcp_arp_check_fp_t tcp_get_config_dhcp_arp_check_api;
extern tcp_set_config_dhcp_arp_check_fp_t tcp_set_config_dhcp_arp_check_api;
extern ble_set_config_bd_addr_fp_t ble_set_config_bd_addr_api;
extern ble_get_config_bd_addr_fp_t ble_get_config_bd_addr_api;

#ifdef __cplusplus
}
#endif

#endif /* _SYS_COMMON_API_IF_H_ */
