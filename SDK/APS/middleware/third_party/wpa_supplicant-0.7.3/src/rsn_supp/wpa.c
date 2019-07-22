/*
 * WPA Supplicant - WPA state machine and EAPOL-Key processing
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
#include "includes.h"
#include "common.h"
#include "aes_wrap.h"
#include "crypto.h"
#include "controller_wifi_com.h"
#include "driver_netlink.h"
#include "wpa.h"
#include "preauth.h"
#include "pmksa_cache.h"
#include "peerkey.h"
#include "wpa_i.h"
#include "wpa_ie.h"
#include "wifi_mac_data.h"

RET_DATA struct ieee802_1x_hdr msg_1_4_hdr;
RET_DATA struct wpa_ptk g_ptk;
RET_DATA struct wpa_gtk_data g_gtk;

RET_DATA char g_passphrase[MAX_LEN_OF_PASSWD];
RET_DATA u16 g_key_info;
RET_DATA unsigned int g_frame_len;
RET_DATA u16 g_key_info_1_4;
RET_DATA u16 g_key_info_3_4;
//RET_DATA u8 g_kck[16]; /* EAPOL-Key Key Confirmation Key (KCK) */ //For GTK Exchange Usage
RET_DATA u8 g_wpa_psk[32];

extern u8 g_fastconn;   //I0064  0000736
extern u8 g_wifi_reconnection_counter;  ////I0064  0000869
extern struct wpa_supplicant *wpa_s;
extern struct wpa_ptk ptk;

//#define wpa_printf(...)
/**
 * wpa_ether_send - Send Ethernet frame
 * @dest: Destination MAC address
 * @proto: Ethertype in host byte order
 * @buf: Frame payload starting from IEEE 802.1X header
 * @len: Frame payload length
 * Returns: >=0 on success, <0 on failure
 */
int wpa_ether_send_impl(const u8 *dest, u16 proto, u8 *buf, size_t len)
{
    wpa_printf(MSG_DEBUG, "[KEY]WPA: send eapol key frame, proto:%d len:%d \r\n", proto, len);
	return wpa_drv_send_eapol(dest, proto, buf, len);
}


/**
 * wpa_eapol_key_send - Send WPA/RSN EAPOL-Key message
 * @sm: Pointer to WPA state machine data from wpa_sm_init()
 * @kck: Key Confirmation Key (KCK, part of PTK)
 * @ver: Version field from Key Info
 * @dest: Destination address for the frame
 * @proto: Ethertype (usually ETH_P_EAPOL)
 * @msg: EAPOL-Key message
 * @msg_len: Length of message
 * @key_mic: Pointer to the buffer to which the EAPOL-Key MIC is written
 */
void wpa_eapol_key_send_impl(struct wpa_sm *sm, const u8 *kck,
			int ver, const u8 *dest, u16 proto,
			u8 *msg, size_t msg_len, u8 *key_mic)
{   //msg: 121 bytes EAPOL-Key Frame
    //msg_len:121

    int i;
//    int ret;

    //wpa_printf(MSG_DEBUG, "\r\n WPA: wpa_eapol_key_send, ver:%d ", ver); //
    wpa_printf(MSG_DEBUG, "[KEY]WPA: kck:");
    for(i=0; i<16; i++) wpa_printf(MSG_DEBUG, "%02x ", kck[i]);
    wpa_printf(MSG_DEBUG, "\r\n");

#if 0
	if (is_zero_ether_addr(dest) && is_zero_ether_addr(sm->bssid)) {
		/*
		 * Association event was not yet received; try to fetch
		 * BSSID from the driver.
		 */
		if (wpa_sm_get_bssid(sm, sm->bssid) < 0) {
			wpa_printf(MSG_DEBUG, "\r\n wpa_eapol_key_send, WPA: Failed to read BSSID for "
				   "EAPOL-Key destination address \r\n");
		} else {
			dest = sm->bssid;
			wpa_printf(MSG_DEBUG, "\r\n wpa_eapol_key_send, WPA: Use BSSID (" MACSTR
				   ") as the destination for EAPOL-Key \r\n",
				   MAC2STR(dest));
		}
	}
	if (key_mic &&
	    wpa_eapol_key_mic(kck, ver, msg, msg_len, key_mic)) {
		wpa_printf(MSG_ERROR, "\r\n wpa_eapol_key_send, WPA: Failed to generate EAPOL-Key "
			   "version %d MIC \r\n", ver);
		goto out;
	}

    wpa_printf(MSG_DEBUG, "\r\n wpa_eapol_key_send, call wpa_eapol_key_mic \r\n");

    ret = wpa_eapol_key_mic(kck, ver, msg, msg_len, key_mic);

    wpa_printf(MSG_DEBUG, "\r\n wpa_eapol_key_send, after wpa_eapol_key_mic \r\n");

    wpa_printf(MSG_DEBUG, "\r\n wpa_eapol_key_send, key_mic[16]: "); //key_mic[16]: 8c 08 c7 da 06 31 ae 55 ac 41 fe f8 e8 82 ea 9e
    for(i=0;i<16;i++) wpa_printf(MSG_ERROR, "%02x ", key_mic[i]);
#endif


    //wpa_printf(MSG_DEBUG, "\r\n wpa_eapol_key_send, param1 dest: ");
    //for(i=0; i<6; i++) wpa_printf(MSG_DEBUG, "%02x ", *(dest + i));
    //wpa_printf(MSG_DEBUG, "\r\n wpa_eapol_key_send, param2 proto: %lx \r\n", proto);
	//wpa_hexdump(MSG_MSGDUMP, "\r\n wpa_eapol_key_send, WPA: TX EAPOL-Key ", msg, msg_len);
    //wpa_printf(MSG_DEBUG, "\r\n wpa_eapol_key_send, call wpa_ether_send \r\n");

    wpa_ether_send(dest, proto, msg, msg_len); //ok

	//eapol_sm_notify_tx_eapol_key(sm->eapol);
//out:
	os_free(msg);
}


/**
 * wpa_sm_key_request - Send EAPOL-Key Request
 * @sm: Pointer to WPA state machine data from wpa_sm_init()
 * @error: Indicate whether this is an Michael MIC error report
 * @pairwise: 1 = error report for pairwise packet, 0 = for group packet
 *
 * Send an EAPOL-Key Request to the current authenticator. This function is
 * used to request rekeying and it is usually called when a local Michael MIC
 * failure is detected.
 */
void wpa_sm_key_request_impl(struct wpa_sm *sm, int error, int pairwise)
{
	size_t rlen;
	struct wpa_eapol_key *reply;
	int key_info, ver;
	u8 bssid[ETH_ALEN], *rbuf;

	if (wpa_key_mgmt_ft(sm->key_mgmt) || wpa_key_mgmt_sha256(sm->key_mgmt))
		ver = WPA_KEY_INFO_TYPE_AES_128_CMAC;
	else if (sm->pairwise_cipher == WPA_CIPHER_CCMP)
		ver = WPA_KEY_INFO_TYPE_HMAC_SHA1_AES;
	else
		ver = WPA_KEY_INFO_TYPE_HMAC_MD5_RC4;

	if (wpa_sm_get_bssid(sm, bssid) < 0) {
		wpa_printf(MSG_WARNING, "[KEY]WPA: Failed to read BSSID for EAPOL-Key request \r\n");
		return;
	}

	rbuf = wpa_sm_alloc_eapol(sm, IEEE802_1X_TYPE_EAPOL_KEY, NULL, sizeof(*reply), &rlen, (void *) &reply);
	if (rbuf == NULL)
		return;

	reply->type = sm->proto == WPA_PROTO_RSN ?
		EAPOL_KEY_TYPE_RSN : EAPOL_KEY_TYPE_WPA;

	key_info = WPA_KEY_INFO_REQUEST | ver;
	if (sm->ptk_set)
		key_info |= WPA_KEY_INFO_MIC;
	if (error)
		key_info |= WPA_KEY_INFO_ERROR;
	if (pairwise)
		key_info |= WPA_KEY_INFO_KEY_TYPE;

	WPA_PUT_BE16(reply->key_info, key_info);
	WPA_PUT_BE16(reply->key_length, 0);
	os_memcpy(reply->replay_counter, sm->request_counter, WPA_REPLAY_COUNTER_LEN);
	inc_byte_array(sm->request_counter, WPA_REPLAY_COUNTER_LEN);
	WPA_PUT_BE16(reply->key_data_length, 0);
	wpa_printf(MSG_DEBUG, "[KEY]WPA: Sending EAPOL-Key Request (error=%d pairwise=%d ptk_set=%d len=%lu) \r\n", error, pairwise, sm->ptk_set, (unsigned long) rlen);

    wpa_eapol_key_send(sm, sm->ptk.kck, ver, bssid, ETH_P_EAPOL,
			   rbuf, rlen, key_info & WPA_KEY_INFO_MIC ?
			   reply->key_mic : NULL);
}

int wpa_supplicant_get_pmk_impl(struct wpa_sm *sm,
				  const unsigned char *src_addr,
				  const u8 *pmkid)
{
#if 0
	int abort_cached = 0;

    //wpa_printf(MSG_DEBUG, "\r\nwpa_supplicant_get_pmk\r\n");
	if (pmkid && !sm->cur_pmksa) {
		/* When using drivers that generate RSN IE, wpa_supplicant may
		 * not have enough time to get the association information
		 * event before receiving this 1/4 message, so try to find a
		 * matching PMKSA cache entry here. */
		sm->cur_pmksa = pmksa_cache_get(sm->pmksa, src_addr, pmkid);
		if (sm->cur_pmksa) {
			wpa_printf(MSG_DEBUG, "\r\nRSN: found matching PMKID from "
				   "PMKSA cache\r\n");
		} else {
			wpa_printf(MSG_DEBUG, "\r\nRSN: no matching PMKID found\r\n");
			abort_cached = 1;
		}
	}
	if (pmkid && sm->cur_pmksa &&
	    os_memcmp(pmkid, sm->cur_pmksa->pmkid, PMKID_LEN) == 0) { //no enter
		wpa_hexdump(MSG_DEBUG, "\r\nRSN: matched PMKID", pmkid, PMKID_LEN);
		wpa_sm_set_pmk_from_pmksa(sm);

		wpa_hexdump_key(MSG_DEBUG, "\r\nRSN: PMK from PMKSA cache",
				sm->pmk, sm->pmk_len);

		//eapol_sm_notify_cached(sm->eapol);

	}
    else if (wpa_key_mgmt_wpa_ieee8021x(sm->key_mgmt) && sm->eapol) {
		int res, pmk_len;
		pmk_len = PMK_LEN;
		res = eapol_sm_get_key(sm->eapol, sm->pmk, PMK_LEN);
		if (res) {
			/*
			 * EAP-LEAP is an exception from other EAP methods: it
			 * uses only 16-byte PMK.
			 */
			res = eapol_sm_get_key(sm->eapol, sm->pmk, 16);
			pmk_len = 16;
		} else {


		}
		if (res == 0) {
			wpa_hexdump_key(MSG_DEBUG, "\r\nWPA: PMK from EAPOL state "
					"machines", sm->pmk, pmk_len);
			sm->pmk_len = pmk_len;

			if (sm->proto == WPA_PROTO_RSN) {
				pmksa_cache_add(sm->pmksa, sm->pmk, pmk_len,
						src_addr, sm->own_addr,
						sm->network_ctx, sm->key_mgmt);
			}
			if (!sm->cur_pmksa && pmkid &&
			    pmksa_cache_get(sm->pmksa, src_addr, pmkid)) {
				wpa_printf(MSG_DEBUG, "\r\nRSN: the new PMK "
					   "matches with the PMKID");
				abort_cached = 0;
			}
		} else {
			wpa_msg(sm->ctx->msg_ctx, MSG_WARNING,
				"\r\nWPA: Failed to get master session key from "
				"EAPOL state machines\r\n");
			wpa_msg(sm->ctx->msg_ctx, MSG_WARNING,
				"WPA: Key handshake aborted");

			if (sm->cur_pmksa) {
				wpa_printf(MSG_DEBUG, "\r\nRSN: Cancelled PMKSA "
					   "caching attempt\r\n");
				sm->cur_pmksa = NULL;
				abort_cached = 1;
			} else if (!abort_cached) {
				return -1;
			}
		}
	}
	if (abort_cached && wpa_key_mgmt_wpa_ieee8021x(sm->key_mgmt)) {
		/* Send EAPOL-Start to trigger full EAP authentication. */
		u8 *buf;
		size_t buflen;

		wpa_printf(MSG_DEBUG, "\r\nRSN: no PMKSA entry found - trigger "
			   "full EAP authentication\r\n");
		buf = wpa_sm_alloc_eapol(sm, IEEE802_1X_TYPE_EAPOL_START,
					 NULL, 0, &buflen, NULL);
		if (buf) {
			wpa_sm_ether_send(sm, sm->bssid, ETH_P_EAPOL,
					  buf, buflen);
			os_free(buf);
			return -2;
		}

		return -1;
	}
#endif //#if 0
	return 0;
}


u8 * wpa_alloc_eapol_impl(u8 ver, u8 type, void *data, u16 data_len, size_t *msg_len, void **data_pos)
{
	struct ieee802_1x_hdr *hdr;

	*msg_len = sizeof(*hdr) + data_len;

    wpa_printf(MSG_DEBUG, "[KEY]WPA: malloc eapol key, ver:%d type:%d data_len:%d msg_len:%d \r\n", ver, type, data_len, *msg_len);

	hdr = os_malloc(*msg_len);
	if (hdr == NULL)
		return NULL;

	hdr->version = ver;
	hdr->type = type;
	hdr->length = host_to_be16(data_len);

	if (data)
		os_memcpy(hdr + 1, data, data_len);
	else
		os_memset(hdr + 1, 0, data_len);

	if (data_pos)
		*data_pos = hdr + 1;

	return (u8 *) hdr;
}


/**
 * wpa_supplicant_send_2_of_4 - Send message 2 of WPA/RSN 4-Way Handshake
 * @sm: Pointer to WPA state machine data from wpa_sm_init()
 * @dst: Destination address for the frame
 * @key: Pointer to the EAPOL-Key frame header
 * @ver: Version bits from EAPOL-Key Key Info
 * @nonce: Nonce value for the EAPOL-Key frame
 * @wpa_ie: WPA/RSN IE
 * @wpa_ie_len: Length of the WPA/RSN IE
 * @ptk: PTK to use for keyed hash and encryption
 * Returns: 0 on success, -1 on failure
 */
int wpa_supplicant_send_2_of_4_impl(struct wpa_sm *sm, const unsigned char *dst,
			       const struct wpa_eapol_key *key,
			       int ver, const u8 *nonce,
			       const u8 *wpa_ie, size_t wpa_ie_len,
			       struct wpa_ptk *ptk)
{
	size_t rlen;
	struct wpa_eapol_key *reply;
	u8 *rbuf;
	//u8 *rsn_ie_buf = NULL;
    int i;
    int len_total;
    int ret;

    wpa_printf(MSG_DEBUG, "[2_4]WPA: ver:%d wpa_ie_len:%d wpa_ie:", ver, wpa_ie_len); //wpa_ie_len:20  wpa_ie[]: 30 14 1 0 0 f ac 4 1 0 0 f ac 4 1 0 0 f ac 2 => no rns capabilityes 2 bytes, why?

    for(i=0;i<wpa_ie_len;i++){
        wpa_printf(MSG_DEBUG, "%02x ", wpa_ie[i]);
    }
    wpa_printf(MSG_DEBUG, "\r\n");

    if (wpa_ie == NULL) {
        wpa_printf(MSG_WARNING, "[2_4]WPA: No wpa_ie set - cannot generate msg 2/4");
        return -1;
    }

    wpa_hexdump(MSG_DEBUG, "[2_4]WPA: WPA IE for msg 2/4 ", wpa_ie, wpa_ie_len); //hexdump(len=20): 30 14 01 00 00 0f ac 04 01 00 00 0f ac 04 01 00 00 0f ac 02
    wpa_printf(MSG_DEBUG, "[2_4]WPA: wpa_ie_len:%d \r\n", wpa_ie_len); //sizeof(*reply):95  wpa_ie_len:20

#ifdef CONFIG_IEEE80211R
	if (wpa_key_mgmt_ft(sm->key_mgmt)) {
		int res;

		/*
		 * Add PMKR1Name into RSN IE (PMKID-List) and add MDIE and
		 * FTIE from (Re)Association Response.
		 */
		rsn_ie_buf = os_malloc(wpa_ie_len + 2 + 2 + PMKID_LEN +
				       sm->assoc_resp_ies_len);
		if (rsn_ie_buf == NULL)
			return -1;
		os_memcpy(rsn_ie_buf, wpa_ie, wpa_ie_len);
		res = wpa_insert_pmkid(rsn_ie_buf, wpa_ie_len,
				       sm->pmk_r1_name);
		if (res < 0) {
			os_free(rsn_ie_buf);
			return -1;
		}
		wpa_ie_len += res;

		if (sm->assoc_resp_ies) {
			os_memcpy(rsn_ie_buf + wpa_ie_len, sm->assoc_resp_ies,
				  sm->assoc_resp_ies_len);
			wpa_ie_len += sm->assoc_resp_ies_len;
		}

		wpa_ie = rsn_ie_buf;
	}
#endif /* CONFIG_IEEE80211R */

    /*
          Total Length-
              sizeof(*reply) + wpa_ie_len
                  95         +     20    
    */
    len_total = sizeof(*reply) + wpa_ie_len;


#if 0
	rbuf = wpa_sm_alloc_eapol(sm, IEEE802_1X_TYPE_EAPOL_KEY,
				  NULL, sizeof(*reply) + wpa_ie_len,
				  &rlen, (void *) &reply);
#else
    //here, ver should be the same as message 1's version of ieee802_1x_hdr
    /*
    rbuf = wpa_alloc_eapol(ver, IEEE802_1X_TYPE_EAPOL_KEY,
				  NULL, len_total,
				  &rlen, (void *) &reply);
    */
    rbuf = wpa_alloc_eapol((int)msg_1_4_hdr.version, IEEE802_1X_TYPE_EAPOL_KEY,
                           NULL, len_total,
                           &rlen, (void *) &reply);
    wpa_printf(MSG_DEBUG, "[2_4]WPA: len_total:%d rlen:%d \r\n", len_total, rlen); //len_total:117 rlen:121
#endif //#if 0

    if (rbuf == NULL) {
        //os_free(rsn_ie_buf);
        wpa_printf(MSG_WARNING, "[2_4]WPA: rbuf == NULL \r\n");
        return -1;
    }

    reply->type = sm->proto == WPA_PROTO_RSN ? EAPOL_KEY_TYPE_RSN : EAPOL_KEY_TYPE_WPA;

    wpa_printf(MSG_DEBUG, "[2_4]WPA: reply->type:%d sm->proto:%d \r\n", reply->type, sm->proto); //reply->type:2 sm->proto:2

    WPA_PUT_BE16(reply->key_info, ver | WPA_KEY_INFO_KEY_TYPE | WPA_KEY_INFO_MIC); //correct

    wpa_printf(MSG_DEBUG, "[2_4]WPA: reply->key_info:%x %x \r\n", reply->key_info[0], reply->key_info[1]);

#if 1
	if (sm->proto == WPA_PROTO_RSN){
		WPA_PUT_BE16(reply->key_length, 0);
	}else{

		os_memcpy(reply->key_length, key->key_length, 2);
		//WPA_PUT_BE16(reply->key_length, 0);
    }
#else
    WPA_PUT_BE16(reply->key_length, 0);
#endif

    wpa_printf(MSG_DEBUG, "[2_4]WPA: reply->key_length:%x %x ,key->key_length:%x %x \r\n", reply->key_length[0],
                                                                                           reply->key_length[1],
                                                                                           key->key_length[0],
                                                                                           key->key_length[1]); //reply->key_length:0x0 0x10 ,key->key_length:0x0 0x10

    os_memcpy(reply->replay_counter, key->replay_counter, WPA_REPLAY_COUNTER_LEN);

    wpa_printf(MSG_DEBUG, "[2_4]WPA: reply->replay_counter:%02x %02x %02x %02x %02x %02x %02x %02x \r\n",
                                                                                           reply->replay_counter[0],
                                                                                           reply->replay_counter[1],
                                                                                           reply->replay_counter[2],
                                                                                           reply->replay_counter[3],
                                                                                           reply->replay_counter[4],
                                                                                           reply->replay_counter[5],
                                                                                           reply->replay_counter[6],
                                                                                           reply->replay_counter[7]); //reply->replay_counter[]: 00 00 00 00 00 00 00 00


    WPA_PUT_BE16(reply->key_data_length, wpa_ie_len);
    //WPA_PUT_BE16(reply->key_data_length, wpa_ie_len + 2);

    wpa_printf(MSG_DEBUG, "[2_4]WPA: reply->key_data_length:%x %x \r\n", reply->key_data_length[0], reply->key_data_length[1]); //reply->key_data_length[]: 00 16  => ok

    os_memcpy(reply + 1, wpa_ie, wpa_ie_len);

    //os_free(rsn_ie_buf);

    os_memcpy(reply->key_nonce, nonce, WPA_NONCE_LEN);

    wpa_printf(MSG_DEBUG, "[2_4]WPA: Sending EAPOL-Key 2/4 \r\n");

    wpa_printf(MSG_DEBUG, "[2_4]WPA: ptk->kck:"); //ptk->kck[16]: d2 3f a2 42 e7 c0 4a 47 9a 94 af 2b 80 6c fd 10
    for (i=0;i<16;i++) wpa_printf(MSG_DEBUG, "%02x ", ptk->kck[i]);
    wpa_printf(MSG_DEBUG, "\r\n");

    wpa_printf(MSG_DEBUG, "[2_4]WPA: dst:"); //dst: 80 26 89 58 64 d3 =>correct
    for (i=0;i<6;i++) wpa_printf(MSG_DEBUG, "%02x ", *(dst+i));
    wpa_printf(MSG_DEBUG, "\r\n");

    //wpa_printf(MSG_DEBUG, "\r\n wpa_supplicant_send_2_of_4, ETH_P_EAPOL:%x ", ETH_P_EAPOL); //ETH_P_EAPOL:888e

    wpa_printf(MSG_DEBUG, "[2_4]WPA: rbuf[%d]:", rlen);   //rbuf[121]: 02 03 00 75 02 01 0a 00 00 00 00 00 00 00 00 00 00 02 07 01 06 06 03 09 00 00 05 07 02 02 01 05 04 02 07 07 00 04 05 07 02 06 09 07 02 02 01 07 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 14 30 14 01 00 00 0f ac 04 01 00 00 0f ac 04 01 00 00 0f ac 02 00 00
    for (i=0;i<rlen;i++) wpa_printf(MSG_DEBUG, "%02x ", *(rbuf+i));
    wpa_printf(MSG_DEBUG, "\r\n");

    wpa_printf(MSG_DEBUG, "[2_4]WPA: reply->key_mic:"); //reply->key_mic[]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  =>correct
    for (i=0;i<16;i++) wpa_printf(MSG_DEBUG, "%02x ", reply->key_mic[i]);
    wpa_printf(MSG_DEBUG, "\r\n");

    //key_mic
    ret = wpa_eapol_key_mic(ptk->kck, ver, rbuf, rlen, reply->key_mic);
    // Prevent compiler warnings
    (void)ret;
    wpa_printf(MSG_DEBUG, "[2_4]WPA: reply->key_mic:"); //key_mic[16]:
    for(i=0;i<16;i++) wpa_printf(MSG_DEBUG, "%02x ", reply->key_mic[i]);
    wpa_printf(MSG_DEBUG, "\r\n");

    //Send EAPOL Key Frame
    wpa_eapol_key_send(sm, ptk->kck, ver, dst, ETH_P_EAPOL, rbuf, rlen, reply->key_mic);

    return 0;
}


int wpa_derive_ptk_impl(struct wpa_sm *sm, const unsigned char *src_addr,
                        const struct wpa_eapol_key *key,
                        struct wpa_ptk *ptk)
{
	size_t ptk_len = sm->pairwise_cipher == WPA_CIPHER_CCMP ? 48 : 64;
    int i;

    wpa_printf(MSG_DEBUG, "[KEY]WPA: sm->pairwise_cipher:%d ptk_len:%d sm->pmk_len:%d \r\n", sm->pairwise_cipher, ptk_len, sm->pmk_len); //sm->pairwise_cipher:16 ptk_len:48 //sm->pmk_len:32

    wpa_printf(MSG_DEBUG, "[KEY]WPA: sm->pmk:");       //sm->pmk:30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30
    for(i=0; i<PMK_LEN; i++) wpa_printf(MSG_DEBUG, "%02x ", sm->pmk[i]);
    wpa_printf(MSG_DEBUG, "\r\n");

    wpa_printf(MSG_DEBUG, "[KEY]WPA: sm->own_addr:"); //sm->own_addr:22 33 44 55 66 76
    for(i=0; i<ETH_ALEN; i++) wpa_printf(MSG_DEBUG, "%02x ", sm->own_addr[i]);
    wpa_printf(MSG_DEBUG, "\r\n");

    wpa_printf(MSG_DEBUG, "[KEY]WPA: sm->bssid:");    //sm->bssid:80 26 89 58 64 d3
    for(i=0; i<ETH_ALEN; i++) wpa_printf(MSG_DEBUG, "%02x ", sm->bssid[i]);
    wpa_printf(MSG_DEBUG, "\r\n");

    wpa_printf(MSG_DEBUG, "[KEY]WPA: sm->snonce:");   //sm->snonce:02 07 01 06 06 03 09 00 00 05 07 02 02 01 05 04 02 07 07 00 04 05 07 02 06 09 07 02 02 01 07 00
    for(i=0; i<WPA_NONCE_LEN; i++) wpa_printf(MSG_DEBUG, "%02x ", sm->snonce[i]);
    wpa_printf(MSG_DEBUG, "\r\n");

    wpa_printf(MSG_DEBUG, "[KEY]WPA: key->key_nonce:"); //key->key_nonce:59 b5 5e d7 b5 ad 96 8d 6a 4f 6d 67 47 eb 5c 59 a1 41 ae f2 80 0e 0f ca ad 47 b0 99 da 80 ef 1a
    for(i=0; i<WPA_NONCE_LEN; i++) wpa_printf(MSG_DEBUG, "%02x ", key->key_nonce[i]);
    wpa_printf(MSG_DEBUG, "\r\n");

    wpa_printf(MSG_DEBUG, "[KEY]WPA: ptk:");     //ptk:00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    for(i=0; i<ptk_len; i++) wpa_printf(MSG_DEBUG, "%02x ", ptk[i]);
    wpa_printf(MSG_DEBUG, "\r\n");

    wpa_printf(MSG_DEBUG, "[KEY]WPA: sm->key_mgmt:%d \r\n", sm->key_mgmt);  //sm->key_mgmt:2

    wpa_pmk_to_ptk(sm->pmk, sm->pmk_len, "Pairwise key expansion",
                   sm->own_addr, sm->bssid, sm->snonce, key->key_nonce,
                   (u8 *) ptk, ptk_len,
                   wpa_key_mgmt_sha256(sm->key_mgmt));

    wpa_printf(MSG_DEBUG, "[KEY]WPA: ptk->kck:"); //ptk->kck[16]: 0e 7c 24 4c 85 25 d9 aa 16 6a bc f5 1c 2f c8 1a
    for(i=0; i<16; i++) wpa_printf(MSG_DEBUG, "%02x ", ptk->kck[i]);
    wpa_printf(MSG_DEBUG, "\r\n");

    wpa_printf(MSG_DEBUG, "[KEY]WPA: ptk->kek:"); //ptk->kek[16]: e4 d4 4d 81 87 2f 3e 7e 3b e9 94 10 2c e9 8b df
    for(i=0; i<16; i++) wpa_printf(MSG_DEBUG, "%02x ", ptk->kek[i]);
    wpa_printf(MSG_DEBUG, "\r\n");

    wpa_printf(MSG_DEBUG, "[KEY]WPA: ptk->tk1:"); //ptk->tk1[16]: 52 d0 d6 5a 1a db 3b d4 2c 67 c2 5b cf a5 1d 73
    for(i=0; i<16; i++) wpa_printf(MSG_DEBUG, "%02x ", ptk->tk1[i]);
    wpa_printf(MSG_DEBUG, "\r\n");

    wpa_printf(MSG_DEBUG, "[KEY]WPA: ptk->u.tk2:"); //ptk->u.tk2[16]: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    for(i=0; i<16; i++) wpa_printf(MSG_DEBUG, "%02x ", ptk->u.tk2[i]);
    wpa_printf(MSG_DEBUG, "\r\n");

	return 0;
}


void wpa_supplicant_process_1_of_4_impl(struct wpa_sm *sm,
					  const unsigned char *src_addr,
					  const struct wpa_eapol_key *key,
					  u16 ver)
{
	struct wpa_eapol_ie_parse ie;
	struct wpa_ptk *ptk;
	u8 buf[8];
//	int res;
    int i;
    u8 mac[ETH_ALEN]={0};
//    u8 bssid[ETH_ALEN]={0};
/*
	if (wpa_sm_get_network_ctx(sm) == NULL) {
		wpa_printf(MSG_WARNING, "WPA: No SSID info found (msg 1 of "
			   "4).");
		return;
	}
*/

    wpa_printf(MSG_DEBUG, "[1_4]WPA: sm->proto:%d ver:%d \r\n", sm->proto, ver);

	wpa_sm_set_state(sm, WPA_4WAY_HANDSHAKE);

	wpa_printf(MSG_DEBUG, "[1_4]WPA: RX message 1 of 4-Way Handshake from " MACSTR " (ver=%d) \r\n", MAC2STR(src_addr), ver); //80:26:89:58:64:d3 (ver=2)  =>correct

	os_memset(&ie, 0, sizeof(ie));

#ifndef CONFIG_NO_WPA2
	if (sm->proto == WPA_PROTO_RSN) { //enter
		/* RSN: msg 1/4 should contain PMKID for the selected PMK */
		const u8 *_buf = (const u8 *) (key + 1);
		size_t len = WPA_GET_BE16(key->key_data_length);

		wpa_hexdump(MSG_DEBUG, "[1_4]WPA: RSN: msg 1/4 key data",  _buf, len); //dd 14 00 0f ac 04 ba 37 7b 1a e7 dd 3e e9 5b 06 ab dc 4a ad 81 54

		wpa_supplicant_parse_ies(_buf, len, &ie);

		if (ie.pmkid) { //enter
			wpa_hexdump(MSG_DEBUG, "[1_4]WPA: RSN: PMKID from Authenticator ", ie.pmkid, PMKID_LEN); //hexdump(len=16): ba 37 7b 1a e7 dd 3e e9 5b 06 ab dc 4a ad 81 54
        }
	}
#endif /* CONFIG_NO_WPA2 */

	//res = wpa_supplicant_get_pmk(sm, src_addr, ie.pmkid);

/*
	if (res == -2) {
		wpa_printf(MSG_DEBUG, "\r\n wpa_supplicant_process_1_of_4, RSN: Do not reply to msg 1/4 - "
			   "requesting full EAP authentication\r\n");
		return;
	}
	if (res)
		goto failed;
*/

	if (sm->renew_snonce) { //enter
        if (os_get_random(sm->snonce, WPA_NONCE_LEN)) { //no enter
            wpa_msg(sm->ctx->msg_ctx, MSG_WARNING, "[1_4]WPA: Failed to get random data for SNonce \r\n");
            goto failed;
        }


        wpa_printf(MSG_DEBUG, "\r\n random[]:");
        for(i=0; i<WPA_NONCE_LEN; i++){
            wpa_printf(MSG_DEBUG, "%d ", sm->snonce[i]);
        }

        sm->renew_snonce = 0;
        wpa_hexdump(MSG_DEBUG, "[1_4]WPA: Renewed SNonce ", sm->snonce, WPA_NONCE_LEN); //hexdump(len=32): 02 07 01 06 06 03 09 00 00 05 07 02 02 01 05 04 02 07 07 00 04 05 07 02 06 09 07 02 02 01 07 00
    }

	/* Calculate PTK which will be stored as a temporary PTK until it has
	 * been verified when processing message 3/4. */
	ptk = &sm->tptk;

    //Initialize the key data to 0
    os_memset(ptk, 0, sizeof(sm->tptk));

    wpa_driver_netlink_get_mac(mac);
    os_memcpy(sm->own_addr, mac, ETH_ALEN);
    //wpa_driver_netlink_get_bssid(bssid);
    os_memcpy(sm->bssid, src_addr, ETH_ALEN);

    wpa_printf(MSG_DEBUG, "\r\n wpa_supplicant_process_1_of_4, before wpa_derive_ptk, sm->own_addr[6]:%02x:%02x:%02x:%02x:%02x:%02x ", sm->own_addr[0],
                                                                                                           sm->own_addr[1],
                                                                                                           sm->own_addr[2],
                                                                                                           sm->own_addr[3],
                                                                                                           sm->own_addr[4],
                                                                                                           sm->own_addr[5]); //sm->own_addr[6]:22:33:44:55:66:76

    wpa_printf(MSG_DEBUG, "\r\n wpa_supplicant_process_1_of_4, before wpa_derive_ptk, sm->bssid[6]:%02x:%02x:%02x:%02x:%02x:%02x ", sm->bssid[0],
                                                                                                        sm->bssid[1],
                                                                                                        sm->bssid[2],
                                                                                                        sm->bssid[3],
                                                                                                        sm->bssid[4],
                                                                                                        sm->bssid[5]); //sm->bssid[6]:80:26:89:58:64:d3

    //ptk = os_malloc(sizeof(struct wpa_ptk)); //size=64
	wpa_derive_ptk(sm, src_addr, key, ptk);

	/* Supplicant: swap tx/rx Mic keys */
	os_memcpy(buf, ptk->u.auth.tx_mic_key, 8);
	os_memcpy(ptk->u.auth.tx_mic_key, ptk->u.auth.rx_mic_key, 8);
	os_memcpy(ptk->u.auth.rx_mic_key, buf, 8);
	sm->tptk_set = 1;
    wpa_printf(MSG_DEBUG, "[1_4]WPA: ptk->u.auth.tx_mic_key:%02x%02x%02x%02x%02x%02x%02x%02x0 \r\n",
                          ptk->u.auth.tx_mic_key[0],
                          ptk->u.auth.tx_mic_key[1],
                          ptk->u.auth.tx_mic_key[2],
                          ptk->u.auth.tx_mic_key[3],
                          ptk->u.auth.tx_mic_key[4],
                          ptk->u.auth.tx_mic_key[5],
                          ptk->u.auth.tx_mic_key[6],
                          ptk->u.auth.tx_mic_key[7]);
    wpa_printf(MSG_DEBUG, "[1_4]WPA: ptk->u.auth.rx_mic_key:%02x%02x%02x%02x%02x%02x%02x%02x0 \r\n",
                      ptk->u.auth.rx_mic_key[0],
                      ptk->u.auth.rx_mic_key[1],
                      ptk->u.auth.rx_mic_key[2],
                      ptk->u.auth.rx_mic_key[3],
                      ptk->u.auth.rx_mic_key[4],
                      ptk->u.auth.rx_mic_key[5],
                      ptk->u.auth.rx_mic_key[6],
                      ptk->u.auth.rx_mic_key[7]);
    //sm->tptk_set = 1;

	if (wpa_supplicant_send_2_of_4(sm, sm->bssid, key, ver, sm->snonce, sm->assoc_wpa_ie, sm->assoc_wpa_ie_len, ptk)) goto failed;

    wpa_printf(MSG_DEBUG, "[1_4]WPA: copy key_nonce to anonce, key->key_nonce:");
    for(i=0;i<WPA_NONCE_LEN;i++) wpa_printf(MSG_DEBUG, "%02x ", key->key_nonce[i]);;
    wpa_printf(MSG_DEBUG, "\r\n");

	os_memcpy(sm->anonce, key->key_nonce, WPA_NONCE_LEN);
	return;

failed:
	wpa_sm_deauthenticate(sm, WLAN_REASON_UNSPECIFIED);
}


void wpa_sm_start_preauth_impl(void *eloop_ctx, void *timeout_ctx)
{
	struct wpa_sm *sm = eloop_ctx;
	rsn_preauth_candidate_process(sm);
}


void wpa_supplicant_key_neg_complete_impl(struct wpa_sm *sm,
					    const u8 *addr, int secure)
{
	wpa_msg(sm->ctx->msg_ctx, MSG_DEBUG,
		"[GTK]WPA: Key negotiation completed with "
		MACSTR " [PTK=%s GTK=%s] \r\n", MAC2STR(addr),
		wpa_cipher_txt(sm->pairwise_cipher),
		wpa_cipher_txt(sm->group_cipher));
	//wpa_sm_cancel_auth_timeout(sm);
	wpa_sm_set_state(sm, WPA_COMPLETED);

#if 0
	if (secure) {
		wpa_sm_mlme_setprotection(
			sm, addr, MLME_SETPROTECTION_PROTECT_TYPE_RX_TX,
			MLME_SETPROTECTION_KEY_TYPE_PAIRWISE);
		eapol_sm_notify_portValid(sm->eapol, TRUE);
		if (wpa_key_mgmt_wpa_psk(sm->key_mgmt))
			eapol_sm_notify_eap_success(sm->eapol, TRUE);
		/*
		 * Start preauthentication after a short wait to avoid a
		 * possible race condition between the data receive and key
		 * configuration after the 4-Way Handshake. This increases the
		 * likelyhood of the first preauth EAPOL-Start frame getting to
		 * the target AP.
		 */
		//eloop_register_timeout(1, 0, wpa_sm_start_preauth, sm, NULL);
	}

	if (sm->cur_pmksa && sm->cur_pmksa->opportunistic) {
		wpa_printf(MSG_DEBUG, "RSN: Authenticator accepted "
			   "opportunistic PMKSA entry - marking it valid");
		sm->cur_pmksa->opportunistic = 0;
	}
#endif
}


void wpa_sm_rekey_ptk_impl(void *eloop_ctx, void *timeout_ctx)
{
	struct wpa_sm *sm = eloop_ctx;
	wpa_printf(MSG_DEBUG, "[KEY]WPA: Request PTK rekeying \r\n");
	wpa_sm_key_request(sm, 0, 1);
}


int wpa_supplicant_install_ptk_impl(struct wpa_sm *sm,
				      const struct wpa_eapol_key *key)
{
    int i;
    wpa_printf(MSG_DEBUG, "[KEY]WPA: pairwise_cipher:%d proto:%d \r\n", sm->pairwise_cipher, sm->proto);
    wpa_printf(MSG_DEBUG, "[KEY]WPA: Installing PTK to the driver \r\n");

    os_memcpy(g_ptk.kck, sm->ptk.kck, sizeof(g_ptk.kck));
    os_memcpy(g_ptk.kek, sm->ptk.kek, sizeof(g_ptk.kek));
    os_memcpy(g_ptk.tk1, sm->ptk.tk1, sizeof(g_ptk.tk1));
    os_memcpy(g_ptk.u.auth.tx_mic_key, sm->ptk.u.auth.tx_mic_key, sizeof(g_ptk.u.auth.tx_mic_key));
    os_memcpy(g_ptk.u.auth.rx_mic_key, sm->ptk.u.auth.rx_mic_key, sizeof(g_ptk.u.auth.rx_mic_key));

    wpa_printf(MSG_DEBUG, "\r\n\r\n#################### Installed Keys ###############################\r\n");

    wpa_printf(MSG_DEBUG, "[KEY]WPA: g_ptk.kck: ");
    for(i=0; i<sizeof(g_ptk.kck); i++) wpa_printf(MSG_DEBUG, "%02x ", g_ptk.kck[i]);
    wpa_printf(MSG_DEBUG, "\r\n");

    wpa_printf(MSG_DEBUG, "[KEY]WPA: g_ptk.kek: ");
    for(i=0; i<sizeof(g_ptk.kek); i++) wpa_printf(MSG_DEBUG, "%02x ", g_ptk.kek[i]);
    wpa_printf(MSG_DEBUG, "\r\n");

    wpa_printf(MSG_DEBUG, "[KEY]WPA: g_ptk.tk1: ");
    for(i=0; i<sizeof(g_ptk.tk1); i++) wpa_printf(MSG_DEBUG, "%02x ", g_ptk.tk1[i]);
    wpa_printf(MSG_DEBUG, "\r\n");

    wpa_printf(MSG_DEBUG, "[KEY]WPA: g_ptk.u.auth.tx_mic_key: ");
    for(i=0; i<sizeof(g_ptk.u.auth.tx_mic_key); i++) wpa_printf(MSG_DEBUG, "%02x ", g_ptk.u.auth.tx_mic_key[i]);
    wpa_printf(MSG_DEBUG, "\r\n");

    wpa_printf(MSG_DEBUG, "[KEY]WPA: g_ptk.u.auth.rx_mic_key: ");
    for(i=0; i<sizeof(g_ptk.u.auth.rx_mic_key); i++) wpa_printf(MSG_DEBUG, "%02x ", g_ptk.u.auth.rx_mic_key[i]);
    wpa_printf(MSG_DEBUG, "\r\n");

    wpa_printf(MSG_DEBUG, "################################################################### \r\n\r\n");
	return 0;
}


int wpa_supplicant_check_group_cipher_impl(int group_cipher,
					     int keylen, int maxkeylen,
					     int *key_rsc_len,
					     enum wpa_alg *alg)
{
	int ret = 0;

	switch (group_cipher) {
	case WPA_CIPHER_CCMP:
		if (keylen != 16 || maxkeylen < 16) {
			ret = -1;
			break;
		}
		*key_rsc_len = 6;
		*alg = WPA_ALG_CCMP;
		break;
	case WPA_CIPHER_TKIP:
		if (keylen != 32 || maxkeylen < 32) {
			ret = -1;
			break;
		}
		*key_rsc_len = 6;
		*alg = WPA_ALG_TKIP;
		break;
	case WPA_CIPHER_WEP104:
		if (keylen != 13 || maxkeylen < 13) {
			ret = -1;
			break;
		}
		*key_rsc_len = 0;
		*alg = WPA_ALG_WEP;
		break;
	case WPA_CIPHER_WEP40:
		if (keylen != 5 || maxkeylen < 5) {
			ret = -1;
			break;
		}
		*key_rsc_len = 0;
		*alg = WPA_ALG_WEP;
		break;
	default:
		wpa_printf(MSG_WARNING, "[GTK]WPA: Unsupported Group Cipher %d \r\n", group_cipher);
		return -1;
	}

	if (ret < 0 ) {
		wpa_printf(MSG_WARNING, "[GTK]WPA: Unsupported %s Group Cipher key length %d (%d). \r\n", wpa_cipher_txt(group_cipher), keylen, maxkeylen);
	}

	return ret;
}

/*
struct wpa_gtk_data {
	enum wpa_alg alg;
	int tx, key_rsc_len, keyidx;
	u8 gtk[32];
	int gtk_len;
};
*/

int wpa_supplicant_install_gtk_impl(struct wpa_sm *sm,
				      const struct wpa_gtk_data *gd,
				      const u8 *key_rsc)
{
	const u8 *_gtk = gd->gtk;
	u8 gtk_buf[32];
    int i;

    wpa_printf(MSG_DEBUG, "[GTK]WPA: sm->group_cipher:%d sm->pairwise_cipher:%d \r\n", sm->group_cipher, sm->pairwise_cipher);

    wpa_printf(MSG_DEBUG, "[GTK]WPA: gd->gtk_len:%d gd->gtk:", gd->gtk_len);
    for(i=0; i<gd->gtk_len; i++) wpa_printf(MSG_DEBUG, "%02x ", gd->gtk[i]);
    wpa_printf(MSG_DEBUG, "\r\n");

	wpa_hexdump_key(MSG_DEBUG, "[GTK]WPA: Group Key", gd->gtk, gd->gtk_len);
	wpa_printf(MSG_DEBUG, "[GTK]WPA: Installing GTK to the driver (keyidx=%d tx=%d len=%d). \r\n", gd->keyidx, gd->tx, gd->gtk_len);
	wpa_hexdump(MSG_DEBUG, "[GTK]WPA: RSC", key_rsc, gd->key_rsc_len);

	if (sm->group_cipher == WPA_CIPHER_TKIP) {
        //wpa_printf(MSG_DEBUG, "\r\n wpa_supplicant_install_gtk, group_cipher == WPA_CIPHER_TKIP \r\n");

		/* Swap Tx/Rx keys for Michael MIC */
		os_memcpy(gtk_buf, gd->gtk, 16);
		os_memcpy(gtk_buf + 16, gd->gtk + 24, 8);
		os_memcpy(gtk_buf + 24, gd->gtk + 16, 8);
		_gtk = gtk_buf;
	}

#if 0
	if (sm->pairwise_cipher == WPA_CIPHER_NONE) {
		if (wpa_sm_set_key(sm, gd->alg,
				   (u8 *) "\xff\xff\xff\xff\xff\xff",
				   gd->keyidx, 1, key_rsc, gd->key_rsc_len,
				   _gtk, gd->gtk_len) < 0) {
			wpa_printf(MSG_WARNING, "WPA: Failed to set "
				   "GTK to the driver (Group only).");
			return -1;
		}
	} else if (wpa_sm_set_key(sm, gd->alg,
				  (u8 *) "\xff\xff\xff\xff\xff\xff",
				  gd->keyidx, gd->tx, key_rsc, gd->key_rsc_len,
				  _gtk, gd->gtk_len) < 0) {
		wpa_printf(MSG_WARNING, "WPA: Failed to set GTK to "
			   "the driver (alg=%d keylen=%d keyidx=%d)",
			   gd->alg, gd->gtk_len, gd->keyidx);
		return -1;
	}
    return 0;
#else
    if (sm->pairwise_cipher == WPA_CIPHER_NONE) {
        //wpa_printf(MSG_DEBUG, "\r\n wpa_supplicant_install_gtk, pairwise_cipher == WPA_CIPHER_NONE \r\n");
    } else {
        //wpa_printf(MSG_DEBUG, "\r\n wpa_supplicant_install_gtk, pairwise_cipher != WPA_CIPHER_NONE \r\n");
        wpa_printf(MSG_DEBUG, "[GTK]WPA: install gtk \r\n");
        g_gtk.gtk_len = gd->gtk_len;
        memcpy(g_gtk.gtk, _gtk, gd->gtk_len);
    }
    //wpa_printf(MSG_DEBUG, "\r\n wpa_supplicant_install_gtk, g_gtk.gtk_len:%d g_gtk.gtk: ", g_gtk.gtk_len);
    //for (i=0; i<gd->gtk_len; i++) wpa_printf(MSG_DEBUG, "%02x ", g_gtk.gtk[i]);

    return 0;
#endif
}


int wpa_supplicant_gtk_tx_bit_workaround_impl(const struct wpa_sm *sm,
						int tx)
{
	if (tx && sm->pairwise_cipher != WPA_CIPHER_NONE) {
		/* Ignore Tx bit for GTK if a pairwise key is used. One AP
		 * seemed to set this bit (incorrectly, since Tx is only when
		 * doing Group Key only APs) and without this workaround, the
		 * data connection does not work because wpa_supplicant
		 * configured non-zero keyidx to be used for unicast. */
		wpa_printf(MSG_DEBUG, "[GTK]WPA: Tx bit set for GTK, but pairwise keys are used - ignore Tx bit \r\n");
		return 0;
	}
	return tx;
}


int wpa_supplicant_pairwise_gtk_impl(struct wpa_sm *sm,
				       const struct wpa_eapol_key *key,
				       const u8 *gtk, size_t gtk_len,
				       int key_info)
{
#ifndef CONFIG_NO_WPA2
	struct wpa_gtk_data gd;

	/*
	 * IEEE Std 802.11i-2004 - 8.5.2 EAPOL-Key frames - Figure 43x
	 * GTK KDE format:
	 * KeyID[bits 0-1], Tx [bit 2], Reserved [bits 3-7]
	 * Reserved [bits 0-7]
	 * GTK
	 */

	os_memset(&gd, 0, sizeof(gd));
	wpa_hexdump_key(MSG_DEBUG, "[GTK]WPA: RSN: received GTK in pairwise handshake", gtk, gtk_len);

	if (gtk_len < 2 || gtk_len - 2 > sizeof(gd.gtk))
		return -1;

	gd.keyidx = gtk[0] & 0x3;
	gd.tx = wpa_supplicant_gtk_tx_bit_workaround(sm,
						     !!(gtk[0] & BIT(2)));
	gtk += 2;
	gtk_len -= 2;

	os_memcpy(gd.gtk, gtk, gtk_len);
	gd.gtk_len = gtk_len;

#if 0
	if (wpa_supplicant_check_group_cipher(sm->group_cipher,
					      gtk_len, gtk_len,
					      &gd.key_rsc_len, &gd.alg) ||
	    wpa_supplicant_install_gtk(sm, &gd, key->key_rsc)) {
		//wpa_printf(MSG_DEBUG, "RSN: Failed to install GTK");
		//return -1;
	}
#else
    wpa_supplicant_check_group_cipher(sm->group_cipher, gtk_len, gtk_len, &gd.key_rsc_len, &gd.alg);
    wpa_supplicant_install_gtk(sm, &gd, key->key_rsc);
#endif

	wpa_supplicant_key_neg_complete(sm, sm->bssid, key_info & WPA_KEY_INFO_SECURE);

	return 0;
#else /* CONFIG_NO_WPA2 */
	return -1;
#endif /* CONFIG_NO_WPA2 */
}


void wpa_report_ie_mismatch_impl(struct wpa_sm *sm,
				   const char *reason, const u8 *src_addr,
				   const u8 *wpa_ie, size_t wpa_ie_len,
				   const u8 *rsn_ie, size_t rsn_ie_len)
{
	wpa_msg(sm->ctx->msg_ctx, MSG_WARNING, "[IE]WPA: %s (src=" MACSTR ") \r\n", reason, MAC2STR(src_addr));

	if (sm->ap_wpa_ie) {
		wpa_hexdump(MSG_DEBUG, "[IE]WPA: WPA IE in Beacon/ProbeResp", sm->ap_wpa_ie, sm->ap_wpa_ie_len);
	}
	if (wpa_ie) {
		if (!sm->ap_wpa_ie) {
			wpa_printf(MSG_DEBUG, "[IE]WPA: No WPA IE in Beacon/ProbeResp \r\n");
		}
		wpa_hexdump(MSG_DEBUG, "[IE]WPA: WPA IE in 3/4 msg", wpa_ie, wpa_ie_len);
	}

	if (sm->ap_rsn_ie) {
		wpa_hexdump(MSG_DEBUG, "[IE]WPA: RSN IE in Beacon/ProbeResp", sm->ap_rsn_ie, sm->ap_rsn_ie_len);
	}
	if (rsn_ie) {
		if (!sm->ap_rsn_ie) {
			wpa_printf(MSG_DEBUG, "[IE]WPA: No RSN IE in Beacon/ProbeResp \r\n");
		}
		wpa_hexdump(MSG_DEBUG, "[IE]WPA: RSN IE in 3/4 msg", rsn_ie, rsn_ie_len);
	}

	wpa_sm_disassociate(sm, WLAN_REASON_IE_IN_4WAY_DIFFERS);
}

#if 0
int wpa_supplicant_validate_ie(struct wpa_sm *sm,
				      const unsigned char *src_addr,
				      struct wpa_eapol_ie_parse *ie)
{
	if (sm->ap_wpa_ie == NULL && sm->ap_rsn_ie == NULL) {
		wpa_printf(MSG_DEBUG, "WPA: No WPA/RSN IE for this AP known. "
			   "Trying to get from scan results");
		if (wpa_sm_get_beacon_ie(sm) < 0) {
			wpa_printf(MSG_WARNING, "WPA: Could not find AP from "
				   "the scan results");
		} else {
			wpa_printf(MSG_DEBUG, "WPA: Found the current AP from "
				   "updated scan results");
		}
	}

	if (ie->wpa_ie == NULL && ie->rsn_ie == NULL &&
	    (sm->ap_wpa_ie || sm->ap_rsn_ie)) {
		wpa_report_ie_mismatch(sm, "IE in 3/4 msg does not match "
				       "with IE in Beacon/ProbeResp (no IE?)",
				       src_addr, ie->wpa_ie, ie->wpa_ie_len,
				       ie->rsn_ie, ie->rsn_ie_len);
		return -1;
	}

	if ((ie->wpa_ie && sm->ap_wpa_ie &&
	     (ie->wpa_ie_len != sm->ap_wpa_ie_len ||
	      os_memcmp(ie->wpa_ie, sm->ap_wpa_ie, ie->wpa_ie_len) != 0)) ||
	    (ie->rsn_ie && sm->ap_rsn_ie &&
	     wpa_compare_rsn_ie(wpa_key_mgmt_ft(sm->key_mgmt),
				sm->ap_rsn_ie, sm->ap_rsn_ie_len,
				ie->rsn_ie, ie->rsn_ie_len))) {
		wpa_report_ie_mismatch(sm, "IE in 3/4 msg does not match "
				       "with IE in Beacon/ProbeResp",
				       src_addr, ie->wpa_ie, ie->wpa_ie_len,
				       ie->rsn_ie, ie->rsn_ie_len);
		return -1;
	}

	if (sm->proto == WPA_PROTO_WPA &&
	    ie->rsn_ie && sm->ap_rsn_ie == NULL && sm->rsn_enabled) {
		wpa_report_ie_mismatch(sm, "Possible downgrade attack "
				       "detected - RSN was enabled and RSN IE "
				       "was in msg 3/4, but not in "
				       "Beacon/ProbeResp",
				       src_addr, ie->wpa_ie, ie->wpa_ie_len,
				       ie->rsn_ie, ie->rsn_ie_len);
		return -1;
	}

	return 0;
}
#else
int wpa_supplicant_validate_ie_impl(struct wpa_sm *sm,
				      unsigned char *src_addr,
				      struct wpa_eapol_ie_parse *ie)
{
    int i;

    if (src_addr != NULL) {
        wpa_printf(MSG_DEBUG, "[IE]WPA: src_addr:");
        for(i=0;i<6;i++) {
            wpa_printf(MSG_DEBUG, "%02x", *(src_addr + i));
        }
        wpa_printf(MSG_DEBUG, "\r\n");
    }

    if(ie != NULL){
        wpa_printf(MSG_DEBUG, "[IE]WPA: ie->rsn_ie_len:%ld \r\n", ie->rsn_ie_len);
        if(ie->rsn_ie != NULL) {
            wpa_printf(MSG_DEBUG, "[IE]WPA: rsn_ie:");
            for(i=0; i<ie->rsn_ie_len; i++) {
                wpa_printf(MSG_DEBUG, "%02x", *(ie->rsn_ie + i) );
            }
            wpa_printf(MSG_DEBUG, "\r\n");
        }
    }

	if (sm->ap_wpa_ie == NULL && sm->ap_rsn_ie == NULL) {
		wpa_printf(MSG_DEBUG, "[IE]WPA: No WPA/RSN IE for this AP known. Trying to get from scan results \r\n");
		if (wpa_sm_get_beacon_ie(sm) < 0) {
			wpa_printf(MSG_WARNING, "[IE]WPA: Could not find AP from the scan results \r\n");
		} else {
			wpa_printf(MSG_DEBUG, "[IE]WPA: Found the current AP from updated scan results \r\n");
		}
	}

//TBD
#if 0
	if (ie->wpa_ie == NULL && ie->rsn_ie == NULL &&
	    (sm->ap_wpa_ie || sm->ap_rsn_ie)) {
		wpa_report_ie_mismatch(sm, "IE in 3/4 msg does not match with IE in Beacon/ProbeResp (no IE?)",
				       src_addr, ie->wpa_ie, ie->wpa_ie_len,
				       ie->rsn_ie, ie->rsn_ie_len);
		return -1;
	}

	if ((ie->wpa_ie && sm->ap_wpa_ie &&
	     (ie->wpa_ie_len != sm->ap_wpa_ie_len ||
	      os_memcmp(ie->wpa_ie, sm->ap_wpa_ie, ie->wpa_ie_len) != 0)) ||
	    (ie->rsn_ie && sm->ap_rsn_ie &&
	     wpa_compare_rsn_ie(wpa_key_mgmt_ft(sm->key_mgmt),
				sm->ap_rsn_ie, sm->ap_rsn_ie_len,
				ie->rsn_ie, ie->rsn_ie_len))) {



		wpa_report_ie_mismatch(sm, "IE in 3/4 msg does not match with IE in Beacon/ProbeResp",
				       src_addr, ie->wpa_ie, ie->wpa_ie_len,
				       ie->rsn_ie, ie->rsn_ie_len);
		return -1;
	}

    wpa_printf(MSG_DEBUG, "\r\n compare rsn ie pass \r\n");

	if (sm->proto == WPA_PROTO_WPA &&
	    ie->rsn_ie && sm->ap_rsn_ie == NULL && sm->rsn_enabled) {
		wpa_report_ie_mismatch(sm, "Possible downgrade attack "
				       "detected - RSN was enabled and RSN IE "
				       "was in msg 3/4, but not in "
				       "Beacon/ProbeResp",
				       src_addr, ie->wpa_ie, ie->wpa_ie_len,
				       ie->rsn_ie, ie->rsn_ie_len);
		return -1;
	}
#endif

	return 0;
}
#endif /* #if 0 */

/**
 * wpa_supplicant_send_4_of_4 - Send message 4 of WPA/RSN 4-Way Handshake
 * @sm: Pointer to WPA state machine data from wpa_sm_init()
 * @dst: Destination address for the frame
 * @key: Pointer to the EAPOL-Key frame header
 * @ver: Version bits from EAPOL-Key Key Info
 * @key_info: Key Info
 * @kde: KDEs to include the EAPOL-Key frame
 * @kde_len: Length of KDEs
 * @ptk: PTK to use for keyed hash and encryption
 * Returns: 0 on success, -1 on failure
 */
int wpa_supplicant_send_4_of_4_impl(struct wpa_sm *sm, const unsigned char *dst,
			       const struct wpa_eapol_key *key,
			       u16 ver, u16 key_info,
			       const u8 *kde, size_t kde_len,
			       struct wpa_ptk *ptk)
{
	size_t rlen;
	struct wpa_eapol_key *reply;
	u8 *rbuf;
    int i;
    int len_total;
    int ret;

    wpa_printf(MSG_DEBUG, "[4_4]WPA: ver:%x key_info:%x key->type:%d key->key_info:%x %x \r\n",
                          ver,
                          key_info,
                          key->type,
                          key->key_info[0],
                          key->key_info[1]);

    wpa_printf(MSG_DEBUG, "[4_4]WPA: key->key_length:%02x%02x key->key_data_length:%02x%02x \r\n",
                          key->key_length[0],
                          key->key_length[1],
                          key->key_data_length[0],
                          key->key_data_length[1]);

    wpa_printf(MSG_DEBUG, "[4_4]WPA: kde:");
    for(i=0; i< kde_len; i++) wpa_printf(MSG_DEBUG, "%02x ", *(kde + i));
    wpa_printf(MSG_DEBUG, "\r\n");

    wpa_printf(MSG_DEBUG, "[4_4]WPA: sm->ptk.kck:");
    for(i=0; i< 16; i++) wpa_printf(MSG_DEBUG, "%02x ", sm->ptk.kck[i]);
    wpa_printf(MSG_DEBUG, "\r\n");

    wpa_printf(MSG_DEBUG, "[4_4]WPA: ptk->kck:");
    for(i=0; i< 16; i++) wpa_printf(MSG_DEBUG, "%02x ", ptk->kck[i]);
    wpa_printf(MSG_DEBUG, "\r\n");

    wpa_printf(MSG_DEBUG, "[4_4]WPA: sm->snonce:");
    for(i=0; i< WPA_NONCE_LEN; i++) wpa_printf(MSG_DEBUG, "%02x ", sm->snonce[i]);
    wpa_printf(MSG_DEBUG, "\r\n");

    wpa_printf(MSG_DEBUG, "[4_4]WPA: key->key_mic:");
    for(i=0; i< 16; i++) wpa_printf(MSG_DEBUG, "%02x ", key->key_mic[i]);
    wpa_printf(MSG_DEBUG, "\r\n");

#if 0
	if (kde)
		wpa_hexdump(MSG_DEBUG, "WPA: KDE for msg 4/4", kde, kde_len);
#endif

    if (sm->proto == WPA_PROTO_RSN)
    {
        len_total = sizeof(*reply);
    }
    else
    {
        len_total = sizeof(*reply);
    }
#if 0
	rbuf = wpa_sm_alloc_eapol(sm, IEEE802_1X_TYPE_EAPOL_KEY, NULL,
				  len_total,
				  &rlen, (void *) &reply);
#endif
    rbuf = wpa_alloc_eapol(ver, IEEE802_1X_TYPE_EAPOL_KEY,
			  NULL, len_total,
			  &rlen, (void *) &reply);

	if (rbuf == NULL)
		return -1;

    wpa_printf(MSG_DEBUG, "[4_4]WPA: len_total:%d rlen:%d sm->proto:%d \r\n", len_total, rlen, sm->proto); //len_total:95 rlen:99

	reply->type = sm->proto == WPA_PROTO_RSN ? EAPOL_KEY_TYPE_RSN : EAPOL_KEY_TYPE_WPA;

	key_info &= WPA_KEY_INFO_SECURE;
	key_info |= ver | WPA_KEY_INFO_KEY_TYPE | WPA_KEY_INFO_MIC;
	WPA_PUT_BE16(reply->key_info, key_info);

    wpa_printf(MSG_DEBUG, "[4_4]WPA: key_info:%x reply->key_info:%x %x \r\n",
                          key_info,
                          reply->key_info[0],
                          reply->key_info[1]);

#if 0
	if (sm->proto == WPA_PROTO_RSN)
    {
		WPA_PUT_BE16(reply->key_length, 0);
	}else
	{
	    //os_memcpy(reply->key_length, key->key_length, 2);
        WPA_PUT_BE16(reply->key_length, 0);
    }
#else
    WPA_PUT_BE16(reply->key_length, 0);
#endif

    //os_memcpy(reply->key_nonce, sm->snonce, WPA_NONCE_LEN);

	os_memcpy(reply->replay_counter, key->replay_counter, WPA_REPLAY_COUNTER_LEN); //ok

	//WPA_PUT_BE16(reply->key_data_length, kde_len);
	//reply->key_data_length = 0;
    //os_memcpy(reply->key_length, key->key_length, sizeof(key->key_length));
    os_memset(reply->key_data_length, 0, sizeof(reply->key_data_length));

    wpa_printf(MSG_DEBUG, "[4_4]WPA: reply->key_length:%02x%02x reply->key_data_length:%02x%02x \r\n", reply->key_length[0], reply->key_length[1], reply->key_data_length[0], reply->key_data_length[1]);

#if 0
	if (kde)
		os_memcpy(reply + 1, kde, kde_len);
#endif

    ret = wpa_eapol_key_mic(ptk->kck, ver, rbuf, rlen, reply->key_mic);
    // Prevent compiler warnings
    (void)ret;

    wpa_printf(MSG_DEBUG, "[4_4]WPA: reply->key_mic:");
    for(i=0; i<16; i++) wpa_printf(MSG_DEBUG, "%02x ", reply->key_mic[i]);
    wpa_printf(MSG_DEBUG, "\r\n");

	wpa_printf(MSG_DEBUG, "[4_4]WPA: Sending EAPOL-Key 4/4 \r\n");

	wpa_eapol_key_send(sm, ptk->kck, ver, dst, ETH_P_EAPOL, rbuf, rlen, reply->key_mic);

	return 0;
}


void wpa_supplicant_process_3_of_4_impl(struct wpa_sm *sm,
                                        const struct wpa_eapol_key *key,
                                        u16 ver)
{
    u16 key_info, keylen, len;
    const u8 *pos;
    struct wpa_eapol_ie_parse ie;
    int i;
    hap_control_t *hap_temp;    //I0064 0000318
    hap_temp=get_hap_control_struct();  //I0064 0000318

    wpa_sm_set_state(sm, WPA_4WAY_HANDSHAKE);
    wpa_printf(MSG_DEBUG, "[3_4]WPA: RX message 3 of 4-Way Handshake from "
                          MACSTR " (ver=%d) \r\n", MAC2STR(sm->bssid), ver);

    key_info = WPA_GET_BE16(key->key_info); //0x13ca => OK
    pos = (const u8 *) (key + 1);
    len = WPA_GET_BE16(key->key_data_length);

    wpa_printf(MSG_DEBUG, "[3_4]WPA: key_info:%x len:%d \r\n", key_info, len);
    wpa_hexdump(MSG_DEBUG, "[3_4]WPA: WPA: IE KeyData", pos, len);

    wpa_supplicant_parse_ies(pos, len, &ie);

    wpa_printf(MSG_DEBUG, "[3_4]WPA: after wpa_supplicant_parse_ies, os_strlen(ie.gtk):%d \r\n", os_strlen((const char*)(ie.gtk)));

    if (ie.gtk != NULL) { //enter
        wpa_printf(MSG_DEBUG, "[3_4]WPA: ie.gtk:");
        for(i=0; i< os_strlen((const char*)(ie.gtk)); i++) wpa_printf(MSG_DEBUG, "%02x ", *(ie.gtk + i));
        wpa_printf(MSG_DEBUG, "\r\n");
    }

    if (ie.gtk && !(key_info & WPA_KEY_INFO_ENCR_KEY_DATA)) { //no enter
        wpa_printf(MSG_WARNING, "[3_4]WPA: GTK IE in unencrypted key data \r\n");
        goto failed;
    }

    if (wpa_supplicant_validate_ie(sm, (unsigned char *) sm->bssid, &ie) < 0)
    {
        wpa_printf(MSG_WARNING, "[3_4]WPA: Validate IE fail \r\n");
        goto failed;
    }

    wpa_printf(MSG_DEBUG, "[3_4]WPA: sm->anonce:");
    for(i=0; i<WPA_NONCE_LEN; i++) wpa_printf(MSG_DEBUG, "%02x ", sm->anonce[i]);
    wpa_printf(MSG_DEBUG, "\r\n");

    wpa_printf(MSG_DEBUG, "[3_4]WPA: key->key_nonce:");
    for(i=0; i<WPA_NONCE_LEN; i++) wpa_printf(MSG_DEBUG, "%02x ", key->key_nonce[i]);
    wpa_printf(MSG_DEBUG, "\r\n");

    if (os_memcmp(sm->anonce, key->key_nonce, WPA_NONCE_LEN) != 0) {
        wpa_printf(MSG_WARNING, "[3_4]WPA: ANonce from message 1 of 4-Way "
               "Handshake differs from 3 of 4-Way Handshake - drop"
               " packet (src=" MACSTR ") \r\n", MAC2STR(sm->bssid));
        goto failed;
    }

    keylen = WPA_GET_BE16(key->key_length);

    wpa_printf(MSG_DEBUG, "[3_4]WPA: keylen:%d pairwise_cipher:%d \r\n", keylen, sm->pairwise_cipher);

    switch (sm->pairwise_cipher) {
    case WPA_CIPHER_CCMP:
        if (keylen != 16) {
            wpa_printf(MSG_WARNING, "[3_4]WPA: Invalid CCMP key length %d (src=" MACSTR ") \r\n", keylen, MAC2STR(sm->bssid));
            goto failed;
        }
        break;
    case WPA_CIPHER_TKIP:
        if (keylen != 32) {
            wpa_printf(MSG_WARNING, "[3_4]WPA: WPA: Invalid TKIP key length %d (src=" MACSTR ") \r\n",
                   keylen, MAC2STR(sm->bssid));
            goto failed;
        }
        break;
    }

    wpa_printf(MSG_DEBUG, "[3_4]WPA: key_info:%x \r\n", key_info);

    wpa_printf(MSG_DEBUG, "[3_4]WPA: sm->ptk.kck:");
    for(i=0; i<16; i++) wpa_printf(MSG_DEBUG, "%02x ", sm->ptk.kck[i]);
    wpa_printf(MSG_DEBUG, "\r\n");

    wpa_printf(MSG_DEBUG, "[3_4]WPA: sm->ptk.kek:");
    for(i=0; i<16; i++) wpa_printf(MSG_DEBUG, "%02x ", sm->ptk.kek[i]);
    wpa_printf(MSG_DEBUG, "\r\n");

    wpa_printf(MSG_DEBUG, "[3_4]WPA: sm->ptk.tk1:");
    for(i=0; i<16; i++) wpa_printf(MSG_DEBUG, "%02x ", sm->ptk.tk1[i]);
    wpa_printf(MSG_DEBUG, "\r\n");

    /* SNonce was successfully used in msg 3/4, so mark it to be renewed
     * for the next 4-Way Handshake. If msg 3 is received again, the old
     * SNonce will still be used to avoid changing PTK. */
    sm->renew_snonce = 1;

    wpa_printf(MSG_DEBUG, "[3_4]WPA: key_info:%x \r\n", key_info);

    if (key_info & WPA_KEY_INFO_INSTALL) {
        wpa_printf(MSG_DEBUG, "[3_4]WPA: call wpa_supplicant_install_ptk \r\n");
        if (wpa_supplicant_install_ptk(sm, key))
            goto failed;
    }


    if (wpa_supplicant_send_4_of_4(sm, sm->bssid, key, ver, key_info, NULL, 0, &sm->ptk)) {
        wpa_printf(MSG_WARNING, "[3_4]WPA: wpa_supplicant_send_4_of_4 return fail \r\n");
        goto failed;
    }


#if 0
	if (key_info & WPA_KEY_INFO_SECURE) {
		wpa_sm_mlme_setprotection(
			sm, sm->bssid, MLME_SETPROTECTION_PROTECT_TYPE_RX,
			MLME_SETPROTECTION_KEY_TYPE_PAIRWISE);
		eapol_sm_notify_portValid(sm->eapol, TRUE);
	}
#endif

    //Group Key for WPA PSK, TBD here
#if 1
    wpa_sm_set_state(sm, WPA_GROUP_HANDSHAKE);

    wpa_printf(MSG_DEBUG, "[3_4]WPA: key_info:%x \r\n", key_info);
    wpa_printf(MSG_DEBUG, "[3_4]WPA: ie.gtk_len:%d ie.gtk:", ie.gtk_len);
    for(i=0; i<ie.gtk_len; i++) wpa_printf(MSG_DEBUG, "%02x ", *(ie.gtk + i));
    wpa_printf(MSG_DEBUG, "\r\n");

    if (ie.gtk && wpa_supplicant_pairwise_gtk(sm, key, ie.gtk, ie.gtk_len, key_info) < 0)
    {
        wpa_printf(MSG_DEBUG, "[3_4]WPA: RSN: Failed to configure GTK \r\n");
        goto failed;
    }
    //I0064  0000318_add_begin
    if (hap_temp->hap_en)
    {
        hiddenap_complete();
    }
    //I0064  0000318_add_end
    g_wifi_reconnection_counter = MAX_WIFI_RECONNECTION; //I0064  0000869
#endif

    wpa_clr_key_info();
    wpa_printf(MSG_INFO, "[3-4]WPA: secured connected\r\n");
    g_fastconn = 0;            //I0064  0000736
    wifi_sta_join_complete(1); // 1 means success
    send_port_security_done_event();
    wifi_mac_set_encrypt_eapol_frame(true);
    
    /* Set successfully connect info to Auto Connect list */
    switch(get_auto_connect_mode()) {
        case AUTO_CONNECT_MANUAL:
            add_auto_connect_list();
            //compatible auto/manual connect
            set_auto_connect_mode(AUTO_CONNECT_ENABLE);
            break;
        default:
            break;
    }

#if 0
    if (ieee80211w_set_keys(sm, &ie) < 0) {
        wpa_printf(MSG_DEBUG, "RSN: Failed to configure IGTK");
        goto failed;
    }
#endif

	return;

failed:
	//wpa_sm_deauthenticate(sm, WLAN_REASON_UNSPECIFIED);
	//need to trigger a deauthenticate here

    return;
}


int wpa_supplicant_process_1_of_2_rsn_impl(struct wpa_sm *sm,
					     const u8 *keydata,
					     size_t keydatalen,
					     u16 key_info,
					     struct wpa_gtk_data *gd)
{
	int maxkeylen;
	struct wpa_eapol_ie_parse ie;

	wpa_hexdump(MSG_DEBUG, "[1_2_r]WPA: RSN: msg 1/2 key data", keydata, keydatalen);

	wpa_supplicant_parse_ies(keydata, keydatalen, &ie);

    wpa_printf(MSG_DEBUG, "[1_2_r]WPA: ie.gtk:%d key_info:%x \r\n", ie.gtk, key_info);

	if (ie.gtk && !(key_info & WPA_KEY_INFO_ENCR_KEY_DATA)) {
		wpa_printf(MSG_WARNING, "[1_2_r]WPA: GTK IE in unencrypted key data \r\n");
		return -1;
	}

	if (ie.gtk == NULL) {
		wpa_printf(MSG_DEBUG, "[1_2_r]WPA: No GTK IE in Group Key msg 1/2 \r\n");
		return -1;
	}

	maxkeylen = gd->gtk_len = ie.gtk_len - 2;

	if (wpa_supplicant_check_group_cipher(sm->group_cipher, gd->gtk_len, maxkeylen, &gd->key_rsc_len, &gd->alg))
	{
	    wpa_printf(MSG_DEBUG, "[1_2_r]WPA: wpa_supplicant_check_group_cipher return fail \r\n");
		return -1;
    }

	wpa_hexdump(MSG_DEBUG, "[1_2_r]WPA: RSN: received GTK in group key handshake", ie.gtk, ie.gtk_len);

	gd->keyidx = ie.gtk[0] & 0x3;
	gd->tx = wpa_supplicant_gtk_tx_bit_workaround(sm, !!(ie.gtk[0] & BIT(2)));

	if (ie.gtk_len - 2 > sizeof(gd->gtk)) {
		wpa_printf(MSG_DEBUG, "[1_2_r]WPA: RSN: Too long GTK in GTK IE (len=%lu) \r\n", (unsigned long) ie.gtk_len - 2);
		return -1;
	}
	os_memcpy(gd->gtk, ie.gtk + 2, ie.gtk_len - 2);

#if 0
	if (ieee80211w_set_keys(sm, &ie) < 0)
		wpa_printf(MSG_DEBUG, "RSN: Failed to configure IGTK");
#endif

	return 0;
}


int wpa_supplicant_process_1_of_2_wpa_impl(struct wpa_sm *sm,
					     const struct wpa_eapol_key *key,
					     size_t keydatalen, int key_info,
					     size_t extra_len, u16 ver,
					     struct wpa_gtk_data *gd)
{
	size_t maxkeylen;
	u8 ek[32];

	gd->gtk_len = WPA_GET_BE16(key->key_length);
	maxkeylen = keydatalen;
	if (keydatalen > extra_len) {
		wpa_printf(MSG_DEBUG, "[1_2_w]WPA: Truncated EAPOL-Key packet: key_data_length=%lu > extra_len=%lu \r\n",
			   (unsigned long) keydatalen,
			   (unsigned long) extra_len);
		return -1;
	}
	if (ver == WPA_KEY_INFO_TYPE_HMAC_SHA1_AES) {
		if (maxkeylen < 8) {
			wpa_printf(MSG_DEBUG, "[1_2_w]WPA: Too short maxkeylen (%lu) \r\n", (unsigned long) maxkeylen);
			return -1;
		}
		maxkeylen -= 8;
	}

	if (wpa_supplicant_check_group_cipher(sm->group_cipher,
					      gd->gtk_len, maxkeylen,
					      &gd->key_rsc_len, &gd->alg))
		return -1;

	gd->keyidx = (key_info & WPA_KEY_INFO_KEY_INDEX_MASK) >> WPA_KEY_INFO_KEY_INDEX_SHIFT;
	if (ver == WPA_KEY_INFO_TYPE_HMAC_MD5_RC4) {
		os_memcpy(ek, key->key_iv, 16);
		os_memcpy(ek + 16, sm->ptk.kek, 16);
		if (keydatalen > sizeof(gd->gtk)) {
			wpa_printf(MSG_WARNING, "[1_2_w]WPA: RC4 key data too long (%lu) \r\n", (unsigned long) keydatalen);
			return -1;
		}
		os_memcpy(gd->gtk, key + 1, keydatalen);
		if (rc4_skip(ek, 32, 256, gd->gtk, keydatalen)) {
			wpa_printf(MSG_ERROR, "[1_2_w]WPA: RC4 failed");
			return -1;
		}
	} else if (ver == WPA_KEY_INFO_TYPE_HMAC_SHA1_AES) {
		if (keydatalen % 8) {
			wpa_printf(MSG_WARNING, "[1_2_w]WPA: Unsupported AES-WRAP len %lu \r\n", (unsigned long) keydatalen);
			return -1;
		}
		if (maxkeylen > sizeof(gd->gtk)) {
			wpa_printf(MSG_WARNING, "[1_2_w]WPA: AES-WRAP key data too long (keydatalen=%lu maxkeylen=%lu) \r\n", (unsigned long) keydatalen, (unsigned long) maxkeylen);
			return -1;
		}
		if (aes_unwrap(sm->ptk.kek, maxkeylen / 8,(const u8 *) (key + 1), gd->gtk)) {
			wpa_printf(MSG_WARNING, "[1_2_w]WPA: AES unwrap failed - could not decrypt GTK \r\n");
			return -1;
		}
	} else {
		wpa_printf(MSG_WARNING, "[1_2_w]WPA: Unsupported key_info type %d \r\n", ver);
		return -1;
	}
	gd->tx = wpa_supplicant_gtk_tx_bit_workaround( sm, !!(key_info & WPA_KEY_INFO_TXRX));
	return 0;
}


int wpa_supplicant_send_2_of_2_impl(struct wpa_sm *sm,
				      const struct wpa_eapol_key *key,
				      int ver, u16 key_info)
{
	size_t rlen;
	struct wpa_eapol_key *reply;
	u8 *rbuf;
    int ret;

    wpa_printf(MSG_DEBUG, "[2_2][GTK]WPA: ver:%d key_info:%x \r\n", ver, key_info);

#if 0
	rbuf = wpa_sm_alloc_eapol(sm, IEEE802_1X_TYPE_EAPOL_KEY, NULL,
				  sizeof(*reply), &rlen, (void *) &reply);
#endif
    rbuf = wpa_alloc_eapol(ver, IEEE802_1X_TYPE_EAPOL_KEY,
			  NULL, sizeof(*reply),
			  &rlen, (void *) &reply);

	if (rbuf == NULL)
		return -1;

	reply->type = sm->proto == WPA_PROTO_RSN ?
		EAPOL_KEY_TYPE_RSN : EAPOL_KEY_TYPE_WPA;
	key_info &= WPA_KEY_INFO_KEY_INDEX_MASK;
	key_info |= ver | WPA_KEY_INFO_MIC | WPA_KEY_INFO_SECURE;

    wpa_printf(MSG_DEBUG, "[2_2][GTK]WPA: key_info(after):%x \r\n", key_info);

	WPA_PUT_BE16(reply->key_info, key_info);
	//os_memcpy(reply->key_info, key->key_info, sizeof(reply->key_info));

    wpa_printf(MSG_DEBUG, "[2_2][GTK]WPA: reply->key_info:%x key->key_length:%d reply->key_length:%d \r\n", reply->key_info, key->key_length, reply->key_length);
    wpa_printf(MSG_DEBUG, "[2_2][GTK]WPA: key->replay_counter:%d reply->replay_counter:%d \r\n", key->replay_counter, reply->replay_counter);

	if (sm->proto == WPA_PROTO_RSN)
		WPA_PUT_BE16(reply->key_length, 0);
	else
		os_memcpy(reply->key_length, key->key_length, 2);
    //os_memset(reply->key_length, 0, sizeof(reply->key_length));

	os_memcpy(reply->replay_counter, key->replay_counter, WPA_REPLAY_COUNTER_LEN);

	WPA_PUT_BE16(reply->key_data_length, 0);
	//os_memset(reply->key_data_length, 0, sizeof(reply->key_data_length));

    wpa_printf(MSG_DEBUG, "[2_2][GTK]WPA: reply->key_info:%x %x \r\n", reply->key_info[0], reply->key_info[1]);
    wpa_printf(MSG_DEBUG, "[2_2][GTK]WPA: reply->key_length[]:0x%x 0x%x \r\n", reply->key_length[0], reply->key_length[1]);
    wpa_printf(MSG_DEBUG, "[2_2][GTK]WPA: reply->key_data_length[]:0x%x 0x%x \r\n", reply->key_data_length[0], reply->key_data_length[1]);

    
    wpa_printf(MSG_DEBUG, "\r\n [GTK] wpa_supplicant_send_2_of_2, reply->replay_counter[]:0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x \r\n",
                              reply->replay_counter[0], reply->replay_counter[1], reply->replay_counter[2], reply->replay_counter[3],
                              reply->replay_counter[4], reply->replay_counter[5], reply->replay_counter[6], reply->replay_counter[7]);
    
    //ret = wpa_eapol_key_mic(ptk.kck, ver, rbuf, rlen, reply->key_mic);
    ret = wpa_eapol_key_mic(g_ptk.kck, ver, rbuf, rlen, reply->key_mic);
    // Prevent compiler warnings
    (void)ret;

#if 0
	wpa_eapol_key_send(sm, sm->ptk.kck, ver, sm->bssid, ETH_P_EAPOL,
			   rbuf, rlen, reply->key_mic);
#else
    wpa_eapol_key_send(sm, g_ptk.kck, ver, sm->bssid, ETH_P_EAPOL, rbuf, rlen, reply->key_mic);

#endif

	return 0;
}


void wpa_supplicant_process_1_of_2_impl(struct wpa_sm *sm,
					  const unsigned char *src_addr,
					  const struct wpa_eapol_key *key,
					  int extra_len, u16 ver)
{
	u16 key_info, keydatalen;
	int rekey, ret;
	struct wpa_gtk_data gd;

	os_memset(&gd, 0, sizeof(gd));

    wpa_printf(MSG_DEBUG, "[1_2][GTK]WPA: src_addr:" MACSTR " extra_len:%d ver:%d \r\n",
                          MAC2STR(src_addr), extra_len, ver);

	rekey = wpa_sm_get_state(sm) == WPA_COMPLETED;
	wpa_printf(MSG_DEBUG, "[1_2][GTK]WPA: RX message 1 of Group Key Handshake from "
		   MACSTR " (ver=%d) \r\n", MAC2STR(src_addr), ver);

	key_info = WPA_GET_BE16(key->key_info);
	keydatalen = WPA_GET_BE16(key->key_data_length);

    wpa_printf(MSG_DEBUG, "[1_2][GTK]WPA: rekey:%d key_info:%x keydatalen:%d sm->proto:%d \r\n",
                          rekey, key_info, keydatalen, sm->proto);

	if (sm->proto == WPA_PROTO_RSN) {
		ret = wpa_supplicant_process_1_of_2_rsn(sm,
							(const u8 *) (key + 1),
							keydatalen, key_info,
							&gd);
	} else {
		ret = wpa_supplicant_process_1_of_2_wpa(sm, key, keydatalen,
							key_info, extra_len,
							ver, &gd);
	}

	wpa_sm_set_state(sm, WPA_GROUP_HANDSHAKE);

	if (ret)
		goto failed;

#if 0
	if (wpa_supplicant_install_gtk(sm, &gd, key->key_rsc) ||
	    wpa_supplicant_send_2_of_2(sm, key, ver, key_info))
		goto failed;
#else

    ret = wpa_supplicant_install_gtk(sm, &gd, key->key_rsc);
    wpa_printf(MSG_DEBUG, "[1_2][GTK]WPA: wpa_supplicant_install_gtk ret:%d \r\n", ret);

    ret = wpa_supplicant_send_2_of_2(sm, key, ver, key_info);
    wpa_printf(MSG_DEBUG, "[1_2][GTK]WPA: wpa_supplicant_send_2_of_2 ret:%d \r\n", ret);

#endif

	if (rekey) {
		wpa_msg(sm->ctx->msg_ctx, MSG_DEBUG, "[1_2][GTK]WPA: Group rekeying completed with " MACSTR " [GTK=%s] \r\n", MAC2STR(sm->bssid), wpa_cipher_txt(sm->group_cipher));
        //wpa_sm_cancel_auth_timeout(sm);
		wpa_sm_set_state(sm, WPA_COMPLETED);
	} else {
		wpa_supplicant_key_neg_complete(sm, sm->bssid, key_info & WPA_KEY_INFO_SECURE);
	}
	return;

failed:
	wpa_sm_deauthenticate(sm, WLAN_REASON_UNSPECIFIED);
}


int wpa_supplicant_verify_eapol_key_mic_impl(struct wpa_sm *sm,
					       struct wpa_eapol_key *key,
					       u16 ver,
					       const u8 *buf, size_t len)
{
	u8 mic[16];
	int ok = 0;
    int i;

    wpa_printf(MSG_DEBUG, "[KEY][MIC]WPA: ver:%d len:%ld sm->tptk_set:%d \r\n", ver, len, sm->tptk_set);
    wpa_printf(MSG_DEBUG, "[KEY][MIC]WPA: key->key_mic:");
    for(i=0; i<16; i++) wpa_printf(MSG_DEBUG, "%02x ", key->key_mic[i]);
    wpa_printf(MSG_DEBUG, "\r\n");

	os_memcpy(mic, key->key_mic, 16);

	if (sm->tptk_set) {
		os_memset(key->key_mic, 0, 16);

        wpa_printf(MSG_DEBUG, "[KEY][MIC]WPA: sm->tptk.kck:");
        for(i=0;i<16;i++) wpa_printf(MSG_DEBUG, "%02x ", sm->tptk.kck[i]);
        wpa_printf(MSG_DEBUG, "\r\n");

        wpa_printf(MSG_DEBUG, "[KEY][MIC]WPA: buf:");
        for(i=0;i<len;i++) wpa_printf(MSG_DEBUG, "%02x ", *(buf + i));
        wpa_printf(MSG_DEBUG, "\r\n");

		wpa_eapol_key_mic(sm->tptk.kck, ver, buf, len, key->key_mic);

		if (os_memcmp(mic, key->key_mic, 16) != 0) {
            wpa_printf(MSG_DEBUG, "[KEY][MIC]WPA: MIC compare fail \r\n");
			wpa_printf(MSG_WARNING, "[KEY][MIC]WPA: Invalid EAPOL-Key MIC when using TPTK - ignoring TPTK \r\n");
		} else {
		    wpa_printf(MSG_DEBUG, "[KEY][MIC]WPA: MIC compare ok \r\n");
			ok = 1;
			sm->tptk_set = 0;
			sm->ptk_set = 1;
			os_memcpy(&sm->ptk, &sm->tptk, sizeof(sm->ptk));
		}
	}

    wpa_printf(MSG_DEBUG, "[KEY][MIC]WPA: ok:%d sm->ptk_set:%d \r\n", ok, sm->ptk_set);

	if (!ok && sm->ptk_set) {
		os_memset(key->key_mic, 0, 16);

        wpa_printf(MSG_DEBUG, "[KEY][MIC]WPA: sm->ptk.kck:");
        for(i=0;i<16;i++) wpa_printf(MSG_DEBUG, "%02x ", sm->ptk.kck[i]);
        wpa_printf(MSG_DEBUG, "\r\n");

		wpa_eapol_key_mic(sm->ptk.kck, ver, buf, len, key->key_mic);

		if (os_memcmp(mic, key->key_mic, 16) != 0) {
			wpa_printf(MSG_WARNING, "[KEY][MIC]WPA: Invalid EAPOL-Key MIC - dropping packet \r\n");
			return -1;
		}
		ok = 1;
	}

	if (!ok) {
		wpa_printf(MSG_WARNING, "[KEY][MIC]WPA: Could not verify EAPOL-Key MIC - dropping packet \r\n");
		return -1;
	}

	os_memcpy(sm->rx_replay_counter, key->replay_counter, WPA_REPLAY_COUNTER_LEN);
	sm->rx_replay_counter_set = 1;

	return 0;
}


/* Decrypt RSN EAPOL-Key key data (RC4 or AES-WRAP) */
int wpa_supplicant_decrypt_key_data_impl(struct wpa_sm *sm,
					   struct wpa_eapol_key *key, u16 ver)
{
	u16 keydatalen = WPA_GET_BE16(key->key_data_length);

	wpa_hexdump(MSG_DEBUG, "[KEY]WPA: encrypted key data", (u8 *) (key + 1), keydatalen);
	if (!sm->ptk_set) {
		wpa_printf(MSG_WARNING, "[KEY]WPA: PTK not available, cannot decrypt EAPOL-Key key data. \r\n");
		return -1;
	}

	/* Decrypt key data here so that this operation does not need
	 * to be implemented separately for each message type. */
	if (ver == WPA_KEY_INFO_TYPE_HMAC_MD5_RC4) {
		u8 ek[32];
		os_memcpy(ek, key->key_iv, 16);
		os_memcpy(ek + 16, sm->ptk.kek, 16);
		if (rc4_skip(ek, 32, 256, (u8 *) (key + 1), keydatalen)) {
			wpa_printf(MSG_ERROR, "[KEY]WPA: RC4 failed \r\n");
			return -1;
		}
	} else if (ver == WPA_KEY_INFO_TYPE_HMAC_SHA1_AES ||
		   ver == WPA_KEY_INFO_TYPE_AES_128_CMAC) {
		u8 *buf;
		if (keydatalen % 8) {
			wpa_printf(MSG_WARNING, "[KEY]WPA: Unsupported AES-WRAP len %d \r\n", keydatalen);
			return -1;
		}
		keydatalen -= 8; /* AES-WRAP adds 8 bytes */
		buf = os_malloc(keydatalen);
		if (buf == NULL) {
			wpa_printf(MSG_WARNING, "[KEY]WPA: No memory for AES-UNWRAP buffer \r\n");
			return -1;
		}
		if (aes_unwrap(sm->ptk.kek, keydatalen / 8,
			       (u8 *) (key + 1), buf)) {
			os_free(buf);
			wpa_printf(MSG_WARNING, "[KEY]WPA: AES unwrap failed - could not decrypt EAPOL-Key key data \r\n");
			return -1;
		}
		os_memcpy(key + 1, buf, keydatalen);
		os_free(buf);
		WPA_PUT_BE16(key->key_data_length, keydatalen);
	} else {
		wpa_printf(MSG_WARNING, "[KEY]WPA: Unsupported key_info type %d \r\n", ver);
		return -1;
	}
	wpa_hexdump_key(MSG_DEBUG, "[KEY]WPA: decrypted EAPOL-Key key data", (u8 *) (key + 1), keydatalen);
	return 0;
}


void wpa_eapol_key_dump_impl(const struct wpa_eapol_key *key)
{
#ifndef CONFIG_NO_STDOUT_DEBUG
	u16 key_info = WPA_GET_BE16(key->key_info);

	wpa_printf(MSG_DEBUG, "\r\n[DUMP] EAPOL-Key type=%d \r\n", key->type);//  2
	wpa_printf(MSG_DEBUG, "[DUMP] key_info 0x%x (ver=%d keyidx=%d rsvd=%d %s%s%s%s%s%s%s%s) \r\n",
		   key_info, key_info & WPA_KEY_INFO_TYPE_MASK,
		   (key_info & WPA_KEY_INFO_KEY_INDEX_MASK) >>
		   WPA_KEY_INFO_KEY_INDEX_SHIFT,
		   (key_info & (BIT(13) | BIT(14) | BIT(15))) >> 13,
		   key_info & WPA_KEY_INFO_KEY_TYPE ? "Pairwise" : "Group",
		   key_info & WPA_KEY_INFO_INSTALL ? " Install" : "",
		   key_info & WPA_KEY_INFO_ACK ? " Ack" : "",
		   key_info & WPA_KEY_INFO_MIC ? " MIC" : "",
		   key_info & WPA_KEY_INFO_SECURE ? " Secure" : "",
		   key_info & WPA_KEY_INFO_ERROR ? " Error" : "",
		   key_info & WPA_KEY_INFO_REQUEST ? " Request" : "",
		   key_info & WPA_KEY_INFO_ENCR_KEY_DATA ? " Encr" : "");  //key_info:0x8a=138 ver:2 keyidx:0 reserved:0 "Pairwise Ack"
	wpa_printf(MSG_DEBUG,  "[DUMP] key_length=%u key_data_length=%u \r\n", WPA_GET_BE16(key->key_length), WPA_GET_BE16(key->key_data_length));  //key_length:16  key_data_length:22
	wpa_hexdump(MSG_DEBUG, "[DUMP] replay_counter", key->replay_counter, WPA_REPLAY_COUNTER_LEN);  //replay_counter: 00 00 00 00 00 00 00 00
    wpa_hexdump(MSG_DEBUG, "[DUMP] key_nonce", key->key_nonce, WPA_NONCE_LEN); //key_nonce: 59 b5 5e d7 b5 ad 96 8d 6a 4f 6d 67 47 eb 5c 59 a1 41 ae f2 80 0e 0f ca ad 47 b0 99 da 80 ef 1a
    wpa_hexdump(MSG_DEBUG, "[DUMP] key_iv", key->key_iv, 16);                  //key_iv: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    wpa_hexdump(MSG_DEBUG, "[DUMP] key_rsc", key->key_rsc, 8);                 //key_rsc: 00 00 00 00 00 00 00 00
    wpa_hexdump(MSG_DEBUG, "[DUMP] key_id (reserved)", key->key_id, 8);        //key_id: 00 00 00 00 00 00 00 00
    wpa_hexdump(MSG_DEBUG, "[DUMP] key_mic", key->key_mic, 16);                //key_mic: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    wpa_printf(MSG_DEBUG, "\r\n");
#endif /* CONFIG_NO_STDOUT_DEBUG */
}


/**
 * wpa_sm_rx_eapol - Process received WPA EAPOL frames
 * @sm: Pointer to WPA state machine data from wpa_sm_init()
 * @src_addr: Source MAC address of the EAPOL packet
 * @buf: Pointer to the beginning of the EAPOL data (EAPOL header)
 * @len: Length of the EAPOL frame
 * Returns: 1 = WPA EAPOL-Key processed, 0 = not a WPA EAPOL-Key, -1 failure
 *
 * This function is called for each received EAPOL frame. Other than EAPOL-Key
 * frames can be skipped if filtering is done elsewhere. wpa_sm_rx_eapol() is
 * only processing WPA and WPA2 EAPOL-Key frames.
 *
 * The received EAPOL-Key packets are validated and valid packets are replied
 * to. In addition, key material (PTK, GTK) is configured at the end of a
 * successful key handshake.
 */
int wpa_sm_rx_eapol_impl(struct wpa_sm *sm, const u8 *src_addr,
		    const u8 *buf, size_t len)
{
	size_t plen, data_len, extra_len;
	struct ieee802_1x_hdr *hdr;
	struct wpa_eapol_key *key;
	u16 key_info, ver;
	u8 *tmp;
	int ret = -1;
	struct wpa_peerkey *peerkey = NULL;

    wpa_printf(MSG_DEBUG, "[KEY]WPA: len:%d \r\n", len);

	if (len < sizeof(*hdr) + sizeof(*key)) { //no enter
		wpa_printf(MSG_DEBUG, "[KEY]WPA: EAPOL frame too short to be a WPA EAPOL-Key (len %lu, expecting at least %lu) \r\n", (unsigned long) len, (unsigned long) sizeof(*hdr) + sizeof(*key));
		return 0;
	}

	tmp = os_malloc(len);
	if (tmp == NULL) //no enter
		return -1;
	os_memcpy(tmp, buf, len);

	hdr = (struct ieee802_1x_hdr *) tmp;
	key = (struct wpa_eapol_key *) (hdr + 1);
	plen = be_to_host16(hdr->length);
	data_len = plen + sizeof(*hdr);
	wpa_printf(MSG_DEBUG, "[KEY]WPA: IEEE 802.1X RX: version=%d type=%d length=%lu \r\n", hdr->version, hdr->type, (unsigned long) plen); //hdr->version:1 hdr->type:3 length:117

    os_memset(&msg_1_4_hdr, 0, sizeof(msg_1_4_hdr));
    msg_1_4_hdr.version = hdr->version;
    msg_1_4_hdr.type = hdr->type;
    msg_1_4_hdr.length = hdr->length;
    wpa_printf(MSG_DEBUG, "[KEY]WPA: put ieee802_1x_hdr to msg_1_4_hdr \r\n");
    wpa_printf(MSG_DEBUG, "[KEY]WPA: msg_1_4_hdr.version:%d \r\n", msg_1_4_hdr.version);
    wpa_printf(MSG_DEBUG, "[KEY]WPA: msg_1_4_hdr.type:%d \r\n", msg_1_4_hdr.type);
    wpa_printf(MSG_DEBUG, "[KEY]WPA: msg_1_4_hdr.length:%d \r\n", msg_1_4_hdr.length);

	if (hdr->version < EAPOL_VERSION) { //enter this, but do nothing
		/* TODO: backwards compatibility */
	}

	if (hdr->type != IEEE802_1X_TYPE_EAPOL_KEY) { //no enter
		wpa_printf(MSG_DEBUG, "[KEY]WPA: EAPOL frame (type %u) discarded, not a Key frame \r\n", hdr->type);
		ret = 0;
		goto out;
	}

    wpa_printf(MSG_DEBUG, "[KEY]WPA: plen:%ld len:%d sizeof(*hdr):%ld sizeof(*key):%ld \r\n", plen, len, sizeof(*hdr), sizeof(*key));

#if 0
	if (plen > len - sizeof(*hdr) || plen < sizeof(*key)) { //no enter
		wpa_printf(MSG_DEBUG, "\r\n wpa_sm_rx_eapol, WPA: EAPOL frame payload size %lu "
			   "invalid (frame size %lu)",
			   (unsigned long) plen, (unsigned long) len);
		ret = 0;
		goto out;
	}
#endif

    wpa_printf(MSG_DEBUG, "[KEY]WPA: EAPOL-Key type (%d) ", key->type); //WPA2 PSK =>key->type:2

	if (key->type != EAPOL_KEY_TYPE_WPA && key->type != EAPOL_KEY_TYPE_RSN) //no enter
	{
		wpa_printf(MSG_DEBUG, "[KEY]WPA: EAPOL-Key type (%d) unknown, discarded", key->type);
		ret = 0;
		goto out;
	}

	wpa_eapol_key_dump(key);

	//eapol_sm_notify_lower_layer_success(sm->eapol, 0);
	wpa_hexdump(MSG_MSGDUMP, "[KEY]WPA: RX EAPOL-Key", tmp, len);

    wpa_printf(MSG_DEBUG, "[KEY]WPA: len:%d data_len:%d ", len, data_len); //len:125   data_len:121
	if (data_len < len) { //enter
		wpa_printf(MSG_DEBUG, "[KEY]WPA: ignoring %lu bytes after the IEEE 802.1X data ", (unsigned long) len - data_len); //4
	}
	key_info = WPA_GET_BE16(key->key_info); //message 1 => key_info: 0x8a==138
	                                        //message 2 => key_info:0x13ca

    wpa_printf(MSG_DEBUG, "[KEY]WPA: key_info:%x g_key_info:%x \r\n", key_info, g_key_info);
    wpa_printf(MSG_DEBUG, "[KEY]WPA: frame_len:%x g_frame_len:%x \r\n", len, g_frame_len);

#if 0
    //Set key_info to global variable g_key_info for next EAPOL-Key Frame checking, it the same with it, it means it's the duplicate frame, can ignore it
    if(key_info == g_key_info ||
       key_info == g_key_info_1_4 ||
       key_info == g_key_info_3_4 )
    { // Skip this frame
        wpa_printf(MSG_DEBUG, "[KEY]WPA: key_info == g_key_info, Skip this EAPOL-Key Frame \r\n");
        ret = 0;
        goto out;
    }

    //Set len to global variable g_frame_len for next EAPOL-Key Frame checking, it the same with it, it means it's the duplicate frame, can ignore it
    if(len == g_frame_len)
    { // Skip this frame
        wpa_printf(MSG_DEBUG, "[KEY]WPA: len == g_frame_len, Skip this EAPOL-Key Frame \r\n");
        ret = 0;
        goto out;
    }
#endif

    g_key_info = key_info;
    wpa_printf(MSG_DEBUG, "[KEY]WPA: set g_key_info=%x \r\n", g_key_info);

    g_frame_len = len;
    wpa_printf(MSG_DEBUG, "[KEY]WPA: set g_frame_len=%x \r\n", g_frame_len);


	ver = key_info & WPA_KEY_INFO_TYPE_MASK;
    wpa_printf(MSG_DEBUG, "[KEY]WPA: ver:%d \r\n", ver);   // 2
	if (ver != WPA_KEY_INFO_TYPE_HMAC_MD5_RC4 &&
	    ver != WPA_KEY_INFO_TYPE_HMAC_SHA1_AES) {  //no enter
		wpa_printf(MSG_DEBUG, "[KEY]WPA: Unsupported EAPOL-Key descriptor version %d. \r\n", ver);
		goto out;
	}

    wpa_printf(MSG_DEBUG, "[KEY]WPA: pairwise_cipher:%d group_cipher:%d \r\n", sm->pairwise_cipher, sm->group_cipher);

	if (sm->pairwise_cipher == WPA_CIPHER_CCMP &&
	    ver != WPA_KEY_INFO_TYPE_HMAC_SHA1_AES) { //no enter
		wpa_printf(MSG_DEBUG, "[KEY]WPA: CCMP is used, but EAPOL-Key descriptor version (%d) is not 2. \r\n", ver);
		if (sm->group_cipher != WPA_CIPHER_CCMP &&
		    !(key_info & WPA_KEY_INFO_KEY_TYPE)) {
			/* Earlier versions of IEEE 802.11i did not explicitly
			 * require version 2 descriptor for all EAPOL-Key
			 * packets, so allow group keys to use version 1 if
			 * CCMP is not used for them. */
			wpa_printf(MSG_DEBUG, "[KEY]WPA: Backwards compatibility: allow invalid version for non-CCMP group keys \r\n");
		} else
			goto out;
	}

#ifdef CONFIG_PEERKEY
	for (peerkey = sm->peerkey; peerkey; peerkey = peerkey->next) {
		if (os_memcmp(peerkey->addr, src_addr, ETH_ALEN) == 0)
			break;
	}

	if (!(key_info & WPA_KEY_INFO_SMK_MESSAGE) && peerkey) {
		if (!peerkey->initiator && peerkey->replay_counter_set &&
		    os_memcmp(key->replay_counter, peerkey->replay_counter,
			      WPA_REPLAY_COUNTER_LEN) <= 0) {
			wpa_printf(MSG_WARNING, "RSN: EAPOL-Key Replay "
				   "Counter did not increase (STK) - dropping "
				   "packet");
			goto out;
		} else if (peerkey->initiator) {
			u8 _tmp[WPA_REPLAY_COUNTER_LEN];
			os_memcpy(_tmp, key->replay_counter,
				  WPA_REPLAY_COUNTER_LEN);
			inc_byte_array(_tmp, WPA_REPLAY_COUNTER_LEN);
			if (os_memcmp(_tmp, peerkey->replay_counter,
				      WPA_REPLAY_COUNTER_LEN) != 0) {
				wpa_printf(MSG_DEBUG, "RSN: EAPOL-Key Replay "
					   "Counter did not match (STK) - "
					   "dropping packet");
				goto out;
			}
		}
	}

	if (peerkey && peerkey->initiator && (key_info & WPA_KEY_INFO_ACK)) {
		wpa_printf(MSG_DEBUG, "RSN: Ack bit in key_info from STK peer");
		goto out;
	}
#endif /* CONFIG_PEERKEY */

    wpa_printf(MSG_DEBUG, "[KEY]WPA: sm->rx_replay_counter_set:%d \r\n", sm->rx_replay_counter_set); //sm->rx_replay_counter_set:0
    wpa_printf(MSG_DEBUG, "[KEY]WPA: key->replay_counter[0~7]: %x %x %x %x %x %x %x %x \r\n", //key->replay_counter[0~7]: 0 0 0 0 0 0 0 1
                            key->replay_counter[0],
                            key->replay_counter[1],
                            key->replay_counter[2],
                            key->replay_counter[3],
                            key->replay_counter[4],
                            key->replay_counter[5],
                            key->replay_counter[6],
                            key->replay_counter[7]);
    wpa_printf(MSG_DEBUG, "[KEY]WPA: sm->rx_replay_counter[0~7]: %x %x %x %x %x %x %x %x \r\n", //sm->rx_replay_counter[0~7]: 0 0 0 0 0 0 0 0
                            sm->rx_replay_counter[0],
                            sm->rx_replay_counter[1],
                            sm->rx_replay_counter[2],
                            sm->rx_replay_counter[3],
                            sm->rx_replay_counter[4],
                            sm->rx_replay_counter[5],
                            sm->rx_replay_counter[6],
                            sm->rx_replay_counter[7]);



#if 0
	if (!peerkey && sm->rx_replay_counter_set &&
	    os_memcmp(key->replay_counter, sm->rx_replay_counter,
		      WPA_REPLAY_COUNTER_LEN) <= 0) { //no enter
		wpa_printf(MSG_WARNING, "\r\n wpa_sm_rx_eapol, WPA: EAPOL-Key Replay Counter did not"
			   " increase - dropping packet ");
		goto out;
	}

	if (os_memcmp(key->replay_counter, sm->rx_replay_counter, WPA_REPLAY_COUNTER_LEN) <= 0)
    {
		wpa_printf(MSG_WARNING, "\r\n wpa_sm_rx_eapol, WPA: EAPOL-Key Replay Counter did not"
			   " increase - dropping packet");
		goto out;
	}
#endif


#if 0
	if (!(key_info & (WPA_KEY_INFO_ACK | WPA_KEY_INFO_SMK_MESSAGE))
#ifdef CONFIG_PEERKEY
	    && (peerkey == NULL || !peerkey->initiator)
#endif /* CONFIG_PEERKEY */
	) { //no enter
		wpa_printf(MSG_DEBUG, "\r\n wpa_sm_rx_eapol, WPA: No Ack bit in key_info ");
		goto out;
	}
#endif

	if (key_info & WPA_KEY_INFO_REQUEST) { //no enter
		wpa_printf(MSG_DEBUG, "[KEY]WPA: EAPOL-Key with Request bit - dropped \r\n");
		goto out;
	}

#if 0
	if ((key_info & WPA_KEY_INFO_MIC) && !peerkey &&
	    wpa_supplicant_verify_eapol_key_mic(sm, key, ver, tmp, data_len)) //no enter
		goto out;
#else
    if ((key_info & WPA_KEY_INFO_MIC) && wpa_supplicant_verify_eapol_key_mic(sm, key, ver, tmp, data_len)) //message 1 => no enter
    {                                                                                                      //message 2 => enter, fail
        wpa_printf(MSG_DEBUG, "[KEY]WPA: key mic verify fail \r\n");
		goto out;
    }
#endif

#ifdef CONFIG_PEERKEY
	if ((key_info & WPA_KEY_INFO_MIC) && peerkey &&
	    peerkey_verify_eapol_key_mic(sm, peerkey, key, ver, tmp, data_len))
		goto out;
#endif /* CONFIG_PEERKEY */

	extra_len = data_len - sizeof(*hdr) - sizeof(*key);

    wpa_printf(MSG_DEBUG, "[KEY]WPA: data_len:%d sizeof(*hdr):%d sizeof(*key):%d extra_len:%d \r\n",
                         data_len,
                         sizeof(*hdr),
                         sizeof(*key),
                         extra_len); //data_len:121 sizeof(*hdr):4 sizeof(*key):95 extra_len:22

	if (WPA_GET_BE16(key->key_data_length) > extra_len) { //no enter
		wpa_msg(sm->ctx->msg_ctx, MSG_DEBUG, "[KEY]WPA: Invalid EAPOL-Key frame - key_data overflow (%d > %lu) \r\n", WPA_GET_BE16(key->key_data_length), (unsigned long) extra_len);
		goto out;
	}
	extra_len = WPA_GET_BE16(key->key_data_length);

	if (sm->proto == WPA_PROTO_RSN &&
	    (key_info & WPA_KEY_INFO_ENCR_KEY_DATA)) { //no enter
	    wpa_printf(MSG_DEBUG, "[KEY]WPA: wpa_s->proto == WPA_PROTO_RSN \r\n");
		if (wpa_supplicant_decrypt_key_data(sm, key, ver))
			goto out;
		extra_len = WPA_GET_BE16(key->key_data_length);
	}

    wpa_printf(MSG_DEBUG, "[KEY]WPA: key_info:%x \r\n", key_info);

	if (key_info & WPA_KEY_INFO_KEY_TYPE) { //enter
		if (key_info & WPA_KEY_INFO_KEY_INDEX_MASK) { //no enter
			wpa_printf(MSG_WARNING, "[KEY]WPA: Ignored EAPOL-Key (Pairwise) with non-zero key index \r\n");
			goto out;
		}
		if (peerkey) { //no enter
            wpa_printf(MSG_DEBUG, "[KEY]WPA: Do peerkey_rx_eapol_4way \r\n");
			/* PeerKey 4-Way Handshake */
			peerkey_rx_eapol_4way(sm, peerkey, key, key_info, ver);
		} else if (key_info & WPA_KEY_INFO_MIC) { //no enter
		    wpa_printf(MSG_DEBUG, "[KEY]WPA: do wpa_supplicant_process_3_of_4 \r\n");

            //Save key_info of message 3/4
            g_key_info_3_4 = key_info;
            
            wifi_mac_set_encrypt_eapol_frame(false);
            
			/* 3/4 4-Way Handshake */
			wpa_supplicant_process_3_of_4(sm, key, ver);
		} else { //enter
    		wpa_printf(MSG_DEBUG, "[KEY]WPA: do wpa_supplicant_process_1_of_4 \r\n");

            //Save key_info of message 1/4
            g_key_info_1_4 = key_info;

			/* 1/4 4-Way Handshake */
			wpa_supplicant_process_1_of_4(sm, src_addr, key, ver);
		}
	} else if (key_info & WPA_KEY_INFO_SMK_MESSAGE) {
		/* PeerKey SMK Handshake */
		peerkey_rx_eapol_smk(sm, src_addr, key, extra_len, key_info, ver);
	} else {
		if (key_info & WPA_KEY_INFO_MIC) {
            wpa_printf(MSG_WARNING, "[KEY]WPA: call wpa_supplicant_process_1_of_2 \r\n");

			/* 1/2 Group Key Handshake */
			wpa_supplicant_process_1_of_2(sm, src_addr, key, extra_len, ver);
		} else {
			wpa_printf(MSG_WARNING, "[KEY]WPA: EAPOL-Key (Group) without Mic bit - dropped \r\n");
		}
	}

	ret = 1;
out:
	os_free(tmp);
	return ret;
}


#ifdef CONFIG_CTRL_IFACE
int wpa_cipher_bits(int cipher)
{
	switch (cipher) {
	case WPA_CIPHER_CCMP:
		return 128;
	case WPA_CIPHER_TKIP:
		return 256;
	case WPA_CIPHER_WEP104:
		return 104;
	case WPA_CIPHER_WEP40:
		return 40;
	default:
		return 0;
	}
}


u32 wpa_key_mgmt_suite(struct wpa_sm *sm)
{
	switch (sm->key_mgmt) {
	case WPA_KEY_MGMT_IEEE8021X:
		return (sm->proto == WPA_PROTO_RSN ?
			RSN_AUTH_KEY_MGMT_UNSPEC_802_1X :
			WPA_AUTH_KEY_MGMT_UNSPEC_802_1X);
	case WPA_KEY_MGMT_PSK:
		return (sm->proto == WPA_PROTO_RSN ?
			RSN_AUTH_KEY_MGMT_PSK_OVER_802_1X :
			WPA_AUTH_KEY_MGMT_PSK_OVER_802_1X);
	case WPA_KEY_MGMT_WPA_NONE:
		return WPA_AUTH_KEY_MGMT_NONE;
	default:
		return 0;
	}
}


u32 wpa_cipher_suite(struct wpa_sm *sm, int cipher)
{
	switch (cipher) {
	case WPA_CIPHER_CCMP:
		return (sm->proto == WPA_PROTO_RSN ?
			RSN_CIPHER_SUITE_CCMP : WPA_CIPHER_SUITE_CCMP);
	case WPA_CIPHER_TKIP:
		return (sm->proto == WPA_PROTO_RSN ?
			RSN_CIPHER_SUITE_TKIP : WPA_CIPHER_SUITE_TKIP);
	case WPA_CIPHER_WEP104:
		return (sm->proto == WPA_PROTO_RSN ?
			RSN_CIPHER_SUITE_WEP104 : WPA_CIPHER_SUITE_WEP104);
	case WPA_CIPHER_WEP40:
		return (sm->proto == WPA_PROTO_RSN ?
			RSN_CIPHER_SUITE_WEP40 : WPA_CIPHER_SUITE_WEP40);
	case WPA_CIPHER_NONE:
		return (sm->proto == WPA_PROTO_RSN ?
			RSN_CIPHER_SUITE_NONE : WPA_CIPHER_SUITE_NONE);
	default:
		return 0;
	}
}


#define RSN_SUITE "%02x-%02x-%02x-%d"
#define RSN_SUITE_ARG(s) \
((s) >> 24) & 0xff, ((s) >> 16) & 0xff, ((s) >> 8) & 0xff, (s) & 0xff

/**
 * wpa_sm_get_mib - Dump text list of MIB entries
 * @sm: Pointer to WPA state machine data from wpa_sm_init()
 * @buf: Buffer for the list
 * @buflen: Length of the buffer
 * Returns: Number of bytes written to buffer
 *
 * This function is used fetch dot11 MIB variables.
 */
int wpa_sm_get_mib_impl(struct wpa_sm *sm, char *buf, size_t buflen)
{
	char pmkid_txt[PMKID_LEN * 2 + 1];
	int rsna, ret;
	size_t len;

	if (sm->cur_pmksa) {
		wpa_snprintf_hex(pmkid_txt, sizeof(pmkid_txt),
				 sm->cur_pmksa->pmkid, PMKID_LEN);
	} else
		pmkid_txt[0] = '\0';

	if ((wpa_key_mgmt_wpa_psk(sm->key_mgmt) ||
	     wpa_key_mgmt_wpa_ieee8021x(sm->key_mgmt)) &&
	    sm->proto == WPA_PROTO_RSN)
		rsna = 1;
	else
		rsna = 0;

	ret = os_snprintf(buf, buflen,
			  "dot11RSNAOptionImplemented=TRUE\n"
			  "dot11RSNAPreauthenticationImplemented=TRUE\n"
			  "dot11RSNAEnabled=%s\n"
			  "dot11RSNAPreauthenticationEnabled=%s\n"
			  "dot11RSNAConfigVersion=%d\n"
			  "dot11RSNAConfigPairwiseKeysSupported=5\n"
			  "dot11RSNAConfigGroupCipherSize=%d\n"
			  "dot11RSNAConfigPMKLifetime=%d\n"
			  "dot11RSNAConfigPMKReauthThreshold=%d\n"
			  "dot11RSNAConfigNumberOfPTKSAReplayCounters=1\n"
			  "dot11RSNAConfigSATimeout=%d\n",
			  rsna ? "TRUE" : "FALSE",
			  rsna ? "TRUE" : "FALSE",
			  RSN_VERSION,
			  wpa_cipher_bits(sm->group_cipher),
			  sm->dot11RSNAConfigPMKLifetime,
			  sm->dot11RSNAConfigPMKReauthThreshold,
			  sm->dot11RSNAConfigSATimeout);
	if (ret < 0 || (size_t) ret >= buflen)
		return 0;
	len = ret;

	ret = os_snprintf(
		buf + len, buflen - len,
		"dot11RSNAAuthenticationSuiteSelected=" RSN_SUITE "\n"
		"dot11RSNAPairwiseCipherSelected=" RSN_SUITE "\n"
		"dot11RSNAGroupCipherSelected=" RSN_SUITE "\n"
		"dot11RSNAPMKIDUsed=%s\n"
		"dot11RSNAAuthenticationSuiteRequested=" RSN_SUITE "\n"
		"dot11RSNAPairwiseCipherRequested=" RSN_SUITE "\n"
		"dot11RSNAGroupCipherRequested=" RSN_SUITE "\n"
		"dot11RSNAConfigNumberOfGTKSAReplayCounters=0\n"
		"dot11RSNA4WayHandshakeFailures=%u\n",
		RSN_SUITE_ARG(wpa_key_mgmt_suite(sm)),
		RSN_SUITE_ARG(wpa_cipher_suite(sm, sm->pairwise_cipher)),
		RSN_SUITE_ARG(wpa_cipher_suite(sm, sm->group_cipher)),
		pmkid_txt,
		RSN_SUITE_ARG(wpa_key_mgmt_suite(sm)),
		RSN_SUITE_ARG(wpa_cipher_suite(sm, sm->pairwise_cipher)),
		RSN_SUITE_ARG(wpa_cipher_suite(sm, sm->group_cipher)),
		sm->dot11RSNA4WayHandshakeFailures);
	if (ret >= 0 && (size_t) ret < buflen)
		len += ret;

	return (int) len;
}
#endif /* CONFIG_CTRL_IFACE */


/**
 * wpa_sm_init - Initialize WPA state machine
 * @ctx: Context pointer for callbacks; this needs to be an allocated buffer
 * Returns: Pointer to the allocated WPA state machine data
 *
 * This function is used to allocate a new WPA state machine and the returned
 * value is passed to all WPA state machine calls.
 */
struct wpa_sm * wpa_sm_init(struct wpa_sm_ctx *ctx)
{
	struct wpa_sm *sm;

	sm = os_zalloc(sizeof(*sm));
	if (sm == NULL)
		return NULL;

	sm->renew_snonce = 1;
	sm->ctx = ctx;

	return sm;
}


/**
 * wpa_sm_deinit - Deinitialize WPA state machine
 * @sm: Pointer to WPA state machine data from wpa_sm_init()
 */
void wpa_sm_deinit_impl(struct wpa_sm *sm)
{
	if (sm == NULL)
		return;
	//pmksa_cache_deinit(sm->pmksa);
	//eloop_cancel_timeout(wpa_sm_start_preauth, sm, NULL);
	//eloop_cancel_timeout(wpa_sm_rekey_ptk, sm, NULL);
	os_free(sm->assoc_wpa_ie);
	os_free(sm->ap_wpa_ie);
	os_free(sm->ap_rsn_ie);
	os_free(sm->ctx);
	peerkey_deinit(sm);
	os_free(sm);
}


/**
 * wpa_sm_notify_assoc - Notify WPA state machine about association
 * @sm: Pointer to WPA state machine data from wpa_sm_init()
 * @bssid: The BSSID of the new association
 *
 * This function is called to let WPA state machine know that the connection
 * was established.
 */
void wpa_sm_notify_assoc_impl(struct wpa_sm *sm, const u8 *bssid)
{
	int clear_ptk = 1;

	if (sm == NULL)
		return;

	wpa_printf(MSG_DEBUG, "[NOTI]WPA: Association event - clear replay counter \r\n");

	os_memcpy(sm->bssid, bssid, ETH_ALEN);
	os_memset(sm->rx_replay_counter, 0, WPA_REPLAY_COUNTER_LEN);
	sm->rx_replay_counter_set = 0;
	sm->renew_snonce = 1;
	if (os_memcmp(sm->preauth_bssid, bssid, ETH_ALEN) == 0)
		rsn_preauth_deinit(sm);

	if (clear_ptk) {
		/*
		 * IEEE 802.11, 8.4.10: Delete PTK SA on (re)association if
		 * this is not part of a Fast BSS Transition.
		 */
		wpa_printf(MSG_DEBUG, "[NOTI]WPA: Clear old PTK \r\n");
		sm->ptk_set = 0;
		sm->tptk_set = 0;
	}
}


/**
 * wpa_sm_notify_disassoc - Notify WPA state machine about disassociation
 * @sm: Pointer to WPA state machine data from wpa_sm_init()
 *
 * This function is called to let WPA state machine know that the connection
 * was lost. This will abort any existing pre-authentication session.
 */
void wpa_sm_notify_disassoc_impl(struct wpa_sm *sm)
{
    wpa_printf(MSG_DEBUG, "[NOTI]WPA: Disassociation event - deinit \r\n");

	rsn_preauth_deinit(sm);
	if (wpa_sm_get_state(sm) == WPA_4WAY_HANDSHAKE)
		sm->dot11RSNA4WayHandshakeFailures++;
}


/**
 * wpa_sm_set_pmk - Set PMK
 * @sm: Pointer to WPA state machine data from wpa_sm_init()
 * @pmk: The new PMK
 * @pmk_len: The length of the new PMK in bytes
 *
 * Configure the PMK for WPA state machine.
 */
void wpa_sm_set_pmk_impl(struct wpa_sm *sm, const u8 *pmk, size_t pmk_len)
{
#if 0
    int i;
    wpa_printf(MSG_DEBUG, "\r\n wpa_sm_set_pmk");
    wpa_printf(MSG_DEBUG, "\r\n wpa_sm_set_pmk, pmk[%d]: ", pmk_len);
    for(i=0;i<pmk_len;i++){
        wpa_printf(MSG_DEBUG, "%x ", *(pmk + i));
    }
#endif

	if (sm == NULL)
		return;

    //wpa_printf(MSG_DEBUG, "\r\n wpa_sm_set_pmk, copy pmk to sm->pmk \r\n");

	sm->pmk_len = pmk_len;
	os_memcpy(sm->pmk, pmk, pmk_len);
}


/**
 * wpa_sm_set_pmk_from_pmksa - Set PMK based on the current PMKSA
 * @sm: Pointer to WPA state machine data from wpa_sm_init()
 *
 * Take the PMK from the current PMKSA into use. If no PMKSA is active, the PMK
 * will be cleared.
 */
void wpa_sm_set_pmk_from_pmksa_impl(struct wpa_sm *sm)
{
	if (sm == NULL)
		return;

	if (sm->cur_pmksa) {
		sm->pmk_len = sm->cur_pmksa->pmk_len;
		os_memcpy(sm->pmk, sm->cur_pmksa->pmk, sm->pmk_len);
	} else {
		sm->pmk_len = PMK_LEN;
		os_memset(sm->pmk, 0, PMK_LEN);
	}
}


/**
 * wpa_sm_set_config - Notification of current configration change
 * @sm: Pointer to WPA state machine data from wpa_sm_init()
 * @config: Pointer to current network configuration
 *
 * Notify WPA state machine that configuration has changed. config will be
 * stored as a backpointer to network configuration. This can be %NULL to clear
 * the stored pointed.
 */
void wpa_sm_set_config_impl(struct wpa_sm *sm, struct rsn_supp_config *config)
{
	if (!sm)
		return;

	if (config) {
		sm->network_ctx = config->network_ctx;
		sm->peerkey_enabled = config->peerkey_enabled;
		sm->allowed_pairwise_cipher = config->allowed_pairwise_cipher;
		sm->proactive_key_caching = config->proactive_key_caching;
		sm->eap_workaround = config->eap_workaround;
		sm->eap_conf_ctx = config->eap_conf_ctx;
		if (config->ssid) {
			os_memcpy(sm->ssid, config->ssid, config->ssid_len);
			sm->ssid_len = config->ssid_len;
		} else
			sm->ssid_len = 0;
		sm->wpa_ptk_rekey = config->wpa_ptk_rekey;
	} else {
		sm->network_ctx = NULL;
		sm->peerkey_enabled = 0;
		sm->allowed_pairwise_cipher = 0;
		sm->proactive_key_caching = 0;
		sm->eap_workaround = 0;
		sm->eap_conf_ctx = NULL;
		sm->ssid_len = 0;
		sm->wpa_ptk_rekey = 0;
	}
	//if (config == NULL || config->network_ctx != sm->network_ctx)
	//	pmksa_cache_notify_reconfig(sm->pmksa);
}


/**
 * wpa_sm_set_own_addr - Set own MAC address
 * @sm: Pointer to WPA state machine data from wpa_sm_init()
 * @addr: Own MAC address
 */
void wpa_sm_set_own_addr_impl(struct wpa_sm *sm, const u8 *addr)
{
	if (sm)
		os_memcpy(sm->own_addr, addr, ETH_ALEN);
#if 0
    wpa_printf(MSG_ERROR, "\r\n wpa_sm_set_own_addr, sm->own_addr[6]:%02x:%02x:%02x:%02x:%02x:%02x \r\n",
                      sm->own_addr[0],
                      sm->own_addr[1],
                      sm->own_addr[2],
                      sm->own_addr[3],
                      sm->own_addr[4],
                      sm->own_addr[5]);
#endif
}


/**
 * wpa_sm_set_eapol - Set EAPOL state machine pointer
 * @sm: Pointer to WPA state machine data from wpa_sm_init()
 * @eapol: Pointer to EAPOL state machine allocated with eapol_sm_init()
 */
void wpa_sm_set_eapol_impl(struct wpa_sm *sm, struct eapol_sm *eapol)
{
	if (sm)
		sm->eapol = eapol;
}


/**
 * wpa_sm_set_param - Set WPA state machine parameters
 * @sm: Pointer to WPA state machine data from wpa_sm_init()
 * @param: Parameter field
 * @value: Parameter value
 * Returns: 0 on success, -1 on failure
 */
int wpa_sm_set_param_impl(struct wpa_sm *sm, enum wpa_sm_conf_params param,
		     unsigned int value)
{
	int ret = 0;

	if (sm == NULL)
		return -1;

	switch (param) {
	case RSNA_PMK_LIFETIME:
		if (value > 0)
			sm->dot11RSNAConfigPMKLifetime = value;
		else
			ret = -1;

		break;
	case RSNA_PMK_REAUTH_THRESHOLD:
		if (value > 0 && value <= 100)
			sm->dot11RSNAConfigPMKReauthThreshold = value;
		else
			ret = -1;
		break;
	case RSNA_SA_TIMEOUT:
		if (value > 0)
			sm->dot11RSNAConfigSATimeout = value;
		else
			ret = -1;
		break;
	case WPA_PARAM_PROTO:
		sm->proto = value;
		break;
	case WPA_PARAM_PAIRWISE:
		sm->pairwise_cipher = value;
		break;
	case WPA_PARAM_GROUP:
		sm->group_cipher = value;
		break;
	case WPA_PARAM_KEY_MGMT:
		sm->key_mgmt = value;
		break;
	case WPA_PARAM_RSN_ENABLED:
		sm->rsn_enabled = value;
		break;
	case WPA_PARAM_MFP:
		sm->mfp = value;
		break;
	default:
		break;
	}

	return ret;
}


/**
 * wpa_sm_get_param - Get WPA state machine parameters
 * @sm: Pointer to WPA state machine data from wpa_sm_init()
 * @param: Parameter field
 * Returns: Parameter value
 */
unsigned int wpa_sm_get_param_impl(struct wpa_sm *sm, enum wpa_sm_conf_params param)
{
	if (sm == NULL)
		return 0;

	switch (param) {
	case RSNA_PMK_LIFETIME:
		return sm->dot11RSNAConfigPMKLifetime;
	case RSNA_PMK_REAUTH_THRESHOLD:
		return sm->dot11RSNAConfigPMKReauthThreshold;
	case RSNA_SA_TIMEOUT:
		return sm->dot11RSNAConfigSATimeout;
	case WPA_PARAM_PROTO:
		return sm->proto;
	case WPA_PARAM_PAIRWISE:
		return sm->pairwise_cipher;
	case WPA_PARAM_GROUP:
		return sm->group_cipher;
	case WPA_PARAM_KEY_MGMT:
		return sm->key_mgmt;
	case WPA_PARAM_RSN_ENABLED:
		return sm->rsn_enabled;
	default:
		return 0;
	}
}


/**
 * wpa_sm_get_status - Get WPA state machine
 * @sm: Pointer to WPA state machine data from wpa_sm_init()
 * @buf: Buffer for status information
 * @buflen: Maximum buffer length
 * @verbose: Whether to include verbose status information
 * Returns: Number of bytes written to buf.
 *
 * Query WPA state machine for status information. This function fills in
 * a text area with current status information. If the buffer (buf) is not
 * large enough, status information will be truncated to fit the buffer.
 */
int wpa_sm_get_status_impl(struct wpa_sm *sm, char *buf, size_t buflen,
		      int verbose)
{
	char *pos = buf, *end = buf + buflen;
	int ret;

	ret = os_snprintf(pos, end - pos,
			  "pairwise_cipher=%s\n"
			  "group_cipher=%s\n"
			  "key_mgmt=%s\n",
			  wpa_cipher_txt(sm->pairwise_cipher),
			  wpa_cipher_txt(sm->group_cipher),
			  wpa_key_mgmt_txt(sm->key_mgmt, sm->proto));
	if (ret < 0 || ret >= end - pos)
		return pos - buf;
	pos += ret;
	return pos - buf;
}


/**
 * wpa_sm_set_assoc_wpa_ie_default - Generate own WPA/RSN IE from configuration
 * @sm: Pointer to WPA state machine data from wpa_sm_init()
 * @wpa_ie: Pointer to buffer for WPA/RSN IE
 * @wpa_ie_len: Pointer to the length of the wpa_ie buffer
 * Returns: 0 on success, -1 on failure
 */
int wpa_sm_set_assoc_wpa_ie_default_impl(struct wpa_sm *sm, u8 *wpa_ie,
				    size_t *wpa_ie_len)
{
	int res;

    wpa_printf(MSG_DEBUG, "[IE]WPA: generate WPA IE default \r\n");

	if (sm == NULL)
		return -1;

	res = wpa_gen_wpa_ie(sm, wpa_ie, *wpa_ie_len);
	if (res < 0)
		return -1;
	*wpa_ie_len = res;

	wpa_hexdump(MSG_DEBUG, "[IE]WPA: Set own WPA IE default \r\n", wpa_ie, *wpa_ie_len);

	if (sm->assoc_wpa_ie == NULL) {
		/*
		 * Make a copy of the WPA/RSN IE so that 4-Way Handshake gets
		 * the correct version of the IE even if PMKSA caching is
		 * aborted (which would remove PMKID from IE generation).
		 */
		sm->assoc_wpa_ie = os_malloc(*wpa_ie_len);
		if (sm->assoc_wpa_ie == NULL)
			return -1;

		os_memcpy(sm->assoc_wpa_ie, wpa_ie, *wpa_ie_len);
		sm->assoc_wpa_ie_len = *wpa_ie_len;
	}

	return 0;
}


/**
 * wpa_sm_set_assoc_wpa_ie - Set own WPA/RSN IE from (Re)AssocReq
 * @sm: Pointer to WPA state machine data from wpa_sm_init()
 * @ie: Pointer to IE data (starting from id)
 * @len: IE length
 * Returns: 0 on success, -1 on failure
 *
 * Inform WPA state machine about the WPA/RSN IE used in (Re)Association
 * Request frame. The IE will be used to override the default value generated
 * with wpa_sm_set_assoc_wpa_ie_default().
 */
int wpa_sm_set_assoc_wpa_ie_impl(struct wpa_sm *sm, const u8 *ie, size_t len)
{
    wpa_printf(MSG_DEBUG, "[IE]WPA: Set WPA IE \r\n");

	if (sm == NULL)
		return -1;

	os_free(sm->assoc_wpa_ie);
	if (ie == NULL || len == 0) {
		wpa_printf(MSG_DEBUG, "[IE]WPA: clearing own WPA/RSN IE \r\n");
		sm->assoc_wpa_ie = NULL;
		sm->assoc_wpa_ie_len = 0;
	} else {
		wpa_hexdump(MSG_DEBUG, "[IE]WPA: set own WPA/RSN IE \r\n", ie, len);
		sm->assoc_wpa_ie = os_malloc(len);
		if (sm->assoc_wpa_ie == NULL)
			return -1;
		os_memcpy(sm->assoc_wpa_ie, ie, len);
		sm->assoc_wpa_ie_len = len;
	}

	return 0;
}


/**
 * wpa_sm_set_ap_wpa_ie - Set AP WPA IE from Beacon/ProbeResp
 * @sm: Pointer to WPA state machine data from wpa_sm_init()
 * @ie: Pointer to IE data (starting from id)
 * @len: IE length
 * Returns: 0 on success, -1 on failure
 *
 * Inform WPA state machine about the WPA IE used in Beacon / Probe Response
 * frame.
 */
int wpa_sm_set_ap_wpa_ie_impl(struct wpa_sm *sm, const u8 *ie, size_t len)
{
    wpa_printf(MSG_DEBUG, "[IE]WPA: set AP WPA IE \r\n");

	if (sm == NULL)
		return -1;

	os_free(sm->ap_wpa_ie);
	if (ie == NULL || len == 0) {
		wpa_printf(MSG_DEBUG, "[IE]WPA: clearing AP WPA IE \r\n");
		sm->ap_wpa_ie = NULL;
		sm->ap_wpa_ie_len = 0;
	} else {
		wpa_hexdump(MSG_DEBUG, "[IE]WPA: set AP WPA IE \r\n", ie, len);
		sm->ap_wpa_ie = os_malloc(len);
		if (sm->ap_wpa_ie == NULL)
			return -1;

		os_memcpy(sm->ap_wpa_ie, ie, len);
		sm->ap_wpa_ie_len = len;
	}

	return 0;
}


/**
 * wpa_sm_set_ap_rsn_ie - Set AP RSN IE from Beacon/ProbeResp
 * @sm: Pointer to WPA state machine data from wpa_sm_init()
 * @ie: Pointer to IE data (starting from id)
 * @len: IE length
 * Returns: 0 on success, -1 on failure
 *
 * Inform WPA state machine about the RSN IE used in Beacon / Probe Response
 * frame.
 */
int wpa_sm_set_ap_rsn_ie_impl(struct wpa_sm *sm, const u8 *ie, size_t len)
{
    wpa_printf(MSG_DEBUG, "[IE]WPA: Set AP RSN IE \r\n");

	if (sm == NULL)
		return -1;

	os_free(sm->ap_rsn_ie);
	if (ie == NULL || len == 0) {
		wpa_printf(MSG_DEBUG, "[IE]WPA: clearing AP RSN IE \r\n");
		sm->ap_rsn_ie = NULL;
		sm->ap_rsn_ie_len = 0;
	} else {
		wpa_hexdump(MSG_DEBUG, "[IE]WPA: set AP RSN IE \r\n", ie, len);
		sm->ap_rsn_ie = os_malloc(len);
		if (sm->ap_rsn_ie == NULL)
			return -1;

		os_memcpy(sm->ap_rsn_ie, ie, len);
		sm->ap_rsn_ie_len = len;
	}

	return 0;
}


/**
 * wpa_sm_parse_own_wpa_ie - Parse own WPA/RSN IE
 * @sm: Pointer to WPA state machine data from wpa_sm_init()
 * @data: Pointer to data area for parsing results
 * Returns: 0 on success, -1 if IE is not known, or -2 on parsing failure
 *
 * Parse the contents of the own WPA or RSN IE from (Re)AssocReq and write the
 * parsed data into data.
 */
int wpa_sm_parse_own_wpa_ie_impl(struct wpa_sm *sm, struct wpa_ie_data *data)
{
    wpa_printf(MSG_DEBUG, "[IE]WPA: Parse WPA IE \r\n");

	if (sm == NULL || sm->assoc_wpa_ie == NULL) {
		wpa_printf(MSG_DEBUG, "[IE]WPA: No WPA/RSN IE available from association info \r\n");
		return -1;
	}
	if (wpa_parse_wpa_ie(sm->assoc_wpa_ie, sm->assoc_wpa_ie_len, data))
		return -2;
	return 0;
}


void wpa_sm_drop_sa_impl(struct wpa_sm *sm)
{
	wpa_printf(MSG_DEBUG, "[IE]WPA: Clear old PMK and PTK \r\n");
	sm->ptk_set = 0;
	sm->tptk_set = 0;
	os_memset(sm->pmk, 0, sizeof(sm->pmk));
	os_memset(&sm->ptk, 0, sizeof(sm->ptk));
	os_memset(&sm->tptk, 0, sizeof(sm->tptk));
}

int wpa_sm_has_ptk_impl(struct wpa_sm *sm)
{
	if (sm == NULL)
		return 0;
	return sm->ptk_set;
}

void wpa_clr_key_info_impl(void)
{
    g_key_info = 0;
    g_key_info_1_4 = 0;
    g_key_info_3_4 = 0;
    g_frame_len = 0;
    os_memset(&msg_1_4_hdr, 0, sizeof(msg_1_4_hdr));
}

void wpa_sm_set_state_impl(struct wpa_sm *sm, enum wpa_states state)
{
	WPA_ASSERT(sm->ctx->set_state);
	sm->ctx->set_state(sm->ctx->ctx, state);
}

enum wpa_states wpa_sm_get_state_impl(struct wpa_sm *sm)
{
	WPA_ASSERT(sm->ctx->get_state);
	return sm->ctx->get_state(sm->ctx->ctx);
}

void wpa_sm_deauthenticate_impl(struct wpa_sm *sm, int reason_code)
{
	WPA_ASSERT(sm->ctx->deauthenticate);
	sm->ctx->deauthenticate(sm->ctx->ctx, reason_code);
}

void wpa_sm_disassociate_impl(struct wpa_sm *sm, int reason_code)
{
	WPA_ASSERT(sm->ctx->disassociate);
	sm->ctx->disassociate(sm->ctx->ctx, reason_code);
}

int wpa_sm_set_key_impl(struct wpa_sm *sm, enum wpa_alg alg,
				 const u8 *addr, int key_idx, int set_tx,
				 const u8 *seq, size_t seq_len,
				 const u8 *key, size_t key_len)
{
	WPA_ASSERT(sm->ctx->set_key);
	return sm->ctx->set_key(sm->ctx->ctx, alg, addr, key_idx, set_tx,
				seq, seq_len, key, key_len);
}

void * wpa_sm_get_network_ctx_impl(struct wpa_sm *sm)
{
	WPA_ASSERT(sm->ctx->get_network_ctx);
	return sm->ctx->get_network_ctx(sm->ctx->ctx);
}

int wpa_sm_get_bssid_impl(struct wpa_sm *sm, u8 *bssid)
{
	WPA_ASSERT(sm->ctx->get_bssid);
	return sm->ctx->get_bssid(sm->ctx->ctx, bssid);
}

int wpa_sm_ether_send_impl(struct wpa_sm *sm, const u8 *dest,
				    u16 proto, const u8 *buf, size_t len)
{
	WPA_ASSERT(sm->ctx->ether_send);
	return sm->ctx->ether_send(sm->ctx->ctx, dest, proto, buf, len);
}

int wpa_sm_get_beacon_ie_impl(struct wpa_sm *sm)
{
	WPA_ASSERT(sm->ctx->get_beacon_ie);
	return sm->ctx->get_beacon_ie(sm->ctx->ctx);
}

void wpa_sm_cancel_auth_timeout_impl(struct wpa_sm *sm)
{
	WPA_ASSERT(sm->ctx->cancel_auth_timeout);
	sm->ctx->cancel_auth_timeout(sm->ctx->ctx);
}

u8 * wpa_sm_alloc_eapol_impl(struct wpa_sm *sm, u8 type,
				      const void *data, u16 data_len,
				      size_t *msg_len, void **data_pos)
{
	WPA_ASSERT(sm->ctx->alloc_eapol);
	return sm->ctx->alloc_eapol(sm->ctx->ctx, type, data, data_len,
				    msg_len, data_pos);
}

int wpa_sm_add_pmkid_impl(struct wpa_sm *sm, const u8 *bssid,
				   const u8 *pmkid)
{
	WPA_ASSERT(sm->ctx->add_pmkid);
	return sm->ctx->add_pmkid(sm->ctx->ctx, bssid, pmkid);
}

int wpa_sm_remove_pmkid_impl(struct wpa_sm *sm, const u8 *bssid,
				      const u8 *pmkid)
{
	WPA_ASSERT(sm->ctx->remove_pmkid);
	return sm->ctx->remove_pmkid(sm->ctx->ctx, bssid, pmkid);
}

int wpa_sm_mlme_setprotection_impl(struct wpa_sm *sm, const u8 *addr,
					    int protect_type, int key_type)
{
	WPA_ASSERT(sm->ctx->mlme_setprotection);
	return sm->ctx->mlme_setprotection(sm->ctx->ctx, addr, protect_type,
					   key_type);
}

int wpa_sm_update_ft_ies(struct wpa_sm *sm, const u8 *md,
				       const u8 *ies, size_t ies_len)
{
	if (sm->ctx->update_ft_ies)
		return sm->ctx->update_ft_ies(sm->ctx->ctx, md, ies, ies_len);
	return -1;
}

int wpa_sm_send_ft_action(struct wpa_sm *sm, u8 action,
					const u8 *target_ap,
					const u8 *ies, size_t ies_len)
{
	if (sm->ctx->send_ft_action)
		return sm->ctx->send_ft_action(sm->ctx->ctx, action, target_ap,
					       ies, ies_len);
	return -1;
}

int wpa_sm_mark_authenticated(struct wpa_sm *sm,
					    const u8 *target_ap)
{
	if (sm->ctx->mark_authenticated)
		return sm->ctx->mark_authenticated(sm->ctx->ctx, target_ap);
	return -1;
}


RET_DATA wpa_sm_set_state_fp_t wpa_sm_set_state;
RET_DATA wpa_sm_get_state_fp_t wpa_sm_get_state;
RET_DATA wpa_sm_deauthenticate_fp_t wpa_sm_deauthenticate;
RET_DATA wpa_sm_disassociate_fp_t wpa_sm_disassociate;
RET_DATA wpa_sm_set_key_fp_t wpa_sm_set_key;
RET_DATA wpa_sm_get_network_ctx_fp_t wpa_sm_get_network_ctx;
RET_DATA wpa_sm_get_bssid_fp_t wpa_sm_get_bssid;
RET_DATA wpa_sm_ether_send_fp_t wpa_sm_ether_send;
RET_DATA wpa_sm_get_beacon_ie_fp_t wpa_sm_get_beacon_ie;
RET_DATA wpa_sm_cancel_auth_timeout_fp_t wpa_sm_cancel_auth_timeout;
RET_DATA wpa_sm_alloc_eapol_fp_t wpa_sm_alloc_eapol;
RET_DATA wpa_sm_add_pmkid_fp_t wpa_sm_add_pmkid;
RET_DATA wpa_sm_remove_pmkid_fp_t wpa_sm_remove_pmkid;
RET_DATA wpa_sm_mlme_setprotection_fp_t wpa_sm_mlme_setprotection;
RET_DATA wpa_sm_deinit_fp_t wpa_sm_deinit;
RET_DATA wpa_sm_notify_assoc_fp_t wpa_sm_notify_assoc;
RET_DATA wpa_sm_notify_disassoc_fp_t wpa_sm_notify_disassoc;
RET_DATA wpa_sm_set_pmk_fp_t wpa_sm_set_pmk;
RET_DATA wpa_sm_set_pmk_from_pmksa_fp_t wpa_sm_set_pmk_from_pmksa;
RET_DATA wpa_sm_set_config_fp_t wpa_sm_set_config;
RET_DATA wpa_sm_set_own_addr_fp_t wpa_sm_set_own_addr;
RET_DATA wpa_sm_set_eapol_fp_t wpa_sm_set_eapol;
RET_DATA wpa_sm_set_assoc_wpa_ie_fp_t wpa_sm_set_assoc_wpa_ie;
RET_DATA wpa_sm_set_assoc_wpa_ie_default_fp_t wpa_sm_set_assoc_wpa_ie_default;
RET_DATA wpa_sm_set_ap_wpa_ie_fp_t wpa_sm_set_ap_wpa_ie;
RET_DATA wpa_sm_set_ap_rsn_ie_fp_t wpa_sm_set_ap_rsn_ie;
RET_DATA wpa_sm_set_param_fp_t wpa_sm_set_param;
RET_DATA wpa_sm_get_param_fp_t wpa_sm_get_param;
RET_DATA wpa_sm_get_status_fp_t wpa_sm_get_status;
RET_DATA wpa_sm_key_request_fp_t wpa_sm_key_request;
RET_DATA wpa_sm_rx_eapol_fp_t wpa_sm_rx_eapol;
RET_DATA wpa_sm_parse_own_wpa_ie_fp_t wpa_sm_parse_own_wpa_ie;
RET_DATA wpa_sm_drop_sa_fp_t wpa_sm_drop_sa;
RET_DATA wpa_sm_has_ptk_fp_t wpa_sm_has_ptk;
RET_DATA wpa_clr_key_info_fp_t wpa_clr_key_info;
RET_DATA wpa_supplicant_get_pmk_fp_t wpa_supplicant_get_pmk;
RET_DATA wpa_ether_send_fp_t wpa_ether_send;
RET_DATA wpa_eapol_key_send_fp_t wpa_eapol_key_send;
RET_DATA wpa_alloc_eapol_fp_t wpa_alloc_eapol;
RET_DATA wpa_supplicant_send_2_of_4_fp_t wpa_supplicant_send_2_of_4;
RET_DATA wpa_supplicant_send_4_of_4_fp_t wpa_supplicant_send_4_of_4;
RET_DATA wpa_derive_ptk_fp_t wpa_derive_ptk;
RET_DATA wpa_supplicant_process_1_of_4_fp_t wpa_supplicant_process_1_of_4;
RET_DATA wpa_sm_start_preauth_fp_t wpa_sm_start_preauth;
RET_DATA wpa_supplicant_key_neg_complete_fp_t wpa_supplicant_key_neg_complete;
RET_DATA wpa_sm_rekey_ptk_fp_t wpa_sm_rekey_ptk;
RET_DATA wpa_supplicant_install_ptk_fp_t wpa_supplicant_install_ptk;
RET_DATA wpa_supplicant_check_group_cipher_fp_t wpa_supplicant_check_group_cipher;
RET_DATA wpa_supplicant_install_gtk_fp_t wpa_supplicant_install_gtk;
RET_DATA wpa_supplicant_gtk_tx_bit_workaround_fp_t wpa_supplicant_gtk_tx_bit_workaround;
RET_DATA wpa_supplicant_pairwise_gtk_fp_t wpa_supplicant_pairwise_gtk;
RET_DATA wpa_report_ie_mismatch_fp_t wpa_report_ie_mismatch;
RET_DATA wpa_supplicant_validate_ie_fp_t wpa_supplicant_validate_ie;
RET_DATA wpa_supplicant_process_3_of_4_fp_t wpa_supplicant_process_3_of_4;
RET_DATA wpa_supplicant_process_1_of_2_rsn_fp_t wpa_supplicant_process_1_of_2_rsn;
RET_DATA wpa_supplicant_process_1_of_2_wpa_fp_t wpa_supplicant_process_1_of_2_wpa;
RET_DATA wpa_supplicant_send_2_of_2_fp_t wpa_supplicant_send_2_of_2;
RET_DATA wpa_supplicant_process_1_of_2_fp_t wpa_supplicant_process_1_of_2;
RET_DATA wpa_supplicant_verify_eapol_key_mic_fp_t wpa_supplicant_verify_eapol_key_mic;
RET_DATA wpa_supplicant_decrypt_key_data_fp_t wpa_supplicant_decrypt_key_data;
RET_DATA wpa_eapol_key_dump_fp_t wpa_eapol_key_dump;


/*
   Interface Initialization: WPA
 */
void wpa_func_init(void)
{
    wpa_sm_set_state                     = wpa_sm_set_state_impl;
    wpa_sm_get_state                     = wpa_sm_get_state_impl;
    wpa_sm_deauthenticate                = wpa_sm_deauthenticate_impl;
    wpa_sm_disassociate                  = wpa_sm_disassociate_impl;
    wpa_sm_set_key                       = wpa_sm_set_key_impl;
    wpa_sm_get_network_ctx               = wpa_sm_get_network_ctx_impl;
    wpa_sm_get_bssid                     = wpa_sm_get_bssid_impl;
    wpa_sm_ether_send                    = wpa_sm_ether_send_impl;
    wpa_sm_get_beacon_ie                 = wpa_sm_get_beacon_ie_impl;
    wpa_sm_cancel_auth_timeout           = wpa_sm_cancel_auth_timeout_impl;
    wpa_sm_alloc_eapol                   = wpa_sm_alloc_eapol_impl;
    wpa_sm_add_pmkid                     = wpa_sm_add_pmkid_impl;
    wpa_sm_remove_pmkid                  = wpa_sm_remove_pmkid_impl;
    wpa_sm_mlme_setprotection            = wpa_sm_mlme_setprotection_impl;
    wpa_sm_deinit                        = wpa_sm_deinit_impl;
    wpa_sm_notify_assoc                  = wpa_sm_notify_assoc_impl;
    wpa_sm_notify_disassoc               = wpa_sm_notify_disassoc_impl;
    wpa_sm_set_pmk                       = wpa_sm_set_pmk_impl;
    wpa_sm_set_pmk_from_pmksa            = wpa_sm_set_pmk_from_pmksa_impl;
    wpa_sm_set_config                    = wpa_sm_set_config_impl;
    wpa_sm_set_own_addr                  = wpa_sm_set_own_addr_impl;
    wpa_sm_set_eapol                     = wpa_sm_set_eapol_impl;
    wpa_sm_set_assoc_wpa_ie              = wpa_sm_set_assoc_wpa_ie_impl;
    wpa_sm_set_assoc_wpa_ie_default      = wpa_sm_set_assoc_wpa_ie_default_impl;
    wpa_sm_set_ap_wpa_ie                 = wpa_sm_set_ap_wpa_ie_impl;
    wpa_sm_set_ap_rsn_ie                 = wpa_sm_set_ap_rsn_ie_impl;
    wpa_sm_set_param                     = wpa_sm_set_param_impl;
    wpa_sm_get_param                     = wpa_sm_get_param_impl;
    wpa_sm_get_status                    = wpa_sm_get_status_impl;
    wpa_sm_key_request                   = wpa_sm_key_request_impl;
    wpa_sm_rx_eapol                      = wpa_sm_rx_eapol_impl;
    wpa_sm_parse_own_wpa_ie              = wpa_sm_parse_own_wpa_ie_impl;
    wpa_sm_drop_sa                       = wpa_sm_drop_sa_impl;
    wpa_sm_has_ptk                       = wpa_sm_has_ptk_impl;
    wpa_clr_key_info                     = wpa_clr_key_info_impl;
    wpa_supplicant_get_pmk               = wpa_supplicant_get_pmk_impl;
    wpa_ether_send                       = wpa_ether_send_impl;
    wpa_eapol_key_send                   = wpa_eapol_key_send_impl;
    wpa_alloc_eapol                      = wpa_alloc_eapol_impl;
    wpa_supplicant_send_2_of_4           = wpa_supplicant_send_2_of_4_impl;
    wpa_supplicant_send_4_of_4           = wpa_supplicant_send_4_of_4_impl;
    wpa_derive_ptk                       = wpa_derive_ptk_impl;
    wpa_supplicant_process_1_of_4        = wpa_supplicant_process_1_of_4_impl;
    wpa_sm_start_preauth                 = wpa_sm_start_preauth_impl;
    wpa_supplicant_key_neg_complete      = wpa_supplicant_key_neg_complete_impl;
    wpa_sm_rekey_ptk                     = wpa_sm_rekey_ptk_impl;
    wpa_supplicant_install_ptk           = wpa_supplicant_install_ptk_impl;
    wpa_supplicant_check_group_cipher    = wpa_supplicant_check_group_cipher_impl;
    wpa_supplicant_install_gtk           = wpa_supplicant_install_gtk_impl;
    wpa_supplicant_gtk_tx_bit_workaround = wpa_supplicant_gtk_tx_bit_workaround_impl;
    wpa_supplicant_pairwise_gtk          = wpa_supplicant_pairwise_gtk_impl;
    wpa_report_ie_mismatch               = wpa_report_ie_mismatch_impl;
    wpa_supplicant_validate_ie           = wpa_supplicant_validate_ie_impl;
    wpa_supplicant_process_3_of_4        = wpa_supplicant_process_3_of_4_impl;
    wpa_supplicant_process_1_of_2_rsn    = wpa_supplicant_process_1_of_2_rsn_impl;
    wpa_supplicant_process_1_of_2_wpa    = wpa_supplicant_process_1_of_2_wpa_impl;
    wpa_supplicant_send_2_of_2           = wpa_supplicant_send_2_of_2_impl;
    wpa_supplicant_process_1_of_2        = wpa_supplicant_process_1_of_2_impl;
    wpa_supplicant_verify_eapol_key_mic  = wpa_supplicant_verify_eapol_key_mic_impl;
    wpa_supplicant_decrypt_key_data      = wpa_supplicant_decrypt_key_data_impl;
    wpa_eapol_key_dump                   = wpa_eapol_key_dump_impl;
}

#if 0
int wpa_sm_pmksa_cache_list(struct wpa_sm *sm, char *buf, size_t len)
{
#ifndef CONFIG_NO_WPA2
	return pmksa_cache_list(sm->pmksa, buf, len);
#else /* CONFIG_NO_WPA2 */
	return -1;
#endif /* CONFIG_NO_WPA2 */
	return -1;
}

int ieee80211w_set_keys(struct wpa_sm *sm,
			       struct wpa_eapol_ie_parse *ie)
{
	return 0;
}

void wpa_sm_pmksa_free_cb(struct rsn_pmksa_cache_entry *entry,
				 void *ctx, int replace)
{
	struct wpa_sm *sm = ctx;

	if (sm->cur_pmksa == entry ||
	    (sm->pmk_len == entry->pmk_len &&
	     os_memcmp(sm->pmk, entry->pmk, sm->pmk_len) == 0)) {
		wpa_printf(MSG_DEBUG, "RSN: removed current PMKSA entry");
		sm->cur_pmksa = NULL;

		if (replace) {
			/* A new entry is being added, so no need to
			 * deauthenticate in this case. This happens when EAP
			 * authentication is completed again (reauth or failed
			 * PMKSA caching attempt). */
			return;
		}

		os_memset(sm->pmk, 0, sizeof(sm->pmk));
		wpa_sm_deauthenticate(sm, WLAN_REASON_UNSPECIFIED);
	}
}

/**
 * wpa_sm_set_fast_reauth - Set fast reauthentication (EAP) enabled/disabled
 * @sm: Pointer to WPA state machine data from wpa_sm_init()
 * @fast_reauth: Whether fast reauthentication (EAP) is allowed
 */
void wpa_sm_set_fast_reauth(struct wpa_sm *sm, int fast_reauth)
{
	if (sm)
		sm->fast_reauth = fast_reauth;
}

/**
 * wpa_sm_set_scard_ctx - Set context pointer for smartcard callbacks
 * @sm: Pointer to WPA state machine data from wpa_sm_init()
 * @scard_ctx: Context pointer for smartcard related callback functions
 */
void wpa_sm_set_scard_ctx(struct wpa_sm *sm, void *scard_ctx)
{
	if (sm == NULL)
		return;
	sm->scard_ctx = scard_ctx;
	if (sm->preauth_eapol)
		eapol_sm_register_scard_ctx(sm->preauth_eapol, scard_ctx);
}

/**
 * wpa_sm_aborted_cached - Notify WPA that PMKSA caching was aborted
 * @sm: Pointer to WPA state machine data from wpa_sm_init()
 */
void wpa_sm_aborted_cached(struct wpa_sm *sm)
{
	if (sm && sm->cur_pmksa) {
		wpa_printf(MSG_DEBUG, "RSN: Cancelling PMKSA caching attempt");
		sm->cur_pmksa = NULL;
	}
}

/**
 * wpa_sm_set_ifname - Set network interface name
 * @sm: Pointer to WPA state machine data from wpa_sm_init()
 * @ifname: Interface name
 * @bridge_ifname: Optional bridge interface name (for pre-auth)
 */
void wpa_sm_set_ifname(struct wpa_sm *sm, const char *ifname,
		       const char *bridge_ifname)
{
	if (sm) {
		sm->ifname = ifname;
		sm->bridge_ifname = bridge_ifname;
	}
}
#endif /* #if 0 */
