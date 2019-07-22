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
*  hal_patch.h
*
*  Project:
*  --------
*  OPL1000 Project - the patch definition file
*
*  Description:
*  ------------
*  This include file is the patch definition file
*
*  Author:
*  -------
*  Jeff Kuo
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
#ifndef _HAL_PATCH_H_
#define _HAL_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

// Sec 0: Comment block of the file


// Sec 1: Include File
#include <stdio.h>
#include <stdint.h>


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous

// Return
#define HAL_PATCH_RET_FAIL          0
#define HAL_PATCH_RET_NEXT          1
#define HAL_PATCH_RET_FINISH        2
#define HAL_PATCH_RET_LOOP          3       // for internal usage


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable


// Sec 5: declaration of global function prototype
void Hal_Patch_PreInitCold(void);

void Hal_Patch_Init(void);
uint8_t Hal_Patch_DataIn(uint32_t *pulAddr, uint32_t ulSize);
void Hal_Patch_ApplyHwPatchM3(void);
void Hal_Patch_ApplyHwPatchM0(void);
typedef void (*Hal_Patch_ProcessCmd_fp)(uint32_t u32Cmd, uint32_t u32Param);
extern Hal_Patch_ProcessCmd_fp Hal_Patch_ProcessCmd;

// for Sw patch test, need to remove
typedef void (*T_Test_ForSwPatchFp)(void);
extern T_Test_ForSwPatchFp Test_ForSwPatch;


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


#ifdef __cplusplus
}
#endif

#endif // _HAL_PATCH_H_
