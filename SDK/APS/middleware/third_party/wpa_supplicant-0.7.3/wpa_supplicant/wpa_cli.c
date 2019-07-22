/*
 * WPA Supplicant - command line interface for wpa_supplicant daemon
 * Copyright (c) 2004-2010, Jouni Malinen <j@w1.fi>
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "driver_netlink.h"
#include "msg.h"
#include "scrt.h"
#include "sys_common_ctrl.h" //I0060 0000710
#include "wpa.h"
#include "wpa_cli.h"
#include "wpa_debug.h"
#include "wifi_nvm.h"
#include "wpa_supplicant_i.h"


RET_DATA u8 g_bssid[6];// = {0};
RET_DATA struct wpa_scan_results res;
RET_DATA struct wpa_config conf;
RET_DATA struct wpa_config rec_conf;
RET_DATA struct wpa_ssid ssid_data;
RET_DATA struct wpa_ssid rec_ssid_data;

extern struct wpa_supplicant *wpa_s;
extern char g_passphrase[MAX_LEN_OF_PASSWD];
extern auto_connect_cfg_t g_AutoConnect;
extern u8 gAutoConnMode;
extern u8 gsta_cfg_mac[MAC_ADDR_LEN];

Boolean isMAC(char *s) {
    int i;
    for(i = 0; i < 17; i++) {
        if(i % 3 != 2 && !isxdigit(s[i]))
            return FALSE;
        if(i % 3 == 2 && s[i] != ':')
            return FALSE;
    }
    if(s[17] != '\0')
        return FALSE;
    return TRUE;
}

//The parameters are designed only for open system connection, not include the security connection
int wpa_cli_connect_impl(struct wpa_config * conf)
{
    u8 ret = FALSE;
    if (conf == NULL) return FALSE;
    if (conf->ssid == NULL) return FALSE;
    ret = wpa_driver_netlink_connect(conf);
    if (ret == FALSE) return FALSE;
    return TRUE;
}

int wpa_cli_connect_by_bssid_impl(u8 *bssid)
{
    if (bssid == NULL) return FALSE;
    wpa_driver_netlink_connect_by_bssid(bssid);
    return TRUE;
}

int wpa_cli_connect_handler_impl(int argc, char *argv[])
{
    u8 bssid[6] = {0};
    int i;
    char passwd[MAX_LEN_OF_PASSWD] = {0};
    int len_passwd = 0;
    int len_ssid = 0;
    u8 ret = FALSE;
    
    if(argc <= 1) {
        wpa_printf(MSG_ERROR, "[CLI]WPA: invalid parameter \r\n");
        return FALSE;
    }

    memset(&conf, 0, sizeof(conf));
    memset(&ssid_data, 0, sizeof(ssid_data));
    conf.ssid=&ssid_data;

    memset(g_passphrase, 0, MAX_LEN_OF_PASSWD);
    memset(bssid, 0, sizeof(bssid)/sizeof(bssid[0]));
    if(conf.ssid == NULL){
        ret=FALSE;
        goto DONE;
    }

    if(isMAC(argv[1])) //wpa_connect "bssid" "passphase"
    {
        //bssid
        hwaddr_aton2(argv[1], bssid);

        for (i=0; i<ETH_ALEN; i++)
        {
            conf.ssid->bssid[i] = bssid[i];
            g_bssid[i] = bssid[i];
        }

        wpa_printf(MSG_DEBUG, "[CLI]WPA: connect bssid=%02x:%02x:%02x:%02x:%02x:%02x \r\n",
                                  bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]);

        //passwd
        if(argc >= 3)
        {
            len_passwd = os_strlen(argv[2]);

            if ((len_passwd >= MAX_LEN_OF_PASSWD) || (len_passwd < MIN_LEN_OF_PASSWD))
            {
                wpa_printf(MSG_ERROR, "[CLI]WPA: invalid parameter \r\n");
                ret=FALSE;
                goto DONE;
            }

            memset(passwd, 0, MAX_LEN_OF_PASSWD);
            os_memcpy(passwd, argv[2], len_passwd);

            if(len_passwd > 0)
            {
                os_memcpy((void *)g_passphrase, (void *)passwd, MAX_LEN_OF_PASSWD);
            }
        }

    }
    else //wpa_connect "ssid" "passphase" "bssid of hidden AP"
    {
        //ssid
        len_ssid = os_strlen(argv[1]);
        if(len_ssid > MAX_LEN_OF_SSID)
        {
            len_ssid = MAX_LEN_OF_SSID;
        }
        conf.ssid->ssid = os_malloc(MAX_LEN_OF_SSID + 1);
        memset(conf.ssid->ssid, 0, (MAX_LEN_OF_SSID + 1));
        os_memcpy(conf.ssid->ssid, argv[1], len_ssid);

        wpa_printf(MSG_DEBUG, "[CLI]WPA: connect ssid=");
        for(i=0;i<len_ssid;i++)
        {
            wpa_printf(MSG_DEBUG, "%c", conf.ssid->ssid[i]);
        }
        wpa_printf(MSG_DEBUG, "\r\n");

        //passwd
        if(argc >= 3)
        {
            len_passwd = os_strlen(argv[2]);

            if ((len_passwd >= MAX_LEN_OF_PASSWD) || (len_passwd < MIN_LEN_OF_PASSWD))
            {
                wpa_printf(MSG_ERROR, "[CLI]WPA: invalid parameter \r\n");
                ret=FALSE;
                goto DONE;
            }

            memset(passwd, 0, MAX_LEN_OF_PASSWD);
            os_memcpy(passwd, argv[2], len_passwd);

            if(len_passwd > 0)
            {
                os_memcpy((void *)g_passphrase, (void *)passwd, MAX_LEN_OF_PASSWD);
            }
        }

        //bssid for hidden ap connect
        if(argc >= 4)
        {
            if(isMAC(argv[3]))
            {
                hwaddr_aton2(argv[3], bssid);

                for (i=0; i<ETH_ALEN; i++)
                {
                    conf.ssid->bssid[i] = bssid[i];
                    g_bssid[i] = bssid[i];
                }
            }
        }
    }

#if 0 //I0060 0000958_del_beg
    //For compatible auto/manual connect
    if (get_auto_connect_mode() == AUTO_CONNECT_ENABLE) {
        set_auto_connect_mode(AUTO_CONNECT_MANUAL);
    }
#endif //I0060 0000958_del_end

    ret = wpa_cli_connect(&conf);
    //if (ret == FALSE) return FALSE;   I0064 0000767

DONE:
    if (conf.ssid->ssid) {
        os_free(conf.ssid->ssid);
        conf.ssid->ssid = NULL;
    }
    
    return (ret)?TRUE:FALSE;
}

int wpa_cli_disconnect_impl(const u8 *bssid, u16 reason_code)
{
    wpa_driver_netlink_disconnect(bssid, 0);
    return TRUE;
}

int wpa_cli_disconnect_handler_impl(int argc, char *argv[])
{
    wpa_cli_disconnect(NULL, 0);
    return TRUE;
}

int wpa_cli_reconnect_impl(struct wpa_config * conf)
{
    if (conf == NULL) return FALSE;
    if (conf->ssid == NULL) return FALSE;
    wpa_driver_netlink_reconnect(conf);
    return TRUE;
}

int wpa_cli_reconnect_handler_impl(int argc, char *argv[])
{
    int i;

    memset(&rec_conf, 0, sizeof(rec_conf));
    memset(&rec_ssid_data, 0, sizeof(rec_ssid_data));

    rec_conf.ssid=&rec_ssid_data;
    if(rec_conf.ssid == NULL) return FALSE;

    for (i=0; i<ETH_ALEN; i++) {
        rec_conf.ssid->bssid[i] = g_bssid[i]; //conf.ssid->bssid => xxxxxxxxxxxx
    }
    wpa_printf(MSG_DEBUG, "[CLI]WPA: reconnect bssid=%02x:%02x:%02x:%02x:%02x:%02x \r\n",
                                rec_conf.ssid->bssid[0],
                                rec_conf.ssid->bssid[1],
                                rec_conf.ssid->bssid[2],
                                rec_conf.ssid->bssid[3],
                                rec_conf.ssid->bssid[4],
                                rec_conf.ssid->bssid[5]);
    wpa_cli_reconnect(&rec_conf);

    return 1;
}

int wpa_cli_scan_impl(int mode)
{
    return wpa_driver_netlink_scan(mode);
}

int wpa_cli_scan_handler_impl(int argc, char *argv[])
{
    int mode = SCAN_MODE_MIX;

    if(argc > 1) {
        mode = atoi(argv[1]);
        if (mode < SCAN_MODE_ACTIVE || mode > SCAN_MODE_MIX) {
            wpa_printf(MSG_ERROR, "[CLI]WPA: invalid parameter \r\n");
            return FALSE;
        }
    }

    wpa_printf(MSG_DEBUG, "[CLI]WPA: scan mode=%d \r\n", mode);
    return wpa_cli_scan(mode);
}

int wpa_cli_scan_by_cfg_impl(void *cfg)
{
    if (cfg == NULL) return FALSE;

    S_WIFI_MLME_SCAN_CFG *scan_cfg = (S_WIFI_MLME_SCAN_CFG *)cfg;

    if (scan_cfg->u32ActiveScanDur < SCAN_ACTIVE_MIN_DUR_TIME_DEF || 
        scan_cfg->u32ActiveScanDur > SCAN_MAX_NUM_OF_DUR_TIME) {
        scan_cfg->u32ActiveScanDur = SCAN_ACTIVE_MIN_DUR_TIME_DEF;
    }

    if (scan_cfg->u32PassiveScanDur < SCAN_PASSIVE_MIN_DUR_TIME_DEF ||
        scan_cfg->u32PassiveScanDur > SCAN_MAX_NUM_OF_DUR_TIME) {
        scan_cfg->u32PassiveScanDur = SCAN_PASSIVE_MIN_DUR_TIME_DEF;
    }
    
    if (scan_cfg->u8Channel > WIFI_MLME_SCAN_MAX_NUM_CHANNELS) {
        scan_cfg->u8Channel = WIFI_MLME_SCAN_ALL_CHANNELS;
    }
    
    wpa_driver_netlink_scan_by_cfg(scan_cfg);

    return TRUE;
}

int wpa_cli_scan_stop_handler_impl(int argc, char *argv[])
{
    return wpa_driver_netlink_scan_stop();
}

int wpa_cli_getscanresults_impl(struct wpa_scan_results * res)
{
    if(res == NULL) return FALSE;
    return wpa_driver_netlink_get_scan_results(res);
}

void wpa_cli_showscanresults_handler_impl(int argc, char *argv[])
{
    wpa_driver_netlink_show_scan_results();
}

void wpa_cli_getbssid_impl(u8 *bssid)
{
    if(bssid == NULL) return;
    wpa_driver_netlink_get_bssid(bssid);
}

void wpa_cli_showbssid_by_param_impl(int argc, char *argv[])
{
    u8 bssid[6] = {0};
    memset(bssid, 0, sizeof(bssid));
    wpa_driver_netlink_get_bssid(bssid);
    wpa_printf(MSG_DEBUG, "[CLI]WPA: bssid=%02x:%02x:%02x:%02x:%02x:%02x\r\n",
                              bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]);
}

void wpa_cli_getssid_impl(u8 *ssid)
{
    if(ssid == NULL) return;
    wpa_driver_netlink_get_ssid(ssid);
}

void wpa_cli_showssid_by_param_impl(int argc, char *argv[])
{
    u8 ssid[MAX_LEN_OF_SSID + 1] = {0};
    int i;

    memset(ssid, 0, sizeof(ssid));
    if(wpa_s->wpa_state == WPA_ASSOCIATED || wpa_s->wpa_state == WPA_COMPLETED)
    {
        memset(ssid, 0, sizeof(ssid));
        wpa_cli_getssid(ssid);
        wpa_printf(MSG_DEBUG, "[CLI]WPA: ssid=");
        for(i = 0; i < sizeof(ssid); i++)
        {
            wpa_printf(MSG_DEBUG, "%c", ssid[i]);
        }
        wpa_printf(MSG_DEBUG, "\r\n");
    }
}

void wpa_cli_getmac_impl(u8 *mac)
{
    if(mac == NULL) return;
    wpa_driver_netlink_sta_cfg(MLME_CMD_GET_PARAM, E_WIFI_PARAM_MAC_ADDRESS, mac);
}

//I0060 0000662_add_beg
int check_mac_addr_len_impl(const char *txt)
{
    u8 count = 0;
    const char *pos = txt;

    while (*pos != NULL) {
        while (*pos == ':' || *pos == '.' || *pos == '-')
            pos++;

        *pos++;
        *pos++;
        count++;
    }

    if (count != MAC_ADDR_LEN)
        return -1;

    return 0;
}
//I0060 0000662_add_end

void wpa_cli_setmac_impl(u8 *mac)
{
    if(mac == NULL) return;

    //I0060 0000662_add_beg
    if (is_broadcast_ether_addr(mac)) {
        wpa_printf(MSG_ERROR, "[CLI]WPA: Invalid mac address, all of mac if 0xFF \r\n");
        return;
    }

    if (is_multicast_ether_addr(mac)) {
        wpa_printf(MSG_ERROR, "[CLI]WPA: Invalid mac address, not allow multicast mac address \r\n");
        return;
    }
    
    if (is_zero_ether_addr(mac)) {
        wpa_printf(MSG_ERROR, "[CLI]WPA: Invalid mac address, all of mac is zero. \r\n");
        return;
    }
    //I0060 0000662_add_end
    
#if 0 //I0060 0000662_del_beg
    if ((mac[0] == 0x00 && mac[1] == 0x00) ||
        (mac[0] == 0xFF) || (mac[0] == 0x01)) {
        msg_print(LOG_HIGH_LEVEL, "[CLI]WPA: Invalid mac address \r\n");
        return;
    }
#endif //I0060 0000662_del_end

#if 0 //I0060 0000778_del_beg
    if (wpa_s->wpa_state == WPA_COMPLETED || wpa_s->wpa_state == WPA_ASSOCIATED) {
        msg_print(LOG_HIGH_LEVEL, "[CLI]WPA: Invalid wpa state \r\n");
        return;
    }
#endif //I0060 0000778_del_end

    memset(&gsta_cfg_mac[0], 0, MAC_ADDR_LEN);
    memcpy(&gsta_cfg_mac[0], &mac[0], MAC_ADDR_LEN);

    wifi_nvm_sta_info_write(WIFI_NVM_STA_INFO_ID_MAC_ADDR, MAC_ADDR_LEN, gsta_cfg_mac);
}

void wpa_cli_mac_by_param_impl(int argc, char *argv[])
{
    u8 mac[6] = {0};

    if(argc == 1) //show mac
    {
        memset(mac, 0, sizeof(mac)/sizeof(mac[0]));
        wpa_cli_getmac(mac);
        tracer_cli(LOG_HIGH_LEVEL, "[CLI]Wifi STA mac address: %02x:%02x:%02x:%02x:%02x:%02x \r\n",
                                  mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    }
    else if (argc == 2) //set mac
    {
        memset(mac, 0, sizeof(mac)/sizeof(mac[0]));
        //I0060 0000662 hwaddr_aton2(argv[1], mac);
        
        //I0060 0000662_add_beg
        if (check_mac_addr_len(argv[1]) == -1) {
            wpa_printf(MSG_ERROR, "[CLI]Invalid mac address, wrong length of mac address \r\n");
            return;
        }
        
        if (hwaddr_aton2(argv[1], mac) == -1) {
            wpa_printf(MSG_ERROR, "[CLI]Invalid mac address \r\n");
            return;
        }
        //I0060 0000662_add_end
        
        wpa_cli_setmac(mac);
    }
}

int wpa_cli_getrssi_impl(void)
{
    int rssi = wpa_driver_netlink_get_rssi();
    wpa_printf(MSG_INFO, "[CLI]WPA: rssi=%d \r\n", rssi);

    return rssi;
}

extern void itp_scrt_test(void);
extern void aes_ccm_sample(void);
extern void aes_ecb_sample(void);
extern void hmac_sha_1_sample(void);

void wpa_cli_scrt_dbg_by_param_impl(int argc, char *argv[])
{
    itp_scrt_test();
    aes_ccm_sample();
    aes_ecb_sample();
    hmac_sha_1_sample();
}

void debug_auto_connect_impl(void)
{
    int i;
    MwFimAutoConnectCFG_t cfg;
    auto_conn_info_t info;
    
    tracer_cli(LOG_HIGH_LEVEL, "AP mode = %d\r\n", get_auto_connect_mode());
    tracer_cli(LOG_HIGH_LEVEL, "AP num = %d\r\n", get_auto_connect_ap_num());
    
    get_auto_connect_ap_cfg(&cfg);
    tracer_cli(LOG_HIGH_LEVEL, "AP cfg flag = %d, front = %d, rear = %d\r\n", cfg.flag, cfg.front, cfg.rear);
    tracer_cli(LOG_HIGH_LEVEL, "AP cfg max_save_num = %d\r\n", cfg.max_save_num);

    for (i=0; i<MAX_NUM_OF_AUTO_CONNECT; i++) {
        get_auto_connect_info(i, &info);
        tracer_cli(LOG_HIGH_LEVEL, "AP[%d] info channel = %d\r\n", i, info.ap_channel);
        tracer_cli(LOG_HIGH_LEVEL, "AP[%d] info fast mode = %d\r\n", i, info.fast_connect);
        tracer_cli(LOG_HIGH_LEVEL, "AP[%d] info bssid %02x %02x %02x %02x %02x %02x\r\n",
                    i, info.bssid[0], info.bssid[1], info.bssid[2], info.bssid[3], info.bssid[4],
                    info.bssid[5]);
        tracer_cli(LOG_HIGH_LEVEL, "AP[%d] info ssid = %s\r\n", i, info.ssid);
        tracer_cli(LOG_HIGH_LEVEL, "AP[%d] info psk = %02x %02x %02x %02x %02x\r\n",
                    i, info.psk[0], info.psk[1], info.psk[2], info.psk[3], info.psk[4]);
        tracer_cli(LOG_HIGH_LEVEL, "AP[%d] DTIM period = %d \r\n", i, info.dtim_prod); //[0000560]
    }
}

int wpa_cli_conn_mode_impl(int argc, char *argv[])
{
    if (argc == 2) {

        s8 mode = atoi(argv[1]);

        if (mode < AUTO_CONNECT_DISABLE || mode > AUTO_CONNECT_ENABLE) {
            return FALSE;
        }

        wifi_nvm_auto_connect_write(WIFI_NVM_ID_AUTO_CONNECT_MODE, 1, NULL, &mode);
        gAutoConnMode = mode;
        g_AutoConnect.retryCount = 0; //restart fasct connect
        g_AutoConnect.targetIdx = 0;
        
        tracer_cli(LOG_HIGH_LEVEL, "connection mode = %d (0:disable, 1:fast connect)\r\n", get_auto_connect_mode());
        
        return TRUE;
    }
    
    return FALSE;
}

int wpa_cli_clear_ac_list_impl(int argc, char *argv[])
{
    // Reset Auto/Fast connect configuartion in the FIM
    MwFim_FileWriteDefault(MW_FIM_IDX_WIFI_AUTO_CONN_MODE, 0);
    MwFim_FileWriteDefault(MW_FIM_IDX_WIFI_AUTO_CONN_AP_NUM, 0);
    MwFim_FileWriteDefault(MW_FIM_IDX_WIFI_AUTO_CONN_CFG, 0);
    MwFim_FileWriteDefault(MW_FIM_IDX_WIFI_AUTO_CONN_AP_INFO, 0);
    MwFim_FileWriteDefault(MW_FIM_IDX_WIFI_AUTO_CONN_AP_INFO, 1);
    MwFim_FileWriteDefault(MW_FIM_IDX_WIFI_AUTO_CONN_AP_INFO, 2);

    // Reset Auto/Fast connect configuration of global variables
    reset_auto_connect_list();

    //Reset Sta information
    MwFim_FileWriteDefault(MW_FIM_IDX_GP03_STA_MAC_ADDR, 0); //[0000526]
    MwFim_FileWriteDefault(MW_FIM_IDX_STA_SKIP_DTIM, 0); //[0000526]
    
    return TRUE;
}

int wpa_cli_fast_connect_impl(int argc, char *argv[])
{
    u8 mode, ap_index;
    
    if(argc != 3) {
        wpa_printf(MSG_ERROR, "[CLI]WPA: invalid parameter \r\n");
        return FALSE;
    }

    mode = atoi(argv[1]);
    ap_index = atoi(argv[2]);

    if (mode > AUTO_CONNECT_ENABLE || ap_index >= MAX_NUM_OF_AUTO_CONNECT) {
        wpa_printf(MSG_ERROR, "[CLI]WPA: invalid parameter \r\n");
        return FALSE;
    }

    wpa_driver_netlink_fast_connect(mode, ap_index);
    
    return TRUE;
}

//I0060 0000710_add_beg
void debug_cli_mac_addr_src_impl(void)
{
    u8 type;

    tracer_cli(LOG_HIGH_LEVEL, "Mac Address Source:\r\n");
    tracer_cli(LOG_HIGH_LEVEL, "   0:OTP, 1:Flash\r\n");
    base_mac_addr_src_get_cfg(BASE_NVM_MAC_SRC_IFACE_ID_STA, &type);
    tracer_cli(LOG_HIGH_LEVEL, "   WIFI STA : %d", type);
    base_mac_addr_src_get_cfg(BASE_NVM_MAC_SRC_IFACE_ID_SOFTAP, &type);
    tracer_cli(LOG_HIGH_LEVEL, "   SoftAP: %d", type);
    base_mac_addr_src_get_cfg(BASE_NVM_MAC_SRC_IFACE_ID_BLE, &type);
    tracer_cli(LOG_HIGH_LEVEL, "   BLE : %d\r\n", type);
}
//I0060 0000710_add_end

/* debug use */
int wpa_cli_dbg_impl(int argc, char *argv[])
{
    u8 mac[MAC_ADDR_LEN] = {0};
    
    if (!strcmp(argv[1], "h")) {
        tracer_cli(LOG_HIGH_LEVEL, "wpa debug :\r\n");
        tracer_cli(LOG_HIGH_LEVEL, "   h : help\r\n");
        tracer_cli(LOG_HIGH_LEVEL, "   p : print memory variable of auto connect/CBS ...\r\n");
        tracer_cli(LOG_HIGH_LEVEL, "   ia : Test input mac addr/manufacture name for CBS\r\n");
        tracer_cli(LOG_HIGH_LEVEL, "   ib : Test input mac address source config\r\n"); //I0060 0000710
        tracer_cli(LOG_HIGH_LEVEL, "        iface, type\r\n"); //I0060 0000710
        return TRUE;
    }
    
    if (!strcmp(argv[1], "p") || argc == 1) {
        debug_auto_connect();
        debug_cli_mac_addr_src(); //I0060 0000710
        return TRUE;
    }

    if (!strcmp(argv[1], "ia")) {
        if (argc >= 3) { // debug for CBS
            hwaddr_aton2(argv[2], mac);
            wpa_cli_setmac(mac);
        }
        
        if (argc >= 4) {
            wifi_nvm_sta_info_write(WIFI_NVM_STA_INFO_MANUFACTURE_NAME, STA_INFO_MAX_MANUF_NAME_SIZE, (u8 *)argv[3]);
        }
    }
    
    //I0060 0000710_add_beg
    if (!strcmp(argv[1], "ib")) {
        u8 iface, type;
        iface = atoi(argv[2]);
        type = atoi(argv[3]);
        base_mac_addr_src_set_cfg(iface, type);
    }
    //I0060 0000710_add_end

    return TRUE;
}

int wpa_cli_cmd_level_impl(int argc, char *argv[])
{
    if (argc == 1) {
        tracer_cli(LOG_HIGH_LEVEL, "Current level: %d \n", wpa_driver_netlink_get_log_level());
        tracer_cli(LOG_HIGH_LEVEL, "EXCESSIVE = 0, MSGDUMP = 1, DEBUG = 2, INFO = 3, WARNING = 4, ERROR = 5 \n");
    }
    else {
        u8 level = atoi(argv[1]);
        if (level > 5) {
            return FALSE;
        }
        wpa_driver_netlink_set_log_level(level);
        
        tracer_cli(LOG_HIGH_LEVEL, "Current level: %d \n", wpa_driver_netlink_get_log_level());
        tracer_cli(LOG_HIGH_LEVEL, "EXCESSIVE = 0, MSGDUMP = 1, DEBUG = 2, INFO = 3, WARNING = 4, ERROR = 5 \n");
    }
    return TRUE;
}

int wpa_cli_cmd_status_impl(int argc, char *argv[])
{
    char buf[64] = {0};
    u8 mac[MAC_ADDR_LEN] = {0};
    u8 ssid[MAX_LEN_OF_SSID + 1] = {0};
    
    if (wpa_s->wpa_state >= WPA_ASSOCIATED) {
        wpa_printf(MSG_INFO, "bssid=" MACSTR "\n", MAC2STR(wpa_s->bssid));
        wpa_printf(MSG_INFO, "freq=%u \n", wpa_s->assoc_freq);
        wpa_cli_getssid(ssid);
        wpa_printf(MSG_INFO, "ssid=%s \n", ssid);
        wpa_sm_get_status(wpa_s->wpa, buf, sizeof(buf), 0);
        wpa_printf(MSG_INFO, "%s", buf);
    }
    
    wpa_printf(MSG_INFO, "wpa_state=%s \n", wpa_supplicant_state_txt(wpa_s->wpa_state));
    wpa_cli_getmac(mac);
    wpa_printf(MSG_INFO, "address=" MACSTR "\n", MAC2STR(mac));
    
    return TRUE;
}

//I0064 0000907_add_beg
int wpa_cli_set_encrypt_mode_impl(int argc, char *argv[])
{
    int ret=TRUE;
    int en_value=0;
    if(argc!=2)
    {
         wpa_printf(MSG_ERROR, "[CLI]WPA: invalid parameter \r\n");
         return FALSE;
    }    
    en_value=atoi(argv[1]);
    ret=wpa_driver_netlink_set_encrypt_mode(en_value);
    
    return ret;
}
//I0064 0000907_add_end


uint32_t wpa_cli_cmd_handler_impl(int argc, char *argv[])
{
    if (argc < 1) return FALSE;
    tracer_cli(LOG_HIGH_LEVEL, "\n");

    if (os_strncasecmp(WPA_CLI_CMD_SCAN, argv[0], os_strlen(argv[0])) == 0){
        wpa_cli_scan_handler(argc, argv);
    } else if(strncasecmp(WPA_CLI_CMD_SCAN_STOP, argv[0], os_strlen(WPA_CLI_CMD_SCAN_STOP)) == 0) {
        wpa_cli_scan_stop_handler(argc, argv);
    } else if (os_strncasecmp(WPA_CLI_CMD_SCAN_RESULTS, argv[0], os_strlen(argv[0])) == 0) {
        wpa_cli_showscanresults_handler(argc, argv);
    } else if (os_strncasecmp(WPA_CLI_CMD_CONNECT, argv[0], os_strlen(argv[0])) == 0) {
        wpa_cli_connect_handler(argc, argv);
    } else if (os_strncasecmp(WPA_CLI_CMD_RECONNECT, argv[0], os_strlen(argv[0])) == 0) {
        wpa_cli_reconnect_handler(argc, argv);
    } else if (os_strncasecmp(WPA_CLI_CMD_DISCONNECT, argv[0], os_strlen(argv[0])) == 0) {
        wpa_cli_disconnect_handler(argc, argv);
    } else if (os_strncasecmp(WPA_CLI_CMD_BSSID, argv[0], os_strlen(argv[0])) == 0) {
        wpa_cli_showbssid_by_param(argc, argv);
    } else if (os_strncasecmp(WPA_CLI_CMD_SSID, argv[0], os_strlen(argv[0])) == 0) {
        wpa_cli_showssid_by_param(argc, argv);
    } else if (os_strncasecmp(WPA_CLI_CMD_HELP, argv[0], os_strlen(argv[0])) == 0) {
        wpa_printf(MSG_INFO, "[CLI]WPA: not support yet \r\n");
    } else if (os_strncasecmp(WPA_CLI_CMD_STATE, argv[0], os_strlen(argv[0])) == 0) {
        wpa_cli_cmd_status(argc, argv);
    } else if (os_strncasecmp(WPA_CLI_CMD_ADD_NETWORK, argv[0], os_strlen(argv[0])) == 0) {
        wpa_printf(MSG_INFO, "[CLI]WPA: not support yet \r\n");
    } else if (os_strncasecmp(WPA_CLI_CMD_SET_NETWORK, argv[0], os_strlen(argv[0])) == 0) {
        wpa_printf(MSG_INFO, "[CLI]WPA: not support yet \r\n");
    } else if (os_strncasecmp(WPA_CLI_CMD_SAVE_CONFIG, argv[0], os_strlen(argv[0])) == 0) {
        wpa_printf(MSG_INFO, "[CLI]WPA: not support yet \r\n");
    } else if (os_strncasecmp(WPA_CLI_CMD_MAC, argv[0], os_strlen(argv[0])) == 0) {
        wpa_cli_mac_by_param(argc, argv);
    } else if (os_strncasecmp(WPA_CLI_CMD_SCRT_DEBUG, argv[0], os_strlen(argv[0])) == 0) {
        wpa_cli_scrt_dbg_by_param(argc, argv);
    } else if (os_strncasecmp(WPA_CLI_CMD_RSSI, argv[0], os_strlen(argv[0])) == 0) {
        wpa_cli_getrssi();
    }
    else if (os_strncasecmp(WPA_CLI_CMD_CONN_MODE, argv[0], os_strlen(argv[0])) == 0) {
        wpa_cli_conn_mode(argc, argv);
    }
    else if (os_strncasecmp(WPA_CLI_CMD_CLEAR_AC_LIST, argv[0], os_strlen(argv[0])) == 0){
        wpa_cli_clear_ac_list(argc, argv);
    }
    else if (os_strncasecmp(WPA_CLI_CMD_FAST_CONNECT, argv[0], os_strlen(argv[0])) == 0){
        wpa_cli_fast_connect(argc, argv);
    }
    else if (os_strncasecmp(WPA_CLI_CMD_DBG, argv[0], os_strlen(WPA_CLI_CMD_DBG)) == 0){
        wpa_cli_dbg(argc, argv);
    }
    else if (os_strncasecmp(WPA_CLI_CMD_LOG_LEVEL, argv[0], os_strlen(WPA_CLI_CMD_LOG_LEVEL)) == 0) {
        wpa_cli_cmd_level(argc, argv);
    }
    else if (os_strncasecmp(WPA_CLI_CMD_SET_ENCRYPT_MODE, argv[0], os_strlen(WPA_CLI_CMD_SET_ENCRYPT_MODE)) == 0) { //I0064 0000907
        wpa_cli_set_encrypt_mode(argc, argv);                                                                       //I0064 0000907
    }
    else {
        //nothing
        msg_print(LOG_HIGH_LEVEL, "\r\n");
    }

    return TRUE;
}


void wpa_cli_parse_impl(char* pszData) //pszData => wpa_xxx parm1 parm2 ...
{
    char *argv[WPA_CLI_CMD_NUM_MAX] = {0}; //argv[0]:wap_xxx argv[1]:parm1 argv[2]:parm3
    int count = 0;
    char *p = NULL;
    char *pSavedPtr = NULL;

    /* get the first word from the message, seperated by
          space character */
    p = strtok_r(pszData, " ", &pSavedPtr); //word1: wpa_xxx
    argv[count] = p; //count = 0
    count++;

    /* the following loop gets the rest of the words until the
     * end of the message */
    while ((p = strtok_r(NULL, " ", &pSavedPtr)) != NULL){ //p: parmx
        argv[count] = p;
        count++;
    }

    wpa_cli_cmd_handler(count, argv);
}

RET_DATA wpa_cli_connect_fp_t wpa_cli_connect;
RET_DATA wpa_cli_connect_by_bssid_fp_t wpa_cli_connect_by_bssid;
RET_DATA wpa_cli_connect_handler_fp_t wpa_cli_connect_handler;
RET_DATA wpa_cli_disconnect_fp_t wpa_cli_disconnect;
RET_DATA wpa_cli_disconnect_handler_fp_t wpa_cli_disconnect_handler;
RET_DATA wpa_cli_reconnect_fp_t wpa_cli_reconnect;
RET_DATA wpa_cli_reconnect_handler_fp_t wpa_cli_reconnect_handler;
RET_DATA wpa_cli_scan_fp_t wpa_cli_scan;
RET_DATA wpa_cli_scan_handler_fp_t wpa_cli_scan_handler;
RET_DATA wpa_cli_scan_stop_handler_fp_t wpa_cli_scan_stop_handler;
RET_DATA wpa_cli_getscanresults_fp_t wpa_cli_getscanresults;
RET_DATA wpa_cli_showscanresults_handler_fp_t wpa_cli_showscanresults_handler;
RET_DATA wpa_cli_getbssid_fp_t wpa_cli_getbssid;
RET_DATA wpa_cli_showbssid_by_param_fp_t wpa_cli_showbssid_by_param;
RET_DATA wpa_cli_getssid_fp_t wpa_cli_getssid;
RET_DATA wpa_cli_showssid_by_param_fp_t wpa_cli_showssid_by_param;
RET_DATA wpa_cli_getmac_fp_t wpa_cli_getmac;
RET_DATA wpa_cli_mac_by_param_fp_t wpa_cli_mac_by_param;
RET_DATA wpa_cli_setmac_fp_t wpa_cli_setmac;
RET_DATA wpa_cli_parse_fp_t wpa_cli_parse;
RET_DATA wpa_cli_scrt_dbg_by_param_fp_t wpa_cli_scrt_dbg_by_param;
RET_DATA wpa_cli_cmd_handler_fp_t wpa_cli_cmd_handler;
RET_DATA wpa_cli_getrssi_fp_t wpa_cli_getrssi;
RET_DATA debug_auto_connect_fp_t debug_auto_connect;
RET_DATA wpa_cli_conn_mode_fp_t wpa_cli_conn_mode;
RET_DATA wpa_cli_clear_ac_list_fp_t wpa_cli_clear_ac_list;
RET_DATA wpa_cli_fast_connect_fp_t wpa_cli_fast_connect;
RET_DATA wpa_cli_dbg_fp_t wpa_cli_dbg;
RET_DATA wpa_cli_cmd_level_fp_t wpa_cli_cmd_level;
RET_DATA wpa_cli_set_encrypt_mode_fp_t wpa_cli_set_encrypt_mode;   //I0064 0000907

RET_DATA check_mac_addr_len_fp_t check_mac_addr_len; //I0060 0000662
RET_DATA debug_cli_mac_addr_src_fp_t debug_cli_mac_addr_src; //I0060 0000710
RET_DATA wpa_cli_scan_by_cfg_fp_t wpa_cli_scan_by_cfg;
RET_DATA wpa_cli_cmd_status_fp_t wpa_cli_cmd_status;

/*
   Interface Initialization: WPA CLI
 */
void wpa_cli_func_init(void)
{
    wpa_cli_connect                 = wpa_cli_connect_impl;
    wpa_cli_connect_by_bssid        = wpa_cli_connect_by_bssid_impl;
    wpa_cli_connect_handler         = wpa_cli_connect_handler_impl;
    wpa_cli_disconnect              = wpa_cli_disconnect_impl;
    wpa_cli_disconnect_handler      = wpa_cli_disconnect_handler_impl;
    wpa_cli_reconnect               = wpa_cli_reconnect_impl;
    wpa_cli_reconnect_handler       = wpa_cli_reconnect_handler_impl;
    wpa_cli_scan                    = wpa_cli_scan_impl;
    wpa_cli_scan_handler            = wpa_cli_scan_handler_impl;
    wpa_cli_scan_stop_handler       = wpa_cli_scan_stop_handler_impl;
    wpa_cli_getscanresults          = wpa_cli_getscanresults_impl;
    wpa_cli_showscanresults_handler = wpa_cli_showscanresults_handler_impl;
    wpa_cli_getbssid                = wpa_cli_getbssid_impl;
    wpa_cli_showbssid_by_param      = wpa_cli_showbssid_by_param_impl;
    wpa_cli_getssid                 = wpa_cli_getssid_impl;
    wpa_cli_showssid_by_param       = wpa_cli_showssid_by_param_impl;
    wpa_cli_getmac                  = wpa_cli_getmac_impl;
    wpa_cli_mac_by_param            = wpa_cli_mac_by_param_impl;
    wpa_cli_setmac                  = wpa_cli_setmac_impl;
    wpa_cli_parse                   = wpa_cli_parse_impl;
    wpa_cli_scrt_dbg_by_param       = wpa_cli_scrt_dbg_by_param_impl;
    wpa_cli_cmd_handler             = wpa_cli_cmd_handler_impl;
    wpa_cli_getrssi                 = wpa_cli_getrssi_impl;
    debug_auto_connect              = debug_auto_connect_impl;
    wpa_cli_conn_mode               = wpa_cli_conn_mode_impl;
    wpa_cli_clear_ac_list           = wpa_cli_clear_ac_list_impl;
    wpa_cli_fast_connect            = wpa_cli_fast_connect_impl;
    wpa_cli_dbg                     = wpa_cli_dbg_impl;
    check_mac_addr_len              = check_mac_addr_len_impl; //I0060 0000662
    debug_cli_mac_addr_src          = debug_cli_mac_addr_src_impl; //I0060 0000710
    wpa_cli_scan_by_cfg             = wpa_cli_scan_by_cfg_impl;
    wpa_cli_cmd_level               = wpa_cli_cmd_level_impl;
    wpa_cli_cmd_status              = wpa_cli_cmd_status_impl;
    wpa_cli_set_encrypt_mode        = wpa_cli_set_encrypt_mode_impl; //I0064 0000907
}
