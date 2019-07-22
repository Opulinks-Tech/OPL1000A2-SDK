/*
 * WPA Supplicant - Scanning
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

#ifndef SCAN_H
#define SCAN_H
#include "driver.h"


typedef int (*wpa_supplicant_trigger_scan_fp_t)(struct wpa_supplicant *wpa_s, struct wpa_driver_scan_params *params);
typedef void (*wpa_supplicant_scan_fp_t)(void *eloop_ctx, void *timeout_ctx);
typedef void (*wpa_supplicant_req_scan_fp_t)(struct wpa_supplicant *wpa_s, int sec, int usec);
typedef void (*wpa_supplicant_cancel_scan_fp_t)(struct wpa_supplicant *wpa_s);
typedef const u8 * (*wpa_scan_get_ie_fp_t)(const struct wpa_scan_res *res, u8 ie);
//typedef struct wpa_scan_results *(*wpa_supplicant_get_scan_results_fp_t)(struct wpa_supplicant *wpa_s, struct scan_info *info, int new_scan);
typedef int (*wpa_supplicant_update_scan_results_fp_t)(struct wpa_supplicant *wpa_s);
typedef void (*wpa_scan_results_free_fp_t)(struct wpa_scan_results *res);


extern wpa_supplicant_trigger_scan_fp_t wpa_supplicant_trigger_scan;
extern wpa_supplicant_scan_fp_t wpa_supplicant_scan;
extern wpa_supplicant_req_scan_fp_t wpa_supplicant_req_scan;
extern wpa_supplicant_cancel_scan_fp_t wpa_supplicant_cancel_scan;
extern wpa_scan_get_ie_fp_t wpa_scan_get_ie;
//extern wpa_supplicant_get_scan_results_fp_t wpa_supplicant_get_scan_results;
extern wpa_supplicant_update_scan_results_fp_t wpa_supplicant_update_scan_results;
extern wpa_scan_results_free_fp_t wpa_scan_results_free;


/*
   Interface Initialization: WPA Scan
 */
void wpa_scan_func_init(void);


#if 0
const u8 * wpa_scan_get_vendor_ie(const struct wpa_scan_res *res, u32 vendor_type);
struct wpabuf * wpa_scan_get_vendor_ie_multi(const struct wpa_scan_res *res, u32 vendor_type);
void wpa_supplicant_notify_scanning(struct wpa_supplicant *wpa_s, int scanning);
int wpa_supplicant_enabled_networks(struct wpa_config *conf);
//struct wpa_driver_scan_params;
void wpa_supplicant_req_scan(struct wpa_supplicant *wpa_s, int sec, int usec);
void wpa_supplicant_cancel_scan(struct wpa_supplicant *wpa_s);
int wpa_supplicant_trigger_scan(struct wpa_supplicant *wpa_s, struct wpa_driver_scan_params *params);
struct wpa_scan_results * wpa_supplicant_get_scan_results(struct wpa_supplicant *wpa_s, struct scan_info *info, int new_scan);
int wpa_supplicant_update_scan_results(struct wpa_supplicant *wpa_s);
const u8 * wpa_scan_get_ie(const struct wpa_scan_res *res, u8 ie);
void wpa_scan_results_free(struct wpa_scan_results *res);
#endif

#endif /* SCAN_H */
