/*
 * WPA Supplicant - Glue code to setup EAPOL and RSN modules
 * Copyright (c) 2003-2008, Jouni Malinen <j@w1.fi>
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

#ifndef WPAS_GLUE_H
#define WPAS_GLUE_H


#ifndef CONFIG_NO_WPA
typedef int (*wpa_get_beacon_ie_fp_t)(struct wpa_supplicant *wpa_s);
typedef enum wpa_states (*wpa_supplicant_get_state_fp_t)(struct wpa_supplicant *wpa_s);
typedef int (*wpa_supplicant_get_bssid_fp_t)(void *ctx, u8 *bssid);
typedef int (*wpa_supplicant_get_beacon_ie_fp_t)(void *ctx);
typedef void (*_wpa_supplicant_cancel_auth_timeout_fp_t)(void *wpa_s);
typedef void (*_wpa_supplicant_set_state_fp_t)(void *wpa_s, enum wpa_states state);
typedef enum wpa_states (*_wpa_supplicant_get_state_fp_t)(void *wpa_s);
typedef void (*_wpa_supplicant_disassociate_fp_t)(void *wpa_s, int reason_code);
typedef void (*_wpa_supplicant_deauthenticate_fp_t)(void *wpa_s, int reason_code);
typedef void * (*wpa_supplicant_get_network_ctx_fp_t)(void *wpa_s);
typedef int (*wpa_supplicant_set_key_fp_t)(void *_wpa_s, enum wpa_alg alg, const u8 *addr, int key_idx, int set_tx, const u8 *seq, size_t seq_len, const u8 *key, size_t key_len);
typedef int (*wpa_supplicant_add_pmkid_fp_t)(void *wpa_s, const u8 *bssid, const u8 *pmkid);
typedef int (*wpa_supplicant_remove_pmkid_fp_t)(void *wpa_s, const u8 *bssid, const u8 *pmkid);
extern wpa_get_beacon_ie_fp_t wpa_get_beacon_ie;
extern wpa_supplicant_get_state_fp_t wpa_supplicant_get_state;
extern wpa_supplicant_get_bssid_fp_t wpa_supplicant_get_bssid;
extern wpa_supplicant_get_beacon_ie_fp_t wpa_supplicant_get_beacon_ie;
extern _wpa_supplicant_cancel_auth_timeout_fp_t _wpa_supplicant_cancel_auth_timeout;
extern _wpa_supplicant_set_state_fp_t _wpa_supplicant_set_state;
extern _wpa_supplicant_get_state_fp_t _wpa_supplicant_get_state;
extern _wpa_supplicant_disassociate_fp_t _wpa_supplicant_disassociate;
extern _wpa_supplicant_deauthenticate_fp_t _wpa_supplicant_deauthenticate;
extern wpa_supplicant_get_network_ctx_fp_t wpa_supplicant_get_network_ctx;
extern wpa_supplicant_set_key_fp_t wpa_supplicant_set_key;
extern wpa_supplicant_add_pmkid_fp_t wpa_supplicant_add_pmkid;
extern wpa_supplicant_remove_pmkid_fp_t wpa_supplicant_remove_pmkid;
#endif //#ifndef CONFIG_NO_WPA


#ifdef IEEE8021X_EAPOL
typedef int (*wpa_supplicant_eapol_send_fp_t)(void *ctx, int type, const u8 *buf, size_t len);
typedef int (*wpa_eapol_set_wep_key_fp_t)(void *ctx, int unicast, int keyidx, const u8 *key, size_t keylen);
typedef void (*wpa_supplicant_eapol_cb_fp_t)(struct eapol_sm *eapol, int success, void *ctx);
typedef void (*wpa_supplicant_notify_eapol_done_fp_t)(void *ctx);
extern wpa_supplicant_eapol_send_fp_t wpa_supplicant_eapol_send;
extern wpa_eapol_set_wep_key_fp_t wpa_eapol_set_wep_key;
extern wpa_supplicant_eapol_cb_fp_t wpa_supplicant_eapol_cb;
extern wpa_supplicant_notify_eapol_done_fp_t wpa_supplicant_notify_eapol_done;
#if defined(CONFIG_CTRL_IFACE) || !defined(CONFIG_NO_STDOUT_DEBUG)
typedef void (*wpa_supplicant_eap_param_needed_fp_t)(void *ctx, const char *field, const char *txt);
//extern wpa_supplicant_eap_param_needed_fp_t wpa_supplicant_eap_param_needed;
#endif //#if defined(CONFIG_CTRL_IFACE) || !defined(CONFIG_NO_STDOUT_DEBUG)
#endif //#ifdef IEEE8021X_EAPOL


typedef int (*wpa_supplicant_init_eapol_fp_t)(struct wpa_supplicant *wpa_s);
typedef int (*wpa_supplicant_init_wpa_fp_t)(struct wpa_supplicant *wpa_s);
extern wpa_supplicant_init_eapol_fp_t wpa_supplicant_init_eapol;
extern wpa_supplicant_init_wpa_fp_t wpa_supplicant_init_wpa;


/*
   Interface Initialization: WPAS GLUE
 */
void wpa_glue_func_init(void);


#if 0
int wpa_supplicant_init_eapol(struct wpa_supplicant *wpa_s);
int wpa_supplicant_init_wpa(struct wpa_supplicant *wpa_s);
void wpa_supplicant_rsn_supp_set_config(struct wpa_supplicant *wpa_s, struct wpa_ssid *ssid);
#endif

#endif /* WPAS_GLUE_H */

