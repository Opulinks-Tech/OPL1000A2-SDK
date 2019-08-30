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
#ifndef _MW_FIM_DEFAULT_GROUP08_PROJECT_H_
#define _MW_FIM_DEFAULT_GROUP08_PROJECT_H_

#ifdef __cplusplus
extern "C" {
#endif

// Sec 0: Comment block of the file


// Sec 1: Include File
#include "mw_fim.h"
#include "mw_fim_default_group08.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
// the file ID
// xxxx_xxxx_xxxx_xxxx_xxxx_xxxx_xxxx_xxxx
// ^^^^ ^^^^ Zone (0~3)
//           ^^^^ ^^^^ Group (0~8), 0 is reserved for swap
//                     ^^^^ ^^^^ ^^^^ ^^^^ File ID, start from 0
typedef enum
{
    MW_FIM_IDX_GP08_PROJECT_START = 0x00080000,             // the start IDX of group 08

    MW_FIM_IDX_GP08_PROJECT_POWER_SAVING,
    
    MW_FIM_IDX_GP08_PROJECT_BLE_SERVICE_UUID,
    MW_FIM_IDX_GP08_PROJECT_BLE_DEVICE_NAME,
    MW_FIM_IDX_GP08_PROJECT_BLE_ADV_INTERVAL,

    MW_FIM_IDX_GP08_PROJECT_WIFI_CONNECT_SETTINGS,
    
    MW_FIM_IDX_GP08_PROJECT_MAX
} E_MwFimIdxGroup08_Project;


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, uniou, enum, linked list
// Power saving
typedef struct
{
    uint8_t ubPowerSaving;
    uint8_t ubRFPower;
    uint8_t ubaReserved[2];
} T_MwFim_GP08_PowerSaving;

#define MW_FIM_GP08_POWER_SAVING_SIZE   sizeof(T_MwFim_GP08_PowerSaving)
#define MW_FIM_GP08_POWER_SAVING_NUM    1

// BLE service uuid
typedef struct
{
    uint16_t uwServiceUUID;
    uint16_t uwDataInUUID;
    uint16_t uwDataOutUUID;
    uint8_t ubaReserved[2];
} T_MwFim_GP08_BleServiceUUID;

#define MW_FIM_GP08_BLE_SERVICE_UUID_SIZE   sizeof(T_MwFim_GP08_BleServiceUUID)
#define MW_FIM_GP08_BLE_SERVICE_UUID_NUM    1

// BLE device name
typedef struct
{
    uint8_t ubNameMethod;
    uint8_t ubNamePostfixMacCount;
    uint8_t ubaNamePrefix[30];
    uint8_t ubaNameFull[30];
    uint8_t ubaReserved[2];
} T_MwFim_GP08_BleDeviceName;

#define MW_FIM_GP08_BLE_DEVICE_NAME_SIZE   sizeof(T_MwFim_GP08_BleDeviceName)
#define MW_FIM_GP08_BLE_DEVICE_NAME_NUM    1

// BLE advertisement interval
typedef struct
{
    uint16_t uwIntervalMin;
    uint16_t uwIntervalMax;
} T_MwFim_GP08_BleAdvInterval;

#define MW_FIM_GP08_BLE_ADV_INTERVAL_SIZE   sizeof(T_MwFim_GP08_BleAdvInterval)
#define MW_FIM_GP08_BLE_ADV_INTERVAL_NUM    1

// Wifi connect settings
typedef struct
{
    uint32_t ulAutoConnectIntervalInit;
    uint32_t ulAutoConnectIntervalDiff;
    uint32_t ulAutoConnectIntervalMax;
    uint32_t ulDtimInterval;
    uint8_t ubConnectRetry;
    uint8_t ubaReserved[3];
} T_MwFim_GP08_WifiConnectSettings;

#define MW_FIM_GP08_WIFI_CONNECT_SETTINGS_SIZE  sizeof(T_MwFim_GP08_WifiConnectSettings)
#define MW_FIM_GP08_WIFI_CONNECT_SETTINGS_NUM   1


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern const T_MwFimFileInfo g_taMwFimGroupTable08_project[];

extern const T_MwFim_GP08_PowerSaving g_tMwFimDefaultGp08PowerSaving;
extern const T_MwFim_GP08_BleServiceUUID g_tMwFimDefaultGp08BleServiceUUID;
extern const T_MwFim_GP08_BleDeviceName g_tMwFimDefaultGp08BleDeviceName;
extern const T_MwFim_GP08_BleAdvInterval g_tMwFimDefaultGp08BleAdvInterval;
extern const T_MwFim_GP08_WifiConnectSettings g_tMwFimDefaultGp08WifiConnectSettings;

// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


#ifdef __cplusplus
}
#endif

#endif // _MW_FIM_DEFAULT_GROUP08_PROJECT_H_
