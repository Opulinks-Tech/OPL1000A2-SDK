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

#include <stdio.h>
#include <stdlib.h>

#include "../../wpa_supplicant/config.h"
#include "controller_wifi_com.h"
#include "driver_netlink.h"
#include "wpa_supplicant_i.h"
#include "driver_netlink_patch.h"
#include "events_netlink.h"

extern struct wpa_supplicant *wpa_s;
extern u8 g_bssid[6];
extern u8 g_fastconn;
extern u8 g_wifi_reconnection_counter;

static const char *TAG = "DRV";

Boolean wpa_driver_netlink_scan_patch(int mode)
{
    if (wifi_ctrl_state_get() == WIFI_CTRL_SCANNING_ST ||
        wifi_ctrl_state_get() == WIFI_CTRL_CONNECTING_ST ||
        wifi_ctrl_state_get() == WIFI_CTRL_CONNECTED_SCAN_ST) {
        log_print(LOG_MED_LEVEL, TAG, "WPA: Ignore, is processing\r\n");
        return FALSE;
    }
    else {
        if (wifi_ctrl_state_get() == WIFI_CTRL_CONNECTED_ST) {
            wifi_ctrl_state_set(WIFI_CTRL_CONNECTED_SCAN_ST);
        }
        else {
            wifi_ctrl_state_set(WIFI_CTRL_SCANNING_ST);
            wpa_supplicant_set_state(wpa_s, WPA_SCANNING);
        }
        wifi_scan_req(mode);
    }
    return TRUE;
}

Boolean wpa_driver_netlink_scan_by_cfg_patch(void *cfg)
{
    if (cfg == NULL) return FALSE;
    if (wifi_ctrl_state_get() == WIFI_CTRL_SCANNING_ST ||
        wifi_ctrl_state_get() == WIFI_CTRL_CONNECTING_ST ||
        wifi_ctrl_state_get() == WIFI_CTRL_CONNECTED_SCAN_ST) {
        log_print(LOG_MED_LEVEL, TAG, "WPA: Ignore, is processing\r\n");
        return FALSE;
    }
    else {
        if (wifi_ctrl_state_get() == WIFI_CTRL_CONNECTED_ST) {
            wifi_ctrl_state_set(WIFI_CTRL_CONNECTED_SCAN_ST);
        }
        else {
            wifi_ctrl_state_set(WIFI_CTRL_SCANNING_ST);
            wpa_supplicant_set_state(wpa_s, WPA_SCANNING);
        }
        wifi_scan_req_by_cfg(cfg);
    }
    return TRUE;
}

Boolean wpa_driver_netlink_connect_patch(struct wpa_config * conf)
{
    int ret = 0;
    scan_info_t *pInfo = NULL;
    int i;
    uint32_t u32Remain = 0;
    hap_control_t *hap_temp;
    hap_temp=get_hap_control_struct();
    
    if (conf == NULL) return FALSE;
    if (conf->ssid == NULL) return FALSE;
    
    if (get_wifi_certified_mode() == WIFI_CERTIFIED) {
        // get AP information
        pInfo = get_target_ap_record(conf->ssid->bssid, (char*)conf->ssid->ssid);
        if (pInfo == NULL) {
            log_print(LOG_HIGH_LEVEL, TAG, "WPA: AP index is null \r\n");
            ret = -1;
            goto done;
        }
        
        // check AP security mode
        if (!target_ap_security_mode_chk(pInfo)) {
            ret = -1;
            goto done;
        }
        
        // check RSNE format
        if (!target_ap_rsne_format_chk(pInfo)) {
            ret = -1;
            goto done;
        }
        
#if 0 // for debug use (Wifi Certified 5.2.2/3 handles RSNE unexpected values test)
        if (!test_dummy_rsne_format(pInfo)) {
            ret = -1;
            goto done;
        }
#endif
    }
    
    if (wifi_ctrl_state_get() == WIFI_CTRL_SCANNING_ST ||
        wifi_ctrl_state_get() == WIFI_CTRL_CONNECTING_ST ||
        wifi_ctrl_state_get() == WIFI_CTRL_CONNECTED_SCAN_ST) {
        log_print(LOG_MED_LEVEL, TAG, "WPA: Ignore, is processing\r\n");
        ret = -1;
        goto done;
    }
    
    if (wifi_ctrl_state_get() == WIFI_CTRL_CONNECTED_ST) {
        if (!connection_connected_ctrl()) {
            ret = -1;
            goto done;
        }
    }
    
    if (conf->ssid->ssid == NULL) {
        if(get_wifi_certified_mode() == WIFI_CERTIFIED)
        {
            if(wpa_supplicant_is_bssid_blocked_by_mic_error(conf->ssid->bssid, &u32Remain))
            {
                log_print(LOG_HIGH_LEVEL, TAG, "WPA: BSSID is blocked by MIC failure (%u seconds remaining)\r\n", u32Remain);
                ret = -1;
                goto done;
            }
        }
        
        wpa_supplicant_set_state(wpa_s, WPA_ASSOCIATING);
        
        ret = wifi_sta_join(conf->ssid->bssid);
    } 
    else {
        pInfo = wifi_get_scan_record_by_ssid((char*)(conf->ssid->ssid));
        if (pInfo == NULL) {
            if (hap_temp->hap_final_index!=0){
                hap_temp->hap_en=1;
            }
            else{
                ret = -1;
                goto done; // not found target AP in the list
            }
        }

        if(get_wifi_certified_mode() == WIFI_CERTIFIED)
        {
            if(wpa_supplicant_is_bssid_blocked_by_mic_error(pInfo->bssid, &u32Remain))
            {
                log_print(LOG_HIGH_LEVEL, TAG, "WPA: BSSID is blocked by MIC failure (%u seconds remaining)\r\n", u32Remain);
                ret = -1;
                goto done;
            }
        }

        for (i=0; i<ETH_ALEN; i++) {
            g_bssid[i] = pInfo->bssid[i];
        }
        
        if (hap_temp->hap_en) {
            hap_temp->hap_ap_info = malloc(sizeof(auto_conn_info_t));
            wifi_sta_join_for_hiddenap();
            goto done;
        }
        
        wpa_supplicant_set_state(wpa_s, WPA_ASSOCIATING);
        g_fastconn = 0;
        g_wifi_reconnection_counter = 0;
        ret = wifi_sta_join(pInfo->bssid);
    }

done:
    if(ret == 0) return TRUE;
    return FALSE;
}

void wpa_driver_func_init_patch(void)
{
    wpa_driver_netlink_scan        = wpa_driver_netlink_scan_patch;
    wpa_driver_netlink_scan_by_cfg = wpa_driver_netlink_scan_by_cfg_patch;
    wpa_driver_netlink_connect     = wpa_driver_netlink_connect_patch;
}
