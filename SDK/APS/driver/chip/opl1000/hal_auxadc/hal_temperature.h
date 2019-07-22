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
*  hal_temperature.h
*
*  Project:
*  --------
*  OPL1000 Project - the Temperature Sensor definition file
*
*  Description:
*  ------------
*  This include file is the Temperature Sensor definition file
*
*  Author:
*  -------
*  Jeff Kuo
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
#ifndef _HAL_TEMPERATURE_H_
#define _HAL_TEMPERATURE_H_

#ifdef __cplusplus
extern "C" {
#endif

// Sec 0: Comment block of the file


// Sec 1: Include File
#include <stdio.h>
#include <stdint.h>


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define HAL_TMPR_OK                 1
#define HAL_TMPR_FAIL               0

#define HAL_TMPR_STEP_MAX           24      // 25 ~ 48

/******************************
Declaration of data structure
******************************/
// Sec 3: structure, uniou, enum, linked list
// the calibration data of Temperature Sensor
typedef struct
{
    float fBaseTemperature;                 // 25
    float faThermistor[HAL_TMPR_STEP_MAX];  // 25 ~ 48
    float fVolDivResistor;                  // Voltage divider resistor
} T_HalTmprCalData;


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable


// Sec 5: declaration of global function prototype
void Hal_Tmpr_PreInitCold(void);

typedef void (*T_Hal_Tmpr_Init_Fp)(void);
typedef uint8_t (*T_Hal_Tmpr_TemperatureGet_Fp)(uint8_t ubGpioIdx, float *pfTemperature);

extern T_Hal_Tmpr_Init_Fp Hal_Tmpr_Init;
extern T_Hal_Tmpr_TemperatureGet_Fp Hal_Tmpr_TemperatureGet;


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


#ifdef __cplusplus
}
#endif

#endif // _HAL_TEMPERATURE_H_
