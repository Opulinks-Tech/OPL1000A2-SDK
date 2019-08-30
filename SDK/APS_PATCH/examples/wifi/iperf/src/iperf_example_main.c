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
#include "console.h"
#include "cmd_parser.h"
#include "event_loop.h"
#include "event_groups.h"
#include "lwip_helper.h"
#include "iperf_example_main.h"
#include "iperf.h"
#include "sys_common_api.h"
#include "wifi_api.h"
#include "wifi_types.h"
#include "wifi_event.h"
#include "wifi_event_handler.h"
#include "msg.h"
#include "opulinks_log.h"

/********************************************
Declaration of Global Variables & Functions
********************************************/
EventGroupHandle_t wifi_event_group;
const int CONNECTED_BIT0    = 0x01;
const int DISCONNECTED_BIT1 = 0x02;

extern T_TracerTaskInfo *g_ptTracerExtTaskInfo;

/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
osThreadId app_task_id;
static const char *TAG="iperf";

static int wifi_event_handler_cb(wifi_event_id_t event_id, void *data, uint16_t length)
{
    switch(event_id) {
    case WIFI_EVENT_STA_CONNECTED:
        LOGI_DRCT(TAG, "Wi-Fi Connected");
        lwip_net_start(WIFI_MODE_STA);
        break;
    case WIFI_EVENT_STA_DISCONNECTED:
        LOGI_DRCT(TAG, "Wi-Fi Disonnected");
        xEventGroupClearBits(wifi_event_group, CONNECTED_BIT0);
        xEventGroupSetBits(wifi_event_group, DISCONNECTED_BIT1);
        break;
    case WIFI_EVENT_SCAN_COMPLETE:
        break;
    case WIFI_EVENT_STA_GOT_IP:
        LOGI_DRCT(TAG, "Wi-Fi Got IP");
        xEventGroupClearBits(wifi_event_group, DISCONNECTED_BIT1);
        xEventGroupSetBits(wifi_event_group, CONNECTED_BIT0);
        lwip_get_ip_info("st1");
        break;
    case WIFI_EVENT_STA_CONNECTION_FAILED:
        LOGI_DRCT(TAG, "Wi-Fi Connected failed");
        break;
    default:
        LOGW_DRCT(TAG, "Unknown Event %d", event_id);
        break;
    }
    return 0;
}

static int console_init(void)
{
    console_cmd_register(&cmd_parser_func);
    return 0;
}

static void initial_internal_log_level(void)
{
    int i;
    for (i=0; i<15; i++) {
        tracer_log_level_set(i, 0);
    }
    
    tracer_log_level_set(8, 4);
    
    strncpy(g_ptTracerExtTaskInfo[0].baName, IPERF_CLIENT_TASK_NAME, 
           (strlen(IPERF_CLIENT_TASK_NAME) > 15) ? 15 : strlen(IPERF_CLIENT_TASK_NAME));
    strncpy(g_ptTracerExtTaskInfo[1].baName, IPERF_SERVER_TASK_NAME, 
           (strlen(IPERF_SERVER_TASK_NAME) > 15) ? 15 : strlen(IPERF_SERVER_TASK_NAME));
    strncpy(g_ptTracerExtTaskInfo[2].baName, IPERF_REPORT_TASK_NAME, 
           (strlen(IPERF_REPORT_TASK_NAME) > 15) ? 15 :  strlen(IPERF_REPORT_TASK_NAME));
    
    g_ptTracerExtTaskInfo[0].bLevel = 3;
    g_ptTracerExtTaskInfo[1].bLevel = 3;
    g_ptTracerExtTaskInfo[2].bLevel = 7;
} 

void initial_wifi_config(void)
{
    wifi_event_group = xEventGroupCreate();
    wifi_auto_connect_set_mode(0); //Disable Auto Connect
}

void initial_network(void *args)
{
    /* initial internal log level */
    initial_internal_log_level();
    
    /* Tcpip stack and net interface initialization,  dhcp client process initialization. */
    lwip_network_init(WIFI_MODE_STA);
    
    vTaskDelete(NULL);
}

void creat_task_to_init_network(void)
{
    osThreadDef_t task_def;
    
    /* Create task */
    task_def.name = "user_app";
    task_def.stacksize = OS_TASK_STACK_SIZE_APP;
    task_def.tpriority = OS_TASK_PRIORITY_APP;
    task_def.pthread = initial_network;
    app_task_id = osThreadCreate(&task_def, (void*)NULL);

    if(app_task_id == NULL)
    {
        LOGE(TAG, "user_app Task create fail \r\n");
    }
    else
    {
        LOGI(TAG, "user_app Task create successful \r\n");
    }
}

void AppInit(void)
{
    /* Event Loop Initialization */
    wifi_event_loop_init((wifi_event_cb_t)wifi_event_handler_cb);

    /* Initialize wifi stack and register wifi init complete event handler */
    wifi_init(NULL, NULL);
    
    /* Initial Wifi Setting for this example */
    initial_wifi_config();
    
    /* Initial Network process */
    creat_task_to_init_network();
    
    /* Initial/Register commands */
    console_init();
    
    printf("\n ===========================================\n");
    printf(" |     Start to test WiFi throughput       |\n");
    printf(" |  1. Print 'help' to get command list    |\n");
    printf(" |  2. Setup WiFi connection               |\n");
    printf(" |  3. Run iperf to test throughput        |\n");
    printf(" ===========================================\n");
}
