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

#ifndef __BLEWIFI_CONFIGURATION_H__
#define __BLEWIFI_CONFIGURATION_H__

// Common part
/*
FIM version
*/
#define MW_FIM_VER08_PROJECT            0x04    // 0x00 ~ 0xFF

/*
Smart sleep
*/
#define BLEWIFI_COM_POWER_SAVE_EN       (0)     // 1: enable    0: disable

/*
RF Power

.-----------------.----------------.----------------.
|                 |  BLE Low Power | BLE High Power |
:-----------------+----------------+----------------:
| WIFI Low power  |  0x00          | 0x0F           |
:-----------------+----------------+----------------:
| WIFI High power |  0xF0          | 0xFF           |
'-----------------'----------------'----------------'
*/
#define BLEWIFI_COM_RF_POWER_SETTINGS   (0xF0)

/*
SNTP
*/
#define SNTP_FUNCTION_EN      (0)                   // SNTP 1: enable / 0: disable
#define SNTP_SERVER           "1.cn.pool.ntp.org"   // SNTP Server 
#define SNTP_PORT_NUM         "123"                 // SNTP port Number
#define SNTP_TIME_ZONE        (8)                   // Time zone: GMT+8

/* 
BLE OTA FLAG 
*/
#define BLE_OTA_FUNCTION_EN      (1)  // BLE OTA Function Enable (1: enable / 0: disable)

/* 
WIFI OTA FLAG 
*/
#define WIFI_OTA_FUNCTION_EN     (1)  // WIFI OTA Function Enable (1: enable / 0: disable)
#define WIFI_OTA_HTTP_URL        "http://192.168.0.100/ota.bin"

/*
IoT device
    1. if want to send data to server, set the Tx path to enable
    2. if want to receive data from server, set the Rx path to enable
*/
#define IOT_DEVICE_DATA_TX_EN               (0)     // 1: enable / 0: disable
#define IOT_DEVICE_DATA_RX_EN               (0)     // 1: enable / 0: disable
#define IOT_DEVICE_DATA_TASK_STACK_SIZE_TX  (1024)
#define IOT_DEVICE_DATA_TASK_STACK_SIZE_RX  (1024)
#define IOT_DEVICE_DATA_QUEUE_SIZE_TX       (20)


// BLE part
/*
BLE Service UUID
*/
#define BLEWIFI_BLE_UUID_SERVICE        0xAAAA
#define BLEWIFI_BLE_UUID_DATA_IN        0xBBB0
#define BLEWIFI_BLE_UUID_DATA_OUT       0xBBB1

/* Device Name
method 1: use prefix + mac address
    The max length of prefix is 17 bytes.
    The length of mac address is 12 bytes.
    
    Ex: OPL_33:44:55:66
    
method 2: full name
    The max length of device name is 29 bytes.
*/
#define BLEWIFI_BLE_DEVICE_NAME_METHOD      1           // 1 or 2
#define BLEWIFI_BLE_DEVICE_NAME_POST_COUNT  4           // for method 1 "OPL_33:44:55:66"
#define BLEWIFI_BLE_DEVICE_NAME_PREFIX      "OPL_"      // for method 1 "OPL_33:44:55:66"
#define BLEWIFI_BLE_DEVICE_NAME_FULL        "OPL1000"   // for method 2

/* Advertisement Interval Calculation Method:
1000 (ms) / 0.625 (ms) = 1600 = 0x640
*/
#define BLEWIFI_BLE_ADVERTISEMENT_INTERVAL_MIN  0x640  // For 1 Second
#define BLEWIFI_BLE_ADVERTISEMENT_INTERVAL_MAX  0x640  // For 1 Second


// Wifi part
/* Connection Retry times:
When BLE send connect reqest for connecting AP.
If failure will retry times which define below.
*/
#define BLEWIFI_WIFI_REQ_CONNECT_RETRY_TIMES    5


/* Auto Connection Interval:
if the auto connection is fail, the interval will be increased
    Ex: Init 5000, Diff 1000, Max 30000
        1st:  5000 ms
        2nd:  6000 ms
            ...
        26th: 30000 ms
        27th: 30000 ms
            ...
*/
#define BLEWIFI_WIFI_AUTO_CONNECT_INTERVAL_INIT     (5000)      // ms
#define BLEWIFI_WIFI_AUTO_CONNECT_INTERVAL_DIFF     (1000)      // ms
#define BLEWIFI_WIFI_AUTO_CONNECT_INTERVAL_MAX      (30000)     // ms

/* DTIM the times of Interval: ms
*/
#define BLEWIFI_WIFI_DTIM_INTERVAL                  (2000)      // ms

#endif /* __BLEWIFI_CONFIGURATION_H__ */

