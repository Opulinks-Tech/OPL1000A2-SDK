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
*  hal_auxadc_internal.h
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
#ifndef _HAL_AUXADC_INTERNAL_H_
#define _HAL_AUXADC_INTERNAL_H_

#ifdef __cplusplus
extern "C" {
#endif

// Sec 0: Comment block of the file


// Sec 1: Include File
#include "hal_auxadc.h"


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
typedef uint8_t (*T_Hal_Aux_SourceSelect_Fp)(E_HalAux_Src_t tSrc, uint8_t ubGpioIdx);
typedef uint8_t (*T_Hal_Aux_AdcValueGet_Fp)(uint32_t *pulValue);

typedef void (*T_Hal_Aux_CmdParse_Fp)(char *sCmd);
typedef void (*T_Hal_Aux_Pinmux_Fp)(uint8_t ubIdx);

extern T_Hal_Aux_SourceSelect_Fp Hal_Aux_SourceSelect;
extern T_Hal_Aux_AdcValueGet_Fp Hal_Aux_AdcValueGet;

extern T_Hal_Aux_CmdParse_Fp Hal_Aux_CmdParse;
extern T_Hal_Aux_Pinmux_Fp Hal_Aux_Pinmux;


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


#ifdef __cplusplus
}
#endif

#endif // _HAL_AUXADC_INTERNAL_H_
