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

#include <stdlib.h>
#include <string.h>
#include "cmsis_os.h"
#include "sys_os_config.h"

#include "iot_data.h"
#include "blewifi_common.h"
#include "blewifi_configuration.h"
#include "blewifi_ctrl.h"


#if (IOT_DEVICE_DATA_TX_EN == 1)
osThreadId g_tAppIotDataTxTaskId;
osMessageQId g_tAppIotDataTxQueueId;
#endif
#if (IOT_DEVICE_DATA_RX_EN == 1)
osThreadId g_tAppIotDataRxTaskId;
#endif

#if (IOT_DEVICE_DATA_TX_EN == 1)
static void Iot_Data_TxTaskEvtHandler_DataPost(uint32_t evt_type, void *data, int len);
static T_IoT_Data_EvtHandlerTbl g_tAppIotDataTxTaskEvtHandlerTbl[] =
{
    {IOT_DATA_TX_MSG_DATA_POST,             Iot_Data_TxTaskEvtHandler_DataPost},
    
    {0xFFFFFFFF,                            NULL}
};

static void Iot_Data_TxTaskEvtHandler_DataPost(uint32_t evt_type, void *data, int len)
{
    // get the IoT data here, or the data are from *data and len parameters.

    // send the data to cloud
}

void Iot_Data_TxTaskEvtHandler(uint32_t evt_type, void *data, int len)
{
    uint32_t i = 0;

    while (g_tAppIotDataTxTaskEvtHandlerTbl[i].ulEventId != 0xFFFFFFFF)
    {
        // match
        if (g_tAppIotDataTxTaskEvtHandlerTbl[i].ulEventId == evt_type)
        {
            g_tAppIotDataTxTaskEvtHandlerTbl[i].fpFunc(evt_type, data, len);
            break;
        }

        i++;
    }

    // not match
    if (g_tAppIotDataTxTaskEvtHandlerTbl[i].ulEventId == 0xFFFFFFFF)
    {
    }
}

void Iot_Data_TxTask(void *args)
{
    osEvent rxEvent;
    xIotDataMessage_t *rxMsg;

    // do the initialization
    while (1)
    {
        #if 1
        if (true == BleWifi_Ctrl_EventStatusWait(BLEWIFI_CTRL_EVENT_BIT_GOT_IP, 0xFFFFFFFF))
        {
            // init behavior
            BleWifi_Ctrl_EventStatusSet(BLEWIFI_CTRL_EVENT_BIT_IOT_INIT, true);
            break;
        }
        // !!! if the IoT initialization is executed once by Tx or Rx, we could wait the behavior finish.
        #else
        if (true == BleWifi_Ctrl_EventStatusWait(BLEWIFI_CTRL_EVENT_BIT_IOT_INIT, 0xFFFFFFFF))
        {
            break;
        }
        #endif
    }

    while (1)
    {
        /* Wait event */
        rxEvent = osMessageGet(g_tAppIotDataTxQueueId, osWaitForever);
        if(rxEvent.status != osEventMessage)
            continue;

        rxMsg = (xIotDataMessage_t *)rxEvent.value.p;
        Iot_Data_TxTaskEvtHandler(rxMsg->event, rxMsg->ucaMessage, rxMsg->length);

        /* Release buffer */
        if (rxMsg != NULL)
            free(rxMsg);
    }
}

int Iot_Data_TxTask_MsgSend(int msg_type, uint8_t *data, int data_len)
{
    xIotDataMessage_t *pMsg = 0;

	if (NULL == g_tAppIotDataTxQueueId)
	{
        BLEWIFI_ERROR("BLEWIFI: No IoT Tx task queue \r\n");
        return -1;
	}

    /* Mem allocate */
    pMsg = malloc(sizeof(xIotDataMessage_t) + data_len);
    if (pMsg == NULL)
	{
        BLEWIFI_ERROR("BLEWIFI: IoT Tx task pmsg allocate fail \r\n");
	    goto error;
    }
    
    pMsg->event = msg_type;
    pMsg->length = data_len;
    if (data_len > 0)
    {
        memcpy(pMsg->ucaMessage, data, data_len);
    }

    if (osMessagePut(g_tAppIotDataTxQueueId, (uint32_t)pMsg, osWaitForever) != osOK)
    {
        BLEWIFI_ERROR("BLEWIFI: IoT Tx task message send fail \r\n");
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

void Iot_Data_TxInit(void)
{
    osThreadDef_t task_def;
    osMessageQDef_t queue_def;

    /* Create IoT Tx task */
    task_def.name = "iot tx";
    task_def.stacksize = IOT_DEVICE_DATA_TASK_STACK_SIZE_TX;
    task_def.tpriority = OS_TASK_PRIORITY_APP;
    task_def.pthread = Iot_Data_TxTask;
    g_tAppIotDataTxTaskId = osThreadCreate(&task_def, (void*)NULL);
    if(g_tAppIotDataTxTaskId == NULL)
    {
        BLEWIFI_INFO("BLEWIFI: IoT Tx task create fail \r\n");
    }
    else
    {
        BLEWIFI_INFO("BLEWIFI: IoT Tx task create successful \r\n");
    }

    /* Create IoT Tx message queue*/
    queue_def.item_sz = sizeof(xIotDataMessage_t);
    queue_def.queue_sz = IOT_DEVICE_DATA_QUEUE_SIZE_TX;
    g_tAppIotDataTxQueueId = osMessageCreate(&queue_def, NULL);
    if(g_tAppIotDataTxQueueId == NULL)
    {
        BLEWIFI_ERROR("BLEWIFI: IoT Tx create queue fail \r\n");
    }
}
#endif  // end of #if (IOT_DEVICE_DATA_TX_EN == 1)

#if (IOT_DEVICE_DATA_RX_EN == 1)
void Iot_Data_RxTask(void *args)
{
    // do the initialization
    while (1)
    {
        #if 1
        if (true == BleWifi_Ctrl_EventStatusWait(BLEWIFI_CTRL_EVENT_BIT_GOT_IP, 0xFFFFFFFF))
        {
            // init behavior
            BleWifi_Ctrl_EventStatusSet(BLEWIFI_CTRL_EVENT_BIT_IOT_INIT, true);
            break;
        }
        // !!! if the IoT initialization is executed once by Tx or Rx, we could wait the behavior finish.
        #else
        if (true == BleWifi_Ctrl_EventStatusWait(BLEWIFI_CTRL_EVENT_BIT_IOT_INIT, 0xFFFFFFFF))
        {
            break;
        }
        #endif
    }
    
    // do the rx behavior
    while (1)
    {
        if (true == BleWifi_Ctrl_EventStatusWait(BLEWIFI_CTRL_EVENT_BIT_GOT_IP, 0xFFFFFFFF))
        {
            // rx behavior
            osDelay(10000); // if do nothing for rx behavior, the delay must be exist.
                            // if do something for rx behavior, the delay could be removed.
        }
    }
}

void Iot_Data_RxInit(void)
{
    osThreadDef_t task_def;

    /* Create IoT Rx task */
    task_def.name = "iot rx";
    task_def.stacksize = IOT_DEVICE_DATA_TASK_STACK_SIZE_RX;
    task_def.tpriority = OS_TASK_PRIORITY_APP;
    task_def.pthread = Iot_Data_RxTask;
    g_tAppIotDataRxTaskId = osThreadCreate(&task_def, (void*)NULL);
    if(g_tAppIotDataRxTaskId == NULL)
    {
        BLEWIFI_INFO("BLEWIFI: IoT Rx task create fail \r\n");
    }
    else
    {
        BLEWIFI_INFO("BLEWIFI: IoT Rx task create successful \r\n");
    }
}
#endif  // end of #if (IOT_DEVICE_DATA_RX_EN == 1)

#if (IOT_DEVICE_DATA_TX_EN == 1) || (IOT_DEVICE_DATA_RX_EN == 1)
void Iot_Data_Init(void)
{
#if (IOT_DEVICE_DATA_TX_EN == 1)
    Iot_Data_TxInit();
#endif

#if (IOT_DEVICE_DATA_RX_EN == 1)
    Iot_Data_RxInit();
#endif
}
#endif  // end of #if (IOT_DEVICE_DATA_TX_EN == 1) || (IOT_DEVICE_DATA_RX_EN == 1)
