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

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include "mw_fim_default_group11.h"
#include "mw_fim_default_group11_project.h"
#include "blewifi_configuration.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
// the default value of power saving
const T_MwFim_GP11_PowerSaving g_tMwFimDefaultGp11PowerSaving =
{
    BLEWIFI_COM_POWER_SAVE_EN,      // uint8_t ubPowerSaving;
    BLEWIFI_COM_RF_POWER_SETTINGS,  // uint8_t ubRFPower;
    0xFF, 0xFF                      // uint8_t ubaReserved[2];
};
// the address buffer of power saving
uint32_t g_ulaMwFimAddrBufferGp11PowerSaving[MW_FIM_GP11_POWER_SAVING_NUM];

// the default value of BLE service UUID
const T_MwFim_GP11_BleServiceUUID g_tMwFimDefaultGp11BleServiceUUID =
{
    BLEWIFI_BLE_UUID_SERVICE,   // uint16_t uwServiceUUID;
    BLEWIFI_BLE_UUID_DATA_IN,   // uint16_t uwDataInUUID;
    BLEWIFI_BLE_UUID_DATA_OUT,  // uint16_t uwDataOutUUID;
    0xFF, 0xFF                  // uint8_t ubaReserved[2];
};
// the address buffer of BLE service UUID
uint32_t g_ulaMwFimAddrBufferGp11BleServiceUUID[MW_FIM_GP11_BLE_SERVICE_UUID_NUM];

// the default value of BLE device name
const T_MwFim_GP11_BleDeviceName g_tMwFimDefaultGp11BleDeviceName =
{
    .ubNameMethod           = BLEWIFI_BLE_DEVICE_NAME_METHOD,
    .ubNamePostfixMacCount  = BLEWIFI_BLE_DEVICE_NAME_POST_COUNT,
    .ubaNamePrefix          = BLEWIFI_BLE_DEVICE_NAME_PREFIX,
    .ubaNameFull            = BLEWIFI_BLE_DEVICE_NAME_FULL,
    .ubaReserved            = {0xFF, 0xFF}
};
// the address buffer of BLE device name
uint32_t g_ulaMwFimAddrBufferGp11BleDeviceName[MW_FIM_GP11_BLE_DEVICE_NAME_NUM];

// the default value of BLE advertisement interval
const T_MwFim_GP11_BleAdvInterval g_tMwFimDefaultGp11BleAdvInterval =
{
    BLEWIFI_BLE_ADVERTISEMENT_INTERVAL_MIN, // uint16_t uwIntervalMin;
    BLEWIFI_BLE_ADVERTISEMENT_INTERVAL_MAX  // uint16_t uwIntervalMax;
};
// the address buffer of BLE advertisement interval
uint32_t g_ulaMwFimAddrBufferGp11BleAdvInterval[MW_FIM_GP11_BLE_ADV_INTERVAL_NUM];

// the default value of Wifi connect settings
const T_MwFim_GP11_WifiConnectSettings g_tMwFimDefaultGp11WifiConnectSettings =
{
    BLEWIFI_WIFI_AUTO_CONNECT_INTERVAL_INIT,    // uint32_t ulAutoConnectIntervalInit;
    BLEWIFI_WIFI_AUTO_CONNECT_INTERVAL_DIFF,    // uint32_t ulAutoConnectIntervalDiff;
    BLEWIFI_WIFI_AUTO_CONNECT_INTERVAL_MAX,     // uint32_t ulAutoConnectIntervalMax;
    BLEWIFI_WIFI_DTIM_INTERVAL,                 // uint32_t ulDtimInterval;
    BLEWIFI_WIFI_REQ_CONNECT_RETRY_TIMES,       // uint8_t ubConnectRetry;
    0xFF, 0xFF, 0xFF                            // uint8_t ubaReserved[3];
};
// the address buffer of Wifi connect settings
uint32_t g_ulaMwFimAddrBufferGp11WifiConnectSettings[MW_FIM_GP11_WIFI_CONNECT_SETTINGS_NUM];

// the information table of group 11
const T_MwFimFileInfo g_taMwFimGroupTable11_project[] =
{
    {MW_FIM_IDX_GP11_PROJECT_POWER_SAVING,          MW_FIM_GP11_POWER_SAVING_NUM,           MW_FIM_GP11_POWER_SAVING_SIZE,          (uint8_t*)&g_tMwFimDefaultGp11PowerSaving,          g_ulaMwFimAddrBufferGp11PowerSaving},
    {MW_FIM_IDX_GP11_PROJECT_BLE_SERVICE_UUID,      MW_FIM_GP11_BLE_SERVICE_UUID_NUM,       MW_FIM_GP11_BLE_SERVICE_UUID_SIZE,      (uint8_t*)&g_tMwFimDefaultGp11BleServiceUUID,       g_ulaMwFimAddrBufferGp11BleServiceUUID},
    {MW_FIM_IDX_GP11_PROJECT_BLE_DEVICE_NAME,       MW_FIM_GP11_BLE_DEVICE_NAME_NUM,        MW_FIM_GP11_BLE_DEVICE_NAME_SIZE,       (uint8_t*)&g_tMwFimDefaultGp11BleDeviceName,        g_ulaMwFimAddrBufferGp11BleDeviceName},
    {MW_FIM_IDX_GP11_PROJECT_BLE_ADV_INTERVAL,      MW_FIM_GP11_BLE_ADV_INTERVAL_NUM,       MW_FIM_GP11_BLE_ADV_INTERVAL_SIZE,      (uint8_t*)&g_tMwFimDefaultGp11BleAdvInterval,       g_ulaMwFimAddrBufferGp11BleAdvInterval},
    {MW_FIM_IDX_GP11_PROJECT_WIFI_CONNECT_SETTINGS, MW_FIM_GP11_WIFI_CONNECT_SETTINGS_NUM,  MW_FIM_GP11_WIFI_CONNECT_SETTINGS_SIZE, (uint8_t*)&g_tMwFimDefaultGp11WifiConnectSettings,  g_ulaMwFimAddrBufferGp11WifiConnectSettings},

    // the end, don't modify and remove it
    {0xFFFFFFFF,            0x00,              0x00,               NULL,                            NULL}
};


// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


/***********
C Functions
***********/
// Sec 8: C Functions
