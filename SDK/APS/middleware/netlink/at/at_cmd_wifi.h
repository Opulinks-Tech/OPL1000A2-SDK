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
/**
 * @file at_cmd_wifi.h
 * @author Michael Liao
 * @date 14 Dec 2017
 * @brief File containing declaration of at_cmd_wifi api & definition of structure for reference.
 *
 */

#ifndef __AT_CMD_WIFI_H__
#define __AT_CMD_WIFI_H__

#define AT_WIFI_SHOW_ECN_BIT        0x00000001U
#define AT_WIFI_SHOW_SSID_BIT       0x00000002U
#define AT_WIFI_SHOW_RSSI_BIT       0x00000004U
#define AT_WIFI_SHOW_MAC_BIT        0x00000008U
#define AT_WIFI_SHOW_CHANNEL_BIT    0x00000010U
#define AT_WIFI_SHOW_ALL_BIT        0x0000001FU

#define MAX_NUM_OF_HOST_NAME_SIZE 32 //I0060 0000664

typedef enum {
    AT_WIFI_SKIP_DTIM_CFG,
    AT_WIFI_MAX_NUM,
} at_wifi_mac_cfg_e;

/*
 * @brief AT Command Interface Initialization for Wi-Fi modules
 *
 */
void at_cmd_wifi_func_init(void);

#endif //__AT_CMD_WIFI_H__

