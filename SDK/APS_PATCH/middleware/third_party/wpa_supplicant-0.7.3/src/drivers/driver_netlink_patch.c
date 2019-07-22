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

extern struct wpa_supplicant *wpa_s;

Boolean wpa_driver_netlink_scan_patch(int mode)
{
    //msg_print(LOG_HIGH_LEVEL, "wpa_driver_netlink_scan, mode:%d \r\n", mode);
    if (wifi_ctrl_state_get() == WIFI_CTRL_SCANNING_ST ||
        wifi_ctrl_state_get() == WIFI_CTRL_CONNECTING_ST ||
        wifi_ctrl_state_get() == WIFI_CTRL_CONNECTED_SCAN_ST) {
        msg_print(LOG_HIGH_LEVEL, "[DRV]WPA: Ignore this command since now is processing\r\n");
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
        msg_print(LOG_HIGH_LEVEL, "[DRV]WPA: Ignore this command since now is processing\r\n");
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

void wpa_driver_func_init_patch(void)
{
    wpa_driver_netlink_scan        = wpa_driver_netlink_scan_patch;
    wpa_driver_netlink_scan_by_cfg = wpa_driver_netlink_scan_by_cfg_patch;
}
