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

#ifndef _PINMUX_COMMON__H_
#define _PINMUX_COMMON__H_

#include "Hal_pinmux_api.h"
#include "OPL1000_pin_mux_define.h"

#define HAL_PINMUX_PRINT     printf

typedef enum {
    RET_FAIL = -1,
    RET_SUCCESS = 0,
    RET_ERR_NO_MEM,
    RET_ERR_INVALID_ARG,
    RET_ERR_INVALID_STATE,
    RET_ERR_INVALID_SIZE,
    RET_ERR_NOT_FOUND,
    RET_ERR_NOT_SUPPORTED,       
    RET_ERR_TIMEOUT,
    RET_ERR_INVALID_RESPONSE,
    RET_ERR_INVALID_CRC,
    RET_ERR_INVALID_VERSION,
    RET_ERR_INVALID_MAC,
    RET_ERR_WIFI_BASE,
} ERR_CODE;


/**
 * @brief  This structure describes the mapping relationship of IO
 */
typedef struct  
{
    uint8_t sour;         /**< IO sour */
    E_GpioIdx_t dist;        /**< IO dist */
} T_Pin_Unit;

/**
 * @brief  All IO type information
 */
typedef enum
{
    UART0_TX_E,        /**< cinfig UART0_TX  */
    UART0_RX_E,        /**< cinfig UART0_RX  */
    UART0_CTS_E,       /**< cinfig UART0_CTS */
    UART0_RTS_E,       /**< cinfig UART0_RTS */

    UART1_TX_E,        /**< cinfig UART1_TX  */
    UART1_RX_E,        /**< cinfig UART1_RX  */
    UART1_CTS_E,       /**< cinfig UART1_CTS */
    UART1_RTS_E,       /**< cinfig UART1_RTS */
    
    SPI0_CS_E,         /**< cinfig SPI0_CS   */
    SPI0_CLK_E,        /**< cinfig SPI0_CLK  */
    SPI0_MOSI_E,       /**< cinfig SPI0_MOSI */
    SPI0_MISO_E,       /**< cinfig SPI0_MISO */

    SPI1_CS_E,         /**< cinfig SPI1_CS   */
    SPI1_CLK_E,        /**< cinfig SPI1_CLK  */
    SPI1_MOSI_E,       /**< cinfig SPI1_MOSI */
    SPI1_MISO_E,       /**< cinfig SPI1_MISO */

    SPI2_CS_E,         /**< cinfig SPI2_CS   */
    SPI2_CLK_E,        /**< cinfig SPI2_CLK  */
    SPI2_MOSI_E,       /**< cinfig SPI2_MOSI */
    SPI2_MISO_E,       /**< cinfig SPI2_MISO */

    I2C_SCL_OUT_E,     /**< cinfig I2C_SCL_OUT */
    I2C_SCL_IN_E,      /**< cinfig I2C_SCL_IN  */
    I2C_SDA_OUT_E,     /**< cinfig I2C_SDA_OUT */
    I2C_SDA_IN_E,      /**< cinfig I2C_SDA_IN  */

    PWM_E,             /**< cinfig PWM_E       */

}E_OPLXXX_Periph;

/**
  * @brief  Get structure entry address
  *
  * @param  struct of pin
  *
  * @return The entrance of the structure collection
  */
T_Pin Hal_Get_Point(uint8_t io,E_OPLXXX_Periph temp);

extern T_OPL1000_Periph OPL1000_periph_blank;
extern T_Pin_Unit const pin_map_table[];

void App_Pin_InitConfig(void);

#endif
