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


#include "Hal_pinmux_api.h"


T_Pin_If const g_PinMap= 
{
    //uart0_tx :IO2/IO6/IO10/IO16/IO20
    {{2,2,3},{6,6,4},{10,10,3},{16,16,3},{20,20,0}},
    //uart0_rx :IO3/IO7/IO11/IO17/IO21
    {{3,1,0},{7,1,1},{11,1,2},{17,1,4},{21,1,5}},
    //uart0_cts :IO8/IO18
    {{8,0,1},{18,0,2}},
    //uart0_rts :IO9/IO19
    {{9,9,3},{19,19,3}},

    //uart1_tx :IO4/IO8/IO11/IO17/IO18/IO21/IO22
    {{4,4,3},{8,8,4},{11,11,4},{17,17,4},{18,18,0},{21,21,4},{22,22,3}},
    //uart1_rx :IO5/IO9/IO10/IO19/IO20/IO23
    {{5,3,1},{9,3,2},{10,3,3},{16,3,5},{19,3,6},{20,3,7},{23,3,8}},
    //uart1_cts :7/IO20
    {{7,2,0},{20,2,2}},
    //uart1_rts :IO6/IO21
    {{6,6,3},{21,21,3}},
   
    //spi1_cs :IO4/IO5/IO6/IO10/IO11
    {{4,4,4},{5,5,3},{6,6,0},{10,10,5},{11,11,5}},
    //spi1_clk :IO7
    {{7,7,0}},
    //spi1 mosi :IO8
    {{8,8,0}},
    //spi miso :IO9
    {{9,9,0}}, 

    //spi2_cs IO4/IO5/IO6/IO18/IO22/IO23
    {{4,4,5},{5,5,0},{6,6,5},{18,18,5},{22,22,4},{23,23,4}},
    //spi2_clk :IO4/IO7/IO19
    {{4,4,0},{7,7,1},{19,19,5}},
    //mosi :IO3/IO8/IO20
    {{3,3,0},{8,8,1},{20,20,5}}, 
    //miso :IO2/IO9/IO21
    {{2,2,0},{9,9,1},{21,21,5}}, 

    //i2c_scl_out :IO3/IO4/IO6/Io9/IO10/IO19/IO20/IO23
    {{3,3,2},{4,4,2},{6,6,2},{9,9,2},{10,10,2},{16,16,2},{19,19,2},{20,20,2},{23,23,2}},
    //i2c_scl_in :IO3/IO4/IO6/Io9/IO10/IO19/IO20/IO23
    {{3,6,1},{4,6,2},{6,6,3},{9,6,4},{10,6,5},{16,6,8},{19,6,9},{20,6,10},{23,6,11}},
    //i2c_sda_out :IO2/IO5/IO7/IO17/IO8/IO11/IO18/IO21/IO22
    {{2,2,2},{5,5,2},{7,7,2},{8,8,2},{11,11,2},{17,17,2},{18,18,2},{21,21,2},{22,22,2}}, 
    //i2c_sda_in IO2/IO5/IO7/IO8/IO11/IO18/IO21/IO22
    {{2,7,1},{5,7,2},{7,7,3},{8,7,4},{11,7,5},{17,7,8},{18,7,9},{21,7,10},{22,7,11}},

    //pwm :IO18/IO19/IO20/IO21/IO22/IO23
    {{18,18,1},{19,19,1},{20,20,1},{21,21,1},{22,22,1},{23,23,1}},

};

uint8_t Hal_PinMux_OutPutClose(T_Pin pin)
{
    uint32_t tmp;
    uint8_t io_num = pin.io ;
    uint32_t index = 0;

    if(io_num <= GPIO_IDX_07)
    {
        index = io_num * 4;
        tmp = PIN->RG_PDI_SRC_IO_A;
        tmp &= ~(0xF << index);
        tmp |=(OOUTPUT_OFF << index);
        PIN->RG_PDI_SRC_IO_A = tmp;
    }
    else if(io_num <= GPIO_IDX_14)
    {
        index = (io_num - 8) * 4;
        tmp = PIN->RG_PDI_SRC_IO_B;
        tmp &= ~(0xF << index);
        tmp |=(OOUTPUT_OFF << index);
        PIN->RG_PDI_SRC_IO_B = tmp;
    }
    else if(io_num <= GPIO_IDX_22)
    {
        index = (io_num - 16) * 4;
        tmp = PIN->RG_PDI_SRC_IO_B;
        tmp &= ~(0xF << index);
        tmp |=(OOUTPUT_OFF << index);
        PIN->RG_PDI_SRC_IO_B = tmp;
    }
    else
    {
        return 1;
    }
    return 0;
}

uint8_t Hal_PinMux_InPutConfig(T_Pin pin)
{
    uint32_t tmp;
    uint8_t io_num = pin.io ;
    uint16_t offset = pin.row;
    uint16_t value  = pin.col;
    uint32_t index = 0;
    //rx-3,1,0
    if(offset <= GPIO_IDX_07)
    {
        index = offset * 4;
        tmp = PIN->RG_PTS_INMUX_A;
        tmp &= ~ (0xF << index);  
        tmp |= (value << index);
        PIN->RG_PTS_INMUX_A = tmp;
    }
    else if(offset <= GPIO_IDX_14)
    {
        index = (offset - 8)* 4;
        tmp = PIN->RG_PTS_INMUX_B;
        tmp &= ~ (0xF << index); 
        tmp |= (value << index);
        PIN->RG_PTS_INMUX_B = tmp;
    }
    else if(offset <= GPIO_IDX_22)
    {
        index = (offset - 16) * 4;
        tmp = PIN->RG_PTS_INMUX_C;
        tmp &= ~ (0xF << index); 
        tmp |= (value << index);
        PIN->RG_PTS_INMUX_C = tmp;
    }
    else
    {
        return 1;
    }
    Hal_PinMux_Config(io_num);
    return 0;
}

uint8_t Hal_PinMux_OutPutConfig(T_Pin pin)
{
    uint32_t tmp;
    uint8_t io_num = pin.io;
    uint8_t value  = pin.col;
    uint8_t index  = 0;
 
    if(io_num <= GPIO_IDX_07)
    {
        index = io_num * 4;
        tmp = PIN->RG_PDI_SRC_IO_A;
        tmp &= ~(0xF << index);
        tmp |=(value << index);
        PIN->RG_PDI_SRC_IO_A = tmp;
    }
    else if(io_num <= GPIO_IDX_14)
    {
        index = (io_num - 8) * 4;
        tmp = PIN->RG_PDI_SRC_IO_B;
        tmp &= ~(0xF << index);
        tmp |=(value << index);
        PIN->RG_PDI_SRC_IO_B = tmp;
        
    }
    else if(io_num <= GPIO_IDX_22)
    {
        index = (io_num - 16) * 4;
        tmp = PIN->RG_PDI_SRC_IO_C;
        tmp &= ~ (0xF << index); 
        tmp |=  (value << index);
        PIN->RG_PDI_SRC_IO_C = tmp;
    }
    else
    {
        return 1;
    }
    Hal_PinMux_Config(io_num);
    return 0;
}

void Hal_PinMux_Config(uint8_t io_num)
{
    uint32_t tmp = 0;
    
    // input Enable
    tmp = PIN->RG_PD_IE;
    tmp |= (0x1 << io_num);
    PIN->RG_PD_IE = tmp;

    // pull-up / pull-down
    tmp = PIN->RG_PD_PE;
    tmp |= (0x1 << io_num);
    PIN->RG_PD_PE = tmp;
 
    // function pin
    tmp = PIN->RG_PDOC_MODE;
    tmp |= (0x1 << io_num);
    PIN->RG_PDOC_MODE = tmp;

    tmp = PIN->RG_PDOV_MODE;
    tmp |= (0x1 << io_num);
    PIN->RG_PDOV_MODE = tmp;
}

void Hal_PinMux_PullDownConfig(uint8_t io)
{
    uint32_t tmp = 0;

    // pull-up / pull-down
    tmp = PIN->RG_PD_PE;
    tmp &= ~(0x01 << io);
    PIN->RG_PD_PE = tmp;

}

void Hal_PinMux_PullUpConfig(uint8_t io)
{
    uint32_t tmp = 0;

    // pull-up / pull-down
    tmp = PIN->RG_PD_PE;
    tmp |= (0x01 << io);
    PIN->RG_PD_PE = tmp;
}
