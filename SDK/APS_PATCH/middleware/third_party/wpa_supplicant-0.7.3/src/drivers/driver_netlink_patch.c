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
#include "wpa.h"
#include "driver_netlink_patch.h"
#include "events_netlink.h"
#include "Sha1-pbkdf2.h"

extern struct wpa_supplicant *wpa_s;
extern struct wpa wpa;
extern u8 g_bssid[6];
extern u8 g_fastconn;
extern u8 g_wifi_reconnection_counter;
extern u8 g_wpa_psk[32];
extern char g_passphrase[MAX_LEN_OF_PASSPHRASE]; 
extern auto_connect_cfg_t g_AutoConnect;

static const char *TAG = "DRV";

#define WPA_IS_PROCESSING "WPA: Ignore, is processing\r\n"

static char *get_ssid_by_bsssid(char *pbssid)
{
    int i;
    
    if (gScanReport.uScanApNum == 0)
        return NULL;

    for (i=0; i<gScanReport.uScanApNum; i++) {
        if(os_memcmp((void*)(pbssid), (void*)(gScanReport.pScanInfo[i].bssid), MAC_ADDR_LEN) == 0) {
            return gScanReport.pScanInfo[i].ssid;
        }
    }
    
    return NULL;
}

static int wpa_driver_cal_psk(char *pssid, uint16_t ssid_len, char *ppassphrase, uint16_t pass_len)
{
    if (pssid == NULL)
        return -1;
    
    if (ssid_len == 0)
        return -1;
    
    if (ppassphrase == NULL)
        return -1;
    
    if (pass_len == 0)
        return -1;
    
    pbkdf2_sha1(ppassphrase, pssid, ssid_len, 4096, wpa.psk, PMK_LEN);
    wpa_sm_set_pmk(wpa_s->wpa, wpa.psk, PMK_LEN);
    memcpy(&g_wpa_psk[0], wpa.psk, PMK_LEN);
    
    return 0;
}

Boolean wpa_driver_netlink_scan_patch(int mode)
{
    if (wifi_ctrl_state_get() == WIFI_CTRL_SCANNING_ST ||
        wifi_ctrl_state_get() == WIFI_CTRL_CONNECTING_ST ||
        wifi_ctrl_state_get() == WIFI_CTRL_CONNECTED_SCAN_ST) {
        log_print(LOG_MED_LEVEL, TAG, WPA_IS_PROCESSING);
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
        log_print(LOG_MED_LEVEL, TAG, WPA_IS_PROCESSING);
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
        
        if (strlen(g_passphrase) != 0) {
            char *pssid = get_ssid_by_bsssid((char *)conf->ssid->bssid);
            wpa_driver_cal_psk(pssid, strlen(pssid), g_passphrase, strlen(g_passphrase));
        }
        
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
        
        wpa_supplicant_set_state(wpa_s, WPA_ASSOCIATING);
        if (strlen(g_passphrase) != 0) {
            wpa_driver_cal_psk((char *)conf->ssid->ssid, strlen((char *)conf->ssid->ssid), g_passphrase, strlen(g_passphrase));
        }
        
        if (hap_temp->hap_en) {
            hap_temp->hap_ap_info = malloc(sizeof(auto_conn_info_t));
            wifi_sta_join_for_hiddenap_impl();
            goto done;
        }
        
        g_fastconn = 0;
        g_wifi_reconnection_counter = 0;
        
        ret = wifi_sta_join(pInfo->bssid);
    }

done:
    if(ret == 0) return TRUE;
    return FALSE;
}

Boolean wpa_driver_netlink_connect_from_ac_patch(u8 index)
{
    if (index > MAX_NUM_OF_AUTO_CONNECT) {
//        log_print(LOG_HIGH_LEVEL, TAG, "WPA: Invalid Parameter \r\n");
        return FALSE;
    }
    
    if (wifi_ctrl_state_get() == WIFI_CTRL_SCANNING_ST ||
        wifi_ctrl_state_get() == WIFI_CTRL_CONNECTING_ST ||
        wifi_ctrl_state_get() == WIFI_CTRL_CONNECTED_SCAN_ST) {
        log_print(LOG_MED_LEVEL, TAG, WPA_IS_PROCESSING);
        return FALSE;
    }
    
    wpa_sm_set_pmk(wpa_s->wpa, g_AutoConnect.pFCInfo[index].psk, PMK_LEN);
    
    control_auto_connect_by_index(index);
    return TRUE;
}

void wpa_driver_func_init_patch(void)
{
    wpa_driver_netlink_scan        = wpa_driver_netlink_scan_patch;
    wpa_driver_netlink_scan_by_cfg = wpa_driver_netlink_scan_by_cfg_patch;
    wpa_driver_netlink_connect     = wpa_driver_netlink_connect_patch;
    wpa_driver_netlink_connect_from_ac = wpa_driver_netlink_connect_from_ac_patch;
}
