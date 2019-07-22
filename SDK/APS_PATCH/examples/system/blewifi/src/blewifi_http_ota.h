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

#ifndef __BLEWIFI_HTTP_OTA_H__
#define __BLEWIFI_HTTP_OTA_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "blewifi_configuration.h"

#if (WIFI_OTA_FUNCTION_EN == 1)
#define LOG_I(tag, fmt, arg...)             printf("[%s]:" fmt "\r\n",tag , ##arg)
#define LOG_E(tag, fmt, arg...)             printf("[%s]:" fmt "\r\n",tag , ##arg)

#define OTA_BUF_SIZE    (1024 + 1)
#define OTA_URL_BUF_LEN (256)

#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"

uint8_t ota_get_version(uint16_t *, uint16_t *, uint16_t *);
int ota_download_by_http(char *);
int ota_download_by_http_get_server_version(char *param, uint16_t *uwfid);
#endif  /* #if (WIFI_OTA_FUNCTION_EN == 1) */

#ifdef __cplusplus
}
#endif

#endif /* __BLEWIFI_HTTP_OTA_H__ */
