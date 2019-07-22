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
*  hal_uart.c
*
*  Project:
*  --------
*  OPL1000 series
*
*  Description:
*  ------------
*  This source file defines the functions of uart .
*  Ref. document is << DesignWare DW_apb_uart Databook >>
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
#include <string.h>
#include "opl1000.h"
#include "hal_tick.h"
#include "hal_system.h"
#include "hal_vic.h"
#include "hal_uart.h"
#include "mw_fim_default_group01.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define UART_0       ((S_Uart_Reg_t *) UART0_BASE)
#define UART_1       ((S_Uart_Reg_t *) UART1_BASE)
#define SYSTEM_CLK   XTAL
#define DEFAULT_BAUD 115200

/*
 * baud rate:115200 -> 100 us per char, 
 * Max core 176Mhz -> 1ms = 176*1000 tick = 0x2AF80 => 0x30000
 */
#define UART_TIMEOUT_COUNT_MAX  ( 0x60000 )
#define UART_TIMEOUT_MS_MAX     ( Hal_Tick_MilliSecMax() )

#define UART_BAUD_DIV_MAX    0xFFFF
#define UART_BAUD_DIV_L_MASK 0xFF
#define UART_BAUD_DIV_H_MASK 0xFF

#define UART_IER_PTIME_EN (1<<7)
#define UART_IER_EDSSI_EN (1<<3)
#define UART_IER_ELSI_EN  (1<<2)
#define UART_IER_ETBEI_EN (1<<1)
#define UART_IER_ERBFI_EN (1<<0)

#define UART_FCR_REVC_TRIG_ANY      (0<<6)
#define UART_FCR_REVC_TRIG_QUARTER  (1<<6)
#define UART_FCR_REVC_TRIG_HALF     (2<<6)
#define UART_FCR_REVC_TRIG_TWO_AVA  (3<<6)
#define UART_FCR_REVC_TRIG_MASK     (0x3<<6)
#define UART_FCR_EMPTY_TRIG_NONE    (0<<4)
#define UART_FCR_EMPTY_TRIG_TWO     (1<<4)
#define UART_FCR_EMPTY_TRIG_QUARTER (2<<4)
#define UART_FCR_EMPTY_TRIG_HALF    (3<<4)
#define UART_FCR_EMPTY_TRIG_MASK    (0x3<<4)
#define UART_FCR_XMIT_FIFO_RST      (1<<2)
#define UART_FCR_RECV_FIFO_RST      (1<<1)
#define UART_FCR_XMIT_FIFO_EN       (1<<0)

#define UART_IIR_INT_ID_MASK         0xF

#define UART_LCR_BAUD_DIV_LATCH  (1<<7)
#define UART_LCR_EVEN_PARITY     (1<<4)
#define UART_LCR_PARITY_EN       (1<<3)
#define UART_LCR_STOP_BIT        (1<<2)
#define UART_LCR_DATA_BITS_5     0
#define UART_LCR_DATA_BITS_6     1
#define UART_LCR_DATA_BITS_7     2
#define UART_LCR_DATA_BITS_8     3
#define UART_LCR_DATA_BITS_MASK  0x3

#define UART_LSR_XMIT_HOLD_EMPTY (1<<5)
#define UART_LSR_DATA_READY      (1<<0)

#define UART_MCR_RTS           (1<<1)
#define UART_MCR_AUTO_FLOW_CTL (1<<5)

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list¡K
typedef struct
{
    volatile uint32_t DATA;         //0x00, RBR(R) / THR(W) / DLL
    volatile uint32_t INT_EN;       //0x04, DLH / IER
    volatile uint32_t INT_STATUS;   //0x08, IIR(R) / FCR(W)
    volatile uint32_t LCR;          //0x0C, Line control
    volatile uint32_t MCR;          //0x10, Moden control
    volatile uint32_t LSR;          //0x14, Line status
    volatile uint32_t MSR;          //0x18, Moden status
    volatile uint32_t resv[24];     //0x1C ~ 0x78
    volatile uint32_t USR;          //0x7C UART status
} S_Uart_Reg_t;

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global  variable
RET_DATA T_Uart_RxCallBack            Uart_0_RxCallBack;
RET_DATA T_Uart_RxCallBack            Uart_1_RxCallBack;
RET_DATA T_HalUartConfig              g_taHalUartCurrentConfig[UART_IDX_MAX];

RET_DATA T_Hal_Uart_RxIntEn           Hal_Uart_RxIntEn;
RET_DATA T_Hal_Uart_IntIdRead         Hal_Uart_IntIdRead;
RET_DATA T_Hal_Uart_IntClear          Hal_Uart_IntClear;
RET_DATA T_Hal_Uart_BaudRateSet       Hal_Uart_BaudRateSet;
RET_DATA T_Hal_Uart_Init              Hal_Uart_Init;
RET_DATA T_Hal_Uart_ConfigGet         Hal_Uart_ConfigGet;
RET_DATA T_Hal_Uart_ConfigSet         Hal_Uart_ConfigSet;
RET_DATA T_Hal_Uart_RxCallBackFuncSet Hal_Uart_RxCallBackFuncSet;

// Those function are used for general case, time-out with default value(1 ms)
RET_DATA T_Hal_Uart_DataSend Hal_Uart_DataSend;
RET_DATA T_Hal_Uart_DataRecv Hal_Uart_DataRecv;

// Those functions are recommand used only for bootROM and ISR, time-out with the given value
RET_DATA T_Hal_Uart_DataSendTimeOut Hal_Uart_DataSendTimeOut;
RET_DATA T_Hal_Uart_DataRecvTimeOut Hal_Uart_DataRecvTimeOut;

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
*  Hal_Uart_RxIntEn
*
* DESCRIPTION:
*   1. Enable rx interrupts
*
* CALLS
*
* PARAMETERS
*   1. eUartIdx : The index of UART. refert to E_UartIdx_t
*   2. u8Enable : 1 for enable/0 for disable the interrupt
*
* RETURNS
*   0: setting complete
*   1: error
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Uart_RxIntEn_impl(E_UartIdx_t eUartIdx, uint8_t u8Enable)
{
    S_Uart_Reg_t *pUart = 0;
    IRQn_Type eIrq = IRQn_MAX;
    uint32_t u32IrqPriorty = 0xFF;

    // Check UART Index
    if(eUartIdx == UART_IDX_0)
    {
        pUart = UART_0;
        eIrq = UART0_IRQn;
        u32IrqPriorty = IRQ_PRIORITY_UART0;
    }
    else if(eUartIdx == UART_IDX_1)
    {
        pUart = UART_1;
        eIrq = UART1_IRQn;
        u32IrqPriorty = IRQ_PRIORITY_UART1;
    }
    else
    {
        return 1;
    }

    // Write IER, enable interrupt  
    if(u8Enable)
    {
        // VIC 1) Clear interrupt
        Hal_Vic_IntClear(eIrq);
        // VIC 2) Enable VIC
        Hal_Vic_IntEn(eIrq, 1);
        // VIC 3) un-Mask VIC
        Hal_Vic_IntMask(eIrq, 0);

        // NVIC 1) Clean NVIC
        NVIC_ClearPendingIRQ(eIrq);
        // NVIC 2) Enable NVIC
        NVIC_EnableIRQ(eIrq);
        // NVIC 3) Set prority
        NVIC_SetPriority(eIrq, u32IrqPriorty);
        
        // Module part: Enable module
        pUart->INT_EN |= UART_IER_ERBFI_EN;
    }else{
        // Module part: Disable module
        pUart->INT_EN &= ~UART_IER_ERBFI_EN;

        // VIC part: Disable VIC
        Hal_Vic_IntEn(eIrq, 0);

        // NVIC part: Disable NVIC
        NVIC_DisableIRQ(eIrq);
    }
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Uart_IntIdRead
*
* DESCRIPTION:
*   1. Get interrupt ID
*
* CALLS
*
* PARAMETERS
*   1. eUartIdx : The index of UART. refert to E_UartIdx_t
*
* RETURNS
*   Interrupt status. refer to E_UartIntId_t
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Uart_IntIdRead_impl(E_UartIdx_t eUartIdx)
{
    S_Uart_Reg_t *pUart = 0;
    uint32_t u32Result = 0XF;

    // Check UART Index
    if(eUartIdx == UART_IDX_0)
    {
        pUart = UART_0;
    }
    else if(eUartIdx == UART_IDX_1)
    {
        pUart = UART_1;
    }
    else
    {
        return u32Result;
    }

    return (pUart->INT_STATUS & UART_IIR_INT_ID_MASK);
}

/*************************************************************************
* FUNCTION:
*  Hal_Uart_IntClear
*
* DESCRIPTION:
*   1. Clear a interrupt
*
* CALLS
*
* PARAMETERS
*   1. eUartIdx : The index of UART. refert to E_UartIdx_t
*   2. eIntID   : Interrupt status. refer to E_UartIntId_t
*   3. pu32Buff : The pointer to return data.  
*                 The return data will diffrent according to eIntID.
*                     UART_INT_MODEM_STAT : MSR (0x018)
*                     UART_INT_NONE       : none
*                     UART_INT_TX_EMPTY   : IIR (0x008)
*                     UART_INT_RX_AVALIBLE: FIFO data
*                     UART_INT_RECV_LINE  : LSR (0x014)
*                     UART_INT_BUSY       : USR (0x07C)
*                     UART_INT_CHAR       : FIFO data
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Uart_IntClear_impl(E_UartIdx_t eUartIdx, E_UartIntId_t eIntID, uint32_t *pu32Buff)
{
    S_Uart_Reg_t *pUart = 0;

    // Check UART Index
    if(eUartIdx == UART_IDX_0)
    {
        pUart = UART_0;
    }
    else if(eUartIdx == UART_IDX_1)
    {
        pUart = UART_1;
    }
    else
    {
        return 1;
    }
    
    // Clean module parts
    switch(eIntID)
    {
        case UART_INT_MODEM_STAT:
                *pu32Buff = pUart->MSR;
            break;
        case UART_INT_NONE:
                // Do nothing...
            break;
        case UART_INT_TX_EMPTY:
                *pu32Buff = pUart->INT_STATUS;
            break;
        case UART_INT_RX_AVALIBLE:
                *pu32Buff = pUart->DATA;
            break;
        case UART_INT_RECV_LINE:
                *pu32Buff = pUart->LSR;
            break;
        case UART_INT_BUSY:
                *pu32Buff = pUart->USR;
            break;
        case UART_INT_CHAR_TIMEOUT:
                *pu32Buff = pUart->DATA;
            break;
        default:
            return 1;
    }

    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Uart_BaudRateSet
*
* DESCRIPTION:
*   1. Set baur-rate
*
* CALLS
*
* PARAMETERS
*   1. eUartIdx: The index of UART. refert to E_UartIdx_t
*   2. u32Baud : Baud-rate
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Uart_BaudRateSet_impl(E_UartIdx_t eUartIdx, uint32_t u32Baud)
{
    S_Uart_Reg_t *pUart = 0;
    uint32_t u32Div = 0;

    // Check UART Index
    if(eUartIdx == UART_IDX_0)
    {
        pUart = UART_0;
    }
    else if(eUartIdx == UART_IDX_1)
    {
        pUart = UART_1;
    }
    else
    {
        return 1;
    }
	
    // Caculate DIV
    // Fomula: baud rate = (uart clk freq) / (16 * divisor), and round off it (page 92)
    u32Div = ( (SYSTEM_CLK >>4 ) + (u32Baud >> 2) ) / u32Baud;
    if(u32Div > UART_BAUD_DIV_MAX)
        return 1;
	
    // Latch to Div setting mode
    pUart->LCR |= UART_LCR_BAUD_DIV_LATCH;
	
    // Read DIV_L and DIV_H regs
    pUart->DATA = u32Div & UART_BAUD_DIV_L_MASK;
    pUart->INT_EN = (u32Div>>8) & UART_BAUD_DIV_H_MASK;
	
    // Un-latch
    pUart->LCR &= ~UART_LCR_BAUD_DIV_LATCH;

    g_taHalUartCurrentConfig[eUartIdx].ulBuadrate = u32Baud;
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Uart_Init
*
* DESCRIPTION:
*   1. uart initail function
*
* CALLS
*
* PARAMETERS
*   1. eUartIdx   : The index of UART. refert to E_UartIdx_t
*   2. u32Baud    : Baud-rate
*   3. u8Bits     : Data bits. refert to E_UartDataBit_t
*   4. u8Parity   : Parity mode. refer to E_UartParity_t
*   5. u8StopBit  : Stop bits. refert to E_UartStopBit_t 
*   6. u8EnFlowCtl: Enable/Disable CTS and RTS. 1 for enable/0 for disable 
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Uart_Init_impl(E_UartIdx_t eUartIdx, uint32_t u32Baud, E_UartDataBit_t u8Bits, E_UartParity_t u8Parity, E_UartStopBit_t u8StopBit, uint8_t u8EnFlowCtl)
{
    S_Uart_Reg_t *pUart = 0;
    T_HalUartConfig tConfig;

    // Check UART Index
    if(eUartIdx == UART_IDX_0)
    {
        pUart = UART_0;
    }
    else if(eUartIdx == UART_IDX_1)
    {
        pUart = UART_1;
    }
    else
    {
        return 1;
    }

    // Check StopBit setting
    if(u8StopBit == STOP_BIT_1P5)
    {
        // Only Bits = 5 can set StopBit =1.5
        if(u8Bits != DATA_BIT_5)
            return 1;
    }
	
    // Reset uart
    if(eUartIdx  == UART_IDX_0)
    {
        // Enable module
        Hal_Sys_ApsClkEn(1, APS_CLK_UART_0);
        // Reset module
        Hal_Sys_ApsModuleRst(APS_RST_UART_0);
    }else if(eUartIdx == UART_IDX_1){
        // Enable module
        Hal_Sys_ApsClkEn(1, APS_CLK_UART_1);
        // Reset module
        Hal_Sys_ApsModuleRst(APS_RST_UART_1);
    }
	
    tConfig.ulBuadrate = u32Baud;
    tConfig.ubDataBit = u8Bits;
    tConfig.ubStopBit = u8StopBit;
    tConfig.ubParity = u8Parity;
    tConfig.ubFlowCtrl = u8EnFlowCtl;
    if (0 != Hal_Uart_ConfigSet(eUartIdx, &tConfig))
        return 1;
	
    // Write FCR
    pUart->INT_STATUS = UART_FCR_REVC_TRIG_ANY | //UART_FCR_REVC_TRIG_HALF |
                        UART_FCR_EMPTY_TRIG_QUARTER |
                        UART_FCR_XMIT_FIFO_EN;

    // Write IER, disable all interrupts
    pUart->INT_EN = 0;
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Uart_ConfigGet
*
* DESCRIPTION:
*   1. get the current config of UART
*
* CALLS
*
* PARAMETERS
*   1. eUartIdx    : The index of UART. refert to E_UartIdx_t
*   2. ptConfig    : the config of UART
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Uart_ConfigGet_impl(E_UartIdx_t eUartIdx, T_HalUartConfig *ptConfig)
{
    // Check UART Index
    if (eUartIdx >= UART_IDX_MAX)
        return 1;
    
    // get the current config of UART
    memcpy(ptConfig, &(g_taHalUartCurrentConfig[eUartIdx]), sizeof(T_HalUartConfig));
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Uart_ConfigSet
*
* DESCRIPTION:
*   1. set the current config of UART
*
* CALLS
*
* PARAMETERS
*   1. eUartIdx    : The index of UART. refert to E_UartIdx_t
*   2. ptConfig    : the config of UART
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Uart_ConfigSet_impl(E_UartIdx_t eUartIdx, T_HalUartConfig *ptConfig)
{
    S_Uart_Reg_t *pUart = 0;
    uint32_t u32Temp = 0;

    // Check UART Index
    if(eUartIdx == UART_IDX_0)
    {
        pUart = UART_0;
    }
    else if(eUartIdx == UART_IDX_1)
    {
        pUart = UART_1;
    }
    else
    {
        return 1;
    }

    // Check StopBit setting
    if(ptConfig->ubStopBit == STOP_BIT_1P5)
    {
        // Only Bits = 5 can set StopBit =1.5
        if(ptConfig->ubDataBit != DATA_BIT_5)
            return 1;
    }
	
    // sequence ref to Page.134
    // Write MCR
    if(ptConfig->ubFlowCtrl)
        pUart->MCR = UART_MCR_AUTO_FLOW_CTL | UART_MCR_RTS;
    else
        pUart->MCR = 0;
	
    // Write divider ( latch is contained ) 
    Hal_Uart_BaudRateSet(eUartIdx, ptConfig->ulBuadrate);
	
    // Write LCR
    if(ptConfig->ubParity == PARITY_EVEN)
    {
        u32Temp |= UART_LCR_PARITY_EN | 
                   UART_LCR_EVEN_PARITY;
    }else if(ptConfig->ubParity == PARITY_ODD){
        u32Temp |= UART_LCR_PARITY_EN;
    }
    if(ptConfig->ubStopBit != STOP_BIT_1)
        u32Temp |= UART_LCR_STOP_BIT;
    u32Temp |= ptConfig->ubDataBit;
    pUart->LCR = u32Temp;
		
    // save the current config
    memcpy(&(g_taHalUartCurrentConfig[eUartIdx]), ptConfig, sizeof(T_HalUartConfig));
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Uart_DataSendTimeOut
*
* DESCRIPTION:
*   1. Sent a data
*
* CALLS
*
* PARAMETERS
*   1. eUartIdx    : Index of UART. refert to E_UartIdx_t
*   2. u32Data    : Value of data
*   3. u32MilliSec: Time-Out value. Recommand not over 10 sec (Estimate by 200Mhz system clock)
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Uart_DataSendTimeOut_impl(E_UartIdx_t eUartIdx, uint32_t u32Data, uint32_t u32MilliSec)
{
    S_Uart_Reg_t *pUart = 0;
    uint32_t u32TimeOutTick = 0;
    uint32_t u32TimeOutStart = 0;

    if(u32MilliSec >= UART_TIMEOUT_MS_MAX)
        u32TimeOutTick = UART_TIMEOUT_MS_MAX * Hal_Tick_PerMilliSec();
    else
        u32TimeOutTick = u32MilliSec * Hal_Tick_PerMilliSec();
  
    if(eUartIdx == UART_IDX_0)
    {
        pUart = UART_0;
    }
    else if(eUartIdx == UART_IDX_1)
    {
        pUart = UART_1;
    }
    else
    {
        return 1;
    }
	
    pUart->DATA = u32Data;

    u32TimeOutStart = Hal_Tick_Diff(0);
    // Refert to Page 134, not work...
    //while( (pUart->INT_STATUS & UART_IIR_INT_ID_MASK) != UART_IIR_INT_ID_THR_EMPTY)
    while( !(pUart->LSR & UART_LSR_XMIT_HOLD_EMPTY) )
    {
        // loop when TX is not empty
        if(Hal_Tick_Diff(u32TimeOutStart) > u32TimeOutTick)
            return 1;
    }
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Uart_DataSend
*
* DESCRIPTION:
*   1. Sent a data
*
* CALLS
*
* PARAMETERS
*   1. eUartIdx: Index of UART. refert to E_UartIdx_t
*   2. u32Data: Value of data
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Uart_DataSend_impl(E_UartIdx_t eUartIdx, uint32_t u32Data)
{
    S_Uart_Reg_t *pUart = 0;
    uint32_t u32TimeOutCount = 0;
  
    if(eUartIdx == UART_IDX_0)
    {
        pUart = UART_0;
    }
    else if(eUartIdx == UART_IDX_1)
    {
        pUart = UART_1;
    }
    else
    {
        return 1;
    }
	
    pUart->DATA = u32Data;

    // Refert to Page 134, not work...
    //while( (pUart->INT_STATUS & UART_IIR_INT_ID_MASK) != UART_IIR_INT_ID_THR_EMPTY)
    while( !(pUart->LSR & UART_LSR_XMIT_HOLD_EMPTY) )
    {
        // loop when TX is not empty
        if(u32TimeOutCount > UART_TIMEOUT_COUNT_MAX)
            return 1;
        u32TimeOutCount++;
    }
    return 0;

}

/*************************************************************************
* FUNCTION:
*  Hal_Uart_DataRecvTimeOut
*
* DESCRIPTION:
*   1. Receive a data
*
* CALLS
*
* PARAMETERS
*   1. eUartIdx: index of UART. refert to E_UartIdx_t
*   2. pu32Data: pointer of data
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Uart_DataRecvTimeOut_impl(E_UartIdx_t eUartIdx, uint32_t* pu32Data, uint32_t u32MilliSec)
{
    S_Uart_Reg_t *pUart = 0;
    uint32_t u32TimeOutTick = 0;
    uint32_t u32TimeOutStart = 0;

    if(u32MilliSec >= UART_TIMEOUT_MS_MAX)
        u32TimeOutTick = UART_TIMEOUT_MS_MAX * Hal_Tick_PerMilliSec();
    else
        u32TimeOutTick = u32MilliSec * Hal_Tick_PerMilliSec();

    if(eUartIdx == UART_IDX_0)
    {
        pUart = UART_0;
    }
    else if(eUartIdx == UART_IDX_1)
    {
        pUart = UART_1;
    }
    else
    {
        return 1;
    }

    u32TimeOutStart = Hal_Tick_Diff(0);
    // Refert to Page 135,, not work...
    //while( (pUart->INT_STATUS & UART_IIR_INT_ID_MASK) != UART_IIR_INT_ID_RX_DATA)
    while( !(pUart->LSR & UART_LSR_DATA_READY) )
    {
        // loop when RX is empty
        if(Hal_Tick_Diff(u32TimeOutStart) > u32TimeOutTick)
            return 1;
    }

    *pu32Data = pUart->DATA;
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Uart_DataRecv
*
* DESCRIPTION:
*   1. Receive a data
*
* CALLS
*
* PARAMETERS
*   1. eUartIdx: index of UART. refert to E_UartIdx_t
*   2. pu32Data: pointer of data
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Uart_DataRecv_impl(E_UartIdx_t eUartIdx, uint32_t* pu32Data)
{
    S_Uart_Reg_t *pUart = 0;
    uint32_t u32TimeOutCount = 0;

    if(eUartIdx == UART_IDX_0)
    {
        pUart = UART_0;
    }
    else if(eUartIdx == UART_IDX_1)
    {
        pUart = UART_1;
    }
    else
    {
        return 1;
    }

    // Refert to Page 135,, not work...
    //while( (pUart->INT_STATUS & UART_IIR_INT_ID_MASK) != UART_IIR_INT_ID_RX_DATA)
    while( !(pUart->LSR & UART_LSR_DATA_READY) )
    {
        // loop when RX is empty
        if(u32TimeOutCount > UART_TIMEOUT_COUNT_MAX)
            return 1;
        u32TimeOutCount++;
    }

    *pu32Data = pUart->DATA;
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Uart_RxCallBackFuncSet
*
* DESCRIPTION:
*   1. Setup rx callback function
*
* CALLS
*
* PARAMETERS
*   1. eUartIdx: index of UART. refert to E_UartIdx_t
*   2. tFunc: Callback function
*
* RETURNS
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Uart_RxCallBackFuncSet_impl(E_UartIdx_t eUartIdx, T_Uart_RxCallBack tFunc)
{
    if(eUartIdx == UART_IDX_0)
    {
        Uart_0_RxCallBack = tFunc;
    }
    else if(eUartIdx == UART_IDX_1)
    {
        Uart_1_RxCallBack = tFunc;
    }
    else
    {
        return;
    }
}

/*************************************************************************
* FUNCTION:
*  Hal_Uart_Pre_Init
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
void Hal_Uart_Pre_Init(void)
{
    Uart_0_RxCallBack = 0;
    Uart_1_RxCallBack = 0;
    memcpy(&(g_taHalUartCurrentConfig[UART_IDX_0]), &g_tMwFimDefaultUartConfig, sizeof(T_HalUartConfig));
    memcpy(&(g_taHalUartCurrentConfig[UART_IDX_1]), &g_tMwFimDefaultUartConfig, sizeof(T_HalUartConfig));

    Hal_Uart_RxIntEn           = Hal_Uart_RxIntEn_impl;
    Hal_Uart_IntIdRead         = Hal_Uart_IntIdRead_impl;
    Hal_Uart_IntClear          = Hal_Uart_IntClear_impl;
    Hal_Uart_BaudRateSet       = Hal_Uart_BaudRateSet_impl;
    Hal_Uart_Init              = Hal_Uart_Init_impl;
    Hal_Uart_ConfigGet         = Hal_Uart_ConfigGet_impl;
    Hal_Uart_ConfigSet         = Hal_Uart_ConfigSet_impl;
    Hal_Uart_DataSend          = Hal_Uart_DataSend_impl;
    Hal_Uart_DataRecv          = Hal_Uart_DataRecv_impl;
    Hal_Uart_RxCallBackFuncSet = Hal_Uart_RxCallBackFuncSet_impl;

    // Those function are used for general case, time-out with default value(1 ms)
    Hal_Uart_DataSend = Hal_Uart_DataSend_impl;
    Hal_Uart_DataRecv = Hal_Uart_DataRecv_impl;

    // Those functions are recommand used only for bootROM and ISR, time-out with the given value
    Hal_Uart_DataSendTimeOut = Hal_Uart_DataSendTimeOut_impl;
    Hal_Uart_DataRecvTimeOut = Hal_Uart_DataRecvTimeOut_impl;
}

