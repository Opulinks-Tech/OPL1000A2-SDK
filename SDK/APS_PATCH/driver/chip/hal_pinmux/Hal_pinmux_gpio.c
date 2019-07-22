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

#include "Hal_pinmux_gpio.h"

#define PIN_TOTAL 18

extern T_Pin_Unit const pin_map_table[];


E_GpioIdx_t Hal_Pinmux_GetIO(uint8_t io)
{
    uint8_t index;
    for(index = 0;index < PIN_TOTAL; index++)
    {
        if(pin_map_table[index].sour == io)
        {
             return  pin_map_table[index].dist;
        }
    }
    return GPIO_IDX_MAX;
}
                                                                            

ERR_CODE Hal_Pinmux_Gpio_Init(T_OPL1000_Gpio *gpio)
{
    E_GpioIdx_t gpioIdx; 
    gpioIdx = Hal_Pinmux_GetIO(gpio->pin);
    if(gpioIdx < GPIO_IDX_MAX)
    {
        if(gpio->type == IO_OUTPUT)                                        
        {
            //Hal_Vic_GpioDirection(gpioIdx, GPIO_OUTPUT);
            if(gpio->pull == PULL_DOWN)                                     
            {
                //Hal_Vic_GpioOutput(gpioIdx, GPIO_LEVEL_LOW);  
                Hal_Vic_GpioPinmux(gpioIdx, GPIO_OUTPUT, GPIO_LEVEL_LOW);							
            }
            else if(gpio->pull == PULL_UP)                                 
            {
                //Hal_Vic_GpioOutput(gpioIdx, GPIO_LEVEL_HIGH); 
                Hal_Vic_GpioPinmux(gpioIdx, GPIO_OUTPUT, GPIO_LEVEL_HIGH);								
            }
        }
        else if(gpio->type == IO_INPUT)                                 
        {
            Hal_Vic_GpioDirection(gpioIdx, GPIO_INPUT); 
        }
        else
        {
            return RET_FAIL;
        }
    }

    return RET_SUCCESS;
}








