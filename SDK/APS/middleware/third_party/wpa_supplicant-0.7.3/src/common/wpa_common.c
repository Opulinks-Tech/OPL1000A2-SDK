/*
 * WPA/RSN - Shared functions for supplicant and authenticator
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
#include "includes.h"
#include "common.h"
#include "md5.h"
#include "sha1.h"
#include "sha256.h"
#include "aes_wrap.h"
#include "crypto.h"
#include "ieee802_11_defs.h"
#include "wifi_defs.h" //I0060 0000905
#include "wpa_common.h"
#include "scrt.h"


/**
 * wpa_eapol_key_mic - Calculate EAPOL-Key MIC
 * @key: EAPOL-Key Key Confirmation Key (KCK)
 * @ver: Key descriptor version (WPA_KEY_INFO_TYPE_*)
 * @buf: Pointer to the beginning of the EAPOL header (version field)
 * @len: Length of the EAPOL frame (from EAPOL header to the end of the frame)
 * @mic: Pointer to the buffer to which the EAPOL-Key MIC is written
 * Returns: 0 on success, -1 on failure
 *
 * Calculate EAPOL-Key MIC for an EAPOL-Key packet. The EAPOL-Key MIC field has
 * to be cleared (all zeroes) when calling this function.
 *
 * Note: 'IEEE Std 802.11i-2004 - 8.5.2 EAPOL-Key frames' has an error in the
 * description of the Key MIC calculation. It includes packet data from the
 * beginning of the EAPOL-Key header, not EAPOL header. This incorrect change
 * happened during final editing of the standard and the correct behavior is
 * defined in the last draft (IEEE 802.11i/D10).
 */
int wpa_eapol_key_mic_impl(const u8 *key, int ver, const u8 *buf, size_t len,
		      u8 *mic)
{
	u8 hash[SHA1_MAC_LEN];
    bool ret;

    wpa_printf(MSG_DEBUG, "[KEY]WPA: MIC Caculation, ver:%d len:%d \r\n", ver, len);

	switch (ver) {
	case WPA_KEY_INFO_TYPE_HMAC_MD5_RC4:
        //wpa_printf(MSG_INFO, "\r\n wpa_eapol_key_mic, case WPA_KEY_INFO_TYPE_HMAC_MD5_RC4 \r\n");
		return hmac_md5(key, 16, buf, len, mic);
	case WPA_KEY_INFO_TYPE_HMAC_SHA1_AES:
        //wpa_printf(MSG_INFO, "\r\n wpa_eapol_key_mic, case WPA_KEY_INFO_TYPE_HMAC_SHA1_AES \r\n");
#if 0
		if (hmac_sha1(key, 16, buf, len, hash))
			return -1;
#endif
        wpa_printf(MSG_DEBUG, "[KEY]WPA: call hw api nl_hmac_sha_1 \r\n");

        //ret = nl_hmac_sha_1(key, 16, buf, len, hash);
        ret = nl_hmac_sha_1((uint8_t *) key, 16, (uint8_t *) buf, (int) len, (uint8_t *) hash);

        if(ret == false) return -1;

		os_memcpy(mic, hash, MD5_MAC_LEN);
		break;
	default:
		return -1;
	}

	return 0;
}


/**
 * wpa_pmk_to_ptk - Calculate PTK from PMK, addresses, and nonces
 * @pmk: Pairwise master key
 * @pmk_len: Length of PMK
 * @label: Label to use in derivation
 * @addr1: AA or SA
 * @addr2: SA or AA
 * @nonce1: ANonce or SNonce
 * @nonce2: SNonce or ANonce
 * @ptk: Buffer for pairwise transient key
 * @ptk_len: Length of PTK
 * @use_sha256: Whether to use SHA256-based KDF
 *
 * IEEE Std 802.11i-2004 - 8.5.1.2 Pairwise key hierarchy
 * PTK = PRF-X(PMK, "Pairwise key expansion",
 *             Min(AA, SA) || Max(AA, SA) ||
 *             Min(ANonce, SNonce) || Max(ANonce, SNonce))
 *
 * STK = PRF-X(SMK, "Peer key expansion",
 *             Min(MAC_I, MAC_P) || Max(MAC_I, MAC_P) ||
 *             Min(INonce, PNonce) || Max(INonce, PNonce))
 */
void wpa_pmk_to_ptk_impl(const u8 *pmk, size_t pmk_len, const char *label,
		    const u8 *addr1, const u8 *addr2,
		    const u8 *nonce1, const u8 *nonce2,
		    u8 *ptk, size_t ptk_len, int use_sha256)
{
	u8 data[2 * ETH_ALEN + 2 * WPA_NONCE_LEN];
    int ret;

	if (os_memcmp(addr1, addr2, ETH_ALEN) < 0) {
		os_memcpy(data, addr1, ETH_ALEN);
		os_memcpy(data + ETH_ALEN, addr2, ETH_ALEN);
	} else {
		os_memcpy(data, addr2, ETH_ALEN);
		os_memcpy(data + ETH_ALEN, addr1, ETH_ALEN);
	}

	if (os_memcmp(nonce1, nonce2, WPA_NONCE_LEN) < 0) {
		os_memcpy(data + 2 * ETH_ALEN, nonce1, WPA_NONCE_LEN);
		os_memcpy(data + 2 * ETH_ALEN + WPA_NONCE_LEN, nonce2,
			  WPA_NONCE_LEN);
	} else {
		os_memcpy(data + 2 * ETH_ALEN, nonce2, WPA_NONCE_LEN);
		os_memcpy(data + 2 * ETH_ALEN + WPA_NONCE_LEN, nonce1,
			  WPA_NONCE_LEN);
	}

    wpa_printf(MSG_DEBUG, "[KEY]WPA: call sha1_prf ");
	ret = sha1_prf(pmk, pmk_len, label, data, sizeof(data), ptk, ptk_len);
    if (ret != 0) {
        wpa_printf(MSG_DEBUG, "[KEY]WPA: ret of sha1_prf: %d ", ret); //0
    }
	//wpa_printf(MSG_DEBUG, "\r\n wpa_pmk_to_ptk, WPA: PTK derivation - A1=" MACSTR " A2=" MACSTR,
	//	   MAC2STR(addr1), MAC2STR(addr2));

    wpa_printf(MSG_DEBUG, "[KEY]WPA: PTK derivation - A1=%02x:%02x:%02x:%02x:%02x:%02x ",
                         *(addr1+0),
                         *(addr1+1),
                         *(addr1+2),
                         *(addr1+3),
                         *(addr1+4),
                         *(addr1+5)); //A1=22:33:44:55:66:76
    wpa_printf(MSG_DEBUG, "[KEY]WPA: PTK derivation - A2=%02x:%02x:%02x:%02x:%02x:%02x ",
                         *(addr2+0),
                         *(addr2+1),
                         *(addr2+2),
                         *(addr2+3),
                         *(addr2+4),
                         *(addr2+5)); //A2=80:26:89:58:64:d3
	wpa_hexdump_key(MSG_DEBUG, "[KEY]WPA: PMK", pmk, pmk_len); //PMK - hexdump(len=32): e8 d1 d6 69 13 0c c6 d7 aa 5a 4e 0b 07 9f 49 a8 c5 b6 0b d3 30 23 4e f0 f0 30 ec a6 fc 04 29 fd
	wpa_hexdump_key(MSG_DEBUG, "[KEY]WPA: PTK", ptk, ptk_len); //PTK - hexdump(len=48): cb 63 d8 48 4f 7c 55 c4 c8 de 67 72 56 af 62 08 43 44 5b 42 73 d8 79 95 ff a1 aa e1 c0 f1 df 2d 9f c3 3f 01 ea 8a 16 3d f5 e8 3e d3 75 5a bb 63
}


/**
 * wpa_parse_wpa_ie_rsn - Parse RSN IE
 * @rsn_ie: Buffer containing RSN IE
 * @rsn_ie_len: RSN IE buffer length (including IE number and length octets)
 * @data: Pointer to structure that will be filled in with parsed data
 * Returns: 0 on success, <0 on failure
 */
int wpa_parse_wpa_ie_rsn_impl(const u8 *rsn_ie, size_t rsn_ie_len,
			 struct wpa_ie_data *data)
{
#ifndef CONFIG_NO_WPA2
	const struct rsn_ie_hdr *hdr;
	const u8 *pos;
	int left;
	int i, count;

	os_memset(data, 0, sizeof(*data));
	data->proto = WPA_PROTO_RSN;
	data->pairwise_cipher = WPA_CIPHER_CCMP;
	data->group_cipher = WPA_CIPHER_CCMP;
	data->key_mgmt = WPA_KEY_MGMT_IEEE8021X;
	data->capabilities = 0;
	data->pmkid = NULL;
	data->num_pmkid = 0;

	data->mgmt_group_cipher = 0;

	if (rsn_ie_len == 0) {
		/* No RSN IE - fail silently */
		return -1;
	}

	if (rsn_ie_len < sizeof(struct rsn_ie_hdr)) {
		wpa_printf(MSG_DEBUG, "[KEY]WPA: %s: ie len too short %lu",
			   __func__, (unsigned long) rsn_ie_len);
		return -1;
	}

	hdr = (const struct rsn_ie_hdr *) rsn_ie;

	if (hdr->elem_id != WLAN_EID_RSN ||
	    hdr->len != rsn_ie_len - 2 ||
	    WPA_GET_LE16(hdr->version) != RSN_VERSION) {
		wpa_printf(MSG_DEBUG, "[KEY]WPA: %s: malformed ie or unknown version",
			   __func__);
		return -2;
	}

	pos = (const u8 *) (hdr + 1);
	left = rsn_ie_len - sizeof(*hdr);

	if (left >= RSN_SELECTOR_LEN) {
		data->group_cipher = rsn_selector_to_bitfield(pos);
		pos += RSN_SELECTOR_LEN;
		left -= RSN_SELECTOR_LEN;
	} else if (left > 0) {
		wpa_printf(MSG_DEBUG, "[KEY]WPA: %s: ie length mismatch, %u too much",
			   __func__, left);
		return -3;
	}

	if (left >= 2) {
		data->pairwise_cipher = 0;
		count = WPA_GET_LE16(pos);
		pos += 2;
		left -= 2;
		if (count == 0 || left < count * RSN_SELECTOR_LEN) {
			wpa_printf(MSG_DEBUG, "[KEY]WPA: %s: ie count botch (pairwise), "
				   "count %u left %u", __func__, count, left);
			return -4;
		}
		for (i = 0; i < count; i++) {
			data->pairwise_cipher |= rsn_selector_to_bitfield(pos);
			pos += RSN_SELECTOR_LEN;
			left -= RSN_SELECTOR_LEN;
		}

	} else if (left == 1) {
		wpa_printf(MSG_DEBUG, "[KEY]WPA: %s: ie too short (for key mgmt)",
			   __func__);
		return -5;
	}

	if (left >= 2) {
		data->key_mgmt = 0;
		count = WPA_GET_LE16(pos);
		pos += 2;
		left -= 2;
		if (count == 0 || left < count * RSN_SELECTOR_LEN) {
			wpa_printf(MSG_DEBUG, "[KEY]WPA: %s: ie count botch (key mgmt), "
				   "count %u left %u", __func__, count, left);
			return -6;
		}
		for (i = 0; i < count; i++) {
			data->key_mgmt |= rsn_key_mgmt_to_bitfield(pos);
			pos += RSN_SELECTOR_LEN;
			left -= RSN_SELECTOR_LEN;
		}
	} else if (left == 1) {
		wpa_printf(MSG_DEBUG, "[KEY]WPA: %s: ie too short (for capabilities)",
			   __func__);
		return -7;
	}

	if (left >= 2) {
		data->capabilities = WPA_GET_LE16(pos);
		pos += 2;
		left -= 2;
	}

	if (left >= 2) {
		data->num_pmkid = WPA_GET_LE16(pos);
		pos += 2;
		left -= 2;
		if (left < (int) data->num_pmkid * PMKID_LEN) {
			wpa_printf(MSG_DEBUG, "[KEY]WPA: %s: PMKID underflow "
				   "(num_pmkid=%lu left=%d)",
				   __func__, (unsigned long) data->num_pmkid,
				   left);
			data->num_pmkid = 0;
			return -9;
		} else {
			data->pmkid = pos;
			pos += data->num_pmkid * PMKID_LEN;
			left -= data->num_pmkid * PMKID_LEN;
		}
	}

	if (left > 0) {
		wpa_printf(MSG_DEBUG, "[KEY]WPA: %s: ie has %u trailing bytes - ignored",
			   __func__, left);
	}

	return 0;
#else /* CONFIG_NO_WPA2 */
	return -1;
#endif /* CONFIG_NO_WPA2 */
}


/**
 * rsn_pmkid - Calculate PMK identifier
 * @pmk: Pairwise master key
 * @pmk_len: Length of pmk in bytes
 * @aa: Authenticator address
 * @spa: Supplicant address
 * @pmkid: Buffer for PMKID
 * @use_sha256: Whether to use SHA256-based KDF
 *
 * IEEE Std 802.11i-2004 - 8.5.1.2 Pairwise key hierarchy
 * PMKID = HMAC-SHA1-128(PMK, "PMK Name" || AA || SPA)
 */
void rsn_pmkid_impl(const u8 *pmk, size_t pmk_len, const u8 *aa, const u8 *spa,
	       u8 *pmkid, int use_sha256)
{
	char *title = "PMK Name";
	const u8 *addr[3];
	const size_t len[3] = { 8, ETH_ALEN, ETH_ALEN };
	unsigned char hash[SHA256_MAC_LEN];

	addr[0] = (u8 *) title;
	addr[1] = aa;
	addr[2] = spa;

		hmac_sha1_vector(pmk, pmk_len, 3, addr, len, hash);
	os_memcpy(pmkid, hash, PMKID_LEN);
}


/**
 * wpa_cipher_txt - Convert cipher suite to a text string
 * @cipher: Cipher suite (WPA_CIPHER_* enum)
 * Returns: Pointer to a text string of the cipher suite name
 */
const char * wpa_cipher_txt(int cipher)
{
	switch (cipher) {
	case WPA_CIPHER_NONE:
		return "NONE";
	case WPA_CIPHER_WEP40:
		return "WEP-40";
	case WPA_CIPHER_WEP104:
		return "WEP-104";
	case WPA_CIPHER_TKIP:
		return "TKIP";
	case WPA_CIPHER_CCMP:
		return "CCMP";
	case WPA_CIPHER_CCMP | WPA_CIPHER_TKIP:
		return "CCMP+TKIP";
	default:
		return "UNKNOWN";
	}
}


/**
 * wpa_key_mgmt_txt - Convert key management suite to a text string
 * @key_mgmt: Key management suite (WPA_KEY_MGMT_* enum)
 * @proto: WPA/WPA2 version (WPA_PROTO_*)
 * Returns: Pointer to a text string of the key management suite name
 */
const char * wpa_key_mgmt_txt(int key_mgmt, int proto)
{
	switch (key_mgmt) {
	case WPA_KEY_MGMT_IEEE8021X:
		if (proto == (WPA_PROTO_RSN | WPA_PROTO_WPA))
			return "WPA2+WPA/IEEE 802.1X/EAP";
		return proto == WPA_PROTO_RSN ?
			"WPA2/IEEE 802.1X/EAP" : "WPA/IEEE 802.1X/EAP";
	case WPA_KEY_MGMT_PSK:
		if (proto == (WPA_PROTO_RSN | WPA_PROTO_WPA))
			return "WPA2-PSK+WPA-PSK";
		return proto == WPA_PROTO_RSN ?
			"WPA2-PSK" : "WPA-PSK";
	case WPA_KEY_MGMT_NONE:
		return "NONE";
	case WPA_KEY_MGMT_IEEE8021X_NO_WPA:
		return "IEEE 802.1X (no WPA)";

	default:
		return "UNKNOWN";
	}
}


int wpa_compare_rsn_ie_impl(int ft_initial_assoc,
		       const u8 *ie1, size_t ie1len,
		       const u8 *ie2, size_t ie2len)
{
    int i;
    wpa_printf(MSG_DEBUG, "[IE]WPA: ft_initial_assoc:%d ie1len:%ld ie2len:%ld \r\n", ft_initial_assoc, ie1len, ie2len);

	if (ie1 == NULL || ie2 == NULL)
		return -1;

    wpa_printf(MSG_DEBUG, "[IE]WPA: os_memcmp:%d \r\n", os_memcmp(ie1, ie2, ie1len));

    wpa_printf(MSG_DEBUG, "[IE]WPA: ie1: ");
    for(i=0; i<ie1len; i++) {
        wpa_printf(MSG_DEBUG, "%02x ", *(ie1 + i));
    }
    wpa_printf(MSG_DEBUG, "\r\r");

    wpa_printf(MSG_DEBUG, "[IE]WPA: ie2: ");
    for(i=0; i<ie2len; i++) {
        wpa_printf(MSG_DEBUG, "%02x ", *(ie2 + i));
    }
    wpa_printf(MSG_DEBUG, "\r\r");

#if 0
	if (ie1len == ie2len && os_memcmp(ie1, ie2, ie1len) == 0)
		return 0; /* identical IEs */
#else
    if(os_memcmp(ie1, ie2, ie1len) == 0) return 0;
#endif

	return -1;
}


#ifndef CONFIG_NO_WPA2
int rsn_selector_to_bitfield_impl(const u8 *s)
{
	if (RSN_SELECTOR_GET(s) == RSN_CIPHER_SUITE_NONE)
		return WPA_CIPHER_NONE;
	if (RSN_SELECTOR_GET(s) == RSN_CIPHER_SUITE_WEP40)
		return WPA_CIPHER_WEP40;
	if (RSN_SELECTOR_GET(s) == RSN_CIPHER_SUITE_TKIP)
		return WPA_CIPHER_TKIP;
	if (RSN_SELECTOR_GET(s) == RSN_CIPHER_SUITE_CCMP)
		return WPA_CIPHER_CCMP;
	if (RSN_SELECTOR_GET(s) == RSN_CIPHER_SUITE_WEP104)
		return WPA_CIPHER_WEP104;
	return 0;
}
int rsn_key_mgmt_to_bitfield_impl(const u8 *s)
{
	if (RSN_SELECTOR_GET(s) == RSN_AUTH_KEY_MGMT_UNSPEC_802_1X)
		return WPA_KEY_MGMT_IEEE8021X;
	if (RSN_SELECTOR_GET(s) == RSN_AUTH_KEY_MGMT_PSK_OVER_802_1X)
		return WPA_KEY_MGMT_PSK;
	return 0;
}
#endif /* CONFIG_NO_WPA2 */

int wpa_cipher_valid_pairwise_impl(int cipher)
{
	return cipher == WPA_CIPHER_CCMP_256 ||
		cipher == WPA_CIPHER_GCMP_256 ||
		cipher == WPA_CIPHER_CCMP ||
		cipher == WPA_CIPHER_GCMP ||
		cipher == WPA_CIPHER_TKIP;
}

int wpa_cipher_valid_group_impl(int cipher)
{
	return wpa_cipher_valid_pairwise(cipher) ||
		cipher == WPA_CIPHER_GTK_NOT_USED;
}

RET_DATA wpa_eapol_key_mic_fp_t wpa_eapol_key_mic;
RET_DATA wpa_pmk_to_ptk_fp_t wpa_pmk_to_ptk;
RET_DATA wpa_parse_wpa_ie_rsn_fp_t wpa_parse_wpa_ie_rsn;
RET_DATA rsn_pmkid_fp_t rsn_pmkid;
RET_DATA wpa_compare_rsn_ie_fp_t wpa_compare_rsn_ie;
#ifndef CONFIG_NO_WPA2
RET_DATA rsn_selector_to_bitfield_fp_t rsn_selector_to_bitfield;
RET_DATA rsn_key_mgmt_to_bitfield_fp_t rsn_key_mgmt_to_bitfield;
#endif
RET_DATA wpa_cipher_valid_pairwise_fp_t wpa_cipher_valid_pairwise;
RET_DATA wpa_cipher_valid_group_fp_t wpa_cipher_valid_group;

/*
   Interface Initialization: WPA COMMON
 */
void wpa_common_func_init(void)
{
    wpa_eapol_key_mic         = wpa_eapol_key_mic_impl;
    wpa_pmk_to_ptk            = wpa_pmk_to_ptk_impl;
    wpa_parse_wpa_ie_rsn      = wpa_parse_wpa_ie_rsn_impl;
    rsn_pmkid                 = rsn_pmkid_impl;
    wpa_compare_rsn_ie        = wpa_compare_rsn_ie_impl;
#ifndef CONFIG_NO_WPA2
    rsn_selector_to_bitfield  = rsn_selector_to_bitfield_impl;
    rsn_key_mgmt_to_bitfield  = rsn_key_mgmt_to_bitfield_impl;
#endif
    wpa_cipher_valid_pairwise = wpa_cipher_valid_pairwise_impl;
    wpa_cipher_valid_group    = wpa_cipher_valid_group_impl;
}
