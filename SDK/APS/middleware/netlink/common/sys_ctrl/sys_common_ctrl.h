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

#ifndef _SYS_COMMON_CTRL_H_
#define _SYS_COMMON_CTRL_H_

#include <stdint.h>
#include <stdbool.h>

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    BASE_NVM_MAC_SRC_IFACE_ID_STA = 0,
    BASE_NVM_MAC_SRC_IFACE_ID_SOFTAP,
    BASE_NVM_MAC_SRC_IFACE_ID_BLE,
    WIFI_NVM_MAC_SRC_IFACE_ID_MAX_NUM,
} base_nvm_mac_src_iface_id_t;

typedef enum {
    BASE_NVM_MAC_SRC_TYPE_ID_OTP = 0,
    BASE_NVM_MAC_SRC_TYPE_ID_FLASH,
    BASE_NVM_MAC_SRC_TYPE_ID_MAX_NUM,
} base_nvm_mac_src_type_id_t;

void sys_common_ctrl_func_init(void);

typedef int (*base_mac_addr_src_get_cfg_fp_t) (u8 iface, u8* type);
typedef int (*base_mac_addr_src_set_cfg_fp_t) (u8 iface, u8 type);
typedef int (*base_nvm_mac_addr_src_read_fp_t) (u16 id, u16 len, void *buf);
typedef int (*base_nvm_mac_addr_src_write_fp_t) (u16 id, u16 len, void *ptr);
typedef int (*get_rf_power_level_fp_t) (u8 *level);
typedef int (*set_rf_power_level_fp_t) (u8 level);
typedef int (*get_dhcp_arp_check_fp_t) (void);
typedef int (*get_dhcp_arp_check_from_fim_fp_t) (void);
typedef int (*set_dhcp_arp_check_fp_t) (u8 mode);
typedef int (*set_dhcp_interval_retry_times_fp_t)(u8 dhcp_mode,u32 dhcp_interval,u8 dhcp_retry_times);
typedef int (*set_ble_bd_addr_fp_t) (uint8_t *addr);
typedef int (*get_ble_bd_addr_fp_t) (uint8_t *addr);

extern base_mac_addr_src_get_cfg_fp_t base_mac_addr_src_get_cfg;
extern base_mac_addr_src_set_cfg_fp_t base_mac_addr_src_set_cfg;
extern base_nvm_mac_addr_src_read_fp_t base_nvm_mac_addr_src_read;
extern base_nvm_mac_addr_src_write_fp_t base_nvm_mac_addr_src_write;
extern get_rf_power_level_fp_t get_rf_power_level;
extern set_rf_power_level_fp_t set_rf_power_level;
extern get_dhcp_arp_check_fp_t get_dhcp_arp_check;
extern get_dhcp_arp_check_from_fim_fp_t get_dhcp_arp_check_from_fim;
extern set_dhcp_arp_check_fp_t set_dhcp_arp_check;
extern set_dhcp_interval_retry_times_fp_t set_dhcp_interval_retry_times;
extern set_ble_bd_addr_fp_t set_ble_bd_addr;
extern get_ble_bd_addr_fp_t get_ble_bd_addr;

#ifdef __cplusplus
}
#endif

#endif /* _SYS_COMMON_CTRL_H_ */
