#ifndef _IEEE80211_CRYPTO_CCMP_H_
#define _IEEE80211_CRYPTO_CCMP_H_

#include <stdint.h>

#include "common/wifi_ieee802_11_defs.h"

#ifdef __SIMULATOR__
#define USE_SW_CCM_CRYPT
#endif


u8* ccmp_decrypt_impl(const u8 *tk, const struct ieee80211_hdr *hdr, u8 *data, size_t data_len, size_t *decrypted_len);
u8* ccmp_encrypt_impl(const u8 *tk, u8 *frame, size_t len, size_t hdrlen, u8 *qos, u8 *pn, int keyid, size_t *encrypted_len);



/* Rom patch interface */
typedef u8* (*ccmp_decrypt_fp_t)(const u8 *tk, const struct ieee80211_hdr *hdr, u8 *data, size_t data_len, size_t *decrypted_len);
typedef u8* (*ccmp_encrypt_fp_t)(const u8 *tk, u8 *frame, size_t len, size_t hdrlen, u8 *qos, u8 *pn, int keyid, size_t *encrypted_len);

/* Export interface funtion pointer */
extern ccmp_decrypt_fp_t ccmp_decrypt;
extern ccmp_encrypt_fp_t ccmp_encrypt;

#endif /* _IEEE80211_CRYPTO_CCMP_H_ */
