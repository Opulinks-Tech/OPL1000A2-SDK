/*
 * SHA1-based key derivation function (PBKDF2) for IEEE 802.11i
 * Copyright (c) 2003-2005, Jouni Malinen <j@w1.fi>
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
#include "sha1.h"
#include "md5.h"
#include "crypto.h"
#include "scrt.h"
#include "sha1-pbkdf2.h"


#if 0
static int pbkdf2_sha1_f(const char *passphrase, const char *ssid,
			 size_t ssid_len, int iterations, unsigned int count,
			 u8 *digest)
{
	unsigned char tmp[SHA1_MAC_LEN], tmp2[SHA1_MAC_LEN];
	int i, j;
	unsigned char count_buf[4];
	const u8 *addr[2];
	size_t len[2];
	size_t passphrase_len = os_strlen(passphrase);

	addr[0] = (u8 *) ssid;
	len[0] = ssid_len;
	addr[1] = count_buf;
	len[1] = 4;

	/* F(P, S, c, i) = U1 xor U2 xor ... Uc
	 * U1 = PRF(P, S || i)
	 * U2 = PRF(P, U1)
	 * Uc = PRF(P, Uc-1)
	 */

	count_buf[0] = (count >> 24) & 0xff;
	count_buf[1] = (count >> 16) & 0xff;
	count_buf[2] = (count >> 8) & 0xff;
	count_buf[3] = count & 0xff;
	if (hmac_sha1_vector((u8 *) passphrase, passphrase_len, 2, addr, len,
			     tmp))
		return -1;
	os_memcpy(digest, tmp, SHA1_MAC_LEN);

        printf("\r\n pbkdf2_sha1_f, before loop \r\n");

	for (i = 1; i < iterations; i++) {
		if (hmac_sha1((u8 *) passphrase, passphrase_len, tmp,
			      SHA1_MAC_LEN, tmp2))
			return -1;
		os_memcpy(tmp, tmp2, SHA1_MAC_LEN);
		for (j = 0; j < SHA1_MAC_LEN; j++)
			digest[j] ^= tmp2[j];
	}

        printf("\r\n pbkdf2_sha1_f, after loop \r\n");

	return 0;
}
#else // use hw security ip to do hmac sha-1
int pbkdf2_sha1_f_impl(char *passphrase, char *ssid,
			 size_t ssid_len, int iterations, unsigned int count,
			 u8 *digest)
{
	unsigned char tmp[SHA1_MAC_LEN], tmp2[SHA1_MAC_LEN];
	int i, j;
	unsigned char count_buf[4];
	const u8 *addr[2];
	size_t len[2];
	size_t passphrase_len = os_strlen(passphrase);
    //int k;

    os_memset(tmp, 0, SHA1_MAC_LEN);
    os_memset(tmp2, 0, SHA1_MAC_LEN);


	addr[0] = (u8 *) ssid;
	len[0] = ssid_len;
	addr[1] = count_buf;
	len[1] = 4;

	/* F(P, S, c, i) = U1 xor U2 xor ... Uc
	 * U1 = PRF(P, S || i)
	 * U2 = PRF(P, U1)
	 * Uc = PRF(P, Uc-1)
	 */

	count_buf[0] = (count >> 24) & 0xff;
	count_buf[1] = (count >> 16) & 0xff;
	count_buf[2] = (count >> 8) & 0xff;
	count_buf[3] = count & 0xff;
	if (hmac_sha1_vector((u8 *) passphrase, passphrase_len, 2, addr, len,
			     tmp))
		return -1;
	os_memcpy(digest, tmp, SHA1_MAC_LEN);

	for (i = 1; i < iterations; i++) {

#if 0 //SW Caculation
        //printf("\r\n\r\n sw, i:%d ", i);
        //printf("\r\n pbkdf2_sha1_f, passphrase_len:%ld passphrase: ", passphrase_len);
        //for (k=0; k<os_strlen(passphrase); k++) {
        //    printf("%02x ", *(passphrase + k));
        //}

        //printf("\r\n pbkdf2_sha1_f, tmp: ");
        //for (k=0; k<SHA1_MAC_LEN; k++) {
        //    printf("%02x ", tmp[k]);
        //}

		if (hmac_sha1((u8 *) passphrase, passphrase_len, tmp, SHA1_MAC_LEN, tmp2)) return -1; //correct

        //printf("\r\n pbkdf2_sha1_f, tmp2(after): ");
        //for (k=0; k<SHA1_MAC_LEN; k++) {
        //    printf("%02x ", tmp2[k]);
        //}
#else //HW Caculation
        //printf("\r\n\r\n hw, i:%d ", i);
        //printf("\r\n pbkdf2_sha1_f, passphrase_len:%ld passphrase: ", passphrase_len);
        //for (k=0; k<os_strlen(passphrase); k++) {
        //    printf("%02x ", *(passphrase + k));
        //}

        //printf("\r\n pbkdf2_sha1_f, tmp: ");
        //for (k=0; k<SHA1_MAC_LEN; k++) {
        //    printf("%02x ", tmp[k]);
        //}

        nl_hmac_sha_1((uint8_t *)passphrase, (int)passphrase_len, (uint8_t *)tmp, SHA1_MAC_LEN, (uint8_t *)tmp2);

        //printf("\r\n pbkdf2_sha1_f, tmp2(after): ");
        //for (k=0; k<SHA1_MAC_LEN; k++) {
        //    printf("%02x ", tmp2[k]);
        //}
#endif

		os_memcpy(tmp, tmp2, SHA1_MAC_LEN);
		for (j = 0; j < SHA1_MAC_LEN; j++)
			digest[j] ^= tmp2[j];

	}

	return 0;
}

#endif

/**
 * pbkdf2_sha1 - SHA1-based key derivation function (PBKDF2) for IEEE 802.11i
 * @passphrase: ASCII passphrase
 * @ssid: SSID
 * @ssid_len: SSID length in bytes
 * @iterations: Number of iterations to run
 * @buf: Buffer for the generated key
 * @buflen: Length of the buffer in bytes
 * Returns: 0 on success, -1 of failure
 *
 * This function is used to derive PSK for WPA-PSK. For this protocol,
 * iterations is set to 4096 and buflen to 32. This function is described in
 * IEEE Std 802.11-2004, Clause H.4. The main construction is from PKCS#5 v2.0.
 */
 #if 0
int pbkdf2_sha1(const char *passphrase, const char *ssid, size_t ssid_len,
		int iterations, u8 *buf, size_t buflen)
{
	unsigned int count = 0;
	unsigned char *pos = buf;
	size_t left = buflen, plen;
	unsigned char digest[SHA1_MAC_LEN];

	while (left > 0) {
		count++;
		if (pbkdf2_sha1_f(passphrase, ssid, ssid_len, iterations,
				  count, digest))
			return -1;
		plen = left > SHA1_MAC_LEN ? SHA1_MAC_LEN : left;
		os_memcpy(pos, digest, plen);
		pos += plen;
		left -= plen;
	}

	return 0;
}
#else
int pbkdf2_sha1_impl(char *passphrase, char *ssid, size_t ssid_len,
		int iterations, u8 *buf, size_t buflen)
{
	unsigned int count = 0;
	unsigned char *pos = buf;
	size_t left = buflen, plen;
	unsigned char digest[SHA1_MAC_LEN];

	while (left > 0) {
		count++;
        if (pbkdf2_sha1_f(passphrase, ssid, ssid_len, iterations,
				  count, digest))
			return -1;
		plen = left > SHA1_MAC_LEN ? SHA1_MAC_LEN : left;
		os_memcpy(pos, digest, plen);
		pos += plen;
		left -= plen;
	}

	return 0;
}
#endif


pbkdf2_sha1_f_fp_t pbkdf2_sha1_f = pbkdf2_sha1_f_impl;
pbkdf2_sha1_fp_t pbkdf2_sha1 = pbkdf2_sha1_impl;


