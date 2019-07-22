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
*  hal_dbg_uart.c
*
*  Project:
*  --------
*  OPL1000 series
*
*  Description:
*  ------------
*  This source file defines the functions of debug uart .
*  Ref. document is << Cortex-M System Design Kit>>,  4.2 UART
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
#include "opl1000.h"
#include "hal_tick.h"
#include "hal_system.h"
#include "hal_vic.h"
#include "hal_dbg_uart.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define DBG_UART     ((S_DbgUart_Reg_t *) UART_DBG_BASE)
#define DEFAULT_BAUD 115200

/*
 * baud rate:115200 -> 100 us per char, 
 * Max core 176Mhz -> 1ms = 176*1000 tick = 0x2AF80 => 0x30000
 */
#define DBG_UART_TIMEOUT_COUNT_MAX  ( 0x60000 )
#define DBG_UART_TIMEOUT_MS_MAX     ( Hal_Tick_MilliSecMax() )


// 0x00
#define DBG_UART_DATA_MASK 0xFF

// 0x04
#define DBG_UART_STATE_TX_FULL    (1<<0)
#define DBG_UART_STATE_RX_FULL    (1<<1)
#define DBG_UART_STATE_TX_OVERRUN (1<<2)
#define DBG_UART_STATE_RX_OVERRUN (1<<3)

// 0x08
#define DBG_UART_CTRL_TX_EN             (1<<0)
#define DBG_UART_CTRL_RX_EN             (1<<1)
#define DBG_UART_CTRL_TX_INT_EN         (1<<2)
#define DBG_UART_CTRL_RX_INT_EN         (1<<3)
#define DBG_UART_CTRL_TX_OVERRUN_INT_EN (1<<4)
#define DBG_UART_CTRL_RX_OVERRUN_INT_EN (1<<5)

// 0x0C
#define DBG_UART_INT_STATUS_TX         (1<<0)
#define DBG_UART_INT_STATUS_RX         (1<<1)
#define DBG_UART_INT_STATUS_TX_OVERRUN (1<<2)
#define DBG_UART_INT_STATUS_RX_OVERRUN (1<<3)

// 0x10
#define DBG_UART_BAUD_DIV_MIN 0x00010
#define DBG_UART_BAUD_DIV_MAX 0xFFFFF

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...
typedef struct
{
    volatile uint32_t DATA;         //0x00
    volatile uint32_t STATE;        //0x04
    volatile uint32_t CTRL;         //0x08
    volatile uint32_t INT_STATUS;   //0x0C
    volatile uint32_t BAUD_DIV;     //0x10
} S_DbgUart_Reg_t;

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global  variable
T_DbgUart_RxCallBack                     DbgUart_RxCallBack;

RET_DATA uint32_t                        g_ulHalDbgUart_CurrentBaudRate;
RET_DATA uint8_t                         g_ubHalDbgUart_RxIntEnStatus;

RET_DATA T_Hal_DbgUart_RxIntEn           Hal_DbgUart_RxIntEn;
RET_DATA T_Hal_DbgUart_RxIntEnStatusGet  Hal_DbgUart_RxIntEnStatusGet;
RET_DATA T_Hal_DbgUart_IntStatusRead     Hal_DbgUart_IntStatusRead;
RET_DATA T_Hal_DbgUart_IntClear          Hal_DbgUart_IntClear;
RET_DATA T_Hal_DbgUart_BaudRateSet       Hal_DbgUart_BaudRateSet;
RET_DATA T_Hal_DbgUart_BaudRateGet       Hal_DbgUart_BaudRateGet;
RET_DATA T_Hal_DbgUart_Init              Hal_DbgUart_Init;
RET_DATA T_Hal_DbgUart_RxCallBackFuncSet Hal_DbgUart_RxCallBackFuncSet;

// Those function are used for general case, time-out with default value(1 ms)
RET_DATA T_Hal_DbgUart_DataSend          Hal_DbgUart_DataSend;
RET_DATA T_Hal_DbgUart_DataRecv          Hal_DbgUart_DataRecv;

// Those functions are recommand used only for bootROM and ISR, time-out with the given value
RET_DATA T_Hal_DbgUart_DataSendTimeOut   Hal_DbgUart_DataSendTimeOut;
RET_DATA T_Hal_DbgUart_DataRecvTimeOut   Hal_DbgUart_DataRecvTimeOut;

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
/*************************************************************************
* FUNCTION:
*  Hal_DbgUart_RxIntEn
*
* DESCRIPTION:
*   1. Enable rx interrupt
*
* CALLS
*
* PARAMETERS
*   1. u8Enable  : 1 for enable/0 for disable the interrupt
*
* RETURNS
*   0: setting complete
*   1: error
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_DbgUart_RxIntEn_impl(uint8_t u8Enable)
{			
    if(u8Enable)
    {
        // VIC 1) Clear interrupt
        Hal_Vic_IntClear(UARTDBG_IRQn);
        // VIC 2) Enable VIC
        Hal_Vic_IntEn(UARTDBG_IRQn, 1);
        // VIC 1) un-Mask VIC
        Hal_Vic_IntMask(UARTDBG_IRQn, 0);

        // NVIC 1) Clean NVIC
        NVIC_ClearPendingIRQ(UARTDBG_IRQn);
        // NVIC 2) Enable NVIC
        NVIC_EnableIRQ(UARTDBG_IRQn);
        // NVIC 3) Set prority
        NVIC_SetPriority(UARTDBG_IRQn, IRQ_PRIORITY_UARTDBG);
        
        // Module part: Enable module
        DBG_UART->CTRL |= DBG_UART_CTRL_RX_INT_EN;
    }else{
        // Module part: Disable module
        DBG_UART->CTRL &= ~DBG_UART_CTRL_RX_INT_EN;

        // VIC part: Disable VIC
        Hal_Vic_IntEn(UARTDBG_IRQn, 0);

        // NVIC part: Disable NVIC
        NVIC_DisableIRQ(UARTDBG_IRQn); 
    }
    
    g_ubHalDbgUart_RxIntEnStatus = u8Enable;
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_DbgUart_RxIntEnStatusGet
*
* DESCRIPTION:
*   1. get the status of enable rx interrupt
*
* CALLS
*
* PARAMETERS
*
* RETURNS
*   the status of enable rx interrupt
* GLOBALS AFFECTED
* 
*************************************************************************/
uint8_t Hal_DbgUart_RxIntEnStatusGet_impl(void)
{
    return g_ubHalDbgUart_RxIntEnStatus;
}

/*************************************************************************
* FUNCTION:
*  Hal_DbgUart_IntStatusRead
*
* DESCRIPTION:
*   1. Get interrupt status
*
* CALLS
*
* PARAMETERS
*   None
*
* RETURNS
*   single or multi interrupt, ref to E_DbgUartInt_t
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_DbgUart_IntStatusRead_impl(void)
{
    return DBG_UART->INT_STATUS;
}

/*************************************************************************
* FUNCTION:
*  Hal_DbgUart_IntClear
*
* DESCRIPTION:
*   1. Clear interrupts
*
* CALLS
*
* PARAMETERS
*  1. u32Interrupts: the interrupts want to clear.  ref to E_DbgUartInt_t
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_DbgUart_IntClear_impl(uint32_t u32Interrupts)
{
    // Clean module parts
    DBG_UART->INT_STATUS |= u32Interrupts;

    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_DbgUart_BaudRateSet
*
* DESCRIPTION:
*   1. Set baur-rate. (default:115200)
*
* CALLS
*
* PARAMETERS
*   1. u32Baud: Baud-rate
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_DbgUart_BaudRateSet_impl(uint32_t u32Baud)
{
    uint32_t result = 0;

    result = (SystemCoreClockGet() + u32Baud/2)/u32Baud;
    if( (result<DBG_UART_BAUD_DIV_MIN) ||(result>DBG_UART_BAUD_DIV_MAX) )
    {
        DBG_UART->BAUD_DIV = (SystemCoreClockGet() + DEFAULT_BAUD/2) / DEFAULT_BAUD;
        g_ulHalDbgUart_CurrentBaudRate = DEFAULT_BAUD;
        return 1;
    }else{
        DBG_UART->BAUD_DIV = result;
        g_ulHalDbgUart_CurrentBaudRate = u32Baud;
        return 0;
    }
}

/*************************************************************************
* FUNCTION:
*  Hal_DbgUart_BaudRateGet
*
* DESCRIPTION:
*   1. Get baur-rate.
*
* CALLS
*
* PARAMETERS
*
* RETURNS
*   Baud-rate
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_DbgUart_BaudRateGet_impl(void)
{
    return g_ulHalDbgUart_CurrentBaudRate;
}

/*************************************************************************
* FUNCTION:
*  Hal_DbgUart_Init
*
* DESCRIPTION:
*   1. dbg-uart initail function
*
* CALLS
*
* PARAMETERS
*   1. u32Baud: Baud-rate
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_DbgUart_Init_impl(uint32_t u32Baud)
{
    // Enable module
    Hal_Sys_ApsClkEn(1, APS_CLK_DBG_UART);
    
    // Reset dbg_uart
    Hal_Sys_ApsModuleRst(APS_RST_DBG_UART);

    // Only allowed support 8bit, no parity, one stop-bit (page 4-8)
    Hal_DbgUart_BaudRateSet(u32Baud);

    // Enable TX/RX and disable all interupts
    DBG_UART->CTRL = DBG_UART_CTRL_TX_EN | DBG_UART_CTRL_RX_EN;

    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_DbgUart_DataSendTimeOut
*
* DESCRIPTION:
*   1. Sent a data until time-out
*
* CALLS
*
* PARAMETERS
*   1. u32Data    : Value of data
*   2. u32MilliSec: Time-Out value. Recommand not over 10 sec (Estimate by 200Mhz system clock)
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_DbgUart_DataSendTimeOut_impl(uint32_t u32Data, uint32_t u32MilliSec)
{
    uint32_t u32TimeOutTick = 0;
    uint32_t u32TimeOutStart = 0;

    if(u32MilliSec >= DBG_UART_TIMEOUT_MS_MAX)
        u32TimeOutTick = DBG_UART_TIMEOUT_MS_MAX * Hal_Tick_PerMilliSec();
    else
        u32TimeOutTick = u32MilliSec * Hal_Tick_PerMilliSec();

    u32TimeOutStart = Hal_Tick_Diff(0);
    while(DBG_UART->STATE & DBG_UART_STATE_TX_FULL)
    {
        // loop when TX is full
        if(Hal_Tick_Diff(u32TimeOutStart) > u32TimeOutTick)
            return 1;
    }
    DBG_UART->DATA = (char)u32Data;
    DBG_UART->INT_STATUS |= DBG_UART_INT_STATUS_TX;
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_DbgUart_DataSend
*
* DESCRIPTION:
*   1. Sent a data until time-out
*
* CALLS
*
* PARAMETERS
*   1. u32Data: Value of data
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_DbgUart_DataSend_impl(uint32_t u32Data)
{
    uint32_t u32TimeOutCount = 0;

    while(DBG_UART->STATE & DBG_UART_STATE_TX_FULL)
    {
        // loop when TX is full
        if(u32TimeOutCount > DBG_UART_TIMEOUT_COUNT_MAX)
            return 1;
        u32TimeOutCount++;
    }
    DBG_UART->DATA = (char)u32Data;
    DBG_UART->INT_STATUS |= DBG_UART_INT_STATUS_TX;
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_DbgUart_DataRecv
*
* DESCRIPTION:
*   1. Receive a data until time-out 
*
* CALLS
*
* PARAMETERS
*   1. pu32Data  : Pinter of data
*   2. u32MilliSec: Time-Out value. Recommand not over 10 sec (Estimate by 200Mhz system clock)
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_DbgUart_DataRecvTimeOut_impl(uint32_t* pu32Data, uint32_t u32MilliSec)
{
    uint32_t u32TimeOutTick = 0;
    uint32_t u32TimeOutStart = 0;

    if(u32MilliSec >DBG_UART_TIMEOUT_MS_MAX)
        u32TimeOutTick = DBG_UART_TIMEOUT_MS_MAX * Hal_Tick_PerMilliSec();
    else
        u32TimeOutTick = u32MilliSec * Hal_Tick_PerMilliSec();

    u32TimeOutStart = Hal_Tick_Diff(0);
    while( (DBG_UART->STATE & DBG_UART_STATE_RX_FULL) != DBG_UART_STATE_RX_FULL )
    {
        // loop when rx is not full(ie. empty)
        if(Hal_Tick_Diff(u32TimeOutStart) > u32TimeOutTick)
            return 1;
    }
    *pu32Data = DBG_UART->DATA;
    DBG_UART->INT_STATUS |= DBG_UART_INT_STATUS_RX;
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_DbgUart_DataRecv
*
* DESCRIPTION:
*   1. Receive a data until time-out
*
* CALLS
*
* PARAMETERS
*   1. pu32Data: Pinter of data
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_DbgUart_DataRecv_impl(uint32_t* pu32Data)
{
    uint32_t u32TimeOutCount = 0;

    while( (DBG_UART->STATE & DBG_UART_STATE_RX_FULL) != DBG_UART_STATE_RX_FULL )
    {
        // loop when rx is not full(ie. empty)
        if(u32TimeOutCount > DBG_UART_TIMEOUT_COUNT_MAX)
            return 1;

        u32TimeOutCount++;
    }
    *pu32Data = DBG_UART->DATA;
    DBG_UART->INT_STATUS |= DBG_UART_INT_STATUS_RX;
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Dbg_Uart_RxCallBackFuncSet
*
* DESCRIPTION:
*   1. Setup rx callback function
*
* CALLS
*
* PARAMETERS
*   1. tFunc: Callback function
*
* RETURNS
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_DbgUart_RxCallBackFuncSet_impl(T_DbgUart_RxCallBack tFunc)
{
    DbgUart_RxCallBack = tFunc;
}


/*************************************************************************
* FUNCTION:
*  _Hal_DbgUart_FunPoiterInit
*
* DESCRIPTION:
*   1. Inital of functuin pointers
*
* CALLS
*
* PARAMETERS
*   None
*
* RETURNS
*   None
* GLOBALS AFFECTED
* 
*************************************************************************/
void _Hal_DbgUart_FunPoiterInit(void)
{
    g_ulHalDbgUart_CurrentBaudRate = DEFAULT_BAUD;
    g_ubHalDbgUart_RxIntEnStatus   = 0;
    
    Hal_DbgUart_RxIntEn           = Hal_DbgUart_RxIntEn_impl;
    Hal_DbgUart_RxIntEnStatusGet  = Hal_DbgUart_RxIntEnStatusGet_impl;
    Hal_DbgUart_IntStatusRead     = Hal_DbgUart_IntStatusRead_impl;
    Hal_DbgUart_IntClear          = Hal_DbgUart_IntClear_impl;
    Hal_DbgUart_BaudRateSet       = Hal_DbgUart_BaudRateSet_impl;
    Hal_DbgUart_BaudRateGet       = Hal_DbgUart_BaudRateGet_impl;
    Hal_DbgUart_Init              = Hal_DbgUart_Init_impl;
    Hal_DbgUart_RxCallBackFuncSet = Hal_DbgUart_RxCallBackFuncSet_impl;

    // Those function are used for general case, time-out with default value(1 ms)
    Hal_DbgUart_DataSend = Hal_DbgUart_DataSend_impl;
    Hal_DbgUart_DataRecv = Hal_DbgUart_DataRecv_impl;

    // Those functions are recommand used only for bootROM and ISR, time-out with the given value
    Hal_DbgUart_DataSendTimeOut = Hal_DbgUart_DataSendTimeOut_impl;
    Hal_DbgUart_DataRecvTimeOut = Hal_DbgUart_DataRecvTimeOut_impl;
}

/*************************************************************************
* FUNCTION:
*  Hal_DbgUart_ParamInit
*
* DESCRIPTION:
*   1. Inital of glabal parameters
*
* CALLS
*
* PARAMETERS
*   None
*
* RETURNS
*   None
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_DbgUart_ParamInit(void)
{
    DbgUart_RxCallBack = 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_DbgUart_Pre_Init
*
* DESCRIPTION:
*   1. Inital for functuin pointers and glabal parameters
*
* CALLS
*
* PARAMETERS
*   None
*
* RETURNS
*   None
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_DbgUart_Pre_Init(void)
{
    // Param
    Hal_DbgUart_ParamInit();

    // Function pointers
    _Hal_DbgUart_FunPoiterInit();
}

