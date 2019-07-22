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
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "opulinks.h"
#include "os.h"
#include "at_cmd.h"
#include "at_cmd_sys.h"
#include "at_cmd_wifi.h"
#include "at_cmd_ble.h"
#include "at_cmd_tcpip.h"
#include "at_cmd_rf.h"
#include "at_cmd_pip.h"
#include "at_cmd_others.h"
#include "at_cmd_nvm.h"
#include "le_ctrl.h"
#include "hal_uart.h"
#include "at_cmd_common.h"
#include "at_cmd_property.h"
#if defined(__AT_CMD_TASK__)
#include "at_cmd_task.h"
#endif
#include "at_cmd_data_process.h"

#include "at_cmd_patch.h"
#include "at_cmd_data_process_patch.h"

/*
 * @brief An external Function at_cmd_handler prototype declaration retention attribute segment
 *
 */
extern at_cmd_handler_fp_t at_cmd_handler;

/*
 * @brief An external Function at_cmd_parse prototype declaration retention attribute segment
 *
 */
extern at_cmd_parse_fp_t at_cmd_parse;

/*
 * @brief An external Function at_cmd_extend prototype declaration retention attribute segment
 *
 * For extending more commands
 */
//RET_DATA at_cmd_extend_fp_t at_cmd_extend; /** For Future Extend Command Support */

/*
 * @brief An external Function at_cmd_ext_tbl_reg prototype declaration retention attribute segment
 *
 */
extern at_cmd_ext_tbl_reg_fp_t at_cmd_ext_tbl_reg;

/*
 * @brief An external global variable g_AtCmdTbl_Sys_Ptr prototype declaration
 *
 */
extern at_command_t *g_AtCmdTbl_Sys_Ptr;

/*
 * @brief An external global variable g_AtCmdTbl_Wifi_Ptr prototype declaration
 *
 */
extern at_command_t *g_AtCmdTbl_Wifi_Ptr;

/*
 * @brief An external global variable g_AtCmdTbl_Ble_Ptr prototype declaration
 *
 */
extern at_command_t *g_AtCmdTbl_Ble_Ptr;

/*
 * @brief An external global variable g_AtCmdTbl_Tcpip_Ptr prototype declaration
 *
 */
extern at_command_t *g_AtCmdTbl_Tcpip_Ptr;

/*
 * @brief An external global variable g_AtCmdTbl_Rf_Ptr prototype declaration
 *
 */
extern at_command_t *g_AtCmdTbl_Rf_Ptr;

/*
 * @brief An external global variable g_AtCmdTbl_Pip_Ptr prototype declaration
 *
 */
extern at_command_t *g_AtCmdTbl_Pip_Ptr;

/*
 * @brief An external global variable g_AtCmdTbl_Others_Ptr prototype declaration
 *
 */
extern at_command_t *g_AtCmdTbl_Others_Ptr;

extern T_CmdTblLst g_tAtCmdExtTblLst;
extern T_CmdTblLst g_tAtCmdAppTbl;


int at_cmd_handler_patch(char *buf, int len, int mode)
{
    data_process_handler_patch(buf, len);
    return true;
}

void at_cmd_func_init_patch(void)
{
    at_cmd_handler = at_cmd_handler_patch;
    return;
}

