/*
 * wpa_supplicant - WPA/RSN IE and KDE definitions
 * Copyright (c) 2004-2007, Jouni Malinen <j@w1.fi>
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

#ifndef WPA_IE_H
#define WPA_IE_H

struct wpa_eapol_ie_parse {
	const u8 *wpa_ie;
	size_t wpa_ie_len;
	const u8 *rsn_ie;
	size_t rsn_ie_len;
	const u8 *pmkid;
	const u8 *gtk;
	size_t gtk_len;
	const u8 *mac_addr;
	size_t mac_addr_len;
#ifdef CONFIG_PEERKEY
	const u8 *smk;
	size_t smk_len;
	const u8 *nonce;
	size_t nonce_len;
	const u8 *lifetime;
	size_t lifetime_len;
	const u8 *error;
	size_t error_len;
#endif /* CONFIG_PEERKEY */
#ifdef CONFIG_IEEE80211W
	const u8 *igtk;
	size_t igtk_len;
#endif /* CONFIG_IEEE80211W */
};

typedef int (*wpa_supplicant_parse_ies_fp_t)(const u8 *buf, size_t len, struct wpa_eapol_ie_parse *ie);
typedef int (*wpa_gen_wpa_ie_fp_t)(struct wpa_sm *sm, u8 *wpa_ie, size_t wpa_ie_len);
typedef int (*wpa_parse_wpa_ie_wpa_fp_t)(const u8 *wpa_ie, size_t wpa_ie_len, struct wpa_ie_data *data);
typedef int (*wpa_parse_wpa_ie_fp_t)(const u8 *wpa_ie, size_t wpa_ie_len, struct wpa_ie_data *data);
typedef int (*wpa_gen_wpa_ie_wpa_fp_t)(u8 *wpa_ie, size_t wpa_ie_len, int pairwise_cipher, int group_cipher, int key_mgmt);
typedef int (*wpa_gen_wpa_ie_rsn_fp_t)(u8 *rsn_ie, size_t rsn_ie_len, int pairwise_cipher, int group_cipher, int key_mgmt, int mgmt_group_cipher, struct wpa_sm *sm);
typedef int (*wpa_parse_generic_fp_t)(const u8 *pos, const u8 *end, struct wpa_eapol_ie_parse *ie);

extern wpa_supplicant_parse_ies_fp_t wpa_supplicant_parse_ies;
extern wpa_gen_wpa_ie_fp_t wpa_gen_wpa_ie;
extern wpa_parse_wpa_ie_wpa_fp_t wpa_parse_wpa_ie_wpa;
extern wpa_parse_wpa_ie_fp_t wpa_parse_wpa_ie;
extern wpa_gen_wpa_ie_wpa_fp_t wpa_gen_wpa_ie_wpa;
extern wpa_gen_wpa_ie_rsn_fp_t wpa_gen_wpa_ie_rsn;
extern wpa_parse_generic_fp_t wpa_parse_generic;

/*
   Interface Initialization: WPA IE
 */
void wpa_ie_func_init(void);


#if 0
int wpa_supplicant_parse_ies(const u8 *buf, size_t len, struct wpa_eapol_ie_parse *ie);
int wpa_gen_wpa_ie(struct wpa_sm *sm, u8 *wpa_ie, size_t wpa_ie_len);
#endif //#if 0
#endif /* WPA_IE_H */

