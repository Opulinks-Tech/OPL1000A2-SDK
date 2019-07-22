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


#include "Hal_pinmux_pwm.h"


uint8_t Hal_PinMux_Get_Index(uint8_t pin)
{
    uint8_t pwm_index = 0;

    switch(pin)
    {
        case OPL1000_IO23_PIN:
            pwm_index = HAL_PWM_IDX_0;
            break;
        case OPL1000_IO22_PIN:
            pwm_index = HAL_PWM_IDX_1;
            break;
        case OPL1000_IO21_PIN:
            pwm_index = HAL_PWM_IDX_2;
            break;
        case OPL1000_IO20_PIN:
            pwm_index = HAL_PWM_IDX_3;
            break;
        case OPL1000_IO19_PIN:
            pwm_index = HAL_PWM_IDX_4;
            break;
        case OPL1000_IO18_PIN:
            pwm_index = HAL_PWM_IDX_5;
            break;
        default:
            break;
    }
    return pwm_index;
}

ERR_CODE Hal_Pinmux_Pwm_Init()
{
    Hal_Pwm_Init();	
	  return RET_SUCCESS;
}
ERR_CODE Hal_Pinmux_Pwm_Disable(uint8_t ubIdxMask)
{
	  if((ubIdxMask > HAL_PWM_IDX_ALL) || (ubIdxMask < HAL_PWM_IDX_0))
	  {
			printf("PWM index exceed limitation! \r\n");
			return RET_FAIL;
		}
		else 
		{
	    Hal_Pwm_Disable(ubIdxMask);	
			return RET_SUCCESS;
		}
}
ERR_CODE Hal_Pinmux_Pwm_Enable(uint8_t ubIdxMask)
{
	  if((ubIdxMask > HAL_PWM_IDX_ALL) || (ubIdxMask < HAL_PWM_IDX_0)) 
	  {
			printf("PWM index exceed limitation! \r\n");
			return RET_FAIL;
		}
		else 
		{
	    Hal_Pwm_Enable(ubIdxMask);	
			return RET_SUCCESS;
		}
}
ERR_CODE Hal_Pinmux_Pwm_Config(T_OPL1000_Pwm *pwm)
{
    uint8_t pwm_index = 0;
    S_Hal_Pwm_Config_t sConfig;

	  pwm_index = Hal_PinMux_Get_Index(pwm->pin);	
    if(pwm_index != 0)
    {
        		
        printf("pwm_index = %d , clock source = %d \r\n",pwm_index,pwm->clkSrc);
        Hal_Pwm_ClockSourceSet(pwm->clkSrc);
        if(pwm->cfgType == CFG_SIMPLE)
        {
            printf("Hal_Pwm_SimpleConfigSet \r\n");
            Hal_Pwm_SimpleConfigSet(pwm_index,pwm->duty, pwm->clkHz);
        }
        else if(pwm->cfgType == CFG_COMPLEX)
        {
            sConfig.ulPeriod     = pwm->period;
            sConfig.ulDutyBright = pwm->dutyBright;
            sConfig.ulDutyDull   = pwm->dutyDull;
            sConfig.ulRampUp     = pwm->rampUp;
            sConfig.ulRampDown   = pwm->rampDown;
            sConfig.ulHoldBright = pwm->holdBright;
            sConfig.ulHoldDull   = pwm->holdDull;
            printf("Hal_Pwm_ComplexConfigSet \r\n");
            Hal_Pwm_ComplexConfigSet(pwm_index, sConfig);
        }

        Hal_PinMux_OutPutConfig(Hal_Get_Point(pwm->pin,PWM_E));
    }
    else
    {
        printf("pwm pin define (%d) error \r\n",pwm->pin);
			  return RET_FAIL;
    }

    return RET_SUCCESS;
}




