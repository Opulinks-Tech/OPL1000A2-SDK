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
*  hal_dbg_uart.h
*
*  Project:
*  --------
*  OPL1000 series
*
*  Description:
*  ------------
*  This include file defines the proto-types of debug uart .
*
*  Author:
*  -------
*  Chung-chun Wang
******************************************************************************/

#ifndef __HAL_DBG_UART_H__
#define __HAL_DBG_UART_H__

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File 
#include <stdint.h>

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define HAL_DBG_UART_RX_TIMEOUT         1
#define HAL_DBG_UART_RX_SUCCESS         0
/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...
typedef enum 
{
    DBG_UART_INT_TX         = (1<<0),
    DBG_UART_INT_RX         = (1<<1),
    DBG_UART_INT_TX_OVERRUN = (1<<2),
    DBG_UART_INT_RX_OVERRUN = (1<<3)
} E_DbgUartInt_t;

typedef void (*T_DbgUart_RxCallBack)(uint32_t);

typedef uint32_t (*T_Hal_DbgUart_RxIntEn)(uint8_t u8Enable);
typedef uint8_t (*T_Hal_DbgUart_RxIntEnStatusGet)(void);
typedef uint32_t (*T_Hal_DbgUart_IntStatusRead)(void);
typedef uint32_t (*T_Hal_DbgUart_IntClear)(uint32_t u32Interrupts);
typedef uint32_t (*T_Hal_DbgUart_BaudRateSet)(uint32_t u32Baud);
typedef uint32_t (*T_Hal_DbgUart_BaudRateGet)(void);
typedef uint32_t (*T_Hal_DbgUart_Init)(uint32_t baud);
typedef void (*T_Hal_DbgUart_RxCallBackFuncSet)(T_DbgUart_RxCallBack tFunc);

// Those function are used for general case, time-out with default value(1 ms)
typedef uint32_t (*T_Hal_DbgUart_DataSend)(uint32_t u32Data);
typedef uint32_t (*T_Hal_DbgUart_DataRecv)(uint32_t* pu32Data);

// Those functions are recommand used only for bootROM and ISR, time-out with the given value
typedef uint32_t (*T_Hal_DbgUart_DataSendTimeOut)(uint32_t u32Data, uint32_t u32MilliSec);
typedef uint32_t (*T_Hal_DbgUart_DataRecvTimeOut)(uint32_t* pu32Data, uint32_t u32MilliSec);


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
extern T_DbgUart_RxCallBack DbgUart_RxCallBack;

extern T_Hal_DbgUart_RxIntEn Hal_DbgUart_RxIntEn;
extern T_Hal_DbgUart_RxIntEnStatusGet Hal_DbgUart_RxIntEnStatusGet;
extern T_Hal_DbgUart_IntStatusRead Hal_DbgUart_IntStatusRead;
extern T_Hal_DbgUart_IntClear Hal_DbgUart_IntClear;
extern T_Hal_DbgUart_BaudRateSet Hal_DbgUart_BaudRateSet;
extern T_Hal_DbgUart_BaudRateGet Hal_DbgUart_BaudRateGet;
extern T_Hal_DbgUart_Init Hal_DbgUart_Init;
extern T_Hal_DbgUart_RxCallBackFuncSet Hal_DbgUart_RxCallBackFuncSet;

// Those function are used for general case, time-out with default value(1 ms)
extern T_Hal_DbgUart_DataSend Hal_DbgUart_DataSend;
extern T_Hal_DbgUart_DataRecv Hal_DbgUart_DataRecv;

// Those functions are recommand used only for bootROM and ISR, time-out with the given value
extern T_Hal_DbgUart_DataSendTimeOut Hal_DbgUart_DataSendTimeOut;
extern T_Hal_DbgUart_DataRecvTimeOut Hal_DbgUart_DataRecvTimeOut;

// For cold boot init. include function poiters and parameters
void Hal_DbgUart_Pre_Init(void);

// For warm boot init. only parameters
void Hal_DbgUart_ParamInit(void);

#endif

