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
*  mw_fim_default_group02.h
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
#ifndef _MW_FIM_DEFAULT_GROUP02_H_
#define _MW_FIM_DEFAULT_GROUP02_H_

#ifdef __cplusplus
extern "C" {
#endif

// Sec 0: Comment block of the file


// Sec 1: Include File
#include "mw_fim.h"
#include "controller_wifi_com.h"
#include "at_cmd_nvm.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
// the file ID
// xxxx_xxxx_xxxx_xxxx_xxxx_xxxx_xxxx_xxxx
// ^^^^ ^^^^ Zone (0~3)
//           ^^^^ ^^^^ Group (0~8), 0 is reserved for swap
//                     ^^^^ ^^^^ ^^^^ ^^^^ File ID, start from 0
typedef enum
{
    MW_FIM_IDX_GP02_START = 0x00020000,             // the start IDX of group 02
    MW_FIM_IDX_WIFI_AUTO_CONN_MODE,
    MW_FIM_IDX_WIFI_AUTO_CONN_CFG,
    MW_FIM_IDX_WIFI_AUTO_CONN_AP_NUM,
    MW_FIM_IDX_WIFI_AUTO_CONN_AP_INFO,
    MW_FIM_IDX_STA_SKIP_DTIM,
    MW_FIM_IDX_DHCP_ARP_CHK,
    MW_FIM_IDX_MAC_TX_DATA_RATE,
    MW_FIM_IDX_AT_TCPIP_TRANS_CFG,
    MW_FIM_IDX_AT_BLE_WIFI,
    MW_FIM_IDX_GP02_MAX
} E_MwFimIdxGroup02;

#define STA_INFO_MAX_MANUF_NAME_SIZE   32

#define MW_FIM_AUTO_CONN_MODE_NUM     1
#define MW_FIM_AUTO_COMM_MODE_SIZE    1

#define MW_FIM_IDX_WIFI_AUTO_CONN_CFG_NUM 1
#define MW_FIM_IDX_WIFI_AUTO_CONN_CFG_SIZE sizeof(MwFimAutoConnectCFG_t)

#define MW_FIM_AUTO_CONN_AP_NUM     1
#define MW_FIM_AUTO_COMM_AP_SIZE    1

#define MW_FIM_AUTO_CONN_INFO_NUM         3
#define MW_FIM_AUTO_CONN_INFO_SIZE        sizeof(mw_wifi_auto_connect_ap_info_t)

#define MW_FIM_STA_SKIP_DTIM_NUM         1
#define MW_FIM_STA_SKIP_DTIM_SIZE        1

#define MW_FIM_DHCP_ARP_CHK_NUM          1
#define MW_FIM_DHCP_ARP_CHK_SIZE         1

#define MW_FIM_MAC_TX_DATA_RATE_NUM      1
#define MW_FIM_MAC_TX_DATA_RATE_SIZE     1

#define MW_FIM_AT_TCPIP_TRANS_CFG_NUM    1
#define MW_FIM_AT_TCPIP_TRANS_CFG_SIZE   sizeof(at_nvm_trans_config_t)

#define MW_FIM_AT_BLE_WIFI_NUM           1
#define MW_FIM_AT_BLE_WIFI_SIZE          1

/******************************
Declaration of data structure
******************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern const T_MwFimFileInfo g_taMwFimGroupTable02[];
extern const uint32_t gMwFimDefaultAutoConnectMode;
extern const uint32_t gMwFimDefaultAutoConnectAPNum;
extern const MwFimAutoConnectCFG_t gMwFimDefaultAutoConnectCfg;
extern const mw_wifi_auto_connect_ap_info_t gMwFimDefaultAutoConnectAPInfo;
extern const uint8_t gMwFimDefaultwifiStaSkipDtim;
extern const uint8_t gMwFimDefaultMacDataRate;

// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


#ifdef __cplusplus
}
#endif

#endif // _MW_FIM_DEFAULT_GROUP02_H_
