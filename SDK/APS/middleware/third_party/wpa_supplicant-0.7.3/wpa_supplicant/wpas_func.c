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
#include <string.h>
#include "wpas_func.h"
#include "wpa_cli.h"
#include "wpa_common.h"
#include "wpa.h"
#include "wpa_supplicant_i.h"
#include "wpa_ie.h"
#include "wpabuf.h"
#include "wpa_debug.h"
#include "common.h"
#include "scan.h"
#include "bss.h"
#include "notify.h"
#include "l2_packet.h"
#include "events_netlink.h"
#include "eloop.h"
#include "driver_netlink.h"
#include "wpas_glue.h"
#include "wpa_demo.h"
#include "wpa_at_if.h"

extern void wpa_supplicant_task_func_init(void);

void wpas_func_init(void)
{
    wpa_cli_func_init();
    wpa_common_func_init();
    wpa_func_init();
    wpa_supplicant_func_init();
    wpa_ie_func_init();
    wpa_buf_func_init();
    wpa_glue_func_init();
    wpa_debug_func_init();
    wpa_supplicant_task_func_init();
    wpa_utils_common_func_init();
    wpa_scan_func_init();
    wpa_bss_func_init();
    wpa_notify_func_init();
    wpa_l2_packet_func_init();
    wpa_events_func_init();
    wpa_eloop_func_init();
    wpa_driver_func_init();
    wpa_demo_func_init();
    wpa_at_if_func_init();
}

