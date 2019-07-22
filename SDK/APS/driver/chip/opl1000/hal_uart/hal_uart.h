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
*  hal_uart.h
*
*  Project:
*  --------
*  OPL1000 series
*
*  Description:
*  ------------
*  This include file defines the proto-types of uart .
*
*  Author:
*  -------
*  Chung-chun Wang
******************************************************************************/

#ifndef __HAL_UART_H__
#define __HAL_UART_H__

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
// Sec 3: structure, uniou, enum, linked list…
typedef enum 
{
    PARITY_NONE = 0,
    PARITY_EVEN,
    PARITY_ODD
} E_UartParity_t;

typedef enum 
{
    STOP_BIT_1 = 0,
    STOP_BIT_1P5,
    STOP_BIT_2
} E_UartStopBit_t;

typedef enum 
{
    DATA_BIT_5 = 0,
    DATA_BIT_6,
    DATA_BIT_7,
    DATA_BIT_8
} E_UartDataBit_t;

typedef enum 
{
    UART_IDX_0 = 0,
    UART_IDX_1,
    
    UART_IDX_MAX
} E_UartIdx_t;

typedef enum 
{
    UART_INT_MODEM_STAT   = 0x0,
    UART_INT_NONE         = 0x1,
    UART_INT_TX_EMPTY     = 0x2,
    UART_INT_RX_AVALIBLE  = 0x4,
    UART_INT_RECV_LINE    = 0x6,
    UART_INT_BUSY         = 0x7,
    UART_INT_CHAR_TIMEOUT = 0xC
} E_UartIntId_t;

// the information of Uart config
typedef struct
{
    uint32_t ulBuadrate;
    uint8_t ubDataBit;
    uint8_t ubStopBit;
    uint8_t ubParity;
    uint8_t ubFlowCtrl;
} T_HalUartConfig;

typedef void (*T_Uart_RxCallBack)(uint32_t);

typedef uint32_t (*T_Hal_Uart_RxIntEn)(E_UartIdx_t eUartIdx, uint8_t u8Enable);
typedef uint32_t (*T_Hal_Uart_IntIdRead)(E_UartIdx_t eUartIdx);
typedef uint32_t (*T_Hal_Uart_IntClear)(E_UartIdx_t eUartIdx, E_UartIntId_t eIntID, uint32_t *pu32Buff);
typedef uint32_t (*T_Hal_Uart_BaudRateSet)(E_UartIdx_t eUartIdx, uint32_t u32Baud);
typedef uint32_t (*T_Hal_Uart_Init)(E_UartIdx_t eUartIdx, uint32_t u32Baud, E_UartDataBit_t u8Bits, E_UartParity_t u8Parity, E_UartStopBit_t u8StopBit, uint8_t u8EnFlowCtl);
typedef uint32_t (*T_Hal_Uart_ConfigGet)(E_UartIdx_t eUartIdx, T_HalUartConfig *ptConfig);
typedef uint32_t (*T_Hal_Uart_ConfigSet)(E_UartIdx_t eUartIdx, T_HalUartConfig *ptConfig);
typedef uint32_t (*T_Hal_Uart_DataSend)(E_UartIdx_t eUartIdx, uint32_t u32Data);
typedef uint32_t (*T_Hal_Uart_DataRecv)(E_UartIdx_t eUartIdx, uint32_t* pu32Data);
typedef void (*T_Hal_Uart_RxCallBackFuncSet)(E_UartIdx_t eUartIdx, T_Uart_RxCallBack tFunc);

// Those function are used for general case, time-out with default value(1 ms)
typedef uint32_t (*T_Hal_Uart_DataSend)(E_UartIdx_t eUartIdx, uint32_t u32Data);
typedef uint32_t (*T_Hal_Uart_DataRecv)(E_UartIdx_t eUartIdx, uint32_t* pu32Data);

// Those functions are recommand used only for bootROM and ISR, time-out with the given value
typedef uint32_t (*T_Hal_Uart_DataSendTimeOut)(E_UartIdx_t eUartIdx, uint32_t u32Data, uint32_t u32MilliSec);
typedef uint32_t (*T_Hal_Uart_DataRecvTimeOut)(E_UartIdx_t eUartIdx, uint32_t* pu32Data, uint32_t u32MilliSec);


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
extern T_Uart_RxCallBack            Uart_0_RxCallBack;
extern T_Uart_RxCallBack            Uart_1_RxCallBack;

extern T_Hal_Uart_RxIntEn           Hal_Uart_RxIntEn;
extern T_Hal_Uart_IntIdRead         Hal_Uart_IntIdRead;
extern T_Hal_Uart_IntClear          Hal_Uart_IntClear;
extern T_Hal_Uart_BaudRateSet       Hal_Uart_BaudRateSet;
extern T_Hal_Uart_Init              Hal_Uart_Init;
extern T_Hal_Uart_ConfigGet         Hal_Uart_ConfigGet;
extern T_Hal_Uart_ConfigSet         Hal_Uart_ConfigSet;
extern T_Hal_Uart_RxCallBackFuncSet Hal_Uart_RxCallBackFuncSet;

// Those function are used for general case, time-out with default value(1 ms)
extern T_Hal_Uart_DataSend Hal_Uart_DataSend;
extern T_Hal_Uart_DataRecv Hal_Uart_DataRecv;

// Those functions are recommand used only for bootROM and ISR, time-out with the given value
extern T_Hal_Uart_DataSendTimeOut Hal_Uart_DataSendTimeOut;
extern T_Hal_Uart_DataRecvTimeOut Hal_Uart_DataRecvTimeOut;

void Hal_Uart_Pre_Init(void);

#endif

