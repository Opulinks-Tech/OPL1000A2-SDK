/*
 * wpa_supplicant - WPA definitions
 * Copyright (c) 2003-2007, Jouni Malinen <j@w1.fi>
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

#ifndef WPA_H
#define WPA_H

#include "defs.h"
#include "eapol_common.h"
#include "wpa_common.h"

struct wpa_sm;
struct eapol_sm;
struct wpa_config_blob;
struct wpa_eapol_ie_parse;

struct wpa_sm_ctx {
	void *ctx; /* pointer to arbitrary upper level context */
	void *msg_ctx; /* upper level context for wpa_msg() calls */

	void (*set_state)(void *ctx, enum wpa_states state);
	enum wpa_states (*get_state)(void *ctx);
	void (*deauthenticate)(void * ctx, int reason_code);
	void (*disassociate)(void *ctx, int reason_code);
	int (*set_key)(void *ctx, enum wpa_alg alg,
		       const u8 *addr, int key_idx, int set_tx,
		       const u8 *seq, size_t seq_len,
		       const u8 *key, size_t key_len);
	void * (*get_network_ctx)(void *ctx);
	int (*get_bssid)(void *ctx, u8 *bssid);
	int (*ether_send)(void *ctx, const u8 *dest, u16 proto, const u8 *buf,
			  size_t len);
	int (*get_beacon_ie)(void *ctx);
	void (*cancel_auth_timeout)(void *ctx);
	u8 * (*alloc_eapol)(void *ctx, u8 type, const void *data, u16 data_len,
			    size_t *msg_len, void **data_pos);
	int (*add_pmkid)(void *ctx, const u8 *bssid, const u8 *pmkid);
	int (*remove_pmkid)(void *ctx, const u8 *bssid, const u8 *pmkid);
	void (*set_config_blob)(void *ctx, struct wpa_config_blob *blob);
	const struct wpa_config_blob * (*get_config_blob)(void *ctx,
							  const char *name);
	int (*mlme_setprotection)(void *ctx, const u8 *addr,
				  int protection_type, int key_type);
	int (*update_ft_ies)(void *ctx, const u8 *md, const u8 *ies,
			     size_t ies_len);
	int (*send_ft_action)(void *ctx, u8 action, const u8 *target_ap,
			      const u8 *ies, size_t ies_len);
	int (*mark_authenticated)(void *ctx, const u8 *target_ap);
};

enum wpa_sm_conf_params {
	RSNA_PMK_LIFETIME /* dot11RSNAConfigPMKLifetime */,
	RSNA_PMK_REAUTH_THRESHOLD /* dot11RSNAConfigPMKReauthThreshold */,
	RSNA_SA_TIMEOUT /* dot11RSNAConfigSATimeout */,
	WPA_PARAM_PROTO,
	WPA_PARAM_PAIRWISE,
	WPA_PARAM_GROUP,
	WPA_PARAM_KEY_MGMT,
	WPA_PARAM_MGMT_GROUP,
	WPA_PARAM_RSN_ENABLED,
	WPA_PARAM_MFP
};

struct rsn_supp_config {
	void *network_ctx;
	int peerkey_enabled;
	int allowed_pairwise_cipher; /* bitfield of WPA_CIPHER_* */
	int proactive_key_caching;
	int eap_workaround;
	void *eap_conf_ctx;
	const u8 *ssid;
	size_t ssid_len;
	int wpa_ptk_rekey;
};

#ifdef CONFIG_PEERKEY
int wpa_sm_stkstart(struct wpa_sm *sm, const u8 *peer);
#else /* CONFIG_PEERKEY */
static inline int wpa_sm_stkstart(struct wpa_sm *sm, const u8 *peer)
{
	return -1;
}
#endif /* CONFIG_PEERKEY */


static inline int
wpa_sm_set_ft_params(struct wpa_sm *sm, const u8 *ies, size_t ies_len)
{
	return 0;
}

static inline int wpa_ft_prepare_auth_request(struct wpa_sm *sm,
					      const u8 *mdie)
{
	return 0;
}

static inline int
wpa_ft_process_response(struct wpa_sm *sm, const u8 *ies, size_t ies_len,
			int ft_action, const u8 *target_ap)
{
	return 0;
}

static inline int wpa_ft_is_completed(struct wpa_sm *sm)
{
	return 0;
}

static inline int
wpa_ft_validate_reassoc_resp(struct wpa_sm *sm, const u8 *ies, size_t ies_len,
			     const u8 *src_addr)
{
	return -1;
}

typedef void (*wpa_sm_deinit_fp_t)(struct wpa_sm *sm);
typedef void (*wpa_sm_notify_assoc_fp_t)(struct wpa_sm *sm, const u8 *bssid);
typedef void (*wpa_sm_notify_disassoc_fp_t)(struct wpa_sm *sm);
typedef void (*wpa_sm_set_pmk_fp_t)(struct wpa_sm *sm, const u8 *pmk, size_t pmk_len);
typedef void (*wpa_sm_set_pmk_from_pmksa_fp_t)(struct wpa_sm *sm);
typedef void (*wpa_sm_set_config_fp_t)(struct wpa_sm *sm, struct rsn_supp_config *config);
typedef void (*wpa_sm_set_own_addr_fp_t)(struct wpa_sm *sm, const u8 *addr);
typedef void (*wpa_sm_set_eapol_fp_t)(struct wpa_sm *sm, struct eapol_sm *eapol);
typedef int (*wpa_sm_set_assoc_wpa_ie_fp_t)(struct wpa_sm *sm, const u8 *ie, size_t len);
typedef int (*wpa_sm_set_assoc_wpa_ie_default_fp_t)(struct wpa_sm *sm, u8 *wpa_ie, size_t *wpa_ie_len);
typedef int (*wpa_sm_set_ap_wpa_ie_fp_t)(struct wpa_sm *sm, const u8 *ie, size_t len);
typedef int (*wpa_sm_set_ap_rsn_ie_fp_t)(struct wpa_sm *sm, const u8 *ie, size_t len);
typedef int (*wpa_sm_get_mib_fp_t)(struct wpa_sm *sm, char *buf, size_t buflen);
typedef int (*wpa_sm_set_param_fp_t)(struct wpa_sm *sm, enum wpa_sm_conf_params param, unsigned int value);
typedef unsigned int (*wpa_sm_get_param_fp_t)(struct wpa_sm *sm, enum wpa_sm_conf_params param);
typedef int (*wpa_sm_get_status_fp_t)(struct wpa_sm *sm, char *buf, size_t buflen, int verbose);
typedef void (*wpa_sm_key_request_fp_t)(struct wpa_sm *sm, int error, int pairwise);
//typedef int (*wpa_parse_wpa_ie_fp_t)(const u8 *wpa_ie, size_t wpa_ie_len, struct wpa_ie_data *data);
typedef int (*wpa_sm_rx_eapol_fp_t)(struct wpa_sm *sm, const u8 *src_addr, const u8 *buf, size_t len);
typedef int (*wpa_sm_parse_own_wpa_ie_fp_t)(struct wpa_sm *sm, struct wpa_ie_data *data);
typedef void (*wpa_sm_drop_sa_fp_t)(struct wpa_sm *sm);
typedef int (*wpa_sm_has_ptk_fp_t)(struct wpa_sm *sm);
typedef void (*wpa_clr_key_info_fp_t)(void);
typedef int (*wpa_supplicant_get_pmk_fp_t)(struct wpa_sm *sm, const unsigned char *src_addr, const u8 *pmkid);
typedef int (*wpa_derive_ptk_fp_t)(struct wpa_sm *sm, const unsigned char *src_addr, const struct wpa_eapol_key *key, struct wpa_ptk *ptk);
typedef void (*wpa_supplicant_process_1_of_4_fp_t)(struct wpa_sm *sm, const unsigned char *src_addr, const struct wpa_eapol_key *key, u16 ver);
typedef void (*wpa_sm_start_preauth_fp_t)(void *eloop_ctx, void *timeout_ctx);
typedef void (*wpa_supplicant_key_neg_complete_fp_t)(struct wpa_sm *sm, const u8 *addr, int secure);
typedef void (*wpa_sm_rekey_ptk_fp_t)(void *eloop_ctx, void *timeout_ctx);
typedef int (*wpa_supplicant_install_ptk_fp_t)(struct wpa_sm *sm, const struct wpa_eapol_key *key);
typedef int (*wpa_supplicant_check_group_cipher_fp_t)(int group_cipher, int keylen, int maxkeylen, int *key_rsc_len, enum wpa_alg *alg);
typedef int (*wpa_supplicant_install_gtk_fp_t)(struct wpa_sm *sm, const struct wpa_gtk_data *gd, const u8 *key_rsc);
typedef int (*wpa_supplicant_gtk_tx_bit_workaround_fp_t)(const struct wpa_sm *sm, int tx);
typedef int (*wpa_supplicant_pairwise_gtk_fp_t)(struct wpa_sm *sm, const struct wpa_eapol_key *key, const u8 *gtk, size_t gtk_len, int key_info);
typedef void (*wpa_report_ie_mismatch_fp_t)(struct wpa_sm *sm, const char *reason, const u8 *src_addr, const u8 *wpa_ie, size_t wpa_ie_len, const u8 *rsn_ie, size_t rsn_ie_len);
typedef int (*wpa_supplicant_validate_ie_fp_t)(struct wpa_sm *sm, unsigned char *src_addr, struct wpa_eapol_ie_parse *ie);
typedef void (*wpa_supplicant_process_3_of_4_fp_t)(struct wpa_sm *sm, const struct wpa_eapol_key *key, u16 ver);
typedef int (*wpa_supplicant_process_1_of_2_rsn_fp_t)(struct wpa_sm *sm, const u8 *keydata, size_t keydatalen, u16 key_info, struct wpa_gtk_data *gd);
typedef int (*wpa_supplicant_process_1_of_2_wpa_fp_t)(struct wpa_sm *sm, const struct wpa_eapol_key *key, size_t keydatalen, int key_info, size_t extra_len, u16 ver, struct wpa_gtk_data *gd);
typedef int (*wpa_supplicant_send_2_of_2_fp_t)(struct wpa_sm *sm, const struct wpa_eapol_key *key, int ver, u16 key_info);
typedef void (*wpa_supplicant_process_1_of_2_fp_t)(struct wpa_sm *sm, const unsigned char *src_addr, const struct wpa_eapol_key *key, int extra_len, u16 ver);
typedef int (*wpa_supplicant_verify_eapol_key_mic_fp_t)(struct wpa_sm *sm, struct wpa_eapol_key *key, u16 ver, const u8 *buf, size_t len);
typedef int (*wpa_supplicant_decrypt_key_data_fp_t)(struct wpa_sm *sm, struct wpa_eapol_key *key, u16 ver);
typedef void (*wpa_eapol_key_dump_fp_t)(const struct wpa_eapol_key *key);
typedef int (*wpa_ether_send_fp_t)(const u8 *dest, u16 proto, u8 *buf, size_t len);
typedef void (*wpa_eapol_key_send_fp_t)(struct wpa_sm *sm, const u8 *kck, int ver, const u8 *dest, u16 proto, u8 *msg, size_t msg_len, u8 *key_mic);
typedef u8 * (*wpa_alloc_eapol_fp_t)(u8 ver, u8 type, void *data, u16 data_len, size_t *msg_len, void **data_pos);
typedef int (*wpa_supplicant_send_2_of_4_fp_t)(struct wpa_sm *sm, const unsigned char *dst, const struct wpa_eapol_key *key, int ver, const u8 *nonce, const u8 *wpa_ie, size_t wpa_ie_len, struct wpa_ptk *ptk);
typedef int (*wpa_supplicant_send_4_of_4_fp_t)(struct wpa_sm *sm, const unsigned char *dst, const struct wpa_eapol_key *key, u16 ver, u16 key_info, const u8 *kde, size_t kde_len, struct wpa_ptk *ptk);

extern wpa_sm_deinit_fp_t wpa_sm_deinit;
extern wpa_sm_notify_assoc_fp_t wpa_sm_notify_assoc;
extern wpa_sm_notify_disassoc_fp_t wpa_sm_notify_disassoc;
extern wpa_sm_set_pmk_fp_t wpa_sm_set_pmk;
extern wpa_sm_set_pmk_from_pmksa_fp_t wpa_sm_set_pmk_from_pmksa;
extern wpa_sm_set_config_fp_t wpa_sm_set_config;
extern wpa_sm_set_own_addr_fp_t wpa_sm_set_own_addr;
extern wpa_sm_set_eapol_fp_t wpa_sm_set_eapol;
extern wpa_sm_set_assoc_wpa_ie_fp_t wpa_sm_set_assoc_wpa_ie;
extern wpa_sm_set_assoc_wpa_ie_default_fp_t wpa_sm_set_assoc_wpa_ie_default;
extern wpa_sm_set_ap_wpa_ie_fp_t wpa_sm_set_ap_wpa_ie;
extern wpa_sm_set_ap_rsn_ie_fp_t wpa_sm_set_ap_rsn_ie;
extern wpa_sm_get_mib_fp_t wpa_sm_get_mib;
extern wpa_sm_set_param_fp_t wpa_sm_set_param;
extern wpa_sm_get_param_fp_t wpa_sm_get_param;
extern wpa_sm_get_status_fp_t wpa_sm_get_status;
extern wpa_sm_key_request_fp_t wpa_sm_key_request;
//extern wpa_parse_wpa_ie_fp_t wpa_parse_wpa_ie;
extern wpa_sm_rx_eapol_fp_t wpa_sm_rx_eapol;
extern wpa_sm_parse_own_wpa_ie_fp_t wpa_sm_parse_own_wpa_ie;
extern wpa_sm_drop_sa_fp_t wpa_sm_drop_sa;
extern wpa_sm_has_ptk_fp_t wpa_sm_has_ptk;
extern wpa_clr_key_info_fp_t wpa_clr_key_info;
extern wpa_supplicant_get_pmk_fp_t wpa_supplicant_get_pmk;
extern wpa_derive_ptk_fp_t wpa_derive_ptk;
extern wpa_supplicant_process_1_of_4_fp_t wpa_supplicant_process_1_of_4;
extern wpa_sm_start_preauth_fp_t wpa_sm_start_preauth;
extern wpa_supplicant_key_neg_complete_fp_t wpa_supplicant_key_neg_complete;
extern wpa_sm_rekey_ptk_fp_t wpa_sm_rekey_ptk;
extern wpa_supplicant_install_ptk_fp_t wpa_supplicant_install_ptk;
extern wpa_supplicant_check_group_cipher_fp_t wpa_supplicant_check_group_cipher;
extern wpa_supplicant_install_gtk_fp_t wpa_supplicant_install_gtk;
extern wpa_supplicant_gtk_tx_bit_workaround_fp_t wpa_supplicant_gtk_tx_bit_workaround;
extern wpa_supplicant_pairwise_gtk_fp_t wpa_supplicant_pairwise_gtk;
extern wpa_report_ie_mismatch_fp_t wpa_report_ie_mismatch;
//extern wpa_supplicant_validate_ie_fp_t wpa_supplicant_validate_ie;
extern wpa_supplicant_validate_ie_fp_t wpa_supplicant_validate_ie;
extern wpa_supplicant_process_3_of_4_fp_t wpa_supplicant_process_3_of_4;
extern wpa_supplicant_process_1_of_2_rsn_fp_t wpa_supplicant_process_1_of_2_rsn;
extern wpa_supplicant_process_1_of_2_wpa_fp_t wpa_supplicant_process_1_of_2_wpa;
extern wpa_supplicant_send_2_of_2_fp_t wpa_supplicant_send_2_of_2;
extern wpa_supplicant_process_1_of_2_fp_t wpa_supplicant_process_1_of_2;
extern wpa_supplicant_verify_eapol_key_mic_fp_t wpa_supplicant_verify_eapol_key_mic;
extern wpa_supplicant_decrypt_key_data_fp_t wpa_supplicant_decrypt_key_data;
extern wpa_eapol_key_dump_fp_t wpa_eapol_key_dump;
extern wpa_ether_send_fp_t wpa_ether_send;
extern wpa_eapol_key_send_fp_t wpa_eapol_key_send;
extern wpa_alloc_eapol_fp_t wpa_alloc_eapol;
extern wpa_supplicant_send_2_of_4_fp_t wpa_supplicant_send_2_of_4;
extern wpa_supplicant_send_4_of_4_fp_t wpa_supplicant_send_4_of_4;

typedef void (*wpa_sm_set_state_fp_t)(struct wpa_sm *sm, enum wpa_states state);
typedef enum wpa_states (*wpa_sm_get_state_fp_t)(struct wpa_sm *sm);
typedef void (*wpa_sm_deauthenticate_fp_t)(struct wpa_sm *sm, int reason_code);
typedef void (*wpa_sm_disassociate_fp_t)(struct wpa_sm *sm, int reason_code);
typedef int (*wpa_sm_set_key_fp_t)(struct wpa_sm *sm, enum wpa_alg alg, const u8 *addr, int key_idx, int set_tx, const u8 *seq, size_t seq_len, const u8 *key, size_t key_len);
typedef void * (*wpa_sm_get_network_ctx_fp_t)(struct wpa_sm *sm);
typedef int (*wpa_sm_get_bssid_fp_t)(struct wpa_sm *sm, u8 *bssid);
typedef int (*wpa_sm_ether_send_fp_t)(struct wpa_sm *sm, const u8 *dest, u16 proto, const u8 *buf, size_t len);
typedef int (*wpa_sm_get_beacon_ie_fp_t)(struct wpa_sm *sm);
typedef void (*wpa_sm_cancel_auth_timeout_fp_t)(struct wpa_sm *sm);
typedef u8 * (*wpa_sm_alloc_eapol_fp_t)(struct wpa_sm *sm, u8 type, const void *data, u16 data_len, size_t *msg_len, void **data_pos);
typedef int (*wpa_sm_add_pmkid_fp_t)(struct wpa_sm *sm, const u8 *bssid, const u8 *pmkid);
typedef int (*wpa_sm_remove_pmkid_fp_t)(struct wpa_sm *sm, const u8 *bssid, const u8 *pmkid);
typedef int (*wpa_sm_mlme_setprotection_fp_t)(struct wpa_sm *sm, const u8 *addr, int protect_type, int key_type);

extern wpa_sm_set_state_fp_t wpa_sm_set_state;
extern wpa_sm_get_state_fp_t wpa_sm_get_state;
extern wpa_sm_deauthenticate_fp_t wpa_sm_deauthenticate;
extern wpa_sm_disassociate_fp_t wpa_sm_disassociate;
extern wpa_sm_set_key_fp_t wpa_sm_set_key;
extern wpa_sm_get_network_ctx_fp_t wpa_sm_get_network_ctx;
extern wpa_sm_get_bssid_fp_t wpa_sm_get_bssid;
extern wpa_sm_ether_send_fp_t wpa_sm_ether_send;
extern wpa_sm_get_beacon_ie_fp_t wpa_sm_get_beacon_ie;
extern wpa_sm_cancel_auth_timeout_fp_t wpa_sm_cancel_auth_timeout;
extern wpa_sm_alloc_eapol_fp_t wpa_sm_alloc_eapol;
extern wpa_sm_add_pmkid_fp_t wpa_sm_add_pmkid;
extern wpa_sm_remove_pmkid_fp_t wpa_sm_remove_pmkid;
extern wpa_sm_mlme_setprotection_fp_t wpa_sm_mlme_setprotection;

/*
   Interface Initialization: WPA
 */
void wpa_func_init(void);


#ifndef CONFIG_NO_WPA
struct wpa_sm * wpa_sm_init(struct wpa_sm_ctx *ctx);

#if 0
void wpa_sm_deinit(struct wpa_sm *sm);
void wpa_sm_notify_assoc(struct wpa_sm *sm, const u8 *bssid);
void wpa_sm_notify_disassoc(struct wpa_sm *sm);
void wpa_sm_set_pmk(struct wpa_sm *sm, const u8 *pmk, size_t pmk_len);
void wpa_sm_set_pmk_from_pmksa(struct wpa_sm *sm);
void wpa_sm_set_config(struct wpa_sm *sm, struct rsn_supp_config *config);
void wpa_sm_set_own_addr(struct wpa_sm *sm, const u8 *addr);
void wpa_sm_set_eapol(struct wpa_sm *sm, struct eapol_sm *eapol);
int wpa_sm_set_assoc_wpa_ie(struct wpa_sm *sm, const u8 *ie, size_t len);
int wpa_sm_set_assoc_wpa_ie_default(struct wpa_sm *sm, u8 *wpa_ie, size_t *wpa_ie_len);
int wpa_sm_set_ap_wpa_ie(struct wpa_sm *sm, const u8 *ie, size_t len);
int wpa_sm_set_ap_rsn_ie(struct wpa_sm *sm, const u8 *ie, size_t len);
int wpa_sm_get_mib(struct wpa_sm *sm, char *buf, size_t buflen);
int wpa_sm_set_param(struct wpa_sm *sm, enum wpa_sm_conf_params param, unsigned int value);
unsigned int wpa_sm_get_param(struct wpa_sm *sm, enum wpa_sm_conf_params param);
int wpa_sm_get_status(struct wpa_sm *sm, char *buf, size_t buflen, int verbose);
void wpa_sm_key_request(struct wpa_sm *sm, int error, int pairwise);
int wpa_parse_wpa_ie(const u8 *wpa_ie, size_t wpa_ie_len, struct wpa_ie_data *data);
int wpa_sm_rx_eapol(struct wpa_sm *sm, const u8 *src_addr, const u8 *buf, size_t len);
int wpa_sm_parse_own_wpa_ie(struct wpa_sm *sm, struct wpa_ie_data *data);
void wpa_sm_drop_sa(struct wpa_sm *sm);
int wpa_sm_has_ptk(struct wpa_sm *sm);
//void wpa_sm_set_fast_reauth(struct wpa_sm *sm, int fast_reauth);
//void wpa_sm_set_scard_ctx(struct wpa_sm *sm, void *scard_ctx);
//void wpa_sm_set_ifname(struct wpa_sm *sm, const char *ifname, const char *bridge_ifname);
//void wpa_sm_aborted_cached(struct wpa_sm *sm);
//int wpa_sm_pmksa_cache_list(struct wpa_sm *sm, char *buf, size_t len);
#endif //#if 0

#else /* CONFIG_NO_WPA */

#if 0
static inline struct wpa_sm * wpa_sm_init(struct wpa_sm_ctx *ctx)
{
	return (struct wpa_sm *) 1;
}

static inline void wpa_sm_deinit(struct wpa_sm *sm)
{
}

static inline void wpa_sm_notify_assoc(struct wpa_sm *sm, const u8 *bssid)
{
}

static inline void wpa_sm_notify_disassoc(struct wpa_sm *sm)
{
}

static inline void wpa_sm_set_pmk(struct wpa_sm *sm, const u8 *pmk,
				  size_t pmk_len)
{
}

static inline void wpa_sm_set_pmk_from_pmksa(struct wpa_sm *sm)
{
}

static inline void wpa_sm_set_fast_reauth(struct wpa_sm *sm, int fast_reauth)
{
}

static inline void wpa_sm_set_scard_ctx(struct wpa_sm *sm, void *scard_ctx)
{
}

static inline void wpa_sm_set_config(struct wpa_sm *sm,
				     struct rsn_supp_config *config)
{
}

static inline void wpa_sm_set_own_addr(struct wpa_sm *sm, const u8 *addr)
{
}

static inline void wpa_sm_set_ifname(struct wpa_sm *sm, const char *ifname,
				     const char *bridge_ifname)
{
}

static inline void wpa_sm_set_eapol(struct wpa_sm *sm, struct eapol_sm *eapol)
{
}

static inline int wpa_sm_set_assoc_wpa_ie(struct wpa_sm *sm, const u8 *ie,
					  size_t len)
{
	return -1;
}

static inline int wpa_sm_set_assoc_wpa_ie_default(struct wpa_sm *sm,
						  u8 *wpa_ie,
						  size_t *wpa_ie_len)
{
	return -1;
}

static inline int wpa_sm_set_ap_wpa_ie(struct wpa_sm *sm, const u8 *ie,
				       size_t len)
{
	return -1;
}

static inline int wpa_sm_set_ap_rsn_ie(struct wpa_sm *sm, const u8 *ie,
				       size_t len)
{
	return -1;
}

static inline int wpa_sm_get_mib(struct wpa_sm *sm, char *buf, size_t buflen)
{
	return 0;
}

static inline int wpa_sm_set_param(struct wpa_sm *sm,
				   enum wpa_sm_conf_params param,
				   unsigned int value)
{
	return -1;
}

static inline unsigned int wpa_sm_get_param(struct wpa_sm *sm,
					    enum wpa_sm_conf_params param)
{
	return 0;
}

static inline int wpa_sm_get_status(struct wpa_sm *sm, char *buf,
				    size_t buflen, int verbose)
{
	return 0;
}

static inline void wpa_sm_key_request(struct wpa_sm *sm, int error,
				      int pairwise)
{
}

static inline int wpa_parse_wpa_ie(const u8 *wpa_ie, size_t wpa_ie_len,
				   struct wpa_ie_data *data)
{
	return -1;
}

static inline void wpa_sm_aborted_cached(struct wpa_sm *sm)
{
}

static inline int wpa_sm_rx_eapol(struct wpa_sm *sm, const u8 *src_addr,
				  const u8 *buf, size_t len)
{
	return -1;
}

static inline int wpa_sm_parse_own_wpa_ie(struct wpa_sm *sm,
					  struct wpa_ie_data *data)
{
	return -1;
}

static inline int wpa_sm_pmksa_cache_list(struct wpa_sm *sm, char *buf,
					  size_t len)
{
	return -1;
}

static inline void wpa_sm_drop_sa(struct wpa_sm *sm)
{
}

static inline int wpa_sm_has_ptk(struct wpa_sm *sm)
{
	return 0;
}
#endif //#if 0
#endif /* CONFIG_NO_WPA */
#endif /* WPA_H */

