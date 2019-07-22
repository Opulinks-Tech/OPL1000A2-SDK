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

#ifndef _SYS_COMMON_API_H_
#define _SYS_COMMON_API_H_

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
 
/**
  * @brief     Get the configuration of Mac address source
  *
  * @attention 1. API returns false if try to get Configuration which something error
  *
  * @param[in]   iface: The interface of mac address
  *              - MAC_IFACE_WIFI_STA
  *              - MAC_IFACE_WIFI_SOFTAP (Not support yet)
  *              - MAC_IFACE_BLE
  *
  * @param[out]  type: Mac address source type
  *              - MAC_SOURCE_FROM_OTP
  *              - MAC_SOURCE_FROM_FLASH
  *
  * @return    0  : success
  * @return    other : failed
 */
int mac_addr_get_config_source(mac_iface_t iface, mac_source_type_t *type);

/**
  * @brief     Set the configuration of Mac address source
  *
  * @attention 1. API returns false if try to set Configuration which something error
  *
  * @param[in]   iface: The interface of mac address
  *              - MAC_IFACE_WIFI_STA
  *              - MAC_IFACE_WIFI_SOFTAP (Not support yet)
  *              - MAC_IFACE_BLE
  *
  * @param[in]  type: Mac address source type
  *              - MAC_SOURCE_FROM_OTP
  *              - MAC_SOURCE_FROM_FLASH
  *
  * @return    0  : success
  * @return    other : failed
 */
int mac_addr_set_config_source(mac_iface_t iface, mac_source_type_t type);

/**
  * @brief     Get the configuration of RF power level
  *
  * @attention 1. API returns false if try to get Configuration which something error
  *
  * @param[out]   level: The level of RF power
  *              - SYS_RF_LOW_POWER
  *              - SYS_RF_HIGH_POWER
  *
  * @return    0  : success
  * @return    other : failed
 */
int sys_get_config_rf_power_level(uint8_t *level);

/**
  * @brief     Set the configuration of RF power level
  *
  * @attention 1. API returns false if try to set Configuration which something error
  *
  * @param[in]   level: The level of RF power
  *              - [7:4] WiFi Power level. 0 for lowest and 0xF for highest
  *              - [3:0] BLE Power level. 0 for lowest and 0xF for highest
  *
  * @return    0  : success
  * @return    other : failed
 */
int sys_set_config_rf_power_level(uint8_t level);

/**
  * @brief     Get the configuration of DHCP ARP check mechanism
  *
  * @attention 1. API returns false if try to set Configuration which something error
  * @attention 2. Default is Enable.
  *
  * @param[out]   mode: Enable/Disable DHCP ARP check mechanism
  *              - 0 : Disable
  *              - 1 : Enable
  *
  * @return    0  : success
  * @return    other : failed
 */
int tcp_get_config_dhcp_arp_check(uint8_t *mode);

/**
  * @brief     Set the configuration of DHCP ARP check mechanism
  *
  * @attention 1. API returns false if try to set Configuration which something error
  *
  * @param[in]   mode: Enable/Disable DHCP ARP check mechanism
  *              - 0 : Disable
  *              - 1 : Enable
  *
  * @return    0  : success
  * @return    other : failed
 */
int tcp_set_config_dhcp_arp_check(uint8_t mode);


/**
  * @brief     Set the configuration of DHCP retry mode,retry interval and maximum retry count 
  *
  * @attention 1. API returns false if try to set Configuration which something error
  *
  * @param[in]   dhcp_mode: specified DHCP retry mechanism
  *              - 0 : Fix mode , this mode will send DHCP discover packet in fix frequency which you set in dhcp_interval   
  *              - 1 : Exponential mode , when DHCP get IP fail, this mode will make DHCP discover packet sending interval increase as exponential rate 
  *
  * @param[in]   dhcp_interval: 
  *              - In Fix mode, recommand to set this value to 1000,1500,2000 or 2500 (ms).        
  *              - In Exponential mode, recommand to set this value to 250,500,750 or 1000 (ms)
  *
  * @param[in]   dhcp_retry_times: specified the maximum DHCP retry times
  *              - In Fix mode, recommand to set this value to 1~255
  *              - In Exponential mode, recommand to set this value to 9
  * @return    0  : success
  * @return    other : failed
 */
int tcp_set_dhcp_interval_retry_times(uint8_t dhcp_mode,uint32_t dhcp_interval,uint8_t dhcp_retry_times);


/**
  * @brief     Write BLE BD_ADDR to flash 
  *
  * @attention 1. API returns false if try to set Configuration which something error
  *
  * @param[in]   bd_addr:
  *              - 6 bytes BLE BD_ADDR (new BD_ADDR only take effect after reboot)
  *
  * @return    0  : success
  * @return    other : failed
 */
int ble_set_config_bd_addr(uint8_t *bd_addr);

/**
  * @brief     Read BLE BD_ADDR
  *
  * @attention 1. API returns false if try to set Configuration which something error
  *
  * @param[out]   bd_addr:
  *               - 6 bytes BLE BD_ADDR
  *
  * @return    0  : success
  * @return    other : failed
 */
int ble_get_config_bd_addr(uint8_t *bd_addr);

#ifdef __cplusplus
}
#endif

#endif /* _SYS_COMMON_API_H_ */
