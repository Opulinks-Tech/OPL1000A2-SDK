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
#include <stdio.h>
#include <string.h>
#include "wifi_cli.h"
#include "console.h"
#include "FreeRTOS.h"
#include "event_groups.h"
#include "netif.h"
#include "opulinks_log.h"
#include "wifi_api.h"
#include "sys_common_api.h"

extern EventGroupHandle_t wifi_event_group;
extern const int CONNECTED_BIT0;

static const char *TAG="cmd_wifi";

int wifi_cmd_sta(int argc, char** argv)
{
    LOGI_DRCT(TAG, "connecting to %s", argv[1]);
    
    wifi_config_t wifi_config = {0};
    wifi_get_config(WIFI_MODE_STA, &wifi_config);
    
    memcpy(&wifi_config.sta_config.ssid, argv[1], strlen(argv[1]));
    wifi_config.sta_config.ssid_length = strlen(argv[1]);
    
    memcpy(&wifi_config.sta_config.password, argv[2], strlen(argv[2]));
    wifi_config.sta_config.password_length = strlen(argv[2]);
    
    int bits = xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT0, 0, 1, 0);
    if (bits & CONNECTED_BIT0) {
        xEventGroupClearBits(wifi_event_group, CONNECTED_BIT0);
    }
    
    wifi_connection_connect(&wifi_config);
    return 0;
}

int wifi_cmd_scan(int argc, char** argv)
{
    LOGI_DRCT(TAG, "sta scanning");
    
    wifi_scan_config_t scan_config;
    memset(&scan_config, 0, sizeof(scan_config));
    scan_config.scan_type = WIFI_SCAN_TYPE_MIX;
    wifi_scan_start(&scan_config, NULL);
    return 0;
}

int wifi_cmd_query(int argc, char **argv)
{
    wifi_ap_record_t ap_info = {0};
    struct netif *p_netif = NULL;
    
    LOGI_DRCT(TAG, "query wifi states");
    int bits = xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT0, 0, 1, 0);
    if (bits & CONNECTED_BIT0) {
        wifi_sta_get_ap_info(&ap_info);
        p_netif = netif_find("st1");
        
        LOGI_DRCT(TAG, "Wifi Connected.");
        LOGI_DRCT(TAG, "AP SSID:  %s", ap_info.ssid);
        LOGI_DRCT(TAG, "IP:       %s", ip4addr_ntoa(&p_netif->ip_addr.u_addr.ip4));
        LOGI_DRCT(TAG, "Netmask:  %s", ip4addr_ntoa(&p_netif->netmask.u_addr.ip4));
        LOGI_DRCT(TAG, "Gateway:  %s", ip4addr_ntoa(&p_netif->gw.u_addr.ip4));
    }
    else {
        LOGI_DRCT(TAG, "Disconnected");
    }
    
    return 0;
}

int wifi_cmd_disconnect(int argc, char **argv)
{
    wifi_connection_disconnect_ap();
    return 0;
}

int wifi_cmd_data_rate(int argc, char **argv)
{
    wifi_mac_data_rate_t data_rate;
    LOGI_DRCT(TAG, "set wifi Tx fix data rate");
    
    data_rate = (wifi_mac_data_rate_t)atoi(argv[1]);
    wifi_config_set_mac_tx_data_rate(data_rate);
    
    LOGI_DRCT(TAG, "current setting : %d", data_rate);
    LOGI_DRCT(TAG, "  0:DTO, 1:1M, 2:2M, 3:5.5M, 4:11M");
    
    return 0;
}

static int hex2num(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return -1;
}

/**
 * hwaddr_aton2 - Convert ASCII string to MAC address (in any known format)
 * @txt: MAC address as a string (e.g., 00:11:22:33:44:55 or 0011.2233.4455)
 * @addr: Buffer for the MAC address (ETH_ALEN = 6 bytes)
 * Returns: Characters used (> 0) on success, -1 on failure
 */
static int hwaddr_aton2(const char *txt, uint8_t *addr)
{
    int i;
    const char *pos = txt;

    for (i = 0; i < 6; i++) {
        int a, b;

        while (*pos == ':' || *pos == '.' || *pos == '-')
            pos++;

        a = hex2num(*pos++);
        if (a < 0)
            return -1;
        b = hex2num(*pos++);
        if (b < 0)
            return -1;
        *addr++ = (a << 4) | b;
    }

    return pos - txt;
}

int wifi_cmd_sta_mac_addr(int argc, char **argv)
{
    uint8_t mac[6] = {0};
    
    if (argc == 1) {
        wifi_config_get_mac_address(WIFI_MODE_STA, &mac[0]);
        LOGI_DRCT(TAG, "Mac addres : %02x:%02x:%02x:%02x:%02x:%02x", 
                  mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    }
    else if (argc > 1) {
        if (memcmp(argv[1], "set", 3)) {
            LOGE_DRCT(TAG, "Invalid parameter.");
            return -1;
        }
        
        hwaddr_aton2(argv[2], mac);
        
        if (mac_addr_set_config_source(MAC_IFACE_WIFI_STA, MAC_SOURCE_FROM_FLASH)) {
            LOGE_DRCT(TAG, "Set Wifi mac cource failed.");
            return -1;
        }
        
        if (wifi_config_set_mac_address(WIFI_MODE_STA, &mac[0])) {
            LOGE_DRCT(TAG, "Set Wifi mac addres failed.");
            return -1;
        }
        LOGI_DRCT(TAG, "OK");
    }

    return 0;
}
