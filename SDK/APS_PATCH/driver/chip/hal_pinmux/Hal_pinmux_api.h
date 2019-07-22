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

#ifndef _PINMUX_API_H_
#define _PINMUX_API_H_

#include "stdint.h"
#include "opl1000.h"
#include "hal_vic.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define PIN     ((S_App_Pin_Config_t *) (AOS_BASE + 0x090))
 
#define OOUTPUT_OFF             0xD

// Sec 3: structure, uniou, enum, linked list
typedef struct
{
    volatile uint32_t RG_PD_IE;           // 0x090
    volatile uint32_t RG_PD_PE;           // 0x094
    volatile uint32_t RG_PD_O_INV;        // 0x098
    volatile uint32_t RG_PD_DS;           // 0x09C
    volatile uint32_t RG_GPO;             // 0x0A0
    volatile uint32_t RG_PD_I_INV;        // 0x0A4
    volatile uint32_t RG_PDOV_MODE;       // 0x0A8
    volatile uint32_t RG_PD_DIR;          // 0x0AC
    volatile uint32_t RG_PD_OENP_INV;     // 0x0B0
    volatile uint32_t RG_PDOC_MODE;       // 0x0B4
    volatile uint32_t RG_GPI;             // 0x0B8
    volatile uint32_t reserve_0bc;        // 0x0BC, reserved
    volatile uint32_t RG_PDI_SRC_IO_A;    // 0x0C0
    volatile uint32_t RG_PDI_SRC_IO_B;    // 0x0C4
    volatile uint32_t RG_PDI_SRC_IO_C;    // 0x0C8
    volatile uint32_t RG_PDI_SRC_IO_D;    // 0x0CC
    volatile uint32_t RG_PTS_INMUX_A;     // 0x0D0
    volatile uint32_t RG_PTS_INMUX_B;     // 0x0D4
    volatile uint32_t RG_PTS_INMUX_C;     // 0x0D8
    volatile uint32_t RG_PTS_INMUX_D;     // 0x0DC
} S_App_Pin_Config_t;

/**
 * @brief The basic structure of IO information
 */
typedef struct 
{
    uint8_t io;         /**< GPIO index */
    uint8_t row;        /**< excel row  */
    uint8_t col;        /**< excel col  */
}T_Pin;

/**
 * @brief Uart0 TX IO information
 */
typedef struct 
{
    T_Pin pin2;     /**< UART0 TX :pin2  */
    T_Pin pin6;     /**< UART0 TX :pin6  */
    T_Pin pin10;    /**< UART0 TX :pin10 */
    T_Pin pin16;    /**< UART0 TX :pin16 */
    T_Pin pin20;    /**< UART0 TX :pin20 */
}T_Uart0_TX;

/**
 * @brief Uart0 RX IO information
 */
typedef struct 
{
    T_Pin pin3;     /**< UART0 RX :pin3  */
    T_Pin pin7;     /**< UART0 RX :pin7  */
    T_Pin pin11;    /**< UART0 RX :pin11 */
    T_Pin pin17;    /**< UART0 RX :pin17 */
    T_Pin pin21;    /**< UART0 RX :pin21 */
}T_Uart0_RX;

/**
 * @brief Uart0 CTS IO information 
 */
typedef struct 
{
    T_Pin pin8;     /**< UART0 CTS :pin8  */
    T_Pin pin18;    /**< UART0 CTS :pin18 */
}T_Uart0_CTS;

/**
 * @brief Uart0 RTS IO information 
 */
typedef struct 
{
    T_Pin pin9;     /**< UART0 RTS :pin9  */
    T_Pin pin19;    /**< UART0 RTS :pin19 */
}T_Uart0_RTS;

/**
 * @brief Uart1 TX IO information 
 */
typedef struct 
{
    T_Pin pin4;     /**< UART1 TX :pin4  */
    T_Pin pin8;     /**< UART1 TX :pin8  */
    T_Pin pin11;    /**< UART1 TX :pin11 */
    T_Pin pin17;    /**< UART1 TX :pin17 */
    T_Pin pin18;    /**< UART1 TX :pin18 */
    T_Pin pin21;    /**< UART1 TX :pin21 */
    T_Pin pin22;    /**< UART1 TX :pin22 */
}T_Uart1_TX;

/**
 * @brief Uart1 RX IO information 
 */
typedef struct 
{
    T_Pin pin5;     /**< UART1 RX :pin5  */
    T_Pin pin9;     /**< UART1 RX :pin9  */
    T_Pin pin10;    /**< UART1 RX :pin10 */
    T_Pin pin16;    /**< UART1 RX :pin16 */
    T_Pin pin19;    /**< UART1 RX :pin19 */
    T_Pin pin20;    /**< UART1 RX :pin20 */
    T_Pin pin23;    /**< UART1 RX :pin23 */
}T_Uart1_RX;

/**
 * @brief Uart1 CTS IO information 
 */
typedef struct 
{
    T_Pin pin7;     /**< UART1 CTS :pin7 */
    T_Pin pin20;    /**< UART1 CTS :pin20 */
}T_Uart1_CTS;

/**
 * @brief Uart1 RTS IO information 
 */
typedef struct 
{
    T_Pin pin6;     /**< UART1 RTS :pin6 */
    T_Pin pin21;    /**< UART1 RTS :pin21 */
}T_Uart1_RTS;

/**
 * @brief SPI1 CS IO information  
 */
typedef struct 
{
    T_Pin pin4;     /**< SPI1 CS :pin4  */
    T_Pin pin5;     /**< SPI1 CS :pin5  */
    T_Pin pin6;     /**< SPI1 CS :pin6  */
    T_Pin pin10;    /**< SPI1 CS :pin10 */
    T_Pin pin11;    /**< SPI1 CS :pin11 */
}T_Spi1_CS;

/**
 * @brief SPI1 CLK IO information  
 */
typedef struct 
{
    T_Pin pin7;    /**< SPI1 CLK :pin7 */
}T_Spi1_CLK;

/**
 * @brief SPI1 MOSI IO information  
 */
typedef struct 
{
    T_Pin pin8;     /**< SPI1 MOSI :pin8 */
}T_Spi1_MOSI;

/**
 * @brief SPI1 MISO IO information   
 */
typedef struct 
{
    T_Pin pin9;     /**< SPI1 MISO :pin9 */
}T_Spi1_MISO;


/**
 * @brief SPI2 CS IO information   
 */
typedef struct 
{
    T_Pin pin4;     /**< SPI2 CS :pin4 */
    T_Pin pin5;     /**< SPI2 CS :pin5 */
    T_Pin pin6;     /**< SPI2 CS :pin6 */
    T_Pin pin18;    /**< SPI2 CS :pin18 */
    T_Pin pin22;    /**< SPI2 CS :pin22 */
    T_Pin pin23;    /**< SPI2 CS :pin23 */
}T_Spi2_CS;

/**
 * @brief  SPI2 CLK IO information   
 */
typedef struct 
{
    T_Pin pin4;     /**< SPI2 CLK :pin4 */
    T_Pin pin7;     /**< SPI2 CLK :pin7 */
    T_Pin pin19;    /**< SPI2 CLK :pin19 */
}T_Spi2_CLK;

/**
 * @brief  SPI2 MOSI IO information   
 */
typedef struct 
{
    T_Pin pin3;     /**< SPI2 MOSI :pin3 */
    T_Pin pin8;     /**< SPI2 MOSI :pin8 */
    T_Pin pin20;    /**< SPI2 MOSI :pin20 */
}T_Spi2_MOSI;

/**
 * @brief  SPI2 MISO IO information   
 */
typedef struct 
{
    T_Pin pin2;     /**< SPI2 MISO :pin2  */
    T_Pin pin9;     /**< SPI2 MISO :pin9  */
    T_Pin pin21;    /**< SPI2 MISO :pin21 */
}T_Spi2_MISO;

/**
 * @brief  I2C SCL OUT IO information   
 */
typedef struct 
{
    T_Pin pin3;     /**< SPI2 SCL OUT :pin3  */
    T_Pin pin4;     /**< SPI2 SCL OUT :pin4  */
    T_Pin pin6;     /**< SPI2 SCL OUT :pin6  */
    T_Pin pin9;     /**< SPI2 SCL OUT :pin9  */
    T_Pin pin10;    /**< SPI2 SCL OUT :pin10  */
    T_Pin pin16;    /**< SPI2 SCL OUT :pin16  */
    T_Pin pin19;    /**< SPI2 SCL OUT :pin19  */
    T_Pin pin20;    /**< SPI2 SCL OUT :pin20  */
    T_Pin pin23;    /**< SPI2 SCL OUT :pin23  */
}T_I2c_SCL_Out;

/**
 * @brief I2C SCL IN IO information   
 */
typedef struct 
{
    T_Pin pin3;     /**< I2C SCL IN :pin3   */
    T_Pin pin4;     /**< I2C SCL IN :pin4   */
    T_Pin pin6;     /**< I2C SCL IN :pin6   */
    T_Pin pin9;     /**< I2C SCL IN :pin9   */
    T_Pin pin10;    /**< I2C SCL IN :pin10  */
    T_Pin pin16;    /**< I2C SCL IN :pin16  */
    T_Pin pin19;    /**< I2C SCL IN :pin19  */
    T_Pin pin20;    /**< I2C SCL IN :pin20  */
    T_Pin pin23;    /**< I2C SCL IN :pin23  */
}T_I2C_SCL_In;

/**
 * @brief  I2C SDA OUT IO information   
 */
typedef struct 
{
    T_Pin pin2;      /**< I2C SCL OUT :pin2  */
    T_Pin pin5;      /**< I2C SCL OUT :pin5  */
    T_Pin pin7;      /**< I2C SCL OUT :pin7  */
    T_Pin pin8;      /**< I2C SCL OUT :pin8  */
    T_Pin pin11;     /**< I2C SCL OUT :pin11  */
    T_Pin pin17;     /**< I2C SCL OUT :pin17  */
    T_Pin pin18;     /**< I2C SCL OUT :pin18  */
    T_Pin pin21;     /**< I2C SCL OUT :pin21  */
    T_Pin pin22;     /**< I2C SCL OUT :pin22  */
}T_I2c_SDA_Out;

/**
 * @brief  I2C SDA IN IO information   
 */
typedef struct 
{
    T_Pin pin2;       /**< I2C SDA IN :pin2  */
    T_Pin pin5;       /**< I2C SDA IN :pin5  */
    T_Pin pin7;       /**< I2C SDA IN :pin7  */
    T_Pin pin8;       /**< I2C SDA IN :pin8  */
    T_Pin pin11;      /**< I2C SDA IN :pin11  */
    T_Pin pin17;      /**< I2C SDA IN :pin17  */
    T_Pin pin18;      /**< I2C SDA IN :pin18  */
    T_Pin pin21;      /**< I2C SDA IN :pin21  */
    T_Pin pin22;      /**< I2C SDA IN :pin22  */
}T_I2c_SDA_In;

/**
 * @brief PWM IO information   
 */
typedef struct 
{
    T_Pin pin18;       /**< PWM :pin18  */
    T_Pin pin19;       /**< PWM :pin19  */
    T_Pin pin20;       /**< PWM :pin20  */
    T_Pin pin21;       /**< PWM :pin21  */
    T_Pin pin22;       /**< PWM :pin22  */
    T_Pin pin23;       /**< PWM :pin23  */
}T_Pwm;

/**
 * @brief  PIN interface  
 */
typedef struct
{
    T_Uart0_TX  uart0_tx;            /**< UART0 TX  */
    T_Uart0_RX  uart0_rx;            /**< UART0 RX  */
    T_Uart0_CTS uart0_cts;           /**< UART0 CTS */
    T_Uart0_RTS uart0_rts;           /**< UART0 RTS */
    
    T_Uart1_TX  uart1_tx;            /**< UART1 TX  */
    T_Uart1_RX  uart1_rx;            /**< UART1 RX  */
    T_Uart1_CTS uart1_cts;           /**< UART1 CTS */
    T_Uart1_RTS uart1_rts;           /**< UART1 RTS */

    T_Spi1_CS  spi1_cs;              /**< SPI1 CS   */
    T_Spi1_CLK spi1_clk;             /**< SPI1 CLK  */
    T_Spi1_MOSI spi1_mosi;           /**< SPI1 MOSI */
    T_Spi1_MISO spi1_miso;           /**< SPI1 MISO */

    T_Spi2_CS  spi2_cs;              /**< SPI2 CS   */
    T_Spi2_CLK spi2_clk;             /**< SPI2 CLK  */
    T_Spi2_MOSI spi2_mosi;           /**< SPI2 MOSI */
    T_Spi2_MISO spi2_miso;           /**< SPI2 MISO */

    T_I2c_SCL_Out   i2c_scl_out;     /**< IC2 SCL OUT */
    T_I2C_SCL_In    i2c_scl_in;      /**< IC2 CCL IN  */
    T_I2c_SDA_Out   i2c_sda_out;     /**< IC2 SDA OUT */
    T_I2c_SDA_In    i2c_sda_in;      /**< IC2 SDA IN  */

    //PWM   
    T_Pwm pwm;                       /**< PWM          */

}T_Pin_If;


extern T_Pin_If const g_PinMap;

void Hal_PinMux_Config(uint8_t io_num);

/**
 * @brief  Pinmux in put config   
 *
 * @param[in] the gpio struct   
 *
 * @return - 0: successful
 *         - other: failed
 */
uint8_t Hal_PinMux_InPutConfig(T_Pin pin);

/**
 * @brief  Pinmux out put config   
 *
 * @param[in] the gpio struct   
 *
 * @return - 0: successful
 *         - other: failed
 */
uint8_t Hal_PinMux_OutPutConfig(T_Pin pin);

/**
 * @brief  close out put function
 *
 * @param[in] the gpio struct   
 *
 * @return - 0: successful
 *         - other: failed
 */
uint8_t Hal_PinMux_OutPutClose(T_Pin pin);

/**
 * @brief  Pinmux Pull up Config   
 *
 * @param[in] the gpio number   
 *                     
 */
void Hal_PinMux_PullUpConfig(uint8_t io);

/**
 * @brief  Pinmux Pull Down Config   
 *
 * @param[in] the gpio number   
 *                     
 */
void Hal_PinMux_PullDownConfig(uint8_t io);

void App_Pin_InitConfig(void);

#endif /* _PINMUX__H */




















