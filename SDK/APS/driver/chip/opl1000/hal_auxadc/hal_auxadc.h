/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
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
*  hal_auxadc.h
*
*  Project:
*  --------
*  OPL1000 Project - the AUXADC definition file
*
*  Description:
*  ------------
*  This include file is the AUXADC definition file
*
*  Author:
*  -------
*  Jeff Kuo
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
#ifndef _HAL_AUXADC_H_
#define _HAL_AUXADC_H_

#ifdef __cplusplus
extern "C" {
#endif

// Sec 0: Comment block of the file


// Sec 1: Include File
#include <stdio.h>
#include <stdint.h>


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define HAL_AUX_OK                  1
#define HAL_AUX_FAIL                0

#define HAL_AUX_GPIO_NUM_MAX        16  // support 16 IOs : from 0 to 15

#define HAL_AUX_BASE_VBAT           0   // 0V
#define HAL_AUX_BASE_IO_VOL         0   // 0V

#define HAL_AUX_AVERAGE_COUNT       30  // the average count of ADC value


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, uniou, enum, linked list
// source type
typedef enum
{
    HAL_AUX_SRC_GPIO = 0,
    HAL_AUX_SRC_VBAT,
    HAL_AUX_SRC_LDO_VCO,
    HAL_AUX_SRC_LDO_RF,
    HAL_AUX_SRC_TEMP_SEN,
    HAL_AUX_SRC_HPBG_REF,
    HAL_AUX_SRC_LPBG_REF,
    HAL_AUX_SRC_PMU_SF,
    
    HAL_AUX_SRC_MAX
} E_HalAux_Src_t;

// the calibration data of AUXADC
typedef struct
{
    float fSlopeVbat;
    float fSlopeIo;
    int16_t wDcOffsetVbat;      // 0V
    int16_t wDcOffsetIo;        // 0V
} T_HalAuxCalData;


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable


// Sec 5: declaration of global function prototype
void Hal_Aux_PreInitCold(void);

typedef void (*T_Hal_Aux_Init_Fp)(void);
typedef uint8_t (*T_Hal_Aux_VbatGet_Fp)(float *pfVbat);
typedef uint8_t (*T_Hal_Aux_IoVoltageGet_Fp)(uint8_t ubGpioIdx, float *pfVoltage);
typedef uint8_t (*T_Hal_Aux_VbatCalibration_Fp)(float fVbat);
typedef uint8_t (*T_Hal_Aux_IoVoltageCalibration_Fp)(uint8_t ubGpioIdx, float fVoltage);

extern T_Hal_Aux_Init_Fp Hal_Aux_Init;
extern T_Hal_Aux_VbatGet_Fp Hal_Aux_VbatGet;
extern T_Hal_Aux_IoVoltageGet_Fp Hal_Aux_IoVoltageGet;
extern T_Hal_Aux_VbatCalibration_Fp Hal_Aux_VbatCalibration;
extern T_Hal_Aux_IoVoltageCalibration_Fp Hal_Aux_IoVoltageCalibration;


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


#ifdef __cplusplus
}
#endif

#endif // _HAL_AUXADC_H_
