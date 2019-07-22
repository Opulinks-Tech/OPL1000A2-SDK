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
*  hal_tmr.h
*
*  Project:
*  --------
*  OPL1000 series
*
*  Description:
*  ------------
*  This include file defines the proto-types of Timer functions
*
*  Author:
*  -------
*  Luke Liao
******************************************************************************/

#ifndef __HAL_TMR_H__
#define __HAL_TMR_H__

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File
#include <stdio.h>
#include <stdint.h>

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...
typedef void (*T_Tmr_CallBack)(uint32_t u32TmrIdx);

typedef void (*T_Hal_Tmr_Init)(uint32_t u32Timer);
typedef void (*T_Hal_Tmr_Start)(uint32_t u32Timer, uint32_t u32Us);
typedef void (*T_Hal_Tmr_Stop)(uint32_t u32Timer);
typedef void (*T_Hal_Tmr_Reload)(uint32_t u32Timer, uint32_t u32Us);
typedef uint32_t (*T_Hal_Tmr_ReadCurrValue)(uint32_t u32Timer);
typedef void (*T_Hal_Tmr_IntClear)(uint32_t u32Timer);
typedef void (*T_Hal_Tmr_CallBackFuncSet)(uint32_t u32Timer, T_Tmr_CallBack tFunc);


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern T_Tmr_CallBack Tmr_CallBack[2];

// Sec 5: declaration of global function prototype

/***************************************************
Declaration of static Global Variables &  Functions
***************************************************/
// Sec 6: declaration of static global  variable

// Sec 7: declaration of static function prototype

/***********
C Functions
***********/
// Sec 8: C Functions
extern T_Hal_Tmr_Init Hal_Tmr_Init;
extern T_Hal_Tmr_Start Hal_Tmr_Start;
extern T_Hal_Tmr_Stop Hal_Tmr_Stop;
extern T_Hal_Tmr_Reload Hal_Tmr_Reload;
extern T_Hal_Tmr_ReadCurrValue Hal_Tmr_ReadCurrValue;
extern T_Hal_Tmr_IntClear Hal_Tmr_IntClear;
extern T_Hal_Tmr_CallBackFuncSet Hal_Tmr_CallBackFuncSet;

void Hal_Tmr_Pre_Init(void);

#endif
