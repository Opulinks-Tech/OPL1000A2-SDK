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

#ifndef _PINMUX_PWM__H_
#define _PINMUX_PWM__H_

#include "Hal_pinmux_common.h"
#include "hal_pwm.h"

/**
 * @brief  Pinmux pwm init    
 *
 * @param[in] none
 *                
 * @return - 0: successful
 *         - other: failed
 */
ERR_CODE Hal_Pinmux_Pwm_Init(void);

/**
 * @brief  Pinmux pwm port configuration   
 *
 * @param[in] pointer of pwm parameter struct 
 *                
 * @return - 0: successful
 *         - other: failed
 */
ERR_CODE Hal_Pinmux_Pwm_Config(T_OPL1000_Pwm *pwm);

/**
 * @brief  Pinmux pwm enable operation   
 *
 * @param[in] pwm index mask byte 
 *                
 * @return - 0: successful
 *         - other: failed
 */
ERR_CODE Hal_Pinmux_Pwm_Enable(uint8_t ubIdxMask);

/**
 * @brief  Pinmux pwm disable operation     
 *
 * @param[in] pwm index mask byte
 *                
 * @return - 0: successful
 *         - other: failed
 */
ERR_CODE Hal_Pinmux_Pwm_Disable(uint8_t ubIdxMask);

/**
 * @brief  Get certain pwm port index      
 *
 * @param[in] pwm port pin number 
 *                
 * @return  pwm index mask 
 *  
 */
uint8_t Hal_PinMux_Get_Index(uint8_t pin);

#endif

