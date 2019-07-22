/*
 * Internal WPA/RSN supplicant state machine definitions
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

#ifndef WPA_I_H
#define WPA_I_H

#include "..\utils\list.h"

struct wpa_peerkey;
struct wpa_eapol_key;

/**
 * struct wpa_sm - Internal WPA state machine data
 */
struct wpa_sm {
	u8 pmk[PMK_LEN];
	size_t pmk_len;
	struct wpa_ptk ptk, tptk;
	int ptk_set, tptk_set;
	u8 snonce[WPA_NONCE_LEN];
	u8 anonce[WPA_NONCE_LEN];                   /* ANonce from the last 1/4 msg */
	int renew_snonce;
	u8 rx_replay_counter[WPA_REPLAY_COUNTER_LEN];
	int rx_replay_counter_set;
	u8 request_counter[WPA_REPLAY_COUNTER_LEN];
	struct eapol_sm *eapol;                     /* EAPOL state machine from upper level code */
	struct rsn_pmksa_cache *pmksa;              /* PMKSA cache */
	struct rsn_pmksa_cache_entry *cur_pmksa;    /* current PMKSA entry */
	struct dl_list pmksa_candidates;
	struct l2_packet_data *l2_preauth;
	struct l2_packet_data *l2_preauth_br;
	u8 preauth_bssid[ETH_ALEN];                 /* current RSN pre-auth peer or 00:00:00:00:00:00 if no pre-auth is in progress */
	struct eapol_sm *preauth_eapol;
	struct wpa_sm_ctx *ctx;
	void *network_ctx;
	int peerkey_enabled;
	int allowed_pairwise_cipher;                /* bitfield of WPA_CIPHER_* */
	int proactive_key_caching;
	int eap_workaround;
	void *eap_conf_ctx;
	u8 ssid[32];
	size_t ssid_len;
	int wpa_ptk_rekey;
	u8 own_addr[ETH_ALEN];
	const char *ifname;
	u8 bssid[ETH_ALEN];

	unsigned int dot11RSNAConfigPMKLifetime;
	unsigned int dot11RSNAConfigPMKReauthThreshold;
	unsigned int dot11RSNAConfigSATimeout;
	unsigned int dot11RSNA4WayHandshakeFailures;

	/* Selected configuration (based on Beacon/ProbeResp WPA IE) */
	unsigned int proto;
	unsigned int pairwise_cipher;
	unsigned int group_cipher;
	unsigned int key_mgmt;
	unsigned int mgmt_group_cipher;

	int rsn_enabled;    /* Whether RSN is enabled in configuration */
	int mfp;            /* 0 = disabled, 1 = optional, 2 = mandatory */
	u8 *assoc_wpa_ie;   /* Own WPA/RSN IE from (Re)AssocReq */
	size_t assoc_wpa_ie_len;
	u8 *ap_wpa_ie, *ap_rsn_ie;
	size_t ap_wpa_ie_len, ap_rsn_ie_len;

#ifdef CONFIG_PEERKEY
	struct wpa_peerkey *peerkey;
#endif /* CONFIG_PEERKEY */

    //const char *bridge_ifname;
    //void *scard_ctx; /* context for smartcard callbacks */
	//int fast_reauth; /* whether EAP fast re-authentication is enabled */
};

#if 0
u8 * wpa_alloc_eapol(u8 ver, u8 type, void *data, u16 data_len, size_t *msg_len, void **data_pos);
void wpa_eapol_key_send(struct wpa_sm *sm, const u8 *kck,
			int ver, const u8 *dest, u16 proto,
			u8 *msg, size_t msg_len, u8 *key_mic);
int wpa_ether_send( const u8 *dest, u16 proto, u8 *buf, size_t len);
int wpa_supplicant_send_2_of_4(struct wpa_sm *sm, const unsigned char *dst,
			       const struct wpa_eapol_key *key,
			       int ver, const u8 *nonce,
			       const u8 *wpa_ie, size_t wpa_ie_len,
			       struct wpa_ptk *ptk);
int wpa_supplicant_send_4_of_4(struct wpa_sm *sm, const unsigned char *dst,
			       const struct wpa_eapol_key *key,
			       u16 ver, u16 key_info,
			       const u8 *kde, size_t kde_len,
			       struct wpa_ptk *ptk);
int wpa_derive_ptk_ft(struct wpa_sm *sm, const unsigned char *src_addr,
		      const struct wpa_eapol_key *key,
		      struct wpa_ptk *ptk, size_t ptk_len);
#endif //#if 0

#endif /* WPA_I_H */

