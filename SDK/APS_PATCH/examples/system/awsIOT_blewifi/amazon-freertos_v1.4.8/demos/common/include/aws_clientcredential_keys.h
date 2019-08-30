#ifndef AWS_CLIENT_CREDENTIAL_KEYS_H
#define AWS_CLIENT_CREDENTIAL_KEYS_H

#include <stdint.h>

/*
 * PEM-encoded client certificate
 *
 * Must include the PEM header and footer:
 * "-----BEGIN CERTIFICATE-----\n"\
 * "...base64 data...\n"\
 * "-----END CERTIFICATE-----\n"
 */
//#define keyCLIENT_CERTIFICATE_PEM                   "Paste client certificate here."


#define keyCLIENT_CERTIFICATE_PEM \
"-----BEGIN CERTIFICATE-----\n"\
"MIIDWTCCAkGgAwIBAgIUTK1C9ucswWor944N/r2k8F92Ah4wDQYJKoZIhvcNAQEL\n"\
"BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\n"\
"SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTE5MDgwMTA2MzMw\n"\
"MloXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0\n"\
"ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKLYDx+i1YPbHvaxU9NX\n"\
"ehftWxdHE7nHbmIV5gK7SxM1UPxy5TNoFQk6y9gsPue2nNJnaX8dCnfRedHt70sy\n"\
"dJJvgFnyxMK4D+pL9GNi0Lm0jsOr8BYy2gGusr1dhtiP4XfUSx4PuMDCLyI64WDJ\n"\
"j1y4+F+Qcr8JPSu6QlEVRWwOtQtu0VCuhrRcSyR8JtWaAHugr9KBEa8A0jhldfHM\n"\
"IFle/EcJyI5wcGw7WqVTzdeTOS1+YdE5yjzJvqyWIZ1tcNyefxtOv+8WG7RXTQZ4\n"\
"kZGOcWIxLS9drnEb8M/6FPUM6+ZXtOoGyoE51Atux2IQfUZUYHX+SmmlZbqvH5Wm\n"\
"VfECAwEAAaNgMF4wHwYDVR0jBBgwFoAUTLp1KcQfp4Psarj6shnmS3U/DFkwHQYD\n"\
"VR0OBBYEFCziYnAqqNAWf2KpZvfex5+STuv9MAwGA1UdEwEB/wQCMAAwDgYDVR0P\n"\
"AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQBoY8Hsq1OapSGyJ5vxZ8PIyhhS\n"\
"wqdcKlIF0s/TsKsBcqsMw++FpkxPrl4wa9UxNUrcyX8hwHT9Fs7UMFjWAG3u+ytW\n"\
"yciTpSmgdGMkCnNFbAo1Q36NcNZsQceXsLdof0FpMwRe2084mWioxZipYqai2PG1\n"\
"wQbER1MCCLSYijNbMf5pjMpyWWCvzLTVQGJv0xgUixeJQ2vZkG3dkVdEWBEspbH6\n"\
"1AyacHTbwjvIFlCNHYJvEBpIImTKQYPIhAQsP4OETRqBnt8oQyQSRzecDrlpxvyh\n"\
"GvPb18u6W1Xikp/jwG6wEY0YDGgVCifEvt8uBLGI0QdXQ5NEGecezwFA5Jca\n"\
"-----END CERTIFICATE-----\n"


/*
 * PEM-encoded issuer certificate for AWS IoT Just In Time Registration (JITR).
 * This is required if you're using JITR, since the issuer (Certificate
 * Authority) of the client certificate is used by the server for routing the
 * device's initial request. (The device client certificate must always be
 * sent as well.) For more information about JITR, see:
 *  https://docs.aws.amazon.com/iot/latest/developerguide/jit-provisioning.html,
 *  https://aws.amazon.com/blogs/iot/just-in-time-registration-of-device-certificates-on-aws-iot/.
 *
 * If you're not using JITR, set below to NULL.
 *
 * Must include the PEM header and footer:
 * "-----BEGIN CERTIFICATE-----\n"\
 * "...base64 data...\n"\
 * "-----END CERTIFICATE-----\n"
 */
#define keyJITR_DEVICE_CERTIFICATE_AUTHORITY_PEM    NULL

/*
 * PEM-encoded client private key.
 *
 * Must include the PEM header and footer:
 * "-----BEGIN RSA PRIVATE KEY-----\n"\
 * "...base64 data...\n"\
 * "-----END RSA PRIVATE KEY-----\n"
 */
//#define keyCLIENT_PRIVATE_KEY_PEM                   "Paste client private key here."

#define keyCLIENT_PRIVATE_KEY_PEM \
"-----BEGIN RSA PRIVATE KEY-----\n"\
"MIIEpAIBAAKCAQEAotgPH6LVg9se9rFT01d6F+1bF0cTucduYhXmArtLEzVQ/HLl\n"\
"M2gVCTrL2Cw+57ac0mdpfx0Kd9F50e3vSzJ0km+AWfLEwrgP6kv0Y2LQubSOw6vw\n"\
"FjLaAa6yvV2G2I/hd9RLHg+4wMIvIjrhYMmPXLj4X5Byvwk9K7pCURVFbA61C27R\n"\
"UK6GtFxLJHwm1ZoAe6Cv0oERrwDSOGV18cwgWV78RwnIjnBwbDtapVPN15M5LX5h\n"\
"0TnKPMm+rJYhnW1w3J5/G06/7xYbtFdNBniRkY5xYjEtL12ucRvwz/oU9Qzr5le0\n"\
"6gbKgTnUC27HYhB9RlRgdf5KaaVluq8flaZV8QIDAQABAoIBAQCdi0I2bSb8hgWS\n"\
"zUQ39y2b8HpsLsuatKLKKj147xJW5wIO+mlcLpoLb+K+pS/GpRlbRcj+vW3fNb4R\n"\
"p4wWyB2XfmBCpWhOqhxbpFgvvu4Ie/KSw8lm8JORhR5PrihDZA/SCoUR9Evd0QAg\n"\
"sa5SIS6dmiZQ7kLVJ28S2wRFxbqAKr9SXxk/BooatJ0vkihMcGHdKDaqRe+uwD1n\n"\
"Q5lK2G36ZbKI47aQm1OuiKTFBG7TxiC8qpMG7FRBLPJXaXWVj5fFtkkkWJ/K9uwz\n"\
"5v5gZwkR8J2j0DPm2vjPhgDkRNToIFxAUzG3PemNC7lo6Bmbb8h+he783PPH3E26\n"\
"N4pRNwcBAoGBAM0TrzQNgGkMZAVa6CgqLCZLkPqtR9yKaxmo9XcpYeSXxn1t9+iA\n"\
"yXxaMnP8iFYWlMVEd8aXrSkkVWkVKFR/4mWfeTM0qmVDWeYuf0retYDsYCgjgeDV\n"\
"c/BEEluDnLCd0FBUExyAW2PW71LhscduIm9ydUkR+h8h+1o24U7ojBIpAoGBAMtH\n"\
"tytQEauhGEyeSBKCDI2QY944099AlwdLpnPAFy9K+OjdZ7uiBKxKYdgVwbaszVZc\n"\
"Ier8T0LV8TYQgl9xJiWfkvZbmJib+kMkcad1BRDVyWVxFS0WfVvf4CfewuBW9QLu\n"\
"YDySXrtcswlCigj/uvAyz1KN8WZSkL8+E9t2K26JAoGBAIDQ0oaxzG0v2Uv71EQW\n"\
"aXEMml67pZ4EXE3b8YQOPG1oYMe84bHr46P2zCZLBCB/Qqe+4DWgZKapKfU90v1w\n"\
"7vbCe1EMzdOZwMhzgIW46u22jJfp5dUifxb2y0ksnKncF1O929tVTRKjmeTH2/HX\n"\
"Xk3+6Wkin6gaolYCjvgP3pwxAoGASuLFgKhDijWTbQ5WQLM2CqSplcDblIAH40Ek\n"\
"NboLEHoPDkIaJ8486HeSafdB4JjyICFawWVnkZic/qzASoa1ZeDOVAFUSswTOs7f\n"\
"K7B5SEVppvwsr+p8De+AdU+j/N8r7YOBYm8NhPpWXk/r5/rFlB0zsRbnWimKd1lY\n"\
"GV4EC/kCgYBt+0eOZL0yQBObUK8Z7pMYdfe9AmdaxbWsKaUUcMisZ5fWsllKraeV\n"\
"rkrdFkFI63GGMpcFBmBOjQ8rcXdg9CcwBt3ck++YrOFiuIJHwccjwacw50uh8kpV\n"\
"KiLP6loOM0Jkoyape/vKmpoc58DFGBK4mOkYVOdDpoJ/PYavNJysGw==\n"\
"-----END RSA PRIVATE KEY-----\n"

/* The constants above are set to const char * pointers defined in aws_dev_mode_key_provisioning.c,
 * and externed here for use in C files.  NOTE!  THIS IS DONE FOR CONVENIENCE
 * DURING AN EVALUATION PHASE AND IS NOT GOOD PRACTICE FOR PRODUCTION SYSTEMS
 * WHICH MUST STORE KEYS SECURELY. */
extern const char clientcredentialCLIENT_CERTIFICATE_PEM[];
extern const char * clientcredentialJITR_DEVICE_CERTIFICATE_AUTHORITY_PEM;
extern const char clientcredentialCLIENT_PRIVATE_KEY_PEM[];
extern const uint32_t clientcredentialCLIENT_CERTIFICATE_LENGTH;
extern const uint32_t clientcredentialCLIENT_PRIVATE_KEY_LENGTH;

#endif /* AWS_CLIENT_CREDENTIAL_KEYS_H */
