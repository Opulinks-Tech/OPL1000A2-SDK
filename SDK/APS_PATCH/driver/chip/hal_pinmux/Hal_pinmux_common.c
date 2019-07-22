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

#include "Hal_pinmux_common.h"

T_Pin_Unit const pin_map_table[]=
{
    {OPL1000_IO2_PIN,GPIO_IDX_02},
    {OPL1000_IO3_PIN,GPIO_IDX_03},
    {OPL1000_IO4_PIN,GPIO_IDX_04},
    {OPL1000_IO5_PIN,GPIO_IDX_05},
    {OPL1000_IO6_PIN,GPIO_IDX_06},
    {OPL1000_IO7_PIN,GPIO_IDX_07},
    {OPL1000_IO8_PIN,GPIO_IDX_08},
    {OPL1000_IO9_PIN,GPIO_IDX_09},
    {OPL1000_IO10_PIN,GPIO_IDX_10},
    {OPL1000_IO11_PIN,GPIO_IDX_11},
    {OPL1000_IO16_PIN,GPIO_IDX_16},
    {OPL1000_IO17_PIN,GPIO_IDX_17},
    {OPL1000_IO18_PIN,GPIO_IDX_18},
    {OPL1000_IO19_PIN,GPIO_IDX_19},
    {OPL1000_IO20_PIN,GPIO_IDX_20},
    {OPL1000_IO21_PIN,GPIO_IDX_21},
    {OPL1000_IO22_PIN,GPIO_IDX_22},
    {OPL1000_IO23_PIN,GPIO_IDX_23},
};


T_Pin Hal_Get_Point(uint8_t io,E_OPLXXX_Periph periph_pin)
{
    T_Pin *pin_p=NULL;
    uint8_t io_index = 0;

    uint8_t i;
    uint8_t pin_cnt = 0;
    uint8_t pin_daul_size;
    switch(periph_pin)
    {
        //UART0
        case UART0_TX_E:
            pin_cnt = sizeof(g_PinMap.uart0_tx) / sizeof(T_Pin);
            pin_p = (T_Pin*)&g_PinMap.uart0_tx;
            break;
        
        case UART0_RX_E:
            pin_cnt = sizeof(g_PinMap.uart0_rx) / sizeof(T_Pin);
            pin_p = (T_Pin*)&g_PinMap.uart0_rx;
            break;
        
        case UART0_CTS_E:
            pin_cnt = sizeof(g_PinMap.uart0_cts) / sizeof(T_Pin);
            pin_p = (T_Pin*)&g_PinMap.uart0_cts;
            break;
        
        case UART0_RTS_E:
            pin_cnt = sizeof(g_PinMap.uart0_rts) / sizeof(T_Pin);
            pin_p = (T_Pin*)&g_PinMap.uart0_rts;
            break;
        
        //UART1
        case UART1_TX_E:
            pin_cnt = sizeof(g_PinMap.uart1_tx) / sizeof(T_Pin);;
            pin_p = (T_Pin*)&g_PinMap.uart1_tx;
            break;
        
        case UART1_RX_E:
            pin_cnt = sizeof(g_PinMap.uart1_rx) /sizeof(T_Pin);
            pin_p = (T_Pin*)&g_PinMap.uart1_rx;
            break;
        
        case UART1_CTS_E:
            pin_cnt = sizeof(g_PinMap.uart1_cts) / sizeof(T_Pin);
            pin_p = (T_Pin*)&g_PinMap.uart1_cts;
            break;
        
        case UART1_RTS_E:
            pin_cnt = sizeof(g_PinMap.uart1_rts) / sizeof(T_Pin);
            pin_p = (T_Pin*)&g_PinMap.uart1_rts;
            break;

        //SPI1
        case SPI1_CS_E:
            pin_cnt = sizeof(g_PinMap.spi1_cs) / sizeof(T_Pin);
            pin_p = (T_Pin*)&g_PinMap.spi1_cs;
            break;
        
        case SPI1_CLK_E:
            pin_cnt = sizeof(g_PinMap.spi1_clk) /sizeof(T_Pin);
            pin_p = (T_Pin*)&g_PinMap.spi1_clk;
            break;
        
        case SPI1_MOSI_E:
            pin_cnt = sizeof(g_PinMap.spi1_mosi) / sizeof(T_Pin);
            pin_p = (T_Pin*)&g_PinMap.spi1_mosi;
            break;
        
        case SPI1_MISO_E:
            pin_cnt = sizeof(g_PinMap.spi1_miso) / sizeof(T_Pin);
            pin_p = (T_Pin*)&g_PinMap.spi1_miso;
            break;
        
        //SPI2
        case SPI2_CS_E:
            pin_cnt = sizeof(g_PinMap.spi2_cs) / sizeof(T_Pin);
            pin_p = (T_Pin*)&g_PinMap.spi2_cs;
            break;
        
        case SPI2_CLK_E:
            pin_cnt = sizeof(g_PinMap.spi2_clk) / sizeof(T_Pin);
            pin_p = (T_Pin*)&g_PinMap.spi2_clk;
            break;
        
        case SPI2_MOSI_E:
            pin_cnt = sizeof(g_PinMap.spi2_mosi) / sizeof(T_Pin);
            pin_p = (T_Pin*)&g_PinMap.spi2_mosi;
            break;
        
        case SPI2_MISO_E:
            pin_cnt = sizeof(g_PinMap.spi2_miso) / sizeof(T_Pin);
            pin_p = (T_Pin*)&g_PinMap.spi2_miso;
            break;

        //I2C
        case I2C_SCL_OUT_E:
            pin_cnt = sizeof(g_PinMap.i2c_scl_out) / sizeof(T_Pin);
            pin_p = (T_Pin*)&g_PinMap.i2c_scl_out;
            break;
        
        case I2C_SCL_IN_E:
            pin_cnt = sizeof(g_PinMap.i2c_scl_in) / sizeof(T_Pin);
            pin_p = (T_Pin*)&g_PinMap.i2c_scl_in;
            break;
        
        case I2C_SDA_OUT_E:
            pin_cnt = sizeof(g_PinMap.i2c_sda_out) / sizeof(T_Pin);
            pin_p = (T_Pin*)&g_PinMap.i2c_sda_out;
            break;
        
        case I2C_SDA_IN_E:
            pin_cnt = sizeof(g_PinMap.i2c_sda_in) / sizeof(T_Pin);
            pin_p = (T_Pin*)&g_PinMap.i2c_sda_in;
            break;
        
        //PWM
        case PWM_E:
            pin_cnt = sizeof(g_PinMap.pwm) / sizeof(T_Pin);
            pin_p = (T_Pin*)&g_PinMap.pwm;
            break;
        
        default:
            HAL_PINMUX_PRINT("not find %d \r\n",periph_pin);
            break;
    }
    //mapping relationship
    pin_daul_size = sizeof(pin_map_table) / sizeof(T_Pin_Unit);
    for(i=0;i<pin_daul_size;i++)
    {
        //HAL_PINMUX_PRINT("find sour IO =%d,curr sour IO = %d \r\n",io,pin_map_table[i].sour);
        if(pin_map_table[i].sour == io)
        {
            io_index = i;
            break;
        }
    }
    if(i >=  pin_daul_size)
    {
        HAL_PINMUX_PRINT("get io relation error \r\n");
    }

    for(i=0;i<pin_cnt;i++)
    {
        //HAL_PINMUX_PRINT("point = %p,find GPIO = %d,curr GPIO = %d \r\n",pin_p,pin_map_table[io_index].dist,(*pin_p).io);
        if((*pin_p).io == pin_map_table[io_index].dist)
        {
            //HAL_PINMUX_PRINT("OPL1000 IO = %d,point entry = %p \r\n",(*pin_p).io,pin_p);
            break;
        }
        pin_p++; 
    }
    if(i >= pin_cnt)
    {
        HAL_PINMUX_PRINT("get pin error \n");
    }
    return *pin_p;
}



