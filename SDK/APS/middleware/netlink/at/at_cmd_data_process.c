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
 * @file at_cmd_data_process.c
 * @author Michael Liao
 * @date 21 Mar 2018
 * @brief File contains the AT Command's data processing behavior.
 *
 */

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
#include "le_cmd_app_cmd.h"
#include "le_host_test_cmd.h"
#include "at_cmd_ble.h"
#include "at_cmd_task.h"

RET_DATA int g_at_lock;
RET_DATA int g_at_ble_data_len;
RET_DATA int g_at_tcpip_data_len;

extern uint8_t at_state;

extern at_command_t *g_AtCmdTbl_Wifi_Ptr;
extern at_command_t *g_AtCmdTbl_Tcpip_Ptr;
extern at_command_t *g_AtCmdTbl_Sys_Ptr;
extern at_command_t *g_AtCmdTbl_Rf_Ptr;
extern at_command_t *g_AtCmdTbl_property_Ptr;

#define AT_CMD_LINE_TERMINATOR  "\r\n"

uint8_t cmd_info_buf[AT_RBUF_SIZE];

typedef struct {
    uint8_t *cmd;
    uint8_t  cmd_len;
    uint8_t  op;
    uint8_t  para_num;
    uint8_t *para[AT_MAX_CMD_ARGS];
} at_cmd_information_t;

static at_cmd_information_t at_cmd_info;

void data_process_init(void)
{
    g_at_lock = LOCK_NONE;
    g_at_ble_data_len = 0;
    g_at_tcpip_data_len = 0;
    at_state = AT_STATE_IDLE;
}

void data_process_lock(int module, int data_len)
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

        case LOCK_NONE:
            data_process_init();
            break;

        default:
            g_at_lock = LOCK_NONE;
            break;
    }
}

void data_process_unlock(void)
{
    data_process_init();
}

int data_process_lock_get(void)
{
    return g_at_lock;
}

int data_process_data_len_get(void)
{
    int len = 0;

    switch(g_at_lock)
    {
        case LOCK_BLE:
            len = g_at_ble_data_len;
            break;

        case LOCK_TCPIP:
            len = g_at_tcpip_data_len;
            break;

        default:
            break;
    }

    return len;
}

int data_process_wifi_parse(char *pbuf, int len, int mode)
{
    const at_command_t *cmd_ptr = NULL;

    if(pbuf == 0) return false;

    for(cmd_ptr=g_AtCmdTbl_Wifi_Ptr; cmd_ptr->cmd; cmd_ptr++)
    {
        if(strcasecmp((char*)at_cmd_info.cmd, cmd_ptr->cmd) == 0)
        {
            msg_print_uart1("\r\n");
            cmd_ptr->cmd_handle(pbuf, len, mode);
            return true;
        }
    }

	return false;
}

int data_process_wifi(char *pbuf, int len, int mode)
{
    int ret = false;
    ret = data_process_wifi_parse(pbuf, len, mode);
    return ret;
}

int data_process_ble(char *pbuf, int len, int mode)
{
	int ret;
    //1. Check ble table, if it's ble command, return true; else return false;
    if (strncasecmp(pbuf, "at+blemode", strlen("at+blemode"))==0)
    {
        msg_print_uart1("\r\nOK\r\n");
        at_cmd_ble_mode(pbuf, len, mode);
        return true;
    }
    else if (strncasecmp(pbuf, "at+letest", strlen("at+letest"))==0)
    {
        msg_print_uart1("\r\nOK\r\n");
        at_cmd_letest(pbuf, len, mode);
        return true;
    }
    else if (strncasecmp(pbuf, "at+mpbleaddr", strlen("at+mpbleaddr"))==0)
    {
        msg_print_uart1("\r\nOK\r\n");
        at_cmd_mp_ble_addr(pbuf, len, mode);
        return true;
    }
    //2. Find the specified command handler, do it

    //Be noticed, as to BLE case, the pbuf is transfered to BLE protocol stack, and the command will be handled at that time in BLE statck

	ret = LeCmdAppProcessAtCmd(pbuf, len);

	if (!ret)
	{
		ret = LeHostProcessHostTestCmd(pbuf, len);
    }

	return ret;

    //return false;
}

int data_process_tcpip(char *pbuf, int len, int mode)
{
    const at_command_t *cmd_ptr = NULL;

    if(pbuf == 0) return false;

    for(cmd_ptr=g_AtCmdTbl_Tcpip_Ptr; cmd_ptr->cmd; cmd_ptr++)
    {
        if(strcasecmp((char*)at_cmd_info.cmd, cmd_ptr->cmd) == 0)
        {
            msg_print_uart1("\r\n");
            cmd_ptr->cmd_handle(pbuf, len, mode);
            return true;
        }
    }

	return false;
}

int data_process_sys(char *pbuf, int len, int mode)
{
    const at_command_t *cmd_ptr = NULL;

    if(pbuf == 0) return false;

    for(cmd_ptr=g_AtCmdTbl_Sys_Ptr; cmd_ptr->cmd; cmd_ptr++)
    {
        if(strcasecmp((char*)at_cmd_info.cmd, cmd_ptr->cmd) == 0)
        {
            msg_print_uart1("\r\n");
            cmd_ptr->cmd_handle(pbuf, len, mode);
            return true;
        }
    }

    return false;
}

int data_process_rf(char *pbuf, int len, int mode)
{
    const at_command_t *cmd_ptr = NULL;

    if(pbuf == 0) return false;

    for(cmd_ptr=g_AtCmdTbl_Rf_Ptr; cmd_ptr->cmd; cmd_ptr++)
    {
        if(strcasecmp((char*)at_cmd_info.cmd, cmd_ptr->cmd) == 0)
        {
            msg_print_uart1("\r\n");
            cmd_ptr->cmd_handle(pbuf, len, mode);
            return true;
        }
    }

    return false;
}

int data_process_pip(char *pbuf, int len, int mode)
{
    //1. Check PIP table, if it's ble command, return true; else return false;

    //2. Find the specified command handler, do it

    return false;
}

int data_process_property(char *pbuf, int len, int mode)
{
    const at_command_t *cmd_ptr = NULL;

    if(pbuf == 0) return false;

    for(cmd_ptr=g_AtCmdTbl_property_Ptr; cmd_ptr->cmd; cmd_ptr++)
    {
        if(strcasecmp((char*)at_cmd_info.cmd, cmd_ptr->cmd) == 0)
        {
            msg_print_uart1("\r\n");
            cmd_ptr->cmd_handle(pbuf, len, mode);
            return true;
        }
    }

	return false;
}

int data_process_others(char *pbuf, int len, int mode)
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
                msg_print_uart1("\r\n");
                cmd_ptr->cmd_handle(pbuf, len, mode);
                iRet = 1;
                goto done;
            }
        }
    }
    
done:
    return iRet;
}

int data_process_cmd_mode(char *pbuf)
{
    char *s = NULL;

    s = strstr(pbuf, PREFIX_AT_CMD_TEST);
    if(s != NULL)
    {
        return AT_CMD_MODE_TESTING;
    }

    s = strstr(pbuf, PREFIX_ATCMD_SET);
    if(s != NULL)
    {
        return AT_CMD_MODE_SET;
    }

    s = strstr(pbuf, PREFIX_AT_CMD_READ);
    if(s != NULL)
    {
        return AT_CMD_MODE_READ;
    }

    return AT_CMD_MODE_EXECUTION;
}

bool at_cmd_info_parsing(uint8_t *pStr, at_cmd_information_t *at_info_ptr)
{
    // first two letter must be "AT" or "at" or "At" or "aT"
    if ((at_info_ptr == NULL) || (pStr == NULL) || (at_strlen((char *)pStr) < 2)) {
        return FALSE;
    }
    at_memset(at_info_ptr, 0x00, sizeof(at_cmd_information_t));
    at_info_ptr->op = AT_CMD_MODE_INVALID;

    if (((*pStr != 'A') && (*pStr != 'a')) || ((*(pStr + 1) != 'T') && (*(pStr + 1) != 't'))) {
        at_uart1_printf("AT CMD NOT FOUND AT\r\n");
        return FALSE;
    }
    at_info_ptr->cmd = pStr;
    at_info_ptr->cmd_len = 2;
    pStr += 2;

    if (*pStr == '\0') {
        // just AT
        at_info_ptr->para_num = 0;
        at_info_ptr->op = AT_CMD_MODE_EXECUTION;
        return TRUE;
    }

    // get cmd
    if (*pStr == '+') {
        pStr++;
        at_info_ptr->cmd_len++;
    }

    while (*pStr != '\0') {
        if ((*pStr >= 'A') && (*pStr <= 'Z')) { // OK
            pStr++;
        } else if ((*pStr >= 'a') && (*pStr <= 'z')) { // upper letter
            *pStr = (*pStr) - 'a' + 'A';
            pStr++;
        } else if ((*pStr >= '0') && (*pStr <= '9')) {
            pStr++;
        } else if ((*pStr == '!') || (*pStr == '%') || (*pStr == '-') || (*pStr == '.')
                   || (*pStr == '/') || (*pStr == ':') || (*pStr == '_')) {
            pStr++;
        } else {
            break;
        }
        at_info_ptr->cmd_len++;
    }
    // get operator
    if (*pStr == '?') {
        *pStr++ = '\0';
        at_info_ptr->op = AT_CMD_MODE_READ;
        goto PARSE_END;
    } else if (*pStr == '=') {
        *pStr++ = '\0';
        if (*pStr == '?') {
            *pStr++ = '\0';
            at_info_ptr->op = AT_CMD_MODE_TESTING;
            goto PARSE_END;
        }
        at_info_ptr->op = AT_CMD_MODE_SET;
    } else {
        goto PARSE_END;
    }
    // get parameter,like AT+CMD='abc'
    at_info_ptr->para_num = 0;
    at_info_ptr->para[at_info_ptr->para_num++] = pStr;
    while (*pStr != '\0') {
        if (*pStr == '\\') {
            pStr += 2;
            continue;
        }

        if (*pStr == ',') {
            *pStr++ = '\0';
            at_info_ptr->para[at_info_ptr->para_num++] = pStr;
            continue;
        }
        pStr++;
    }

PARSE_END:
    if (*pStr == '\0') {
        if (at_info_ptr->op == AT_CMD_MODE_INVALID) {
            at_info_ptr->op = AT_CMD_MODE_EXECUTION;
            *pStr = '\0';
        } else if (at_info_ptr->op == AT_CMD_MODE_SET) {
            *pStr = '\0';
        }
        return TRUE;
    } else {
        at_uart1_printf("\r\nAT CMD ERROR\r\n");
    }

    return FALSE;
}

int data_process_handler(char *pbuf, int len)
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
        }
        else if (strstr(pbuf, "ATE1") != NULL)
        {
            set_echo_on(true);
            msg_print_uart1("\r\nOK\r\n");
        }
    }

    if (g_at_lock == LOCK_NONE) //AT command input
    {
        if (data_process_wifi(pbuf, len, mode))
            return true;
        if (data_process_ble(pbuf, len, mode))
            return true;
        if (data_process_tcpip(pbuf, len, mode))
            return true;
        if (data_process_sys(pbuf, len, mode))
            return true;
        if (data_process_rf(pbuf, len, mode))
            return true;
        if (data_process_pip(pbuf, len, mode))
            return true;
        if (data_process_property(pbuf, len, mode))
            return true;
        if (data_process_others(pbuf, len, mode))
            return true;

        at_response_result(AT_RESULT_CODE_ERROR);
    }

    return true;
}

void data_process_func_init(void)
{
    g_at_lock = LOCK_NONE;
    g_at_ble_data_len = 0;
    g_at_tcpip_data_len = 0;
}

