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
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "os.h"
#include "at_cmd.h"
#include "at_cmd_rf.h"
#include "ipc.h"
#include "data_flow.h"
#include "controller_wifi.h"
#include "at_cmd_common.h"
#include "at_cmd_rf_patch.h"
#include "at_cmd.h"
#include "at_cmd_common.h"
#include "controller_wifi.h"


extern void rf_cmd_param_free(void);
extern int rf_cmd_param_alloc(int iArgc, char *saArgv[]);

/*
 * @brief Command at+come
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_rf_come(char *buf, int len, int mode)
{
    int iRet = 0;
    char *argv[AT_MAX_CMD_ARGS] = {0};
    int argc = 0;

    if(g_u8RfCmdRun)
    {
        at_output("RF cmd is running\r\nERROR\r\n");
        goto ignore;
    }

    if(!at_cmd_buf_to_argc_argv(buf, &argc, argv, AT_MAX_CMD_ARGS))
    {
        goto done;
    }

    if((argc < 1) || (argc > RF_CMD_PARAM_NUM))
    {
        at_output("invalid param number\r\n");
        goto done;
    }

    if (argc > 1)
    {
        if(rf_cmd_param_alloc(argc, argv))
        {
            goto done;
        }
    }
    else
    {
        g_tRfCmd.iArgc = 1;
    }

    g_tRfCmd.u32Type = RF_EVT_COME;
    g_u8RfCmdRun = 1;
    IPC_CMD_SEND(RF_CMD_EVT, (void*)&g_tRfCmd, sizeof(g_tRfCmd));

    iRet = 1;

done:
    if(!iRet)
    {
        rf_cmd_param_free();

        at_output("ERROR\r\n");
    }

ignore:
    return iRet;
}

int at_cmd_rf_dcoc(char *buf, int len, int mode)
{
    int iRet = 0;
    char *argv[AT_MAX_CMD_ARGS] = {0};
    int argc = 0;

    if(g_u8RfCmdRun)
    {
        at_output("RF cmd is running\r\nERROR\r\n");
        goto ignore;
    }

    if(!at_cmd_buf_to_argc_argv(buf, &argc, argv, AT_MAX_CMD_ARGS))
    {
        goto done;
    }

    if(rf_cmd_param_alloc(argc, argv))
    {
        goto done;
    }

    g_tRfCmd.u32Type = RF_EVT_DCOC;
    g_u8RfCmdRun = 1;
    IPC_CMD_SEND(RF_CMD_EVT, (void*)&g_tRfCmd, sizeof(g_tRfCmd));

    iRet = 1;

done:
    if(!iRet)
    {
        rf_cmd_param_free();

        at_output("ERROR\r\n");
    }

ignore:
    return iRet;
}

int at_cmd_rf_dcth(char *buf, int len, int mode)
{
    int iRet = 0;
    char *argv[AT_MAX_CMD_ARGS] = {0};
    int argc = 0;

    if(g_u8RfCmdRun)
    {
        at_output("RF cmd is running\r\nERROR\r\n");
        goto ignore;
    }

    if(!at_cmd_buf_to_argc_argv(buf, &argc, argv, AT_MAX_CMD_ARGS))
    {
        goto done;
    }

    if(rf_cmd_param_alloc(argc, argv))
    {
        goto done;
    }

    g_tRfCmd.u32Type = RF_EVT_DCTH;
    g_u8RfCmdRun = 1;
    IPC_CMD_SEND(RF_CMD_EVT, (void*)&g_tRfCmd, sizeof(g_tRfCmd));

    iRet = 1;

done:
    if(!iRet)
    {
        rf_cmd_param_free();

        at_output("ERROR\r\n");
    }

ignore:
    return iRet;
}
