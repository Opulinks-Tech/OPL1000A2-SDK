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

#ifndef __WIFI_API_IF_H__
#define __WIFI_API_IF_H__

#include <stdint.h>
#include <stdbool.h>

#include "wifi_types.h"
#include "wifi_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \defgroup WIFI_APIs WIFI APIs
  * @brief WIFI APIs
  */

/** @addtogroup WIFI_APIs
  * @{
  */

/** \defgroup WIFI_STA_APIs WIFI STA APIs
 */

/** @addtogroup WIFI_STA_APIs
 * @{
 */


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
int wifi_convert_auth_mode(scan_info_t *pinfo, int privacy);
/* Rom patch interface */

/* Private API fucntion used in wifi_api.c*/
typedef int (*wifi_convert_auth_mode_fp_t)(scan_info_t *pinfo, int privacy);

/*Public API fucntion in wifi_api.c*/
typedef int (*wifi_init_fp_t)(const wifi_init_config_t *config, wifi_init_complete_cb_t init_cb);
typedef int (*wifi_deinit_fp_t)(void);
typedef int (*wifi_set_config_fp_t)(wifi_mode_t interface, wifi_config_t *conf);
typedef int (*wifi_get_config_fp_t)(wifi_mode_t interface, wifi_config_t *conf);
typedef int (*wifi_start_fp_t)(void);
typedef int (*wifi_stop_fp_t)(void);
typedef int (*wifi_scan_start_fp_t)(const wifi_scan_config_t *config, bool block);
typedef int (*wifi_scan_stop_fp_t)(void);
typedef int (*wifi_scan_get_ap_list_fp_t)(wifi_scan_list_t *scan_list);
typedef int (*wifi_scan_get_ap_num_fp_t)(uint16_t *number);
typedef int (*wifi_scan_get_ap_records_fp_t)(uint16_t *number, wifi_scan_info_t *ap_records);
typedef int (*wifi_sta_get_ap_info_fp_t)(wifi_ap_record_t *ap_info);
typedef int (*wifi_connection_disconnect_ap_fp_t)(void);
typedef int (*wifi_connection_disconnect_sta_fp_t)(uint8_t *address);
typedef int (*wifi_connection_connect_fp_t)(wifi_config_t *config);
typedef int (*wifi_connection_scan_start_fp_t)(uint8_t *ssid, uint8_t ssid_length, uint8_t *bssid, uint8_t scan_mode, uint8_t scan_option);
typedef int (*wifi_connection_get_rssi_fp_t)(int8_t *rssi);
typedef int (*wifi_connection_register_event_handler_fp_t)(wifi_event_t event, wifi_event_handler_t handler);
typedef int (*wifi_connection_unregister_event_handler_fp_t)(wifi_event_t event, wifi_event_handler_t handler);
typedef int (*wifi_config_set_opmode_fp_t)(uint8_t mode);
typedef int (*wifi_config_get_opmode_fp_t)(uint8_t *mode);
typedef int (*wifi_config_get_mac_address_fp_t)(wifi_mode_t interface, uint8_t *address);
typedef int (*wifi_config_set_mac_address_fp_t)(wifi_mode_t interface, uint8_t *address);
typedef int (*wifi_config_get_sta_mac_address_from_flash_fp_t)(uint8_t *bssid);
typedef int (*wifi_config_get_ssid_fp_t)(uint8_t *ssid, uint8_t *ssid_length);
typedef int (*wifi_config_set_ssid_fp_t)(wifi_mode_t interface, uint8_t *ssid, uint8_t ssid_length);
typedef int (*wifi_config_get_bssid_fp_t)(uint8_t *bssid);
typedef int (*wifi_config_set_bssid_fp_t)(uint8_t *bssid);
typedef int (*wifi_config_get_channel_fp_t)(wifi_mode_t interface, uint8_t *channel);
typedef int (*wifi_config_set_channel_fp_t)(wifi_mode_t interface, uint8_t channel);
typedef int (*wifi_config_set_bandwidth_fp_t)(wifi_mode_t interface, wifi_bandwidth_t bandwidth);
typedef int (*wifi_config_get_bandwidth_fp_t)(wifi_mode_t interface, wifi_bandwidth_t *bandwidth);
typedef int (*wifi_config_get_dtim_interval_fp_t)(uint8_t *interval);
typedef int (*wifi_config_set_dtim_interval_fp_t)(uint8_t interval);
typedef int (*wifi_config_get_listen_interval_fp_t)(uint8_t *interval);
typedef int (*wifi_config_set_listen_interval_fp_t)(uint8_t interval);
typedef int (*wifi_config_get_skip_dtim_fp_t) (uint8_t *value);
typedef int (*wifi_config_set_skip_dtim_fp_t) (uint8_t value, bool save_flash);
typedef int (*wifi_auto_connect_get_mode_fp_t)(uint8_t *mode);
typedef int (*wifi_auto_connect_set_mode_fp_t)(uint8_t mode);
typedef int (*wifi_auto_connect_get_ap_num_fp_t)(uint8_t *num);
typedef int (*wifi_auto_connect_set_ap_num_fp_t)(uint8_t num);
typedef int (*wifi_auto_connect_get_ap_info_fp_t)(uint8_t index, wifi_auto_connect_info_t *info);
typedef int (*wifi_auto_connect_clear_ap_info_fp_t)(uint8_t index);
typedef int (*wifi_auto_connect_init_fp_t)(void);
typedef int (*wifi_auto_connect_start_fp_t)(void);
typedef int (*wifi_auto_connect_reset_fp_t)(void);
typedef int (*wifi_auto_connect_get_saved_ap_num_fp_t)(uint8_t *num);
typedef int (*wifi_auto_connect_update_ch_fp_t)(uint8_t ac_index, uint8_t channel);
typedef int (*wifi_fast_connect_get_mode_fp_t)(uint8_t ap_index, uint8_t *mode);
typedef int (*wifi_fast_connect_set_mode_fp_t)(uint8_t ap_index, uint8_t mode);
typedef int (*wifi_fast_connect_start_fp_t)(uint8_t ap_index);
typedef int (*wifi_connection_connect_from_ac_list_fp_t) (wifi_config_t *config);
typedef int (*wifi_connection_connect_from_ac_index_fp_t) (uint8_t index);
typedef int (*wifi_config_get_mac_tx_data_rate_fp_t) (wifi_mac_data_rate_t *data_rate);
typedef int (*wifi_config_set_mac_tx_data_rate_fp_t) (wifi_mac_data_rate_t data_rate);

/* Export interface funtion pointer */
extern wifi_convert_auth_mode_fp_t                         wifi_convert_auth_mode_api;

extern wifi_init_fp_t                                      wifi_init_api;
extern wifi_deinit_fp_t                                    wifi_deinit_api;
extern wifi_set_config_fp_t                                wifi_set_config_api;
extern wifi_get_config_fp_t                                wifi_get_config_api;
extern wifi_start_fp_t                                     wifi_start_api;
extern wifi_stop_fp_t                                      wifi_stop_api;
extern wifi_scan_start_fp_t                                wifi_scan_start_api;
extern wifi_scan_stop_fp_t                                 wifi_scan_stop_api;
extern wifi_scan_get_ap_list_fp_t                          wifi_scan_get_ap_list_api;
extern wifi_scan_get_ap_num_fp_t                           wifi_scan_get_ap_num_api;
extern wifi_scan_get_ap_records_fp_t                       wifi_scan_get_ap_records_api;
extern wifi_sta_get_ap_info_fp_t                           wifi_sta_get_ap_info_api;
extern wifi_connection_disconnect_ap_fp_t                  wifi_connection_disconnect_ap_api;
extern wifi_connection_disconnect_sta_fp_t                 wifi_connection_disconnect_sta_api;
extern wifi_connection_connect_fp_t                        wifi_connection_connect_api;
extern wifi_connection_scan_start_fp_t                     wifi_connection_scan_start_api;
extern wifi_connection_get_rssi_fp_t                       wifi_connection_get_rssi_api;
extern wifi_connection_register_event_handler_fp_t         wifi_connection_register_event_handler_api;
extern wifi_connection_unregister_event_handler_fp_t       wifi_connection_unregister_event_handler_api;
extern wifi_config_set_opmode_fp_t                         wifi_config_set_opmode_api;
extern wifi_config_get_opmode_fp_t                         wifi_config_get_opmode_api;
extern wifi_config_get_mac_address_fp_t                    wifi_config_get_mac_address_api;
extern wifi_config_set_mac_address_fp_t                    wifi_config_set_mac_address_api;
extern wifi_config_get_sta_mac_address_from_flash_fp_t     wifi_config_get_sta_mac_address_from_flash_api;
extern wifi_config_get_ssid_fp_t                           wifi_config_get_ssid_api;
extern wifi_config_set_ssid_fp_t                           wifi_config_set_ssid_api;
extern wifi_config_get_bssid_fp_t                          wifi_config_get_bssid_api;
extern wifi_config_set_bssid_fp_t                          wifi_config_set_bssid_api;
extern wifi_config_get_channel_fp_t                        wifi_config_get_channel_api;
extern wifi_config_set_channel_fp_t                        wifi_config_set_channel_api;
extern wifi_config_set_bandwidth_fp_t                      wifi_config_set_bandwidth_api;
extern wifi_config_get_bandwidth_fp_t                      wifi_config_get_bandwidth_api;
extern wifi_config_get_dtim_interval_fp_t                  wifi_config_get_dtim_interval_api;
extern wifi_config_set_dtim_interval_fp_t                  wifi_config_set_dtim_interval_api;
extern wifi_config_get_listen_interval_fp_t                wifi_config_get_listen_interval_api;
extern wifi_config_set_listen_interval_fp_t                wifi_config_set_listen_interval_api;
extern wifi_config_get_skip_dtim_fp_t                      wifi_config_get_skip_dtim_api;
extern wifi_config_set_skip_dtim_fp_t                      wifi_config_set_skip_dtim_api;
extern wifi_auto_connect_get_mode_fp_t                     wifi_auto_connect_get_mode_api;
extern wifi_auto_connect_set_mode_fp_t                     wifi_auto_connect_set_mode_api;
extern wifi_auto_connect_get_ap_num_fp_t                   wifi_auto_connect_get_ap_num_api;
extern wifi_auto_connect_set_ap_num_fp_t                   wifi_auto_connect_set_ap_num_api;
extern wifi_auto_connect_get_ap_info_fp_t                  wifi_auto_connect_get_ap_info_api;
extern wifi_auto_connect_clear_ap_info_fp_t                wifi_auto_connect_clear_ap_info_api;
extern wifi_auto_connect_init_fp_t                         wifi_auto_connect_init_api;
extern wifi_auto_connect_start_fp_t                        wifi_auto_connect_start_api;
extern wifi_auto_connect_reset_fp_t                        wifi_auto_connect_reset_api;
extern wifi_auto_connect_get_saved_ap_num_fp_t             wifi_auto_connect_get_saved_ap_num_api;
extern wifi_auto_connect_update_ch_fp_t                    wifi_auto_connect_update_ch_api;
extern wifi_fast_connect_get_mode_fp_t                     wifi_fast_connect_get_mode_api;
extern wifi_fast_connect_set_mode_fp_t                     wifi_fast_connect_set_mode_api;
extern wifi_fast_connect_start_fp_t                        wifi_fast_connect_start_api;
extern wifi_connection_connect_from_ac_list_fp_t           wifi_connection_connect_from_ac_list_api;
extern wifi_connection_connect_from_ac_index_fp_t          wifi_connection_connect_from_ac_index_api;
extern wifi_config_get_mac_tx_data_rate_fp_t               wifi_config_get_mac_tx_data_rate_api;
extern wifi_config_set_mac_tx_data_rate_fp_t               wifi_config_set_mac_tx_data_rate_api;

#ifdef __cplusplus
}
#endif

#endif /* __WIFI_API_IF_H__ */

