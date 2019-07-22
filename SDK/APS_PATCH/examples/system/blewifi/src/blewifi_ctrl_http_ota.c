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

/**
 * @file blewifi_ctrl_http_ota.c
 * @author Luke Fang
 * @date 11 Sep 2018
 * @brief File creates the blewifi http ota task architecture.
 *
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cmsis_os.h"
#include "sys_os_config.h"
#include "blewifi_configuration.h"
#include "blewifi_ctrl_http_ota.h"
#include "blewifi_http_ota.h"
#include "blewifi_data.h"
#include "blewifi_common.h"
#include "blewifi_ctrl.h"
#include "blewifi_wifi_api.h"

#if (WIFI_OTA_FUNCTION_EN == 1)
osThreadId   BleWifCtrliHttpOtaTaskId;
osMessageQId BleWifiCtrlHttpOtaQueueId;

void blewifi_ctrl_http_ota_task_evt_handler(uint32_t evt_type, void *data, int len)
{
    switch (evt_type)
    {
        case BLEWIFI_CTRL_HTTP_OTA_MSG_TRIG:
            BLEWIFI_INFO("BLEWIFI: MSG BLEWIFI_CTRL_HTTP_OTA_MSG_TRIG \r\n");
            BleWifi_Ctrl_MsgSend(BLEWIFI_CTRL_MSG_OTHER_OTA_ON, NULL, 0);
            BleWifi_Wifi_SetDTIM(0);
            if (ota_download_by_http(data) != 0)
            {
                BleWifi_Wifi_SetDTIM(BleWifi_Ctrl_DtimTimeGet());
                BleWifi_Wifi_OtaTrigRsp(BLEWIFI_WIFI_OTA_FAILURE);
                BleWifi_Ctrl_MsgSend(BLEWIFI_CTRL_MSG_OTHER_OTA_OFF_FAIL, NULL, 0);
            }
            else
            {
                BleWifi_Wifi_SetDTIM(BleWifi_Ctrl_DtimTimeGet());
                BleWifi_Wifi_OtaTrigRsp(BLEWIFI_WIFI_OTA_SUCCESS);
                BleWifi_Ctrl_MsgSend(BLEWIFI_CTRL_MSG_OTHER_OTA_OFF, NULL, 0);
            }
            break;
        case BLEWIFI_CTRL_HTTP_OTA_MSG_DEVICE_VERSION:
        {
            BLEWIFI_INFO("BLEWIFI: MSG BLEWIFI_CTRL_HTTP_OTA_MSG_DEVICE_VERSION \r\n");
            uint16_t pid;
            uint16_t cid;
            uint16_t fid;
            
            ota_get_version(&pid, &cid, &fid);
            
            printf("BLEWIFI_CTRL_HTTP_OTA_MSG_DEVICE_VERSION pid = %d, cid = %d, fid = %d\n", pid, cid, fid);
            BleWifi_Wifi_OtaDeviceVersionRsp(fid);
        }
        break;
        case BLEWIFI_CTRL_HTTP_OTA_MSG_SERVER_VERSION:
        {
            BLEWIFI_INFO("BLEWIFI: MSG BLEWIFI_CTRL_HTTP_OTA_MSG_SERVER_VERSION \r\n");
            uint16_t fid;
            
            if (ota_download_by_http_get_server_version(WIFI_OTA_HTTP_URL, &fid) != 0)
            {
                fid = 0;
                BleWifi_Wifi_OtaServerVersionRsp(fid);
            }
            else
            {
                printf("ota_download_by_http_get_server_version - fid = %d", fid);
                BleWifi_Wifi_OtaServerVersionRsp(fid);
            }
        }
        break;
        
        default:
            break;
    }
}

void blewifi_http_ota_task(void *args)
{
    osEvent rxEvent;
    xBleWifiCtrlHttpOtaMessage_t *rxMsg;
    
    for(;;)
    {
        /* Wait event */
        rxEvent = osMessageGet(BleWifiCtrlHttpOtaQueueId, osWaitForever);
        if(rxEvent.status != osEventMessage)
            continue;
            
        rxMsg = (xBleWifiCtrlHttpOtaMessage_t *)rxEvent.value.p;
        blewifi_ctrl_http_ota_task_evt_handler(rxMsg->event, rxMsg->ucaMessage, rxMsg->length);
        
        /* Release buffer */
        if (rxMsg != NULL)
            free(rxMsg);
    }
}

void blewifi_ctrl_http_ota_task_create(void)
{
    osThreadDef_t task_def;
    osMessageQDef_t blewifi_queue_def;
    
    /* Create ble-wifi task */
    task_def.name = "blewifi ctrl http ota";
    task_def.stacksize = OS_TASK_STACK_SIZE_APP * 2;
    task_def.tpriority = OS_TASK_PRIORITY_APP;
    task_def.pthread = blewifi_http_ota_task;
    
    BleWifCtrliHttpOtaTaskId = osThreadCreate(&task_def, (void*)NULL);
    if(BleWifCtrliHttpOtaTaskId == NULL)
    {
        BLEWIFI_INFO("BLEWIFI: ctrl task create fail \r\n");
    }
    
    /* Create message queue*/
    blewifi_queue_def.item_sz = sizeof(xBleWifiCtrlHttpOtaMessage_t);
    blewifi_queue_def.queue_sz = BLEWIFI_CTRL_HTTP_OTA_QUEUE_SIZE;
    BleWifiCtrlHttpOtaQueueId = osMessageCreate(&blewifi_queue_def, NULL);
    if(BleWifiCtrlHttpOtaQueueId == NULL)
    {
		BLEWIFI_ERROR("BLEWIFI: ctrl task create queue fail \r\n");
    }
}

int blewifi_ctrl_http_ota_msg_send(int msg_type, uint8_t *data, int data_len)
{
    int iRet = -1;
    xBleWifiCtrlHttpOtaMessage_t *pMsg = NULL;
    
    /* Mem allocate */
    pMsg = malloc(sizeof(xBleWifiCtrlHttpOtaMessage_t) + data_len);
    if (pMsg == NULL)
    {
        BLEWIFI_ERROR("BLEWIFI: ctrl http ota task message allocate fail \r\n");
        goto done;
    }
    
    pMsg->event = msg_type;
    pMsg->length = data_len;
    if (data_len > 0)
    {
        memcpy(pMsg->ucaMessage, data, data_len);
    }
    
    if (osMessagePut(BleWifiCtrlHttpOtaQueueId, (uint32_t)pMsg, osWaitForever) != osOK)
    {
        BLEWIFI_ERROR("BLEWIFI: ctrl task message send fail \r\n");
        goto done;
    }
    
    iRet = 0;
    
done:
    if(iRet)
    {
        if (pMsg != NULL)
        {
            free(pMsg);
        }
    }
    
    return iRet;
}
#endif /* #if (WIFI_OTA_FUNCTION_EN == 1) */
