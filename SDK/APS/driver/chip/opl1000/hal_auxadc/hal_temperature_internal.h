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
*  hal_temperature_internal.h
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
#ifndef _HAL_TEMPERATURE_INTERNAL_H_
#define _HAL_TEMPERATURE_INTERNAL_H_

#ifdef __cplusplus
extern "C" {
#endif

// Sec 0: Comment block of the file


// Sec 1: Include File
#include "hal_temperature.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable


// Sec 5: declaration of global function prototype
typedef uint8_t (*T_Hal_Tmpr_CompareResistor_Fp)(float fResistor, float *pfTemperature);
typedef void (*T_Hal_Tmpr_CmdParse_Fp)(char *sCmd);

extern T_Hal_Tmpr_CompareResistor_Fp Hal_Tmpr_CompareResistor;
extern T_Hal_Tmpr_CmdParse_Fp Hal_Tmpr_CmdParse;


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


#ifdef __cplusplus
}
#endif

#endif // _HAL_TEMPERATURE_INTERNAL_H_
