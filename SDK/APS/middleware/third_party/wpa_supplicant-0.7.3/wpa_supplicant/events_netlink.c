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
//#include "hostapd.h"
//#include "blacklist.h"
//#include "ibss_rsn.h"
//#include "bgscan.h"

#include "util_func.h"

//I0051 0000908_add_begin
#define WPAS_MIC_ERR_BLK_TIME           60 // seconds
#define WPAS_MIC_ERR_AP_NUM             4
#define WPAS_SEC_GET(pu32Sec)           util_get_current_time(pu32Sec, NULL)

//#define WPAS_LOG                        printf
#define WPAS_LOG(...)

#define WPAS_DIFF_GET(u32Start,u32End,pu32Diff)   \
if(u32End >= u32Start)  \
{   \
    *pu32Diff = u32End - u32Start;  \
}   \
else    \
{   \
    *pu32Diff = 0xFFFFFFFF - u32Start + 1 + u32End; \
}


typedef struct
{
    uint32_t u32Time; // seconds
    uint8_t u8aBssid[ETH_ALEN];
} T_WpasMicErr;


T_WpasMicErr g_taWpasMicErrCB[WPAS_MIC_ERR_AP_NUM] = {0};
uint8_t g_u8WpasMicErrIdx = 0;

//I0051 0000908_add_end

extern struct wpa wpa;
extern char g_passphrase[MAX_LEN_OF_PASSWD];
extern u8 g_wpa_psk[32];

void wpa_supplicant_mark_disassoc_impl(struct wpa_supplicant *wpa_s)
{
	os_memset(wpa_s->bssid, 0, ETH_ALEN);
	os_memset(wpa_s->pending_bssid, 0, ETH_ALEN);

	wpa_s->current_bss = NULL;
    wpa_s->ap_ies_from_associnfo = 0;
	wpa_s->pairwise_cipher = 0;
	wpa_s->group_cipher = 0;
	wpa_s->mgmt_group_cipher = 0;
	wpa_s->key_mgmt = 0;

    wpa_supplicant_set_state(wpa_s, WPA_DISCONNECTED);
}

int wpa_supplicant_dynamic_keys_impl(struct wpa_supplicant *wpa_s)
{
	if (wpa_s->key_mgmt == WPA_KEY_MGMT_NONE ||
	    wpa_s->key_mgmt == WPA_KEY_MGMT_WPA_NONE)
		return 0;
#if 0
#ifdef IEEE8021X_EAPOL
	if (wpa_s->key_mgmt == WPA_KEY_MGMT_IEEE8021X_NO_WPA &&
	    wpa_s->current_ssid &&
	    !(wpa_s->current_ssid->eapol_flags &
	      (EAPOL_FLAG_REQUIRE_KEY_UNICAST |
	       EAPOL_FLAG_REQUIRE_KEY_BROADCAST))) {
		/* IEEE 802.1X, but not using dynamic WEP keys (i.e., either
		 * plaintext or static WEP keys). */
		return 0;
	}
#endif /* IEEE8021X_EAPOL */
#endif

	return 1;
}


#ifndef CONFIG_NO_SCAN_PROCESSING
void wpa_supplicant_req_new_scan_impl(struct wpa_supplicant *wpa_s,
					int timeout_sec, int timeout_usec)
{
	wpa_supplicant_req_scan(wpa_s, timeout_sec, timeout_usec);
}

void wpa_supplicant_connect_impl(struct wpa_supplicant *wpa_s,
			    struct wpa_bss *selected,
			    struct wpa_ssid *ssid)
{
#if 0
	/*
	 * Do not trigger new association unless the BSSID has changed or if
	 * reassociation is requested. If we are in process of associating with
	 * the selected BSSID, do not trigger new attempt.
	 */
	if (wpa_s->reassociate ||
	    (os_memcmp(selected->bssid, wpa_s->bssid, ETH_ALEN) != 0 &&
	     (wpa_s->wpa_state != WPA_ASSOCIATING ))) {
		wpa_supplicant_associate(wpa_s, selected, ssid);
	} else {
		wpa_printf(MSG_DEBUG, "Already associated with the selected "
			   "AP");
	}
#endif
}

void wpa_supplicant_event_scan_results_impl(struct wpa_supplicant *wpa_s,
					      union wpa_event_data *data)
{
#if 0
	struct wpa_bss *selected;
	struct wpa_ssid *ssid = NULL;
	struct wpa_scan_results *scan_res;
	int ap = 0;

	scan_res = wpa_supplicant_get_scan_results(wpa_s,
						   data ? &data->scan_info :
						   NULL, 1);
	if (scan_res == NULL) {
		if (wpa_s->conf->ap_scan == 2 || ap)
			return;
		wpa_printf(MSG_DEBUG, "Failed to get scan results - try "
			   "scanning again");
		wpa_supplicant_req_new_scan(wpa_s, 1, 0);
		return;
	}

	wpa_printf(MSG_DEBUG, "New scan results available");
	//wpas_notify_scan_results(wpa_s);

	if (wpa_s->disconnected) {
		wpa_supplicant_set_state(wpa_s, WPA_DISCONNECTED);
		wpa_scan_results_free(scan_res);
		return;
	}
#endif
}
#endif //CONFIG_NO_SCAN_PROCESSING

int wpa_supplicant_event_associnfo_impl(struct wpa_supplicant *wpa_s,
					  union wpa_event_data *data)
{
	int l, len, found = 0, wpa_found, rsn_found;
	const u8 *p;

	wpa_printf(MSG_DEBUG, "[HDL_ASSOI]WPA: wpa_supplicant_event_associnfo \r\n");
	if (data->assoc_info.req_ies)
		wpa_hexdump(MSG_DEBUG, "[HDL_ASSOI]WPA: req_ies", data->assoc_info.req_ies, data->assoc_info.req_ies_len);
	if (data->assoc_info.resp_ies)
		wpa_hexdump(MSG_DEBUG, "[HDL_ASSOI]WPA: resp_ies", data->assoc_info.resp_ies, data->assoc_info.resp_ies_len);
	if (data->assoc_info.beacon_ies)
		wpa_hexdump(MSG_DEBUG, "[HDL_ASSOI]WPA: beacon_ies", data->assoc_info.beacon_ies, data->assoc_info.beacon_ies_len);
	if (data->assoc_info.freq)
		wpa_printf(MSG_DEBUG, "[HDL_ASSOI]WPA: freq=%u MHz \r\n", data->assoc_info.freq);

	p = data->assoc_info.req_ies;
	l = data->assoc_info.req_ies_len;

	/* Go through the IEs and make a copy of the WPA/RSN IE, if present. */
	while (p && l >= 2) {
		len = p[1] + 2;
		if (len > l) {
			wpa_hexdump(MSG_DEBUG, "[HDL_ASSOI]WPA: Truncated IE in assoc_info", p, l);
			break;
		}
		if ((p[0] == WLAN_EID_VENDOR_SPECIFIC && p[1] >= 6 &&
		     (os_memcmp(&p[2], "\x00\x50\xF2\x01\x01\x00", 6) == 0)) ||
		    (p[0] == WLAN_EID_RSN && p[1] >= 2)) {
			if (wpa_sm_set_assoc_wpa_ie(wpa_s->wpa, p, len))
				break;
			found = 1;
            /*
			wpa_find_assoc_pmkid(wpa_s);
                   */
			break;
		}
		l -= len;
		p += len;
	}
	if (!found && data->assoc_info.req_ies)
		wpa_sm_set_assoc_wpa_ie(wpa_s->wpa, NULL, 0);

	/* WPA/RSN IE from Beacon/ProbeResp */
	p = data->assoc_info.beacon_ies;
	l = data->assoc_info.beacon_ies_len;

	/* Go through the IEs and make a copy of the WPA/RSN IEs, if present.
	 */
	wpa_found = rsn_found = 0;
	while (p && l >= 2) {
		len = p[1] + 2;
		if (len > l) {
			wpa_hexdump(MSG_DEBUG, "[HDL_ASSOI]WPA: Truncated IE in beacon_ies", p, l);
			break;
		}
		if (!wpa_found &&
		    p[0] == WLAN_EID_VENDOR_SPECIFIC && p[1] >= 6 &&
		    os_memcmp(&p[2], "\x00\x50\xF2\x01\x01\x00", 6) == 0) {
			wpa_found = 1;
			wpa_sm_set_ap_wpa_ie(wpa_s->wpa, p, len);
		}

		if (!rsn_found &&
		    p[0] == WLAN_EID_RSN && p[1] >= 2) {
			rsn_found = 1;
			wpa_sm_set_ap_rsn_ie(wpa_s->wpa, p, len);
		}

		l -= len;
		p += len;
	}

	if (!wpa_found && data->assoc_info.beacon_ies)
		wpa_sm_set_ap_wpa_ie(wpa_s->wpa, NULL, 0);
	if (!rsn_found && data->assoc_info.beacon_ies)
		wpa_sm_set_ap_rsn_ie(wpa_s->wpa, NULL, 0);
	if (wpa_found || rsn_found)
		wpa_s->ap_ies_from_associnfo = 1;

	wpa_s->assoc_freq = data->assoc_info.freq;

	return 0;
}

void wpa_supplicant_event_assoc_impl(struct wpa_supplicant *wpa_s,
				       union wpa_event_data *data)
{
	u8 bssid[ETH_ALEN]={0};
    asso_data *pdata;
    u8 mac_state;
    u8 *ssid;
    int i;
//    int i, j;
    u8 *pReqIE;
    scan_info_t *pInfo = NULL;
#ifdef __WIFI_AUTO_CONNECT__    
    auto_conn_info_t *pacInfo = NULL;
#endif
    u8 rsn_ie_len;
    u8 wpa_ie_len;
    u8 asso_ie_len;
    struct wpa_sm *sm = NULL;

    if(wpa_s == NULL) return;

    sm = wpa.supp;

    //os_memset(passphrase, 0, 32);
    memset(&g_wpa_psk[0], 0, 32); //I0060 0000940
    
    wpa_driver_netlink_get_bssid(bssid);
    wpa_printf(MSG_DEBUG, "[HDL_ASSO]WPA: bssid=%02x:%02x:%02x:%02x:%02x:%02x \r\n", bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]);

    os_memcpy(wpa_s->bssid, bssid, ETH_ALEN);

    wpa_supplicant_set_state(wpa_s, WPA_ASSOCIATED);

    wpa_printf(MSG_DEBUG, "[HDL_ASSO]WPA: Association event - clear replay counter \r\n");
    os_memcpy(sm->bssid, bssid, ETH_ALEN);
    os_memset(sm->rx_replay_counter, 0, WPA_REPLAY_COUNTER_LEN);
    sm->rx_replay_counter_set = 0;

    //Store ap data to bss_info
    pdata = wifi_get_asso_data();
    wpa_s->pairwise_cipher = pdata->pairwise_cipher;
    wpa_s->group_cipher = pdata->group_cipher;
    wpa_s->key_mgmt = pdata->key_mgmt;
    wpa_s->mgmt_group_cipher = pdata->mgmt_group_cipher;

    wpa_printf(MSG_DEBUG, "[HDL_ASSO]WPA: key_mgmt:%d pairwise_cipher:%d group_cipher:%d mgmt_group_cipher:%d \r\n",
                           wpa_s->key_mgmt,
                           wpa_s->pairwise_cipher,
                           wpa_s->group_cipher,
                           wpa_s->mgmt_group_cipher);

    wpa_sm_set_param(wpa_s->wpa, WPA_PARAM_PROTO, pdata->proto);
    wpa_sm_set_param(wpa_s->wpa, WPA_PARAM_PAIRWISE, wpa_s->pairwise_cipher);
    wpa_sm_set_param(wpa_s->wpa, WPA_PARAM_GROUP, wpa_s->group_cipher);
    wpa_sm_set_param(wpa_s->wpa, WPA_PARAM_KEY_MGMT, wpa_s->key_mgmt);
    wpa_sm_set_param(wpa_s->wpa, WPA_PARAM_MGMT_GROUP, wpa_s->mgmt_group_cipher);


    //wpa_s->eapol_received = 0;

    //Check if it's open system connection, it means it's connected
    //Check if it's security connection, it means it's not the final state, the following eapol key frame event will come
    mac_state = wifi_get_mac_state();
    wpa_printf(MSG_DEBUG, "[HDL_ASSO]WPA: mac state:%d \r\n", mac_state);

#if 0
    if (mac_state == MAC_STA_4) {
        wpa_supplicant_set_state(wpa_s, WPA_COMPLETED);
        wpa_printf(MSG_DEBUG, "\r\nconnected\r\n");
    }
#else
    wpa_printf(MSG_DEBUG, "[HDL_ASSO]WPA: key_mgmt:%d \r\n", wpa_s->key_mgmt);
    if (wpa_s->key_mgmt == WPA_KEY_MGMT_NONE ||
        wpa_s->key_mgmt == WPA_KEY_MGMT_WPA_NONE){
        wpa_supplicant_set_state(wpa_s, WPA_COMPLETED);
        wpa_printf(MSG_DEBUG, "[HDL_ASSO]WPA: connected\r\n");
    }
#endif

#if 0
	if ((wpa_s->key_mgmt == WPA_KEY_MGMT_NONE ||
	     wpa_s->key_mgmt == WPA_KEY_MGMT_IEEE8021X_NO_WPA) &&
	    wpa_s->current_ssid && wpa_drv_get_capa(wpa_s, &capa) == 0 &&
	    capa.flags & WPA_DRIVER_FLAGS_SET_KEYS_AFTER_ASSOC_DONE) {
		/* Set static WEP keys again */
		wpa_set_wep_keys(wpa_s, wpa_s->current_ssid);
	}
#endif

    pReqIE = wifi_get_asso_ie();
    asso_ie_len = pReqIE[1] + 2; //pReqIE[1];

    wpa_printf(MSG_DEBUG, "[HDL_ASSO]WPA: Asso IE length: %d \r\n", asso_ie_len);
    wpa_printf(MSG_DEBUG, "[HDL_ASSO]WPA: Asso IE: ");
    for (i = 0; i < asso_ie_len; i++) wpa_printf(MSG_DEBUG, "%02x ", *(pReqIE+i));
    wpa_printf(MSG_DEBUG, "\r\n");

    //Set ASSOC IE
    wpa_sm_set_assoc_wpa_ie(wpa_s->wpa, pReqIE, asso_ie_len);


    pInfo = wifi_get_scan_record(wpa_s->bssid);
    wpa_printf(MSG_DEBUG, "[HDL_ASSO]WPA: pInfo->bssid:%02x:%02x:%02x:%02x:%02x:%02x \r\n", pInfo->bssid[0],pInfo->bssid[1],pInfo->bssid[2],pInfo->bssid[3],pInfo->bssid[4],pInfo->bssid[5]);
    
    //Set AP channel
    wpa_s->assoc_freq = pInfo->ap_channel;
    
    //Get RSN IE
    rsn_ie_len = pInfo->rsn_ie[1];
    wpa_printf(MSG_DEBUG, "[HDL_ASSO]WPA: RSN IE length:%d \r\n", rsn_ie_len);
    wpa_printf(MSG_DEBUG, "[HDL_ASSO]WPA: rsn_ie:");
    for(i = 0; i < rsn_ie_len; i++) wpa_printf(MSG_DEBUG, "%x ", pInfo->rsn_ie[i]);
    wpa_printf(MSG_DEBUG, "\r\n");

    //Get WPA IE
    wpa_ie_len = pInfo->wpa_ie[1];
    wpa_printf(MSG_DEBUG, "[HDL_ASSO]WPA: WPA IE length: %d \r\n", wpa_ie_len);
    wpa_printf(MSG_DEBUG, "[HDL_ASSO]WPA: wpa_ie: ");
    for(i = 0; i < wpa_ie_len; i++) wpa_printf(MSG_DEBUG, "%x ", pInfo->wpa_ie[i]);
    wpa_printf(MSG_DEBUG, "\r\n");

    //Set RSN/WPA IE
    //wpa_printf(MSG_DEBUG, "\r\n wpa_supplicant_event_assoc, call wpa_sm_set_ap_rsn_ie \r\n");
    wpa_sm_set_ap_rsn_ie(wpa_s->wpa, pInfo->rsn_ie, rsn_ie_len);

    //wpa_printf(MSG_DEBUG, "\r\n wpa_supplicant_event_assoc, call wpa_sm_set_ap_wpa_ie \r\n");
    wpa_sm_set_ap_wpa_ie(wpa_s->wpa, pInfo->wpa_ie, wpa_ie_len);

    wpa_printf(MSG_DEBUG, "[HDL_ASSO]WPA: call wpa_sm_set_pmk \r\n");
    //wpa_supplicant_set_suites(wpa_s, NULL, ssid, wpa_ie, &wpa_ie_len)

    if (mac_state == MAC_STA_3) {

        //SSID
        ssid = wifi_get_ssid();
        wpa_printf(MSG_DEBUG, "[HDL_ASSO]WPA: ssid: ");
        for(i=0; i<IEEE80211_MAX_SSID_LEN; i++){
            wpa_printf(MSG_DEBUG, "%c", *(ssid + i));
        }
        wpa_printf(MSG_DEBUG, "\r\n");

        //We should copy ssid to structure here, TBD
        //Passphrase, use a fixed value here temporarily
        //We should copy ssid to structure here, TBD
        //wpa_printf(MSG_DEBUG, "\r\n wpa_supplicant_event_assoc, passphrase: ");
        //for(i=0; i<os_strlen(ssid); i++){
        //    wpa_printf(MSG_DEBUG, "%c", *(passphrase + i));
        wpa_printf(MSG_DEBUG, "[HDL_ASSO]WPA: g_passphrase: ");
        for(i=0; i<os_strlen(g_passphrase); i++){
            wpa_printf(MSG_DEBUG, "%c", *(g_passphrase + i));
        }
        wpa_printf(MSG_DEBUG, "\r\n");

        wpa_printf(MSG_DEBUG, "\r\n wpa_supplicant_event_assoc, wpa.psk(before): ");
        for(i = 0; i < PMK_LEN; i++) wpa_printf(MSG_DEBUG, "%x ", wpa.psk[i]);

        wpa_printf(MSG_DEBUG, "[HDL_ASSO]WPA: caculate PMK, os_strlen(ssid):%d, call pbkdf2_sha1 \r\n", os_strlen((const char*)ssid));

        //1.Caculate the PMK
        //pbkdf2_sha1(passphrase, (char*)ssid, os_strlen((const char*)ssid), 4096, wpa.psk, 32);
        
        //I0060 0000940
        if (get_auto_connect_mode() == AUTO_CONNECT_ENABLE ||
            get_auto_connect_mode() == AUTO_CONNECT_DIRECT) {
            pacInfo = wifi_get_ac_record(wpa_s->bssid);
            wpa_sm_set_pmk(wpa_s->wpa, pacInfo->psk, PMK_LEN);
        }
        else {
            pbkdf2_sha1(g_passphrase, (char*)ssid, os_strlen((const char*)ssid), 4096, wpa.psk, 32);
            wpa_sm_set_pmk(wpa_s->wpa, wpa.psk, PMK_LEN);
            //I0060 0000654 memset(&g_wpa_psk[0], 0, 32);
            memcpy(&g_wpa_psk[0], wpa.psk, 32);
        }

        //os_memcpy(wpa.psk, temp_psk, 32);

        wpa_printf(MSG_DEBUG, "[HDL_ASSO]WPA: after pbkdf2_sha1, wpa.psk:");
        for(i = 0; i < PMK_LEN; i++) wpa_printf(MSG_DEBUG, "%x ", wpa.psk[i]);
        wpa_printf(MSG_DEBUG, "\r\n");

        //2.Set the PMK
        //wpa_sm_set_pmk(wpa_s->wpa, wpa.psk, PMK_LEN);
    }
    else { //Open connection
        /* Set successfully connect info to Auto Connect list */
        //I0060 0000940_add_beg
        if (get_auto_connect_mode() == AUTO_CONNECT_MANUAL) {
            add_auto_connect_list();
            set_auto_connect_mode(AUTO_CONNECT_ENABLE);
        }
        //I0060 0000940_add_end
#if 0 //I0060 0000940_del_beg
        switch(get_auto_connect_mode()) {
            case AUTO_CONNECT_MANUAL:
                add_auto_connect_list();
                set_auto_connect_mode(AUTO_CONNECT_ENABLE);
                break;
            default:
                break;
        }
#endif //I0060 0000940_del_end
    }
}

void wpa_supplicant_event_disassoc_impl(struct wpa_supplicant *wpa_s,
                                        u16 reason_code)
{
    const u8 *bssid;

    bssid = wpa_s->bssid;
    wpa_msg(wpa_s, MSG_INFO, WPA_EVENT_DISCONNECTED "bssid=" MACSTR " reason=%d\r\n", MAC2STR(bssid), reason_code);
    wpa_printf(MSG_DEBUG, "\r\n");
    wpa_supplicant_mark_disassoc(wpa_s);
    wpa_printf(MSG_DEBUG, "[HDL_ASSO]WPA: disconnected\r\n");
}

void wpa_supplicant_event_eapol_rx_impl(struct wpa_supplicant *wpa_s)
{
	const u8 *bssid = NULL;
    rx_eapol_data *pEapolKeyData = NULL;
//    u8 src_addr[ETH_ALEN]={0};

    if (wpa_s == NULL) return;

    wpa_printf(MSG_DEBUG, "[HDL_EKFRX]WPA: state:%d \r\n", wpa_s->wpa_state);
    wpa_printf(MSG_DEBUG, "[HDL_EKFRX]WPA: bssid=%02x:%02x:%02x:%02x:%02x:%02x \r\n",
                          wpa_s->bssid[0],
                          wpa_s->bssid[1],
                          wpa_s->bssid[2],
                          wpa_s->bssid[3],
                          wpa_s->bssid[4],
                          wpa_s->bssid[5]);

#if 0
    if ((wpa_s->wpa_state != WPA_ASSOCIATED) &&
        (wpa_s->wpa_state != WPA_4WAY_HANDSHAKE) &&
        (wpa_s->wpa_state != WPA_GROUP_HANDSHAKE)
       )
    {
        wpa_printf(MSG_DEBUG, "\r\n wpa_supplicant_event_eapol_rx, wpa_state != WPA_ASSOCIATE, Ignored received EAPOL frame \r\n");
		return;
	}
#endif

    //wpa_supplicant_set_state(wpa_s, WPA_4WAY_HANDSHAKE);

    //wpa_driver_netlink_get_bssid(src_addr);
    //os_memcpy(wpa_s->bssid, src_addr, ETH_ALEN);

    bssid = wpa_s->bssid;
    pEapolKeyData = wifi_get_eapol_data();
    if(pEapolKeyData == NULL){
        wpa_printf(MSG_DEBUG, "[HDL_EKFRX]WPA: pEapolKeyData == NULL \r\n");
        return;
    }
    wpa_printf(MSG_DEBUG, "[HDL_EKFRX]WPA: call wpa_supplicant_rx_eapol_netlink, pEapolKeyData->frame_length:%d \r\n", pEapolKeyData->frame_length);

    wpa_supplicant_rx_eapol_netlink(bssid, pEapolKeyData->frame_buffer, pEapolKeyData->frame_length);
}

//I0051 0000908_add_begin
void wpa_supplicant_mic_error_update_impl(uint8_t *u8aBssid, uint32_t u32Sec)
{
    uint8_t i = 0;
    int iIdx = -1;
    int iUnusedIdx = -1;

    for(i = 0; i < WPAS_MIC_ERR_AP_NUM; i++)
    {
        if(g_taWpasMicErrCB[i].u32Time) // used
        {
            if(!memcmp(u8aBssid, g_taWpasMicErrCB[i].u8aBssid, ETH_ALEN))
            {
                iIdx = i;
            }
        }
        else // unused
        {
            if(iUnusedIdx < 0)
            {
                iUnusedIdx = i;
            }
        }
    }

    if(iIdx < 0)
    {
        if(iUnusedIdx >= 0)
        {
            iIdx = iUnusedIdx;
        }
        else
        {
            iIdx = g_u8WpasMicErrIdx;

            if(g_u8WpasMicErrIdx + 1 < WPAS_MIC_ERR_AP_NUM)
            {
                ++g_u8WpasMicErrIdx;
            }
            else
            {
                g_u8WpasMicErrIdx = 0;
            }
        }
    }

    g_taWpasMicErrCB[iIdx].u32Time = u32Sec;
    memcpy(g_taWpasMicErrCB[iIdx].u8aBssid, u8aBssid, ETH_ALEN);

    WPAS_LOG("L[%d] i[%d] b[%02x:%02x:%02x:%02x:%02x:%02x] t[%u]\n", __LINE__
             , iIdx
             , g_taWpasMicErrCB[iIdx].u8aBssid[0], g_taWpasMicErrCB[iIdx].u8aBssid[1], g_taWpasMicErrCB[iIdx].u8aBssid[2]
             , g_taWpasMicErrCB[iIdx].u8aBssid[3], g_taWpasMicErrCB[iIdx].u8aBssid[4], g_taWpasMicErrCB[iIdx].u8aBssid[5]
             , g_taWpasMicErrCB[iIdx].u32Time);
    return;
}

uint32_t wpa_supplicant_last_mic_err_time_get_impl(uint8_t *u8aBssid)
{
    uint32_t u32Time = 0;
    uint8_t i = 0;

    for(i = 0; i < WPAS_MIC_ERR_AP_NUM; i++)
    {
        if(g_taWpasMicErrCB[i].u32Time) // used
        {
            if(!memcmp(u8aBssid, g_taWpasMicErrCB[i].u8aBssid, ETH_ALEN))
            {
                u32Time = g_taWpasMicErrCB[i].u32Time;
                break;
            }
        }
    }

    return u32Time;
}

int wpa_supplicant_is_bssid_blocked_by_mic_error_impl(uint8_t *u8aBssid, uint32_t *pu32Remain)
{
    int iRet = 0;
    uint32_t u32Now = 0;
    uint32_t u32DiffTime = 0;
    uint8_t i = 0;

    WPAS_SEC_GET(&u32Now);

    for(i = 0; i < WPAS_MIC_ERR_AP_NUM; i++)
    {
        if(g_taWpasMicErrCB[i].u32Time) // used
        {
            WPAS_DIFF_GET(g_taWpasMicErrCB[i].u32Time, u32Now, &u32DiffTime);

            if(u32DiffTime > WPAS_MIC_ERR_BLK_TIME)
            {
                WPAS_LOG("L[%d] diff[%u] > [%u]\n", __LINE__, u32DiffTime, WPAS_MIC_ERR_BLK_TIME);
                g_taWpasMicErrCB[i].u32Time = 0;
            }
            else
            {
                if(!memcmp(u8aBssid, g_taWpasMicErrCB[i].u8aBssid, ETH_ALEN))
                {
                    WPAS_LOG("L[%d] Block, i[%u] t[%u]:[%u] b[%02x:%02x:%02x:%02x:%02x:%02x] == mic_e[%02x:%02x:%02x:%02x:%02x:%02x]\n", __LINE__
                             , i
                             , u32Now, g_taWpasMicErrCB[i].u32Time
                             , u8aBssid[0], u8aBssid[1], u8aBssid[2]
                             , u8aBssid[3], u8aBssid[4], u8aBssid[5]
                             , g_taWpasMicErrCB[i].u8aBssid[0], g_taWpasMicErrCB[i].u8aBssid[1], g_taWpasMicErrCB[i].u8aBssid[2]
                             , g_taWpasMicErrCB[i].u8aBssid[3], g_taWpasMicErrCB[i].u8aBssid[4], g_taWpasMicErrCB[i].u8aBssid[5]);

                    iRet = -1;

                    if(pu32Remain)
                    {
                        *pu32Remain = WPAS_MIC_ERR_BLK_TIME - u32DiffTime + 1;
                    }
                }
            }
        }
    }

    return iRet;
}

void
wpa_supplicant_event_michael_mic_failure_impl(struct wpa_supplicant *wpa_s,
					 union wpa_event_data *data)
{
	int pairwise = 0;
	uint32_t u32Now = 0;
    uint32_t u32LastMicErrTime = 0;
    uint32_t u32DiffTime = 0;

	wpa_msg(wpa_s, MSG_WARNING, "Michael MIC failure detected\n");
	pairwise = (data && data->michael_mic_failure.unicast);

    WPAS_SEC_GET(&u32Now);
    u32LastMicErrTime = wpa_supplicant_last_mic_err_time_get(wpa_s->bssid);
    WPAS_LOG("L[%d] t[%u] err_time[%u]\n", __LINE__, u32Now, u32LastMicErrTime);

    if(u32LastMicErrTime)
    {
        WPAS_DIFF_GET(u32LastMicErrTime, u32Now, &u32DiffTime);
    }

    WPAS_LOG("L[%d] now[%u] last[%u] diff[%u]\n", __LINE__, u32Now, u32LastMicErrTime, u32DiffTime);
    
	if ((u32LastMicErrTime) && (u32DiffTime <= WPAS_MIC_ERR_BLK_TIME)) {
		/* Send the new MIC error report immediately since we are going
		 * to start countermeasures and AP better do the same.
		 */
        WPAS_LOG("\nSecond MIC Failure within 60 seconds\n");
		wpa_sm_key_request(wpa_s->wpa, 1, pairwise);

		/* initialize countermeasures */
		wpa_s->countermeasures = 1;
		wpa_msg(wpa_s, MSG_WARNING, "TKIP countermeasures started\n");

		/*
		 * Need to wait for completion of request frame. We do not get
		 * any callback for the message completion, so just wait a
		 * short while and hope for the best. */
        //os_sleep(0, 10);

		wpa_drv_set_countermeasures(wpa_s, 1);

        WPAS_LOG("L[%d] Disconnect by MIC error\n", __LINE__);

        wpa_driver_netlink_disconnect(wpa_s->bssid, WLAN_REASON_MICHAEL_MIC_FAILURE);
        /*
		wpa_supplicant_deauthenticate(wpa_s,
					      WLAN_REASON_MICHAEL_MIC_FAILURE);
        */
        
        /*
        eloop_cancel_timeout(wpa_supplicant_stop_countermeasures, wpa_s, NULL);
		eloop_register_timeout(60, 0,
				       wpa_supplicant_stop_countermeasures,
				       wpa_s, NULL);*/

		/* TODO: mark the AP rejected for 60 second. STA is
		 * allowed to associate with another AP.. */
	} else {
        WPAS_LOG("\nFirst MIC Failure\n");
		wpa_sm_key_request(wpa_s->wpa, 1, pairwise);
	}
    wpa_s->last_michael_mic_error = u32Now;
	wpa_s->mic_errors_seen++;

    wpa_supplicant_mic_error_update(wpa_s->bssid, u32Now);
    return;
}
//I0051 0000908_add_end


RET_DATA wpa_supplicant_mark_disassoc_fp_t wpa_supplicant_mark_disassoc;
RET_DATA wpa_supplicant_dynamic_keys_fp_t wpa_supplicant_dynamic_keys;
RET_DATA wpa_supplicant_event_associnfo_fp_t wpa_supplicant_event_associnfo;
RET_DATA wpa_supplicant_event_assoc_fp_t wpa_supplicant_event_assoc;
RET_DATA wpa_supplicant_event_disassoc_fp_t wpa_supplicant_event_disassoc;
RET_DATA wpa_supplicant_event_eapol_rx_fp_t wpa_supplicant_event_eapol_rx;
RET_DATA wpa_supplicant_event_fp_t wpa_supplicant_event;

//I0051 0000908_add_begin
RET_DATA wpa_supplicant_mic_error_update_fp_t wpa_supplicant_mic_error_update;
RET_DATA wpa_supplicant_last_mic_err_time_get_fp_t wpa_supplicant_last_mic_err_time_get;
RET_DATA wpa_supplicant_is_bssid_blocked_by_mic_error_fp_t wpa_supplicant_is_bssid_blocked_by_mic_error;
RET_DATA wpa_supplicant_event_michael_mic_failure_fp_t wpa_supplicant_event_michael_mic_failure;
//I0051 0000908_add_end

#ifndef CONFIG_NO_SCAN_PROCESSING
RET_DATA wpa_supplicant_req_new_scan_fp_t wpa_supplicant_req_new_scan;
RET_DATA wpa_supplicant_connect_fp_t wpa_supplicant_connect;
RET_DATA wpa_supplicant_event_scan_results_fp_t wpa_supplicant_event_scan_results;
#endif


/*
   Interface Initialization: WPA Events
 */
void wpa_events_func_init(void)
{
    wpa_supplicant_mark_disassoc = wpa_supplicant_mark_disassoc_impl;
    wpa_supplicant_dynamic_keys = wpa_supplicant_dynamic_keys_impl;
    wpa_supplicant_event_associnfo = wpa_supplicant_event_associnfo_impl;
    wpa_supplicant_event_assoc = wpa_supplicant_event_assoc_impl;
    wpa_supplicant_event_disassoc = wpa_supplicant_event_disassoc_impl;
    wpa_supplicant_event_eapol_rx = wpa_supplicant_event_eapol_rx_impl;
    //wpa_supplicant_event = wpa_supplicant_event_impl;

    //I0051 0000908_add_begin
    wpa_supplicant_mic_error_update = wpa_supplicant_mic_error_update_impl;
    wpa_supplicant_last_mic_err_time_get = wpa_supplicant_last_mic_err_time_get_impl;
    wpa_supplicant_is_bssid_blocked_by_mic_error = wpa_supplicant_is_bssid_blocked_by_mic_error_impl;
    wpa_supplicant_event_michael_mic_failure = wpa_supplicant_event_michael_mic_failure_impl;
    //I0051 0000908_add_end

#ifndef CONFIG_NO_SCAN_PROCESSING
    wpa_supplicant_req_new_scan = wpa_supplicant_req_new_scan_impl;
    wpa_supplicant_connect = wpa_supplicant_connect_impl;
    wpa_supplicant_event_scan_results = wpa_supplicant_event_scan_results_impl;
#endif
}

#if 0
void wpa_supplicant_event_impl(void *ctx, enum wpa_event_type event,
			  union wpa_event_data *data)
{
	struct wpa_supplicant *wpa_s = ctx;
	u16 reason_code = 0;

	switch (event) {
	case EVENT_AUTH:
		sme_event_auth(wpa_s, data);
		break;
	case EVENT_ASSOC:
		wpa_supplicant_event_assoc(wpa_s, data);
		break;
	case EVENT_DISASSOC:
		wpa_printf(MSG_DEBUG, "Disassociation notification");

		if (data)
			reason_code = data->deauth_info.reason_code;
        /*
		if (wpa_s->drv_flags & WPA_DRIVER_FLAGS_SME)
			sme_event_disassoc(wpa_s, data);*/
		/* fall through */
	case EVENT_DEAUTH:
		if (event == EVENT_DEAUTH) {
			wpa_printf(MSG_DEBUG, "Deauthentication notification");
			if (data)
				reason_code = data->deauth_info.reason_code;
		}

		wpa_supplicant_event_disassoc(wpa_s, reason_code);
		break;
    /*
	case EVENT_MICHAEL_MIC_FAILURE:
		wpa_supplicant_event_michael_mic_failure(wpa_s, data);
		break;
      */
#ifndef CONFIG_NO_SCAN_PROCESSING
	case EVENT_SCAN_RESULTS:
		//wpa_supplicant_event_scan_results(wpa_s, data);
		break;
#endif /* CONFIG_NO_SCAN_PROCESSING */
	case EVENT_ASSOCINFO:
		wpa_supplicant_event_associnfo(wpa_s, data);
		break;
/*
	case EVENT_INTERFACE_STATUS:
		wpa_supplicant_event_interface_status(wpa_s, data);
		break;
	case EVENT_PMKID_CANDIDATE:
		wpa_supplicant_event_pmkid_candidate(wpa_s, data);
		break;
*/

#ifdef CONFIG_PEERKEY
	case EVENT_STKSTART:
		wpa_supplicant_event_stkstart(wpa_s, data);
		break;
#endif /* CONFIG_PEERKEY */
	case EVENT_ASSOC_REJECT:
		sme_event_assoc_reject(wpa_s, data);
		break;
    /*
	case EVENT_AUTH_TIMED_OUT:
		sme_event_auth_timed_out(wpa_s, data);
		break;
	case EVENT_ASSOC_TIMED_OUT:
		sme_event_assoc_timed_out(wpa_s, data);
		break;
      */

	case EVENT_RX_ACTION:
		wpa_printf(MSG_DEBUG, "Received Action frame: SA=" MACSTR
			   " Category=%u DataLen=%d freq=%d MHz",
			   MAC2STR(data->rx_action.sa),
			   data->rx_action.category, (int) data->rx_action.len,
			   data->rx_action.freq);
		break;

	case EVENT_EAPOL_RX:
		wpa_supplicant_rx_eapol(wpa_s, data->eapol_rx.src,
					data->eapol_rx.data,
					data->eapol_rx.data_len);
		break;
    /*
	case EVENT_SIGNAL_CHANGE:
		bgscan_notify_signal_change(
			wpa_s, data->signal_change.above_threshold);
		break;
      */
	default:
		wpa_printf(MSG_INFO, "Unknown event %d", event);
		break;
	}
}

/**
 * wpa_supplicant_scard_init - Initialize SIM/USIM access with PC/SC
 * @wpa_s: pointer to wpa_supplicant data
 * @ssid: Configuration data for the network
 * Returns: 0 on success, -1 on failure
 *
 * This function is called when starting authentication with a network that is
 * configured to use PC/SC for SIM/USIM access (EAP-SIM or EAP-AKA).
 */
int wpa_supplicant_scard_init(struct wpa_supplicant *wpa_s,
			      struct wpa_ssid *ssid)
{

#ifdef IEEE8021X_EAPOL
	int aka = 0, sim = 0, type;

	if (ssid->eap.pcsc == NULL || wpa_s->scard != NULL)
		return 0;

	if (ssid->eap.eap_methods == NULL) {
		sim = 1;
		aka = 1;
	} else {
		struct eap_method_type *eap = ssid->eap.eap_methods;
		while (eap->vendor != EAP_VENDOR_IETF ||
		       eap->method != EAP_TYPE_NONE) {
			if (eap->vendor == EAP_VENDOR_IETF) {
				if (eap->method == EAP_TYPE_SIM)
					sim = 1;
				else if (eap->method == EAP_TYPE_AKA)
					aka = 1;
			}
			eap++;
		}
	}

	if (eap_peer_get_eap_method(EAP_VENDOR_IETF, EAP_TYPE_SIM) == NULL)
		sim = 0;
	if (eap_peer_get_eap_method(EAP_VENDOR_IETF, EAP_TYPE_AKA) == NULL)
		aka = 0;

	if (!sim && !aka) {
		wpa_printf(MSG_DEBUG, "Selected network is configured to use "
			   "SIM, but neither EAP-SIM nor EAP-AKA are enabled");
		return 0;
	}

	wpa_printf(MSG_DEBUG, "Selected network is configured to use SIM "
		   "(sim=%d aka=%d) - initialize PCSC", sim, aka);
	if (sim && aka)
		type = SCARD_TRY_BOTH;
	else if (aka)
		type = SCARD_USIM_ONLY;
	else
		type = SCARD_GSM_SIM_ONLY;

	wpa_s->scard = scard_init(type);
	if (wpa_s->scard == NULL) {
		wpa_printf(MSG_WARNING, "Failed to initialize SIM "
			   "(pcsc-lite)");
		return -1;
	}
	//wpa_sm_set_scard_ctx(wpa_s->wpa, wpa_s->scard);
	//eapol_sm_register_scard_ctx(wpa_s->eapol, wpa_s->scard);
#endif /* IEEE8021X_EAPOL */

	return 0;
}
#endif /* #if 0 */

#ifndef CONFIG_NO_SCAN_PROCESSING
#if 0
int wpa_supplicant_match_privacy(struct wpa_scan_res *bss,
					struct wpa_ssid *ssid)
{
	int i, privacy = 0;

	if (ssid->mixed_cell)
		return 1;

#ifdef CONFIG_WPS
	if (ssid->key_mgmt & WPA_KEY_MGMT_WPS)
		return 1;
#endif /* CONFIG_WPS */

	for (i = 0; i < NUM_WEP_KEYS; i++) {
		if (ssid->wep_key_len[i]) {
			privacy = 1;
			break;
		}
	}
#ifdef IEEE8021X_EAPOL
	if ((ssid->key_mgmt & WPA_KEY_MGMT_IEEE8021X_NO_WPA) &&
	    ssid->eapol_flags & (EAPOL_FLAG_REQUIRE_KEY_UNICAST |
				 EAPOL_FLAG_REQUIRE_KEY_BROADCAST))
		privacy = 1;
#endif /* IEEE8021X_EAPOL */

	if (bss->caps & IEEE80211_CAP_PRIVACY)
		return privacy;
	return !privacy;
}

int wpa_supplicant_ssid_bss_match(struct wpa_supplicant *wpa_s,
					 struct wpa_ssid *ssid,
					 struct wpa_scan_res *bss)
{
	struct wpa_ie_data ie;
	int proto_match = 0;
	const u8 *rsn_ie, *wpa_ie;
	int ret;

	ret = wpas_wps_ssid_bss_match(wpa_s, ssid, bss);
	if (ret >= 0)
		return ret;

	rsn_ie = wpa_scan_get_ie(bss, WLAN_EID_RSN);
	while ((ssid->proto & WPA_PROTO_RSN) && rsn_ie) {
		proto_match++;

		if (wpa_parse_wpa_ie(rsn_ie, 2 + rsn_ie[1], &ie)) {
			wpa_printf(MSG_DEBUG, "   skip RSN IE - parse failed");
			break;
		}
		if (!(ie.proto & ssid->proto)) {
			wpa_printf(MSG_DEBUG, "   skip RSN IE - proto "
				   "mismatch");
			break;
		}

		if (!(ie.pairwise_cipher & ssid->pairwise_cipher)) {
			wpa_printf(MSG_DEBUG, "   skip RSN IE - PTK cipher "
				   "mismatch");
			break;
		}

		if (!(ie.group_cipher & ssid->group_cipher)) {
			wpa_printf(MSG_DEBUG, "   skip RSN IE - GTK cipher "
				   "mismatch");
			break;
		}

		if (!(ie.key_mgmt & ssid->key_mgmt)) {
			wpa_printf(MSG_DEBUG, "   skip RSN IE - key mgmt "
				   "mismatch");
			break;
		}

		wpa_printf(MSG_DEBUG, "   selected based on RSN IE");
		return 1;
	}

	wpa_ie = wpa_scan_get_vendor_ie(bss, WPA_IE_VENDOR_TYPE);
	while ((ssid->proto & WPA_PROTO_WPA) && wpa_ie) {
		proto_match++;

		if (wpa_parse_wpa_ie(wpa_ie, 2 + wpa_ie[1], &ie)) {
			wpa_printf(MSG_DEBUG, "   skip WPA IE - parse failed");
			break;
		}
		if (!(ie.proto & ssid->proto)) {
			wpa_printf(MSG_DEBUG, "   skip WPA IE - proto "
				   "mismatch");
			break;
		}

		if (!(ie.pairwise_cipher & ssid->pairwise_cipher)) {
			wpa_printf(MSG_DEBUG, "   skip WPA IE - PTK cipher "
				   "mismatch");
			break;
		}

		if (!(ie.group_cipher & ssid->group_cipher)) {
			wpa_printf(MSG_DEBUG, "   skip WPA IE - GTK cipher "
				   "mismatch");
			break;
		}

		if (!(ie.key_mgmt & ssid->key_mgmt)) {
			wpa_printf(MSG_DEBUG, "   skip WPA IE - key mgmt "
				   "mismatch");
			break;
		}

		wpa_printf(MSG_DEBUG, "   selected based on WPA IE");
		return 1;
	}

	if (proto_match == 0)
		wpa_printf(MSG_DEBUG, "   skip - no WPA/RSN proto match");
	return 0;
}

int freq_allowed(int *freqs, int freq)
{
	int i;

	if (freqs == NULL)
		return 1;

	for (i = 0; freqs[i]; i++)
		if (freqs[i] == freq)
			return 1;
	return 0;
}

struct wpa_bss *
wpa_supplicant_select_bss_wpa(struct wpa_supplicant *wpa_s,
			      struct wpa_scan_results *scan_res,
			      struct wpa_ssid *group,
			      struct wpa_ssid **selected_ssid)
{
	struct wpa_ssid *ssid;
	struct wpa_scan_res *bss;
	size_t i;
	struct wpa_blacklist *e;
	const u8 *ie;

	wpa_printf(MSG_DEBUG, "Try to find WPA-enabled AP");
	for (i = 0; i < scan_res->num; i++) {
		const u8 *ssid_;
		u8 wpa_ie_len, rsn_ie_len, ssid_len;
		bss = scan_res->res[i];

		ie = wpa_scan_get_ie(bss, WLAN_EID_SSID);
		ssid_ = ie ? ie + 2 : (u8 *) "";
		ssid_len = ie ? ie[1] : 0;

		ie = wpa_scan_get_vendor_ie(bss, WPA_IE_VENDOR_TYPE);
		wpa_ie_len = ie ? ie[1] : 0;

		ie = wpa_scan_get_ie(bss, WLAN_EID_RSN);
		rsn_ie_len = ie ? ie[1] : 0;

		wpa_printf(MSG_DEBUG, "%d: " MACSTR " ssid='%s' "
			   "wpa_ie_len=%u rsn_ie_len=%u caps=0x%x",
			   (int) i, MAC2STR(bss->bssid),
			   wpa_ssid_txt(ssid_, ssid_len),
			   wpa_ie_len, rsn_ie_len, bss->caps);

		e = wpa_blacklist_get(wpa_s, bss->bssid);
		if (e && e->count > 1) {
			wpa_printf(MSG_DEBUG, "   skip - blacklisted");
			continue;
		}

		if (ssid_len == 0) {
			wpa_printf(MSG_DEBUG, "   skip - SSID not known");
			continue;
		}

		if (wpa_ie_len == 0 && rsn_ie_len == 0) {
			wpa_printf(MSG_DEBUG, "   skip - no WPA/RSN IE");
			continue;
		}

		for (ssid = group; ssid; ssid = ssid->pnext) {
			int check_ssid = 1;

			if (ssid->disabled) {
				wpa_printf(MSG_DEBUG, "   skip - disabled");
				continue;
			}

#ifdef CONFIG_WPS
			if (ssid->ssid_len == 0 &&
			    wpas_wps_ssid_wildcard_ok(wpa_s, ssid, bss))
				check_ssid = 0;
#endif /* CONFIG_WPS */

			if (check_ssid &&
			    (ssid_len != ssid->ssid_len ||
			     os_memcmp(ssid_, ssid->ssid, ssid_len) != 0)) {
				wpa_printf(MSG_DEBUG, "   skip - "
					   "SSID mismatch");
				continue;
			}

			if (ssid->bssid_set &&
			    os_memcmp(bss->bssid, ssid->bssid, ETH_ALEN) != 0)
			{
				wpa_printf(MSG_DEBUG, "   skip - "
					   "BSSID mismatch");
				continue;
			}

			if (!wpa_supplicant_ssid_bss_match(wpa_s, ssid, bss))
				continue;

			if (!freq_allowed(ssid->freq_list, bss->freq)) {
				wpa_printf(MSG_DEBUG, "   skip - "
					   "frequency not allowed");
				continue;
			}

			wpa_printf(MSG_DEBUG, "   selected WPA AP "
				   MACSTR " ssid='%s'",
				   MAC2STR(bss->bssid),
				   wpa_ssid_txt(ssid_, ssid_len));
			*selected_ssid = ssid;
			return wpa_bss_get(wpa_s, bss->bssid, ssid_, ssid_len);
		}
	}
	return NULL;
}

struct wpa_bss *
wpa_supplicant_select_bss_non_wpa(struct wpa_supplicant *wpa_s,
				  struct wpa_scan_results *scan_res,
				  struct wpa_ssid *group,
				  struct wpa_ssid **selected_ssid)
{
	struct wpa_ssid *ssid;
	struct wpa_scan_res *bss;
	size_t i;
	struct wpa_blacklist *e;
	const u8 *ie;

	wpa_printf(MSG_DEBUG, "Try to find non-WPA AP");
	for (i = 0; i < scan_res->num; i++) {
		const u8 *ssid_;
		u8 wpa_ie_len, rsn_ie_len, ssid_len;
		bss = scan_res->res[i];

		ie = wpa_scan_get_ie(bss, WLAN_EID_SSID);
		ssid_ = ie ? ie + 2 : (u8 *) "";
		ssid_len = ie ? ie[1] : 0;

		ie = wpa_scan_get_vendor_ie(bss, WPA_IE_VENDOR_TYPE);
		wpa_ie_len = ie ? ie[1] : 0;

		ie = wpa_scan_get_ie(bss, WLAN_EID_RSN);
		rsn_ie_len = ie ? ie[1] : 0;

		wpa_printf(MSG_DEBUG, "%d: " MACSTR " ssid='%s' "
			   "wpa_ie_len=%u rsn_ie_len=%u caps=0x%x",
			   (int) i, MAC2STR(bss->bssid),
			   wpa_ssid_txt(ssid_, ssid_len),
			   wpa_ie_len, rsn_ie_len, bss->caps);

		e = wpa_blacklist_get(wpa_s, bss->bssid);
		if (e && e->count > 1) {
			wpa_printf(MSG_DEBUG, "   skip - blacklisted");
			continue;
		}

		if (ssid_len == 0) {
			wpa_printf(MSG_DEBUG, "   skip - SSID not known");
			continue;
		}

		for (ssid = group; ssid; ssid = ssid->pnext) {
			int check_ssid = ssid->ssid_len != 0;

			if (ssid->disabled) {
				wpa_printf(MSG_DEBUG, "   skip - disabled");
				continue;
			}

#ifdef CONFIG_WPS
			if (ssid->key_mgmt & WPA_KEY_MGMT_WPS) {
				/* Only allow wildcard SSID match if an AP
				 * advertises active WPS operation that matches
				 * with our mode. */
				check_ssid = 1;
				if (ssid->ssid_len == 0 &&
				    wpas_wps_ssid_wildcard_ok(wpa_s, ssid,
							      bss))
					check_ssid = 0;
			}
#endif /* CONFIG_WPS */

			if (check_ssid &&
			    (ssid_len != ssid->ssid_len ||
			     os_memcmp(ssid_, ssid->ssid, ssid_len) != 0)) {
				wpa_printf(MSG_DEBUG, "   skip - "
					   "SSID mismatch");
				continue;
			}

			if (ssid->bssid_set &&
			    os_memcmp(bss->bssid, ssid->bssid, ETH_ALEN) != 0)
			{
				wpa_printf(MSG_DEBUG, "   skip - "
					   "BSSID mismatch");
				continue;
			}

			if (!(ssid->key_mgmt & WPA_KEY_MGMT_NONE) &&
			    !(ssid->key_mgmt & WPA_KEY_MGMT_WPS) &&
			    !(ssid->key_mgmt & WPA_KEY_MGMT_IEEE8021X_NO_WPA))
			{
				wpa_printf(MSG_DEBUG, "   skip - "
					   "non-WPA network not allowed");
				continue;
			}

			if ((ssid->key_mgmt &
			     (WPA_KEY_MGMT_IEEE8021X | WPA_KEY_MGMT_PSK |
			      WPA_KEY_MGMT_FT_IEEE8021X | WPA_KEY_MGMT_FT_PSK |
			      WPA_KEY_MGMT_IEEE8021X_SHA256 |
			      WPA_KEY_MGMT_PSK_SHA256)) &&
			    (wpa_ie_len != 0 || rsn_ie_len != 0)) {
				wpa_printf(MSG_DEBUG, "   skip - "
					   "WPA network");
				continue;
			}

			if (!wpa_supplicant_match_privacy(bss, ssid)) {
				wpa_printf(MSG_DEBUG, "   skip - "
					   "privacy mismatch");
				continue;
			}

			if (bss->caps & IEEE80211_CAP_IBSS) {
				wpa_printf(MSG_DEBUG, "   skip - "
					   "IBSS (adhoc) network");
				continue;
			}

			if (!freq_allowed(ssid->freq_list, bss->freq)) {
				wpa_printf(MSG_DEBUG, "   skip - "
					   "frequency not allowed");
				continue;
			}

			wpa_printf(MSG_DEBUG, "   selected non-WPA AP "
				   MACSTR " ssid='%s'",
				   MAC2STR(bss->bssid),
				   wpa_ssid_txt(ssid_, ssid_len));
			*selected_ssid = ssid;
			return wpa_bss_get(wpa_s, bss->bssid, ssid_, ssid_len);
		}
	}
}

struct wpa_bss *
wpa_supplicant_select_bss(struct wpa_supplicant *wpa_s,
			  struct wpa_scan_results *scan_res,
			  struct wpa_ssid *group,
			  struct wpa_ssid **selected_ssid)
{
	struct wpa_bss *selected;

	wpa_printf(MSG_DEBUG, "Selecting BSS from priority group %d",
		   group->priority);

	/* First, try to find WPA-enabled AP */
	selected = wpa_supplicant_select_bss_wpa(wpa_s, scan_res, group,
						 selected_ssid);
	if (selected)
		return selected;

	/* If no WPA-enabled AP found, try to find non-WPA AP, if configuration
	 * allows this. */
	return wpa_supplicant_select_bss_non_wpa(wpa_s, scan_res, group,
						 selected_ssid);
}

struct wpa_bss *
wpa_supplicant_pick_network(struct wpa_supplicant *wpa_s,
			    struct wpa_scan_results *scan_res,
			    struct wpa_ssid **selected_ssid)
{
	struct wpa_bss *selected = NULL;
	int prio;

	while (selected == NULL) {
		for (prio = 0; prio < wpa_s->conf->num_prio; prio++) {
			selected = wpa_supplicant_select_bss(
				wpa_s, scan_res, wpa_s->conf->pssid[prio],
				selected_ssid);
			if (selected)
				break;
		}

		if (selected == NULL && wpa_s->blacklist) {
			wpa_printf(MSG_DEBUG, "No APs found - clear blacklist "
				   "and try again");
			wpa_blacklist_clear(wpa_s);
			wpa_s->blacklist_cleared++;
		} else if (selected == NULL)
			break;
	}

	return selected;
}

struct wpa_ssid *
wpa_supplicant_pick_new_network(struct wpa_supplicant *wpa_s)
{
	int prio;
	struct wpa_ssid *ssid;

	for (prio = 0; prio < wpa_s->conf->num_prio; prio++) {
		for (ssid = wpa_s->conf->pssid[prio]; ssid; ssid = ssid->pnext)
		{
			if (ssid->disabled)
				continue;
			if (ssid->mode == IEEE80211_MODE_IBSS ||
			    ssid->mode == IEEE80211_MODE_AP)
				return ssid;
		}
	}
	return NULL;
}

/* TODO: move the rsn_preauth_scan_result*() to be called from notify.c based
 * on BSS added and BSS changed events */
void wpa_supplicant_rsn_preauth_scan_results(
	struct wpa_supplicant *wpa_s, struct wpa_scan_results *scan_res)
{
	int i;

	if (rsn_preauth_scan_results(wpa_s->wpa) < 0)
		return;

	for (i = scan_res->num - 1; i >= 0; i--) {
		const u8 *ssid, *rsn;
		struct wpa_scan_res *r;

		r = scan_res->res[i];

		ssid = wpa_scan_get_ie(r, WLAN_EID_SSID);
		if (ssid == NULL)
			continue;

		rsn = wpa_scan_get_ie(r, WLAN_EID_RSN);
		if (rsn == NULL)
			continue;

		rsn_preauth_scan_result(wpa_s->wpa, r->bssid, ssid, rsn);
	}

}

int wpa_supplicant_need_to_roam(struct wpa_supplicant *wpa_s,
				       struct wpa_bss *selected,
				       struct wpa_ssid *ssid,
				       struct wpa_scan_results *scan_res)
{
	size_t i;
	struct wpa_scan_res *current_bss = NULL;
	int min_diff;

	if (wpa_s->reassociate)
		return 1; /* explicit request to reassociate */
	if (wpa_s->wpa_state < WPA_ASSOCIATED)
		return 1; /* we are not associated; continue */
	if (wpa_s->current_ssid == NULL)
		return 1; /* unknown current SSID */
	if (wpa_s->current_ssid != ssid)
		return 1; /* different network block */

	for (i = 0; i < scan_res->num; i++) {
		struct wpa_scan_res *res = scan_res->res[i];
		const u8 *ie;
		if (os_memcmp(res->bssid, wpa_s->bssid, ETH_ALEN) != 0)
			continue;

		ie = wpa_scan_get_ie(res, WLAN_EID_SSID);
		if (ie == NULL)
			continue;
		if (ie[1] != wpa_s->current_ssid->ssid_len ||
		    os_memcmp(ie + 2, wpa_s->current_ssid->ssid, ie[1]) != 0)
			continue;
		current_bss = res;
		break;
	}

	if (!current_bss)
		return 1; /* current BSS not seen in scan results */

	wpa_printf(MSG_DEBUG, "Considering within-ESS reassociation");
	wpa_printf(MSG_DEBUG, "Current BSS: " MACSTR " level=%d",
		   MAC2STR(current_bss->bssid), current_bss->level);
	wpa_printf(MSG_DEBUG, "Selected BSS: " MACSTR " level=%d",
		   MAC2STR(selected->bssid), selected->level);

	if (wpa_s->current_ssid->bssid_set &&
	    os_memcmp(selected->bssid, wpa_s->current_ssid->bssid, ETH_ALEN) ==
	    0) {
		wpa_printf(MSG_DEBUG, "Allow reassociation - selected BSS has "
			   "preferred BSSID");
		return 1;
	}

	min_diff = 2;
	if (current_bss->level < 0) {
		if (current_bss->level < -85)
			min_diff = 1;
		else if (current_bss->level < -80)
			min_diff = 2;
		else if (current_bss->level < -75)
			min_diff = 3;
		else if (current_bss->level < -70)
			min_diff = 4;
		else
			min_diff = 5;
	}
	if (abs(current_bss->level - selected->level) < min_diff) {
		wpa_printf(MSG_DEBUG, "Skip roam - too small difference in "
			   "signal level");
		return 0;
	}
}
#endif /* #if 0 */
#endif /* CONFIG_NO_SCAN_PROCESSING */

#ifdef CONFIG_DELAYED_MIC_ERROR_REPORT
void wpa_supplicant_delayed_mic_error_report(void *eloop_ctx,
						    void *sock_ctx)
{
	struct wpa_supplicant *wpa_s = eloop_ctx;

	if (!wpa_s->pending_mic_error_report)
		return;

	wpa_printf(MSG_DEBUG, "WPA: Sending pending MIC error report");
	wpa_sm_key_request(wpa_s->wpa, 1, wpa_s->pending_mic_error_pairwise);
	wpa_s->pending_mic_error_report = 0;
}
#endif /* CONFIG_DELAYED_MIC_ERROR_REPORT */

#if 0
void
wpa_supplicant_event_michael_mic_failure(struct wpa_supplicant *wpa_s,
					 union wpa_event_data *data)
{
	int pairwise;
	struct os_time t;

	wpa_msg(wpa_s, MSG_WARNING, "Michael MIC failure detected");
	pairwise = (data && data->michael_mic_failure.unicast);
	os_get_time(&t);
	if ((wpa_s->last_michael_mic_error &&
	     t.sec - wpa_s->last_michael_mic_error <= 60) ||
	    wpa_s->pending_mic_error_report) {
		if (wpa_s->pending_mic_error_report) {
			/*
			 * Send the pending MIC error report immediately since
			 * we are going to start countermeasures and AP better
			 * do the same.
			 */
			wpa_sm_key_request(wpa_s->wpa, 1,
					   wpa_s->pending_mic_error_pairwise);
		}

		/* Send the new MIC error report immediately since we are going
		 * to start countermeasures and AP better do the same.
		 */
		wpa_sm_key_request(wpa_s->wpa, 1, pairwise);

		/* initialize countermeasures */
		wpa_s->countermeasures = 1;
		wpa_msg(wpa_s, MSG_WARNING, "TKIP countermeasures started");

		/*
		 * Need to wait for completion of request frame. We do not get
		 * any callback for the message completion, so just wait a
		 * short while and hope for the best. */
		os_sleep(0, 10000);

		wpa_drv_set_countermeasures(wpa_s, 1);
		wpa_supplicant_deauthenticate(wpa_s,
					      WLAN_REASON_MICHAEL_MIC_FAILURE);

        /*
		eloop_cancel_timeout(wpa_supplicant_stop_countermeasures,
				     wpa_s, NULL);
            */
        /*
		eloop_register_timeout(60, 0,
				       wpa_supplicant_stop_countermeasures,
				       wpa_s, NULL);
	       */
		/* TODO: mark the AP rejected for 60 second. STA is
		 * allowed to associate with another AP.. */
	} else {
#ifdef CONFIG_DELAYED_MIC_ERROR_REPORT
		if (wpa_s->mic_errors_seen) {
			/*
			 * Reduce the effectiveness of Michael MIC error
			 * reports as a means for attacking against TKIP if
			 * more than one MIC failure is noticed with the same
			 * PTK. We delay the transmission of the reports by a
			 * random time between 0 and 60 seconds in order to
			 * force the attacker wait 60 seconds before getting
			 * the information on whether a frame resulted in a MIC
			 * failure.
			 */
			u8 rval[4];
			int sec;

			if (os_get_random(rval, sizeof(rval)) < 0)
				sec = os_random() % 60;
			else
				sec = WPA_GET_BE32(rval) % 60;
			wpa_printf(MSG_DEBUG, "WPA: Delay MIC error report %d "
				   "seconds", sec);
			wpa_s->pending_mic_error_report = 1;
			wpa_s->pending_mic_error_pairwise = pairwise;
			eloop_cancel_timeout(
				wpa_supplicant_delayed_mic_error_report,
				wpa_s, NULL);
			eloop_register_timeout(
				sec, os_random() % 1000000,
				wpa_supplicant_delayed_mic_error_report,
				wpa_s, NULL);
		} else {
			wpa_sm_key_request(wpa_s->wpa, 1, pairwise);
		}
#else /* CONFIG_DELAYED_MIC_ERROR_REPORT */
		wpa_sm_key_request(wpa_s->wpa, 1, pairwise);
#endif /* CONFIG_DELAYED_MIC_ERROR_REPORT */
	}
	wpa_s->last_michael_mic_error = t.sec;
	wpa_s->mic_errors_seen++;
}
#endif

#ifdef CONFIG_TERMINATE_ONLASTIF
int any_interfaces(struct wpa_supplicant *head)
{
	struct wpa_supplicant *wpa_s;

	for (wpa_s = head; wpa_s != NULL; wpa_s = wpa_s->next)
		if (!wpa_s->interface_removed)
			return 1;
	return 0;
}
#endif /* CONFIG_TERMINATE_ONLASTIF */

#if 0
void
wpa_supplicant_event_interface_status(struct wpa_supplicant *wpa_s,
				      union wpa_event_data *data)
{
	if (os_strcmp(wpa_s->ifname, data->interface_status.ifname) != 0)
		return;

	switch (data->interface_status.ievent) {
	case EVENT_INTERFACE_ADDED:
		if (!wpa_s->interface_removed)
			break;
		wpa_s->interface_removed = 0;
		wpa_printf(MSG_DEBUG, "Configured interface was added.");
		if (wpa_supplicant_driver_init(wpa_s) < 0) {
			wpa_printf(MSG_INFO, "Failed to initialize the driver "
				   "after interface was added.");
		}
		break;
	case EVENT_INTERFACE_REMOVED:
		wpa_printf(MSG_DEBUG, "Configured interface was removed.");
		wpa_s->interface_removed = 1;
		wpa_supplicant_mark_disassoc(wpa_s);
		l2_packet_deinit(wpa_s->l2);
		wpa_s->l2 = NULL;
#ifdef CONFIG_TERMINATE_ONLASTIF
		/* check if last interface */
		if (!any_interfaces(wpa_s->global->ifaces))
			eloop_terminate();
#endif /* CONFIG_TERMINATE_ONLASTIF */
		break;
	}
}
#endif /* #if 0 */

#ifdef CONFIG_PEERKEY
void
wpa_supplicant_event_stkstart(struct wpa_supplicant *wpa_s,
			      union wpa_event_data *data)
{
	if (data == NULL)
		return;
	wpa_sm_stkstart(wpa_s->wpa, data->stkstart.peer);
}
#endif /* CONFIG_PEERKEY */

#if 0
int wpa_supplicant_select_config(struct wpa_supplicant *wpa_s)
{
	struct wpa_ssid *ssid, *old_ssid;

	if (wpa_s->conf->ap_scan == 1 && wpa_s->current_ssid)
		return 0;

	wpa_printf(MSG_DEBUG, "Select network based on association "
		   "information");
	ssid = wpa_supplicant_get_ssid(wpa_s);
	if (ssid == NULL) {
		wpa_printf(MSG_INFO, "No network configuration found for the "
			   "current AP");
		return -1;
	}

	if (ssid->disabled) {
		wpa_printf(MSG_DEBUG, "Selected network is disabled");
		return -1;
	}

	wpa_printf(MSG_DEBUG, "Network configuration found for the current "
		   "AP");
	if (ssid->key_mgmt & (WPA_KEY_MGMT_PSK | WPA_KEY_MGMT_IEEE8021X |
			      WPA_KEY_MGMT_WPA_NONE |
			      WPA_KEY_MGMT_FT_PSK | WPA_KEY_MGMT_FT_IEEE8021X |
			      WPA_KEY_MGMT_PSK_SHA256 |
			      WPA_KEY_MGMT_IEEE8021X_SHA256)) {
		u8 wpa_ie[80];
		size_t wpa_ie_len = sizeof(wpa_ie);
		wpa_supplicant_set_suites(wpa_s, NULL, ssid,
					  wpa_ie, &wpa_ie_len);
	} else {
		wpa_supplicant_set_non_wpa_policy(wpa_s, ssid);
	}

	if (wpa_s->current_ssid && wpa_s->current_ssid != ssid)
		eapol_sm_invalidate_cached_session(wpa_s->eapol);
	old_ssid = wpa_s->current_ssid;
	wpa_s->current_ssid = ssid;
	wpa_supplicant_rsn_supp_set_config(wpa_s, wpa_s->current_ssid);
	wpa_supplicant_initiate_eapol(wpa_s);
	if (old_ssid != wpa_s->current_ssid)
		wpas_notify_network_changed(wpa_s);
	return 0;
}

void wpa_supplicant_stop_countermeasures(void *eloop_ctx,
						void *sock_ctx)
{
	struct wpa_supplicant *wpa_s = eloop_ctx;

	if (wpa_s->countermeasures) {
		wpa_s->countermeasures = 0;
		wpa_drv_set_countermeasures(wpa_s, 0);
		wpa_msg(wpa_s, MSG_INFO, "WPA: TKIP countermeasures stopped");
		wpa_supplicant_req_scan(wpa_s, 0, 0);
	}
}

void wpa_find_assoc_pmkid(struct wpa_supplicant *wpa_s)
{
	struct wpa_ie_data ie;
	int pmksa_set = -1;
	size_t i;

	if (wpa_sm_parse_own_wpa_ie(wpa_s->wpa, &ie) < 0 ||
	    ie.pmkid == NULL)
		return;

	for (i = 0; i < ie.num_pmkid; i++) {
		pmksa_set = pmksa_cache_set_current(wpa_s->wpa,
						    ie.pmkid + i * PMKID_LEN,
						    NULL, NULL, 0);
		if (pmksa_set == 0) {
			eapol_sm_notify_pmkid_attempt(wpa_s->eapol, 1);
			break;
		}
	}

	wpa_printf(MSG_DEBUG, "RSN: PMKID from assoc IE %sfound from PMKSA "
		   "cache", pmksa_set == 0 ? "" : "not ");
}

void wpa_supplicant_event_pmkid_candidate(struct wpa_supplicant *wpa_s,
						 union wpa_event_data *data)
{
	if (data == NULL) {
		wpa_printf(MSG_DEBUG, "RSN: No data in PMKID candidate event");
		return;
	}
	wpa_printf(MSG_DEBUG, "RSN: PMKID candidate event - bssid=" MACSTR
		   " index=%d preauth=%d",
		   MAC2STR(data->pmkid_candidate.bssid),
		   data->pmkid_candidate.index,
		   data->pmkid_candidate.preauth);

	pmksa_candidate_add(wpa_s->wpa, data->pmkid_candidate.bssid,
			    data->pmkid_candidate.index,
			    data->pmkid_candidate.preauth);
}
#endif /* #if 0 */
