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
*  hal_int.h
*
*  Project:
*  --------
*  OPL1000 series
*
*  Description:
*  ------------
*  This include file defines the proto-types of vic functions
*
*  Author:
*  -------
*  Luke Liao
******************************************************************************/

#ifndef __HAL_VIC_H__
#define __HAL_VIC_H__

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File 
#include <stdint.h>
#include "opl1000.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...
typedef enum 
{
    IPC_IDX_0 = 0x00,
    IPC_IDX_1,
    IPC_IDX_2,
    IPC_IDX_3,
    IPC_IDX_MAX
} E_IpcIdx_t;

typedef enum 
{
    INT_TYPE_LEVEL        = 0x0,
    INT_TYPE_RISING_EDGE  = 0x1,
    INT_TYPE_FALLING_EDGE = 0x2,
    INT_TYPE_BOTH_EDGE    = 0x3
} E_ItrType_t;

typedef enum 
{
    GPIO_IDX_00 = 0x00,
    GPIO_IDX_01,
    GPIO_IDX_02,
    GPIO_IDX_03,
    GPIO_IDX_04,
    GPIO_IDX_05,
    GPIO_IDX_06,
    GPIO_IDX_07,
    GPIO_IDX_08,
    GPIO_IDX_09,
    GPIO_IDX_10,
    GPIO_IDX_11,
    GPIO_IDX_12,
    GPIO_IDX_13,
    GPIO_IDX_14,
    GPIO_IDX_15,
    GPIO_IDX_16,
    GPIO_IDX_17,
    GPIO_IDX_18,
    GPIO_IDX_19,
    GPIO_IDX_20,
    GPIO_IDX_21,
    GPIO_IDX_22,
    GPIO_IDX_23,
    GPIO_IDX_24,
    GPIO_IDX_25,
    GPIO_IDX_26,
    GPIO_IDX_27,
    GPIO_IDX_28,
    GPIO_IDX_29,
    GPIO_IDX_30,
    GPIO_IDX_MAX
    // Note: only 30 GPIOs
} E_GpioIdx_t;

typedef enum
{
    GPIO_OUTPUT     = 0,
    GPIO_INPUT      = 1
} E_GpioDirection_t;

typedef enum
{
    GPIO_LEVEL_LOW  = 0,
    GPIO_LEVEL_HIGH = 1
} E_GpioLevel_t;

// VIC moudle
typedef void (*T_Hal_Vic_IntEn)(IRQn_Type eInterrupt, uint8_t u8Enable);
typedef void (*T_Hal_Vic_IntClear)(IRQn_Type eInterrupt);
typedef void (*T_Hal_Vic_IntMask)(IRQn_Type eInterrupt, uint8_t u8Enable);
typedef void (*T_Hal_Vic_IntTypeSel)(IRQn_Type eInterrupt, E_ItrType_t eType);
typedef void (*T_Hal_Vic_IntInv)(IRQn_Type eInterrupt, uint8_t u8Enable);
typedef uint32_t (*T_Hal_Vic_IntStatRead)(void);
typedef void (*T_Hal_Vic_Init)(void);

// IPC module
typedef void (*T_Hal_Vic_IpcIntTrig)(E_IpcIdx_t eIpc);
typedef void (*T_Hal_Vic_IpcIntEn)(E_IpcIdx_t eIpc, uint8_t u8Enable);
typedef void (*T_Hal_Vic_IpcIntClear)(E_IpcIdx_t eIpc);

// GPIO module
typedef void (*T_Gpio_CallBack)(E_GpioIdx_t eIdx);

typedef void (*T_Hal_Vic_GpioIntEn)(E_GpioIdx_t eGpio, uint8_t u8Enable);
typedef void (*T_Hal_Vic_GpioIntClear)(E_GpioIdx_t eGpio);
typedef void (*T_Hal_Vic_GpioIntMask)(E_GpioIdx_t eGpio, uint8_t u8Enable);
typedef void (*T_Hal_Vic_GpioIntTypeSel)(E_GpioIdx_t eGpio, E_ItrType_t eType);
typedef void (*T_Hal_Vic_GpioIntInv)(E_GpioIdx_t eGpio, uint8_t u8Enable);
typedef uint32_t (*T_Hal_Vic_GpioIntStatRead)(void);
typedef void (*T_Hal_Vic_GpioCallBackFuncSet)(E_GpioIdx_t eIdx, T_Gpio_CallBack tFunc);

typedef void (*T_Hal_Vic_GpioInit)(void);
typedef E_GpioLevel_t (*T_Hal_Vic_GpioInput)(E_GpioIdx_t eIdx);
typedef void (*T_Hal_Vic_GpioOutput)(E_GpioIdx_t eIdx, E_GpioLevel_t tLevel);
typedef void (*T_Hal_Vic_GpioDirection)(E_GpioIdx_t eIdx, E_GpioDirection_t tDirection);
typedef void (*T_Hal_Vic_GpioPinmux)(E_GpioIdx_t eIdx, E_GpioDirection_t tDirection, E_GpioLevel_t tOutputLevel);

// MmFactor relative
typedef void (*T_Hal_Vic_MmFactorResumeByIpcIntEn)(E_IpcIdx_t eIpc, uint8_t u8Enable);
typedef uint32_t (*T_Hal_Vic_MmFactorResumeByIpcIntStatRead)(void);
typedef void (*T_Hal_Vic_MmFactorResumeByGpioIntEn)(E_GpioIdx_t eGpio, uint8_t u8Enable);
typedef uint32_t (*T_Hal_Vic_MmFactorResumeByGpioIntStatRead)(void);

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global  variable

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
// VIC moudle
extern T_Hal_Vic_IntEn       Hal_Vic_IntEn;
extern T_Hal_Vic_IntClear    Hal_Vic_IntClear;
extern T_Hal_Vic_IntMask     Hal_Vic_IntMask;
extern T_Hal_Vic_IntTypeSel  Hal_Vic_IntTypeSel;
extern T_Hal_Vic_IntInv      Hal_Vic_IntInv;
extern T_Hal_Vic_IntStatRead Hal_Vic_IntStatRead;
extern T_Hal_Vic_Init        Hal_Vic_Init;

// IPC module
extern T_Hal_Vic_IpcIntTrig  Hal_Vic_IpcIntTrig;
extern T_Hal_Vic_IpcIntEn    Hal_Vic_IpcIntEn;
extern T_Hal_Vic_IpcIntClear Hal_Vic_IpcIntClear;

// GPIO module
extern T_Gpio_CallBack g_taHalVicGpioCallBack[GPIO_IDX_MAX];

extern T_Hal_Vic_GpioIntEn           Hal_Vic_GpioIntEn;
extern T_Hal_Vic_GpioIntClear        Hal_Vic_GpioIntClear;
extern T_Hal_Vic_GpioIntMask         Hal_Vic_GpioIntMask;
extern T_Hal_Vic_GpioIntTypeSel      Hal_Vic_GpioIntTypeSel;
extern T_Hal_Vic_GpioIntInv          Hal_Vic_GpioIntInv;
extern T_Hal_Vic_GpioIntStatRead     Hal_Vic_GpioIntStatRead;
extern T_Hal_Vic_GpioCallBackFuncSet Hal_Vic_GpioCallBackFuncSet;

extern T_Hal_Vic_GpioInit      Hal_Vic_GpioInit;
extern T_Hal_Vic_GpioInput     Hal_Vic_GpioInput;
extern T_Hal_Vic_GpioOutput    Hal_Vic_GpioOutput;
extern T_Hal_Vic_GpioDirection Hal_Vic_GpioDirection;
extern T_Hal_Vic_GpioPinmux    Hal_Vic_GpioPinmux;

// MmFactor relative
extern T_Hal_Vic_MmFactorResumeByIpcIntEn        Hal_Vic_MmFactorResumeByIpcIntEn;
extern T_Hal_Vic_MmFactorResumeByIpcIntStatRead  Hal_Vic_MmFactorResumeByIpcIntStatRead;
extern T_Hal_Vic_MmFactorResumeByGpioIntEn       Hal_Vic_MmFactorResumeByGpioIntEn;
extern T_Hal_Vic_MmFactorResumeByGpioIntStatRead Hal_Vic_MmFactorResumeByGpioIntStatRead;

void Hal_Vic_Pre_Init(void);

#endif
