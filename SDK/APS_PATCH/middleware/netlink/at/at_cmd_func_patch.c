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

#include "at_cmd_sys_patch.h"
#include "at_cmd_app_patch.h"
#include "at_cmd_tcpip_patch.h"
#include "at_cmd_task_patch.h"
#include "at_cmd_wifi_patch.h"
#include "at_cmd_patch.h"
#include "at_cmd_common_patch.h"
#include "at_cmd_ext_patch.h"
#include "at_cmd_msg_ext_patch.h"

void at_func_init_patch(void)
{
#if !defined(SDK_LITE)
    at_cmd_app_func_preinit_patch();
#endif
    at_cmd_sys_func_init_patch();
    at_task_func_init_patch();
#if !defined(SDK_LITE)
    at_cmd_tcpip_api_preinit_patch();
    at_cmd_wifi_func_init_patch();
    at_cmd_func_init_patch();
#endif
    at_cmd_common_func_init_patch();
#if !defined(SDK_LITE)
    at_msg_ext_init_patch();
    // add new at command table
    at_cmd_ext_tbl_reg(gAtCmdTbl_Ext);
#endif
}
