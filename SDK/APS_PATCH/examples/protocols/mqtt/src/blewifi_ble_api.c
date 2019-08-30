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

#include "blewifi_ble_api.h"
#include "blewifi_server_app.h"
#include "blewifi_data.h"
#include "sys_common_api.h"

/******************************************************
 *                      Macros
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
 *               Static Function Declarations
 ******************************************************/

/******************************************************
 *               Variable Definitions
 ******************************************************/

/******************************************************
 *               Function Definitions
 ******************************************************/
void BleWifi_Ble_Init(void)
{
    /* BLE Init Step 1: Do BLE initialization first */
    BleWifi_Ble_ServerAppInit();
}

void BleWifi_Ble_StartAdvertising(void)
{
    /* Call BLE Stack API to do ble advertising */
    BleWifi_Ble_SendAppMsgToBle(BLEWIFI_APP_MSG_ENTER_ADVERTISING, 0, NULL);
}

void BleWifi_Ble_StopAdvertising(void)
{
    /* Call BLE Stack API to stop ble advertising */
    BleWifi_Ble_SendAppMsgToBle(BLEWIFI_APP_MSG_EXIT_ADVERTISING, 0, NULL);
}

void BleWifi_Ble_AdvertisingTimeChange(uint16_t interval_min, uint16_t interval_max)
{
    BLE_ADV_TIME_T adv_time;
    
    /* Call BLE Stack API to change advertising time */
    adv_time.interval_min = interval_min;
    adv_time.interval_max = interval_max;
    BleWifi_Ble_SendAppMsgToBle(BLEWIFI_APP_MSG_CHANGE_ADVERTISING_TIME, sizeof(BLE_ADV_TIME_T), (uint8_t *)&adv_time);
}

void BleWifi_Ble_MacAddrWrite(uint8_t *data, int len)
{
    uint8_t ubaMacAddr[6];

    memcpy(&ubaMacAddr[5], &data[0], 1);
    memcpy(&ubaMacAddr[4], &data[1], 1);
    memcpy(&ubaMacAddr[3], &data[2], 1);
    memcpy(&ubaMacAddr[2], &data[3], 1);
    memcpy(&ubaMacAddr[1], &data[4], 1);
    memcpy(&ubaMacAddr[0], &data[5], 1);

    ble_set_config_bd_addr(ubaMacAddr);
    BleWifi_Ble_SendResponse(BLEWIFI_RSP_ENG_BLE_MAC_WRITE, 0);
}

void BleWifi_Ble_MacAddrRead(uint8_t *data, int len)
{
    uint8_t ubaMacAddr[6];
    uint8_t ubaMacAddrReOrder[6];

    ble_get_config_bd_addr(ubaMacAddr);

    ubaMacAddrReOrder[5] = ubaMacAddr[0];
    ubaMacAddrReOrder[4] = ubaMacAddr[1];
    ubaMacAddrReOrder[3] = ubaMacAddr[2];
    ubaMacAddrReOrder[2] = ubaMacAddr[3];
    ubaMacAddrReOrder[1] = ubaMacAddr[4];
    ubaMacAddrReOrder[0] = ubaMacAddr[5];
    
    BleWifi_Ble_DataSendEncap(BLEWIFI_RSP_ENG_BLE_MAC_READ, ubaMacAddrReOrder, 6);
}
