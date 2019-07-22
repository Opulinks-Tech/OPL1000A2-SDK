/*
 * WPA Supplicant - Client mode MLME
 * Copyright (c) 2003-2007, Jouni Malinen <j@w1.fi>
 * Copyright (c) 2004, Instant802 Networks, Inc.
 * Copyright (c) 2005-2006, Devicescape Software, Inc.
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

#ifndef MLME_H
#define MLME_H
#include "wpa_supplicant_i.h"


struct wpa_supplicant;

struct ieee80211_rx_status {
    int freq;
    int channel;
    int ssi;
};


/* Timeouts and intervals in milliseconds */
#define IEEE80211_AUTH_TIMEOUT (200)
#define IEEE80211_AUTH_MAX_TRIES 3
#define IEEE80211_ASSOC_TIMEOUT (200)
#define IEEE80211_ASSOC_MAX_TRIES 3
#define IEEE80211_MONITORING_INTERVAL (2000)
#define IEEE80211_PROBE_INTERVAL (60000)
#define IEEE80211_RETRY_AUTH_INTERVAL (1000)
#define IEEE80211_SCAN_INTERVAL (2000)
#define IEEE80211_SCAN_INTERVAL_SLOW (15000)
#define IEEE80211_IBSS_JOIN_TIMEOUT (20000)
#define IEEE80211_PROBE_DELAY (33)
#define IEEE80211_CHANNEL_TIME (33)
#define IEEE80211_PASSIVE_CHANNEL_TIME (200)
#define IEEE80211_SCAN_RESULT_EXPIRE (10000)
#define IEEE80211_IBSS_MERGE_INTERVAL (30000)
#define IEEE80211_IBSS_INACTIVITY_LIMIT (60000)
#define IEEE80211_IBSS_MAX_STA_ENTRIES 128
#define IEEE80211_FC(type, stype) host_to_le16((type << 2) | (stype << 4))


struct ieee80211_sta_bss {
	struct ieee80211_sta_bss *next;
	struct ieee80211_sta_bss *hnext;
	u8 bssid[ETH_ALEN];
	u8 ssid[MAX_SSID_LEN];
	size_t ssid_len;
	u16 capability; /* host byte order */
	int hw_mode;
	int channel;
	int freq;
	int rssi;
	u8 *ie;
	size_t ie_len;
	u8 *wpa_ie;
	size_t wpa_ie_len;
	u8 *rsn_ie;
	size_t rsn_ie_len;
	u8 *wmm_ie;
	size_t wmm_ie_len;
	u8 *mdie;
	size_t mdie_len;
#define IEEE80211_MAX_SUPP_RATES 32
	u8 supp_rates[IEEE80211_MAX_SUPP_RATES];
	size_t supp_rates_len;
	int beacon_int;
	u64 timestamp;
	int probe_resp;
	struct os_time last_update;
};


#ifdef CONFIG_CLIENT_MLME

int ieee80211_sta_init(struct wpa_supplicant *wpa_s);
void ieee80211_sta_deinit(struct wpa_supplicant *wpa_s);
int ieee80211_sta_req_scan(struct wpa_supplicant *wpa_s, struct wpa_driver_scan_params *params);
int ieee80211_sta_deauthenticate(struct wpa_supplicant *wpa_s, u16 reason);
int ieee80211_sta_disassociate(struct wpa_supplicant *wpa_s, u16 reason);
int ieee80211_sta_associate(struct wpa_supplicant *wpa_s, struct wpa_driver_associate_params *params);
int ieee80211_sta_get_ssid(struct wpa_supplicant *wpa_s, u8 *ssid, size_t *len);
void ieee80211_sta_free_hw_features(struct hostapd_hw_modes *hw_features, size_t num_hw_features);
void ieee80211_sta_rx(struct wpa_supplicant *wpa_s, const u8 *buf, size_t len, struct ieee80211_rx_status *rx_status);
struct wpa_scan_results * ieee80211_sta_get_scan_results(struct wpa_supplicant *wpa_s);
int ieee80211_sta_update_ft_ies(struct wpa_supplicant *wpa_s, const u8 *md, const u8 *ies, size_t ies_len);
int ieee80211_sta_send_ft_action(struct wpa_supplicant *wpa_s, u8 action, const u8 *target_ap, const u8 *ies, size_t ies_len);

#else /* CONFIG_CLIENT_MLME */

#if 0
static inline int ieee80211_sta_init(struct wpa_supplicant *wpa_s)
{
	return 0;
}

static inline void ieee80211_sta_deinit(struct wpa_supplicant *wpa_s)
{
}

static inline int ieee80211_sta_req_scan(struct wpa_supplicant *wpa_s,
					 struct wpa_driver_scan_params *params)
{
	return -1;
}

static inline int ieee80211_sta_deauthenticate(struct wpa_supplicant *wpa_s,
					       u16 reason)
{
	return -1;
}

static inline int ieee80211_sta_disassociate(struct wpa_supplicant *wpa_s,
					     u16 reason)
{
	return -1;
}

static inline int
ieee80211_sta_associate(struct wpa_supplicant *wpa_s,
			struct wpa_driver_associate_params *params)
{
	return -1;
}

static inline int ieee80211_sta_get_ssid(struct wpa_supplicant *wpa_s,
					 u8 *ssid, size_t *len)
{
	return -1;
}

static inline void
ieee80211_sta_free_hw_features(struct hostapd_hw_modes *hw_features,
			       size_t num_hw_features)
{
}

static inline void
ieee80211_sta_rx(struct wpa_supplicant *wpa_s, const u8 *buf, size_t len,
		 struct ieee80211_rx_status *rx_status)
{
}

static inline struct wpa_scan_results *
ieee80211_sta_get_scan_results(struct wpa_supplicant *wpa_s)
{
	return NULL;
}

static inline int
ieee80211_sta_update_ft_ies(struct wpa_supplicant *wpa_s, const u8 *md,
			    const u8 *ies, size_t ies_len)
{
	return -1;
}

static inline int
ieee80211_sta_send_ft_action(struct wpa_supplicant *wpa_s, u8 action,
			     const u8 *target_ap,
			     const u8 *ies, size_t ies_len)
{
	return -1;
}
#endif //#if 0

#endif /* CONFIG_CLIENT_MLME */

#endif /* MLME_H */
