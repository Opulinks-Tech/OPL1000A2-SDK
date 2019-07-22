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

/******************************************************************************
*  Filename:
*  ---------
*  mw_fim_default_group03.h
*
*  Project:
*  --------
*  OPL1000 Project - the Flash Item Management (FIM) definition file
*
*  Description:
*  ------------
*  This include file is the Flash Item Management (FIM) definition file
*
*  Author:
*  -------
*  Jeff Kuo
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
#ifndef _MW_FIM_DEFAULT_GROUP03_H_
#define _MW_FIM_DEFAULT_GROUP03_H_

#ifdef __cplusplus
extern "C" {
#endif

// Sec 0: Comment block of the file


// Sec 1: Include File
#include "mw_fim.h"
#include "hal_auxadc.h"
#include "hal_temperature.h"
#include "sys_common_ctrl.h"
#include "controller_wifi_com.h"
#include "le_ctrl.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
// the file ID
// xxxx_xxxx_xxxx_xxxx_xxxx_xxxx_xxxx_xxxx
// ^^^^ ^^^^ Zone (0~3)
//           ^^^^ ^^^^ Group (0~8), 0 is reserved for swap
//                     ^^^^ ^^^^ ^^^^ ^^^^ File ID, start from 0
typedef enum
{
    MW_FIM_IDX_GP03_START = 0x00030000,             // the start IDX of group 03
    MW_FIM_IDX_GP03_CAL_AUXADC,
    MW_FIM_IDX_GP03_CAL_TEMPERATURE,

    MW_FIM_IDX_GP03_MAC_ADDR_WIFI_STA_SRC,
    MW_FIM_IDX_GP03_MAC_ADDR_WIFI_SOFTAP_SRC,
    MW_FIM_IDX_GP03_MAC_ADDR_BLE_SRC,

    MW_FIM_IDX_GP03_STA_MAC_ADDR,
    MW_FIM_IDX_GP03_DEVICE_MANUF_NAME,

    MW_FIM_IDX_GP03_LE_CFG,
    
    MW_FIM_IDX_GP03_SYS_MODE,
    
    MW_FIM_IDX_GP03_MAX
} E_MwFimIdxGroup03;


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, uniou, enum, linked list
// the calibration data of AUXADC
#define MW_FIM_CAL_AUXADC_SIZE                  sizeof(T_HalAuxCalData)
#define MW_FIM_CAL_AUXADC_NUM                   1

// the calibration data of Temperature Sensor
#define MW_FIM_CAL_TEMPERATURE_SIZE             sizeof(T_HalTmprCalData)
#define MW_FIM_CAL_TEMPERATURE_NUM              1

#define MW_FIM_MAC_ADDR_SRC_WIFI_STA_SIZE       1
#define MW_FIM_MAC_ADDR_SRC_WIFI_STA_NUM        1

#define MW_FIM_MAC_ADDR_SRC_WIFI_SOFT_AP_SIZE   1
#define MW_FIM_MAC_ADDR_SRC_WIFI_SOFTAP_NUM     1

#define MW_FIM_MAC_ADDR_SRC_WIFI_BLE_SIZE       1
#define MW_FIM_MAC_ADDR_SRC_WIFI_BLE_NUM        1

#define MW_FIM_STA_MAC_ADDR_NUM                 1
#define MW_FIM_STA_MAC_ADDR_SIZE                MAC_ADDR_LEN

#define STA_INFO_MAX_MANUF_NAME_SIZE            32
#define MW_FIM_DEVICE_MANUF_NAME_NUM            1
#define MW_FIM_DEVICE_MANUF_NAME_SIZE           STA_INFO_MAX_MANUF_NAME_SIZE

#define MW_FIM_IDX_LE_CFG_SIZE                  sizeof(le_cfg_t)
#define MW_FIM_LE_CFG_NUM                       1

#define FIM_HCI_Version                         0x08
#define FIM_HCI_Revision                        0x0001
#define FIM_LMP_PAL_Version                     0x08
#define FIM_Manufacturer_Name                   0x0000  // TODO: not manufacturer name currently
#define FIM_LMP_PAL_Subversion                  0x0001

// the settings of system mode
typedef struct
{
    uint8_t ubSysMode;
    uint8_t ubaReserved[3];
} T_MwFim_SysMode;

#define MW_FIM_SYS_MODE_SIZE    sizeof(T_MwFim_SysMode)
#define MW_FIM_SYS_MODE_NUM     1

typedef enum
{
    MW_FIM_SYS_MODE_INIT = 0,
    MW_FIM_SYS_MODE_MP,
    MW_FIM_SYS_MODE_USER,

    MW_FIM_SYS_MODE_MAX = 0x80
} E_MwFim_SysMode;


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern const T_MwFimFileInfo g_taMwFimGroupTable03[];

extern const T_HalAuxCalData g_tMwFimDefaultCalAuxadc;
extern const T_HalTmprCalData g_tMwFimDefaultCalTmpr;

extern const uint8_t g_tMwFimDefaultMacAddrWifiSTASrc;
extern const uint8_t g_tMwFimDefaultMacAddrWifiSoftAPSrc;
extern const uint8_t g_tMwFimDefaultMacAddrBleSrc;

extern const uint8_t gMwFimDefaultWifiStaMacAddr[MAC_ADDR_LEN];
extern const uint8_t gMwFimDefaultManufName[STA_INFO_MAX_MANUF_NAME_SIZE];

extern const T_MwFim_SysMode g_tMwFimDefaultSysMode;

// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


#ifdef __cplusplus
}
#endif

#endif // _MW_FIM_DEFAULT_GROUP03_H_
