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

// Sec 1: Include File
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cmsis_os.h"
#include "event_loop.h"
#include "wifi_api.h"
#include "wifi_event.h"
#include "wifi_event_handler.h"
#include "lwip_helper.h"
#include "wpa2_station_app.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define WIFI_READY_TIME 2000


osThreadId g_tAppThread_wifi;

osMessageQId g_tWifiAppMessageQ;
osPoolId g_tWifiAppMemPoolId;

extern osMessageQId g_tAppMessageQ;
extern osPoolId g_tAppMemPoolId;

int wifi_event_sta_connected_handler(wifi_event_t event, uint8_t *data, uint32_t length)
{
    return wifi_station_connected_event_handler(data);
}

int wifi_event_sta_disconnected_handler(wifi_event_t event, uint8_t *data, uint32_t length)
{
    return wifi_station_disconnected_event_handler(data);
}

void wifi_wait_ready(void)
{
    /* wait a while for system ready */
    osDelay(WIFI_READY_TIME);
}

int wifi_do_scan(int mode)
{
    wifi_scan_config_t scan_config;
    memset(&scan_config, 0, sizeof(scan_config));
    wifi_scan_start(&scan_config, NULL);
    return 0;
}

int wifi_connection(void)
{
    int iRet = -1;
    wifi_config_t wifi_config = {0};
    wifi_scan_list_t *p_scan_list = NULL;
    int i = 0;
    int isMatched = 0;
		
    p_scan_list = (wifi_scan_list_t *)malloc(sizeof(wifi_scan_list_t));

    if(p_scan_list == NULL)
    {
        goto done;
    }

    memset(p_scan_list, 0, sizeof(wifi_scan_list_t));

    /* Read Confguration */
    wifi_get_config(WIFI_MODE_STA, &wifi_config);

    /* Get APs list */
    wifi_scan_get_ap_list(p_scan_list);

    /* Search if AP matched */
    for (i=0; i< p_scan_list->num; i++) {
        if (memcmp(p_scan_list->ap_record[i].bssid, wifi_config.sta_config.bssid, WIFI_MAC_ADDRESS_LENGTH) == 0)
        {
            isMatched = 1;
            break;
        }

        if (memcmp(p_scan_list->ap_record[i].ssid, wifi_config.sta_config.ssid, wifi_config.sta_config.ssid_length) == 0)
        {
            isMatched = 1;
            break;
        }
    }

    if(isMatched == 1) {
        /* Wi-Fi Connection */
        wifi_connection_connect(&wifi_config);
        g_AP_connect_result = true;

    } else {
        /* Scan Again */
        wifi_do_scan(WIFI_SCAN_TYPE_MIX);
        g_AP_connect_result = false;
    }
    g_AP_connect_complete = true;
    iRet = 0;

done:
    if(p_scan_list)
    {
        free(p_scan_list);
    }
    return iRet;
}

int wifi_event_handler_cb(wifi_event_id_t event_id, void *data, uint16_t length)
{
    S_MessageQ tMsg;

    switch(event_id) {
    case WIFI_EVENT_STA_START:
        printf("\r\nWi-Fi Start \r\n");
        wifi_wait_ready();
        wifi_do_scan(WIFI_SCAN_TYPE_MIX);
        break;
    case WIFI_EVENT_STA_CONNECTED:
        lwip_net_start(WIFI_MODE_STA);
        printf("\r\nWi-Fi Connected \r\n");

        tMsg.ulEventId = UAER_WIFI_APP_CONNECT_COMPLETE;
        tMsg.bConnectResult = true;        
        wifi_AppMessageQSend(&tMsg, g_tAppMessageQ,g_tAppMemPoolId); 

        break;
    case WIFI_EVENT_STA_DISCONNECTED:
        printf("\r\nWi-Fi Disconnected \r\n");
        wifi_do_scan(WIFI_SCAN_TYPE_MIX);

        tMsg.ulEventId = UAER_WIFI_APP_CONNECT_COMPLETE;
        tMsg.bConnectResult = false;        
        wifi_AppMessageQSend(&tMsg, g_tAppMessageQ,g_tAppMemPoolId); 

        break;
    case WIFI_EVENT_SCAN_COMPLETE:
        printf("\r\nWi-Fi Scan Done \r\n");
        wifi_connection();
        break;
    case WIFI_EVENT_STA_GOT_IP:
        printf("\r\nWi-Fi Got IP \r\n");
        lwip_get_ip_info("st1");
        break;
    case WIFI_EVENT_STA_CONNECTION_FAILED:
        printf("\r\nWi-Fi Connected failed\r\n");
        wifi_do_scan(WIFI_SCAN_TYPE_MIX);
        break;
    default:
        printf("\r\n Unknown Event %d \r\n", event_id);
        break;
    }
    return 0;
}

void user_wifi_app_entry(void *args)
{
    osEvent tEvent;
    S_MessageQ *ptMsgPool;

    /* Tcpip stack and net interface initialization,  dhcp client process initialization. */
    lwip_network_init(WIFI_MODE_STA);

    /* Waiting for connection & got IP from DHCP server */
    lwip_net_ready();



    while (1) {

        tEvent = osMessageGet(g_tWifiAppMessageQ, osWaitForever);
        if (tEvent.status != osEventMessage)
        {
            printf("To receive the message from g_tWifiAppMessageQ is fail.\n");
            continue;
        }    
        // get the content of message
        ptMsgPool = (S_MessageQ *)tEvent.value.p;    

       if(ptMsgPool->ulEventId == UAER_WIFI_APP_RE_CONNECT)
       {
            wifi_wait_ready();
            wifi_do_scan(WIFI_SCAN_TYPE_MIX);          
       }
        //lwip_get_ip_info("st1");
        osDelay(2000);
        
    }
}

void WifiAppInit(void)
{
    osThreadDef_t task_def;
    osMessageQDef_t tMessageDef;
    osPoolDef_t tMemPoolDef;

    wifi_init_config_t int_cfg = {.event_handler = (wifi_event_notify_cb_t)&wifi_event_loop_send, .magic = 0x1F2F3F4F};
    wifi_config_t wifi_config = {0};

    unsigned char bssid[WIFI_MAC_ADDRESS_LENGTH] = {0x74, 0xda, 0xda, 0xe7, 0x08, 0xf1};

    /* Register internal wifi event default handler */
    wifi_register_event_handler(WIFI_EVENT_STA_CONNECTED, (wifi_event_handler_t)wifi_event_sta_connected_handler);
    wifi_register_event_handler(WIFI_EVENT_STA_DISCONNECTED, (wifi_event_handler_t)wifi_event_sta_disconnected_handler);

    /* Event Loop Initialization */
    wifi_event_loop_init((wifi_event_cb_t)wifi_event_handler_cb);

    /* Initialize wifi stack and register wifi init complete event handler */
    wifi_init(&int_cfg, NULL);

    /* Set user's configuration */
    strcpy((char *)wifi_config.sta_config.ssid, WIFI_SSID);
    strcpy((char *)wifi_config.sta_config.password, WIFI_PASSWORD);
    wifi_config.sta_config.ssid_length = strlen(WIFI_SSID);
    wifi_config.sta_config.password_length = strlen(WIFI_PASSWORD);
    memcpy(wifi_config.sta_config.bssid, bssid, WIFI_MAC_ADDRESS_LENGTH);

    wifi_set_config(WIFI_MODE_STA, &wifi_config);

    /* Wi-Fi operation start */
    wifi_start();
    
    /* Create task */
    task_def.name = "user_app";
    task_def.stacksize = OS_TASK_STACK_SIZE_APP;
    task_def.tpriority = OS_TASK_PRIORITY_APP;
    task_def.instances = 0;                           // reserved, it is no used
    task_def.pthread = user_wifi_app_entry;
    g_tAppThread_wifi = osThreadCreate(&task_def, NULL);
    if(g_tAppThread_wifi == NULL)
    {
        printf("user_app Task create fail \r\n");
    }
    else
    {
        printf("user_app Task create successful \r\n");
    }
    
    // create the message queue for AppMessageQ
    tMessageDef.queue_sz = APP_MESSAGE_Q_SIZE;          // number of elements in the queue
    tMessageDef.item_sz = sizeof(S_MessageQ);           // size of an item
    tMessageDef.pool = NULL;                            // reserved, it is no used
    g_tWifiAppMessageQ = osMessageCreate(&tMessageDef, g_tAppThread_wifi);
    if (g_tAppMessageQ == NULL)
    {
        printf("To create the message queue for g_tWifiAppMessageQ is fail.\n");
    }
    else
    {
        printf("To create the message queue for g_tWifiAppMessageQ successful \r\n");
    }
    
    // create the memory pool for AppMessageQ
    tMemPoolDef.pool_sz = APP_MESSAGE_Q_SIZE;           // number of items (elements) in the pool
    tMemPoolDef.item_sz = sizeof(S_MessageQ);           // size of an item
    tMemPoolDef.pool = NULL;                            // reserved, it is no used
    g_tWifiAppMemPoolId = osPoolCreate(&tMemPoolDef);
    if (g_tAppMemPoolId == NULL)
    {
        printf("To create the memory pool for g_tWifiAppMemPoolId is fail.\n");
    }      
    else
    {
        printf("To create the memory pool for g_tWifiAppMemPoolId successful \r\n");
    }
    
}

/*************************************************************************
* FUNCTION:
*   wifi_AppMessageQSend
*
* DESCRIPTION:
*   send the message into AppMessageQ
*
* PARAMETERS
*   1. ptMsg    : [In] the pointer of message content
*
* RETURNS
*   osOK        : successful
*   osErrorOS   : fail
*
*************************************************************************/
osStatus wifi_AppMessageQSend(S_MessageQ *ptMsg, osMessageQId message_id, osPoolId pool_id)
{
    osStatus tRet = osErrorOS;
    S_MessageQ *ptMsgPool;
    
    // allocate the memory pool
    ptMsgPool = (S_MessageQ *)osPoolCAlloc(pool_id);
    if (ptMsgPool == NULL)
    {
        printf("To allocate the memory pool for AppMessageQ is fail.\n");
        goto done;
    }
    
    // copy the message content
    memcpy(ptMsgPool, ptMsg, sizeof(S_MessageQ));
    
    // send the message
    if (osOK != osMessagePut(message_id, (uint32_t)ptMsgPool, osWaitForever))
    {
        printf("To send the message for AppMessageQ is fail.\n");
        
        // free the memory pool
        osPoolFree(pool_id, ptMsgPool);
        goto done;
    }
    
    tRet = osOK;

done:
    return tRet;
}


