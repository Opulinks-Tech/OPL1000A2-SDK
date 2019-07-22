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
*  mw_fim_default_group02.c
*
*  Project:
*  --------
*  OPL1000 Project - the Flash Item Management (FIM) implement file
*
*  Description:
*  ------------
*  This implement file is include the Flash Item Management (FIM) function and api.
*
*  Author:
*  -------
*  Jeff Kuo
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include "ipc.h"
#include "mw_fim_default_group02.h"
#include "controller_wifi_com.h"
#include "wifi_mac_sta.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable

/* Default value of Auto Connect Mode */
const uint32_t gMwFimDefaultAutoConnectMode = true;

/* Default value of Auto Connect AP number */
const uint32_t gMwFimDefaultAutoConnectAPNum = 0;

/* Default value of Auto Connect Configuration */
const MwFimAutoConnectCFG_t gMwFimDefaultAutoConnectCfg = {
    .front = -1,
    .rear = -1,
    .flag = false,
    .targetIdx = 0,
    .max_save_num = MAX_NUM_OF_AUTO_CONNECT,
};

const mw_wifi_auto_connect_ap_info_t gMwFimDefaultAutoConnectAPInfo = {
    .free_ocpy = 0,
    .bssid = {0},
    .ap_channel = 0,
    .latest_beacon_rx_time = 0,
    .ssid = {0},
    .supported_rates = {0},
    .rssi = 0,
    .beacon_interval = 0,
    .capabilities = 0,
    .dtim_prod = 0,
    .wpa_data = {0},
    .rsn_ie = {0},
    .wpa_ie = {0},
    .passphrase = {0},
    .hid_ssid = {0},
    .psk = {0},
    .fast_connect = false,
};

const uint8_t gMwFimDefaultwifiStaSkipDtim = 0; //[0000526]

const uint8_t gMwFimDefaultMacDataRate = 0xFF;

/* Auto Connection Mode */
RET_DATA uint32_t gMwFimAddrAutoConnectMode[MW_FIM_AUTO_CONN_MODE_NUM];

/* Auto Connection Configuration */
RET_DATA uint32_t gMwFimAddrAutoConnectCfg[MW_FIM_IDX_WIFI_AUTO_CONN_CFG_NUM];

/* Auto Connection AP number */
RET_DATA uint32_t gMwFimAddrAutoConnectAPNum[MW_FIM_AUTO_CONN_AP_NUM];

/* Auto Connection Information */
RET_DATA uint32_t gMwFimAddrBufferFastConnectApInfo[MW_FIM_AUTO_CONN_INFO_NUM];

RET_DATA uint32_t gMwFimAddrWifiStaSkipDtim[MW_FIM_STA_SKIP_DTIM_NUM]; //[0000526]


/* Default value of DHCP ARP check */
const uint8_t gMwFimDefaultDhcpArpChk = true;

/* DHCP ARP check */
RET_DATA uint32_t gMwFimAddrDhcpArpChk[MW_FIM_DHCP_ARP_CHK_NUM];

/* MAC TX data rate */
RET_DATA uint32_t gMwFimAddrMacDataRate[MW_FIM_MAC_TX_DATA_RATE_NUM];

/* AT TCPIP Transparent link data */
const at_nvm_trans_config_t gMwFimDefaultTransLinkData = {0};
uint32_t gMwFimAddrTransLinkData[MW_FIM_AT_TCPIP_TRANS_CFG_NUM];

/* AT BLE WiFi mode */
const uint8_t gMwFimDefaultAtBleWifi = 0;
uint32_t gMwFimAddrAtBleWifi[MW_FIM_AT_BLE_WIFI_NUM];

// the information table of group 02
const T_MwFimFileInfo g_taMwFimGroupTable02[] =
{
    {MW_FIM_IDX_WIFI_AUTO_CONN_MODE,      MW_FIM_AUTO_CONN_MODE_NUM,      MW_FIM_AUTO_COMM_MODE_SIZE,            (uint8_t*)&gMwFimDefaultAutoConnectMode,   gMwFimAddrAutoConnectMode},
    {MW_FIM_IDX_WIFI_AUTO_CONN_CFG,       MW_FIM_IDX_WIFI_AUTO_CONN_CFG_NUM, MW_FIM_IDX_WIFI_AUTO_CONN_CFG_SIZE, (uint8_t*)&gMwFimDefaultAutoConnectCfg,    gMwFimAddrAutoConnectCfg},
    {MW_FIM_IDX_WIFI_AUTO_CONN_AP_NUM,    MW_FIM_AUTO_CONN_AP_NUM,        MW_FIM_AUTO_COMM_AP_SIZE,              (uint8_t*)&gMwFimDefaultAutoConnectAPNum,  gMwFimAddrAutoConnectAPNum},
    {MW_FIM_IDX_WIFI_AUTO_CONN_AP_INFO,   MW_FIM_AUTO_CONN_INFO_NUM,      MW_FIM_AUTO_CONN_INFO_SIZE,            (uint8_t*)&gMwFimDefaultAutoConnectAPInfo, gMwFimAddrBufferFastConnectApInfo},
    {MW_FIM_IDX_STA_SKIP_DTIM,            MW_FIM_STA_SKIP_DTIM_NUM,       MW_FIM_STA_SKIP_DTIM_SIZE,             (uint8_t*)&gMwFimDefaultwifiStaSkipDtim,   gMwFimAddrWifiStaSkipDtim}, /* [0000526] */
    {MW_FIM_IDX_DHCP_ARP_CHK,             MW_FIM_DHCP_ARP_CHK_NUM,        MW_FIM_DHCP_ARP_CHK_SIZE,              (uint8_t*)&gMwFimDefaultDhcpArpChk,        gMwFimAddrDhcpArpChk},
    {MW_FIM_IDX_MAC_TX_DATA_RATE,         MW_FIM_MAC_TX_DATA_RATE_NUM,    MW_FIM_MAC_TX_DATA_RATE_SIZE,          (uint8_t*)&gMwFimDefaultMacDataRate,       gMwFimAddrMacDataRate},
    {MW_FIM_IDX_AT_TCPIP_TRANS_CFG,       MW_FIM_AT_TCPIP_TRANS_CFG_NUM,  MW_FIM_AT_TCPIP_TRANS_CFG_SIZE,        (uint8_t*)&gMwFimDefaultTransLinkData,     gMwFimAddrTransLinkData},
    {MW_FIM_IDX_AT_BLE_WIFI,              MW_FIM_AT_BLE_WIFI_NUM,         MW_FIM_AT_BLE_WIFI_SIZE,               (uint8_t*)&gMwFimDefaultAtBleWifi,         gMwFimAddrAtBleWifi},

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
