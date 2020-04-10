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
#include <stdarg.h>
#include "opulinks.h"
#include "os.h"
#include "at_cmd_data_process.h"
#include "at_cmd.h"
#include "at_cmd_common.h"
//#include "le_cmd_app_cmd.h"
//#include "le_host_test_cmd.h"
#include "at_cmd_ble.h"
#include "at_cmd_task.h"

#include "at_cmd_data_process_patch.h"
#include "at_cmd_task_patch.h"
#include "at_cmd_ble_patch.h"

extern int g_at_lock;
extern int g_at_ble_data_len;
extern int g_at_tcpip_data_len;

extern uint8_t at_state;

extern at_command_t *g_AtCmdTbl_Wifi_Ptr;
extern at_command_t *g_AtCmdTbl_Tcpip_Ptr;
extern at_command_t *g_AtCmdTbl_Sys_Ptr;
extern at_command_t *g_AtCmdTbl_Rf_Ptr;
extern at_command_t *g_AtCmdTbl_property_Ptr;

#define AT_CMD_LINE_TERMINATOR  "\r\n"

extern uint8_t cmd_info_buf[AT_RBUF_SIZE];

typedef struct {
    uint8_t *cmd;
    uint8_t  cmd_len;
    uint8_t  op;
    uint8_t  para_num;
    uint8_t *para[AT_MAX_CMD_ARGS];
} at_cmd_information_t;

extern at_cmd_information_t at_cmd_info;

uint32_t g_u32AtAppDataMax = 0;
uint32_t g_u32AtAppDataCurr = 0;
uint32_t g_u32AtAppDataOffset = 0;

extern bool at_cmd_info_parsing(uint8_t *pStr, at_cmd_information_t *at_info_ptr);
extern int data_process_property(char *pbuf, int len, int mode);


void data_process_lock_patch(int module, int data_len)
{
    switch(module)
    {
        case LOCK_BLE:
            g_at_lock = LOCK_BLE;
            g_at_ble_data_len = data_len;
            at_state = AT_STATE_SENDING_RECV;
            break;

        case LOCK_TCPIP:
            g_at_lock = LOCK_TCPIP;
            
            if (data_len == AT_TCP_TRANS_LOCK_ID) {
                g_at_tcpip_data_len = 0;
                at_state = AT_STATE_TRANSMIT;
            }
            else
            {
                g_at_tcpip_data_len = data_len;
                at_state = AT_STATE_SENDING_RECV;
            }
            break;

        case LOCK_OTHERS:
        case LOCK_APP:
            g_at_lock = module;
            g_u32AtAppDataMax = data_len;
            g_u32AtAppDataCurr = 0;
            g_u32AtAppDataOffset = 0;
            at_state = AT_STATE_SENDING_RECV;
            break;

        case LOCK_NONE:
            data_process_init();
            break;

        default:
            g_at_lock = LOCK_NONE;
            break;
    }
}

int data_process_wifi_patch(char *pbuf, int len, int mode)
{
    const at_command_t *cmd_ptr = NULL;

    if(pbuf == 0) return false;

    for(cmd_ptr=g_AtCmdTbl_Wifi_Ptr; cmd_ptr->cmd; cmd_ptr++)
    {
        if(strcasecmp((char*)at_cmd_info.cmd, cmd_ptr->cmd) == 0)
        {
            //msg_print_uart1("\r\n"); // it should be handled by "at_echo_on" flag in "uart1_rx_int_do_at()"
            cmd_ptr->cmd_handle(pbuf, len, mode);
            return true;
        }
    }

	return false;
}

int data_process_ble_patch(char *pbuf, int len, int mode)
{
    int ret = false;
    
    if (strncasecmp(pbuf, "at+mpbleaddr", strlen("at+mpbleaddr"))==0)
    {
        ret = at_cmd_mp_ble_addr_patch(pbuf, len, mode);
        return ret;
    }
    else
    {
        extern int data_process_ble(char *pbuf, int len, int mode);
        return data_process_ble(pbuf, len, mode);
    }
}

int data_process_tcpip_patch(char *pbuf, int len, int mode)
{
    const at_command_t *cmd_ptr = NULL;

    if(pbuf == 0) return false;

    for(cmd_ptr=g_AtCmdTbl_Tcpip_Ptr; cmd_ptr->cmd; cmd_ptr++)
    {
        if(strcasecmp((char*)at_cmd_info.cmd, cmd_ptr->cmd) == 0)
        {
            //msg_print_uart1("\r\n"); // it should be handled by "at_echo_on" flag in "uart1_rx_int_do_at()"
            cmd_ptr->cmd_handle(pbuf, len, mode);
            return true;
        }
    }

	return false;
}

int data_process_sys_patch(char *pbuf, int len, int mode)
{
    const at_command_t *cmd_ptr = NULL;

    if(pbuf == 0) return false;

    for(cmd_ptr=g_AtCmdTbl_Sys_Ptr; cmd_ptr->cmd; cmd_ptr++)
    {
        if(strcasecmp((char*)at_cmd_info.cmd, cmd_ptr->cmd) == 0)
        {
            //msg_print_uart1("\r\n"); // it should be handled by "at_echo_on" flag in "uart1_rx_int_do_at()"
            cmd_ptr->cmd_handle(pbuf, len, mode);
            return true;
        }
    }

    return false;
}

int data_process_rf_patch(char *pbuf, int len, int mode)
{
    const at_command_t *cmd_ptr = NULL;

    if(pbuf == 0) return false;

    for(cmd_ptr=g_AtCmdTbl_Rf_Ptr; cmd_ptr->cmd; cmd_ptr++)
    {
        if(strcasecmp((char*)at_cmd_info.cmd, cmd_ptr->cmd) == 0)
        {
            //msg_print_uart1("\r\n"); // it should be handled by "at_echo_on" flag in "uart1_rx_int_do_at()"
            cmd_ptr->cmd_handle(pbuf, len, mode);
            return true;
        }
    }

    return false;
}

int data_process_property_patch(char *pbuf, int len, int mode)
{
    const at_command_t *cmd_ptr = NULL;

    if(pbuf == 0) return false;

    for(cmd_ptr=g_AtCmdTbl_property_Ptr; cmd_ptr->cmd; cmd_ptr++)
    {
        if(strcasecmp((char*)at_cmd_info.cmd, cmd_ptr->cmd) == 0)
        {
            //msg_print_uart1("\r\n"); // it should be handled by "at_echo_on" flag in "uart1_rx_int_do_at()"
            cmd_ptr->cmd_handle(pbuf, len, mode);
            return true;
        }
    }

	return false;
}

int data_process_others_patch(char *pbuf, int len, int mode)
{
    int iRet = 0;
    T_CmdTblLst *ptLst = NULL;
    const at_command_t *cmd_ptr = NULL;

    if(pbuf == 0)
    {
        goto done;
    }

    for(ptLst = &g_tAtCmdExtTblLst; ptLst != NULL; ptLst = ptLst->ptNext)
    {
        for(cmd_ptr = ptLst->taCmdTbl; cmd_ptr->cmd; cmd_ptr++)
        {
            if(strcasecmp((char*)at_cmd_info.cmd, cmd_ptr->cmd) == 0)
            {
                //msg_print_uart1("\r\n"); // it should be handled by "at_echo_on" flag in "uart1_rx_int_do_at()"
                cmd_ptr->cmd_handle(pbuf, len, mode);
                iRet = 1;
                goto done;
            }
        }
    }
    
done:
    return iRet;
}

int data_process_handler_patch(char *pbuf, int len)
{
    int mode = AT_CMD_MODE_INVALID;

    if (pbuf == NULL) return false;
    mode = data_process_cmd_mode(pbuf);

    memset(cmd_info_buf, 0, AT_RBUF_SIZE);
    memcpy(cmd_info_buf, pbuf, AT_RBUF_SIZE);

    at_cmd_info_parsing(cmd_info_buf, &at_cmd_info);

    if (len == 2) //Command: AT
    {
        msg_print_uart1("\r\nOK\r\n");
        return true;
    }

    if (mode == AT_CMD_MODE_EXECUTION)
    {
        if (strstr(pbuf, "ATE0") != NULL)
        {
            set_echo_on(false);
            msg_print_uart1("\r\nOK\r\n");
            return true;
        }
        else if (strstr(pbuf, "ATE1") != NULL)
        {
            set_echo_on(true);
            msg_print_uart1("\r\nOK\r\n");
            return true;
        }
    }

    if (g_at_lock == LOCK_NONE) //AT command input
    {
        if (data_process_wifi_patch(pbuf, len, mode))
            return true;

        if (data_process_ble_patch(pbuf, len, mode))
        {
            at_cmd_crlf_term_set(1); // Enable CR-LF termination for BLE AT commands
            return true;
        }

        if (data_process_tcpip_patch(pbuf, len, mode))
            return true;
        if (data_process_sys_patch(pbuf, len, mode))
            return true;
        if (data_process_rf_patch(pbuf, len, mode))
            return true;
        if (data_process_pip(pbuf, len, mode))
            return true;
        if (data_process_property_patch(pbuf, len, mode))
            return true;
        if (data_process_others_patch(pbuf, len, mode))
            return true;

        at_response_result(AT_RESULT_CODE_ERROR);
    }

    return true;
}
