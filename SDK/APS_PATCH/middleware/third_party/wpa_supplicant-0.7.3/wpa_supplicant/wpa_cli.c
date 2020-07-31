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
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "controller_wifi_com.h"
#include "wpa_cli.h"
#include "driver_netlink.h"

#include "controller_wifi_com_patch.h"
#include "wpa_cli_patch.h"

static int wpa_cli_connect_patch(struct wpa_config * conf)
{
    if (conf == NULL) return FALSE;
    if (conf->ssid == NULL) return FALSE;
    
    scan_info_t *pInfo = NULL;
    hap_control_t *hap_temp = get_hap_control_struct();
    
    pInfo = get_target_ap_record(conf->ssid->bssid, (char*)conf->ssid->ssid);
    if (pInfo == NULL) {
        if (hap_temp->hap_final_index != 0){
            // Security check move to wifi_get_scan_record_by_ssid() from here.
            if (!wpa_driver_netlink_connect(conf)) return FALSE;
            return TRUE;
        }
        else {
            return FALSE;
        }
    }
    
    if (!target_ap_security_mode_chk(pInfo)) return FALSE;
    if (!wpa_driver_netlink_connect(conf)) return FALSE;
    
    return TRUE;
}

static int wpa_cli_scan_by_cfg_patch(void *cfg)
{
    if (cfg == NULL) return FALSE;

    S_WIFI_MLME_SCAN_CFG *scan_cfg = (S_WIFI_MLME_SCAN_CFG *)cfg;

    /* Duration */
    if (scan_cfg->u32ActiveScanDur < SCAN_MIN_DURATION_TIME || 
        scan_cfg->u32ActiveScanDur > SCAN_MAX_NUM_OF_DUR_TIME) {
        scan_cfg->u32ActiveScanDur = SCAN_ACTIVE_MIN_DUR_TIME_DEF;
    }
    
    if (scan_cfg->u32PassiveScanDur < SCAN_MIN_DURATION_TIME ||
        scan_cfg->u32PassiveScanDur > SCAN_MAX_NUM_OF_DUR_TIME) {
        scan_cfg->u32PassiveScanDur = SCAN_PASSIVE_MIN_DUR_TIME_DEF;
    }
    
    /* channel */
    if (scan_cfg->u8Channel > WIFI_MLME_SCAN_MAX_NUM_CHANNELS) {
        scan_cfg->u8Channel = WIFI_MLME_SCAN_ALL_CHANNELS;
    }
    
    /* re-send counter */
    if (scan_cfg->u8ResendCnt == 0) {
        scan_cfg->u8ResendCnt = SCAN_PROBE_REQ_COUNTERS_DEF;
    }
    
    /* mac address */
    if (is_broadcast_ether_addr(scan_cfg->u8aBssid))
        return FALSE;
    
    if (is_multicast_ether_addr(scan_cfg->u8aBssid))
        return FALSE;

    return wpa_driver_netlink_scan_by_cfg(scan_cfg);
}

void wpa_cli_func_init_patch(void)
{
    wpa_cli_connect            = wpa_cli_connect_patch;
    wpa_cli_scan_by_cfg        = wpa_cli_scan_by_cfg_patch;
}
