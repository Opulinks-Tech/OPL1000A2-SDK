/* *****************************************************************************
 *  Copyright 2017 - 2018, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2018
 *
 *******************************************************************************
 * @file hal_dbg_uart_patch.c
 * 
 * @brief Patch for APS DBG_UART API patch
 *  
 *******************************************************************************/

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "hal_dbg_uart_patch.h"
#include "hal_system_patch.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */

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
/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */
typedef struct
{
    volatile uint32_t DATA;         //0x00
    volatile uint32_t STATE;        //0x04
    volatile uint32_t CTRL;         //0x08
    volatile uint32_t INT_STATUS;   //0x0C
    volatile uint32_t BAUD_DIV;     //0x10
} S_DbgUart_Reg_t;
/*
*************************************************************************
*                           Declarations of Private Functions
*************************************************************************
*/
uint32_t Hal_DbgUart_BaudRateSet_patch(uint32_t u32Baud);
uint32_t Hal_DbgUart_BaudRateGet_patch(void);

/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */

extern uint32_t g_ulHalDbgUart_CurrentBaudRate; /* Baud rate from API */

/*
 *************************************************************************
 *                          Private Variables
 *************************************************************************
 */
 

/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */

void Hal_DbgUart_PatchInit(void)
{
    Hal_DbgUart_BaudRateSet = Hal_DbgUart_BaudRateSet_patch;
    Hal_DbgUart_BaudRateGet = Hal_DbgUart_BaudRateGet_patch;
}

/**
 * @brief Update UART divider.
 *        When UART clock source changed, call this function to update UART divider.
 *        This function will use previous UART setting baud rate to update divider.
 */
void Hal_DbgUart_DividerUpdate(void)
{
    Hal_DbgUart_BaudRateSet_patch(g_ulHalDbgUart_CurrentBaudRate);
}

/*
 *************************************************************************
 *                          Private Functions
 *************************************************************************
 */


/**
 * @brief Set DBG_UART baud rate
 *        DBG_UART baud rate formula = APS_PCLK / DIV, and valid DIV is between 16 and 1048575(0xFFFFF).
 *        If the is not in valid range, it will use default baud rate to set UART.
 *        The working baud rate might no equal to target baud rate.
 * @param u32Baud [in] Target baud rate
 * @return Setting status
 * @retval 0 Success
 * @retval 1 Error
 */
uint32_t Hal_DbgUart_BaudRateSet_patch(uint32_t u32Baud)
{
    uint32_t u32Div;
    uint32_t u32Pclk;

    if (u32Baud == 0)
        return 1;
    
    u32Pclk = Hal_Sys_ApsPclkGet();
    u32Div = ( u32Pclk + u32Baud/2 ) / u32Baud;
    g_ulHalDbgUart_CurrentBaudRate = u32Baud;
    if( (u32Div < DBG_UART_BAUD_DIV_MIN) || (u32Div > DBG_UART_BAUD_DIV_MAX) )
    {   /* Invalid divider range */
        DBG_UART->BAUD_DIV = (u32Pclk + DEFAULT_BAUD/2) / DEFAULT_BAUD;
        return 1;
    }
    else
    {
        DBG_UART->BAUD_DIV = u32Div;
        return 0;
    }
}

/**
 * @brief Get DBG_UART working baud rate
 *        It will return real working baud.
 *        The DBG_UART baud is APS_PCLK / UART_DIV.
 *        This value doesn't equal to the setting baud rate.
 * @return Working baud rate
 */
uint32_t Hal_DbgUart_BaudRateGet_patch(void)
{
    if (DBG_UART->BAUD_DIV)
        return Hal_Sys_ApsPclkGet() / DBG_UART->BAUD_DIV;
    return 0;
}




