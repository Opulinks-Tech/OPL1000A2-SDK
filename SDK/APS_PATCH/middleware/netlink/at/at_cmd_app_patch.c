/******************************************************************************
*  Copyright 2019, Netlink Communication Corp.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Netlink Communication Corp. (C) 2019
******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "at_cmd_app.h"
#include "at_cmd_common.h"
#include "at_cmd_tcpip.h"
#include "at_cmd_msg_ext.h"
#include "event_loop.h"
#include "lwip_helper.h"
#include "wifi_api.h"

#include "at_cmd_app_patch.h"

typedef void (*at_cmd_wifi_hook_fp_t)(void);
typedef int  (*at_wifi_event_handler_cb_fp_t)(wifi_event_id_t event_id, void *data, uint16_t length);

extern at_cmd_wifi_hook_fp_t at_cmd_wifi_hook_adpt;
extern int at_wifi_event_handler_cb(wifi_event_id_t event_id, void *data, uint16_t length);
extern int at_wifi_trans_do_scan_impl(void);
extern void at_wifi_trans_reconnect_impl(void);
extern at_wifi_event_handler_cb_fp_t at_wifi_event_handler_cb_adpt;

extern volatile bool at_ip_mode;
extern int at_wifi_status;
extern int at_wifi_reason;
extern at_state_type_t mdState;

int at_wifi_event_handler_cb_patch(wifi_event_id_t event_id, void *data, uint16_t length)
{
    int reason = *((int*)data);

    switch(event_id) {
    case WIFI_EVENT_STA_START:
        lwip_network_init(WIFI_MODE_STA);
        wifi_auto_connect_start();
        break;
    case WIFI_EVENT_STA_CONNECTED:
        lwip_net_start(WIFI_MODE_STA);
        at_wifi_status = WIFI_EVENT_STA_CONNECTED;
        if (at_ip_mode != true) {
            at_msg_ext_wifi_connect(AT_MSG_EXT_ESPRESSIF, ERR_WIFI_CWJAP_DONE);
        }
        printf("\r\nWiFi Connect, reason %d\r\n", reason);
        break;
    case WIFI_EVENT_STA_DISCONNECTED:
        printf("\r\nWiFi Disconnect, reason %d\r\n", reason);
        lwip_net_stop(WIFI_MODE_STA);
        if (at_ip_mode != true) {
            at_msg_ext_wifi_dispatch_connect_reason(false, reason);
        }
        at_wifi_status = WIFI_EVENT_STA_DISCONNECTED;
        at_wifi_reason = STATION_CONNECT_FAIL;
        mdState = AT_STA_DISCONNECT;
        
        if (at_ip_mode == true) {
            at_wifi_trans_do_scan_impl();
        }
        break;
    case WIFI_EVENT_SCAN_COMPLETE:
        if (at_ip_mode != true) {
            at_msg_ext_wifi_show_all(NULL, NULL);
            at_uart1_printf("\r\nOK\r\n");
        } else {
            at_wifi_trans_reconnect_impl();
        }
        break;
    case WIFI_EVENT_STA_GOT_IP:
        mdState = AT_STA_GOT_IP;
        if (at_wifi_status != WIFI_EVENT_STA_GOT_IP) {
            at_wifi_status = WIFI_EVENT_STA_GOT_IP;
            if (at_ip_mode != true) {
                at_uart1_printf("WIFI GOT IP\r\n");
                at_uart1_printf("\r\nOK\r\n");
            }
        }
        printf("\r\nWiFi Obtained IP!\r\n");
        break;
    case WIFI_EVENT_STA_CONNECTION_FAILED:
        printf("\r\nWiFi Connected failed\r\n");
        
        if (mdState == AT_STA_GOT_IP) {
            lwip_net_stop(WIFI_MODE_STA);
        }
        
        if (at_ip_mode != true) {
            if (at_wifi_status == WIFI_EVENT_STA_CONNECTED ||
                at_wifi_status == WIFI_EVENT_STA_GOT_IP) {
                at_msg_ext_wifi_dispatch_connect_reason(true, reason);
            }
            else {
                at_msg_ext_wifi_dispatch_connect_reason(false, reason);
            }
        }
        
        at_wifi_status = WIFI_EVENT_STA_DISCONNECTED;
        at_wifi_reason = STATION_CONNECT_FAIL;
        
        if (at_ip_mode == true) {
            at_wifi_trans_do_scan_impl();
        }
        break;
    default:
        printf("\r\n Unknown Event %d \r\n", event_id);
        break;
    }
    return 0;
}

void at_cmd_wifi_hook_patch(void)
{
    wifi_init_config_t int_cfg = {.event_handler = (wifi_event_notify_cb_t)&at_wifi_event_handler_cb, .magic = 0x1F2F3F4F};

    /* Event Loop Initialization */
    wifi_event_loop_init((wifi_event_cb_t)at_wifi_event_handler_cb);

    /* Initialize wifi stack and register wifi init complete event handler */
    wifi_init(&int_cfg, NULL);

    /* Wi-Fi operation start */
    wifi_start();
    
    at_trans_save_link_task_create();
}

void at_cmd_app_func_preinit_patch(void)
{
    at_cmd_wifi_hook_adpt          = at_cmd_wifi_hook_patch;
    at_wifi_event_handler_cb_adpt  = at_wifi_event_handler_cb_patch;
}
