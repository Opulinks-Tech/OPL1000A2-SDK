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

#include "Hal_pinmux_uart.h"



typedef struct
{
    E_OPLXXX_Periph tx;
    E_OPLXXX_Periph rx;
    E_OPLXXX_Periph cts;
    E_OPLXXX_Periph rts;
}T_Uart_Map;

T_Uart_Map uart_map[]=
{
    {UART0_TX_E,UART0_RX_E,UART0_CTS_E,UART0_RTS_E},
    {UART1_TX_E,UART1_RX_E,UART1_CTS_E,UART1_RTS_E},
};

ERR_CODE Hal_PinMux_Uart_Init(T_OPL1000_uart *uart)
{
    // Initial UART 0/1 module
    if(uart->index == UART_IDX_0 || uart->index == UART_IDX_1)
    {
        HAL_PINMUX_PRINT("uart %d init \r\n",uart->index);
        //uart parament init
        Hal_Uart_Init(
                uart->index,
                uart->baudrate,
                uart->bits,
                uart->parity,
                uart->stopBit,
                uart->flowCtl
        );
        // IO init
        Hal_PinMux_OutPutConfig(Hal_Get_Point(uart->tx,uart_map[uart->index].tx));
        Hal_PinMux_InPutConfig(Hal_Get_Point(uart->rx,uart_map[uart->index].rx));
        Hal_PinMux_OutPutClose(Hal_Get_Point(uart->rx,uart_map[uart->index].rx));
        
        if(uart->flowCtl == UART_FLOW)
        {
            HAL_PINMUX_PRINT("uart set flow \r\n");
            Hal_PinMux_OutPutConfig(Hal_Get_Point(uart->rts,uart_map[uart->index].rts));
            Hal_PinMux_InPutConfig(Hal_Get_Point(uart->cts,uart_map[uart->index].cts));
            Hal_PinMux_OutPutClose(Hal_Get_Point(uart->cts,uart_map[uart->index].cts));
        }
    }
    else
    {
        HAL_PINMUX_PRINT("Invalid UART index");
        return RET_FAIL;
    }
    return RET_SUCCESS;
}


