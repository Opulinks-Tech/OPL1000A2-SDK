/******************************************************************************
*  Copyright 2017, Netlink Communication Corp.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Netlink Communication Corp. (C) 2017
******************************************************************************/

/******************************************************************************
*  Filename:
*  ---------
*  hal_pin_def.h
*
*  Project:
*  --------
*  NL1000_A1 series
*
*  Description:
*  ------------
*  This include file defines the patch proto-types of system functions
*  Include sys-reg and AOS domains.
*
*  Author:
*  -------
*  Jeff
******************************************************************************/

#ifndef __HAL_PIN_DEF_H__
#define __HAL_PIN_DEF_H__

#ifdef __cplusplus
extern "C" {
#endif

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
// PIN type
typedef enum
{
    PIN_TYPE_NONE = 0,
    
    // UART
    PIN_TYPE_UART0_TX,
    PIN_TYPE_UART0_RX,
    PIN_TYPE_UART0_RTS,
    PIN_TYPE_UART0_CTS,
    
    PIN_TYPE_UART1_TX,
    PIN_TYPE_UART1_RX,
    PIN_TYPE_UART1_RTS,
    PIN_TYPE_UART1_CTS,
    
    PIN_TYPE_UART_APS_TX,
    PIN_TYPE_UART_APS_RX,
    
    PIN_TYPE_UART_MSQ_TX,
    PIN_TYPE_UART_MSQ_RX,
    
    // SPI
    PIN_TYPE_SPI0_CS,
    PIN_TYPE_SPI0_CLK,
    PIN_TYPE_SPI0_IO_0,
    PIN_TYPE_SPI0_IO_1,
    PIN_TYPE_SPI0_IO_2,
    PIN_TYPE_SPI0_IO_3,
    PIN_TYPE_SPI0_IO_MOSI,
    PIN_TYPE_SPI0_IO_MISO,
    
    PIN_TYPE_SPI1_CS,
    PIN_TYPE_SPI1_CLK,
    PIN_TYPE_SPI1_IO_0,
    PIN_TYPE_SPI1_IO_1,
    PIN_TYPE_SPI1_IO_2,
    PIN_TYPE_SPI1_IO_3,
    PIN_TYPE_SPI1_IO_MOSI,
    PIN_TYPE_SPI1_IO_MISO,
    
    PIN_TYPE_SPI2_CS,
    PIN_TYPE_SPI2_CLK,
    PIN_TYPE_SPI2_IO_0,
    PIN_TYPE_SPI2_IO_1,
    PIN_TYPE_SPI2_IO_2,
    PIN_TYPE_SPI2_IO_3,
    PIN_TYPE_SPI2_IO_MOSI,
    PIN_TYPE_SPI2_IO_MISO,
    
    // I2C
    PIN_TYPE_I2C_SCL,
    PIN_TYPE_I2C_SDA,
    
    // PWM
    PIN_TYPE_PWM_0,
    PIN_TYPE_PWM_1,
    PIN_TYPE_PWM_2,
    PIN_TYPE_PWM_3,
    PIN_TYPE_PWM_4,
    PIN_TYPE_PWM_5,
    
    // ICE
    PIN_TYPE_ICE_M3_CLK,
    PIN_TYPE_ICE_M3_DAT,
    
    PIN_TYPE_ICE_M0_CLK,
    PIN_TYPE_ICE_M0_DAT,
    
    // GPIO
    PIN_TYPE_GPIO_INPUT,
    PIN_TYPE_GPIO_OUTPUT_LOW,       // the default output is low
    PIN_TYPE_GPIO_OUTPUT_HIGH,      // the default output is high
    
    // AUX
    PIN_TYPE_AUX_0,
    PIN_TYPE_AUX_1,
    PIN_TYPE_AUX_2,
    PIN_TYPE_AUX_3,
    PIN_TYPE_AUX_4,
    PIN_TYPE_AUX_5,
    PIN_TYPE_AUX_6,
    PIN_TYPE_AUX_7,
    PIN_TYPE_AUX_8,
    PIN_TYPE_AUX_9,
    PIN_TYPE_AUX_10,
    PIN_TYPE_AUX_11,
    PIN_TYPE_AUX_12,
    PIN_TYPE_AUX_13,
    PIN_TYPE_AUX_14,
    PIN_TYPE_AUX_15,

    PIN_TYPE_MAX
} E_PIN_TYPE;

// PIN driving
typedef enum
{
    PIN_DRIVING_FLOAT = 0,      // floating
    
    PIN_DRIVING_LOW,            // pull low
    PIN_DRIVING_HIGH,           // pull high
    
    PIN_DRIVING_MAX
} E_PIN_DRIVING;



/* Uart mode for switching UART */
typedef enum {
    IO01_UART_MODE_AT=0,
    IO01_UART_MODE_DBG,
}E_IO01_UART_MODE;

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

#ifdef __cplusplus
}
#endif

#endif  // end of __HAL_PIN_DEF_H__
