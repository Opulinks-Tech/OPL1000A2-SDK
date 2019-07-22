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
*  hal_wdt.h
*
*  Project:
*  --------
*  OPL1000 series
*
*  Description:
*  ------------
*  This include file defines the proto-types of Watchdog functions
*
*  Author:
*  -------
*  Luke Liao
******************************************************************************/

#ifndef __HAL_WDT_H__
#define __HAL_WDT_H__

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File 
#include <stdint.h>

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global  variable
typedef void (*T_Hal_Wdt_Init)(uint32_t u32Ticks);
typedef void (*T_Hal_Wdt_InitForInt)(uint32_t u32Ticks);
typedef void (*T_Hal_Wdt_Start)(void);
typedef void (*T_Hal_Wdt_Stop)(void);
typedef void (*T_Hal_Wdt_Feed)(uint32_t u32Ticks);
typedef void (*T_Hal_Wdt_Clear)(void);

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
extern T_Hal_Wdt_Init Hal_Wdt_Init;
extern T_Hal_Wdt_InitForInt Hal_Wdt_InitForInt;
extern T_Hal_Wdt_Start Hal_Wdt_Start;
extern T_Hal_Wdt_Stop Hal_Wdt_Stop;
extern T_Hal_Wdt_Feed Hal_Wdt_Feed;
extern T_Hal_Wdt_Clear Hal_Wdt_Clear;

void Hal_Wdt_Pre_Init(void);

#endif
