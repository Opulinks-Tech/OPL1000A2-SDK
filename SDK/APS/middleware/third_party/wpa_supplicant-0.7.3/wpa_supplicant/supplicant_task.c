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
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cmsis_os.h"
#include "msg.h"
#include "supplicant_task.h"
#include "driver.h"
#include "controller_wifi_com.h"
#include "wpa_common.h"
#include "config.h"
#include "wpa_cli.h"
#include "wpa_supplicant_i.h"
#include "common.h"
#include "wpa.h"
#include "events_netlink.h"
#include "sys_os_config.h"
#include "at_cmd_common.h"
#include "tcpecho.h"
#include "wpa_demo.h"
#include "at_cmd_msg_ext.h"

RET_DATA osMessageQId    xSupplicantQueue;
RET_DATA osThreadId      SupplicantTaskHandle;
RET_DATA osPoolId        supplicantMemPoolId;

RET_DATA uint32_t supplicantTaskStackSize;
RET_DATA int supplicantTaskPriority;

extern struct wpa_supplicant *wpa_s;
extern auto_connect_cfg_t g_AutoConnect; //Fast Connect Report

osPoolDef (supplicantMemPoolId, SUPP_QUEUE_SIZE, xSupplicantMessage_t); // memory pool object

void supplicant_task_create_impl(void)
{
    osThreadDef_t   task_def;
    osMessageQDef_t queue_def;

    //create task
    task_def.name = OS_TASK_NAME_SUPPLICANT;
    task_def.stacksize = supplicantTaskStackSize;
    task_def.tpriority = (osPriority)supplicantTaskPriority;
    task_def.pthread = supplicant_task;
    SupplicantTaskHandle = osThreadCreate(&task_def, (void *)SupplicantTaskHandle);
    if(SupplicantTaskHandle == NULL)
    {
        msg_print(LOG_HIGH_LEVEL, "Supplicant task is created failed! \r\n");
    }
    else
    {
        msg_print(LOG_HIGH_LEVEL, "Supplicant task is created successfully! \r\n");
    }

    //create memory pool
    supplicantMemPoolId = osPoolCreate (osPool(supplicantMemPoolId)); // create Mem Pool
    if (!supplicantMemPoolId)
    {
        msg_print(LOG_HIGH_LEVEL, "Supplicant Task Mem Pool create Fail \r\n"); // MemPool object not created, handle failure
    }

    //create queue
    queue_def.item_sz = sizeof( xSupplicantMessage_t );
    queue_def.queue_sz = SUPP_QUEUE_SIZE;
    xSupplicantQueue = osMessageCreate(&queue_def, SupplicantTaskHandle);
    if(xSupplicantQueue == NULL)
    {
        msg_print(LOG_HIGH_LEVEL, "Supplicant Task Queue fail \r\n");
    }
}

void supplicant_task_init_impl(void)
{
    msg_print(LOG_HIGH_LEVEL, "supplicant_task_init entry \r\n");
}

osStatus supplicant_task_send_impl(xSupplicantMessage_t txMsg)
{
    osStatus ret = osErrorOS;
    xSupplicantMessage_t    *pMsg = NULL;

    //Mem pool allocate
    pMsg = (xSupplicantMessage_t *)osPoolCAlloc (supplicantMemPoolId);         // get Mem Block

    if(pMsg == NULL)
    {
        goto done;
    }

    pMsg->event = txMsg.event;
    pMsg->length = txMsg.length;
    pMsg->pcMessage = NULL;

    if((txMsg.pcMessage) && (txMsg.length))
    {
        //malloc buffer
        pMsg->pcMessage = (void *)malloc(txMsg.length);

        if(pMsg->pcMessage == NULL)
        {
            msg_print(LOG_HIGH_LEVEL, "Supplicant task message allocate fail \r\n");
            goto done;
        }

        memcpy(pMsg->pcMessage, txMsg.pcMessage, txMsg.length);
    }

    if(osMessagePut (xSupplicantQueue, (uint32_t)pMsg, osWaitForever) != osOK) // Send Message
    {
        goto done;
    }

    ret = osOK;

done:
    if(ret != osOK)
    {
        if(pMsg)
        {
            if(pMsg->pcMessage)
            {
                free(pMsg->pcMessage);
            }

            osPoolFree(supplicantMemPoolId, pMsg);
        }
    }

    return ret;
}

/*
 supervisor task
*/
void supplicant_task_impl( void *pvParameters )
{
    osEvent         rxEvent;
    xSupplicantMessage_t    *rxMsg;
    //xDiagMessage    txMsg;

    //task init
    supplicant_task_init();

    //For Demo
    wpa_demo_send_msg_to_wpas();

    for(;;)
    {
        //wait event
        rxEvent = osMessageGet(xSupplicantQueue, osWaitForever);
        if(rxEvent.status != osEventMessage)
            continue;

		rxMsg = (xSupplicantMessage_t *) rxEvent.value.p;
		supplicant_task_evt_handle(rxMsg->event);

        /*release buffer*/
        if(rxMsg->pcMessage != NULL)
            free(rxMsg->pcMessage);
        osPoolFree (supplicantMemPoolId, rxMsg);
    }
}

void supplicant_task_evt_handle_impl(uint32_t evt_type)
{
    hap_control_t *hap_temp;    //I0064 0000318
    hap_temp=get_hap_control_struct();  //I0064 0000318
	switch (evt_type)
    {
	    case MLME_EVT_ASSOC:
            msg_print(LOG_LOW_LEVEL, "[EVT]WPA: Event-EVENT_ASSOC \r\n\r\n");
            wpa_clr_key_info();
            wpa_supplicant_event_assoc(wpa_s, NULL);
            wpa_supplicant_set_state(wpa_s, WPA_ASSOCIATED);
            //I0064  0000318_add_begin
            if(hap_temp->hap_en && (hap_temp->hap_ap_info->rsn_ie[0]==0) && (hap_temp->hap_ap_info->wpa_ie[0]==0) )
            {
                hiddenap_complete();
            }
            //I0064  0000318_add_end
			break;

	    case MLME_EVT_DISASSOC:
            msg_print(LOG_LOW_LEVEL, "[EVT]WPA: Event-EVENT_DISASSOC \r\n");
            wpa_supplicant_event_disassoc(wpa_s, WLAN_REASON_DEAUTH_LEAVING);
            wpa_supplicant_set_state(wpa_s, WPA_DISCONNECTED);
            /* Set successfully connect info to Auto Connect list */
            //I0060 0000794
            if (get_auto_connect_mode() == AUTO_CONNECT_MANUAL ||
                get_auto_connect_mode() == AUTO_CONNECT_DIRECT) {
                set_auto_connect_mode(AUTO_CONNECT_ENABLE);
            }
			break;

		case MLME_EVT_SCAN_RESULTS:
            msg_print(LOG_LOW_LEVEL, "[EVT]WPA: Event-EVENT_SCAN_RESULTS \r\n");
            //I0060 0000635 if (wpa_s->wpa_state != WPA_COMPLETED) {
            if (!(wpa_s->wpa_state == WPA_COMPLETED || wpa_s->wpa_state == WPA_ASSOCIATED)) { //I0060 0000635
                wpa_supplicant_set_state(wpa_s, WPA_INACTIVE);
            }
            wpa_cli_showscanresults_handler(NULL, NULL);
			break;

		case MLME_EVT_AUTH:
            msg_print(LOG_LOW_LEVEL, "[EVT]WPA: Event-EVENT_AUTH \r\n");
			break;

		case MLME_EVT_DEAUTH:
            msg_print(LOG_LOW_LEVEL, "[EVT]WPA: Event-EVENT_DEAUTH \r\n");
            wpa_supplicant_set_state(wpa_s, WPA_DISCONNECTED);
            wpa_clr_key_info();
            /* Set successfully connect info to Auto Connect list */
            //I0060 0000635 if (get_auto_connect_mode() == AUTO_CONNECT_MANUAL) {
            //I0060 0000794
            if ((get_auto_connect_mode() == AUTO_CONNECT_MANUAL ||
                 get_auto_connect_mode() == AUTO_CONNECT_DIRECT)) {
                set_auto_connect_mode(AUTO_CONNECT_ENABLE);
            }
			break;

        case MLME_EVT_AUTH_TIMED_OUT:
            msg_print(LOG_LOW_LEVEL, "[EVT]WPA: Event-MLME_EVT_AUTH_TIMED_OUT \r\n");
            wpa_supplicant_set_state(wpa_s, WPA_INACTIVE);
            /* Set successfully connect info to Auto Connect list */
            //I0060 0000794
            if (get_auto_connect_mode() == AUTO_CONNECT_MANUAL ||
                get_auto_connect_mode() == AUTO_CONNECT_DIRECT) {
                set_auto_connect_mode(AUTO_CONNECT_ENABLE);
            }
            break;

		case MLME_EVT_EAPOL_RX:
            if (wpa_s->wpa_state == WPA_ASSOCIATED ||
                wpa_s->wpa_state == WPA_4WAY_HANDSHAKE ||
                wpa_s->wpa_state == WPA_GROUP_HANDSHAKE ||
                wpa_s->wpa_state == WPA_COMPLETED)
            {
                wpa_supplicant_event_eapol_rx(wpa_s);
            }
            break;

        case MLME_EVT_ASSOC_TIMED_OUT:
            msg_print(LOG_LOW_LEVEL, "[EVT]WPA: Event-MLME_EVT_ASSOC_TIMED_OUT \r\n");
            /* Set successfully connect info to Auto Connect list */
            //I0060 0000794
            if (get_auto_connect_mode() == AUTO_CONNECT_MANUAL ||
                get_auto_connect_mode() == AUTO_CONNECT_DIRECT) {
                set_auto_connect_mode(AUTO_CONNECT_ENABLE);
            }
            break;

        case MLME_EVT_ASSOC_REJECT:
            msg_print(LOG_LOW_LEVEL, "[EVT]WPA: Event-MLME_EVT_ASSOC_REJECT \r\n");
            /* Set successfully connect info to Auto Connect list */
            //I0064  0000318_add_begin
            if (hap_temp->hap_en){
                wifi_sta_join_for_hiddenap();
                break;
            }
            //I0064  0000318_add_end
            //I0060 0000794
            if (get_auto_connect_mode() == AUTO_CONNECT_MANUAL ||
                get_auto_connect_mode() == AUTO_CONNECT_DIRECT) {
                set_auto_connect_mode(AUTO_CONNECT_ENABLE);
            }
            break;
        case MLME_EVT_AUTO_CONNECT:
            g_AutoConnect.retryCount++;
            control_auto_connect();
            break;            
        case MLME_EVT_AUTO_CONNECT_START:
            msg_print(LOG_LOW_LEVEL, "Supplicant Receive EVT_AUTO_CONNECT_START \r\n");
            control_auto_connect();
            break;

        //I0051 0000908_add_begin
        case MLME_EVT_MIC_FAILURE:
        {
            union wpa_event_data tData = {0};

            tData.michael_mic_failure.unicast = 1;
            tData.michael_mic_failure.src = NULL;

            msg_print(LOG_HIGH_LEVEL, "Supplicant Receive MLME_EVT_MIC_FAILURE \r\n");

            wpa_supplicant_event_michael_mic_failure(wpa_s, &tData);
            break;
        }
        //I0051 0000908_add_end

		default:
			break;
	}
}

RET_DATA supplicant_task_create_fp_t supplicant_task_create;
RET_DATA supplicant_task_init_fp_t supplicant_task_init;
RET_DATA supplicant_task_fp_t supplicant_task;
RET_DATA supplicant_task_send_fp_t supplicant_task_send;
RET_DATA supplicant_task_evt_handle_fp_t supplicant_task_evt_handle;

/*
   Interface Initialization: Supplicant Task
 */
void wpa_supplicant_task_func_init(void)
{
    supplicantTaskStackSize = OS_TASK_STACK_SIZE_SUPPLICANT;
    supplicantTaskPriority = OS_TASK_PRIORITY_SUPPLICANT;

    supplicant_task_create = supplicant_task_create_impl;
    supplicant_task_init = supplicant_task_init_impl;
    supplicant_task = supplicant_task_impl;
    supplicant_task_send = supplicant_task_send_impl;
    supplicant_task_evt_handle = supplicant_task_evt_handle_impl;
}

