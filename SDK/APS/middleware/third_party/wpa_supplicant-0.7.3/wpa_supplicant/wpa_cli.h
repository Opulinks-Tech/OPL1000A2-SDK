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

#ifndef WPA_CLI_H
#define WPA_CLI_H

#include "config.h"
#include "driver.h"
#include "defs.h"
#include "wpa_common.h"

/* Reason codes (IEEE 802.11-2007, 7.3.1.7, Table 7-22) */
#ifndef WLAN_REASON_UNSPECIFIED
#define WLAN_REASON_UNSPECIFIED 1
#endif
#ifndef WLAN_REASON_PREV_AUTH_NOT_VALID
#define WLAN_REASON_PREV_AUTH_NOT_VALID 2
#endif
#ifndef WLAN_REASON_DEAUTH_LEAVING
#define WLAN_REASON_DEAUTH_LEAVING 3
#endif
#ifndef WLAN_REASON_DISASSOC_DUE_TO_INACTIVITY
#define WLAN_REASON_DISASSOC_DUE_TO_INACTIVITY 4
#endif
#ifndef WLAN_REASON_DISASSOC_AP_BUSY
#define WLAN_REASON_DISASSOC_AP_BUSY 5
#endif
#ifndef WLAN_REASON_CLASS2_FRAME_FROM_NONAUTH_STA
#define WLAN_REASON_CLASS2_FRAME_FROM_NONAUTH_STA 6
#endif
#ifndef WLAN_REASON_CLASS3_FRAME_FROM_NONASSOC_STA
#define WLAN_REASON_CLASS3_FRAME_FROM_NONASSOC_STA 7
#endif
#ifndef WLAN_REASON_DISASSOC_STA_HAS_LEFT
#define WLAN_REASON_DISASSOC_STA_HAS_LEFT 8
#endif
#ifndef WLAN_REASON_STA_REQ_ASSOC_WITHOUT_AUTH
#define WLAN_REASON_STA_REQ_ASSOC_WITHOUT_AUTH 9
#endif
#ifndef WLAN_REASON_PWR_CAPABILITY_NOT_VALID
/* IEEE 802.11h */
#define WLAN_REASON_PWR_CAPABILITY_NOT_VALID 10
#endif
#ifndef WLAN_REASON_SUPPORTED_CHANNEL_NOT_VALID
#define WLAN_REASON_SUPPORTED_CHANNEL_NOT_VALID 11
#endif
#ifndef WLAN_REASON_INVALID_IE
/* IEEE 802.11i */
#define WLAN_REASON_INVALID_IE 13
#endif
#ifndef WLAN_REASON_MICHAEL_MIC_FAILURE
#define WLAN_REASON_MICHAEL_MIC_FAILURE 14
#endif
#ifndef WLAN_REASON_4WAY_HANDSHAKE_TIMEOUT
#define WLAN_REASON_4WAY_HANDSHAKE_TIMEOUT 15
#endif
#ifndef WLAN_REASON_GROUP_KEY_UPDATE_TIMEOUT
#define WLAN_REASON_GROUP_KEY_UPDATE_TIMEOUT 16
#endif
#ifndef WLAN_REASON_IE_IN_4WAY_DIFFERS
#define WLAN_REASON_IE_IN_4WAY_DIFFERS 17
#endif
#ifndef WLAN_REASON_GROUP_CIPHER_NOT_VALID
#define WLAN_REASON_GROUP_CIPHER_NOT_VALID 18
#endif
#ifndef WLAN_REASON_PAIRWISE_CIPHER_NOT_VALID
#define WLAN_REASON_PAIRWISE_CIPHER_NOT_VALID 19
#endif
#ifndef WLAN_REASON_AKMP_NOT_VALID
#define WLAN_REASON_AKMP_NOT_VALID 20
#endif
#ifndef WLAN_REASON_UNSUPPORTED_RSN_IE_VERSION
#define WLAN_REASON_UNSUPPORTED_RSN_IE_VERSION 21
#endif
#ifndef WLAN_REASON_INVALID_RSN_IE_CAPAB
#define WLAN_REASON_INVALID_RSN_IE_CAPAB 22
#endif
#ifndef WLAN_REASON_IEEE_802_1X_AUTH_FAILED
#define WLAN_REASON_IEEE_802_1X_AUTH_FAILED 23
#endif
#ifndef WLAN_REASON_CIPHER_SUITE_REJECTED
#define WLAN_REASON_CIPHER_SUITE_REJECTED 24
#endif

#define WPA_CLI_CMD_NUM_MAX 5
#define WPA_CLI_CMD_LEN_TOTAL 100

#define WPA_CLI_CMD_SCAN            "wpa_scan"
#define WPA_CLI_CMD_SCAN_RESULTS    "wpa_scan_results"
#define WPA_CLI_CMD_SCAN_STOP       "wpa_scan_stop"
#define WPA_CLI_CMD_CONNECT         "wpa_connect"
#define WPA_CLI_CMD_CONNECT_FROM_AC "wpa_connect_ac"
#define WPA_CLI_CMD_RECONNECT       "wpa_reconnect"
#define WPA_CLI_CMD_DISCONNECT      "wpa_disconnect"
#define WPA_CLI_CMD_BSSID           "wpa_bssid"
#define WPA_CLI_CMD_SSID            "wpa_ssid"
#define WPA_CLI_CMD_HELP            "wpa_help"
#define WPA_CLI_CMD_STATE           "wpa_state"
#define WPA_CLI_CMD_ADD_NETWORK     "wpa_add_network"
#define WPA_CLI_CMD_SET_NETWORK     "wpa_set_network"
#define WPA_CLI_CMD_SAVE_CONFIG     "wpa_save_config"
#define WPA_CLI_CMD_MAC             "wpa_mac"
#define WPA_CLI_CMD_SCRT_DEBUG      "wpa_scrt_debug"
#define WPA_CLI_CMD_RSSI            "wpa_rssi"
#define WPA_CLI_CMD_DBG_MODE        "wpa_dbg_mode"
#define WPA_CLI_CMD_CONN_MODE       "wpa_conn_mode"
#define WPA_CLI_CMD_CLEAR_AC_LIST   "wpa_clear_ac"
#define WPA_CLI_CMD_FAST_CONNECT    "wpa_fast_connect"
#define WPA_CLI_CMD_DBG             "wpa_dbg"
#define WPA_CLI_CMD_LOG_LEVEL       "wpa_log_level"
#define WPA_CLI_CMD_SET_ENCRYPT_MODE "wpa_set_encrypt_mode"

/*
  CLI function prototype
*/
typedef int (*wpa_cli_connect_fp_t)(struct wpa_config * conf);
typedef int (*wpa_cli_connect_by_bssid_fp_t)(u8 *bssid);
typedef int (*wpa_cli_connect_handler_fp_t)(int argc, char *argv[]);
typedef int (*wpa_cli_disconnect_fp_t)(const u8 *bssid, u16 reason_code);
typedef int (*wpa_cli_disconnect_handler_fp_t)(int argc, char *argv[]);
typedef int (*wpa_cli_reconnect_fp_t)(struct wpa_config * conf);
typedef int (*wpa_cli_reconnect_handler_fp_t)(int argc, char *argv[]);
typedef int (*wpa_cli_scan_fp_t)(int mode);
typedef int (*wpa_cli_scan_handler_fp_t)(int argc, char *argv[]);
typedef int (*wpa_cli_scan_stop_handler_fp_t)(int argc, char *argv[]);
typedef int (*wpa_cli_getscanresults_fp_t)(struct wpa_scan_results * res);
typedef void (*wpa_cli_showscanresults_handler_fp_t)(int argc, char *argv[]);
typedef void (*wpa_cli_getbssid_fp_t)(u8 *bssid);
typedef void (*wpa_cli_showbssid_by_param_fp_t)(int argc, char *argv[]);
typedef void (*wpa_cli_getssid_fp_t)(u8 *ssid);
typedef void (*wpa_cli_showssid_by_param_fp_t)(int argc, char *argv[]);
typedef void (*wpa_cli_getmac_fp_t)(u8 *mac);
typedef void (*wpa_cli_mac_by_param_fp_t)(int argc, char *argv[]);
typedef void (*wpa_cli_setmac_fp_t)(u8 *mac);
typedef void (*wpa_cli_parse_fp_t)(char* pszData);
typedef void (*wpa_cli_scrt_dbg_by_param_fp_t)(int argc, char *argv[]);
typedef uint32_t (*wpa_cli_cmd_handler_fp_t)(int argc, char *argv[]);
typedef int (*wpa_cli_getrssi_fp_t)(void);
typedef int (*wpa_cli_conn_mode_fp_t) (int argc, char *argv[]);
typedef void (*debug_auto_connect_fp_t) (void);
typedef int (*wpa_cli_clear_ac_list_fp_t) (int argc, char *argv[]);
typedef int (*wpa_cli_fast_connect_fp_t) (int argc, char *argv[]);
typedef int (*wpa_cli_dbg_fp_t) (int argc, char *argv[]);
typedef int (*check_mac_addr_len_fp_t) (const char *txt);
typedef void (*debug_cli_mac_addr_src_fp_t) (void);
typedef int (*wpa_cli_scan_by_cfg_fp_t) (void *cfg);
typedef int (*wpa_cli_cmd_level_fp_t)(int argc, char *argv[]);
typedef int (*wpa_cli_cmd_status_fp_t)(int argc, char *argv[]);
typedef int (*wpa_cli_set_encrypt_mode_fp_t)(int argc, char *argv[]);

extern wpa_cli_connect_fp_t wpa_cli_connect;
extern wpa_cli_connect_by_bssid_fp_t wpa_cli_connect_by_bssid;
extern wpa_cli_connect_handler_fp_t wpa_cli_connect_handler;
extern wpa_cli_disconnect_fp_t wpa_cli_disconnect;
extern wpa_cli_disconnect_handler_fp_t wpa_cli_disconnect_handler;
extern wpa_cli_reconnect_fp_t wpa_cli_reconnect;
extern wpa_cli_reconnect_handler_fp_t wpa_cli_reconnect_handler;
extern wpa_cli_scan_fp_t wpa_cli_scan;
extern wpa_cli_scan_handler_fp_t wpa_cli_scan_handler;
extern wpa_cli_scan_stop_handler_fp_t wpa_cli_scan_stop_handler;
extern wpa_cli_getscanresults_fp_t wpa_cli_getscanresults;
extern wpa_cli_showscanresults_handler_fp_t wpa_cli_showscanresults_handler;
extern wpa_cli_getbssid_fp_t wpa_cli_getbssid;
extern wpa_cli_showbssid_by_param_fp_t wpa_cli_showbssid_by_param;
extern wpa_cli_getssid_fp_t wpa_cli_getssid;
extern wpa_cli_showssid_by_param_fp_t wpa_cli_showssid_by_param;
extern wpa_cli_getmac_fp_t wpa_cli_getmac;
extern wpa_cli_mac_by_param_fp_t wpa_cli_mac_by_param;
extern wpa_cli_setmac_fp_t wpa_cli_setmac;
extern wpa_cli_parse_fp_t wpa_cli_parse;
extern wpa_cli_scrt_dbg_by_param_fp_t wpa_cli_scrt_dbg_by_param;
extern wpa_cli_cmd_handler_fp_t wpa_cli_cmd_handler;
extern wpa_cli_getrssi_fp_t wpa_cli_getrssi;
extern wpa_cli_conn_mode_fp_t wpa_cli_conn_mode;
extern debug_auto_connect_fp_t debug_auto_connect;
extern wpa_cli_clear_ac_list_fp_t wpa_cli_clear_ac_list;
extern wpa_cli_fast_connect_fp_t wpa_cli_fast_connect;
extern wpa_cli_dbg_fp_t wpa_cli_dbg;
extern check_mac_addr_len_fp_t check_mac_addr_len;
extern debug_cli_mac_addr_src_fp_t debug_cli_mac_addr_src;
extern wpa_cli_scan_by_cfg_fp_t wpa_cli_scan_by_cfg;
extern wpa_cli_cmd_level_fp_t wpa_cli_cmd_level;
extern wpa_cli_cmd_status_fp_t wpa_cli_cmd_status;
extern wpa_cli_set_encrypt_mode_fp_t wpa_cli_set_encrypt_mode;

/*
   Interface Initialization: WPA CLI
 */
void wpa_cli_func_init(void);

#endif //#ifndef WPA_CLI_H
