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

#ifndef __HTTP_REQUEST_H__
#define __HTTP_REQUEST_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define WIFI_SSID               "Opulinks-TEST-AP"
#define WIFI_PASSWORD           "1234abcd"

#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"

typedef struct {
    uint8_t li_vn_mode;
    uint8_t stratum;
    uint8_t poll;
    uint8_t precision;
    int32_t rootdelay;
    int32_t rootdispersion;
    int8_t  refid[4];
    int32_t reftimeint;
    int32_t reftimefraction;
    int32_t oritimeint;
    int32_t oritimefraction;
    int32_t recvtimeint;
    int32_t recvtimefraction;
    int32_t trantimeint;
    int32_t trantimefraction;
} sntp_header_t;

#define DEFF_1970    (2209017600)  /* 1970 - 1900 in seconds */
#define DEFF_8HOUR   (28800)

#define CONVERT_TIME(s) (s - DEFF_1970 + 2 * DEFF_8HOUR)
    
    
void WifiAppInit(void);

#ifdef __cplusplus
}
#endif

#endif /* __HTTP_REQUEST_H__ */
