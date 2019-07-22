#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <mbedtls/aes.h>
#include <mbedtls/ccm.h>

void test_ccm_encrypt_decrpty(uint8_t *sk, int sk_len,
						  uint8_t *nonce, int nonce_len,
						  uint8_t * adata, int adata_len,
						  uint8_t *plain_text, uint8_t *encrypted_text, int text_len,
						  uint8_t *tag, int tag_len)
{
	uint8_t output_buf[64];
	uint8_t tag_buf[4];
	mbedtls_ccm_context ccm_ctx;
	int i;
	int result;
	
	// CCM encrypt
	printf("[CCM encrypt]\r\n");
	
	mbedtls_ccm_init(&ccm_ctx);
	if ((result = mbedtls_ccm_setkey(&ccm_ctx, MBEDTLS_CIPHER_ID_AES, sk, sk_len))) printf("mbedtls_ccm_setkey errno:%d\r\n", result);;
	if ((result = mbedtls_ccm_encrypt_and_tag(&ccm_ctx, text_len, nonce, nonce_len, adata, adata_len, plain_text, output_buf, tag_buf, tag_len))) printf("mbedtls_ccm_encrypt_and_tag errno:%d\r\n", result);
	
	printf("encrypted text:[");
	for (i = 0; i < text_len; i++) printf("%02X ", output_buf[i]);
	printf("]\r\n");
	
	printf("mic:[");
	for (i = 0; i < tag_len; i++) printf("%02X ", tag_buf[i]);
	printf("]\r\n");
	
	printf("result: %s\r\n\r\n", result == 0 && memcmp(encrypted_text, output_buf, text_len) == 0 && memcmp(tag, tag_buf, tag_len) == 0 ? "PASS" : "FAIL");

	// CCM decrypt
	printf("[CCM decrypt]\r\n");
	
	if ((result = mbedtls_ccm_auth_decrypt(&ccm_ctx, text_len, nonce, nonce_len, adata, adata_len, encrypted_text, output_buf, tag, tag_len))) printf("mbedtls_ccm_auth_decrypt errno:%d\r\n", result);
		
	printf("decrypted text:[");
	for (i = 0; i < text_len; i++) printf("%02X ", output_buf[i]);
	printf("]\r\n");
	
	printf("result: %s\r\n\r\n", memcmp(plain_text, output_buf, text_len) == 0 ? "PASS" : "FAIL");
}

void ble_encryption_test() {
	
	// MSO to LSO
	uint8_t ltk[16] = {0x4C, 0x68, 0x38, 0x41, 0x39, 0xF5, 0x74, 0xD8, 0x36, 0xBC, 0xF3, 0x4E, 0x9D, 0xFB, 0x01, 0xBF};
	uint8_t skd[16] = {0x02, 0x13, 0x24, 0x35, 0x46, 0x57, 0x68, 0x79, 0xAC, 0xBD, 0xCE, 0xDF, 0xE0, 0xF1, 0x02, 0x13};
	uint8_t sk[16] = {0x99, 0xAD, 0x1B, 0x52, 0x26, 0xA3, 0x7E, 0x3E, 0x05, 0x8E, 0x3B, 0x8E, 0x27, 0xC2, 0xC6, 0x66};
	
	// 1st data
	uint8_t nonce1[13] = {0x00, 0x00, 0x00, 0x00, 0x80, 0x24, 0xAB, 0xDC, 0xBA, 0xBE, 0xBA, 0xAF, 0xDE};
	uint8_t a_data1[1] = {0x03};
	uint8_t plain_text_1[1] = {0x06};
	uint8_t encrypted_text_1[1] = {0x9F};
	uint8_t tag1[4] = {0xCD, 0xA7, 0xF4, 0x48};
	// 2nd data
	uint8_t nonce2[13] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0xAB, 0xDC, 0xBA, 0xBE, 0xBA, 0xAF, 0xDE};
	uint8_t a_data2[1] = {0x03};
	uint8_t plain_text_2[1] = {0x06};
	uint8_t encrypted_text_2[1] = {0xA3};
	uint8_t tag2[4] = {0x4C, 0x13, 0xA4, 0x15};	
	// 3rd data
	uint8_t nonce3[13] = {0x01, 0x00, 0x00, 0x00, 0x80, 0x24, 0xAB, 0xDC, 0xBA, 0xBE, 0xBA, 0xAF, 0xDE};
	uint8_t a_data3[1] = {0x02};
	uint8_t plain_text_3[27] = {0x17, 0x00, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30};
	uint8_t encrypted_text_3[27] = {0x7A, 0x70, 0xD6, 0x64, 0x15, 0x22, 0x6D, 0xF2, 0x6B, 0x17, 0x83, 0x9A, 0x06, 0x04, 0x05, 0x59, 0x6B, 0xD6, 0x56, 0x4F, 0x79, 0x6B, 0x5B, 0x9C, 0xE6, 0xFF, 0x32};
	uint8_t tag3[4] = {0xF7, 0x5A, 0x6D, 0x33};
	// 4rd data
	uint8_t nonce4[13] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x24, 0xAB, 0xDC, 0xBA, 0xBE, 0xBA, 0xAF, 0xDE};
	uint8_t a_data4[1] = {0x02};
	uint8_t plain_text_4[27] = {0x17, 0x00, 0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0x30, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51};
	uint8_t encrypted_text_4[27] = {0xF3, 0x88, 0x81, 0xE7, 0xBD, 0x94, 0xC9, 0xC3, 0x69, 0xB9, 0xA6, 0x68, 0x46, 0xDD, 0x47, 0x86, 0xAA, 0x8C, 0x39, 0xCE, 0x54, 0x0D, 0x0D, 0xAE, 0x3A, 0xDC, 0xDF};
	uint8_t tag4[4] = {0x89, 0xB9, 0x60, 0x88};\



	int i;	
	uint8_t output_buf[16];
	mbedtls_aes_context aes_ctx;
	
	// SK calculation
	printf("============================== sk computing\r\n");

	mbedtls_aes_init(&aes_ctx);
	mbedtls_aes_setkey_enc(&aes_ctx, ltk, 128);
	mbedtls_aes_encrypt(&aes_ctx, skd, output_buf);
	
	printf("sk:[");
	for (i = 0; i < 16; i++) printf("%02X ", output_buf[i]);
	printf("]\r\n");
	
	printf("result: %s\r\n", memcmp(sk, output_buf, 16) == 0 ? "PASS" : "FAIL");



	printf("============================== Sample Data 1\r\n");
	test_ccm_encrypt_decrpty(sk, 128,
							 nonce1, 13,
							 a_data1, 1,
							 plain_text_1, encrypted_text_1, 1,
							 tag1, 4);
	printf("============================== Sample Data 2\r\n");						 
	test_ccm_encrypt_decrpty(sk, 128,
							 nonce2, 13,
							 a_data2, 1,
							 plain_text_2, encrypted_text_2, 1,
							 tag2, 4);
	printf("============================== Sample Data 3\r\n");
	test_ccm_encrypt_decrpty(sk, 128,
							 nonce3, 13,
							 a_data3, 1,
							 plain_text_3, encrypted_text_3, 27,
							 tag3, 4);
	printf("============================== Sample Data 4\r\n");
	test_ccm_encrypt_decrpty(sk, 128,
							 nonce4, 13,
							 a_data4, 1,
							 plain_text_4, encrypted_text_4, 27,
							 tag4, 4);
	
};

/*
plain text: 16 bytes
key: 16 bytes (128 bits key)
nonce: 13 bytes
addition assocaited data: 1 byte
output tag: 4 bytes
output cipher text: 16 bytes
*/
void ble_encryption_test_2(void) 
{	
	// MSO to LSO
	uint8_t ltk[16] = {0x4C, 0x68, 0x38, 0x41, 0x39, 0xF5, 0x74, 0xD8, 0x36, 0xBC, 0xF3, 0x4E, 0x9D, 0xFB, 0x01, 0xBF};
	uint8_t skd[16] = {0x02, 0x13, 0x24, 0x35, 0x46, 0x57, 0x68, 0x79, 0xAC, 0xBD, 0xCE, 0xDF, 0xE0, 0xF1, 0x02, 0x13};
	uint8_t sk[16] = {0x99, 0xAD, 0x1B, 0x52, 0x26, 0xA3, 0x7E, 0x3E, 0x05, 0x8E, 0x3B, 0x8E, 0x27, 0xC2, 0xC6, 0x66};
	
	// 1st data
	uint8_t nonce1[13] = {0x00, 0x00, 0x00, 0x00, 0x80, 0x24, 0xAB, 0xDC, 0xBA, 0xBE, 0xBA, 0xAF, 0xDE};
	uint8_t a_data1[1] = {0x03};
	uint8_t plain_text_1[16] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10};
	uint8_t encrypted_text_1[16] = {0x98, 0x1b, 0x0e, 0x8c, 0xf1, 0xac, 0x1c, 0x68, 0xb0, 0x74, 0xc4, 0xea, 0xf8, 0xf0, 0xe8, 0x67};
	uint8_t tag1[4] = {0xb3, 0x61, 0xe4, 0xfe};

	int i;
	uint8_t output_buf[16];
	mbedtls_aes_context aes_ctx;

	// SK calculation
	printf("============================== sk computing\r\n");

	mbedtls_aes_init(&aes_ctx);
	mbedtls_aes_setkey_enc(&aes_ctx, ltk, 128);
	mbedtls_aes_encrypt(&aes_ctx, skd, output_buf);
	
	printf("sk:[");
	for (i = 0; i < 16; i++) printf("%02X ", output_buf[i]);
	printf("]\r\n");
	
	printf("result: %s\r\n", memcmp(sk, output_buf, 16) == 0 ? "PASS" : "FAIL");



	printf("============================== Sample Data 1\r\n");
	test_ccm_encrypt_decrpty(sk, 128,
							 nonce1, 13,
							 a_data1, 1,
							 plain_text_1, encrypted_text_1, 16,
							 tag1, 4);
	
};


