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
#include <stdlib.h>
#include "cmsis_os.h"
#include "at_cmd.h"
#include "at_cmd_sys.h"
#include "at_cmd_common.h"
#include "at_cmd_data_process.h"

#include "at_cmd_ext_patch.h"
#include "at_cmd_task_patch.h"
#include "le_ctrl_patch.h"


int at_cmd_ext_crlf_term(char *buf, int len, int mode)
{
    int iRet = 0;
    int argc = 0;
    char *argv[AT_MAX_CMD_ARGS] = {0};
    
    if(!at_cmd_buf_to_argc_argv(buf, &argc, argv, AT_MAX_CMD_ARGS))
    {
        goto done;
    }
    
    switch(mode)
    {
        case AT_CMD_MODE_READ:
            msg_print_uart1("AT_CRLF_TERM: %u\r\n", at_cmd_crlf_term_get());
            break;

        case AT_CMD_MODE_SET:
        {
            uint8_t u8Value = 0;

            if(argc < 2)
            {
                goto done;
            }

            u8Value = (uint8_t)strtoul(argv[1], NULL, 10);
            at_cmd_crlf_term_set((u8Value)?(1):(0));
            break;
        }

        default:
            goto done;
    }

    iRet = 1;

done:
    if(iRet)
    {
        msg_print_uart1("OK\r\n");
    }
    else
    {
        msg_print_uart1("ERROR\r\n");
    }
    
    return iRet;
}

int at_cmd_ext_le_info(char *buf, int len, int mode)
{
    int iRet = 0;
    int argc = 0;
    char *argv[AT_MAX_CMD_ARGS] = {0};

    at_cmd_buf_to_argc_argv(buf, &argc, argv, AT_MAX_CMD_ARGS);

    if (mode == AT_CMD_MODE_SET)
    {
        if (argc >= 2)
        {
            int interval = 0;

            if (argc == 3)
                interval = atoi(argv[2]);

            if (!le_ctrl_packet_info_display(atoi(argv[1]), interval))
                iRet = 1;
        }
    }

    if(iRet)
        msg_print_uart1("OK\r\n");
    else
        msg_print_uart1("ERROR\r\n");

    return iRet;
}

at_command_t gAtCmdTbl_Ext[] =
{
    { "at+crlfterm",            at_cmd_ext_crlf_term,     "Enable/disable CRLF termination"},
    { "at+leinfo",              at_cmd_ext_le_info,       "Dump BLE packet info statistics"},
    { NULL,                     NULL,                     NULL},
};

