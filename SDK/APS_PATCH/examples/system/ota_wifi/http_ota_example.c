/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cmsis_os.h"
#include "event_loop.h"
#include "wifi_api.h"
#include "wifi_event.h"
#include "wifi_event_handler.h"
#include "lwip_helper.h"
#include "http_ota_example.h"
#include "httpclient.h"
#include "mw_ota_def.h"
#include "mw_ota.h"
#include "hal_system.h"
#include "lwip/etharp.h"

osThreadId app_task_id;
#define WIFI_READY_TIME 2000

#define LOG_I(tag, fmt, arg...)             printf("[%s]:" fmt "\r\n",tag , ##arg)
#define LOG_E(tag, fmt, arg...)             printf("[%s]:" fmt "\r\n",tag , ##arg)

#define OTA_BUF_SIZE    (1024 + 1)
#define OTA_URL_BUF_LEN (256)

#define  HTP_SERVER_IP    "192.168.1.108" 
#define  HTP_SERVER_PORT    "8000" 
#define HTTP_GET_URL    "http://"HTP_SERVER_IP":"HTP_SERVER_PORT"/opl1000_ota.bin"

/****************************************************************************
 * Static variables
 ****************************************************************************/
static httpclient_t g_fota_httpclient = {0};
static const char *TAG = "ota";
bool g_connection_flag = false;

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
        wifi_do_scan(WIFI_SCAN_TYPE_MIX);
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
        //lwip_get_ip_info("st1");
            g_connection_flag = true;
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

void ota_write_hexdump(const uint8_t *buf, size_t len)
{
    size_t i;
    for (i = 0; i < len; i++) {
        if (i%16 == 0)
            printf("\r\n");
        printf(" %02x", buf[i]);
    }
    printf("\n");
}
static uint8_t ota_get_version(uint16_t *project_id, uint16_t *chip_id, uint16_t *firmware_id)
{
    uint8_t state = MW_OTA_OK;

    state = MwOta_VersionGet(project_id, chip_id, firmware_id);
    return state;
}

static uint8_t ota_prepare(uint16_t project_id, uint16_t chip_id, uint16_t firmware_id, uint32_t img_size, uint32_t img_sum)
{
    uint8_t state = MW_OTA_OK;

    state = MwOta_Prepare(project_id, chip_id, firmware_id, img_size, img_sum);
    return state;
}

static uint8_t ota_data_write(uint8_t *pubAddr, uint32_t ulSize)
{
    uint8_t state = MW_OTA_OK;

    state = MwOta_DataIn(pubAddr, ulSize);
    return state;
}

static uint8_t ota_data_finish(void)
{
    uint8_t state = MW_OTA_OK;

    state = MwOta_DataFinish();
    return state;
}

static uint8_t ota_abort(void)
{
    uint8_t state = MW_OTA_OK;
    state = MwOta_DataGiveUp();
    return state;
}

int ota_http_retrieve_offset(httpclient_t *client, httpclient_data_t *client_data, int offset, int parse_hdr)
{
    int ret = HTTPCLIENT_ERROR_CONN;
    uint8_t ota_hdr1_start = 0;
    T_MwOtaFlashHeader *ota_hdr = NULL;

    ota_hdr1_start = parse_hdr;
    do {
        ret = httpclient_recv_response(client, client_data);
        if (ret < 0) {
            LOG_E(TAG, "http client recv response error, ret = %d \r\n", ret);
            return ret;
        }
        if (ota_hdr1_start == 1) {
            ota_hdr = (T_MwOtaFlashHeader*)client_data->response_buf;
            LOG_E(TAG, "proj_id=%d, chip_id=%d, fw_id=%d, checksum=%d, total_len=%d\r\n",
                ota_hdr->uwProjectId, ota_hdr->uwChipId, ota_hdr->uwFirmwareId, ota_hdr->ulImageSum, ota_hdr->ulImageSize);
            if (ota_prepare(ota_hdr->uwProjectId, ota_hdr->uwChipId, ota_hdr->uwFirmwareId, ota_hdr->ulImageSize, ota_hdr->ulImageSum) != MW_OTA_OK) {
                LOG_I(TAG, "ota_prepare fail\r\n");
                return -1;
            }
            ota_hdr1_start = 0;
        }
        if ( (client_data->response_content_len - client_data->retrieve_len) == offset )
            break;
    } while (ret == HTTPCLIENT_RETRIEVE_MORE_DATA);
    return HTTPCLIENT_OK;
}
int ota_http_retrieve_get(char* get_url, char* buf, uint32_t len)
{
    int ret = HTTPCLIENT_ERROR_CONN;
    httpclient_data_t client_data = {0};
    uint32_t write_count = 0;
    uint32_t data_recv = 0;
    uint32_t recv_temp = 0;
    uint32_t data_len = 0;

    client_data.response_buf = buf;
    client_data.response_buf_len = len;

    // Send request to server
    ret = httpclient_send_request(&g_fota_httpclient, get_url, HTTPCLIENT_GET, &client_data);
    if (ret < 0) {
        LOG_E(TAG, "http client fail to send request \r\n");
        return ret;
    }


        // Receive response from server
    ret = ota_http_retrieve_offset(&g_fota_httpclient, &client_data, MW_OTA_HEADER_ADDR_1, 0);
        if (ret < 0) {
        LOG_E(TAG, "http retrieve offset error, ret = %d \r\n", ret);
            return ret;
        }

        // Response body start
    ret = ota_http_retrieve_offset(&g_fota_httpclient, &client_data, MW_OTA_HEADER_ADDR_2, 1);
    if (ret < 0) {
        LOG_E(TAG, "http retrieve offset error, ret = %d \r\n", ret);



        return ret;
            }

    // skip 2st OTA header,  0x00003000 ~ 0x00004000 : 4 KB

    ret = ota_http_retrieve_offset(&g_fota_httpclient, &client_data, MW_OTA_IMAGE_ADDR_1, 0);
    if (ret < 0) {
        LOG_E(TAG, "http retrieve offset error, ret = %d \r\n", ret);
        return ret;
            }
    recv_temp = client_data.retrieve_len;
    data_recv = client_data.response_content_len - client_data.retrieve_len;
    do {
        ret = httpclient_recv_response(&g_fota_httpclient, &client_data);
        if (ret < 0) {
            LOG_E(TAG, "http client recv response error, ret = %d \r\n", ret);
            return ret;
        }

            data_len = recv_temp - client_data.retrieve_len;
            if (ota_data_write((uint8_t*)client_data.response_buf, data_len) != MW_OTA_OK) {
                LOG_I(TAG, "ota_data_write fail\r\n");
                return -1;
        }

        write_count += data_len;
        data_recv += data_len;
        recv_temp = client_data.retrieve_len;
        LOG_I(TAG, "have written image length %d\r\n", write_count);
        LOG_I(TAG, "download progress = %u \r\n", data_recv * 100 / client_data.response_content_len);

    } while (ret == HTTPCLIENT_RETRIEVE_MORE_DATA);

    LOG_I(TAG, "total write data length : %d\r\n", write_count);
    LOG_I(TAG, "data received length : %d\r\n", data_recv);

    if (data_recv != client_data.response_content_len || httpclient_get_response_code(&g_fota_httpclient) != 200) {
        LOG_E(TAG, "data received not completed, or invalid error code \r\n");
        return -1;
    }
    else if (data_recv == 0) {
        LOG_E(TAG, "receive length is zero, file not found \n");
        return -2;
    }
    else {
        LOG_I(TAG, "download success \n");
        return ret;
    }
}

int ota_download_by_http(char *param)
{
    char get_url[OTA_URL_BUF_LEN];
    int32_t ret = HTTPCLIENT_ERROR_CONN;
    uint32_t len_param = strlen(param);
    uint16_t retry_count = 0;
    uint16_t pid;
    uint16_t cid;
    uint16_t fid;

    if (len_param < 1) {
      return -1;
    }

    memset(get_url, 0, OTA_URL_BUF_LEN);
    LOG_I(TAG, "url length: %d\n", strlen(param));
    strcpy(get_url, param);

    char* buf = pvPortMalloc(OTA_BUF_SIZE);
    if (buf == NULL) {
        LOG_E(TAG, "buf malloc failed.\r\n");
        return -1;
    }

    lwip_auto_arp_enable(1, 0);

    // Connect to server
    do {
    ret = httpclient_connect(&g_fota_httpclient, get_url);
    if (!ret) {
        LOG_I(TAG, "connect to http server");
                break;
    } else {
            LOG_I(TAG, "connect to http server failed! retry again");
                vTaskDelay(1000);
                retry_count++;
                continue;
    }
    } while(retry_count < 3);

    ota_get_version(&pid, &cid, &fid);

    LOG_I(TAG, "pid=%d, cid=%d, fid=%d\r\n", pid, cid, fid);

    ret = ota_http_retrieve_get(get_url, buf, OTA_BUF_SIZE);
    if (ret < 0) {
        if (ota_abort() != MW_OTA_OK) {
            LOG_E(TAG, "ota_abort error.\r\n");
        }
    } else {
        if (ota_data_finish() != MW_OTA_OK) {
            LOG_E(TAG, "ota_data_finish error.\r\n");
        }
    }

    LOG_I(TAG, "download result = %d \r\n", (int)ret);

    // Close http connection
    httpclient_close(&g_fota_httpclient);
    vPortFree(buf);
    buf = NULL;

    if ( HTTPCLIENT_OK == ret)
        return 0;
    else
        return -1;
}

void user_wifi_app_entry(void *args)
{
      g_connection_flag = false; 
    
    /* Tcpip stack and net interface initialization,  dhcp client process initialization. */
    lwip_network_init(WIFI_MODE_STA);

    /* Waiting for connection & got IP from DHCP server */
    lwip_net_ready();

      if(g_connection_flag == true)
          LOG_I(TAG,"Opulinks-TEST-AP connected \r\n");
    
    if (ota_download_by_http(HTTP_GET_URL) == 0)
    {
        //Hal_Sys_SwResetAll();
    }

    while (1) {
        osDelay(2000);
    }
}

void OtaAppInit(void)
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
    task_def.stacksize = OS_TASK_STACK_SIZE_APP*2;
    task_def.tpriority = OS_TASK_PRIORITY_APP;
    task_def.pthread = user_wifi_app_entry;
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










