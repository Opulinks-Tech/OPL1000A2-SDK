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
 * @file at_cmd.c
 * @author Michael Liao
 * @date 14 Dec 2017
 * @brief File creates the major architecture of AT Command parser.
 *
 */
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

/*
 * @brief An external Function at_cmd_handler prototype declaration retention attribute segment
 *
 */
RET_DATA at_cmd_handler_fp_t at_cmd_handler;

/*
 * @brief An external Function at_cmd_parse prototype declaration retention attribute segment
 *
 */
RET_DATA at_cmd_parse_fp_t at_cmd_parse;

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
RET_DATA at_cmd_ext_tbl_reg_fp_t at_cmd_ext_tbl_reg;

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

RET_DATA T_CmdTblLst g_tAtCmdExtTblLst;
RET_DATA T_CmdTblLst g_tAtCmdAppTbl;

int at_cmd_parse_impl(char *pbuf)
{
    at_cmd_handler(pbuf, (int)strlen(pbuf), NULL);
    return true;
}
/*
int at_cmd_extend_impl(char *buf, int len, int mode)
{
    return true;
}
*/
int at_cmd_handler_impl(char *buf, int len, int mode)
{
    data_process_handler(buf, len);
    return true;
}

int at_cmd_ext_tbl_reg_impl(void *ptr)
{
    int iRet = 1;
    T_CmdTblLst *ptLst = &g_tAtCmdExtTblLst;

    if(ptLst->taCmdTbl)
    {
        #if 1
        // only two elements in table list: one for sdk and the other for app
        if(ptLst->ptNext)
        {
            iRet = 0;
            goto done;
        }

        ptLst->ptNext = &g_tAtCmdAppTbl;
        #else
        // more elements in table list: but malloc will occupy more memory (64 to 2048)
        while(ptLst->ptNext)
        {
            ptLst = ptLst->ptNext;
        }

        ptLst->ptNext = (T_CmdTblLst *)malloc(sizeof(T_CmdTblLst));

        if(ptLst->ptNext == NULL)
        {
            iRet = 0;
            goto done;
        }
        #endif

        ptLst = ptLst->ptNext;
    }

    ptLst->taCmdTbl = (at_command_t *)ptr;
    ptLst->ptNext = NULL;

done:
    return iRet;
}

/*
 * @brief AT Command Interface Initialization for AT Command module
 *
 */
void at_cmd_func_init(void)
{
    /** Command Tables */
    at_cmd_sys_func_init();
    at_cmd_wifi_func_init();
    //at_cmd_ble_func_init();
    at_cmd_tcpip_func_init();
    at_cmd_rf_func_init();
    at_cmd_pip_func_init();
    at_cmd_property_func_init();
    at_cmd_others_func_init();
    at_cmd_nvm_func_init();

    /** Functions */
    at_cmd_parse     = at_cmd_parse_impl;
    at_cmd_handler   = at_cmd_handler_impl;
    //at_cmd_extend    = at_cmd_extend_impl;
    at_cmd_ext_tbl_reg = at_cmd_ext_tbl_reg_impl;

    memset(&g_tAtCmdExtTblLst, 0, sizeof(g_tAtCmdExtTblLst));
    memset(&g_tAtCmdAppTbl, 0, sizeof(g_tAtCmdAppTbl));
}

