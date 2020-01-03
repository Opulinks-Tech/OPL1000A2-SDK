/* HTTPS GET Example using plain mbedTLS sockets
 *
 * Contacts the howsmyssl.com API via TLS v1.2 and reads a JSON
 * response.
 *
 * Adapted from the ssl_client1 example in mbedtls.
 *
 * Original Copyright (C) 2006-2016, ARM Limited, All Rights Reserved, Apache 2.0 License.
 * Additions Copyright (C) Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD, Apache 2.0 License.
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cmsis_os.h"
#include "event_loop.h"
#include "wifi_api.h"
#include "wifi_event.h"
#include "wifi_event_handler.h"
#include "lwip_helper.h"
#include "cmsis_os.h"

#include "mbedtls/net.h"
#include "mbedtls/debug.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "mbedtls/certs.h"

#include "https_client_request.h"


#define LOG_I(tag, fmt, arg...)             printf("[%s]:" fmt "\r\n",tag , ##arg)
#define LOG_E(tag, fmt, arg...)             printf("[%s]:" fmt "\r\n",tag , ##arg)

static const char *TAG = "https_request";

osThreadId app_task_id;
#define WIFI_READY_TIME 2000


#define SERVER_PORT "443"
#define SERVER_NAME "www.howsmyssl.com"
#define GET_REQUEST "GET https://"SERVER_NAME"/a/check HTTP/1.1\r\nHost: "SERVER_NAME"\r\n\r\n"

const char ssl_client_ca_crt[] =
"-----BEGIN CERTIFICATE-----\r\n"
"MIIDSjCCAjKgAwIBAgIQRK+wgNajJ7qJMDmGLvhAazANBgkqhkiG9w0BAQUFADA/\r\n"
"MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\r\n"
"DkRTVCBSb290IENBIFgzMB4XDTAwMDkzMDIxMTIxOVoXDTIxMDkzMDE0MDExNVow\r\n"
"PzEkMCIGA1UEChMbRGlnaXRhbCBTaWduYXR1cmUgVHJ1c3QgQ28uMRcwFQYDVQQD\r\n"
"Ew5EU1QgUm9vdCBDQSBYMzCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB\r\n"
"AN+v6ZdQCINXtMxiZfaQguzH0yxrMMpb7NnDfcdAwRgUi+DoM3ZJKuM/IUmTrE4O\r\n"
"rz5Iy2Xu/NMhD2XSKtkyj4zl93ewEnu1lcCJo6m67XMuegwGMoOifooUMM0RoOEq\r\n"
"OLl5CjH9UL2AZd+3UWODyOKIYepLYYHsUmu5ouJLGiifSKOeDNoJjj4XLh7dIN9b\r\n"
"xiqKqy69cK3FCxolkHRyxXtqqzTWMIn/5WgTe1QLyNau7Fqckh49ZLOMxt+/yUFw\r\n"
"7BZy1SbsOFU5Q9D8/RhcQPGX69Wam40dutolucbY38EVAjqr2m7xPi71XAicPNaD\r\n"
"aeQQmxkqtilX4+U9m5/wAl0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNV\r\n"
"HQ8BAf8EBAMCAQYwHQYDVR0OBBYEFMSnsaR7LHH62+FLkHX/xBVghYkQMA0GCSqG\r\n"
"SIb3DQEBBQUAA4IBAQCjGiybFwBcqR7uKGY3Or+Dxz9LwwmglSBd49lZRNI+DT69\r\n"
"ikugdB/OEIKcdBodfpga3csTS7MgROSR6cz8faXbauX+5v3gTt23ADq1cEmv8uXr\r\n"
"AvHRAosZy5Q6XkjEGB5YGV8eAlrwDPGxrancWYaLbumR9YbK+rlmM6pZW87ipxZz\r\n"
"R8srzJmwN0jP41ZL9c8PDHIyh8bwRLtTcm1D9SZImlJnt1ir/md2cXjbDaJWFBM5\r\n"
"JDGFoqgCWjBH4d1QB7wCCZAA62RjYJsWvIjJEubSfZGL+T0yjWW06XyxV3bqxbYo\r\n"
"Ob8VZRzI9neWagqNdwvYkQsEjgfbKbYK7p2CNTUQ\r\n"
"-----END CERTIFICATE-----\r\n";

#define DEBUG_LEVEL 5

static void my_debug( void *ctx, int level,
                      const char *file, int line,
                      const char *str )
{
    const char *p, *basename;

    /* Extract basename from file */
    for( p = basename = file; *p != '\0'; p++ )
        if( *p == '/' || *p == '\\' )
            basename = p + 1;

    printf("%s:%04d: |%d| %s", basename, line, level, str );
}

static int ssl_client_start( void )
{
    int ret = 0, len;
    mbedtls_net_context server_fd;
    uint32_t flags, read_data_len = 0;
    unsigned char buf[512];
    const char *pers = "ssl_client";

    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;
    mbedtls_x509_crt cacert;
    
    static const int ciphersuite_preference[] =
    {
        MBEDTLS_TLS_RSA_WITH_AES_256_CBC_SHA256,
        MBEDTLS_TLS_RSA_WITH_AES_256_CBC_SHA,
        MBEDTLS_TLS_RSA_WITH_AES_128_CBC_SHA256,
        MBEDTLS_TLS_RSA_WITH_AES_128_CBC_SHA,
    };
    
        mbedtls_ssl_setup_preference_ciphersuites(ciphersuite_preference);

    LOG_I(TAG, "SSL client demonstration starts...\n");

#if defined(MBEDTLS_DEBUG_C)
    mbedtls_debug_set_threshold(DEBUG_LEVEL );
#endif

    /*
     * 0. Initialize the RNG and the session data
     */
    mbedtls_net_init( &server_fd );
    mbedtls_ssl_init( &ssl );
    mbedtls_ssl_config_init( &conf );
    mbedtls_x509_crt_init( &cacert );
    mbedtls_ctr_drbg_init( &ctr_drbg );

    LOG_I(TAG, "Seeding the random number generator...");

    mbedtls_entropy_init( &entropy );
    LOG_I(TAG, "entropy_inited");
    if ((ret = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func, &entropy,
                               (const unsigned char *)pers, strlen(pers))) != 0)
    {
        LOG_I(TAG,"failed\n  ! mbedtls_ctr_drbg_seed returned %d\n", ret );
        goto exit;
    }

    /*
     * 0. Initialize certificates
     */
    LOG_I(TAG, "Loading the CA root certificate ...");

    ret = mbedtls_x509_crt_parse(&cacert, (const unsigned char *) ssl_client_ca_crt,
                          sizeof(ssl_client_ca_crt));
    if (ret < 0)
    {
        LOG_I(TAG, "mbedtls_x509_crt_parse returned -0x%x\n\n", -ret );
        goto exit;
    }

    LOG_I(TAG, "ok (%d skipped)\n", ret);

    /*
     * 1. Start the connection
     */
    LOG_I(TAG, "Connecting to tcp/%s/%s...", SERVER_NAME, SERVER_PORT);

    if ((ret = mbedtls_net_connect(&server_fd, SERVER_NAME,
                                   SERVER_PORT, MBEDTLS_NET_PROTO_TCP)) != 0)
    {
        LOG_I(TAG, "mbedtls_net_connect returned %d\n\n", ret);
        goto exit;
    }

    LOG_I(TAG, "ok\n");

    /*
     * 2. Setup stuff
     */
    LOG_I(TAG, "Setting up the SSL/TLS structure...");

    if ((ret = mbedtls_ssl_config_defaults(&conf,
                    MBEDTLS_SSL_IS_CLIENT,
                    MBEDTLS_SSL_TRANSPORT_STREAM,
                    MBEDTLS_SSL_PRESET_DEFAULT)) != 0)
    {
        LOG_I(TAG, "mbedtls_ssl_config_defaults returned %d\n\n", ret);
        goto exit;
    }

    LOG_I(TAG, "ok\n");

    /* OPTIONAL is not optimal for security,
     * but makes interop easier in this simplified example */
    mbedtls_ssl_conf_authmode(&conf, MBEDTLS_SSL_VERIFY_OPTIONAL);
    mbedtls_ssl_conf_ca_chain(&conf, &cacert, NULL );
    mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);
    mbedtls_ssl_conf_dbg(&conf, my_debug, stdout);
    if ((ret = mbedtls_ssl_setup(&ssl, &conf)) != 0)
    {
        LOG_I(TAG, "mbedtls_ssl_setup returned %d\n\n", ret);
        goto exit;
    }

    if ((ret = mbedtls_ssl_set_hostname(&ssl, SERVER_NAME)) != 0)
    {
        LOG_I(TAG, "mbedtls_ssl_set_hostname returned %d\n\n", ret);
        goto exit;
    }

    mbedtls_ssl_set_bio(&ssl, &server_fd, mbedtls_net_send, mbedtls_net_recv, NULL);
    /*
     * 4. Handshake
     */
    LOG_I(TAG, "Performing the SSL/TLS handshake...");

    while ((ret = mbedtls_ssl_handshake( &ssl ) ) != 0)
    {
        if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE)
        {
            LOG_I(TAG, "mbedtls_ssl_handshake returned -0x%x\n\n", -ret);
            goto exit;
        }
    }

    LOG_I(TAG, "ok\n");

    /*
     * 5. Verify the server certificate
     */
    LOG_I(TAG, "Verifying peer X.509 certificate...");

    /* In real life, we probably want to bail out when ret != 0 */
    if ((flags = mbedtls_ssl_get_verify_result( &ssl )) != 0)
    {
        memset(buf, 0, sizeof(buf));
        LOG_I(TAG, "Failed to verify peer certificate!");
        mbedtls_x509_crt_verify_info((char*)&buf[0], sizeof(buf), "  ! ", flags);
        LOG_I(TAG, "verification info: %s", buf);
    }
    else
    {
        LOG_I(TAG, "Certificate verified.");
    }

    LOG_I(TAG, "Cipher suite is %s", mbedtls_ssl_get_ciphersuite(&ssl));

    LOG_I(TAG, "Writing HTTP request...");

    /*
     * 3. Write the GET request
     */
    memset(buf, 0, sizeof(buf));
    len = sprintf( (char *) buf, GET_REQUEST );

    while ((ret = mbedtls_ssl_write(&ssl, buf, len)) <= 0)
    {
        if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE)
        {
            LOG_I(TAG, " failed\n  ! mbedtls_ssl_write returned %d\n\n", ret);
            goto exit;
        }
    }

    len = ret;
    LOG_I(TAG, "%d bytes written\n\n%s", len, (char *) buf);

    /*
     * 7. Read the HTTP response
     */
    LOG_I(TAG, "Reading HTTP response...");
    do
    {
        len = sizeof(buf) - 1;
        memset(buf, 0, sizeof(buf));
        ret = mbedtls_ssl_read( &ssl, buf, len);

        if (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE)
            continue;

        if (ret == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY)
            break;

        if (ret < 0)
        {
            LOG_I(TAG, "mbedtls_ssl_read returned %d\n\n", ret);
            break;
        }

        if (ret == 0)
        {
            LOG_I(TAG,, "connection closed");
            break;
        }

        len = ret;
        read_data_len = len;
        LOG_I(TAG, "%d bytes read\n\n%s", len, (char *) buf);
    }while(1);

    mbedtls_ssl_close_notify( &ssl );

exit:

#ifdef MBEDTLS_ERROR_C
    if( ret != 0 )
    {
        char error_buf[100];
        mbedtls_strerror( ret, error_buf, 100 );
        LOG_I(TAG, "Last error was: %d - %s\n\n", ret, error_buf );
    }
#endif

    mbedtls_net_free( &server_fd );
    mbedtls_x509_crt_free( &cacert );
    mbedtls_ssl_free( &ssl );
    mbedtls_ssl_config_free( &conf );
    mbedtls_ctr_drbg_free( &ctr_drbg );
    mbedtls_entropy_free( &entropy );

    LOG_I(TAG, "SSL client demonstration ends...\n");

    return read_data_len > 0 ? 0 : ret;
}

void wifi_wait_ready(void)
{
    /* wait a while for system ready */
    osDelay(WIFI_READY_TIME);
}

int wifi_do_scan(int mode)
{
    wifi_scan_config_t scan_config;
    memset(&scan_config, 0, sizeof(scan_config));
    wifi_scan_start(&scan_config, NULL);
    return 0;
}

int wifi_connection(void)
{
    int iRet = -1;
    wifi_config_t wifi_config = {0};
    wifi_scan_list_t *p_scan_list = NULL;
    int i = 0;
    int isMatched = 0;

    p_scan_list = (wifi_scan_list_t *)malloc(sizeof(wifi_scan_list_t));

    if(p_scan_list == NULL)
    {
        goto done;
    }

    memset(p_scan_list, 0, sizeof(wifi_scan_list_t));

    /* Read Confguration */
    wifi_get_config(WIFI_MODE_STA, &wifi_config);

    /* Get APs list */
    wifi_scan_get_ap_list(p_scan_list);

    /* Search if AP matched */
    for (i=0; i< p_scan_list->num; i++) {
        if (memcmp(p_scan_list->ap_record[i].bssid, wifi_config.sta_config.bssid, WIFI_MAC_ADDRESS_LENGTH) == 0)
        {
            isMatched = 1;
            break;
        }

        if (memcmp(p_scan_list->ap_record[i].ssid, wifi_config.sta_config.ssid, wifi_config.sta_config.ssid_length) == 0)
        {
            isMatched = 1;
            break;
        }
    }

    if(isMatched == 1) {
        /* Wi-Fi Connection */
        wifi_connection_connect(&wifi_config);
    } else {
        /* Scan Again */
        wifi_do_scan(WIFI_SCAN_TYPE_ACTIVE);
    }

    iRet = 0;

done:
    if(p_scan_list)
    {
        free(p_scan_list);
    }

    return iRet;
}

int wifi_event_handler_cb(wifi_event_id_t event_id, void *data, uint16_t length)
{
    switch(event_id) {
    case WIFI_EVENT_STA_START:
        printf("\r\nWi-Fi Start \r\n");
        wifi_wait_ready();
        wifi_do_scan(WIFI_SCAN_TYPE_ACTIVE);
        break;
    case WIFI_EVENT_STA_CONNECTED:
        lwip_net_start(WIFI_MODE_STA);
        printf("\r\nWi-Fi Connected \r\n");
        break;
    case WIFI_EVENT_STA_DISCONNECTED:
        printf("\r\nWi-Fi Disconnected \r\n");
        wifi_do_scan(WIFI_SCAN_TYPE_ACTIVE);
        break;
    case WIFI_EVENT_SCAN_COMPLETE:
        printf("\r\nWi-Fi Scan Done \r\n");
        wifi_connection();
        break;
    case WIFI_EVENT_STA_GOT_IP:
        printf("\r\nWi-Fi Got IP \r\n");
        lwip_get_ip_info("st1");
        break;
    case WIFI_EVENT_STA_CONNECTION_FAILED:
        printf("\r\nWi-Fi Connected failed\r\n");
        wifi_do_scan(WIFI_SCAN_TYPE_ACTIVE);
        break;
    default:
        printf("\r\n Unknown Event %d \r\n", event_id);
        break;
    }
    return 0;
}


void app_entry(void *args)
{
    /* Tcpip stack and net interface initialization,  dhcp client process initialization. */
    lwip_network_init(WIFI_MODE_STA);

    /* Waiting for connection & got IP from DHCP server */
    lwip_net_ready();

    ssl_client_start();

    while (1) {
        osDelay(2000);
    }
}

void AppInit(void)
{
    osThreadDef_t task_def;
    wifi_init_config_t int_cfg = {.event_handler = (wifi_event_notify_cb_t)&wifi_event_loop_send, .magic = 0x1F2F3F4F};
    wifi_config_t wifi_config = {0};

    unsigned char bssid[WIFI_MAC_ADDRESS_LENGTH] = {0x78, 0x32, 0x1b, 0x3b, 0x2d, 0x24};

    /* Event Loop Initialization */
    wifi_event_loop_init((wifi_event_cb_t)wifi_event_handler_cb);

    /* Initialize wifi stack and register wifi init complete event handler */
    wifi_init(&int_cfg, NULL);

    /* Set user's configuration */
    strcpy((char *)wifi_config.sta_config.ssid, WIFI_SSID);
    strcpy((char *)wifi_config.sta_config.password, WIFI_PASSWORD);
    wifi_config.sta_config.ssid_length = strlen(WIFI_SSID);
    wifi_config.sta_config.password_length = strlen(WIFI_PASSWORD);
    memcpy(wifi_config.sta_config.bssid, bssid, WIFI_MAC_ADDRESS_LENGTH);

    wifi_set_config(WIFI_MODE_STA, &wifi_config);

    /* Wi-Fi operation start */
    wifi_start();

    /* Create task */
    task_def.name = "user_app";
    task_def.stacksize = 512*3;
    task_def.tpriority = OS_TASK_PRIORITY_APP;
    task_def.pthread = app_entry;
    app_task_id = osThreadCreate(&task_def, (void*)NULL);
    if(app_task_id == NULL)
    {
        printf("user_app Task create fail \r\n");
    }
    else
    {
        printf("user_app Task create successful \r\n");
    }
}

