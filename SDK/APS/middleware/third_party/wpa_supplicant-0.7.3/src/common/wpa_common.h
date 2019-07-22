/*
 * WPA definitions shared between hostapd and wpa_supplicant
 * Copyright (c) 2002-2008, Jouni Malinen <j@w1.fi>
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

#ifndef WPA_COMMON_H
#define WPA_COMMON_H

#include "defs.h"

#define WPA_MAX_SSID_LEN 32

/* IEEE 802.11i */
#define PMKID_LEN 16
#define PMK_LEN 32
#define WPA_REPLAY_COUNTER_LEN 8
#define WPA_NONCE_LEN 32
#define WPA_KEY_RSC_LEN 8
#define WPA_GMK_LEN 32
#define WPA_GTK_MAX_LEN 32

#define WPA_SELECTOR_LEN 4
#define WPA_VERSION 1
#define RSN_SELECTOR_LEN 4
#define RSN_VERSION 1

#define RSN_SELECTOR(a, b, c, d) \
	((((u32) (a)) << 24) | (((u32) (b)) << 16) | (((u32) (c)) << 8) | \
	 (u32) (d))

#define WPA_AUTH_KEY_MGMT_NONE RSN_SELECTOR(0x00, 0x50, 0xf2, 0)
#define WPA_AUTH_KEY_MGMT_UNSPEC_802_1X RSN_SELECTOR(0x00, 0x50, 0xf2, 1)
#define WPA_AUTH_KEY_MGMT_PSK_OVER_802_1X RSN_SELECTOR(0x00, 0x50, 0xf2, 2)
#define WPA_CIPHER_SUITE_NONE RSN_SELECTOR(0x00, 0x50, 0xf2, 0)
#define WPA_CIPHER_SUITE_WEP40 RSN_SELECTOR(0x00, 0x50, 0xf2, 1)
#define WPA_CIPHER_SUITE_TKIP RSN_SELECTOR(0x00, 0x50, 0xf2, 2)
#if 0
#define WPA_CIPHER_SUITE_WRAP RSN_SELECTOR(0x00, 0x50, 0xf2, 3)
#endif
#define WPA_CIPHER_SUITE_CCMP RSN_SELECTOR(0x00, 0x50, 0xf2, 4)
#define WPA_CIPHER_SUITE_WEP104 RSN_SELECTOR(0x00, 0x50, 0xf2, 5)


#define RSN_AUTH_KEY_MGMT_UNSPEC_802_1X RSN_SELECTOR(0x00, 0x0f, 0xac, 1)
#define RSN_AUTH_KEY_MGMT_PSK_OVER_802_1X RSN_SELECTOR(0x00, 0x0f, 0xac, 2)

#define RSN_AUTH_KEY_MGMT_802_1X_SHA256 RSN_SELECTOR(0x00, 0x0f, 0xac, 5)
#define RSN_AUTH_KEY_MGMT_PSK_SHA256 RSN_SELECTOR(0x00, 0x0f, 0xac, 6)

#define RSN_CIPHER_SUITE_NONE RSN_SELECTOR(0x00, 0x0f, 0xac, 0)
#define RSN_CIPHER_SUITE_WEP40 RSN_SELECTOR(0x00, 0x0f, 0xac, 1)
#define RSN_CIPHER_SUITE_TKIP RSN_SELECTOR(0x00, 0x0f, 0xac, 2)
#if 0
#define RSN_CIPHER_SUITE_WRAP RSN_SELECTOR(0x00, 0x0f, 0xac, 3)
#endif
#define RSN_CIPHER_SUITE_CCMP RSN_SELECTOR(0x00, 0x0f, 0xac, 4)
#define RSN_CIPHER_SUITE_WEP104 RSN_SELECTOR(0x00, 0x0f, 0xac, 5)

/* EAPOL-Key Key Data Encapsulation
 * GroupKey and PeerKey require encryption, otherwise, encryption is optional.
 */
#define RSN_KEY_DATA_GROUPKEY RSN_SELECTOR(0x00, 0x0f, 0xac, 1)
#if 0
#define RSN_KEY_DATA_STAKEY RSN_SELECTOR(0x00, 0x0f, 0xac, 2)
#endif
#define RSN_KEY_DATA_MAC_ADDR RSN_SELECTOR(0x00, 0x0f, 0xac, 3)
#define RSN_KEY_DATA_PMKID RSN_SELECTOR(0x00, 0x0f, 0xac, 4)
#ifdef CONFIG_PEERKEY
#define RSN_KEY_DATA_SMK RSN_SELECTOR(0x00, 0x0f, 0xac, 5)
#define RSN_KEY_DATA_NONCE RSN_SELECTOR(0x00, 0x0f, 0xac, 6)
#define RSN_KEY_DATA_LIFETIME RSN_SELECTOR(0x00, 0x0f, 0xac, 7)
#define RSN_KEY_DATA_ERROR RSN_SELECTOR(0x00, 0x0f, 0xac, 8)
#endif /* CONFIG_PEERKEY */

#define WPA_OUI_TYPE RSN_SELECTOR(0x00, 0x50, 0xf2, 1)

#define RSN_SELECTOR_PUT(a, val) WPA_PUT_BE32((u8 *) (a), (val))
#define RSN_SELECTOR_GET(a) WPA_GET_BE32((const u8 *) (a))

#define RSN_NUM_REPLAY_COUNTERS_1 0
#define RSN_NUM_REPLAY_COUNTERS_2 1
#define RSN_NUM_REPLAY_COUNTERS_4 2
#define RSN_NUM_REPLAY_COUNTERS_16 3


#ifdef _MSC_VER
#pragma pack(push, 1)
#endif /* _MSC_VER */


/* IEEE 802.11, 7.3.2.25.3 RSN Capabilities */
#define WPA_CAPABILITY_PREAUTH BIT(0)
#define WPA_CAPABILITY_NO_PAIRWISE BIT(1)
/* B2-B3: PTKSA Replay Counter */
/* B4-B5: GTKSA Replay Counter */
#define WPA_CAPABILITY_MFPR BIT(6)
#define WPA_CAPABILITY_MFPC BIT(7)
#define WPA_CAPABILITY_PEERKEY_ENABLED BIT(9)


/* IEEE 802.11r */
#define MOBILITY_DOMAIN_ID_LEN 2
#define FT_R0KH_ID_MAX_LEN 48
#define FT_R1KH_ID_LEN 6
#define WPA_PMK_NAME_LEN 16


/* IEEE 802.11, 8.5.2 EAPOL-Key frames */
#define WPA_KEY_INFO_TYPE_MASK ((u16) (BIT(0) | BIT(1) | BIT(2)))
#define WPA_KEY_INFO_TYPE_HMAC_MD5_RC4 BIT(0)
#define WPA_KEY_INFO_TYPE_HMAC_SHA1_AES BIT(1)
#define WPA_KEY_INFO_TYPE_AES_128_CMAC 3
#define WPA_KEY_INFO_KEY_TYPE BIT(3) /* 1 = Pairwise, 0 = Group key */
/* bit4..5 is used in WPA, but is reserved in IEEE 802.11i/RSN */
#define WPA_KEY_INFO_KEY_INDEX_MASK (BIT(4) | BIT(5))
#define WPA_KEY_INFO_KEY_INDEX_SHIFT 4
#define WPA_KEY_INFO_INSTALL BIT(6) /* pairwise */
#define WPA_KEY_INFO_TXRX BIT(6) /* group */
#define WPA_KEY_INFO_ACK BIT(7)
#define WPA_KEY_INFO_MIC BIT(8)
#define WPA_KEY_INFO_SECURE BIT(9)
#define WPA_KEY_INFO_ERROR BIT(10)
#define WPA_KEY_INFO_REQUEST BIT(11)
#define WPA_KEY_INFO_ENCR_KEY_DATA BIT(12) /* IEEE 802.11i/RSN only */
#define WPA_KEY_INFO_SMK_MESSAGE BIT(13)

/*
  AP Index, just for development test
  In the future, the Index should be modified to SSID
*/
#define WPA_FIRST_SCAN_AP_IDX 0


//95 bytes
struct wpa_eapol_key {
	u8 type;
	/* Note: key_info, key_length, and key_data_length are unaligned */
	u8 key_info[2]; /* big endian */
	u8 key_length[2]; /* big endian */
	u8 replay_counter[WPA_REPLAY_COUNTER_LEN];
	u8 key_nonce[WPA_NONCE_LEN];
	u8 key_iv[16];
	u8 key_rsc[WPA_KEY_RSC_LEN];
	u8 key_id[8]; /* Reserved in IEEE 802.11i/RSN */
	u8 key_mic[16];
	u8 key_data_length[2]; /* big endian */
	/* followed by key_data_length bytes of key_data */
} STRUCT_PACKED;

/**
 * struct wpa_ptk - WPA Pairwise Transient Key
 * IEEE Std 802.11i-2004 - 8.5.1.2 Pairwise key hierarchy
 */
struct wpa_ptk {
	u8 kck[16]; /* EAPOL-Key Key Confirmation Key (KCK) */
	u8 kek[16]; /* EAPOL-Key Key Encryption Key (KEK) */
	u8 tk1[16]; /* Temporal Key 1 (TK1) */
	union {
		u8 tk2[16]; /* Temporal Key 2 (TK2) */
		struct {
			u8 tx_mic_key[8];
			u8 rx_mic_key[8];
		} auth;
	} u;
} STRUCT_PACKED;


struct wpa_gtk_data {
	enum wpa_alg alg;
	int tx, key_rsc_len, keyidx;
	u8 gtk[32];
	int gtk_len;
};

/* WPA IE version 1
 * 00-50-f2:1 (OUI:OUI type)
 * 0x01 0x00 (version; little endian)
 * (all following fields are optional:)
 * Group Suite Selector (4 octets) (default: TKIP)
 * Pairwise Suite Count (2 octets, little endian) (default: 1)
 * Pairwise Suite List (4 * n octets) (default: TKIP)
 * Authenticated Key Management Suite Count (2 octets, little endian)
 *    (default: 1)
 * Authenticated Key Management Suite List (4 * n octets)
 *    (default: unspec 802.1X)
 * WPA Capabilities (2 octets, little endian) (default: 0)
 */

struct wpa_ie_hdr {
	u8 elem_id;
	u8 len;
	u8 oui[4]; /* 24-bit OUI followed by 8-bit OUI type */
	u8 version[2]; /* little endian */
} STRUCT_PACKED;


/* 1/4: PMKID
 * 2/4: RSN IE
 * 3/4: one or two RSN IEs + GTK IE (encrypted)
 * 4/4: empty
 * 1/2: GTK IE (encrypted)
 * 2/2: empty
 */

/* RSN IE version 1
 * 0x01 0x00 (version; little endian)
 * (all following fields are optional:)
 * Group Suite Selector (4 octets) (default: CCMP)
 * Pairwise Suite Count (2 octets, little endian) (default: 1)
 * Pairwise Suite List (4 * n octets) (default: CCMP)
 * Authenticated Key Management Suite Count (2 octets, little endian)
 *    (default: 1)
 * Authenticated Key Management Suite List (4 * n octets)
 *    (default: unspec 802.1X)
 * RSN Capabilities (2 octets, little endian) (default: 0)
 * PMKID Count (2 octets) (default: 0)
 * PMKID List (16 * n octets)
 * Management Group Cipher Suite (4 octets) (default: AES-128-CMAC)
 */

struct rsn_ie_hdr {
	u8 elem_id; /* WLAN_EID_RSN */
	u8 len;
	u8 version[2]; /* little endian */
} STRUCT_PACKED;


#ifdef CONFIG_PEERKEY
enum {
	STK_MUI_4WAY_STA_AP = 1,
	STK_MUI_4WAY_STAT_STA = 2,
	STK_MUI_GTK = 3,
	STK_MUI_SMK = 4
};

enum {
	STK_ERR_STA_NR = 1,
	STK_ERR_STA_NRSN = 2,
	STK_ERR_CPHR_NS = 3,
	STK_ERR_NO_STSL = 4
};
#endif /* CONFIG_PEERKEY */

struct rsn_error_kde {
	be16 mui;
	be16 error_type;
} STRUCT_PACKED;

#ifdef _MSC_VER
#pragma pack(pop)
#endif /* _MSC_VER */

struct wpa_ie_data {
	int proto;
	int pairwise_cipher;
	int group_cipher;
	int key_mgmt;
	int capabilities;
	size_t num_pmkid;
	const u8 *pmkid;
	int mgmt_group_cipher;
};


/*
  Scan Mode
    #0: Active Scan
    #1: Passive Scan
    #2: MIX Scan (Combine Active and Passive)
*/
enum wifi_scan_mode
{
  SCAN_MODE_ACTIVE = 0,
  SCAN_MODE_PASSIVE,
  SCAN_MODE_MIX,
  SCAN_MODE_NUM,
};

const char * wpa_cipher_txt(int cipher);
const char * wpa_key_mgmt_txt(int key_mgmt, int proto);

typedef int (*wpa_eapol_key_mic_fp_t)(const u8 *key, int ver, const u8 *buf, size_t len, u8 *mic);
typedef void (*wpa_pmk_to_ptk_fp_t)(const u8 *pmk, size_t pmk_len, const char *label, const u8 *addr1, const u8 *addr2, const u8 *nonce1, const u8 *nonce2, u8 *ptk, size_t ptk_len, int use_sha256);
typedef int (*wpa_parse_wpa_ie_rsn_fp_t)(const u8 *rsn_ie, size_t rsn_ie_len, struct wpa_ie_data *data);
typedef void (*rsn_pmkid_fp_t)(const u8 *pmk, size_t pmk_len, const u8 *aa, const u8 *spa, u8 *pmkid, int use_sha256);
typedef int (*wpa_compare_rsn_ie_fp_t)(int ft_initial_assoc, const u8 *ie1, size_t ie1len, const u8 *ie2, size_t ie2len);
#ifndef CONFIG_NO_WPA2
typedef int (*rsn_selector_to_bitfield_fp_t)(const u8 *s);
typedef int (*rsn_key_mgmt_to_bitfield_fp_t)(const u8 *s);
#endif
typedef int (*wpa_cipher_valid_pairwise_fp_t)(int cipher);
typedef int (*wpa_cipher_valid_group_fp_t)(int cipher);
    
extern wpa_eapol_key_mic_fp_t wpa_eapol_key_mic;
extern wpa_pmk_to_ptk_fp_t wpa_pmk_to_ptk;
extern wpa_parse_wpa_ie_rsn_fp_t wpa_parse_wpa_ie_rsn;
extern rsn_pmkid_fp_t rsn_pmkid;
extern wpa_compare_rsn_ie_fp_t wpa_compare_rsn_ie;
#ifndef CONFIG_NO_WPA2
extern rsn_selector_to_bitfield_fp_t rsn_selector_to_bitfield;
extern rsn_key_mgmt_to_bitfield_fp_t rsn_key_mgmt_to_bitfield;
#endif
extern wpa_cipher_valid_pairwise_fp_t wpa_cipher_valid_pairwise;
extern wpa_cipher_valid_group_fp_t wpa_cipher_valid_group;

/*
   Interface Initialization: WPA COMMON
 */
void wpa_common_func_init(void);


#if 0
/*
   Command Type of the communication between Link Layer Task and Supplicant Message Task
 */
 enum mlme_message_type
 {
   MLME_CMD_CONNECT = 0,
   MLME_CMD_DISCONNECT,
   MLME_CMD_SCAN,
   MLME_CMD_SCAN_ABORT,
   MLME_CMD_SCAN_RESULTS,
   MLME_CMD_GET_BSSID,
   MLME_CMD_GET_SSID,
   MLME_CMD_TX_STATUS,
   MLME_CMD_NUM
 };

 /*
   Event Type of the communication between Link Layer Task and Supplicant Message Task
 */
 enum mlme_event_type
 {
   MLME_EVT_ASSOC = 0,       //Association completed
   MLME_EVT_DISASSOC,        //Association lost
   MLME_EVT_SCAN_RESULTS,    //Scan results available
   MLME_EVT_ASSOCINFO,       //Report optional extra information for association
   MLME_EVT_AUTH,            //Authentication result
   MLME_EVT_DEAUTH,          //Authentication lost
   MLME_EVT_ASSOC_REJECT,    //Association rejected
   MLME_EVT_AUTH_TIMED_OUT,  //Authentication timed out
   MLME_EVT_ASSOC_TIMED_OUT, //Association timed out
   MLME_EVT_TX_STATUS,       //Report TX status
   MLME_EVT_RX_MGMT,         //Report RX of a management frame
   MLME_EVT_SIGNAL_CHANGE,   //Indicate change in signal strength
   MLME_EVT_EAPOL_RX,        //Report received EAPOL frame
   MLME_EVT_NUM
 };

int wpa_eapol_key_mic(const u8 *key, int ver, const u8 *buf, size_t len, u8 *mic);
void wpa_pmk_to_ptk(const u8 *pmk, size_t pmk_len, const char *label,
		    const u8 *addr1, const u8 *addr2,
		    const u8 *nonce1, const u8 *nonce2,
		    u8 *ptk, size_t ptk_len, int use_sha256);
int wpa_parse_wpa_ie_rsn(const u8 *rsn_ie, size_t rsn_ie_len, struct wpa_ie_data *data);
void rsn_pmkid(const u8 *pmk, size_t pmk_len, const u8 *aa, const u8 *spa, u8 *pmkid, int use_sha256);
int wpa_compare_rsn_ie(int ft_initial_assoc, const u8 *ie1, size_t ie1len, const u8 *ie2, size_t ie2len);
int wpa_insert_pmkid(u8 *ies, size_t ies_len, const u8 *pmkid);
#endif


#endif /* WPA_COMMON_H */

