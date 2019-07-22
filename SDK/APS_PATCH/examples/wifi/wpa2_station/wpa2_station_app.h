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

#ifdef __cplusplus
extern "C" {
#endif

#define  WIFI_SSID    "Opulinks-TEST-AP" 
#define  WIFI_PASSWORD    "1234abcd" 

#define  DTIM_SKIP_COUNT    9
#define  POWER_SAVE_EN      1

//Define following Macro to set LP mode; otherwise, OPL1000 would be in HP mode by default.
#define  __RF_LP_MODE__
//Define following Macro to enable setting MAC address, which is enabled by default.
//#define  __UPDATE_MAC_ADDR_FLAG__

#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "\n%02x:%02x:%02x:%02x:%02x:%02x\n"

#if defined(__UPDATE_MAC_ADDR_FLAG__)
#define  OPL_STA_MAC_ADDR	      {0x78, 0x22, 0x33, 0x44, 0x55, 0x67}

void MacAddrInit(void);
#endif

void WifiAppInit(void);

#ifdef __cplusplus
}
#endif

#endif /* __EVENT_LOOP_H__ */

















