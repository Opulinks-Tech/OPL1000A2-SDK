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
 * @file hal_uart_patch.c
 * 
 * @brief Patch for UART API patch
 *  
 *******************************************************************************/

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "hal_uart_patch.h"

/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */

#define UART_0              ((S_Uart_Reg_t *) UART0_BASE)
#define UART_1              ((S_Uart_Reg_t *) UART1_BASE)
#define UART_SRC_CLK        XTAL
#define DEFAULT_BAUD        115200

/*
 * baud rate:115200 -> 100 us per char, 
 * Max core 176Mhz -> 1ms = 176*1000 tick = 0x2AF80 => 0x30000
 */
#define UART_TIMEOUT_COUNT_MAX  ( 0x60000 )
#define UART_TIMEOUT_MS_MAX     ( Hal_Tick_MilliSecMax() )

#define UART_BAUD_DIV_MIN    0x1
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
/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */
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
/*
*************************************************************************
*                           Declarations of Private Functions
*************************************************************************
*/
uint32_t Hal_Uart_BaudRateSet_patch(E_UartIdx_t eUartIdx, uint32_t u32Baud);
/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */
extern T_HalUartConfig g_taHalUartCurrentConfig[UART_IDX_MAX];

S_Uart_Reg_t *g_Uart_psaUartReg[UART_IDX_MAX] = {UART_0, UART_1};
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

void Hal_Uart_PatchInit(void)
{
    Hal_Uart_BaudRateSet = Hal_Uart_BaudRateSet_patch;
}

/*
 *************************************************************************
 *                          Private Functions
 *************************************************************************
 */



/**
 * @brief Set baud rate.
 *
 *        UART0 and UART1 source clock is from XTAL.
 *        The baud rate is (UART source clock) / ( 16 * divisor)
 *        The divisor range is from 1 to 65535.
 *        If divisor is out of range from given baud rate, it will use 
 *        default baud rate to set divisor.
 * @param eUartIdx [in] The index of UART. refert to E_UartIdx_t
 *          UART_IDX_0 and UART_IDX_1
 * @param u32Baud [in] Baud-rate
 * @return Setting status
 * @retval 0 Success
 * @retval 1 Fail
 */
uint32_t Hal_Uart_BaudRateSet_patch(E_UartIdx_t eUartIdx, uint32_t u32Baud)
{
    S_Uart_Reg_t *psUart = 0;
    uint32_t u32Div = 0;
    uint32_t u32Ret = 0;

    if (eUartIdx >= UART_IDX_MAX)
        return 1;
    
    if (u32Baud == 0)
        return 1;
    
    psUart = g_Uart_psaUartReg[eUartIdx];
    
    // Caculate DIV
    // Formula: baud rate = (uart clk freq) / (16 * divisor), and round off it (page 92)
    u32Div = ( (UART_SRC_CLK >> 4) + (u32Baud >> 2) ) / u32Baud;
    if ( (u32Div < UART_BAUD_DIV_MIN) || (u32Div > UART_BAUD_DIV_MAX) )
    {   /* Invalid divider, set to default baud rate */
        u32Ret = 1;
        u32Div = ( (UART_SRC_CLK >> 4) + (DEFAULT_BAUD >> 2) ) / DEFAULT_BAUD;
    }
	
    // Latch to Div setting mode
    psUart->LCR |= UART_LCR_BAUD_DIV_LATCH;
	
    // Read DIV_L and DIV_H regs
    psUart->DATA = u32Div & UART_BAUD_DIV_L_MASK;
    psUart->INT_EN = (u32Div>>8) & UART_BAUD_DIV_H_MASK;
	
    // Un-latch
    psUart->LCR &= ~UART_LCR_BAUD_DIV_LATCH;

    g_taHalUartCurrentConfig[eUartIdx].ulBuadrate = u32Baud;
    return u32Ret;
}



