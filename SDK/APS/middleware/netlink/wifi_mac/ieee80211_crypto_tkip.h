#ifndef _IEEE80211_CRYPTO_TKIP_H_
#define _IEEE80211_CRYPTO_TKIP_H_

#include <stdint.h>

#include "common/wifi_ieee802_11_defs.h"


u8* tkip_decrypt_impl(const u8 *tk, const struct ieee80211_hdr *hdr, const u8 *data, size_t data_len, size_t *decrypted_len);
u8* tkip_encrypt_impl(const u8 *tk, u8 *frame, size_t len, size_t hdrlen, u8 *qos, u8 *pn, int keyid, size_t *encrypted_len);
int tkip_mic_check_impl(const u8 *tk, const struct ieee80211_hdr *hdr, u8 *plain, size_t plain_len);


/* Rom patch interface */
typedef u8* (*tkip_decrypt_fp_t)(const u8 *tk, const struct ieee80211_hdr *hdr, const u8 *data, size_t data_len, size_t *decrypted_len);
typedef u8* (*tkip_encrypt_fp_t)(const u8 *tk, u8 *frame, size_t len, size_t hdrlen, u8 *qos, u8 *pn, int keyid, size_t *encrypted_len);
typedef int (*tkip_mic_check_fp_t)(const u8 *tk, const struct ieee80211_hdr *hdr, u8 *plain, size_t plain_len);

/* Export interface funtion pointer */
extern tkip_decrypt_fp_t tkip_decrypt;
extern tkip_encrypt_fp_t tkip_encrypt;
extern tkip_mic_check_fp_t tkip_mic_check;

#endif /* _IEEE80211_CRYPTO_TKIP_H_ */
