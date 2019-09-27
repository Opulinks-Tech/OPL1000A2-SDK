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
"MIIDWjCCAkKgAwIBAgIVAPGqzVx4BJGvL0LR6rL9QaFBSCusMA0GCSqGSIb3DQEB\n"\
"CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t\n"\
"IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0xOTA5MTEwOTM0\n"\
"NThaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh\n"\
"dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQC2aTQbWajA90kQ4m8p\n"\
"McJTl0xHrX22TPKdYQOlVpB23VOwSUYzE6ijjiiJzKPagrHAHAVupPI1klcZkfQg\n"\
"TiroR1rtMnxE+XcXzOUIO3re/DiswHvxNuVvzy4SqM8e9OtXR1D6jgIg4L0j2AXw\n"\
"KOoO1YtyAPZrCgt7u1sy2S9huNiyRDhcoXB2k6DanOuYo9DUSag1MspwmNLvZ0SV\n"\
"VUhRXy7n4KZb59tooxq4FYeQg1uFIO9iFILXFFJGUVrVNiRSsJBiiyPfm3/sINX/\n"\
"2u6KvrtA7y778BdbfPztW5vs5gyUJVz5hU6kK66wdZyoOMbmeiAORWx88okgsY+t\n"\
"HbX9AgMBAAGjYDBeMB8GA1UdIwQYMBaAFEOZ+SjBWo37n4Tp3uasWYaagiRIMB0G\n"\
"A1UdDgQWBBQ0IoiRRCxRz4caqhpg9WLKYU2kujAMBgNVHRMBAf8EAjAAMA4GA1Ud\n"\
"DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAhJIquPbrOQsFoiBtcHjG7wGq\n"\
"B2g4gmyU/SQnv52kq2Vx9ztnHiHYYBsg9JoK6SrVyd9PsQrJi9cSj2CK0fHddM49\n"\
"cEpMNw5UsFeMKEI+rRzWStWd2Mklm960l2enlCNRSCGBFozihrskYQHAjz1RTCYm\n"\
"k0BUG+6Obd/v6yxBwUyVhVUSja78vYctUQBcONc09C7npCYgNPftSdbrrTWKul0G\n"\
"DnhwqiyMH0ONJpPA4g7Iqe8/rSXh6RkhJFIL8boBUMvEI3+SFFQHe4eoXwStwnba\n"\
"1qSXjrpq5nxt6BL1v0p7oFgJ2A55vJWwGSmbe/lKLs0VZf9fH90laq3lhTd7YA==\n"\
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
"MIIEpQIBAAKCAQEAtmk0G1mowPdJEOJvKTHCU5dMR619tkzynWEDpVaQdt1TsElG\n"\
"MxOoo44oicyj2oKxwBwFbqTyNZJXGZH0IE4q6Eda7TJ8RPl3F8zlCDt63vw4rMB7\n"\
"8Tblb88uEqjPHvTrV0dQ+o4CIOC9I9gF8CjqDtWLcgD2awoLe7tbMtkvYbjYskQ4\n"\
"XKFwdpOg2pzrmKPQ1EmoNTLKcJjS72dElVVIUV8u5+CmW+fbaKMauBWHkINbhSDv\n"\
"YhSC1xRSRlFa1TYkUrCQYosj35t/7CDV/9ruir67QO8u+/AXW3z87Vub7OYMlCVc\n"\
"+YVOpCuusHWcqDjG5nogDkVsfPKJILGPrR21/QIDAQABAoIBAFh0+fzNZOV2xar3\n"\
"c5qRc8AfnbEgs9bEx96D3kW+jb3mpJ/XWMCKJvxxgr0YTNk5CcO0kvnN6VsLRUu0\n"\
"GKPV5D7R44klM1grdZJIHUT5uyMJQt6bVT9rNKYc3SXVUOOdN8cSYXWvid6xvWb1\n"\
"aiPFY4OgEMhPKTBNJbZx687jvXNwYqcwticTKJEuD7NNEEwZw3bwmG5fKq1R8d1g\n"\
"vN7Dr+W71h7Bxf5UupoJflHGNNSS6gxNgeD51ABNghz0rNWrVFITcTp76ESZy652\n"\
"2Zoc3anzpRuiHOGmcTv+z8L1goq5wLxjIn1uqGMDF6ZrTypBCcr/lM/LU2F9+heb\n"\
"hU3Su+0CgYEA7kKXNCwuQGS+ipo9A2tautx4Nvqhb2XTWTnYQzo1eV4cZn5hV96h\n"\
"Lq8IFnpIC662iuyM5ArS0y0u2sJBchdZTyj6ylZH7u9R8q7oqbk16VNS+qjPSmgH\n"\
"6SJ72HFpAxjpZQhlHNGHEuR6ix6dqTW88/lfk281uB0gJHpwHhmb+18CgYEAw/4W\n"\
"ajmrqkeBtZIm9d6Jv3E3as8hui9w/eQpJW5KfLhZClSaZ/NmEOqk9jcc2KFq4CYR\n"\
"9/jxA6dGOtEWRC5BJrvja2mGEPwH+w+GxjvhErn1tkX8E10lRYXLKsTeV6Ti39WB\n"\
"OGDioUMdlG0MTflnylrwvqYRdY3IEeDX/JSsqCMCgYEAy7N1EK8BMB6x5VAu/+Bl\n"\
"5sKdqOuo39Pf2TYiVn/KviQfBeXhFb4Kch5FPOp49DwAZ1AMuN8kFlZX68IPMBsU\n"\
"SSCRIjKYN4i2uFSgKJNnqz+I/zMaXtQIh2LrHUcA8Bv5V17RC817vURIHO8k/kkq\n"\
"bjBzbVq0Q9/q3iUr/CALAuMCgYEAr/uQaOVLrOP0JfMLpK2jhhDEr9U49ZLvpCmg\n"\
"gImdvItS+CGrWzeEKCBo6uPltOaLrqTAcaZH0v3544omsiXOPldrBuGDjfCfweDZ\n"\
"JVpS6Jutc3M3urNiaiCOouKOuqfFRzLQiq0EEcTP5Jxd9g3CEkLB9I3tE16xjMwV\n"\
"ZCijN30CgYEAh+LAOyKSWtv4O+XE4F/IGOHRdW0a+bw8dvuGUT8ogVCH9HyLZCzM\n"\
"LpqxZhFuNvUTWXulFbsV+JyfE2EGLFhHm4Ov6wfnjI1cQVytPWepMQZqpJFVoKs8\n"\
"wK6lyBWB4WqAO96z8+z4JFXww0CvJf8aO7LPsrUoFrDM61grmGRvn8M=        \n"\
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
