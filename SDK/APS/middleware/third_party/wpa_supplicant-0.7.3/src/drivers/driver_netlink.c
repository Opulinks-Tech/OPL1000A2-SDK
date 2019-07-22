/*
 * Driver interface for RADIUS server or WPS ER only (no driver)
 * Copyright (c) 2008, Atheros Communications
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
#include "cmsis_os.h"
#include "driver_netlink.h"
#include "events_netlink.h" //I0051 0000908
#include "ieee802_11_defs.h" //I0060 0000905
#include "msg.h"
#include "wpa_common.h" //I0060 0000905
#include "wifi_nvm.h"

static const char *TAG="DRV";

osSemaphoreId gConnectedSem = NULL;
u8 g_wifi_certification = WIFI_CERTIFIED_NONE;

typedef int (*wifi_mac_xmit_eapol_frame_fn_t)(u8 *buf, size_t len, const u8 *eth_dest, u16 proto);

extern u8 g_fastconn;  //I0064  0000736
extern u8 g_bssid[6];
extern struct wpa_supplicant *wpa_s;
extern wifi_mac_xmit_eapol_frame_fn_t wifi_mac_xmit_eapol_frame;
extern u8 g_wifi_reconnection_counter;  //I0064  0000736
extern char g_passphrase[MAX_LEN_OF_PASSWD]; //I0060 0000635
extern int wpa_debug_level;

void * wpa_driver_netlink_init_impl(void *ctx, const char *ifname)
{
	//Reserved
	return NULL;
}

void wpa_driver_netlink_deinit_impl(void *priv)
{
	//Reserved
}

Boolean wpa_driver_netlink_scan_impl(int mode)
{
    if (wifi_ctrl_state_get() == WIFI_CTRL_SCANNING_ST ||
        wifi_ctrl_state_get() == WIFI_CTRL_CONNECTING_ST ||
        wifi_ctrl_state_get() == WIFI_CTRL_CONNECTED_SCAN_ST) {
        log_print(LOG_MED_LEVEL, TAG, "WPA: Ignore this command since now is processing\r\n");
        return FALSE;
    }
    else {
        if (wifi_ctrl_state_get() != WIFI_CTRL_CONNECTED_ST) {
            wpa_supplicant_set_state(wpa_s, WPA_SCANNING);
            wifi_ctrl_state_set(WIFI_CTRL_SCANNING_ST);
        }
        wifi_scan_req(mode);
    }
    
    return TRUE;
}

Boolean wpa_driver_netlink_scan_stop_impl(void)
{
    if (wifi_ctrl_state_get() == WIFI_CTRL_SCANNING_ST) {
        wpa_supplicant_set_state(wpa_s, WPA_INACTIVE);
        wifi_ctrl_state_set(WIFI_CTRL_DISCONNECT_ST);
        wifi_scan_stop_req();
        return TRUE;
    }
    
    if (wifi_ctrl_state_get() == WIFI_CTRL_CONNECTED_SCAN_ST) {
        wifi_scan_stop_req();
    }
    
    return TRUE;
}

Boolean wpa_driver_netlink_get_scan_results_impl(struct wpa_scan_results * scan_res)
{
    scan_report_t *result = NULL;
    unsigned int apNum = 0;
    int i = 0;

    if(scan_res == NULL) return FALSE;

    wpa_driver_netlink_scan_results_clear(scan_res);

    result = wifi_get_scan_result();
    if (result == NULL) {
        log_print(LOG_HIGH_LEVEL, TAG, "WPA: result == NULL \r\n");
        return FALSE;
    }

    apNum = (unsigned int) result->uScanApNum;
    if (apNum == 0) {
        log_print(LOG_HIGH_LEVEL, TAG, "ap num=0 \r\n");
        return TRUE;
    }

    scan_res->res = os_malloc(sizeof(struct wpa_scan_res) * apNum);

    if(scan_res->res == NULL)
    {
        log_print(LOG_HIGH_LEVEL, TAG, "WPA: scan_res->res == NULL \r\n");
        return FALSE;
    }

    for (i = 0; i < apNum; i++) {
        memcpy(scan_res->res[i]->bssid, result->pScanInfo[i].bssid, ETH_ALEN);
        scan_res->res[i]->freq = (int) result->pScanInfo[i].ap_channel;
        scan_res->res[i]->beacon_int = (unsigned short) result->pScanInfo[i].beacon_interval;
        scan_res->res[i]->caps = (unsigned short) result->pScanInfo[i].capabilities;
        scan_res->res[i]->level = (int) result->pScanInfo[i].rssi;
    }

    scan_res->num = apNum;

    return TRUE;
}

int wpa_driver_netlink_wpa_cipher_txt_impl(wpa_ie_data_t *data, int pos, char *str)
{
    if (data->pairwise_cipher & WPA_CIPHER_CCMP) {
        pos += snprintf(str + pos, SCAN_STRING_LENGTH - pos, "CCMP");
    }
    else if (data->pairwise_cipher & WPA_CIPHER_TKIP) {
        pos += snprintf(str + pos, SCAN_STRING_LENGTH - pos, "TKIP");
    }
    else if (data->pairwise_cipher & WPA_CIPHER_NONE) {
        pos += snprintf(str + pos, SCAN_STRING_LENGTH - pos, "NONE");
    }
    return pos;
}

int wpa_driver_netlink_wpa_ie_txt_impl(wpa_ie_data_t *data, int pos, char *str)
{
    if (data->key_mgmt & WPA_KEY_MGMT_IEEE8021X) {
        pos += snprintf(str + pos, SCAN_STRING_LENGTH - pos, "EAP-");
    }
    else if (data->key_mgmt & WPA_KEY_MGMT_PSK){
        pos += snprintf(str + pos, SCAN_STRING_LENGTH - pos, "PSK-");
    }
    else if (data->key_mgmt & WPA_KEY_MGMT_WPA_NONE) {
        pos += snprintf(str + pos, SCAN_STRING_LENGTH - pos, "NONE");
    }
    
    wpa_driver_netlink_wpa_cipher_txt(data, pos, str);
    
    return pos;
}

void wpa_driver_netlink_show_scan_results_impl(void)
{
    scan_report_t *result = NULL;
    unsigned int apNum = 0;
    char str[SCAN_STRING_LENGTH];
    int i = 0, pos;
    bool wpa = false, wpa2 = false;
    
    result = wifi_get_scan_result();

    if (result == NULL) {
        log_print(LOG_HIGH_LEVEL, TAG, "[DRV]WPA: result == NULL\r\n");
        goto done;
    }

    apNum = (unsigned int) result->uScanApNum;

    if (apNum == 0) {
        goto done;
    }

    tracer_cli(LOG_HIGH_LEVEL, "\tSSID\t\t          BSSID          CHANNEL   RSSI    SECURITY \r\n");
    for (i = 0; i < apNum; i++) {
        memset(str, 0, sizeof(str));
        wpa = false;  wpa2 = false;
        
        tracer_cli(LOG_HIGH_LEVEL, "%-26s  ", result->pScanInfo[i].ssid);
        tracer_cli(LOG_HIGH_LEVEL, "%02x:%02x:%02x:%02x:%02x:%02x  ",
                  result->pScanInfo[i].bssid[0], result->pScanInfo[i].bssid[1], result->pScanInfo[i].bssid[2],
                  result->pScanInfo[i].bssid[3], result->pScanInfo[i].bssid[4], result->pScanInfo[i].bssid[5]);
        tracer_cli(LOG_HIGH_LEVEL, "    %2d  ", result->pScanInfo[i].ap_channel);
        tracer_cli(LOG_HIGH_LEVEL, "    %2d  ", result->pScanInfo[i].rssi);
        
        if (result->pScanInfo[i].capabilities & IEEE80211_CAP_PRIVACY) {
            if (result->pScanInfo[i].rsn_ie[0] == WIFIS_EID_RSN) {
                wpa2 = true;
                pos = snprintf(str, SCAN_STRING_LENGTH, "RSN-");
                wpa_driver_netlink_wpa_ie_txt(&result->pScanInfo[i].wpa_data, pos, str);
            }
            else if (result->pScanInfo[i].wpa_ie[0] == WIFIS_EID_VENDOR_SPECIFIC &&
                     result->pScanInfo[i].wpa_ie[1] >= 4 &&
                     WPA_GET_BE32(&result->pScanInfo[i].wpa_ie[2])== WPAS_IE_VENDOR_TYPE) {
                wpa = true;
                pos = snprintf(str, SCAN_STRING_LENGTH, "WPA-");
                wpa_driver_netlink_wpa_ie_txt(&result->pScanInfo[i].wpa_data, pos, str);
            }
            
            if (!wpa && !wpa2) {
                snprintf(str, SCAN_STRING_LENGTH, "WEP");
            }
            
        }
        else {
            snprintf(str, SCAN_STRING_LENGTH, "NONE");
        }
        
        tracer_cli(LOG_HIGH_LEVEL, "   %s\n", str);
    }

    tracer_cli(LOG_HIGH_LEVEL, "\r\nAP number = %d\r\n", apNum);
    
done:
    return;
}

int get_wifi_certified_mode_impl(void)
{
    return g_wifi_certification;
}

int set_wifi_certified_mode_impl(u8 mode)
{
    if (mode > WIFI_CERTIFIED)
        return -1;
    g_wifi_certification = mode;
    return TRUE;
}

int connection_connected_ctrl_impl(void)
{
    log_print(LOG_LOW_LEVEL, TAG, "WPA: Disconnect since already connected before\r\n");
    
    if (!gConnectedSem) {
        osSemaphoreDef_t SemDef = {0};
        gConnectedSem = osSemaphoreCreate(&SemDef, 1);
        if (gConnectedSem == NULL) {
            log_print(LOG_HIGH_LEVEL, TAG, "WPA: Initial connected semaphore failed.\r\n");
            return FALSE;
        }
        osSemaphoreWait(gConnectedSem, osWaitForever);
    }
    
    wpa_driver_netlink_disconnect(NULL, 0);
    
    // wait disconnect event from M0 to release the semaphore
    osSemaphoreWait(gConnectedSem, osWaitForever);
    
    if (osSemaphoreDelete(gConnectedSem)) {
        log_print(LOG_HIGH_LEVEL, TAG, "WPA: Delete semaphore failed.\r\n");
        return FALSE;
    }
    gConnectedSem = NULL;
    
    return TRUE;
}

//I0060 0000905_modify_beg
scan_info_t *get_target_ap_record_impl(u8 *mac, char *ssid)
{
    scan_info_t *pInfo = NULL;
    
    if (!is_zero_ether_addr(mac)) {
        pInfo = wifi_get_scan_record(mac);
    }
    else {
        pInfo = wifi_get_scan_record_by_ssid(ssid);
    }
    
    if (pInfo != NULL) {
        return pInfo;
    }
    
    return NULL;
}

int target_ap_security_mode_chk_impl(scan_info_t *pInfo)
{
    if (pInfo == NULL) {
        log_print(LOG_HIGH_LEVEL, TAG, "WPA: AP index is null \r\n");
        return false;
    }
        
    // AP security mode is off
    if (pInfo->rsn_ie[0] == 0 && pInfo->wpa_ie[0] == 0) {
        if (strlen(g_passphrase) != 0) {
            log_print(LOG_HIGH_LEVEL, TAG, "WPA: AP mode is none security, should not has passphrase \r\n");
            return false;
        }
    }
    else {
        if (strlen(g_passphrase) == 0) {
            log_print(LOG_HIGH_LEVEL, TAG, "WPA: AP mode is security, should has passphrase \r\n");
            return false;
        }
    }
    
    return TRUE;
}
//I0060 0000905_modify_end

/**
 * @brief This function will be check target AP's RSNE format correct or not
 *
 *    1. The RSNE information collect by M0 in Beacon or Probe Response frame.
 *    2. Bypass OPEN/WPA mode and return true
 *    3. Check RSNE element ID, length and version
 *    4. Check Group Cipher OUI and Type
 *    5. Check Pairwise Cipher Count and OUI
 *    6. Check AuthKey Mngmnt Count and AKMP suite OUI
 *
 *  RSN Information
 *    Element ID : 0x30
 *    Length     : 0x14
 *    Version    : 0x0100
 *    Group Cipher OUI       : 0x 00-0F-AC
 *    Group Cipher Type      : 0x02
 *    Pairwise Cipher Count  : 0x0100
 *    Pairwise Cipher OUI    : 0x 00-0F-AC-04
 *    AuthKey Mngmnt Count   : 0x1000
 *    AKMP suite OUI      : 0x 00-0F-AC-02
 *    RSN Capabilities    : 0x0000
 *    PMKID Count         : 0x0000
 *    PMKID List          : (16 bytes)
 *    Group Management Cipher Suite : 0x 00-0F-AC-06
 *
 * @param pInfo [in] The pointer to point target AP in scan list
 *
 * @return true  : PASS RSNE check
 *         false : Invalid RSNE format
 */
int target_ap_rsne_format_chk_impl(scan_info_t *pInfo)
{
    const struct rsn_ie_hdr *hdr;
    const u8 *pos;
    u8 rsn_ie_len; // total length of RSNE
    int i, left, count;
    
    // bypass OPEN/WPA mode
    if ((pInfo->rsn_ie[0] == 0 && pInfo->wpa_ie[0] == 0) ||
         pInfo->rsn_ie[0] == 0) {
        return true;
    }
    
    hdr = (struct rsn_ie_hdr *)&pInfo->rsn_ie[0];
    
    if (hdr->len < sizeof(struct rsn_ie_hdr)) {
        log_print(LOG_HIGH_LEVEL, TAG, "WPA: RSNE len too short. \r\n");
        return false;
    }
    
    if (hdr->elem_id != WLAN_EID_RSN ||
        WPA_GET_LE16(hdr->version) != RSN_VERSION) {
        log_print(LOG_HIGH_LEVEL, TAG, "WPA: RSNE malformed ie or unknown version. \r\n");
        return false;
    }
    
    rsn_ie_len = hdr->len + 2; // add 2 bytes since RSN length not including elem_id and hdr->len
    
    pos = (const u8 *) (hdr + 1); // pos->Group Cipher OUI
    left = rsn_ie_len - sizeof(*hdr); //22 - 4 = 18
    
    if (left >= RSN_SELECTOR_LEN) {
        //Check Group Cipher OUI and Type
        if (!wpa_cipher_valid_group(rsn_selector_to_bitfield(pos))) {
            log_print(LOG_HIGH_LEVEL, TAG, "WPA: RSNE invalid group cipher (%08x). \r\n", WPA_GET_BE32(pos));
            return false;
        }
        pos += RSN_SELECTOR_LEN;  //pos->Pairwise Cipher Count
        left -= RSN_SELECTOR_LEN; //18 - 4 = 14
    }
    else if (left > 0) {
        log_print(LOG_HIGH_LEVEL, TAG, "WPA: RSNE ie length mismatch, %u too much. \r\n", left);
        return false;
    }
    
    //Check Pairwise Cipher Count and OUI
    if (left >= 2) {
        count = WPA_GET_LE16(pos); //get Pairwise Cipher Count
        pos += 2;   //pos->Pairwise Cipher OUI
        left -= 2;  //14 - 2 = 12
        if (count == 0 || (count > (left / RSN_SELECTOR_LEN))) {
            log_print(LOG_HIGH_LEVEL, TAG, "WPA: RSNE ie count botch (pairwise), count %u left %u. \r\n", 
                      count, left);
            return false;
        }
        
        for (i=0; i<count; i++) {
            pos += RSN_SELECTOR_LEN;  //pos->AKMP suite OUI
            left -= RSN_SELECTOR_LEN; //12 - 4 = 8 (if count = 1)
        }
    }
    else if (left == 1) {
        log_print(LOG_HIGH_LEVEL, TAG, "WPA: RSNE ie too short (for key mgmt). \r\n");
        return false;
    }
    
    //Check AuthKey Mngmnt Count and AKMP suite OUI
    if (left >= 2) {
        count = WPA_GET_LE16(pos); //get AuthKey Mngmnt Count
        pos += 2;  //pos->AKMP suite OUI
        left -= 2; //8 - 2 = 6
        if (count == 0 || (count > (left / RSN_SELECTOR_LEN))) {
            log_print(LOG_HIGH_LEVEL, TAG, "WPA: RSNE ie count botch (key mgmt), count %u left %u. \r\n", 
                      count, left);
            return false;
        }
        
        for (i=0; i<count; i++) {
            pos += RSN_SELECTOR_LEN;  //pos->RSN Capabilities
            left -= RSN_SELECTOR_LEN; //6 - 4 = 2 (if count = 1)
        }
    }
    else if (left == 1) {
        log_print(LOG_HIGH_LEVEL, TAG, "WPA: RSNE ie ie too short (for capabilities). \r\n");
        return false;
    }
    
    if (left >= 2) {
        pos += 2;  //pos->end
        left -= 2; // 2 - 2 = 0
    }
    
    if (left >= 2) {
        u16 num_pmkid = WPA_GET_LE16(pos);
        pos += 2;
        left -= 2;
        if (num_pmkid > (unsigned int) left / PMKID_LEN) {
            log_print(LOG_HIGH_LEVEL, TAG, "WPA: PMKID underflow, (num_pmkid=%u left=%d). \r\n", 
                      num_pmkid, left);
            return false;
        }
        else {
            pos += num_pmkid * PMKID_LEN;
            left -= num_pmkid * PMKID_LEN;
        }
        
        // For WiFi certification 5.2.4 scenario F
        if (num_pmkid >= 1) {
            return false;
        }
    }
    
    if (left > 0) {
        if (left < 4) {
            log_print(LOG_HIGH_LEVEL, TAG, "WPA: RSNE wpa_parse_wpa_ie_rsn: ignore trailing bytes. \r\n");
            return false;
        }
    }
    
    return true;
}

#if 0 // for debug use (Wifi Certified RSNE values test)
int test_dummy_rsne_format(scan_info_t *pInfo)
{
    u8 dummy_rsne[44] = {0};

    //Scenario A
    dummy_rsne[0] = 0x30; dummy_rsne[1] = 0x16; dummy_rsne[2] = 0x02; dummy_rsne[3] = 0x00;
    dummy_rsne[4] = 0x00; dummy_rsne[5] = 0x0F; dummy_rsne[6] = 0xAC; dummy_rsne[7] = 0x04;
    dummy_rsne[8] = 0x01; dummy_rsne[9] = 0x00; 
    dummy_rsne[10] = 0x00; dummy_rsne[11] = 0x0F; dummy_rsne[12] = 0xAC; dummy_rsne[13] = 0x04;
    dummy_rsne[14] = 0x01; dummy_rsne[15] = 0x00;
    dummy_rsne[16] = 0x00; dummy_rsne[17] = 0x0F; dummy_rsne[18] = 0xAC; dummy_rsne[19] = 0x02;
    dummy_rsne[20] = 0x80; dummy_rsne[21] = 0x00;
    memcpy(pInfo->rsn_ie, &dummy_rsne[0], sizeof(dummy_rsne));
    log_print(LOG_HIGH_LEVEL, TAG, "[TEST] verifies 5.2.2 STAUT handles RSNE scenario A. \r\n");
    target_ap_rsne_format_chk(pInfo);
    
    //Scenario B
    memset(&dummy_rsne[0], 0, sizeof(dummy_rsne));
    dummy_rsne[0] = 0x30; dummy_rsne[1] = 0x12; dummy_rsne[2] = 0x01; dummy_rsne[3] = 0x00;
    dummy_rsne[4] = 0x00; dummy_rsne[5] = 0x0F; dummy_rsne[6] = 0xAC; dummy_rsne[7] = 0x04;
    dummy_rsne[8] = 0x00; dummy_rsne[9] = 0x00;
    dummy_rsne[10] = 0x01; dummy_rsne[11] = 0x00;
    dummy_rsne[12] = 0x00; dummy_rsne[13] = 0x0F; dummy_rsne[14] = 0xAC; dummy_rsne[15] = 0x02;
    dummy_rsne[16] = 0x80; dummy_rsne[17] = 0x00;
    memcpy(pInfo->rsn_ie, &dummy_rsne[0], sizeof(dummy_rsne));
    log_print(LOG_HIGH_LEVEL, TAG, "[TEST] verifies 5.2.2 STAUT handles RSNE scenario B. \r\n");
    target_ap_rsne_format_chk(pInfo);
    
    //Scenario C
    memset(&dummy_rsne[0], 0, sizeof(dummy_rsne));
    dummy_rsne[0] = 0x30; dummy_rsne[1] = 0x10; dummy_rsne[2] = 0x01; dummy_rsne[3] = 0x00;
    dummy_rsne[4] = 0x00; dummy_rsne[5] = 0x0F; dummy_rsne[6] = 0xAC; dummy_rsne[7] = 0x04;
    dummy_rsne[8] = 0x01; dummy_rsne[9] = 0x00; 
    dummy_rsne[10] = 0x00; dummy_rsne[11] = 0x0F; dummy_rsne[12] = 0xAC; dummy_rsne[13] = 0x04;
    dummy_rsne[14] = 0x00; dummy_rsne[15] = 0x00;
    dummy_rsne[16] = 0x80; dummy_rsne[17] = 0x00;
    memcpy(pInfo->rsn_ie, &dummy_rsne[0], sizeof(dummy_rsne));
    log_print(LOG_HIGH_LEVEL, TAG, "[TEST] verifies 5.2.2 STAUT handles RSNE scenario C. \r\n");
    target_ap_rsne_format_chk(pInfo);
    
    //Scenario D
    memset(&dummy_rsne[0], 0, sizeof(dummy_rsne));
    dummy_rsne[0] = 0x30; dummy_rsne[1] = 0x0C; dummy_rsne[2] = 0x01; dummy_rsne[3] = 0x00;
    dummy_rsne[4] = 0x00; dummy_rsne[5] = 0x0F; dummy_rsne[6] = 0xAC; dummy_rsne[7] = 0x04;
    dummy_rsne[8] = 0x00; dummy_rsne[9] = 0x00;
    dummy_rsne[10] = 0x00; dummy_rsne[11] = 0x00;
    dummy_rsne[12] = 0x80; dummy_rsne[13] = 0x00;
    memcpy(pInfo->rsn_ie, &dummy_rsne[0], sizeof(dummy_rsne));
    log_print(LOG_HIGH_LEVEL, TAG, "[TEST] verifies 5.2.2 STAUT handles RSNE scenario D. \r\n");
    target_ap_rsne_format_chk(pInfo);
    
    //Scenario F
    memset(&dummy_rsne[0], 0, sizeof(dummy_rsne));
    dummy_rsne[0] = 0x30; dummy_rsne[1] = 0x01; dummy_rsne[2] = 0x01;
    memcpy(pInfo->rsn_ie, &dummy_rsne[0], sizeof(dummy_rsne));
    log_print(LOG_HIGH_LEVEL, TAG, "[TEST] verifies 5.2.2 STAUT handles RSNE scenario F. \r\n");
    target_ap_rsne_format_chk(pInfo);
    
    //Scenario G
    memset(&dummy_rsne[0], 0, sizeof(dummy_rsne));
    dummy_rsne[0] = 0x30; dummy_rsne[1] = 0x05; dummy_rsne[2] = 0x01; dummy_rsne[3] = 0x00;
    dummy_rsne[4] = 0x00; dummy_rsne[5] = 0x0F; dummy_rsne[6] = 0xAC;
    memcpy(pInfo->rsn_ie, &dummy_rsne[0], sizeof(dummy_rsne));
    log_print(LOG_HIGH_LEVEL, TAG, "[TEST] verifies 5.2.2 STAUT handles RSNE scenario G. \r\n");
    target_ap_rsne_format_chk(pInfo);
    
    //Scenario H
    memset(&dummy_rsne[0], 0, sizeof(dummy_rsne));
    dummy_rsne[0] = 0x30; dummy_rsne[1] = 0x0B; dummy_rsne[2] = 0x01; dummy_rsne[3] = 0x00;
    dummy_rsne[4] = 0x00; dummy_rsne[5] = 0x0F; dummy_rsne[6] = 0xAC; dummy_rsne[7] = 0x04;
    dummy_rsne[8] = 0x01; dummy_rsne[9] = 0x00;
    dummy_rsne[10] = 0x00; dummy_rsne[11] = 0x0F; dummy_rsne[12] = 0xAC;
    memcpy(pInfo->rsn_ie, &dummy_rsne[0], sizeof(dummy_rsne));
    log_print(LOG_HIGH_LEVEL, TAG, "[TEST] verifies 5.2.2 STAUT handles RSNE scenario H. \r\n");
    target_ap_rsne_format_chk(pInfo);
    
    //Scenario I
    memset(&dummy_rsne[0], 0, sizeof(dummy_rsne));
    dummy_rsne[0] = 0x30; dummy_rsne[1] = 0x11; dummy_rsne[2] = 0x01; dummy_rsne[3] = 0x00;
    dummy_rsne[4] = 0x00; dummy_rsne[5] = 0x0F; dummy_rsne[6] = 0xAC; dummy_rsne[7] = 0x04;
    dummy_rsne[8] = 0x01; dummy_rsne[9] = 0x00;
    dummy_rsne[10] = 0x00; dummy_rsne[11] = 0x0F; dummy_rsne[12] = 0xAC; dummy_rsne[13] = 0x04;
    dummy_rsne[14] = 0x01; dummy_rsne[15] = 0x00;
    dummy_rsne[16] = 0x00; dummy_rsne[17] = 0x0F; dummy_rsne[18] = 0xAC;
    memcpy(pInfo->rsn_ie, &dummy_rsne[0], sizeof(dummy_rsne));
    log_print(LOG_HIGH_LEVEL, TAG, "[TEST] verifies 5.2.2 STAUT handles RSNE scenario I. \r\n");
    target_ap_rsne_format_chk(pInfo);
    
    //Scenario J
    memset(&dummy_rsne[0], 0, sizeof(dummy_rsne));
    dummy_rsne[0] = 0x30; dummy_rsne[1] = 0x13; dummy_rsne[2] = 0x01; dummy_rsne[3] = 0x00;
    dummy_rsne[4] = 0x00; dummy_rsne[5] = 0x0F; dummy_rsne[6] = 0xAC; dummy_rsne[7] = 0x04;
    dummy_rsne[8] = 0x01; dummy_rsne[9] = 0x00;
    dummy_rsne[10] = 0x00; dummy_rsne[11] = 0x0F; dummy_rsne[12] = 0xAC; dummy_rsne[13] = 0x04;
    dummy_rsne[14] = 0x01; dummy_rsne[15] = 0x00;
    dummy_rsne[16] = 0x00; dummy_rsne[17] = 0x0F; dummy_rsne[18] = 0xAC; dummy_rsne[19] = 0x02;
    dummy_rsne[20] = 0x80;
    memcpy(pInfo->rsn_ie, &dummy_rsne[0], sizeof(dummy_rsne));
    log_print(LOG_HIGH_LEVEL, TAG, "[TEST] verifies 5.2.2 STAUT handles RSNE scenario J. \r\n");
    target_ap_rsne_format_chk(pInfo);
    
    //Scenario K
    memset(&dummy_rsne[0], 0, sizeof(dummy_rsne));
    dummy_rsne[0] = 0x30; dummy_rsne[1] = 0x15; dummy_rsne[2] = 0x01; dummy_rsne[3] = 0x00;
    dummy_rsne[4] = 0x00; dummy_rsne[5] = 0x0F; dummy_rsne[6] = 0xAC; dummy_rsne[7] = 0x04;
    dummy_rsne[8] = 0x01; dummy_rsne[9] = 0x00;
    dummy_rsne[10] = 0x00; dummy_rsne[11] = 0x0F; dummy_rsne[12] = 0xAC; dummy_rsne[13] = 0x04;
    dummy_rsne[14] = 0x01; dummy_rsne[15] = 0x00;
    dummy_rsne[16] = 0x00; dummy_rsne[17] = 0x0F; dummy_rsne[18] = 0xAC; dummy_rsne[19] = 0x02;
    dummy_rsne[20] = 0x80; dummy_rsne[21] = 0x00;
    memcpy(pInfo->rsn_ie, &dummy_rsne[0], sizeof(dummy_rsne));
    log_print(LOG_HIGH_LEVEL, TAG, "[TEST] verifies 5.2.2 STAUT handles RSNE scenario K. \r\n");
    target_ap_rsne_format_chk(pInfo);
    
    //Scenario L
    memset(&dummy_rsne[0], 0, sizeof(dummy_rsne));
    dummy_rsne[0] = 0x30; dummy_rsne[1] = 0x0C; dummy_rsne[2] = 0x01; dummy_rsne[3] = 0x00;
    dummy_rsne[4] = 0x00; dummy_rsne[5] = 0x0F; dummy_rsne[6] = 0xAC; dummy_rsne[7] = 0x04;
    dummy_rsne[8] = 0x02; dummy_rsne[9] = 0x00;
    dummy_rsne[10] = 0x00; dummy_rsne[11] = 0x0F; dummy_rsne[12] = 0xAC; dummy_rsne[13] = 0x04;
    memcpy(pInfo->rsn_ie, &dummy_rsne[0], sizeof(dummy_rsne));
    log_print(LOG_HIGH_LEVEL, TAG, "[TEST] verifies 5.2.2 STAUT handles RSNE scenario L. \r\n");
    target_ap_rsne_format_chk(pInfo);
    
    //Scenario M
    memset(&dummy_rsne[0], 0, sizeof(dummy_rsne));
    dummy_rsne[0] = 0x30; dummy_rsne[1] = 0x12; dummy_rsne[2] = 0x01; dummy_rsne[3] = 0x00;
    dummy_rsne[4] = 0x00; dummy_rsne[5] = 0x0F; dummy_rsne[6] = 0xAC; dummy_rsne[7] = 0x04;
    dummy_rsne[8] = 0x01; dummy_rsne[9] = 0x00;
    dummy_rsne[10] = 0x00; dummy_rsne[11] = 0x0F; dummy_rsne[12] = 0xAC; dummy_rsne[13] = 0x04;
    dummy_rsne[14] = 0x02; dummy_rsne[15] = 0x00;
    dummy_rsne[16] = 0x00; dummy_rsne[17] = 0x0F; dummy_rsne[18] = 0xAC; dummy_rsne[19] = 0x02;
    memcpy(pInfo->rsn_ie, &dummy_rsne[0], sizeof(dummy_rsne));
    log_print(LOG_HIGH_LEVEL, TAG, "[TEST] verifies 5.2.2 STAUT handles RSNE scenario M. \r\n");
    target_ap_rsne_format_chk(pInfo);
    
    //Wifi Certified 5.2.3 verification of RSN Capabilities test
    //Scenario A
    memset(&dummy_rsne[0], 0, sizeof(dummy_rsne));
    dummy_rsne[0] = 0x30; dummy_rsne[1] = 0x16; dummy_rsne[2] = 0x01; dummy_rsne[3] = 0x00;
    dummy_rsne[4] = 0x00; dummy_rsne[5] = 0x0F; dummy_rsne[6] = 0xAC; dummy_rsne[7] = 0x04;
    dummy_rsne[8] = 0x01; dummy_rsne[9] = 0x00;
    dummy_rsne[10] = 0x00; dummy_rsne[11] = 0x0F; dummy_rsne[12] = 0xAC; dummy_rsne[13] = 0x04;
    dummy_rsne[14] = 0x01; dummy_rsne[15] = 0x00;
    dummy_rsne[16] = 0x00; dummy_rsne[17] = 0x0F; dummy_rsne[18] = 0xAC; dummy_rsne[19] = 0x02;
    dummy_rsne[20] = 0x80; dummy_rsne[21] = 0x00;
    memcpy(pInfo->rsn_ie, &dummy_rsne[0], sizeof(dummy_rsne));
    log_print(LOG_HIGH_LEVEL, TAG, "[TEST] verifies 5.2.3 STAUT handles RSNE scenario A. \r\n");
    if(target_ap_rsne_format_chk(pInfo)) {
        log_print(LOG_HIGH_LEVEL, TAG, "    PASS \r\n");
    }
    
    //Scenario B
    memset(&dummy_rsne[0], 0, sizeof(dummy_rsne));
    dummy_rsne[0] = 0x30; dummy_rsne[1] = 0x16; dummy_rsne[2] = 0x01; dummy_rsne[3] = 0x00;
    dummy_rsne[4] = 0x00; dummy_rsne[5] = 0x0F; dummy_rsne[6] = 0xAC; dummy_rsne[7] = 0x04;
    dummy_rsne[8] = 0x01; dummy_rsne[9] = 0x00;
    dummy_rsne[10] = 0x00; dummy_rsne[11] = 0x0F; dummy_rsne[12] = 0xAC; dummy_rsne[13] = 0x04;
    dummy_rsne[14] = 0x01; dummy_rsne[15] = 0x00;
    dummy_rsne[16] = 0x00; dummy_rsne[17] = 0x0F; dummy_rsne[18] = 0xAC; dummy_rsne[19] = 0x02;
    dummy_rsne[20] = 0x80; dummy_rsne[21] = 0x3F;
    memcpy(pInfo->rsn_ie, &dummy_rsne[0], sizeof(dummy_rsne));
    log_print(LOG_HIGH_LEVEL, TAG, "[TEST] verifies 5.2.3 STAUT handles RSNE scenario B. \r\n");
    if(target_ap_rsne_format_chk(pInfo)) {
        log_print(LOG_HIGH_LEVEL, TAG, "    PASS \r\n");
    }
    
    //Wifi Certified 5.2.4 verification of RSN Capabilities test
    //Scenario A
    memset(&dummy_rsne[0], 0, sizeof(dummy_rsne));
    dummy_rsne[0] = 0x30; dummy_rsne[1] = 0x12; dummy_rsne[2] = 0x01; dummy_rsne[3] = 0x00;
    dummy_rsne[4] = 0x00; dummy_rsne[5] = 0x0F; dummy_rsne[6] = 0xAC; dummy_rsne[7] = 0x04;
    dummy_rsne[8] = 0x01; dummy_rsne[9] = 0x00;
    dummy_rsne[10] = 0x00; dummy_rsne[11] = 0x0F; dummy_rsne[12] = 0xAC; dummy_rsne[13] = 0x04;
    dummy_rsne[14] = 0x01; dummy_rsne[15] = 0x00;
    dummy_rsne[16] = 0x00; dummy_rsne[17] = 0x0F; dummy_rsne[18] = 0xAC; dummy_rsne[19] = 0x02;
    memcpy(pInfo->rsn_ie, &dummy_rsne[0], sizeof(dummy_rsne));
    log_print(LOG_HIGH_LEVEL, TAG, "[TEST] verifies 5.2.4 STAUT handles RSNE scenario A. \r\n");
    if(target_ap_rsne_format_chk(pInfo)) {
        log_print(LOG_HIGH_LEVEL, TAG, "    PASS \r\n");
    }
    
    //Scenario B
    memset(&dummy_rsne[0], 0, sizeof(dummy_rsne));
    dummy_rsne[0] = 0x30; dummy_rsne[1] = 0x14; dummy_rsne[2] = 0x01; dummy_rsne[3] = 0x00;
    dummy_rsne[4] = 0x00; dummy_rsne[5] = 0x0F; dummy_rsne[6] = 0xAC; dummy_rsne[7] = 0x04;
    dummy_rsne[8] = 0x01; dummy_rsne[9] = 0x00;
    dummy_rsne[10] = 0x00; dummy_rsne[11] = 0x0F; dummy_rsne[12] = 0xAC; dummy_rsne[13] = 0x04;
    dummy_rsne[14] = 0x01; dummy_rsne[15] = 0x00;
    dummy_rsne[16] = 0x00; dummy_rsne[17] = 0x0F; dummy_rsne[18] = 0xAC; dummy_rsne[19] = 0x04;
    dummy_rsne[20] = 0x80; dummy_rsne[21] = 0x00;
    memcpy(pInfo->rsn_ie, &dummy_rsne[0], sizeof(dummy_rsne));
    log_print(LOG_HIGH_LEVEL, TAG, "[TEST] verifies 5.2.4 STAUT handles RSNE scenario B. \r\n");
    if(target_ap_rsne_format_chk(pInfo)) {
        log_print(LOG_HIGH_LEVEL, TAG, "    PASS \r\n");
    }
    
    //Scenario C
    memset(&dummy_rsne[0], 0, sizeof(dummy_rsne));
    dummy_rsne[0] = 0x30; dummy_rsne[1] = 0x16; dummy_rsne[2] = 0x01; dummy_rsne[3] = 0x00;
    dummy_rsne[4] = 0x00; dummy_rsne[5] = 0x0F; dummy_rsne[6] = 0xAC; dummy_rsne[7] = 0x04;
    dummy_rsne[8] = 0x01; dummy_rsne[9] = 0x00;
    dummy_rsne[10] = 0x00; dummy_rsne[11] = 0x0F; dummy_rsne[12] = 0xAC; dummy_rsne[13] = 0x04;
    dummy_rsne[14] = 0x01; dummy_rsne[15] = 0x00;
    dummy_rsne[16] = 0x00; dummy_rsne[17] = 0x0F; dummy_rsne[18] = 0xAC; dummy_rsne[19] = 0x02;
    dummy_rsne[20] = 0x80; dummy_rsne[21] = 0x00;
    memcpy(pInfo->rsn_ie, &dummy_rsne[0], sizeof(dummy_rsne));
    log_print(LOG_HIGH_LEVEL, TAG, "[TEST] verifies 5.2.4 STAUT handles RSNE scenario C. \r\n");
    if(target_ap_rsne_format_chk(pInfo)) {
        log_print(LOG_HIGH_LEVEL, TAG, "    PASS \r\n");
    }
    
    //Scenario D
    memset(&dummy_rsne[0], 0, sizeof(dummy_rsne));
    dummy_rsne[0] = 0x30; dummy_rsne[1] = 0x1A; dummy_rsne[2] = 0x01; dummy_rsne[3] = 0x00;
    dummy_rsne[4] = 0x00; dummy_rsne[5] = 0x0F; dummy_rsne[6] = 0xAC; dummy_rsne[7] = 0x04;
    dummy_rsne[8] = 0x01; dummy_rsne[9] = 0x00;
    dummy_rsne[10] = 0x00; dummy_rsne[11] = 0x0F; dummy_rsne[12] = 0xAC; dummy_rsne[13] = 0x04;
    dummy_rsne[14] = 0x01; dummy_rsne[15] = 0x00;
    dummy_rsne[16] = 0x00; dummy_rsne[17] = 0x0F; dummy_rsne[18] = 0xAC; dummy_rsne[19] = 0x02;
    dummy_rsne[20] = 0x80; dummy_rsne[21] = 0x00;
    dummy_rsne[22] = 0x00; dummy_rsne[23] = 0x00;
    dummy_rsne[24] = 0x00; dummy_rsne[25] = 0x0F; dummy_rsne[26] = 0xAC; dummy_rsne[27] = 0x06;
    memcpy(pInfo->rsn_ie, &dummy_rsne[0], sizeof(dummy_rsne));
    log_print(LOG_HIGH_LEVEL, TAG, "[TEST] verifies 5.2.4 STAUT handles RSNE scenario D. \r\n");
    if(target_ap_rsne_format_chk(pInfo)) {
        log_print(LOG_HIGH_LEVEL, TAG, "    PASS \r\n");
    }
    
    //Scenario E
    memset(&dummy_rsne[0], 0, sizeof(dummy_rsne));
    dummy_rsne[0] = 0x30; dummy_rsne[1] = 0x1F; dummy_rsne[2] = 0x01; dummy_rsne[3] = 0x00;
    dummy_rsne[4] = 0x00; dummy_rsne[5] = 0x0F; dummy_rsne[6] = 0xAC; dummy_rsne[7] = 0x04;
    dummy_rsne[8] = 0x01; dummy_rsne[9] = 0x00;
    dummy_rsne[10] = 0x00; dummy_rsne[11] = 0x0F; dummy_rsne[12] = 0xAC; dummy_rsne[13] = 0x04;
    dummy_rsne[14] = 0x01; dummy_rsne[15] = 0x00;
    dummy_rsne[16] = 0x00; dummy_rsne[17] = 0x0F; dummy_rsne[18] = 0xAC; dummy_rsne[19] = 0x02;
    dummy_rsne[20] = 0x80; dummy_rsne[21] = 0x00;
    dummy_rsne[22] = 0x00; dummy_rsne[23] = 0x00;
    dummy_rsne[24] = 0x00; dummy_rsne[25] = 0x0F; dummy_rsne[26] = 0xAC; dummy_rsne[27] = 0x06;
    dummy_rsne[28] = 0x04; dummy_rsne[29] = 0x00; dummy_rsne[30] = 0x01; dummy_rsne[31] = 0x02;
    dummy_rsne[32] = 0x03;
    memcpy(pInfo->rsn_ie, &dummy_rsne[0], sizeof(dummy_rsne));
    log_print(LOG_HIGH_LEVEL, TAG, "[TEST] verifies 5.2.4 STAUT handles RSNE scenario E. \r\n");
    if(target_ap_rsne_format_chk(pInfo)) {
        log_print(LOG_HIGH_LEVEL, TAG, "    PASS \r\n");
    }
    
    //Scenario F
    memset(&dummy_rsne[0], 0, sizeof(dummy_rsne));
    dummy_rsne[0] = 0x30; dummy_rsne[1] = 0x2A; dummy_rsne[2] = 0x01; dummy_rsne[3] = 0x00;
    dummy_rsne[4] = 0x00; dummy_rsne[5] = 0x0F; dummy_rsne[6] = 0xAC; dummy_rsne[7] = 0x04;
    dummy_rsne[8] = 0x01; dummy_rsne[9] = 0x00;
    dummy_rsne[10] = 0x00; dummy_rsne[11] = 0x0F; dummy_rsne[12] = 0xAC; dummy_rsne[13] = 0x04;
    dummy_rsne[14] = 0x01; dummy_rsne[15] = 0x00;
    dummy_rsne[16] = 0x00; dummy_rsne[17] = 0x0F; dummy_rsne[18] = 0xAC; dummy_rsne[19] = 0x02;
    dummy_rsne[20] = 0x80; dummy_rsne[21] = 0x00;
    dummy_rsne[22] = 0x01; dummy_rsne[23] = 0x00;
    dummy_rsne[24] = 0x01; dummy_rsne[25] = 0x02; dummy_rsne[26] = 0x03; dummy_rsne[27] = 0x04;
    dummy_rsne[28] = 0x05; dummy_rsne[29] = 0x06; dummy_rsne[30] = 0x07; dummy_rsne[31] = 0x08;
    dummy_rsne[32] = 0x09; dummy_rsne[33] = 0x0A; dummy_rsne[34] = 0x0B; dummy_rsne[35] = 0x0C;
    dummy_rsne[36] = 0x0D; dummy_rsne[37] = 0x0E; dummy_rsne[38] = 0x0F; dummy_rsne[39] = 0x10;
    dummy_rsne[40] = 0x00; dummy_rsne[41] = 0xCF; dummy_rsne[42] = 0xAC; dummy_rsne[43] = 0x06;
    memcpy(pInfo->rsn_ie, &dummy_rsne[0], sizeof(dummy_rsne));
    log_print(LOG_HIGH_LEVEL, TAG, "[TEST] verifies 5.2.4 STAUT handles RSNE scenario F. \r\n");
    if(!target_ap_rsne_format_chk(pInfo)) {
        log_print(LOG_HIGH_LEVEL, TAG, "    PASS \r\n");
    }
    
    return false;
}
#endif

Boolean wpa_driver_netlink_connect_impl(struct wpa_config * conf)
{
    int ret = 0;
    scan_info_t *pInfo = NULL;
    int i;
    uint32_t u32Remain = 0; //I0051 0000908
    hap_control_t *hap_temp;    //I0064  0000318
    hap_temp=get_hap_control_struct();  //I0064  0000318
    
    if (conf == NULL) return FALSE;
    if (conf->ssid == NULL) return FALSE;
    
    //I0060 0001020_add_beg
    if (get_wifi_certified_mode() == WIFI_CERTIFIED) {
        //I0060 0000905_add_beg
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
        //I0060 0000905_add_end
    }
    //I0060 0001020_add_end
    
    if (wifi_ctrl_state_get() == WIFI_CTRL_SCANNING_ST ||
        wifi_ctrl_state_get() == WIFI_CTRL_CONNECTING_ST ||
        wifi_ctrl_state_get() == WIFI_CTRL_CONNECTED_SCAN_ST) {
        log_print(LOG_MED_LEVEL, TAG, "WPA: Ignore this command since now is processing\r\n");
        ret = -1;
        goto done;
    }
    
    if (wifi_ctrl_state_get() == WIFI_CTRL_CONNECTED_ST) {
        if (!connection_connected_ctrl()) {
            ret = -1;
            goto done;
        }
    }
    
    //I0060 0000958_add_beg
#ifdef __WIFI_AUTO_CONNECT__
    //For compatible auto/manual connect
    if (get_auto_connect_mode() == AUTO_CONNECT_ENABLE) {
        set_auto_connect_mode(AUTO_CONNECT_MANUAL);
    }
#endif
    //I0060 0000958_add_end
    
    if (conf->ssid->ssid == NULL) {
        log_print(LOG_LOW_LEVEL, TAG, "WPA: wpa_driver_netlink_connect, bssid:%x %x %x %x %x %x \r\n",
            conf->ssid->bssid[0], conf->ssid->bssid[1], conf->ssid->bssid[2],
            conf->ssid->bssid[3], conf->ssid->bssid[4], conf->ssid->bssid[5]);
        
        //I0051 0000908_add_begin
        if(get_wifi_certified_mode() == WIFI_CERTIFIED)
        {
            if(wpa_supplicant_is_bssid_blocked_by_mic_error(conf->ssid->bssid, &u32Remain))
            {
                log_print(LOG_HIGH_LEVEL, TAG, "WPA: BSSID is blocked by MIC failure (%u seconds remaining)\r\n", u32Remain);
                ret = -1;
                goto done;
            }
        }
        //I0051 0000908_add_end

        wpa_supplicant_set_state(wpa_s, WPA_ASSOCIATING);

        ret = wifi_sta_join(conf->ssid->bssid);
    } 
    else {
        pInfo = wifi_get_scan_record_by_ssid((char*)(conf->ssid->ssid));
        if (pInfo == NULL) {
            //I0064  0000318_add_begin
            if (hap_temp->hap_final_index!=0){
                hap_temp->hap_en=1;
                //I0060 0000635 hap_temp->hap_ap_info = malloc(sizeof(auto_conn_info_t));
                //I0060 0000635 wifi_sta_join_for_hiddenap();
                //I0060 0000635 return TRUE;
            }
            else{
                ret = -1; //I0060 0000958
                goto done; // not found target AP in the list
            }
            //I0064  0000318_add_end
        }

        //I0051 0000908_add_begin
        if(get_wifi_certified_mode() == WIFI_CERTIFIED)
        {
            if(wpa_supplicant_is_bssid_blocked_by_mic_error(pInfo->bssid, &u32Remain))
            {
                log_print(LOG_HIGH_LEVEL, TAG, "WPA: BSSID is blocked by MIC failure (%u seconds remaining)\r\n", u32Remain);
                ret = -1;
                goto done;
            }
        }
        //I0051 0000908_add_end

        for (i=0; i<ETH_ALEN; i++) {
            g_bssid[i] = pInfo->bssid[i];
        }
        
        if (hap_temp->hap_en) {
            hap_temp->hap_ap_info = malloc(sizeof(auto_conn_info_t));
            wifi_sta_join_for_hiddenap();
            goto done;
        }
        
        wpa_supplicant_set_state(wpa_s, WPA_ASSOCIATING);
        g_fastconn = 0;     //I0064  0000736
        g_wifi_reconnection_counter = 0;    //I0064  0000736
        ret = wifi_sta_join(pInfo->bssid);
    }

//I0060 0000635
done:
    if(ret == 0) return TRUE;
    return FALSE;
}


Boolean wpa_driver_netlink_connect_by_bssid_impl(u8 *bssid)
{
    int ret = 0;
	if (bssid == NULL) return FALSE;
    ret = wifi_sta_join(bssid);
    wpa_supplicant_set_state(wpa_s, WPA_ASSOCIATING);
    if(ret == 0) return TRUE;
    return FALSE;
}

Boolean wpa_driver_netlink_connect_from_ac_impl(u8 index)
{
    if (index > MAX_NUM_OF_AUTO_CONNECT) {
        log_print(LOG_HIGH_LEVEL, TAG, "WPA: Invalid Parameter \r\n");
        return FALSE;
    }
    
    if (wifi_ctrl_state_get() == WIFI_CTRL_SCANNING_ST ||
        wifi_ctrl_state_get() == WIFI_CTRL_CONNECTING_ST ||
        wifi_ctrl_state_get() == WIFI_CTRL_CONNECTED_SCAN_ST) {
        log_print(LOG_MED_LEVEL, TAG, "WPA: Ignore this command since now is processing\r\n");
        return FALSE;
    }
    
    control_auto_connect_by_index(index);
    return TRUE;
}

Boolean wpa_driver_netlink_reconnect_impl(struct wpa_config * conf)
{
    int ret = 0;
	if (conf == NULL) return FALSE;
    if (conf->ssid == NULL) return FALSE;
    log_print(LOG_MED_LEVEL, TAG, "WPA: wpa_driver_netlink_reconnect, bssid:%x %x %x %x %x %x \r\n",
        conf->ssid->bssid[0], conf->ssid->bssid[1], conf->ssid->bssid[2],
        conf->ssid->bssid[3], conf->ssid->bssid[4], conf->ssid->bssid[5]);
    ret = wifi_sta_join(conf->ssid->bssid);
    if(ret == 0) return TRUE;
    return FALSE;
}


Boolean wpa_driver_netlink_associate_impl(void *priv, struct wpa_driver_associate_params *params)
{
	return TRUE;
}


void wpa_driver_netlink_disconnect_impl(const u8 *bssid, u16 reason_code)
{
    g_wifi_reconnection_counter = MAX_WIFI_RECONNECTION;
    //I0051 0000908 wifi_sta_leave();
    wifi_sta_leave(reason_code); //I0051 0000908
}


Boolean wpa_driver_netlink_deauthenticate_impl(void *priv, const u8 *addr, int reason_code)
{
    //I0051 0000908 wifi_sta_leave();
    wifi_sta_leave((u16)reason_code); //I0051 0000908
	return TRUE;
}


Boolean wpa_driver_netlink_get_bssid_impl(u8 *bssid)
{
    u8 buf[6];

    if (bssid == NULL) return FALSE;

    os_memset(buf, 0, sizeof(buf));
	if (bssid != NULL) {
        wifi_get_bssid(buf);
        os_memcpy(bssid, buf, sizeof(buf));
        return TRUE;
    }

	return FALSE;
}


Boolean wpa_driver_netlink_get_ssid_impl(u8 *ssid)
{
    if (ssid == NULL) return FALSE;
    
    os_memcpy(ssid, wifi_get_ssid(), MAX_LEN_OF_SSID + 1);
    
    return TRUE;
}


Boolean wpa_driver_netlink_get_mac_impl(u8 *mac)
{
	if (mac == NULL) return FALSE;
    wifi_get_mac_addr(mac);
	return TRUE;
}

void wpa_driver_netlink_set_mac_impl(u8 *mac)
{
	if (mac == NULL) return;
    wifi_set_mac_addr(mac);
}

int wpa_driver_netlink_get_rssi_impl(void)
{
    int rssi = wifi_get_rssi();
    return rssi;
}

int wpa_driver_netlink_set_key_impl(const char *ifname, void *priv,
				     enum wpa_alg alg, const u8 *addr,
				     int key_idx, int set_tx,
				     const u8 *seq, size_t seq_len,
				     const u8 *key, size_t key_len)
{
    //Not Support Yet in link layer
	return 0;
}


void wpa_driver_netlink_scan_results_free_impl(struct wpa_scan_results *res)
{
    //trigger clean

}


void wpa_driver_netlink_scan_results_clear_impl(struct wpa_scan_results *res)
{
    if((res == NULL) || (res->res == NULL))
    {
        goto done;
    }

    os_free(res->res);
    memset(res, 0, sizeof(struct wpa_scan_results));

done:
    return;
}

int wpa_drv_send_eapol_impl(const u8 *dst, u16 proto, u8 *data, size_t data_len)
{
    log_print(LOG_LOW_LEVEL, TAG, "[KEY]WPA: ready to send eapol key frame, proto:%d data_len:%d \r\n", proto, data_len);
    //call data path api to send it out
    wifi_mac_xmit_eapol_frame(data, data_len, dst, proto);

    return 1;
}

Boolean wpa_driver_netlink_fast_connect_impl(u8 mode, u8 index)
{
    mw_wifi_auto_connect_ap_info_t info;

    if (mode > AUTO_CONNECT_ENABLE || index >= MAX_NUM_OF_AUTO_CONNECT) {
        log_print(LOG_HIGH_LEVEL, TAG, "WPA: invalid parameter \r\n");
        return FALSE;
    }
    
    wifi_nvm_auto_connect_read(WIFI_NVM_ID_AUTO_CONNECT_AP_INFO, sizeof(mw_wifi_auto_connect_ap_info_t), index, &info);

    if(info.bssid[0] == 0 && info.bssid[1] == 0) {
        log_print(LOG_HIGH_LEVEL, TAG, "WPA: the index of AP is empty \r\n");
        return FALSE;
    }

    set_fast_connect_mode(index, mode);
    info.fast_connect = mode;
    wifi_nvm_auto_connect_write(WIFI_NVM_ID_AUTO_CONNECT_AP_INFO, sizeof(mw_wifi_auto_connect_ap_info_t), index, &info);

    return TRUE;
}

Boolean wpa_driver_netlink_sta_cfg_impl(u8 mode, u8 cmd_idx, u8 *value)
{
    if (mode != MLME_CMD_SET_PARAM && mode != MLME_CMD_GET_PARAM) {
        log_print(LOG_HIGH_LEVEL, TAG, "WPA: Invalid Parameter \r\n");
        return FALSE;
    }

    if (value == NULL) {
        log_print(LOG_HIGH_LEVEL, TAG, "WPA: Invalid Parameter \r\n");
        return FALSE;
    }

    switch (mode) {
        case MLME_CMD_GET_PARAM:
            wifi_get_sta_cfg_from_share_memory(cmd_idx, value);
            break;
        case MLME_CMD_SET_PARAM:
            wifi_set_sta_cfg_req(cmd_idx, value);
            break;
        default:
            break;
    }
    
    return TRUE;
}

//I0060 0000665_add_beg
int wpa_driver_netlink_get_state_impl(void)
{
    if (wpa_s == NULL) return 0;
    return ((int)wpa_s->wpa_state);
}
//I0060 0000665_add_end

Boolean wpa_driver_netlink_scan_by_cfg_impl(void *cfg)
{
    if (cfg == NULL) return FALSE;

    if (wifi_ctrl_state_get() == WIFI_CTRL_SCANNING_ST ||
        wifi_ctrl_state_get() == WIFI_CTRL_CONNECTING_ST ||
        wifi_ctrl_state_get() == WIFI_CTRL_CONNECTED_SCAN_ST) {
        log_print(LOG_MED_LEVEL, TAG, "WPA: Ignore this command since now is processing\r\n");
        return FALSE;
    }
    else {
        if (wifi_ctrl_state_get() != WIFI_CTRL_CONNECTED_ST) {
            wpa_supplicant_set_state(wpa_s, WPA_SCANNING);
            wifi_ctrl_state_set(WIFI_CTRL_SCANNING_ST);
        }
        wifi_scan_req_by_cfg(cfg);
    }
    
    return TRUE;
}

//I0060 0000930_add_beg
int wpa_driver_netlink_is_connected_impl(void)
{
    if (wpa_driver_netlink_get_state() == WPA_ASSOCIATED ||
        wpa_driver_netlink_get_state() == WPA_COMPLETED) {
        return TRUE;
    }
    return FALSE;
}
//I0060 0000930_add_end

int wpa_driver_netlink_get_log_level_impl(void)
{
    return wpa_debug_level;
}

int wpa_driver_netlink_set_log_level_impl(int level)
{
    wpa_debug_level = level;
    return TRUE;
}

//I0064 0000907_add_beg
int wpa_driver_netlink_set_encrypt_mode_impl(int mode)
{ 
    return wifi_set_encrypt_mode_impl(mode);
}
//I0064 0000907_add_end

RET_DATA wpa_driver_netlink_init_fp_t wpa_driver_netlink_init;
RET_DATA wpa_driver_netlink_deinit_fp_t wpa_driver_netlink_deinit ;
RET_DATA wpa_driver_netlink_scan_fp_t wpa_driver_netlink_scan;
RET_DATA wpa_driver_netlink_scan_stop_fp_t wpa_driver_netlink_scan_stop;
RET_DATA wpa_driver_netlink_get_scan_results_fp_t wpa_driver_netlink_get_scan_results;
RET_DATA wpa_driver_netlink_connect_fp_t wpa_driver_netlink_connect;
RET_DATA wpa_driver_netlink_connect_from_ac_fp_t wpa_driver_netlink_connect_from_ac;
RET_DATA wpa_driver_netlink_reconnect_fp_t wpa_driver_netlink_reconnect;
RET_DATA wpa_driver_netlink_associate_fp_t wpa_driver_netlink_associate;
RET_DATA wpa_driver_netlink_disconnect_fp_t wpa_driver_netlink_disconnect;
RET_DATA wpa_driver_netlink_deauthenticate_fp_t wpa_driver_netlink_deauthenticate;
RET_DATA wpa_driver_netlink_get_bssid_fp_t wpa_driver_netlink_get_bssid;
RET_DATA wpa_driver_netlink_get_ssid_fp_t wpa_driver_netlink_get_ssid;
RET_DATA wpa_driver_netlink_get_mac_fp_t wpa_driver_netlink_get_mac;
RET_DATA wpa_driver_netlink_set_key_fp_t wpa_driver_netlink_set_key;
RET_DATA wpa_driver_netlink_scan_results_free_fp_t wpa_driver_netlink_scan_results_free;
RET_DATA wpa_drv_send_eapol_fp_t wpa_drv_send_eapol;
RET_DATA wpa_driver_netlink_connect_by_bssid_fp_t wpa_driver_netlink_connect_by_bssid;
RET_DATA wpa_driver_netlink_set_mac_fp_t wpa_driver_netlink_set_mac;
RET_DATA wpa_driver_netlink_get_rssi_fp_t wpa_driver_netlink_get_rssi;
RET_DATA wpa_driver_netlink_show_scan_results_fp_t wpa_driver_netlink_show_scan_results;
RET_DATA wpa_driver_netlink_scan_results_free_fp_t wpa_driver_netlink_scan_results_clear;
RET_DATA wpa_driver_netlink_fast_connect_fp_t wpa_driver_netlink_fast_connect;
RET_DATA wpa_driver_netlink_sta_cfg_fp_t wpa_driver_netlink_sta_cfg;
RET_DATA wpa_driver_netlink_get_state_fp_t wpa_driver_netlink_get_state; //I0060 0000665
RET_DATA wpa_driver_netlink_scan_by_cfg_fp_t wpa_driver_netlink_scan_by_cfg;
RET_DATA wpa_driver_netlink_is_connected_fp_t wpa_driver_netlink_is_connected;
RET_DATA wpa_driver_netlink_wpa_cipher_txt_fp_t wpa_driver_netlink_wpa_cipher_txt;
RET_DATA wpa_driver_netlink_wpa_ie_txt_fp_t wpa_driver_netlink_wpa_ie_txt;
RET_DATA wpa_driver_netlink_get_log_level_fp_t wpa_driver_netlink_get_log_level;
RET_DATA wpa_driver_netlink_set_log_level_fp_t wpa_driver_netlink_set_log_level;
RET_DATA get_target_ap_record_fp_t get_target_ap_record;
RET_DATA target_ap_security_mode_chk_fp_t target_ap_security_mode_chk;
RET_DATA target_ap_rsne_format_chk_fp_t target_ap_rsne_format_chk;
RET_DATA connection_connected_ctrl_fp_t connection_connected_ctrl;
RET_DATA get_wifi_certified_mode_fp_t get_wifi_certified_mode;
RET_DATA set_wifi_certified_mode_fp_t set_wifi_certified_mode;
RET_DATA wpa_driver_netlink_set_encrypt_mode_fp_t wpa_driver_netlink_set_encrypt_mode;  //I0064 0000907

/*
   Interface Initialization: WPA Driver
 */
void wpa_driver_func_init(void)
{
    wpa_driver_netlink_init                = wpa_driver_netlink_init_impl;
    wpa_driver_netlink_deinit              = wpa_driver_netlink_deinit_impl;
    wpa_driver_netlink_scan                = wpa_driver_netlink_scan_impl;
    wpa_driver_netlink_scan_stop           = wpa_driver_netlink_scan_stop_impl;
    wpa_driver_netlink_get_scan_results    = wpa_driver_netlink_get_scan_results_impl;
    wpa_driver_netlink_connect             = wpa_driver_netlink_connect_impl;
    wpa_driver_netlink_connect_from_ac     = wpa_driver_netlink_connect_from_ac_impl;
    wpa_driver_netlink_reconnect           = wpa_driver_netlink_reconnect_impl;
    wpa_driver_netlink_associate           = wpa_driver_netlink_associate_impl;
    wpa_driver_netlink_disconnect          = wpa_driver_netlink_disconnect_impl;
    wpa_driver_netlink_deauthenticate      = wpa_driver_netlink_deauthenticate_impl;
    wpa_driver_netlink_get_bssid           = wpa_driver_netlink_get_bssid_impl;
    wpa_driver_netlink_get_ssid            = wpa_driver_netlink_get_ssid_impl;
    wpa_driver_netlink_get_mac             = wpa_driver_netlink_get_mac_impl;
    wpa_driver_netlink_set_key             = wpa_driver_netlink_set_key_impl;
    wpa_driver_netlink_scan_results_free   = wpa_driver_netlink_scan_results_free_impl;
    wpa_drv_send_eapol                     = wpa_drv_send_eapol_impl;
    wpa_driver_netlink_connect_by_bssid    = wpa_driver_netlink_connect_by_bssid_impl;
    wpa_driver_netlink_set_mac             = wpa_driver_netlink_set_mac_impl;
    wpa_driver_netlink_get_rssi            = wpa_driver_netlink_get_rssi_impl;
    wpa_driver_netlink_show_scan_results   = wpa_driver_netlink_show_scan_results_impl;
    wpa_driver_netlink_scan_results_clear  = wpa_driver_netlink_scan_results_clear_impl;
    wpa_driver_netlink_fast_connect        = wpa_driver_netlink_fast_connect_impl;
    wpa_driver_netlink_sta_cfg             = wpa_driver_netlink_sta_cfg_impl;
    wpa_driver_netlink_get_state           = wpa_driver_netlink_get_state_impl; //I0060 0000665
    wpa_driver_netlink_scan_by_cfg         = wpa_driver_netlink_scan_by_cfg_impl;
    wpa_driver_netlink_is_connected        = wpa_driver_netlink_is_connected_impl;
    wpa_driver_netlink_wpa_cipher_txt      = wpa_driver_netlink_wpa_cipher_txt_impl;
    wpa_driver_netlink_wpa_ie_txt          = wpa_driver_netlink_wpa_ie_txt_impl;
    get_target_ap_record                   = get_target_ap_record_impl;
    wpa_driver_netlink_get_log_level       = wpa_driver_netlink_get_log_level_impl;
    wpa_driver_netlink_set_log_level       = wpa_driver_netlink_set_log_level_impl;
    target_ap_security_mode_chk            = target_ap_security_mode_chk_impl;
    target_ap_rsne_format_chk              = target_ap_rsne_format_chk_impl;
    connection_connected_ctrl              = connection_connected_ctrl_impl;
    get_wifi_certified_mode                = get_wifi_certified_mode_impl;
    set_wifi_certified_mode                = set_wifi_certified_mode_impl;
    wpa_driver_netlink_set_encrypt_mode    = wpa_driver_netlink_set_encrypt_mode_impl;  //I0064 0000907
}
