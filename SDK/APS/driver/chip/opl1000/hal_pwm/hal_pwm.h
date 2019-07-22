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
*  hal_pwm.h
*
*  Project:
*  --------
*  OPL1000 Project - the PWM definition file
*
*  Description:
*  ------------
*  This include file is the PWM definition file
*
*  Author:
*  -------
*  Jeff Kuo
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
#ifndef _HAL_PWM_H_
#define _HAL_PWM_H_

#ifdef __cplusplus
extern "C" {
#endif

// Sec 0: Comment block of the file


// Sec 1: Include File
#include <stdio.h>
#include <stdint.h>


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
// Index
#define HAL_PWM_IDX_0       0x01
#define HAL_PWM_IDX_1       0x02
#define HAL_PWM_IDX_2       0x04
#define HAL_PWM_IDX_3       0x08
#define HAL_PWM_IDX_4       0x10
#define HAL_PWM_IDX_5       0x20
#define HAL_PWM_IDX_ALL     0x3F

#define HAL_PWM_MAX         6       // total count

// clock source
#define HAL_PWM_CLK_32K     0x01    // 32768 or 32000
#define HAL_PWM_CLK_22M     0x02    // 22MHz

// min and max value
#define HAL_PWM_MIN_PERIOD  1
#define HAL_PWM_MAX_PERIOD  255     // 8 bits

#define HAL_PWM_MIN_DUTY    0
#define HAL_PWM_MAX_DUTY    255     // 8 bits

#define HAL_PWM_MIN_RAMP    0
#define HAL_PWM_MAX_RAMP    255     // 8 bits

#define HAL_PWM_MIN_HOLD    1
#define HAL_PWM_MAX_HOLD    511     // 9 bits

// return
#define HAL_PWM_RET_FAIL    0
#define HAL_PWM_RET_PASS    1


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, uniou, enum, linked list
typedef struct
{
    // the bright state is the max tick count of high level in one clock cycle
    // the dull state is the min tick count of high level in one clock cycle

    uint32_t ulPeriod;          // the tick count of one clock cycle
    uint32_t ulDutyBright;      // the max tick count of high level in one clock cycle
    uint32_t ulDutyDull;        // the min tick count of high level in one clock cycle
    uint32_t ulRampUp;          // the delta count from dull to bright per clock cycle
    uint32_t ulRampDown;        // the delta count from bright to dull per clock cycle
    uint32_t ulHoldBright;      // the hold times of the bright state
    uint32_t ulHoldDull;        // the hold times the dull state
} S_Hal_Pwm_Config_t;


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable


// Sec 5: declaration of global function prototype
void Hal_Pwm_PreInitCold(void);

typedef void (*T_Hal_Pwm_InitFp)(void);
typedef void (*T_Hal_Pwm_EnableFp)(uint8_t ubIdxMask);
typedef void (*T_Hal_Pwm_SyncEnableFp)(uint8_t ubIdxMask);
typedef void (*T_Hal_Pwm_DisableFp)(uint8_t ubIdxMask);
typedef void (*T_Hal_Pwm_ClockSourceSetFp)(uint8_t ubSrc);
typedef uint8_t (*T_Hal_Pwm_SimpleConfigSetFp)(uint8_t ubIdxMask, uint8_t ubDuty, uint32_t ulHz);
typedef uint8_t (*T_Hal_Pwm_ComplexConfigSetFp)(uint8_t ubIdxMask, S_Hal_Pwm_Config_t tConfig);

extern T_Hal_Pwm_InitFp Hal_Pwm_Init;
extern T_Hal_Pwm_EnableFp Hal_Pwm_Enable;
extern T_Hal_Pwm_SyncEnableFp Hal_Pwm_SyncEnable;
extern T_Hal_Pwm_DisableFp Hal_Pwm_Disable;
extern T_Hal_Pwm_ClockSourceSetFp Hal_Pwm_ClockSourceSet;
extern T_Hal_Pwm_SimpleConfigSetFp Hal_Pwm_SimpleConfigSet;
extern T_Hal_Pwm_ComplexConfigSetFp Hal_Pwm_ComplexConfigSet;


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


#ifdef __cplusplus
}
#endif

#endif // _HAL_PWM_H_
