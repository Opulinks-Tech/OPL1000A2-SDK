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
#include "Hal_pinmux_uart.h"
#include "Hal_pinmux_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

#define WIFI_SSID               "Opulinks-TEST-AP"
#define WIFI_PASSWORD           "1234abcd"

#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"


void WifiAppInit(void);

#ifdef __cplusplus
}
#endif

#endif /* __EVENT_LOOP_H__ */
