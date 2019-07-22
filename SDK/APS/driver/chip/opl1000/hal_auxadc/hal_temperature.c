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

/******************************************************************************
*  Filename:
*  ---------
*  hal_temperature.c
*
*  Project:
*  --------
*  OPL1000 Project - the Temperature Sensor implement file
*
*  Description:
*  ------------
*  This implement file is include the Temperature Sensor function and api.
*
*  Author:
*  -------
*  Jeff Kuo
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include <stdlib.h>
#include <string.h>
#include "hal_temperature.h"
#include "hal_temperature_internal.h"
#include "boot_sequence.h"
#include "mw_fim_default_group03.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define HAL_TMPR_RESISTOR_MAX       10000


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
uint8_t g_ubHalTmpr_Init;
RET_DATA T_HalTmprCalData g_tHalTmpr_CalData;

RET_DATA T_Hal_Tmpr_Init_Fp Hal_Tmpr_Init;
RET_DATA T_Hal_Tmpr_TemperatureGet_Fp Hal_Tmpr_TemperatureGet;
RET_DATA T_Hal_Tmpr_CompareResistor_Fp Hal_Tmpr_CompareResistor;
RET_DATA T_Hal_Tmpr_CmdParse_Fp Hal_Tmpr_CmdParse;


// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


/***********
C Functions
***********/
// Sec 8: C Functions

/*************************************************************************
* FUNCTION:
*   Hal_Tmpr_Init
*
* DESCRIPTION:
*   Temperature Sensor init
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Hal_Tmpr_Init_impl(void)
{
    // before init
    g_ubHalTmpr_Init = 0;

    // cold boot
    if (0 == Boot_CheckWarmBoot())
    {
        if (MW_FIM_OK != MwFim_FileRead(MW_FIM_IDX_GP03_CAL_TEMPERATURE, 0, MW_FIM_CAL_TEMPERATURE_SIZE, (uint8_t*)&g_tHalTmpr_CalData))
        {
            // if fail, get the default value
            memcpy(&g_tHalTmpr_CalData, &g_tMwFimDefaultCalTmpr, MW_FIM_CAL_TEMPERATURE_SIZE);
        }
    }

    // after init
    g_ubHalTmpr_Init = 1;
}

/*************************************************************************
* FUNCTION:
*   Hal_Tmpr_TemperatureGet
*
* DESCRIPTION:
*   get the temperature
*
* PARAMETERS
*   1. ubGpioIdx     : [In] the index of GPIO
*   2. pfTemperature : [Out] the temperature
*
* RETURNS
*   1. HAL_TMPR_OK   : success
*   2. HAL_TMPR_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Tmpr_TemperatureGet_impl(uint8_t ubGpioIdx, float *pfTemperature)
{
    float fVbat;
    float fIoVoltage;
    float fResistor;
    uint8_t ubRet = HAL_TMPR_FAIL;
    
    // check init
    if (g_ubHalTmpr_Init != 1)
        goto done;

    if (HAL_AUX_OK != Hal_Aux_VbatGet(&fVbat))
        goto done;
    
    if (HAL_AUX_OK != Hal_Aux_IoVoltageGet(ubGpioIdx, &fIoVoltage))
        goto done;

    // Vio = (R2/(R1+R2)) * Vbat
    // Vio * (R1+R2) = R2 * Vbat
    // Vio * R1 = R2 * (Vbat - Vio)
    // R1 = R2 * (Vbat - Vio) / Vio

    // error handle
    if (fIoVoltage == 0)
    {
        fResistor = HAL_TMPR_RESISTOR_MAX;
    }
    else
    {
        fResistor = g_tHalTmpr_CalData.fVolDivResistor * (fVbat - fIoVoltage) / fIoVoltage;
    }
    if (HAL_TMPR_OK != Hal_Tmpr_CompareResistor(fResistor, pfTemperature))
        goto done;
    
    ubRet = HAL_TMPR_OK;

done:
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*   Hal_Tmpr_CompareResistor
*
* DESCRIPTION:
*   compare the resistor to get the temperature
*
* PARAMETERS
*   1. fResistor     : [In] the resistor
*   2. pfTemperature : [Out] the temperature
*
* RETURNS
*   1. HAL_TMPR_OK   : success
*   2. HAL_TMPR_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Tmpr_CompareResistor_impl(float fResistor, float *pfTemperature)
{
    float fInterpolation;
    uint8_t i;
    
    // max resistor : min temperature
    if (fResistor >= g_tHalTmpr_CalData.faThermistor[0])
    {
        *pfTemperature = g_tHalTmpr_CalData.fBaseTemperature;
    }
    // min resistor : max temperature
    else if (fResistor < g_tHalTmpr_CalData.faThermistor[HAL_TMPR_STEP_MAX - 1])
    {
        *pfTemperature = g_tHalTmpr_CalData.fBaseTemperature + (HAL_TMPR_STEP_MAX - 1);
    }
    // others
    else
    {
        for (i=0; i<(HAL_TMPR_STEP_MAX-1); i++)
        {
            if ((g_tHalTmpr_CalData.faThermistor[i] > fResistor) && (fResistor >= g_tHalTmpr_CalData.faThermistor[i+1]))
            {
                fInterpolation = 1 - ((fResistor - g_tHalTmpr_CalData.faThermistor[i+1])
                                      / (g_tHalTmpr_CalData.faThermistor[i] - g_tHalTmpr_CalData.faThermistor[i+1]));
                *pfTemperature = g_tHalTmpr_CalData.fBaseTemperature + i + fInterpolation;
                
                break;
            }
        }
    }
    
    return HAL_TMPR_OK;
}

/*************************************************************************
* FUNCTION:
*   Hal_Tmpr_CmdParse
*
* DESCRIPTION:
*   Parse the Temperature Sensor cmd
*
* PARAMETERS
*   1. sCmd    : [In] the input string
*
* RETURNS
*
*************************************************************************/
void Hal_Tmpr_CmdParse_impl(char *sCmd)
{
    float fValue;
    uint32_t ulValue;
    uint8_t ubIdx;

    uint8_t ucaSeps[] = " \t\r\n";         // space, tab, new line
    uint8_t *pucaToken[8] = {0};
    uint8_t i;
    char *pSavedPtr = NULL;

    printf("\n");

    // get the first token
    pucaToken[0] = (uint8_t*)strtok_r(sCmd, (const char*)ucaSeps, &pSavedPtr);

    // get the other token
    if (NULL != pucaToken[0])
    {
        for (i=1; i<8; i++)
        {
            pucaToken[i] = (uint8_t*)strtok_r(NULL, (const char*)ucaSeps, &pSavedPtr);
            if (NULL == pucaToken[i])
                break;
        }
    }

    // get the temperature value
    if (i == 2)
    {
        ubIdx = strtoul((const char*)pucaToken[1], NULL, 0);
        
        if (HAL_TMPR_OK == Hal_Tmpr_TemperatureGet(ubIdx, &fValue))
        {
            ulValue = (uint32_t)(fValue * 1000);
            printf("Temperature [%u.%03u]\n", ulValue / 1000, ulValue % 1000);
        }
    }
    else
    {
        printf("It is an invalid command.\n");
    }
}

/*************************************************************************
* FUNCTION:
*   Hal_Tmpr_PreInitCold
*
* DESCRIPTION:
*   the previous initial for cold boot
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Hal_Tmpr_PreInitCold(void)
{
    g_ubHalTmpr_Init = 0;
    memcpy(&g_tHalTmpr_CalData, &g_tMwFimDefaultCalTmpr, MW_FIM_CAL_TEMPERATURE_SIZE);

    Hal_Tmpr_Init = Hal_Tmpr_Init_impl;
    Hal_Tmpr_TemperatureGet = Hal_Tmpr_TemperatureGet_impl;
    Hal_Tmpr_CompareResistor = Hal_Tmpr_CompareResistor_impl;
    Hal_Tmpr_CmdParse = Hal_Tmpr_CmdParse_impl;
}
