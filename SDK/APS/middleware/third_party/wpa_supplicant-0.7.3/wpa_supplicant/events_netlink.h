/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/
#ifndef EVENTS_NETLINK_H
#define EVENTS_NETLINK_H

#include "driver.h"
#include "wpa_supplicant_i.h"

typedef void (*wpa_supplicant_mark_disassoc_fp_t)(struct wpa_supplicant *wpa_s);
typedef int (*wpa_supplicant_dynamic_keys_fp_t)(struct wpa_supplicant *wpa_s);
typedef int (*wpa_supplicant_event_associnfo_fp_t)(struct wpa_supplicant *wpa_s, union wpa_event_data *data);
typedef void (*wpa_supplicant_event_assoc_fp_t)(struct wpa_supplicant *wpa_s, union wpa_event_data *data);
typedef void (*wpa_supplicant_event_disassoc_fp_t)(struct wpa_supplicant *wpa_s, u16 reason_code);
typedef void (*wpa_supplicant_event_eapol_rx_fp_t)(struct wpa_supplicant *wpa_s);
typedef void (*wpa_supplicant_event_fp_t)(void *ctx, enum wpa_event_type event, union wpa_event_data *data);
typedef void (*wpa_supplicant_mic_error_update_fp_t)(uint8_t *u8aBssid, uint32_t u32Sec);
typedef uint32_t (*wpa_supplicant_last_mic_err_time_get_fp_t)(uint8_t *u8aBssid);
typedef int (*wpa_supplicant_is_bssid_blocked_by_mic_error_fp_t)(uint8_t *u8aBssid, uint32_t *pu32Remain);
typedef void (*wpa_supplicant_event_michael_mic_failure_fp_t)(struct wpa_supplicant *wpa_s, union wpa_event_data *data);

extern wpa_supplicant_mark_disassoc_fp_t wpa_supplicant_mark_disassoc;
extern wpa_supplicant_dynamic_keys_fp_t wpa_supplicant_dynamic_keys;
extern wpa_supplicant_event_associnfo_fp_t wpa_supplicant_event_associnfo;
extern wpa_supplicant_event_assoc_fp_t wpa_supplicant_event_assoc;
extern wpa_supplicant_event_disassoc_fp_t wpa_supplicant_event_disassoc;
extern wpa_supplicant_event_eapol_rx_fp_t wpa_supplicant_event_eapol_rx;
extern wpa_supplicant_event_fp_t wpa_supplicant_event;
extern wpa_supplicant_mic_error_update_fp_t wpa_supplicant_mic_error_update;
extern wpa_supplicant_last_mic_err_time_get_fp_t wpa_supplicant_last_mic_err_time_get;
extern wpa_supplicant_is_bssid_blocked_by_mic_error_fp_t wpa_supplicant_is_bssid_blocked_by_mic_error;
extern wpa_supplicant_event_michael_mic_failure_fp_t wpa_supplicant_event_michael_mic_failure;

#ifndef CONFIG_NO_SCAN_PROCESSING
typedef void (*wpa_supplicant_req_new_scan_fp_t)(struct wpa_supplicant *wpa_s, int timeout_sec, int timeout_usec);
typedef void (*wpa_supplicant_connect_fp_t)(struct wpa_supplicant *wpa_s, struct wpa_bss *selected, struct wpa_ssid *ssid);
typedef void (*wpa_supplicant_event_scan_results_fp_t)(struct wpa_supplicant *wpa_s, union wpa_event_data *data);
extern wpa_supplicant_req_new_scan_fp_t wpa_supplicant_req_new_scan;
extern wpa_supplicant_connect_fp_t wpa_supplicant_connect;
extern wpa_supplicant_event_scan_results_fp_t wpa_supplicant_event_scan_results;
#endif

/*
   Interface Initialization: WPA Events
 */
void wpa_events_func_init(void);


#endif //EVENTS_NETLINK_H

