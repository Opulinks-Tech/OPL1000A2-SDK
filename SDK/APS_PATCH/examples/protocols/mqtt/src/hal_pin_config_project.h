/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
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
*  hal_pin_config_project.h
*
*  Project:
*  --------
*  OPL1000 Project - the pin config definition file
*
*  Description:
*  ------------
*  This include file is the pin config definition file
*
*  Author:
*  -------
*  Jeff
******************************************************************************/

#ifndef __HAL_PIN_CONFIG_PROJECT_H__
#define __HAL_PIN_CONFIG_PROJECT_H__

#ifdef __cplusplus
extern "C" {
#endif

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File
#include <stdint.h>
#include "hal_pin_def.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
/* Select IO 0/1 UART mode. For switching UART use */
#define HAL_PIN_0_1_UART_MODE       IO01_UART_MODE_AT       // IO01_UART_MODE_AT
                                                            // IO01_UART_MODE_DBG
// IO type select
#define HAL_PIN_TYPE_IO_0   PIN_TYPE_UART1_TX       // PIN_TYPE_NONE
                                                    // PIN_TYPE_GPIO_INPUT
                                                    // PIN_TYPE_GPIO_OUTPUT_LOW
                                                    // PIN_TYPE_GPIO_OUTPUT_HIGH
                                                    // PIN_TYPE_UART0_CTS
                                                    // PIN_TYPE_UART1_TX
                                                    // PIN_TYPE_I2C_SCL
                                                    // PIN_TYPE_SPI2_IO_3
                                                    // PIN_TYPE_AUX_0
                                                    // PIN_TYPE_UART_APS_TX
                                                    // PIN_TYPE_UART_MSQ_RX
                                                    // PIN_TYPE_ICE_M3_DAT
                                                    // PIN_TYPE_ICE_M0_CLK

#define HAL_PIN_TYPE_IO_1   PIN_TYPE_UART1_RX       // PIN_TYPE_NONE
                                                    // PIN_TYPE_GPIO_INPUT
                                                    // PIN_TYPE_GPIO_OUTPUT_LOW
                                                    // PIN_TYPE_GPIO_OUTPUT_HIGH
                                                    // PIN_TYPE_UART0_RTS
                                                    // PIN_TYPE_UART1_RX
                                                    // PIN_TYPE_I2C_SDA
                                                    // PIN_TYPE_SPI2_IO_2
                                                    // PIN_TYPE_AUX_1
                                                    // PIN_TYPE_UART_APS_RX
                                                    // PIN_TYPE_UART_MSQ_TX
                                                    // PIN_TYPE_ICE_M3_CLK
                                                    // PIN_TYPE_ICE_M0_DAT

#define HAL_PIN_TYPE_IO_2   PIN_TYPE_NONE           // PIN_TYPE_NONE
                                                    // PIN_TYPE_GPIO_INPUT
                                                    // PIN_TYPE_GPIO_OUTPUT_LOW
                                                    // PIN_TYPE_GPIO_OUTPUT_HIGH
                                                    // PIN_TYPE_UART0_TX
                                                    // PIN_TYPE_I2C_SDA
                                                    // PIN_TYPE_SPI2_IO_1
                                                    // PIN_TYPE_SPI2_IO_MISO
                                                    // PIN_TYPE_AUX_2
                                                    // PIN_TYPE_UART_APS_TX
                                                    // PIN_TYPE_UART_MSQ_RX
                                                    // PIN_TYPE_ICE_M3_DAT
                                                    // PIN_TYPE_ICE_M0_CLK

#define HAL_PIN_TYPE_IO_3   PIN_TYPE_NONE           // PIN_TYPE_NONE
                                                    // PIN_TYPE_GPIO_INPUT
                                                    // PIN_TYPE_GPIO_OUTPUT_LOW
                                                    // PIN_TYPE_GPIO_OUTPUT_HIGH
                                                    // PIN_TYPE_UART0_RX
                                                    // PIN_TYPE_I2C_SCL
                                                    // PIN_TYPE_SPI2_IO_0
                                                    // PIN_TYPE_SPI2_IO_MOSI
                                                    // PIN_TYPE_AUX_3
                                                    // PIN_TYPE_UART_APS_RX
                                                    // PIN_TYPE_UART_MSQ_TX
                                                    // PIN_TYPE_ICE_M3_CLK
                                                    // PIN_TYPE_ICE_M0_DAT

#define HAL_PIN_TYPE_IO_4   PIN_TYPE_NONE           // PIN_TYPE_NONE
                                                    // PIN_TYPE_GPIO_INPUT
                                                    // PIN_TYPE_GPIO_OUTPUT_LOW
                                                    // PIN_TYPE_GPIO_OUTPUT_HIGH
                                                    // PIN_TYPE_UART1_TX
                                                    // PIN_TYPE_I2C_SCL
                                                    // PIN_TYPE_SPI2_CLK
                                                    // PIN_TYPE_AUX_4
                                                    // PIN_TYPE_UART_APS_TX
                                                    // PIN_TYPE_UART_MSQ_RX
                                                    // PIN_TYPE_ICE_M3_DAT
                                                    // PIN_TYPE_ICE_M0_CLK

#define HAL_PIN_TYPE_IO_5   PIN_TYPE_NONE           // PIN_TYPE_NONE
                                                    // PIN_TYPE_GPIO_INPUT
                                                    // PIN_TYPE_GPIO_OUTPUT_LOW
                                                    // PIN_TYPE_GPIO_OUTPUT_HIGH
                                                    // PIN_TYPE_UART1_RX
                                                    // PIN_TYPE_I2C_SDA
                                                    // PIN_TYPE_SPI2_CS
                                                    // PIN_TYPE_AUX_5
                                                    // PIN_TYPE_UART_APS_RX
                                                    // PIN_TYPE_UART_MSQ_TX
                                                    // PIN_TYPE_ICE_M3_CLK
                                                    // PIN_TYPE_ICE_M0_DAT

#define HAL_PIN_TYPE_IO_6   PIN_TYPE_NONE           // PIN_TYPE_NONE
                                                    // PIN_TYPE_GPIO_INPUT
                                                    // PIN_TYPE_GPIO_OUTPUT_LOW
                                                    // PIN_TYPE_GPIO_OUTPUT_HIGH
                                                    // PIN_TYPE_UART0_TX
                                                    // PIN_TYPE_UART1_RTS
                                                    // PIN_TYPE_I2C_SCL
                                                    // PIN_TYPE_SPI1_CS
                                                    // PIN_TYPE_AUX_6
                                                    // PIN_TYPE_UART_APS_TX
                                                    // PIN_TYPE_UART_MSQ_RX
                                                    // PIN_TYPE_ICE_M3_DAT
                                                    // PIN_TYPE_ICE_M0_CLK

#define HAL_PIN_TYPE_IO_7   PIN_TYPE_NONE           // PIN_TYPE_NONE
                                                    // PIN_TYPE_GPIO_INPUT
                                                    // PIN_TYPE_GPIO_OUTPUT_LOW
                                                    // PIN_TYPE_GPIO_OUTPUT_HIGH
                                                    // PIN_TYPE_UART0_RX
                                                    // PIN_TYPE_UART1_CTS
                                                    // PIN_TYPE_I2C_SDA
                                                    // PIN_TYPE_SPI1_CLK
                                                    // PIN_TYPE_AUX_7
                                                    // PIN_TYPE_UART_APS_RX
                                                    // PIN_TYPE_UART_MSQ_TX
                                                    // PIN_TYPE_ICE_M3_CLK
                                                    // PIN_TYPE_ICE_M0_DAT

#define HAL_PIN_TYPE_IO_8   PIN_TYPE_UART_APS_TX    // PIN_TYPE_NONE
                                                    // PIN_TYPE_GPIO_INPUT
                                                    // PIN_TYPE_GPIO_OUTPUT_LOW
                                                    // PIN_TYPE_GPIO_OUTPUT_HIGH
                                                    // PIN_TYPE_UART0_CTS
                                                    // PIN_TYPE_UART1_TX
                                                    // PIN_TYPE_I2C_SDA
                                                    // PIN_TYPE_SPI1_IO_0
                                                    // PIN_TYPE_SPI1_IO_MOSI
                                                    // PIN_TYPE_AUX_8
                                                    // PIN_TYPE_UART_APS_TX
                                                    // PIN_TYPE_UART_MSQ_RX
                                                    // PIN_TYPE_ICE_M3_DAT
                                                    // PIN_TYPE_ICE_M0_CLK

#define HAL_PIN_TYPE_IO_9   PIN_TYPE_UART_APS_RX    // PIN_TYPE_NONE
                                                    // PIN_TYPE_GPIO_INPUT
                                                    // PIN_TYPE_GPIO_OUTPUT_LOW
                                                    // PIN_TYPE_GPIO_OUTPUT_HIGH
                                                    // PIN_TYPE_UART0_RTS
                                                    // PIN_TYPE_UART1_RX
                                                    // PIN_TYPE_I2C_SCL
                                                    // PIN_TYPE_SPI1_IO_1
                                                    // PIN_TYPE_SPI1_IO_MISO
                                                    // PIN_TYPE_AUX_9
                                                    // PIN_TYPE_UART_APS_RX
                                                    // PIN_TYPE_UART_MSQ_TX
                                                    // PIN_TYPE_ICE_M3_CLK
                                                    // PIN_TYPE_ICE_M0_DAT

#define HAL_PIN_TYPE_IO_10  PIN_TYPE_NONE           // PIN_TYPE_NONE
                                                    // PIN_TYPE_GPIO_INPUT
                                                    // PIN_TYPE_GPIO_OUTPUT_LOW
                                                    // PIN_TYPE_GPIO_OUTPUT_HIGH
                                                    // PIN_TYPE_UART0_TX
                                                    // PIN_TYPE_UART1_RX
                                                    // PIN_TYPE_I2C_SCL
                                                    // PIN_TYPE_SPI1_IO_2
                                                    // PIN_TYPE_AUX_10
                                                    // PIN_TYPE_UART_APS_TX
                                                    // PIN_TYPE_UART_MSQ_RX
                                                    // PIN_TYPE_ICE_M3_DAT
                                                    // PIN_TYPE_ICE_M0_CLK

#define HAL_PIN_TYPE_IO_11  PIN_TYPE_NONE           // PIN_TYPE_NONE
                                                    // PIN_TYPE_GPIO_INPUT
                                                    // PIN_TYPE_GPIO_OUTPUT_LOW
                                                    // PIN_TYPE_GPIO_OUTPUT_HIGH
                                                    // PIN_TYPE_UART0_RX
                                                    // PIN_TYPE_UART1_TX
                                                    // PIN_TYPE_I2C_SDA
                                                    // PIN_TYPE_SPI1_IO_3
                                                    // PIN_TYPE_AUX_11
                                                    // PIN_TYPE_UART_APS_RX
                                                    // PIN_TYPE_UART_MSQ_TX
                                                    // PIN_TYPE_ICE_M3_CLK
                                                    // PIN_TYPE_ICE_M0_DAT

#define HAL_PIN_TYPE_IO_12  PIN_TYPE_SPI0_CS        // PIN_TYPE_NONE
                                                    // PIN_TYPE_GPIO_INPUT
                                                    // PIN_TYPE_GPIO_OUTPUT_LOW
                                                    // PIN_TYPE_GPIO_OUTPUT_HIGH
                                                    // PIN_TYPE_UART0_TX
                                                    // PIN_TYPE_UART1_CTS
                                                    // PIN_TYPE_I2C_SCL
                                                    // PIN_TYPE_SPI0_CS
                                                    // PIN_TYPE_AUX_12
                                                    // PIN_TYPE_UART_APS_TX
                                                    // PIN_TYPE_UART_MSQ_RX
                                                    // PIN_TYPE_ICE_M3_DAT
                                                    // PIN_TYPE_ICE_M0_CLK

#define HAL_PIN_TYPE_IO_13  PIN_TYPE_SPI0_CLK       // PIN_TYPE_NONE
                                                    // PIN_TYPE_GPIO_INPUT
                                                    // PIN_TYPE_GPIO_OUTPUT_LOW
                                                    // PIN_TYPE_GPIO_OUTPUT_HIGH
                                                    // PIN_TYPE_UART0_RX
                                                    // PIN_TYPE_UART1_RTS
                                                    // PIN_TYPE_I2C_SDA
                                                    // PIN_TYPE_SPI0_CLK
                                                    // PIN_TYPE_AUX_13
                                                    // PIN_TYPE_UART_APS_RX
                                                    // PIN_TYPE_UART_MSQ_TX
                                                    // PIN_TYPE_ICE_M3_CLK
                                                    // PIN_TYPE_ICE_M0_DAT

#define HAL_PIN_TYPE_IO_14  PIN_TYPE_SPI0_IO_MOSI   // PIN_TYPE_NONE
                                                    // PIN_TYPE_GPIO_INPUT
                                                    // PIN_TYPE_GPIO_OUTPUT_LOW
                                                    // PIN_TYPE_GPIO_OUTPUT_HIGH
                                                    // PIN_TYPE_UART1_TX
                                                    // PIN_TYPE_I2C_SDA
                                                    // PIN_TYPE_SPI0_IO_0
                                                    // PIN_TYPE_SPI0_IO_MOSI
                                                    // PIN_TYPE_AUX_14
                                                    // PIN_TYPE_UART_APS_TX
                                                    // PIN_TYPE_UART_MSQ_RX
                                                    // PIN_TYPE_ICE_M3_DAT
                                                    // PIN_TYPE_ICE_M0_CLK

#define HAL_PIN_TYPE_IO_15  PIN_TYPE_SPI0_IO_MISO   // PIN_TYPE_NONE
                                                    // PIN_TYPE_GPIO_INPUT
                                                    // PIN_TYPE_GPIO_OUTPUT_LOW
                                                    // PIN_TYPE_GPIO_OUTPUT_HIGH
                                                    // PIN_TYPE_UART1_RX
                                                    // PIN_TYPE_I2C_SCL
                                                    // PIN_TYPE_SPI0_IO_1
                                                    // PIN_TYPE_SPI0_IO_MISO
                                                    // PIN_TYPE_AUX_15
                                                    // PIN_TYPE_UART_APS_RX
                                                    // PIN_TYPE_UART_MSQ_TX
                                                    // PIN_TYPE_ICE_M3_CLK
                                                    // PIN_TYPE_ICE_M0_DAT

#define HAL_PIN_TYPE_IO_16  PIN_TYPE_NONE           // PIN_TYPE_NONE
                                                    // PIN_TYPE_GPIO_INPUT
                                                    // PIN_TYPE_GPIO_OUTPUT_LOW
                                                    // PIN_TYPE_GPIO_OUTPUT_HIGH
                                                    // PIN_TYPE_UART0_TX
                                                    // PIN_TYPE_UART1_RX
                                                    // PIN_TYPE_I2C_SCL
                                                    // PIN_TYPE_SPI0_IO_2
                                                    // PIN_TYPE_UART_APS_TX
                                                    // PIN_TYPE_UART_MSQ_RX
                                                    // PIN_TYPE_ICE_M3_DAT
                                                    // PIN_TYPE_ICE_M0_CLK

#define HAL_PIN_TYPE_IO_17  PIN_TYPE_NONE           // PIN_TYPE_NONE
                                                    // PIN_TYPE_GPIO_INPUT
                                                    // PIN_TYPE_GPIO_OUTPUT_LOW
                                                    // PIN_TYPE_GPIO_OUTPUT_HIGH
                                                    // PIN_TYPE_UART0_RX
                                                    // PIN_TYPE_UART1_TX
                                                    // PIN_TYPE_I2C_SDA
                                                    // PIN_TYPE_SPI0_IO_3
                                                    // PIN_TYPE_UART_APS_RX
                                                    // PIN_TYPE_UART_MSQ_TX
                                                    // PIN_TYPE_ICE_M3_CLK
                                                    // PIN_TYPE_ICE_M0_DAT

#define HAL_PIN_TYPE_IO_18  PIN_TYPE_NONE           // PIN_TYPE_NONE
                                                    // PIN_TYPE_GPIO_INPUT
                                                    // PIN_TYPE_GPIO_OUTPUT_LOW
                                                    // PIN_TYPE_GPIO_OUTPUT_HIGH
                                                    // PIN_TYPE_UART0_CTS
                                                    // PIN_TYPE_UART1_TX
                                                    // PIN_TYPE_I2C_SDA
                                                    // PIN_TYPE_SPI2_CS
                                                    // PIN_TYPE_PWM_5
                                                    // PIN_TYPE_UART_APS_TX
                                                    // PIN_TYPE_UART_MSQ_RX
                                                    // PIN_TYPE_ICE_M3_DAT
                                                    // PIN_TYPE_ICE_M0_CLK

#define HAL_PIN_TYPE_IO_19  PIN_TYPE_NONE           // PIN_TYPE_NONE
                                                    // PIN_TYPE_GPIO_INPUT
                                                    // PIN_TYPE_GPIO_OUTPUT_LOW
                                                    // PIN_TYPE_GPIO_OUTPUT_HIGH
                                                    // PIN_TYPE_UART0_RTS
                                                    // PIN_TYPE_UART1_RX
                                                    // PIN_TYPE_I2C_SCL
                                                    // PIN_TYPE_SPI2_CLK
                                                    // PIN_TYPE_PWM_4
                                                    // PIN_TYPE_UART_APS_RX
                                                    // PIN_TYPE_UART_MSQ_TX
                                                    // PIN_TYPE_ICE_M3_CLK
                                                    // PIN_TYPE_ICE_M0_DAT

#define HAL_PIN_TYPE_IO_20  PIN_TYPE_NONE           // PIN_TYPE_NONE
                                                    // PIN_TYPE_GPIO_INPUT
                                                    // PIN_TYPE_GPIO_OUTPUT_LOW
                                                    // PIN_TYPE_GPIO_OUTPUT_HIGH
                                                    // PIN_TYPE_UART0_TX
                                                    // PIN_TYPE_UART1_CTS
                                                    // PIN_TYPE_UART1_RX
                                                    // PIN_TYPE_I2C_SCL
                                                    // PIN_TYPE_SPI2_IO_0
                                                    // PIN_TYPE_SPI2_IO_MOSI
                                                    // PIN_TYPE_PWM_3
                                                    // PIN_TYPE_UART_APS_TX
                                                    // PIN_TYPE_UART_MSQ_RX
                                                    // PIN_TYPE_ICE_M3_DAT
                                                    // PIN_TYPE_ICE_M0_CLK

#define HAL_PIN_TYPE_IO_21  PIN_TYPE_NONE           // PIN_TYPE_NONE
                                                    // PIN_TYPE_GPIO_INPUT
                                                    // PIN_TYPE_GPIO_OUTPUT_LOW
                                                    // PIN_TYPE_GPIO_OUTPUT_HIGH
                                                    // PIN_TYPE_UART0_RX
                                                    // PIN_TYPE_UART1_RTS
                                                    // PIN_TYPE_UART1_TX
                                                    // PIN_TYPE_I2C_SDA
                                                    // PIN_TYPE_SPI2_IO_1
                                                    // PIN_TYPE_SPI2_IO_MISO
                                                    // PIN_TYPE_PWM_2
                                                    // PIN_TYPE_UART_APS_RX
                                                    // PIN_TYPE_UART_MSQ_TX
                                                    // PIN_TYPE_ICE_M3_CLK
                                                    // PIN_TYPE_ICE_M0_DAT

#define HAL_PIN_TYPE_IO_22  PIN_TYPE_NONE           // PIN_TYPE_NONE
                                                    // PIN_TYPE_GPIO_INPUT
                                                    // PIN_TYPE_GPIO_OUTPUT_LOW
                                                    // PIN_TYPE_GPIO_OUTPUT_HIGH
                                                    // PIN_TYPE_UART1_TX
                                                    // PIN_TYPE_I2C_SDA
                                                    // PIN_TYPE_SPI2_IO_2
                                                    // PIN_TYPE_PWM_1
                                                    // PIN_TYPE_UART_APS_TX
                                                    // PIN_TYPE_UART_MSQ_RX
                                                    // PIN_TYPE_ICE_M3_DAT
                                                    // PIN_TYPE_ICE_M0_CLK

#define HAL_PIN_TYPE_IO_23  PIN_TYPE_NONE           // PIN_TYPE_NONE
                                                    // PIN_TYPE_GPIO_INPUT
                                                    // PIN_TYPE_GPIO_OUTPUT_LOW
                                                    // PIN_TYPE_GPIO_OUTPUT_HIGH
                                                    // PIN_TYPE_UART1_RX
                                                    // PIN_TYPE_I2C_SCL
                                                    // PIN_TYPE_SPI2_IO_3
                                                    // PIN_TYPE_PWM_0
                                                    // PIN_TYPE_UART_APS_RX
                                                    // PIN_TYPE_UART_MSQ_TX
                                                    // PIN_TYPE_ICE_M3_CLK
                                                    // PIN_TYPE_ICE_M0_DAT

// IO driving select
#define HAL_PIN_DRIVING_IO_0    PIN_DRIVING_FLOAT   // PIN_DRIVING_FLOAT | PIN_DRIVING_HIGH
#define HAL_PIN_DRIVING_IO_1    PIN_DRIVING_LOW     // PIN_DRIVING_FLOAT | PIN_DRIVING_LOW
#define HAL_PIN_DRIVING_IO_2    PIN_DRIVING_FLOAT   // PIN_DRIVING_FLOAT | PIN_DRIVING_HIGH
#define HAL_PIN_DRIVING_IO_3    PIN_DRIVING_FLOAT   // PIN_DRIVING_FLOAT | PIN_DRIVING_LOW
#define HAL_PIN_DRIVING_IO_4    PIN_DRIVING_FLOAT   // PIN_DRIVING_FLOAT | PIN_DRIVING_HIGH
#define HAL_PIN_DRIVING_IO_5    PIN_DRIVING_FLOAT   // PIN_DRIVING_FLOAT | PIN_DRIVING_HIGH
#define HAL_PIN_DRIVING_IO_6    PIN_DRIVING_FLOAT   // PIN_DRIVING_FLOAT | PIN_DRIVING_LOW
#define HAL_PIN_DRIVING_IO_7    PIN_DRIVING_FLOAT   // PIN_DRIVING_FLOAT | PIN_DRIVING_HIGH
#define HAL_PIN_DRIVING_IO_8    PIN_DRIVING_FLOAT   // PIN_DRIVING_FLOAT | PIN_DRIVING_LOW
#define HAL_PIN_DRIVING_IO_9    PIN_DRIVING_HIGH    // PIN_DRIVING_FLOAT | PIN_DRIVING_HIGH
#define HAL_PIN_DRIVING_IO_10   PIN_DRIVING_FLOAT   // PIN_DRIVING_FLOAT | PIN_DRIVING_HIGH
#define HAL_PIN_DRIVING_IO_11   PIN_DRIVING_FLOAT   // PIN_DRIVING_FLOAT | PIN_DRIVING_HIGH
#define HAL_PIN_DRIVING_IO_12   PIN_DRIVING_FLOAT   // PIN_DRIVING_FLOAT | PIN_DRIVING_HIGH
#define HAL_PIN_DRIVING_IO_13   PIN_DRIVING_FLOAT   // PIN_DRIVING_FLOAT | PIN_DRIVING_LOW
#define HAL_PIN_DRIVING_IO_14   PIN_DRIVING_FLOAT   // PIN_DRIVING_FLOAT | PIN_DRIVING_HIGH
#define HAL_PIN_DRIVING_IO_15   PIN_DRIVING_FLOAT   // PIN_DRIVING_FLOAT | PIN_DRIVING_LOW
#define HAL_PIN_DRIVING_IO_16   PIN_DRIVING_FLOAT   // PIN_DRIVING_FLOAT | PIN_DRIVING_HIGH
#define HAL_PIN_DRIVING_IO_17   PIN_DRIVING_FLOAT   // PIN_DRIVING_FLOAT | PIN_DRIVING_LOW
#define HAL_PIN_DRIVING_IO_18   PIN_DRIVING_FLOAT   // PIN_DRIVING_FLOAT | PIN_DRIVING_HIGH
#define HAL_PIN_DRIVING_IO_19   PIN_DRIVING_FLOAT   // PIN_DRIVING_FLOAT | PIN_DRIVING_LOW
#define HAL_PIN_DRIVING_IO_20   PIN_DRIVING_FLOAT   // PIN_DRIVING_FLOAT | PIN_DRIVING_HIGH
#define HAL_PIN_DRIVING_IO_21   PIN_DRIVING_FLOAT   // PIN_DRIVING_FLOAT | PIN_DRIVING_LOW
#define HAL_PIN_DRIVING_IO_22   PIN_DRIVING_FLOAT   // PIN_DRIVING_FLOAT | PIN_DRIVING_HIGH
#define HAL_PIN_DRIVING_IO_23   PIN_DRIVING_FLOAT   // PIN_DRIVING_FLOAT | PIN_DRIVING_LOW

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...

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

#endif  // end of __HAL_PIN_CONFIG_PROJECT_H__
