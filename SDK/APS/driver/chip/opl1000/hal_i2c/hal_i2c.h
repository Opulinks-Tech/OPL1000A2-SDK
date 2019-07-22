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

#ifndef __HAL_I2C_H__
#define __HAL_I2C_H__

/******************************************************************************
*  Filename:
*  ---------
*  hal_i2c.c
*
*  Project:
*  --------
*  OPL1000 series
*
*  Description:
*  ------------
*  This include file defines the proto-types of I2C .
*
*  Author:
*  -------
*  Chung-Chun Wang
******************************************************************************/

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
typedef enum 
{
    I2C_SPEED_STANDARD, /* 100K */
    I2C_SPEED_FAST      /* 400K */
    //I2C_SPEED_HIGH    /* 3.4M, not supported */
} E_I2cSpeed_t;

typedef enum 
{
    I2C_07BIT,
    I2C_10BIT
} E_I2cAddrMode_t;

typedef E_I2cSpeed_t (*T_Hal_I2c_SpeedGet)(void);
typedef uint32_t (*T_Hal_I2c_SpeedSet)(E_I2cSpeed_t eSpeed);
typedef uint32_t (*T_Hal_I2c_FsClockSet)(uint16_t u16Hcnt, uint16_t u16Lcnt);
typedef void (*T_Hal_I2c_IntHandler)(void);

/* Master mode relative */
typedef uint32_t (*T_Hal_I2c_TargetAddrSet)(uint16_t u16TargetAddr);
typedef uint32_t (*T_Hal_I2c_MasterReceive)(uint8_t *pu8Data, uint32_t u32Length, uint8_t u8NeedStopBit);
typedef uint32_t (*T_Hal_I2c_MasterTrasmit)(uint8_t *pu8Data, uint32_t u32Length, uint8_t u8NeedStopBit);
typedef uint32_t (*T_Hal_I2c_MasterInit)(E_I2cAddrMode_t eAddrMode, E_I2cSpeed_t eSpeed);

/* Slave mode relative */
typedef void (*T_Hal_I2c_SlaveIntTxCallBack)(void);
typedef void (*T_Hal_I2c_SlaveIntRxCallBack)(uint8_t);
typedef uint32_t (*T_Hal_I2c_SlaveReceive)(uint8_t *pu8Data, uint32_t u32Length);
typedef uint32_t (*T_Hal_I2c_SlaveTrasmit)(uint8_t *pu8Data, uint32_t u32Length);
typedef uint32_t (*T_Hal_I2c_SlaveInit)(E_I2cAddrMode_t eAddrMode, uint16_t u16SlaveAddr);
typedef uint32_t (*T_Hal_I2c_SlaveIntTxEn)(uint8_t u8Enable);
typedef uint32_t (*T_Hal_I2c_SlaveIntRxEn)(uint8_t u8Enable);
typedef void (*T_Hal_I2c_SlaveIntTxCallBackFuncSet)(T_Hal_I2c_SlaveIntTxCallBack tFunc);
typedef void (*T_Hal_I2c_SlaveIntRxCallBackFuncSet)(T_Hal_I2c_SlaveIntRxCallBack tFunc);


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
extern T_Hal_I2c_SpeedGet   Hal_I2c_SpeedGet;
extern T_Hal_I2c_SpeedSet   Hal_I2c_SpeedSet;
extern T_Hal_I2c_IntHandler Hal_I2c_IntHandler;
extern T_Hal_I2c_FsClockSet Hal_I2c_FsClockSet;

/* Master mode relative */
extern T_Hal_I2c_TargetAddrSet Hal_I2c_TargetAddrSet;
extern T_Hal_I2c_MasterReceive Hal_I2c_MasterReceive;
extern T_Hal_I2c_MasterTrasmit Hal_I2c_MasterTrasmit;
extern T_Hal_I2c_MasterInit    Hal_I2c_MasterInit;

/* Slave mode relative */
extern T_Hal_I2c_SlaveReceive              Hal_I2c_SlaveReceive;
extern T_Hal_I2c_SlaveTrasmit              Hal_I2c_SlaveTrasmit;
extern T_Hal_I2c_SlaveInit                 Hal_I2c_SlaveInit;
extern T_Hal_I2c_SlaveIntTxEn              Hal_I2c_SlaveIntTxEn;
extern T_Hal_I2c_SlaveIntRxEn              Hal_I2c_SlaveIntRxEn;
extern T_Hal_I2c_SlaveIntTxCallBackFuncSet Hal_I2c_SlaveIntTxCallBackFuncSet;
extern T_Hal_I2c_SlaveIntRxCallBackFuncSet Hal_I2c_SlaveIntRxCallBackFuncSet;

void Hal_I2c_Pre_Init(void);

#endif
