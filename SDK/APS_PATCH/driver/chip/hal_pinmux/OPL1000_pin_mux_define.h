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


#ifndef __OPU1000_PIN_MUX_DEFINE_H
#define __OPU1000_PIN_MUX_DEFINE_H

#define  NULL_INDEX       0   
#define  BLANK_PIN        0
#define  OPL1000_IO2_PIN   23
#define  OPL1000_IO3_PIN   24
#define  OPL1000_IO4_PIN   25
#define  OPL1000_IO5_PIN   27
#define  OPL1000_IO6_PIN   28
#define  OPL1000_IO7_PIN   30
#define  OPL1000_IO8_PIN   31
#define  OPL1000_IO9_PIN   32
#define  OPL1000_IO10_PIN  33
#define  OPL1000_IO11_PIN  34
#define  OPL1000_IO16_PIN  40
#define  OPL1000_IO17_PIN  42
#define  OPL1000_IO18_PIN  44
#define  OPL1000_IO19_PIN  45
#define  OPL1000_IO20_PIN  46
#define  OPL1000_IO21_PIN  47
#define  OPL1000_IO22_PIN  48
#define  OPL1000_IO23_PIN  29

#define  OPL1000_AVALIABLE_PIN  18 

#define  UART_MAX_NUM          2
#define  I2C_MAX_NUM           1  
#define  SPI_MAX_NUM           2  
#define  PWM_MAX_NUM           6  
#define  AUX_MAX_NUM           16  
#define  GPIO_MAX_NUM          OPL1000_AVALIABLE_PIN

#define  MAX_PORT_TYPE_NUM     7  // I2C has master and slave two types  

#include "hal_uart.h"
#include "hal_i2c.h"
#include "hal_spi.h"
#include "hal_pwm.h"
//#include "ble_type_define.h"

typedef enum
{
    PORT_UNKNOWN = 0x00,
    PORT_UART = 0x10,
    PORT_I2C_MASTER = 0x30,
    PORT_I2C_SLAVE = 0x40,    
    PORT_SPI = 0x50, 
    PORT_PWM = 0x70, 
    PORT_AUX = 0x80,  
    PORT_GPIO = 0xA0                      
}E_PortType_t;

typedef enum
{
    BLANK    = 0x00,
    UART0_TX = 0x11,
    UART0_RX,
    UART0_RTS,
    UART0_CTS, 
    UART1_TX = 0x21,
    UART1_RX,
    UART1_RTS,
    UART1_CTS,  
    MASTER_I2C_SCL = 0x31,
    MASTER_I2C_SDA,  
    SLAVE_I2C_SCL = 0x41,
    SLAVE_I2C_SDA,  
    SPI1_CS = 0x51,
    SPI1_CLK,       
    SPI1_MISO,  
    SPI1_MOSI,
    SPI1_IO2,
    SPI1_IO3,
    SPI2_CS = 0x61,
    SPI2_CLK,       
    SPI2_MISO,  
    SPI2_MOSI,
    SPI2_IO2,
    SPI2_IO3,
    PWM1 = 0x71,
    PWM2, 
    PWM3,
    PWM4, 
    PWM5,
    PWM6,     
    AUX1 = 0x81,    
    AUX2,
    AUX3,    
    AUX4,
    AUX5, 
    AUX6,
    AUX7,    
    AUX8,
    AUX9,
    AUX10,
    GPIO1 = 0xA1,    
    GPIO2,
    GPIO3,   
    GPIO4,
    GPIO5, 
    GPIO6,
    GPIO7, 
    GPIO8,
    GPIO9, 
    GPIO10,
    GPIO11, 
    GPIO12,
    GPIO13, 
    GPIO14,
    GPIO15, 
    GPIO16,
    GPIO17, 
    GPIO18   
}E_PortSignal_t;

typedef enum
{
    UART_SIMPLE = 0,
    UART_FLOW       // has RTS and CTS flow control
} E_UartFlowCtrl_t; 

typedef struct {
    E_UartIdx_t index; 
    uint8_t tx;                  // TX pin 
    uint8_t rx;                  // RX pin  
    uint8_t rts;                 // RTS pin 
    uint8_t cts;                 // CTS pin   
    uint32_t baudrate;           // baud rate     
    E_UartDataBit_t bits;        // indicate 8 or 7 bits, 7 or 8 
    E_UartParity_t parity;       // parity mode, 0 or 1  
    E_UartStopBit_t stopBit;     // stop bit, 0 or 1 
    E_UartFlowCtrl_t flowCtl;    // Enable/Disable CTS and RTS. 1: enable, 0: disable       
}T_OPL1000_uart;

typedef enum
{
    MASTER_NO_STOP  = 0,     // does not need stop bit  
    MASTER_HAS_STOP = 1,     // need stop bit for master 
    SLAVE_FLAG = 2
} E_I2cStopBit_t; 

typedef struct { 
    E_I2cSpeed_t type;      // 100k or 400k speed  
    uint8_t scl;            // SCK pin 
    uint8_t sda;            // SDA pin  
    E_I2cAddrMode_t addr;   // 7 bit or 10 bit address format 
    uint16_t address;       // I2C address      
    E_I2cStopBit_t stopBit; // has stop bit or not in master mode     
}T_OPL1000_I2c;

typedef enum
{
    QMODE_DISABLE = 0,
    QMODE_ENABLE    
} E_SpiQMode_t;
    
typedef struct {
    E_SpiIdx_t index;           // inheritted from hal_spi.h
    uint8_t cs;                 // chip select pin 
    uint8_t clk;                // clock pin  
    uint8_t miso;               // MISO or IO0 pin   
    uint8_t mosi;               // MOSI or IO1 pin
    uint8_t io2;                // IO2 pin, for fast mode  
    uint8_t io3;                // IO3 pin, for fast mode      
    uint32_t baudrate;          // for example 1000000 
    E_SpiClkPolarity_t polar;   // SPI clock polarity. 0 or 1 
    E_SpiClkPhase_t phase;      // SPI clock phase.  
    E_SpiFrameFormat_t format;  // transmit protocol 
    E_SpiDataFrameSize_t size;  // format size 
    E_SpiQMode_t qMode;         // Qaud-mode select. 1 for enable/0 for disable               
}T_OPL1000_Spi; 

typedef enum
{
    CLK_32KHz = 0x01,    // 32KHz clock, equals HAL_PWM_CLK_32K  
    CLK_22MHz            // 22MHz clock, equals HAL_PWM_CLK_22M   
} E_PwmClockSrc_t;    // PWM clock source 

typedef enum
{
    CFG_SIMPLE = 0,    // simple config, only use Duty and Colck parameters 
    CFG_COMPLEX        // complex config, use period, dutyBright etc. 7 parameters 
} E_PwmCfgType_t;      // PWM configuration type  

typedef struct {
    uint8_t pin;                 // pin number 
    E_PwmClockSrc_t clkSrc;        // clock source
    E_PwmCfgType_t cfgType;      // clock pin  
    uint8_t duty;                // MISO or IO0 pin   
    uint32_t clkHz;              // clock frequency in Hz unit 
    uint32_t period;             // the tick count of one clock cycle  
    uint32_t dutyBright;         // max tick count of high level in one clock cycle      
    uint32_t dutyDull;           // min tick count of high level in one clock cycle 
    uint32_t rampUp;             // delta count from dull to bright per clock cycle
    uint32_t rampDown;           // delta count from bright to dull per clock cycle
    uint32_t holdBright;         // hold times of the bright state
    uint32_t holdDull;           // hold times the dull state    
}T_OPL1000_Pwm; 

typedef enum
{
    IO_OUTPUT = 0,   // OUTPUT 
    IO_INPUT         // INPUT
} E_GpioType_t; 

typedef enum
{
    PULL_UP = 0,    // Pull Up
    PULL_DOWN       // Pull down 
} E_GpioPull_t;     // define output pin is pull_up or pull_down type 

typedef struct {
    uint8_t pin;              // pin number 
    E_GpioType_t type;        // input or output
    E_GpioPull_t pull;        // pull_up or pull_down type for output type     
}T_OPL1000_Gpio;

typedef struct { 
    uint8_t uart_num;
    T_OPL1000_uart uart[UART_MAX_NUM]; // store pin number that assigned to UART 
    uint8_t i2c_num;
    T_OPL1000_I2c i2c[I2C_MAX_NUM];    // store pin number that assigned to I2C 
    uint8_t spi_num;
    T_OPL1000_Spi spi[SPI_MAX_NUM];    // store pin number that assigned to SPI 
    uint8_t pwm_num; 
    T_OPL1000_Pwm pwm[PWM_MAX_NUM];    // store pin number that assigned to PWM    
    uint8_t aux_num; 
    uint8_t aux[AUX_MAX_NUM];              // store pin number that assigned to AUX (analog input)
    uint8_t gpio_num; 
    T_OPL1000_Gpio gpio[GPIO_MAX_NUM]; // store pin number that assigned to GPIO   
}T_OPL1000_Periph; 


extern T_OPL1000_Periph OPL1000_periph; 



#endif /* __OPU1000_PIN_MUX_DEFINE_H */
