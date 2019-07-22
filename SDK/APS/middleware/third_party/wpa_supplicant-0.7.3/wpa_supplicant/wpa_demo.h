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
 * @file wpa_demo.h
 * @author Michael Liao
 * @date 14 Dec 2017
 * @brief File containing declaration of demo related api & definition of structure for reference.
 *
 */

#ifndef __WPA_DEMO_H__
#define __WPA_DEMO_H__
#include "common.h"

typedef int (*wpa_demo_connect_fp_t)(int argc, char *argv[]);
typedef int (*wpa_demo_connect_bssid_fp_t)(char * bssid);
typedef void (*wpa_demo_send_msg_to_wpas_fp_t)(void);
typedef void (*wpa_demo_sck_svr_start_fp_t)(void);
typedef void (*wpa_demo_hdl_evt_assoc_fp_t)(void);
typedef void (*wpa_demo_hdl_evt_scan_results_fp_t)(void);
typedef void (*wpa_demo_hdl_evt_deauth_fp_t)(void);
typedef void (*wpa_demo_hdl_evt_demo_scan_start_fp_t)(void);
typedef void (*wpa_demo_clr_scan_info_fp_t)(void);
typedef void (*wpa_demo_trigger_scan_fp_t)(void);

extern wpa_demo_connect_fp_t wpa_demo_connect;
extern wpa_demo_connect_bssid_fp_t wpa_demo_connect_bssid;
extern wpa_demo_send_msg_to_wpas_fp_t wpa_demo_send_msg_to_wpas;
extern wpa_demo_sck_svr_start_fp_t wpa_demo_sck_svr_start;
extern wpa_demo_hdl_evt_assoc_fp_t wpa_demo_hdl_evt_assoc;
extern wpa_demo_hdl_evt_scan_results_fp_t wpa_demo_hdl_evt_scan_results;
extern wpa_demo_hdl_evt_deauth_fp_t wpa_demo_hdl_evt_deauth;
extern wpa_demo_hdl_evt_demo_scan_start_fp_t wpa_demo_hdl_evt_demo_scan_start;
extern wpa_demo_clr_scan_info_fp_t wpa_demo_clr_scan_info;
extern wpa_demo_trigger_scan_fp_t wpa_demo_trigger_scan;

/*
   Interface Initialization: Demo
 */
void wpa_demo_func_init(void);

#endif //__WPA_DEMO_H__

