/******************************************************************************
*  Copyright 2019, Netlink Communication Corp.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Netlink Communication Corp. (C) 2019
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "at_cmd.h"
#include "wifi_api.h"
#include "at_cmd_msg_ext_patch.h"

extern int g_wifi_argc;
extern char *g_wifi_argv[AT_MAX_CMD_ARGS];
/*support SCAN HISTORY*/
extern u16 g_scan_history_end;
wifi_scan_info_t g_scan_history[16]={0};

extern void at_msg_ext_wifi_show_one_ap_impl(int argc, char *argv[]);

#ifdef __ATCWLAP_SCAN_HISTORY_EN__
void at_msg_ext_wifi_show_all_scan_history_patch(int argc, char *argv[])
{
    u16 apCount = 0;
    
    wifi_scan_get_ap_num(&apCount);
    if(apCount == 0) return;
    
    wifi_scan_get_ap_records(&apCount, g_scan_history);
    
    at_msg_ext_wifi_scan_by_option(g_scan_history_end+1, g_scan_history);
}
#endif

void at_msg_ext_init_patch(void)
{
    #ifdef __ATCWLAP_SCAN_HISTORY_EN__
    at_msg_ext_wifi_show_all = at_msg_ext_wifi_show_all_scan_history_patch;
    #endif
}
