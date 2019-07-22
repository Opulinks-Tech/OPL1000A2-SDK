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
 * @file hal_pin_patch.c
 * 
 * @brief Patch for Pin-mux API patch
 *  
 *******************************************************************************/

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "hal_pin_patch.h"
#include "hal_pin_def.h"
#include "hal_gpio.h"

/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */

/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */

/*
*************************************************************************
*                           Declarations of Private Functions
*************************************************************************
*/
extern uint8_t Hal_Pin_ConfigSet_impl(uint8_t ubIoIdx, uint16_t uwConfig, uint8_t ubDriving);
uint8_t Hal_Pin_ConfigSet_patch(uint8_t ubIoIdx, uint16_t uwConfig, uint8_t ubDriving);
void Hal_Pin_SetAutoControl(uint8_t ubIoIdx, uint16_t uwConfig);
/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */


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

void Hal_Pin_PatchInit(void)
{
    Hal_Pin_ConfigSet = Hal_Pin_ConfigSet_patch;
}

/*
 *************************************************************************
 *                          Private Functions
 *************************************************************************
 */

/**
 * @brief Pin-mux configuration
 *        Add sleep IO control
 * @param ubIoIdx [In] the index of IO
 * @param uwConfig [In] the type config of IO
 * @param ubDriving [In] the driving of IO
 * @return configuration status
 * @retval HAL_PIN_OK success
 * @retval HAL_PIN_FAIL fail
 */
uint8_t Hal_Pin_ConfigSet_patch(uint8_t ubIoIdx, uint16_t uwConfig, uint8_t ubDriving)
{
    uint8_t u8Ret;
    u8Ret = Hal_Pin_ConfigSet_impl(ubIoIdx, uwConfig, ubDriving);
    if (u8Ret == HAL_PIN_FAIL)
        return HAL_PIN_FAIL;
    
    Hal_Pin_SetAutoControl(ubIoIdx, uwConfig);
    return HAL_PIN_OK;
}

/**
 * @brief According the pinmux, set auto control
 * @param ubIoIdx [In] the index of IO
 * @param uwConfig [In] the type config of IO
 */
void Hal_Pin_SetAutoControl(uint8_t ubIoIdx, uint16_t uwConfig)
{
    switch (uwConfig)
    {
        /* Auto pull high */
        /* UART */
        case PIN_TYPE_UART0_TX:
        case PIN_TYPE_UART0_RTS:
        case PIN_TYPE_UART1_TX:
        case PIN_TYPE_UART1_RTS:
        case PIN_TYPE_UART_APS_TX:
        case PIN_TYPE_UART_MSQ_TX:
        
        /* SPI */
        case PIN_TYPE_SPI0_CS:
        case PIN_TYPE_SPI0_IO_0:
        case PIN_TYPE_SPI0_IO_1:
        case PIN_TYPE_SPI0_IO_2:
        case PIN_TYPE_SPI0_IO_3:
        case PIN_TYPE_SPI0_IO_MOSI:
        case PIN_TYPE_SPI1_CS:
        case PIN_TYPE_SPI1_IO_0:
        case PIN_TYPE_SPI1_IO_1:
        case PIN_TYPE_SPI1_IO_2:
        case PIN_TYPE_SPI1_IO_3:
        case PIN_TYPE_SPI1_IO_MOSI:
        case PIN_TYPE_SPI2_CS:
        case PIN_TYPE_SPI2_IO_0:
        case PIN_TYPE_SPI2_IO_1:
        case PIN_TYPE_SPI2_IO_2:
        case PIN_TYPE_SPI2_IO_3:
        case PIN_TYPE_SPI2_IO_MOSI:
        /* I2C */
        case PIN_TYPE_I2C_SCL:
        case PIN_TYPE_I2C_SDA:
            Hal_Gpio_SleepIoAutoCtrlSet((E_GpioIdx_t)ubIoIdx, SLEEP_OUTPUT_ENABLE_HIGH);
            break;
        
        /* Auto pull low */
        /* SPI */
        case PIN_TYPE_SPI0_CLK:
        case PIN_TYPE_SPI1_CLK:
        case PIN_TYPE_SPI2_CLK:
            Hal_Gpio_SleepIoAutoCtrlSet((E_GpioIdx_t)ubIoIdx, SLEEP_OUTPUT_ENABLE_LOW);
            break;
        /* No auto control */
        default:
            Hal_Gpio_SleepIoAutoCtrlSet((E_GpioIdx_t)ubIoIdx, SLEEP_OUTPUT_DISABLE);
    }
}


