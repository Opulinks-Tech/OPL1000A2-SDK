/*
 * WPA Supplicant - Driver event processing
 * Copyright (c) 2003-2010, Jouni Malinen <j@w1.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 */
#include "includes.h"
#include "common.h"
#include "eapol_supp_sm.h"
#include "wpa.h"
#include "eloop.h"
#include "config.h"
#include "l2_packet.h"
#include "wpa_supplicant_i.h"
#include "driver_i.h"
#include "pcsc_funcs.h"
#include "preauth.h"
#include "pmksa_cache.h"
#include "wpa_ctrl.h"
#include "eap.h"
#include "notify.h"
#include "ieee802_11_defs.h"
#include "wpas_glue.h"
#include "wps_supplicant.h"
#include "sme.h"
#include "ap.h"
#include "bss.h"
#include "mlme.h"
#include "scan.h"
#include "controller_wifi_com.h"
#include "wpa_i.h"
#include "driver_netlink.h"
#include "events_netlink.h"
#include "Sha1-pbkdf2.h"
#include "at_cmd_common.h"

#include "util_func.h"

#include "controller_wifi_com_patch.h"

//#define WPAS_LOG                        printf
#define WPAS_LOG(...)

extern struct wpa wpa;
extern char g_passphrase[MAX_LEN_OF_PASSWD];
extern u8 g_wpa_psk[32];

void wpa_supplicant_event_assoc_patch(struct wpa_supplicant *wpa_s,
          union wpa_event_data *data)
{
    u8 bssid[ETH_ALEN]={0};
    asso_data *pdata;
    u8 *pReqIE;
    scan_info_t *pInfo = NULL;
    auto_conn_info_t *pacInfo = NULL;
    u8 rsn_ie_len;
    u8 wpa_ie_len;
    u8 asso_ie_len;
    struct wpa_sm *sm = NULL;

    if(wpa_s == NULL) return;

    sm = wpa.supp;
    
    wpa_driver_netlink_get_bssid(bssid);

    os_memcpy(wpa_s->bssid, bssid, ETH_ALEN);

    wpa_supplicant_set_state(wpa_s, WPA_ASSOCIATED);

    os_memcpy(sm->bssid, bssid, ETH_ALEN);
    os_memset(sm->rx_replay_counter, 0, WPA_REPLAY_COUNTER_LEN);
    sm->rx_replay_counter_set = 0;

    //Store ap data to bss_info
    pdata = wifi_get_asso_data();
    wpa_s->pairwise_cipher = pdata->pairwise_cipher;
    wpa_s->group_cipher = pdata->group_cipher;
    wpa_s->key_mgmt = pdata->key_mgmt;
    wpa_s->mgmt_group_cipher = pdata->mgmt_group_cipher;

    wpa_sm_set_param(wpa_s->wpa, WPA_PARAM_PROTO, pdata->proto);
    wpa_sm_set_param(wpa_s->wpa, WPA_PARAM_PAIRWISE, wpa_s->pairwise_cipher);
    wpa_sm_set_param(wpa_s->wpa, WPA_PARAM_GROUP, wpa_s->group_cipher);
    wpa_sm_set_param(wpa_s->wpa, WPA_PARAM_KEY_MGMT, wpa_s->key_mgmt);
    wpa_sm_set_param(wpa_s->wpa, WPA_PARAM_MGMT_GROUP, wpa_s->mgmt_group_cipher);

    //wpa_s->eapol_received = 0;

    //Check if it's open system connection, it means it's connected
    //Check if it's security connection, it means it's not the final state, the following eapol key frame event will come

    if (wpa_s->key_mgmt == WPA_KEY_MGMT_NONE ||
        wpa_s->key_mgmt == WPA_KEY_MGMT_WPA_NONE){
        wpa_supplicant_set_state(wpa_s, WPA_COMPLETED);
    }

    pReqIE = wifi_get_asso_ie();
    asso_ie_len = pReqIE[1] + 2; //pReqIE[1];

    //Set ASSOC IE
    wpa_sm_set_assoc_wpa_ie(wpa_s->wpa, pReqIE, asso_ie_len);
    
    pInfo = wifi_get_scan_record(wpa_s->bssid);

    //Set AP channel
    wpa_s->assoc_freq = pInfo->ap_channel;
    
    //Get RSN IE
    rsn_ie_len = pInfo->rsn_ie[1];

    //Get WPA IE
    wpa_ie_len = pInfo->wpa_ie[1];

    //Set RSN/WPA IE
    wpa_sm_set_ap_rsn_ie(wpa_s->wpa, pInfo->rsn_ie, rsn_ie_len);

    wpa_sm_set_ap_wpa_ie(wpa_s->wpa, pInfo->wpa_ie, wpa_ie_len);

    //wpa_supplicant_set_suites(wpa_s, NULL, ssid, wpa_ie, &wpa_ie_len)

    if (CTRL_WIFI_ACT_BIT_GET(ctrl_wifi_act, CTRL_WIFI_ACT_AUTO)) {
        pacInfo = wifi_get_ac_record(wpa_s->bssid);
        wpa_sm_set_pmk(wpa_s->wpa, pacInfo->psk, PMK_LEN);
    }
}

/*
   Interface Initialization: WPA Events
 */
void wpa_events_func_init_patch(void)
{
    wpa_supplicant_event_assoc = wpa_supplicant_event_assoc_patch;
}
