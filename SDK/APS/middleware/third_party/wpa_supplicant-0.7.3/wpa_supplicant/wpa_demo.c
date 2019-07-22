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
 * @file wpa_demo.c
 * @author Michael Liao
 * @date 14 Dec 2017
 * @brief File creates the Wi-Fi Demo capability.
 *
 */
#include "os.h"
#include "sys_common.h"
#include "wpa_demo.h"
#include "driver_netlink.h"
#include "controller_wifi_com.h"

RET_DATA scan_info_t scanInfo;

int wpa_demo_connect_impl(int argc, char *argv[])
{
    //TBD
    return true;
}

int wpa_demo_connect_bssid_impl(char * bssid)
{
    //TBD
    return true;
}

void wpa_demo_send_msg_to_wpas_impl(void)
{
    //Send Message to Supplicant Task
}

void wpa_demo_sck_svr_start_impl(void)
{
    //Start socket server
}

void wpa_demo_hdl_evt_assoc_impl(void)
{
    //TBD
}

void wpa_demo_hdl_evt_scan_results_impl(void)
{
    //TBD
}

void wpa_demo_hdl_evt_deauth_impl(void)
{
    //TBD
}

void wpa_demo_hdl_evt_demo_scan_start_impl(void)
{
    //TBD
}

void wpa_demo_clr_scan_info_impl(void)
{
    os_memset(&scanInfo, 0, sizeof(scanInfo));
}

void wpa_demo_trigger_scan_impl(void)
{
    //TBD
}

RET_DATA wpa_demo_connect_fp_t wpa_demo_connect;
RET_DATA wpa_demo_connect_bssid_fp_t wpa_demo_connect_bssid;
RET_DATA wpa_demo_send_msg_to_wpas_fp_t wpa_demo_send_msg_to_wpas;
RET_DATA wpa_demo_sck_svr_start_fp_t wpa_demo_sck_svr_start;
RET_DATA wpa_demo_hdl_evt_assoc_fp_t wpa_demo_hdl_evt_assoc;
RET_DATA wpa_demo_hdl_evt_scan_results_fp_t wpa_demo_hdl_evt_scan_results;
RET_DATA wpa_demo_hdl_evt_deauth_fp_t wpa_demo_hdl_evt_deauth;
RET_DATA wpa_demo_hdl_evt_demo_scan_start_fp_t wpa_demo_hdl_evt_demo_scan_start;
RET_DATA wpa_demo_clr_scan_info_fp_t wpa_demo_clr_scan_info;
RET_DATA wpa_demo_trigger_scan_fp_t wpa_demo_trigger_scan;

/*
   Interface Initialization: Demo
 */
void wpa_demo_func_init(void)
{
    wpa_demo_connect = wpa_demo_connect_impl;
    wpa_demo_connect_bssid = wpa_demo_connect_bssid_impl;
    wpa_demo_send_msg_to_wpas = wpa_demo_send_msg_to_wpas_impl;
    wpa_demo_sck_svr_start = wpa_demo_sck_svr_start_impl;
    wpa_demo_hdl_evt_assoc = wpa_demo_hdl_evt_assoc_impl;
    wpa_demo_hdl_evt_scan_results = wpa_demo_hdl_evt_scan_results_impl;
    wpa_demo_hdl_evt_deauth = wpa_demo_hdl_evt_deauth_impl;
    wpa_demo_hdl_evt_demo_scan_start = wpa_demo_hdl_evt_demo_scan_start_impl;
    wpa_demo_clr_scan_info = wpa_demo_clr_scan_info_impl;
    wpa_demo_trigger_scan = wpa_demo_trigger_scan_impl;
}

