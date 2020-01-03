/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

#ifndef __BLEWIFI_WIFI_API_H__
#define __BLEWIFI_WIFI_API_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "wifi_types.h"
#include "wifi_event.h"

#ifdef __cplusplus
extern "C" {
#endif


#define BLEWIFI_MANUFACTURER_NAME_LEN 32

/** @brief This enumeration defines the wireless authentication mode to indicate the Wi-Fi device authentication attribute.
*/
typedef enum {
    BLEWIFI_AUTH_OPEN = 0,             /**< authenticate mode : open */
    BLEWIFI_AUTH_WEP,                  /**< authenticate mode : WEP */
    BLEWIFI_AUTH_WPA_PSK,              /**< authenticate mode : WPA_PSK */
    BLEWIFI_AUTH_WPA2_PSK,             /**< authenticate mode : WPA2_PSK */
    BLEWIFI_AUTH_WPA_WPA2_PSK,         /**< authenticate mode : WPA_WPA2_PSK */
} blewifi_auth_mode_t;

/** @brief This enumeration defines wireless security cipher suits.
*/
typedef enum {
    BLEWIFI_CIPHER_TYPE_NONE = 0,      /**< 0, the cipher type is none */
    BLEWIFI_CIPHER_TYPE_WEP40,         /**< 1, the cipher type is WEP40 */
    BLEWIFI_CIPHER_TYPE_WEP104,        /**< 2, the cipher type is WEP104 */
    BLEWIFI_CIPHER_TYPE_TKIP,          /**< 3, the cipher type is TKIP */
    BLEWIFI_CIPHER_TYPE_CCMP,          /**< 4, the cipher type is CCMP */
    BLEWIFI_CIPHER_TYPE_TKIP_CCMP,     /**< 5, the cipher type is TKIP and CCMP */
    BLEWIFI_CIPHER_TYPE_UNKNOWN,       /**< 6, the cipher type is unknown */
} blewifi_cipher_type_t;

/** @brief This enumeration defines the wireless STA scan type
*/
typedef enum {
    BLEWIFI_SCAN_TYPE_ACTIVE = 0,      /**< Actively scan a network by sending 802.11 probe(s)         */
    BLEWIFI_SCAN_TYPE_PASSIVE,         /**< Passively scan a network by listening for beacons from APs */
} blewifi_scan_type_t;

typedef struct {
    uint8_t show_hidden;                 /**< enable/disable to scan AP whose SSID is hidden */
    uint8_t scan_type;                   /**< scan type, active or passive */
} blewifi_cmd_scan_t;

typedef struct {
    uint8_t bssid[WIFI_MAC_ADDRESS_LENGTH];   /**< The MAC address of the target AP. */
    uint8_t password_length;                  /**< The length of the password. */
    uint8_t password[WIFI_LENGTH_PASSPHRASE]; /**< The password of the target AP. */
} blewifi_cmd_connect_t;

/** @brief This structure defines the information of scanned APs
*/
typedef struct {
    uint8_t  ssid_length;                      /**< Length of the SSID. */
    uint8_t  ssid[WIFI_MAX_LENGTH_OF_SSID];    /**< Stores the predefined SSID. */
    uint8_t  bssid[WIFI_MAC_ADDRESS_LENGTH];   /**< AP's MAC address. */
    uint8_t  auth_mode;                        /**< Please refer to the definition of #wifi_auth_mode_t. */
    int8_t   rssi;                             /**< Records the RSSI value when probe response is received. */
    uint8_t  connected;                        /**< AP was connected before. */
}__attribute__((packed)) blewifi_scan_info_t;

/** @brief This structure defines the information of device
*/
typedef struct {
    uint8_t  device_id[WIFI_MAC_ADDRESS_LENGTH];                /**< Stores the predefined device ID (MAC). */
    uint8_t  name_len;                                          /**< Length of manufacturer . */
    uint8_t  manufacturer_name[BLEWIFI_MANUFACTURER_NAME_LEN];  /**< manufacturer name */
}__attribute__((packed)) blewifi_device_info_t;

/** @brief This structure defines the information of Wi-Fi status
*/
typedef struct {
    uint8_t  status;                                  /**< Length of the SSID. */
    uint8_t  ssid_length;                             /**< Length of the SSID. */
    uint8_t  ssid[WIFI_MAX_LENGTH_OF_SSID];           /**< Stores the predefined SSID. */
    uint8_t  bssid[WIFI_MAC_ADDRESS_LENGTH];          /**< AP's MAC address. */
    unsigned long   IP;                               /**< Device's IP. */
    unsigned long   mask;                             /**< Device's Mask. */
    unsigned long   Gateway;                          /**< Device's Gateway. */     
    unsigned long   DNS;                              /**< Device's DNS. */
}__attribute__((packed)) blewifi_wifi_status_info_t;

typedef int (*T_BleWifi_Wifi_EventHandler_Fp)(wifi_event_id_t event_id, void *data, uint16_t length);
typedef struct
{
    uint32_t ulEventId;
    T_BleWifi_Wifi_EventHandler_Fp fpFunc;
} T_BleWifi_Wifi_EventHandlerTbl;

#define BLEWIFI_WIFI_CONNECTED_DONE     0
#define BLEWIFI_WIFI_CONNECTED_FAIL     1
#define BLEWIFI_WIFI_DISCONNECTED_DONE  0
#define BLEWIFI_WIFI_DISCONNECTED_FAIL  1


void BleWifi_Wifi_DoScan(uint8_t *data, int len);
void BleWifi_Wifi_DoConnect(uint8_t *data, int len);
void BleWifi_Wifi_DoDisconnect(void);
void BleWifi_Wifi_ReadDeviceInfo(void);
void BleWifi_Wifi_WriteDeviceInfo(uint8_t *data, int len);
void BleWifi_Wifi_SendStatusInfo(uint16_t uwType);
void BleWifi_Wifi_ResetRecord(void);
void BleWifi_Wifi_MacAddrWrite(uint8_t *data, int len);
void BleWifi_Wifi_MacAddrRead(uint8_t *data, int len);

int BleWifi_Wifi_SendScanReport(void);
int BleWifi_Wifi_UpdateScanInfoToAutoConnList(void);
uint8_t BleWifi_Wifi_AutoConnectListNum(void);
void BleWifi_Wifi_DoAutoConnect(void);
void BleWifi_Wifi_ReqConnectRetry(void);
int BleWifi_Wifi_Rssi(int8_t *rssi);
int BleWifi_Wifi_SetDTIM(uint32_t value);
void BleWifi_Wifi_UpdateBeaconInfo(void);
    
int BleWifi_Wifi_EventHandlerCb(wifi_event_id_t event_id, void *data, uint16_t length);
void BleWifi_Wifi_Init(void);

#ifdef __cplusplus
}
#endif

#endif  // end of __BLEWIFI_WIFI_API_H__
