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
 * @file at_cmd_rf.c
 * @author Michael Liao
 * @date 14 Dec 2017
 * @brief File supports the RF module AT Commands.
 *
 */

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
#include "at_cmd_rf.h"
#include "at_cmd.h"
#include "at_cmd_common.h"
#include "controller_wifi.h"

volatile uint8_t g_u8RfCmdRun = 0;
T_RfCmd g_tRfCmd = {0};

void rf_cmd_param_free(void)
{
    int i = 0;

    for(i = 1; i < g_tRfCmd.iArgc; i++)
    {
        if(g_tRfCmd.saArgv[i] != NULL)
        {
            free(g_tRfCmd.saArgv[i]);
            g_tRfCmd.saArgv[i] = NULL;
        }
    }

    g_tRfCmd.iArgc = 0;
    g_u8RfCmdRun = 0;
    return;
}

int rf_cmd_param_alloc(int iArgc, char *saArgv[])
{
    int iRet = -1;
    int i = 0;

    g_tRfCmd.iArgc = iArgc;

    for(i = 1; i < g_tRfCmd.iArgc; i++)
    {
        if(saArgv[i] == NULL)
        {
            goto done;
        }

        g_tRfCmd.saArgv[i] = (char *)malloc(strlen(saArgv[i]) + 1);

        if(g_tRfCmd.saArgv[i] == NULL)
        {
            goto done;
        }

        strcpy(g_tRfCmd.saArgv[i], saArgv[i]);
    }

    iRet = 0;

done:
    if(iRet)
    {
        rf_cmd_param_free();
    }

    return iRet;
}

/*
 * @brief Command at+mode
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_rf_mode(char *buf, int len, int mode)
{
    /** at+mode=3 | 4 --> WIFI | BLE */
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

    if((argc < 2) || (argc > RF_CMD_PARAM_NUM))
    {
        at_output("invalid param number\r\n");
        goto done;
    }

    if(rf_cmd_param_alloc(argc, argv))
    {
        goto done;
    }

    g_tRfCmd.u32Type = RF_EVT_MODE;
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

/*
 * @brief Command at+go
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_rf_go(char *buf, int len, int mode)
{
    /** at+go=1,30,40,1 */
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

    if((argc < 5) || (argc > RF_CMD_PARAM_NUM))
    {
        at_output("invalid param number\r\n");
        goto done;
    }

    if(rf_cmd_param_alloc(argc, argv))
    {
        goto done;
    }

    g_tRfCmd.u32Type = RF_EVT_GO;
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

/*
 * @brief Command at+channel
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_rf_channel(char *buf, int len, int mode)
{
    /** at+channel=7 */
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

    if((argc < 2) || (argc > RF_CMD_PARAM_NUM))
    {
        at_output("invalid param number\r\n");
        goto done;
    }

    if(rf_cmd_param_alloc(argc, argv))
    {
        goto done;
    }

    g_tRfCmd.u32Type = RF_EVT_CHANNEL;
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

/*
 * @brief Command at+at_cmd_rf_resetcnts
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_rf_resetcnts(char *buf, int len, int mode)
{
    /** at+reset_cnts */
    int iRet = 0;

    if(g_u8RfCmdRun)
    {
        at_output("RF cmd is running\r\nERROR\r\n");
        goto ignore;
    }

    g_tRfCmd.u32Type = RF_EVT_RX_RESET_CNTS;
    g_u8RfCmdRun = 1;
    IPC_CMD_SEND(RF_CMD_EVT, (void*)&g_tRfCmd, sizeof(g_tRfCmd));

    iRet = 1;

ignore:
    return iRet;
}

/*
 * @brief Command at+counters?
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_rf_counters(char *buf, int len, int mode)
{
    /** at+counters */
    int iRet = 0;

    if(g_u8RfCmdRun)
    {
        at_output("RF cmd is running\r\nERROR\r\n");
        goto ignore;
    }

    g_tRfCmd.u32Type = RF_EVT_RX_CNTS;
    g_u8RfCmdRun = 1;
    IPC_CMD_SEND(RF_CMD_EVT, (void*)&g_tRfCmd, sizeof(g_tRfCmd));

    iRet = 1;

ignore:
    return iRet;
}

/*
 * @brief Command at+tx
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_rf_tx(char *buf, int len, int mode)
{
    /** at+tx=1 | 0 */
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

    if((argc < 2) || (argc > RF_CMD_PARAM_NUM))
    {
        at_output("invalid param number\r\n");
        goto done;
    }

    if(rf_cmd_param_alloc(argc, argv))
    {
        goto done;
    }

    g_tRfCmd.u32Type = RF_EVT_WIFI_TX;
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

/*
 * @brief Command at+rx
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_rf_rx(char *buf, int len, int mode)
{
    /** at+rx=1 | 0 */
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

    if((argc < 2) || (argc > RF_CMD_PARAM_NUM))
    {
        at_output("invalid param number\r\n");
        goto done;
    }

    if(rf_cmd_param_alloc(argc, argv))
    {
        goto done;
    }

    g_tRfCmd.u32Type = RF_EVT_WIFI_RX;
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

int at_cmd_rf_show_sca(char *buf, int len, int mode)
{
    /** at+showsca=2400 */
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

    if(rf_cmd_param_alloc(argc, argv))
    {
        goto done;
    }

    g_tRfCmd.u32Type = RF_EVT_SHOW_SCA;
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

int at_cmd_rf_set_sca(char *buf, int len, int mode)
{
    /** at+setsca=2400 */
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

    if((argc < 4) || (argc > RF_CMD_PARAM_NUM))
    {
        at_output("invalid param number\r\n");
        goto done;
    }

    if(rf_cmd_param_alloc(argc, argv))
    {
        goto done;
    }

    g_tRfCmd.u32Type = RF_EVT_SET_SCA;
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

int at_cmd_rf_cal_vco(char *buf, int len, int mode)
{
    /** at+vco=2400 */
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

    if(rf_cmd_param_alloc(argc, argv))
    {
        goto done;
    }

    g_tRfCmd.u32Type = RF_EVT_CAL_VCO;
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

int at_cmd_rf_test_mode(char *buf, int len, int mode)
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

    if((argc < 2) || (argc > RF_CMD_PARAM_NUM))
    {
        at_output("invalid param number\r\n");
        goto done;
    }

    if(rf_cmd_param_alloc(argc, argv))
    {
        goto done;
    }

    g_tRfCmd.u32Type = RF_EVT_TEST_MODE;
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

int at_cmd_sys_phy_dump(char *buf, int len, int mode)
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

    if(argc < 2)
    {
        at_output("invalid param number\r\n");
        goto done;
    }

    if(rf_cmd_param_alloc(argc, argv))
    {
        goto done;
    }

    g_tRfCmd.u32Type = RF_EVT_PHY_DUMP;
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

int at_cmd_le_dbg_dtm(char *buf, int len, int mode)
{
    /** at+dtm= */
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

    if(rf_cmd_param_alloc(argc, argv))
    {
        goto done;
    }

    g_tRfCmd.u32Type = RF_EVT_BLE_DTM;
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

int at_cmd_le_dbg_adv(char *buf, int len, int mode)
{
    /** at+adv= */
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

    if(rf_cmd_param_alloc(argc, argv))
    {
        goto done;
    }

    g_tRfCmd.u32Type = RF_EVT_BLE_ADV;
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

int at_cmd_rf_reload_ow(char *buf, int len, int mode)
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

    if(argc < 1)
    {
        at_output("invalid param number\r\n");
        goto done;
    }

    if(rf_cmd_param_alloc(argc, argv))
    {
        goto done;
    }

    g_tRfCmd.u32Type = RF_EVT_RELOAD_OW;
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

int at_cmd_rf_store_ow(char *buf, int len, int mode)
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

    if(argc < 1)
    {
        at_output("invalid param number\r\n");
        goto done;
    }

    if(rf_cmd_param_alloc(argc, argv))
    {
        goto done;
    }

    g_tRfCmd.u32Type = RF_EVT_STORE_OW;
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

int at_cmd_rf_show_ow(char *buf, int len, int mode)
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

    if(argc < 2)
    {
        at_output("invalid param number\r\n");
        goto done;
    }

    if(rf_cmd_param_alloc(argc, argv))
    {
        goto done;
    }

    g_tRfCmd.u32Type = RF_EVT_SHOW_OW;
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

int at_cmd_rf_add_ow(char *buf, int len, int mode)
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

    if(argc < 4)
    {
        at_output("invalid param number\r\n");
        goto done;
    }

    if(rf_cmd_param_alloc(argc, argv))
    {
        goto done;
    }

    g_tRfCmd.u32Type = RF_EVT_ADD_OW;
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

int at_cmd_rf_del_ow(char *buf, int len, int mode)
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

    if(argc < 3)
    {
        at_output("invalid param number\r\n");
        goto done;
    }

    if(rf_cmd_param_alloc(argc, argv))
    {
        goto done;
    }

    g_tRfCmd.u32Type = RF_EVT_DEL_OW;
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

/**
  * @brief AT Command Table for RF Module
  *
  */
at_command_t gAtCmdTbl_Rf[] =
{
    { "at+mode",                at_cmd_rf_mode,           "Switch standard mode, debug mode" },
    { "at+go",                  at_cmd_rf_go,             "Setup Wi-Fi preamble, data length, interval and data rate" },
    { "at+channel",             at_cmd_rf_channel,        "Setup Wi-Fi channel number" },
    { "at+reset_cnts",          at_cmd_rf_resetcnts,      "Reset Wi-Fi rx receive pkt counter" },
    { "at+counters",            at_cmd_rf_counters,       "read pkt continues" },
    { "at+tx",                  at_cmd_rf_tx,             "Enable/Disable continue tx" },
    { "at+rx",                  at_cmd_rf_rx,             "Enable/Disable continue rx" },
    { "at+showsca",             at_cmd_rf_show_sca,       "List SCA table. No Param. for all or given freq"},
    { "at+setsca",              at_cmd_rf_set_sca,        "Overwrite SCA value with freq and mode"},
    { "at+vco",                 at_cmd_rf_cal_vco,        "VCO calibration to updated SCA values of given freq"},
    { "at+rftm",                at_cmd_rf_test_mode,      "Set RF test mode"},
    { "at+phydump",             at_cmd_sys_phy_dump,      "PHY dump mode (for RD debug)"},
    // BLE
    { "at+dtm",                 at_cmd_le_dbg_dtm,        "BLE direct test mode operation"},
    { "at+adv",                 at_cmd_le_dbg_adv,        "BLE advertising test"},

    // Overwrite
    { "at+reloadow",            at_cmd_rf_reload_ow,      "Read setting from flash"},
    { "at+storeow",             at_cmd_rf_store_ow,       "Store setting to flash"},
    { "at+showow",              at_cmd_rf_show_ow,        "Display overwrite table"},
    { "at+addow",               at_cmd_rf_add_ow,         "Add entry to overwrite table"},
    { "at+delow",               at_cmd_rf_del_ow,         "Delete entry from overwrite table"},

    { NULL,                     NULL,                     NULL},
};

/*
 * @brief Global variable g_AtCmdTbl_Rf_Ptr retention attribute segment
 *
 */
RET_DATA at_command_t *g_AtCmdTbl_Rf_Ptr;

/*
 * @brief AT Command Interface Initialization for RF modules
 *
 */
void at_cmd_rf_func_init(void)
{
    /** Command Table (RF) */
    g_AtCmdTbl_Rf_Ptr = gAtCmdTbl_Rf;
}

