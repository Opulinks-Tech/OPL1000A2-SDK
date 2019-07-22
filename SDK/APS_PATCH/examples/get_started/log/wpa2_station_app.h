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

#ifndef __WPA2_STATION_APP_H__
#define __WPA2_STATION_APP_H__

#include <stdint.h>
#include <stdbool.h>
#include "cmsis_os.h"

#ifdef __cplusplus
extern "C" {
#endif

// the number of elements in the message queue
#define APP_MESSAGE_Q_SIZE  16
    
#define WIFI_SSID               "Opulinks-TEST-AP"
#define WIFI_PASSWORD           "1234abcd"

#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"

#define  UAER_WIFI_APP_RE_CONNECT         0x55AADD 
#define  UAER_WIFI_APP_CONNECT_COMPLETE   0x55AAFF 

typedef struct
{
    uint32_t ulEventId;
    bool bConnectResult; 
} S_MessageQ;

void WifiAppInit(void);

osStatus wifi_AppMessageQSend(S_MessageQ *ptMsg, osMessageQId message_id, osPoolId pool_id);

extern bool g_AP_connect_result;
extern bool g_AP_connect_complete;

#ifdef __cplusplus
}
#endif

#endif /* __EVENT_LOOP_H__ */
