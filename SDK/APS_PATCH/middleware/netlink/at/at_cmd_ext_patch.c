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
#include "at_cmd_rf_patch.h"
#include "le_ctrl_patch.h"
#include "hal_auxadc_patch.h"
#include "hal_auxadc_internal.h"
#include "hal_flash.h"


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

int at_cmd_ext_le_gain(char *buf, int len, int mode)
{
    int iRet = 0;
    int argc = 0;
    char *argv[AT_MAX_CMD_ARGS] = {0};

    at_cmd_buf_to_argc_argv(buf, &argc, argv, AT_MAX_CMD_ARGS);

    if (mode == AT_CMD_MODE_SET)
    {
        uint8_t rf_gain_level      = le_ctrl_agc_rf_gain_level;
        uint8_t digital_gain_level = le_ctrl_agc_digital_gain_level;

        if (argc >= 2)
            rf_gain_level = atoi(argv[1]);

        if (argc >= 3)
            digital_gain_level = atoi(argv[2]);

        iRet = !le_ctrl_set_agc_gain_level(rf_gain_level, digital_gain_level);
    }
    else if (mode == AT_CMD_MODE_READ)
    {
        iRet = 1;
        msg_print_uart1("+LEGAIN:%d,%d\r\n", le_ctrl_agc_rf_gain_level, le_ctrl_agc_digital_gain_level);
    }

    if(iRet)
        msg_print_uart1("OK\r\n");
    else
        msg_print_uart1("ERROR\r\n");

    return iRet;
}

int at_cmd_ext_auxadcdbg(char *buf, int len, int mode)
{
    int iRet = 0;
    uint8_t u8WriteDirect_bak = g_ubHalAux_Pu_WriteDirect;
    uint8_t ubSrc = 0;
    uint8_t ubGpioIdx = 0;
    uint32_t u32Res = 0;
    uint32_t u32Temp = 0;

    int argc = 0;
    char *argv[AT_MAX_CMD_ARGS] = {0};

    at_cmd_buf_to_argc_argv(buf, &argc, argv, AT_MAX_CMD_ARGS);

    if (argc >= 2)
        ubSrc = atoi(argv[1]);

    if(argc >= 3)
        ubGpioIdx = atoi(argv[2]);

    Hal_Aux_Init();
    Hal_Aux_AdcCal_Init();
    // Force-enable
    g_ubHalAux_Pu_WriteDirect = 1;
    u32Res = Hal_Aux_SourceSelect( (E_HalAux_Src_t)ubSrc, ubGpioIdx);
    if(u32Res == HAL_AUX_FAIL)
        goto done;
    u32Res = Hal_Aux_AdcValueGet( &u32Temp );
    if(u32Res == HAL_AUX_FAIL)
        goto done;

    msg_print_uart1("Ref points (mV, Data) = ");
    msg_print_uart1("(%d, 0x%X) and ", sAuxadcCalTable.stIntSrc[ 0 ].u16MiniVolt, sAuxadcCalTable.stIntSrc[ 0 ].u16RawData);
    msg_print_uart1("(%d, 0x%X)\n\r", sAuxadcCalTable.stIntSrc[ 1 ].u16MiniVolt, sAuxadcCalTable.stIntSrc[ 1 ].u16RawData);
    if(ubSrc == HAL_AUX_SRC_GPIO)
        msg_print_uart1("Auxadc(gpio = %d)", ubGpioIdx);
    else
        msg_print_uart1("Auxadc(tSrc = %s)", pAuxadcSrcName[ ubSrc ]);
    msg_print_uart1("value = 0x%04X (%f mV)\r\n", u32Temp, Hal_Aux_AdcMiniVolt_Convert(u32Temp));

    iRet = 1;
done:
    // Remove Force-enable
    g_ubHalAux_Pu_WriteDirect = u8WriteDirect_bak;
    if(iRet)
        msg_print_uart1("OK\r\n");
    else
        msg_print_uart1("ERROR\r\n");

    return iRet;
}

int at_cmd_ext_auxadc(char *buf, int len, int mode)
{
    int iRet = 0;
    uint8_t u8WriteDirect_bak = g_ubHalAux_Pu_WriteDirect;
    uint8_t ubSrc = 0;
    uint8_t ubGpioIdx = 0;
    uint32_t u32Res = 0;
    uint32_t u32Temp = 0;

    int argc = 0;
    char *argv[AT_MAX_CMD_ARGS] = {0};

    at_cmd_buf_to_argc_argv(buf, &argc, argv, AT_MAX_CMD_ARGS);

    if (argc >= 2)
        ubSrc = atoi(argv[1]);

    if(argc >= 3)
        ubGpioIdx = atoi(argv[2]);

    Hal_Aux_Init();
    Hal_Aux_AdcCal_Init();
    // Force-enable
    g_ubHalAux_Pu_WriteDirect = 1;
    u32Res = Hal_Aux_AdcConvValue_Get( (E_HalAux_Src_Patch_t)ubSrc, ubGpioIdx, &u32Temp);
    if(u32Res == HAL_AUX_FAIL)
        goto done;

    if(ubSrc == HAL_AUX_SRC_GPIO)
        msg_print_uart1("Auxadc(gpio = %d)", ubGpioIdx);
    else
        msg_print_uart1("Auxadc(tSrc = %s)", pAuxadcSrcName[ ubSrc ]);
    msg_print_uart1("value = 0x%04X\r\n", u32Temp);

    iRet = 1;
done:
    // Remove Force-enable
    g_ubHalAux_Pu_WriteDirect = u8WriteDirect_bak;
    if(iRet)
        msg_print_uart1("OK\r\n");
    else
        msg_print_uart1("ERROR\r\n");

    return iRet;
}

int at_cmd_ext_adccalvbat(char *buf, int len, int mode)
{
    int iRet = 0;
    uint16_t u16MiniVlot = 0;
    uint8_t u8PtxIdx = 1;
    uint32_t u32Res = 0;

    int argc = 0;
    char *argv[AT_MAX_CMD_ARGS] = {0};

    at_cmd_buf_to_argc_argv(buf, &argc, argv, AT_MAX_CMD_ARGS);

    if(argc >= 2)
    {
        u16MiniVlot = atoi(argv[1]);
        if(argc >= 3)
            u8PtxIdx = atoi(argv[2]);

        Hal_Aux_Init();
        Hal_Aux_AdcCal_Init();
        u32Res = Hal_Aux_AdcVbatInCal(u16MiniVlot, u8PtxIdx);
        msg_print_uart1("Cal from Vbat (%d mV, Pt_Idx = %d)\r\n", u16MiniVlot, u8PtxIdx);
        if(u32Res == HAL_AUX_FAIL)
            goto done;
    }else{
        iRet = 0;
    }

    iRet = 1;
done:
    if(iRet)
        msg_print_uart1("OK\r\n");
    else
        msg_print_uart1("ERROR\r\n");

    return iRet;
}

int at_cmd_ext_adccalgpio(char *buf, int len, int mode)
{
    int iRet = 0;
    uint8_t ubGpioIdx = 0;
    uint16_t u16MiniVlot = 0;
    uint8_t u8PtxIdx = 1;
    uint32_t u32Res = 0;

    int argc = 0;
    char *argv[AT_MAX_CMD_ARGS] = {0};

    at_cmd_buf_to_argc_argv(buf, &argc, argv, AT_MAX_CMD_ARGS);

    if(argc >= 3)
    {
        ubGpioIdx = atoi(argv[1]);
        u16MiniVlot = atoi(argv[2]);
        if(argc >= 4)
            u8PtxIdx = atoi(argv[3]);

        Hal_Aux_Init();
        Hal_Aux_AdcCal_Init();
        u32Res = Hal_Aux_AdcGpioInCal(ubGpioIdx, u16MiniVlot, u8PtxIdx);
        msg_print_uart1("Cal from GPIO_%d (%d mV, Pt_Idx = %d)\r\n", ubGpioIdx, u16MiniVlot, u8PtxIdx);
        if(u32Res == HAL_AUX_FAIL)
            goto done;
    }else{
        iRet = 0;
    }

    iRet = 1;
done:
    if(iRet == HAL_AUX_OK)
        msg_print_uart1("OK\r\n");
    else
        msg_print_uart1("ERROR\r\n");

    return iRet;
}

int at_cmd_ext_adcdef(char *buf, int len, int mode)
{
    int iRet = 0;

    iRet = Hal_Aux_AdcCal_LoadDef();

    if(iRet == HAL_AUX_OK)
        msg_print_uart1("OK\r\n");
    else
        msg_print_uart1("ERROR\r\n");

    return iRet;
}
int at_cmd_ext_adcerase(char *buf, int len, int mode)
{
    int iRet = 0;

    iRet = Hal_Aux_AdcCal_EraseFlash();
    
    if(iRet == HAL_AUX_OK)
        msg_print_uart1("OK\r\n");
    else
        msg_print_uart1("ERROR\r\n");

    return iRet;
}

int at_cmd_ext_adcstore(char *buf, int len, int mode)
{
    int iRet = 0;

    iRet = Hal_Aux_AdcCal_StoreFlash();

    if(iRet == HAL_AUX_OK)
        msg_print_uart1("OK\r\n");
    else
        msg_print_uart1("ERROR\r\n");

    return iRet;
}

int at_cmd_ext_adcreload(char *buf, int len, int mode)
{
    int iRet = 0;

    iRet = Hal_Aux_AdcCal_LoadFlash();

    if(iRet == HAL_AUX_OK)
        msg_print_uart1("OK\r\n");
    else
        msg_print_uart1("ERROR\r\n");

    return iRet;
}

int at_cmd_ext_adcvbat(char *buf, int len, int mode)
{
    int iRet = 0;
    uint8_t u8WriteDirect_bak = g_ubHalAux_Pu_WriteDirect;
    float fVbat = 0;

    Hal_Aux_Init();
    Hal_Aux_AdcCal_Init();
    g_ubHalAux_Pu_WriteDirect = 1;
    iRet = Hal_Aux_VbatGet( &fVbat );
    g_ubHalAux_Pu_WriteDirect = u8WriteDirect_bak;

    msg_print_uart1("Got Vbat = %f\r\n", fVbat);

    if(iRet == HAL_AUX_OK)
        msg_print_uart1("OK\r\n");
    else
        msg_print_uart1("ERROR\r\n");
    return iRet;
}

int at_cmd_ext_adcgpio(char *buf, int len, int mode)
{
    int iRet = 0;
    uint8_t u8WriteDirect_bak = g_ubHalAux_Pu_WriteDirect;
    float fVbat = 0;
    uint8_t ubGpioIdx = 0;

    int argc = 0;
    char *argv[AT_MAX_CMD_ARGS] = {0};

    at_cmd_buf_to_argc_argv(buf, &argc, argv, AT_MAX_CMD_ARGS);

    if(argc >= 2)
        ubGpioIdx = atoi(argv[1]);

    Hal_Aux_Init();
    Hal_Aux_AdcCal_Init();
    g_ubHalAux_Pu_WriteDirect = 1;
    iRet = Hal_Aux_IoVoltageGet( ubGpioIdx, &fVbat );
    g_ubHalAux_Pu_WriteDirect = u8WriteDirect_bak;

    msg_print_uart1("Got GPIO = %f\r\n", fVbat);

    if(iRet == HAL_AUX_OK)
        msg_print_uart1("OK\r\n");
    else
        msg_print_uart1("ERROR\r\n");
    return iRet;
}
#if !defined(SDK_LITE)
at_command_t gAtCmdTbl_Ext[] =
{
    { "at+crlfterm",            at_cmd_ext_crlf_term,     "Enable/disable CRLF termination"},
    { "at+leinfo",              at_cmd_ext_le_info,       "Dump BLE packet info statistics"},
    { "at+legain",              at_cmd_ext_le_gain,       "Configure LE Rx AGC gain"},
    { "at+auxadcdbg",           at_cmd_ext_auxadcdbg,     "Auxadc raw-data (for debug)"},
    { "at+auxadc",              at_cmd_ext_auxadc,        "Auxadc value"},
    { "at+adccalvbat",          at_cmd_ext_adccalvbat,    "Auxadc cal. from VBAT"},
    { "at+adccalgpio",          at_cmd_ext_adccalgpio,    "Auxadc cal. from GPIO"},
    { "at+adcdef",              at_cmd_ext_adcdef,        "Auxadc re-cal all via int. src."},
    { "at+adcerase",            at_cmd_ext_adcerase,      "Erase adc-cal result"},
    { "at+adcstore",            at_cmd_ext_adcstore,      "Store adc-cal result"},
    { "at+adcreload",           at_cmd_ext_adcreload,     "Load adc-cal from flash"},
    { "at+adcvbat",             at_cmd_ext_adcvbat,       "Volt from Vbat"},
    { "at+adcgpio",             at_cmd_ext_adcgpio,       "Volt from GPIO"},
    { "at+come",                at_cmd_rf_come,           "Parameters for AT+RX"},
    { "at+dcoc",                at_cmd_rf_dcoc,           "Trigger DCOC"},
    { "at+dcth",                at_cmd_rf_dcth,           "Set/Got DCOC threshold"},
    { NULL,                     NULL,                     NULL},
};
#endif /* !defined(SDK_LITE) */

