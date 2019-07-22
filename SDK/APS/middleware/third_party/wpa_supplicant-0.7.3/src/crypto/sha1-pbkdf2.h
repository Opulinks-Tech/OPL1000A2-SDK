/*
 * SHA1 PBKDF2 - For Key Exchange
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 * Add by Michael
 */
#ifndef SHA1_PBKDF2_H
#define SHA1_PBKDF2_H
#include "wpabuf.h"

typedef int (*pbkdf2_sha1_f_fp_t)(char *passphrase, char *ssid, size_t ssid_len, int iterations, unsigned int count, u8 *digest);
typedef int (*pbkdf2_sha1_fp_t)(char *passphrase, char *ssid, size_t ssid_len, int iterations, u8 *buf, size_t buflen);

extern pbkdf2_sha1_f_fp_t pbkdf2_sha1_f;
extern pbkdf2_sha1_fp_t pbkdf2_sha1;

#endif //SHA1_PBKDF2_H
