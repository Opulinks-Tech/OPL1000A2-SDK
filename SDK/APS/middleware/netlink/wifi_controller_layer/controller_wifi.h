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

#ifndef __CONTROLLER_WIFI_H__
#define __CONTROLLER_WIFI_H__

#include "controller_task.h"
#include "ieee80211_mac_header.h"
#include "wifi_mac_frame.h"
#include "controller_wifi_com.h"

#define REG_TRX_BASE            0X40003000
#define REG_TRX_WIFI_BSSID0     (REG_TRX_BASE + 0X10C)
#define REG_TRX_WIFI_BSSID1     (REG_TRX_BASE + 0X110)
#define REG_TRX_WIFI_OWN_MAC0   (REG_TRX_BASE + 0X118)
#define REG_TRX_WIFI_OWN_MAC1   (REG_TRX_BASE + 0X11C)

#define WIFI_SCAN_TOUT_TIME     10000   //10 seconds
#define WIFI_SCAN_BY_CFG_TOUT_TIME     22000   //20 seconds
#define WIFI_CONN_TOUT_TIME     8000    // Connection Timer, 8 seconds
#define WIFI_CMD_TOUT_TIME      8000    // Common Command Timer, 8 seconds

#define RF_CMD_PARAM_NUM    10


typedef struct
{
    u32  cmd_type:8;
    u32  arg1:8;
    u32  arg2:16;
    void *prvData;
} wifi_cmd_t;

typedef struct
{
    uint32_t evt_type; /*mlme_evt_type_e*/
    void *prvData;
} wifi_evt_t;

typedef enum
{
    RF_EVT_MODE = 0,
    RF_EVT_GO,
    RF_EVT_CHANNEL,
    RF_EVT_RX_RESET_CNTS,
    RF_EVT_RX_CNTS,
    RF_EVT_WIFI_RX,
    RF_EVT_WIFI_TX,

    RF_EVT_BLE_DTM,
    RF_EVT_BLE_ADV,

    RF_EVT_SHOW_SCA,
    RF_EVT_SET_SCA,
    RF_EVT_CAL_VCO,
    RF_EVT_TEST_MODE,
    RF_EVT_PHY_DUMP,

    RF_EVT_RELOAD_OW,
    RF_EVT_STORE_OW,
    RF_EVT_SHOW_OW,
    RF_EVT_ADD_OW,
    RF_EVT_DEL_OW,

    RF_EVT_IPC_ENABLE,

    RF_EVT_MAX
} T_RfCmdEvtType;

typedef struct
{
    uint32_t u32Type;
    int iArgc;
    char *saArgv[RF_CMD_PARAM_NUM];
} T_RfCmd;

typedef struct
{
    uint32_t u32Type;
    uint8_t u8Status;
    uint8_t u8aData[59];
} T_RfDefEvt;

typedef struct
{
    uint32_t u32Type;
    uint8_t u8Status;

    uint8_t u8IpcEnable;
    uint8_t u8Unicast;
    uint8_t u8Reserved;
    uint32_t u32Mode;
    uint32_t u32RfChannel;
    uint32_t u32Freq;
    uint16_t u16RfMode;

    // BLE
    uint16_t u16RxCnt;
    uint16_t u16RxCrcOkCnt;
    int8_t i8Rssi;
    uint8_t u8Phy;
    uint8_t u8Pkt;
    uint8_t u8Len;
    uint8_t u8Freq;

    void *pParam;
} T_RfEvt;


extern scan_report_t gScanReport;

void CtrlWifi_PsTout(void const *arg);

int controller_wifi_init_impl(void *pScanResult);
int controller_wifi_cmd_handler_impl(wifi_cmd_t * pWifiCmd);
int controller_wifi_evt_handler_impl(wifi_evt_t * pWifiEvt);
int CtrlWifi_PsStateForce_impl(WifiSta_PSForceMode_t mode, uint32_t timeout);

/* Rom patch interface */
typedef int (*controller_wifi_init_fp_t)(void *pScanResult);
typedef int (*controller_wifi_cmd_handler_fp_t)(wifi_cmd_t *pWifiCmd);
typedef int (*controller_wifi_evt_handler_fp_t)(wifi_evt_t *pWifiEvt);
typedef int (*CtrlWifi_PsStateForce_fp_t)(WifiSta_PSForceMode_t mode, uint32_t timeout);

/* Export interface funtion pointer */
extern controller_wifi_init_fp_t controller_wifi_init;
extern controller_wifi_cmd_handler_fp_t controller_wifi_cmd_handler;
extern controller_wifi_evt_handler_fp_t controller_wifi_evt_handler;
extern CtrlWifi_PsStateForce_fp_t CtrlWifi_PsStateForce;

#endif  //__CONTROLLER_WIFI_H__

