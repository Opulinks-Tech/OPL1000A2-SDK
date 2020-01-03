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
 * @file blewifi_ctrl.c
 * @author Vincent Chen, Michael Liao
 * @date 20 Feb 2018
 * @brief File creates the blewifi ctrl task architecture.
 *
 */

#include <stdlib.h>
#include <string.h>
#include "cmsis_os.h"
#include "event_groups.h"
#include "sys_os_config.h"
#include "sys_os_config_patch.h"
#include "at_cmd_common.h"

#include "blewifi_common.h"
#include "blewifi_configuration.h"
#include "blewifi_ctrl.h"
#include "blewifi_wifi_api.h"
#include "blewifi_ble_api.h"
#include "blewifi_data.h"
#include "blewifi_app.h"
#include "mw_ota_def.h"
#include "mw_ota.h"
#include "hal_system.h"
#include "mw_fim_default_group03.h"
#include "mw_fim_default_group03_patch.h"
#include "mw_fim_default_group11_project.h"
#include "ps_public.h"

#ifdef ALI_BLE_WIFI_PROVISION
#include "awss_notify.h"
#include "awss_cmp.h"
#include "wifi_api.h"
#include "blewifi_server_app.h"
#include "breeze_export.h"
#include "..\src\breeze\include\common.h"
#include "infra_compat.h"
#include "passwd.h"

breeze_apinfo_t g_apInfo;
extern uint32_t tx_func_indicate(uint8_t cmd, uint8_t *p_data, uint16_t length);

extern bool g_noti_flag;
extern bool g_Indi_flag;
#endif
#define BLEWIFI_CTRL_RESET_DELAY    (3000)  // ms

osThreadId   g_tAppCtrlTaskId;
osMessageQId g_tAppCtrlQueueId;
osTimerId    g_tAppCtrlAutoConnectTriggerTimer;
osTimerId    g_tAppCtrlSysTimer;
EventGroupHandle_t g_tAppCtrlEventGroup;

uint8_t g_ulAppCtrlSysMode;
uint8_t g_ubAppCtrlSysStatus;

uint8_t g_ubAppCtrlRequestRetryTimes;
uint32_t g_ulAppCtrlAutoConnectInterval;
uint32_t g_ulAppCtrlWifiDtimTime;
#ifdef ALI_BLE_WIFI_PROVISION
uint8_t g_Ali_wifi_provision =0;
#endif
T_MwFim_GP11_WifiConnectSettings g_tAppCtrlWifiConnectSettings;

static void BleWifi_Ctrl_TaskEvtHandler_BleInitComplete(uint32_t evt_type, void *data, int len);
static void BleWifi_Ctrl_TaskEvtHandler_BleAdvertisingCfm(uint32_t evt_type, void *data, int len);
static void BleWifi_Ctrl_TaskEvtHandler_BleAdvertisingExitCfm(uint32_t evt_type, void *data, int len);
static void BleWifi_Ctrl_TaskEvtHandler_BleAdvertisingTimeChangeCfm(uint32_t evt_type, void *data, int len);
static void BleWifi_Ctrl_TaskEvtHandler_BleConnectionComplete(uint32_t evt_type, void *data, int len);
static void BleWifi_Ctrl_TaskEvtHandler_BleConnectionFail(uint32_t evt_type, void *data, int len);
static void BleWifi_Ctrl_TaskEvtHandler_BleDisconnect(uint32_t evt_type, void *data, int len);
static void BleWifi_Ctrl_TaskEvtHandler_BleDataInd(uint32_t evt_type, void *data, int len);
static void BleWifi_Ctrl_TaskEvtHandler_WifiInitComplete(uint32_t evt_type, void *data, int len);
static void BleWifi_Ctrl_TaskEvtHandler_WifiScanDoneInd(uint32_t evt_type, void *data, int len);
static void BleWifi_Ctrl_TaskEvtHandler_WifiConnectionInd(uint32_t evt_type, void *data, int len);
static void BleWifi_Ctrl_TaskEvtHandler_WifiDisconnectionInd(uint32_t evt_type, void *data, int len);
static void BleWifi_Ctrl_TaskEvtHandler_WifiGotIpInd(uint32_t evt_type, void *data, int len);
static void BleWifi_Ctrl_TaskEvtHandler_WifiAutoConnectInd(uint32_t evt_type, void *data, int len);
#ifdef ALI_BLE_WIFI_PROVISION
static void BleWifi_Ctrl_TaskEvtHandler_AliWifiConnect(uint32_t evt_type, void *data, int len);
#endif
static void BleWifi_Ctrl_TaskEvtHandler_OtherOtaOn(uint32_t evt_type, void *data, int len);
static void BleWifi_Ctrl_TaskEvtHandler_OtherOtaOff(uint32_t evt_type, void *data, int len);
static void BleWifi_Ctrl_TaskEvtHandler_OtherOtaOffFail(uint32_t evt_type, void *data, int len);
static void BleWifi_Ctrl_TaskEvtHandler_OtherSysTimer(uint32_t evt_type, void *data, int len);
static void BleWifi_Ctrl_TaskEvtHandler_NetworkingStart(uint32_t evt_type, void *data, int len);
static void BleWifi_Ctrl_TaskEvtHandler_NetworkingStop(uint32_t evt_type, void *data, int len);
static T_BleWifi_Ctrl_EvtHandlerTbl g_tCtrlEvtHandlerTbl[] =
{
    {BLEWIFI_CTRL_MSG_BLE_INIT_COMPLETE,                BleWifi_Ctrl_TaskEvtHandler_BleInitComplete},
    {BLEWIFI_CTRL_MSG_BLE_ADVERTISING_CFM,              BleWifi_Ctrl_TaskEvtHandler_BleAdvertisingCfm},
    {BLEWIFI_CTRL_MSG_BLE_ADVERTISING_EXIT_CFM,         BleWifi_Ctrl_TaskEvtHandler_BleAdvertisingExitCfm},
    {BLEWIFI_CTRL_MSG_BLE_ADVERTISING_TIME_CHANGE_CFM,  BleWifi_Ctrl_TaskEvtHandler_BleAdvertisingTimeChangeCfm},
    {BLEWIFI_CTRL_MSG_BLE_CONNECTION_COMPLETE,          BleWifi_Ctrl_TaskEvtHandler_BleConnectionComplete},
    {BLEWIFI_CTRL_MSG_BLE_CONNECTION_FAIL,              BleWifi_Ctrl_TaskEvtHandler_BleConnectionFail},
    {BLEWIFI_CTRL_MSG_BLE_DISCONNECT,                   BleWifi_Ctrl_TaskEvtHandler_BleDisconnect},
    {BLEWIFI_CTRL_MSG_BLE_DATA_IND,                     BleWifi_Ctrl_TaskEvtHandler_BleDataInd},
    
    {BLEWIFI_CTRL_MSG_WIFI_INIT_COMPLETE,               BleWifi_Ctrl_TaskEvtHandler_WifiInitComplete},
    {BLEWIFI_CTRL_MSG_WIFI_SCAN_DONE_IND,               BleWifi_Ctrl_TaskEvtHandler_WifiScanDoneInd},
    {BLEWIFI_CTRL_MSG_WIFI_CONNECTION_IND,              BleWifi_Ctrl_TaskEvtHandler_WifiConnectionInd},
    {BLEWIFI_CTRL_MSG_WIFI_DISCONNECTION_IND,           BleWifi_Ctrl_TaskEvtHandler_WifiDisconnectionInd},
    {BLEWIFI_CTRL_MSG_WIFI_GOT_IP_IND,                  BleWifi_Ctrl_TaskEvtHandler_WifiGotIpInd},
    {BLEWIFI_CTRL_MSG_WIFI_AUTO_CONNECT_IND,            BleWifi_Ctrl_TaskEvtHandler_WifiAutoConnectInd},
#ifdef ALI_BLE_WIFI_PROVISION
    {BLEWIFI_CTRL_MSG_ALI_WIFI_CONNECT,                 BleWifi_Ctrl_TaskEvtHandler_AliWifiConnect},
#endif

    {BLEWIFI_CTRL_MSG_OTHER_OTA_ON,                     BleWifi_Ctrl_TaskEvtHandler_OtherOtaOn},
    {BLEWIFI_CTRL_MSG_OTHER_OTA_OFF,                    BleWifi_Ctrl_TaskEvtHandler_OtherOtaOff},
    {BLEWIFI_CTRL_MSG_OTHER_OTA_OFF_FAIL,               BleWifi_Ctrl_TaskEvtHandler_OtherOtaOffFail},
    {BLEWIFI_CTRL_MSG_OTHER_SYS_TIMER,                  BleWifi_Ctrl_TaskEvtHandler_OtherSysTimer},
    {BLEWIFI_CTRL_MSG_NETWORKING_START,                 BleWifi_Ctrl_TaskEvtHandler_NetworkingStart},
    {BLEWIFI_CTRL_MSG_NETWORKING_STOP,                  BleWifi_Ctrl_TaskEvtHandler_NetworkingStop},
    {0xFFFFFFFF,                                        NULL}
};

void BleWifi_Ctrl_SysModeSet(uint8_t mode)
{
    g_ulAppCtrlSysMode = mode;
}

uint8_t BleWifi_Ctrl_SysModeGet(void)
{
    return g_ulAppCtrlSysMode;
}

#ifdef ALI_BLE_WIFI_PROVISION
void linkkit_event_monitor(int event)
{
    switch (event) {
        case IOTX_CONN_REPORT_TOKEN_SUC:
//#ifdef EN_COMBO_NET
            {
                uint8_t rsp[] = { 0x01, 0x01, 0x03 };
//                if(ble_connected){
                    tx_func_indicate(BZ_CMD_STATUS, rsp, sizeof(rsp));
//                    breeze_post(rsp, sizeof(rsp));
//                }
            }
//#endif
            printf("---- report token success ----\r\n");
            if (true == BleWifi_Ctrl_EventStatusGet(BLEWIFI_CTRL_EVENT_BIT_ALI_STOP_BLE))
            {
                BleWifi_Ctrl_MsgSend(BLEWIFI_CTRL_MSG_NETWORKING_STOP, NULL, 0);
                BleWifi_Ctrl_EventStatusSet(BLEWIFI_CTRL_EVENT_BIT_ALI_STOP_BLE, false);                
            }
            break;
        default:
            break;
    }
}
#endif

void BleWifi_Ctrl_EventStatusSet(uint32_t dwEventBit, uint8_t status)
{
// ISR mode is not supported.
#if 0
    BaseType_t xHigherPriorityTaskWoken, xResult;
    
    // check if it is ISR mode or not
    if (0 != __get_IPSR())
    {
        if (true == status)
        {
            // xHigherPriorityTaskWoken must be initialised to pdFALSE.
            xHigherPriorityTaskWoken = pdFALSE;

            // Set bit in xEventGroup.
            xResult = xEventGroupSetBitsFromISR(g_tAppCtrlEventGroup, dwEventBit, &xHigherPriorityTaskWoken);
            if( xResult == pdPASS )
            {
                // If xHigherPriorityTaskWoken is now set to pdTRUE then a context
                // switch should be requested.  The macro used is port specific and
                // will be either portYIELD_FROM_ISR() or portEND_SWITCHING_ISR() -
                // refer to the documentation page for the port being used.
                portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
            }
        }
        else
            xEventGroupClearBitsFromISR(g_tAppCtrlEventGroup, dwEventBit);
    }
    // Taske mode
    else
#endif
    {
        if (true == status)
            xEventGroupSetBits(g_tAppCtrlEventGroup, dwEventBit);
        else
            xEventGroupClearBits(g_tAppCtrlEventGroup, dwEventBit);
    }
}

uint8_t BleWifi_Ctrl_EventStatusGet(uint32_t dwEventBit)
{
    EventBits_t tRetBit;

    tRetBit = xEventGroupGetBits(g_tAppCtrlEventGroup);
    if (dwEventBit == (dwEventBit & tRetBit))
        return true;

    return false;
}

uint8_t BleWifi_Ctrl_EventStatusWait(uint32_t dwEventBit, uint32_t millisec)
{
    EventBits_t tRetBit;

    tRetBit = xEventGroupWaitBits(g_tAppCtrlEventGroup,
                                  dwEventBit,
                                  pdFALSE,
                                  pdFALSE,
                                  millisec);
    if (dwEventBit == (dwEventBit & tRetBit))
        return true;

    return false;
}

void BleWifi_Ctrl_DtimTimeSet(uint32_t value)
{
    g_ulAppCtrlWifiDtimTime = value;
    BleWifi_Wifi_SetDTIM(g_ulAppCtrlWifiDtimTime);
}

uint32_t BleWifi_Ctrl_DtimTimeGet(void)
{
    return g_ulAppCtrlWifiDtimTime;
}

void BleWifi_Ctrl_DoAutoConnect(void)
{
    uint8_t data[2];

    // if the count of auto-connection list is empty, don't do the auto-connect
    if (0 == BleWifi_Wifi_AutoConnectListNum())
        return;

    // if request connect by Peer side, don't do the auto-connection
    if (g_ubAppCtrlRequestRetryTimes <= g_tAppCtrlWifiConnectSettings.ubConnectRetry)
        return;

    // BLE is disconnect and Wifi is disconnect, too.
    if ((false == BleWifi_Ctrl_EventStatusGet(BLEWIFI_CTRL_EVENT_BIT_BLE)) && (false == BleWifi_Ctrl_EventStatusGet(BLEWIFI_CTRL_EVENT_BIT_WIFI)))
    {
        // start to scan
        // after scan, do the auto-connect
        if (g_ubAppCtrlRequestRetryTimes == BLEWIFI_CTRL_AUTO_CONN_STATE_IDLE)
        {
            data[0] = 1;    // Enable to scan AP whose SSID is hidden
            data[1] = 2;    // mixed mode
            BleWifi_Wifi_DoScan(data, 2);

            g_ubAppCtrlRequestRetryTimes = BLEWIFI_CTRL_AUTO_CONN_STATE_SCAN;
        }
    }
}

void BleWifi_Ctrl_AutoConnectTrigger(void const *argu)
{
    BleWifi_Ctrl_MsgSend(BLEWIFI_CTRL_MSG_WIFI_AUTO_CONNECT_IND, NULL, 0);
}

void BleWifi_Ctrl_SysStatusChange(void)
{
    T_MwFim_SysMode tSysMode;
    T_MwFim_GP11_PowerSaving tPowerSaving;

    // get the settings of system mode
    if (MW_FIM_OK != MwFim_FileRead(MW_FIM_IDX_GP03_PATCH_SYS_MODE, 0, MW_FIM_SYS_MODE_SIZE, (uint8_t*)&tSysMode))
    {
        // if fail, get the default value
        memcpy(&tSysMode, &g_tMwFimDefaultSysMode, MW_FIM_SYS_MODE_SIZE);
    }
        
    // get the settings of power saving
    if (MW_FIM_OK != MwFim_FileRead(MW_FIM_IDX_GP11_PROJECT_POWER_SAVING, 0, MW_FIM_GP11_POWER_SAVING_SIZE, (uint8_t*)&tPowerSaving))
    {
        // if fail, get the default value
        memcpy(&tPowerSaving, &g_tMwFimDefaultGp11PowerSaving, MW_FIM_GP11_POWER_SAVING_SIZE);
    }
        
    // change from init to normal
    if (g_ubAppCtrlSysStatus == BLEWIFI_CTRL_SYS_INIT)
    {
        g_ubAppCtrlSysStatus = BLEWIFI_CTRL_SYS_NORMAL;

        /* Power saving settings */
        if (tSysMode.ubSysMode == MW_FIM_SYS_MODE_USER) 
            ps_smart_sleep(tPowerSaving.ubPowerSaving);
                
//        // start the sys timer
//        osTimerStop(g_tAppCtrlSysTimer);
//        osTimerStart(g_tAppCtrlSysTimer, BLEWIFI_COM_SYS_TIME_NORMAL);
    }
    // change from normal to ble off
    else if (g_ubAppCtrlSysStatus == BLEWIFI_CTRL_SYS_NORMAL)
    {
        g_ubAppCtrlSysStatus = BLEWIFI_CTRL_SYS_BLE_OFF;

//        // change the advertising time
//        BleWifi_Ble_AdvertisingTimeChange(BLEWIFI_BLE_ADVERTISEMENT_INTERVAL_PS_MIN, BLEWIFI_BLE_ADVERTISEMENT_INTERVAL_PS_MAX);
    }
}

uint8_t BleWifi_Ctrl_SysStatusGet(void)
{
    return g_ubAppCtrlSysStatus;
}

void BleWifi_Ctrl_SysTimeout(void const *argu)
{
    BleWifi_Ctrl_MsgSend(BLEWIFI_CTRL_MSG_OTHER_SYS_TIMER, NULL, 0);
}

#ifdef ALI_BLE_WIFI_PROVISION
int BleWifi_Wifi_Get_BSsid(void)
{
    wifi_scan_info_t *ap_list = NULL;
    uint16_t apCount = 0;
    int8_t ubAppErr = 0;
    int32_t i = 0;
    
    wifi_scan_get_ap_num(&apCount);
    
    if(apCount == 0)
    {
        printf("No AP found\r\n");
        goto err;
    }
    printf("ap num = %d\n", apCount);
    ap_list = (wifi_scan_info_t *)malloc(sizeof(wifi_scan_info_t) * apCount);
    
    if (!ap_list) {
        printf("malloc fail, ap_list is NULL\r\n");
        ubAppErr = -1;
        goto err;
    }
    
    wifi_scan_get_ap_records(&apCount, ap_list);
    
    
    /* build blewifi ap list */
    for (i = 0; i < apCount; ++i)
    {
        if(!memcmp(ap_list[i].ssid, g_apInfo.ssid, sizeof(ap_list[i].ssid) ))
        {
            memcpy(g_apInfo.bssid, ap_list[i].bssid, 6);
            break;
        }
    }    
err:
    if (ap_list)
        free(ap_list);
    
    return ubAppErr; 


}

void Ali_WiFi_Connect(uint8_t ssid_len, uint8_t bssid_len, uint8_t pwd_len)
{

    uint8_t *conn_data;
    uint8_t conn_data_len=bssid_len+pwd_len+2;

        BleWifi_Ctrl_EventStatusSet(BLEWIFI_CTRL_EVENT_BIT_ALI_WIFI_PRO, true);
 
    conn_data = malloc(sizeof(uint8_t)*conn_data_len);
        if(conn_data==NULL)
        {
             printf("Ali_WiFi_Connect--malloc failed!\r");
             return;
        }
    memset(conn_data, 0, sizeof(uint8_t)*conn_data_len);
    memcpy(conn_data, g_apInfo.bssid, bssid_len);
    *(conn_data+bssid_len) = 0;
    *(conn_data+bssid_len+1) = pwd_len;
    memcpy((conn_data+bssid_len+2), g_apInfo.pw, pwd_len);
    BleWifi_Wifi_DoConnect(conn_data, conn_data_len);
    free(conn_data);
}
#endif

static void BleWifi_Ctrl_TaskEvtHandler_BleInitComplete(uint32_t evt_type, void *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: MSG BLEWIFI_CTRL_MSG_BLE_INIT_COMPLETE \r\n");
    g_noti_flag=false;
    g_Indi_flag=false;
    /* BLE Init Step 2: Do BLE Advertising*/
    BleWifi_Ble_StartAdvertising();
}

static void BleWifi_Ctrl_TaskEvtHandler_BleAdvertisingCfm(uint32_t evt_type, void *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: MSG BLEWIFI_CTRL_MSG_BLE_ADVERTISING_CFM \r\n");
    
    /* BLE Init Step 3: BLE is ready for peer BLE device's connection trigger */
}

static void BleWifi_Ctrl_TaskEvtHandler_BleAdvertisingExitCfm(uint32_t evt_type, void *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: MSG BLEWIFI_CTRL_MSG_BLE_ADVERTISING_EXIT_CFM \r\n");
}

static void BleWifi_Ctrl_TaskEvtHandler_BleAdvertisingTimeChangeCfm(uint32_t evt_type, void *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: MSG BLEWIFI_CTRL_MSG_BLE_ADVERTISING_TIME_CHANGE_CFM \r\n");
}

static void BleWifi_Ctrl_TaskEvtHandler_BleConnectionComplete(uint32_t evt_type, void *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: MSG BLEWIFI_CTRL_MSG_BLE_CONNECTION_COMPLETE \r\n");
    BleWifi_Ctrl_EventStatusSet(BLEWIFI_CTRL_EVENT_BIT_BLE, true);
#ifdef ALI_BLE_WIFI_PROVISION
    BleWifi_Ctrl_EventStatusSet(BLEWIFI_CTRL_EVENT_BIT_LINK_CONN, false); 
    BleWifi_Ctrl_EventStatusSet(BLEWIFI_CTRL_EVENT_BIT_UNBIND, true); 
#endif    
    /* BLE Init Step 4: BLE said it's connected with a peer BLE device */
}

static void BleWifi_Ctrl_TaskEvtHandler_BleConnectionFail(uint32_t evt_type, void *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: MSG BLEWIFI_CTRL_MSG_BLE_CONNECTION_FAIL \r\n");
    BleWifi_Ble_StartAdvertising();
}

static void BleWifi_Ctrl_TaskEvtHandler_BleDisconnect(uint32_t evt_type, void *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: MSG BLEWIFI_CTRL_MSG_BLE_DISCONNECT \r\n");
    BleWifi_Ctrl_EventStatusSet(BLEWIFI_CTRL_EVENT_BIT_BLE, false);
    BleWifi_Ble_StartAdvertising();
    
    /* start to do auto-connection. */
    g_ulAppCtrlAutoConnectInterval = g_tAppCtrlWifiConnectSettings.ulAutoConnectIntervalInit;
    BleWifi_Ctrl_DoAutoConnect();
    
    /* stop the OTA behavior */
    if (gTheOta)
    {
        MwOta_DataGiveUp();
        free(gTheOta);
        gTheOta = 0;
    
        BleWifi_Ctrl_MsgSend(BLEWIFI_CTRL_MSG_OTHER_OTA_OFF_FAIL, NULL, 0);
    }
}

static void BleWifi_Ctrl_TaskEvtHandler_BleDataInd(uint32_t evt_type, void *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: MSG BLEWIFI_CTRL_MSG_BLE_DATA_IND \r\n");
    BleWifi_Ble_DataRecvHandler(data, len);
}

static void BleWifi_Ctrl_TaskEvtHandler_WifiInitComplete(uint32_t evt_type, void *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: MSG BLEWIFI_CTRL_MSG_WIFI_INIT_COMPLETE \r\n");
    
    /* When device power on, start to do auto-connection. */
    g_ulAppCtrlAutoConnectInterval = g_tAppCtrlWifiConnectSettings.ulAutoConnectIntervalInit;
    BleWifi_Ctrl_DoAutoConnect();
}

static void BleWifi_Ctrl_TaskEvtHandler_WifiScanDoneInd(uint32_t evt_type, void *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: MSG BLEWIFI_CTRL_MSG_WIFI_SCAN_DONE_IND \r\n");
    // scan by auto-connect
    if (g_ubAppCtrlRequestRetryTimes == BLEWIFI_CTRL_AUTO_CONN_STATE_SCAN)
    {
        BleWifi_Wifi_UpdateScanInfoToAutoConnList();
        BleWifi_Wifi_DoAutoConnect();
        g_ulAppCtrlAutoConnectInterval = g_ulAppCtrlAutoConnectInterval + g_tAppCtrlWifiConnectSettings.ulAutoConnectIntervalDiff;
        if (g_ulAppCtrlAutoConnectInterval > g_tAppCtrlWifiConnectSettings.ulAutoConnectIntervalMax)
            g_ulAppCtrlAutoConnectInterval = g_tAppCtrlWifiConnectSettings.ulAutoConnectIntervalMax;
    
        g_ubAppCtrlRequestRetryTimes = BLEWIFI_CTRL_AUTO_CONN_STATE_IDLE;
    }
    // scan by user
    else
    {
#ifdef ALI_BLE_WIFI_PROVISION
       if(g_Ali_wifi_provision == 0)
        {
#endif
            BleWifi_Wifi_SendScanReport();        
            BleWifi_Ble_SendResponse(BLEWIFI_RSP_SCAN_END, 0);
#ifdef ALI_BLE_WIFI_PROVISION                
        }
        else
        {
            BleWifi_Wifi_Get_BSsid();                
            Ali_WiFi_Connect(strlen(g_apInfo.ssid),  6, strlen(g_apInfo.pw));        
        }
#endif
    }
}

static void BleWifi_Ctrl_TaskEvtHandler_WifiConnectionInd(uint32_t evt_type, void *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: MSG BLEWIFI_CTRL_MSG_WIFI_CONNECTION_IND \r\n");
    BleWifi_Ctrl_EventStatusSet(BLEWIFI_CTRL_EVENT_BIT_WIFI, true);
    
    // return to the idle of the connection retry
    g_ubAppCtrlRequestRetryTimes = BLEWIFI_CTRL_AUTO_CONN_STATE_IDLE;
    g_ulAppCtrlAutoConnectInterval = g_tAppCtrlWifiConnectSettings.ulAutoConnectIntervalInit;
    BleWifi_Ble_SendResponse(BLEWIFI_RSP_CONNECT, BLEWIFI_WIFI_CONNECTED_DONE);
}

static void BleWifi_Ctrl_TaskEvtHandler_WifiDisconnectionInd(uint32_t evt_type, void *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: MSG BLEWIFI_CTRL_MSG_WIFI_DISCONNECTION_IND \r\n");
    BleWifi_Ctrl_EventStatusSet(BLEWIFI_CTRL_EVENT_BIT_WIFI, false);
    BleWifi_Ctrl_EventStatusSet(BLEWIFI_CTRL_EVENT_BIT_GOT_IP, false);
    BleWifi_Wifi_SetDTIM(0);   

    // continue the connection retry
    if (g_ubAppCtrlRequestRetryTimes < g_tAppCtrlWifiConnectSettings.ubConnectRetry)
    {
        BleWifi_Wifi_ReqConnectRetry();
        g_ubAppCtrlRequestRetryTimes++;
    }
    // stop the connection retry
    else if (g_ubAppCtrlRequestRetryTimes == g_tAppCtrlWifiConnectSettings.ubConnectRetry)
    {
        // return to the idle of the connection retry
        g_ubAppCtrlRequestRetryTimes = BLEWIFI_CTRL_AUTO_CONN_STATE_IDLE;
        g_ulAppCtrlAutoConnectInterval = g_tAppCtrlWifiConnectSettings.ulAutoConnectIntervalInit;
        BleWifi_Ble_SendResponse(BLEWIFI_RSP_CONNECT, BLEWIFI_WIFI_CONNECTED_FAIL);
    
#ifdef ALI_BLE_WIFI_PROVISION
        if (true == BleWifi_Ctrl_EventStatusGet(BLEWIFI_CTRL_EVENT_BIT_ALI_WIFI_PRO))
        {
            BleWifi_Ctrl_EventStatusSet(BLEWIFI_CTRL_EVENT_BIT_ALI_WIFI_PRO, false);
        }
#endif    
        /* do auto-connection. */
        if (false == BleWifi_Ctrl_EventStatusGet(BLEWIFI_CTRL_EVENT_BIT_BLE))
        {
            osTimerStop(g_tAppCtrlAutoConnectTriggerTimer);
            osTimerStart(g_tAppCtrlAutoConnectTriggerTimer, g_ulAppCtrlAutoConnectInterval);
        }
    }
    else
    {
        BleWifi_Ble_SendResponse(BLEWIFI_RSP_DISCONNECT, BLEWIFI_WIFI_DISCONNECTED_DONE);
    
        /* do auto-connection. */
        if (false == BleWifi_Ctrl_EventStatusGet(BLEWIFI_CTRL_EVENT_BIT_BLE))
        {
            osTimerStop(g_tAppCtrlAutoConnectTriggerTimer);
            osTimerStart(g_tAppCtrlAutoConnectTriggerTimer, g_ulAppCtrlAutoConnectInterval);
        }
    }
}

static void BleWifi_Ctrl_TaskEvtHandler_WifiGotIpInd(uint32_t evt_type, void *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: MSG BLEWIFI_CTRL_MSG_WIFI_GOT_IP_IND \r\n");
#if (SNTP_FUNCTION_EN == 1)            
    BleWifi_SntpInit();
#endif
    BleWifi_Wifi_UpdateBeaconInfo();
    BleWifi_Wifi_SetDTIM(BleWifi_Ctrl_DtimTimeGet());
    BleWifi_Wifi_SendStatusInfo(BLEWIFI_IND_IP_STATUS_NOTIFY);

#ifdef ALI_BLE_WIFI_PROVISION
    if(g_Ali_wifi_provision==1)
    {
#if ALI_TOKEN_FROM_DEVICE        
        awss_cmp_local_init(AWSS_LC_INIT_SUC);            
        awss_suc_notify_stop();
        awss_suc_notify();
#else
        uint8_t rsp[] = { 0x01, 0x01, 0x01 };
        tx_func_indicate(BZ_CMD_STATUS, rsp, sizeof(rsp));

#endif        
        g_Ali_wifi_provision =0;                
    }
    BleWifi_Ctrl_EventStatusSet(BLEWIFI_CTRL_EVENT_BIT_LINK_CONN, true);
    if (true == BleWifi_Ctrl_EventStatusGet(BLEWIFI_CTRL_EVENT_BIT_ALI_WIFI_PRO))
    {
        BleWifi_Ctrl_EventStatusSet(BLEWIFI_CTRL_EVENT_BIT_ALI_STOP_BLE, true);
        BleWifi_Ctrl_EventStatusSet(BLEWIFI_CTRL_EVENT_BIT_ALI_WIFI_PRO, false);
    }
#endif
    BleWifi_Ctrl_EventStatusSet(BLEWIFI_CTRL_EVENT_BIT_GOT_IP, true);
}

static void BleWifi_Ctrl_TaskEvtHandler_WifiAutoConnectInd(uint32_t evt_type, void *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: MSG BLEWIFI_CTRL_MSG_WIFI_AUTO_CONNECT_IND \r\n");
    BleWifi_Ctrl_DoAutoConnect();
}

#ifdef ALI_BLE_WIFI_PROVISION
static void BleWifi_Ctrl_TaskEvtHandler_AliWifiConnect(uint32_t evt_type, void *data, int len)
{
    uint8_t ssid_len=0;
    uint8_t pw_len=0;
    uint8_t bssid_len=0;
    uint8_t apptoken_len=0;
    uint8_t*  ap_data = (uint8_t *)(data);
                
    memset(&g_apInfo,0,sizeof(g_apInfo));
                
            
    ssid_len = *(ap_data);
    memcpy(g_apInfo.ssid, (ap_data+1), ssid_len);
                
    bssid_len = *(ap_data+ssid_len+1);
    memcpy(g_apInfo.bssid, (ap_data+ssid_len+2), bssid_len);
                
    pw_len = *(ap_data+ssid_len+bssid_len+2);
    memcpy(g_apInfo.pw, (ap_data+ssid_len+bssid_len+3), pw_len);

    apptoken_len = *(ap_data+ssid_len+bssid_len+pw_len+3);   
    memcpy(g_apInfo.apptoken, (ap_data+ssid_len+bssid_len+pw_len+4), apptoken_len);

    
#ifdef ALI_OPL_DBG
    printf("AliWifiConnect::Send apptoken data(%d):", apptoken_len);
    for(int i=0;i<apptoken_len;i++){                        
        printf("0x%02x ", g_apInfo.apptoken[i]);                
    }
    printf("\r\n");
#endif

    awss_set_token(g_apInfo.apptoken);

    uint8_t tmpdata[2];
    tmpdata[0] = 1;    // Enable to scan AP whose SSID is hidden
    tmpdata[1] = 2;    // mixed mode
    g_Ali_wifi_provision =1;
    BleWifi_Wifi_DoScan(tmpdata, 2);   
}
#endif

static void BleWifi_Ctrl_TaskEvtHandler_OtherOtaOn(uint32_t evt_type, void *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: MSG BLEWIFI_CTRL_MSG_OTHER_OTA_ON \r\n");
    BleWifi_Ctrl_EventStatusSet(BLEWIFI_CTRL_EVENT_BIT_OTA, true);
}

static void BleWifi_Ctrl_TaskEvtHandler_OtherOtaOff(uint32_t evt_type, void *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: MSG BLEWIFI_CTRL_MSG_OTHER_OTA_OFF \r\n");
    BleWifi_Ctrl_EventStatusSet(BLEWIFI_CTRL_EVENT_BIT_OTA, false);
    msg_print_uart1("OK\r\n");
    
    // restart the system
    osDelay(BLEWIFI_CTRL_RESET_DELAY);
    Hal_Sys_SwResetAll();
}

static void BleWifi_Ctrl_TaskEvtHandler_OtherOtaOffFail(uint32_t evt_type, void *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: MSG BLEWIFI_CTRL_MSG_OTHER_OTA_OFF_FAIL \r\n");
    BleWifi_Ctrl_EventStatusSet(BLEWIFI_CTRL_EVENT_BIT_OTA, false);
    msg_print_uart1("ERROR\r\n");
}

static void BleWifi_Ctrl_TaskEvtHandler_OtherSysTimer(uint32_t evt_type, void *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: MSG BLEWIFI_CTRL_MSG_OTHER_SYS_TIMER \r\n");
    BleWifi_Ctrl_SysStatusChange();
}

void BleWifi_Ctrl_NetworkingStart(void)
{
    if (false == BleWifi_Ctrl_EventStatusGet(BLEWIFI_CTRL_EVENT_BIT_NETWORK))
    {
        BLEWIFI_INFO("[%s %d] start\n", __func__, __LINE__);

        BleWifi_Ctrl_EventStatusSet(BLEWIFI_CTRL_EVENT_BIT_NETWORK, true);
    }
    else
    {
        BLEWIFI_WARN("[%s %d] BLEWIFI_CTRL_EVENT_BIT_NETWORK already true\n", __func__, __LINE__);
    }
}

void BleWifi_Ctrl_NetworkingStop(void)
{
    if (true == BleWifi_Ctrl_EventStatusGet(BLEWIFI_CTRL_EVENT_BIT_NETWORK))
    {
        BLEWIFI_INFO("[%s %d] start\n", __func__, __LINE__);

        BleWifi_Ctrl_EventStatusSet(BLEWIFI_CTRL_EVENT_BIT_NETWORK, false);
    }
    else
    {
        BLEWIFI_WARN("[%s %d] BLEWIFI_CTRL_EVENT_BIT_NETWORK already false\n", __func__, __LINE__);
    }
}

static void BleWifi_Ctrl_TaskEvtHandler_NetworkingStart(uint32_t evt_type, void *data, int len)
{
    BleWifi_Ctrl_NetworkingStart();
}

static void BleWifi_Ctrl_TaskEvtHandler_NetworkingStop(uint32_t evt_type, void *data, int len)
{
    BleWifi_Ctrl_NetworkingStop();
}

void BleWifi_Ctrl_TaskEvtHandler(uint32_t evt_type, void *data, int len)
{
    uint32_t i = 0;

    while (g_tCtrlEvtHandlerTbl[i].ulEventId != 0xFFFFFFFF)
    {
        // match
        if (g_tCtrlEvtHandlerTbl[i].ulEventId == evt_type)
        {
            g_tCtrlEvtHandlerTbl[i].fpFunc(evt_type, data, len);
            break;
        }

        i++;
    }

    // not match
    if (g_tCtrlEvtHandlerTbl[i].ulEventId == 0xFFFFFFFF)
    {
    }
}

void BleWifi_Ctrl_Task(void *args)
{
    osEvent rxEvent;
    xBleWifiCtrlMessage_t *rxMsg;

    for(;;)
    {
        /* Wait event */
        rxEvent = osMessageGet(g_tAppCtrlQueueId, osWaitForever);
        if(rxEvent.status != osEventMessage)
            continue;

        rxMsg = (xBleWifiCtrlMessage_t *)rxEvent.value.p;
        BleWifi_Ctrl_TaskEvtHandler(rxMsg->event, rxMsg->ucaMessage, rxMsg->length);

        /* Release buffer */
        if (rxMsg != NULL)
            free(rxMsg);
    }
}

int BleWifi_Ctrl_MsgSend(int msg_type, uint8_t *data, int data_len)
{
    xBleWifiCtrlMessage_t *pMsg = 0;

    if (NULL == g_tAppCtrlQueueId)
    {
        BLEWIFI_ERROR("BLEWIFI: No queue \r\n");
        return -1;
    }

    /* Mem allocate */
    pMsg = malloc(sizeof(xBleWifiCtrlMessage_t) + data_len);
    if (pMsg == NULL)
    {
        BLEWIFI_ERROR("BLEWIFI: ctrl task pmsg allocate fail \r\n");
        goto error;
    }
    
    pMsg->event = msg_type;
    pMsg->length = data_len;
    if (data_len > 0)
    {
        memcpy(pMsg->ucaMessage, data, data_len);
    }

    if (osMessagePut(g_tAppCtrlQueueId, (uint32_t)pMsg, osWaitForever) != osOK)
    {
        BLEWIFI_ERROR("BLEWIFI: ctrl task message send fail \r\n");
        goto error;
    }

    return 0;

error:
    if (pMsg != NULL)
    {
        free(pMsg);
    }
    
    return -1;
}

#ifdef ALI_BLE_WIFI_PROVISION
#define OS_TASK_STACK_SIZE_ALI_BLEWIFI_CTRL        (512)
#endif

void BleWifi_Ctrl_Init(void)
{
    osThreadDef_t task_def;
    osMessageQDef_t blewifi_queue_def;
    osTimerDef_t timer_auto_connect_def;
    osTimerDef_t timer_sys_def;

    /* Create ble-wifi task */
    task_def.name = "blewifi ctrl";
#ifdef ALI_BLE_WIFI_PROVISION    
    task_def.stacksize = OS_TASK_STACK_SIZE_ALI_BLEWIFI_CTRL;
#else
    task_def.stacksize = OS_TASK_STACK_SIZE_BLEWIFI_CTRL;
#endif    
    task_def.tpriority = OS_TASK_PRIORITY_APP;
    task_def.pthread = BleWifi_Ctrl_Task;
    g_tAppCtrlTaskId = osThreadCreate(&task_def, (void*)NULL);
    if(g_tAppCtrlTaskId == NULL)
    {
        BLEWIFI_INFO("BLEWIFI: ctrl task create fail \r\n");
    }
    else
    {
        BLEWIFI_INFO("BLEWIFI: ctrl task create successful \r\n");
    }

    /* Create message queue*/
    blewifi_queue_def.item_sz = sizeof(xBleWifiCtrlMessage_t);
    blewifi_queue_def.queue_sz = BLEWIFI_CTRL_QUEUE_SIZE;
    g_tAppCtrlQueueId = osMessageCreate(&blewifi_queue_def, NULL);
    if(g_tAppCtrlQueueId == NULL)
    {
        BLEWIFI_ERROR("BLEWIFI: ctrl task create queue fail \r\n");
    }

    /* create timer to trig auto connect */
    timer_auto_connect_def.ptimer = BleWifi_Ctrl_AutoConnectTrigger;
    g_tAppCtrlAutoConnectTriggerTimer = osTimerCreate(&timer_auto_connect_def, osTimerOnce, NULL);
    if(g_tAppCtrlAutoConnectTriggerTimer == NULL)
    {
        BLEWIFI_ERROR("BLEWIFI: ctrl task create auto-connection timer fail \r\n");
    }

    /* create timer to trig the sys state */
    timer_sys_def.ptimer = BleWifi_Ctrl_SysTimeout;
    g_tAppCtrlSysTimer = osTimerCreate(&timer_sys_def, osTimerOnce, NULL);
    if(g_tAppCtrlSysTimer == NULL)
    {
        BLEWIFI_ERROR("BLEWIFI: ctrl task create SYS timer fail \r\n");
    }

    /* Create the event group */
    g_tAppCtrlEventGroup = xEventGroupCreate();
    if(g_tAppCtrlEventGroup == NULL)
    {
        BLEWIFI_ERROR("BLEWIFI: ctrl task create event group fail \r\n");
    }

    /* the init state of system mode is init */
    g_ulAppCtrlSysMode = MW_FIM_SYS_MODE_INIT;

    // get the settings of Wifi connect settings
    if (MW_FIM_OK != MwFim_FileRead(MW_FIM_IDX_GP11_PROJECT_WIFI_CONNECT_SETTINGS, 0, MW_FIM_GP11_WIFI_CONNECT_SETTINGS_SIZE, (uint8_t*)&g_tAppCtrlWifiConnectSettings))
    {
        // if fail, get the default value
        memcpy(&g_tAppCtrlWifiConnectSettings, &g_tMwFimDefaultGp11WifiConnectSettings, MW_FIM_GP11_WIFI_CONNECT_SETTINGS_SIZE);
    }

    /* the init state of SYS is init */
    g_ubAppCtrlSysStatus = BLEWIFI_CTRL_SYS_INIT;
    // start the sys timer
    osTimerStop(g_tAppCtrlSysTimer);
    osTimerStart(g_tAppCtrlSysTimer, BLEWIFI_COM_SYS_TIME_INIT);

    /* Init the DTIM time (ms) */
    g_ulAppCtrlWifiDtimTime = g_tAppCtrlWifiConnectSettings.ulDtimInterval;

    // the idle of the connection retry
    g_ubAppCtrlRequestRetryTimes = BLEWIFI_CTRL_AUTO_CONN_STATE_IDLE;
    g_ulAppCtrlAutoConnectInterval = g_tAppCtrlWifiConnectSettings.ulAutoConnectIntervalInit;
}
