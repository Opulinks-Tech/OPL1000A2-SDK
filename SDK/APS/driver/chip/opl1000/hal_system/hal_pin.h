/******************************************************************************
*  Copyright 2017, Netlink Communication Corp.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Netlink Communication Corp. (C) 2017
******************************************************************************/

/******************************************************************************
*  Filename:
*  ---------
*  hal_pin.h
*
*  Project:
*  --------
*  NL1000_A1 series
*
*  Description:
*  ------------
*  This include file defines the patch proto-types of system functions
*  Include sys-reg and AOS domains.
*
*  Author:
*  -------
*  Jeff
******************************************************************************/

#ifndef __HAL_PIN_H__
#define __HAL_PIN_H__

#ifdef __cplusplus
extern "C" {
#endif

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File
#include <stdio.h>
#include <stdint.h>

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define HAL_PIN_IO_COUNT    31

#define HAL_PIN_OK          1
#define HAL_PIN_FAIL        0

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global  variable

// Sec 5: declaration of global function prototype
// External
typedef uint8_t (*T_Hal_Pin_ConfigSet_Fp)(uint8_t ubIoIdx, uint16_t uwConfig, uint8_t ubDriving);
extern T_Hal_Pin_ConfigSet_Fp Hal_Pin_ConfigSet;

// Internal

// the pre-init
void Hal_Pin_PreInitCold(void);

/***************************************************
Declaration of static Global Variables &  Functions
***************************************************/
// Sec 6: declaration of static global  variable

// Sec 7: declaration of static function prototype

#ifdef __cplusplus
}
#endif

#endif  // end of __HAL_PIN_H__
