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

/**
 * @file blewifi_data.h
 * @author Michael Liao
 * @date 20 Feb 2018
 * @brief File includes the function declaration of blewifi task.
 *
 */

#ifndef __BLEWIFI_DATA_H__
#define __BLEWIFI_DATA_H__

#include <stdint.h>
#include <stdbool.h>


enum
{
	BLEWIFI_OTA_SUCCESS,
	BLEWIFI_OTA_ERR_NOT_ACTIVE,
	BLEWIFI_OTA_ERR_HW_FAILURE,
	BLEWIFI_OTA_ERR_IN_PROGRESS,
	BLEWIFI_OTA_ERR_INVALID_LEN,
	BLEWIFI_OTA_ERR_CHECKSUM,
	BLEWIFI_OTA_ERR_MEM_CAPACITY_EXCEED,


};

typedef enum {
    BLEWIFI_REQ_SCAN = 0,                   // Wifi scan
    BLEWIFI_REQ_CONNECT,                    // Wifi connect
    BLEWIFI_REQ_DISCONNECT,                 // Wifi disconnect
    BLEWIFI_REQ_RECONNECT,                  // Wifi reconnect
    BLEWIFI_REQ_READ_DEVICE_INFO,           // Wifi read device information
    BLEWIFI_REQ_WRITE_DEVICE_INFO,          // Wifi write device information
    BLEWIFI_REQ_WIFI_STATUS,                // Wifi read AP status
    BLEWIFI_REQ_RESET,                      // Wifi reset AP
    
    BLEWIFI_REQ_OTA_VERSION = 0x100,        // Ble OTA
    BLEWIFI_REQ_OTA_UPGRADE,                // Ble OTA
    BLEWIFI_REQ_OTA_RAW,                    // Ble OTA
    BLEWIFI_REQ_OTA_END,                    // Ble OTA

    BLEWIFI_REQ_HTTP_OTA_TRIG = 0x200,      // Wifi OTA
    BLEWIFI_REQ_HTTP_OTA_DEVICE_VERSION,    // Wifi OTA
    BLEWIFI_REQ_HTTP_OTA_SERVER_VERSION,    // Wifi OTA

    BLEWIFI_REQ_MP_START = 0x400,
    BLEWIFI_REQ_MP_CAL_VBAT,
    BLEWIFI_REQ_MP_CAL_IO_VOLTAGE,
    BLEWIFI_REQ_MP_CAL_TMPR,
    BLEWIFI_REQ_MP_SYS_MODE_WRITE,
    BLEWIFI_REQ_MP_SYS_MODE_READ,

    BLEWIFI_REQ_ENG_START = 0x600,
    BLEWIFI_REQ_ENG_SYS_RESET,
    BLEWIFI_REQ_ENG_WIFI_MAC_WRITE,
    BLEWIFI_REQ_ENG_WIFI_MAC_READ,
    BLEWIFI_REQ_ENG_BLE_MAC_WRITE,
    BLEWIFI_REQ_ENG_BLE_MAC_READ,
    BLEWIFI_REQ_ENG_BLE_CMD,

    BLEWIFI_REQ_APP_START = 0x800,

    BLEWIFI_RSP_SCAN_REPORT = 0x1000,
    BLEWIFI_RSP_SCAN_END,
    BLEWIFI_RSP_CONNECT,
    BLEWIFI_RSP_DISCONNECT,
    BLEWIFI_RSP_RECONNECT,
    BLEWIFI_RSP_READ_DEVICE_INFO,
    BLEWIFI_RSP_WRITE_DEVICE_INFO,
    BLEWIFI_RSP_WIFI_STATUS,
    BLEWIFI_RSP_RESET,
    
    BLEWIFI_RSP_OTA_VERSION = 0x1100,
    BLEWIFI_RSP_OTA_UPGRADE,
    BLEWIFI_RSP_OTA_RAW,
    BLEWIFI_RSP_OTA_END,

    BLEWIFI_RSP_HTTP_OTA_TRIG = 0x1200,
    BLEWIFI_RSP_HTTP_OTA_DEVICE_VERSION,
    BLEWIFI_RSP_HTTP_OTA_SERVER_VERSION,

    BLEWIFI_RSP_MP_START = 0x1400,
    BLEWIFI_RSP_MP_CAL_VBAT,
    BLEWIFI_RSP_MP_CAL_IO_VOLTAGE,
    BLEWIFI_RSP_MP_CAL_TMPR,
    BLEWIFI_RSP_MP_SYS_MODE_WRITE,
    BLEWIFI_RSP_MP_SYS_MODE_READ,

    BLEWIFI_RSP_ENG_START = 0x1600,
    BLEWIFI_RSP_ENG_SYS_RESET,
    BLEWIFI_RSP_ENG_WIFI_MAC_WRITE,
    BLEWIFI_RSP_ENG_WIFI_MAC_READ,
    BLEWIFI_RSP_ENG_BLE_MAC_WRITE,
    BLEWIFI_RSP_ENG_BLE_MAC_READ,
    BLEWIFI_RSP_ENG_BLE_CMD,

    BLEWIFI_RSP_APP_START = 0x1800,

    BLEWIFI_IND_IP_STATUS_NOTIFY = 0x2000,  // Wifi notify AP status

    BLEWIFI_TYPE_END = 0xFFFF
}blewifi_type_id_e;

/* BLEWIF protocol */
typedef struct blewifi_hdr_tag
{
    uint16_t type;
    uint16_t data_len;
    uint8_t  data[]; //variable size
}blewifi_hdr_t;

typedef void (*T_BleWifi_Ble_ProtocolHandler_Fp)(uint16_t type, uint8_t *data, int len);
typedef struct
{
    uint32_t ulEventId;
    T_BleWifi_Ble_ProtocolHandler_Fp fpFunc;
} T_BleWifi_Ble_ProtocolHandlerTbl;

void BleWifi_Ble_DataRecvHandler(uint8_t *data, int len);
void BleWifi_Ble_DataSendEncap(uint16_t type_id, uint8_t *data, int total_data_len);
void BleWifi_Ble_SendResponse(uint16_t type_id, uint8_t status);

void BleWifi_Wifi_OtaTrigReq(uint8_t *data);
void BleWifi_Wifi_OtaTrigRsp(uint8_t status);
void BleWifi_Wifi_OtaDeviceVersionReq(void);
void BleWifi_Wifi_OtaDeviceVersionRsp(uint16_t fid);
void BleWifi_Wifi_OtaServerVersionReq(void);
void BleWifi_Wifi_OtaServerVersionRsp(uint16_t fid);

#endif /* __BLEWIFI_DATA_H__ */

