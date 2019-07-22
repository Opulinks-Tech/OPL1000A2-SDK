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
*  hal_pwm.c
*
*  Project:
*  --------
*  OPL1000 Project - the PWM implement file
*
*  Description:
*  ------------
*  This implement file is include the PWM function and api.
*
*  Author:
*  -------
*  Jeff Kuo
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include "opl1000.h"
#include "hal_system.h"
#include "hal_pwm.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define PWM                         ((S_Pwm_Reg_t *) PWM_BASE)

/* PWM Register Definitions */
#define PWM_MODE_POS                0U
#define PWM_MODE_MASK               (0x01UL << PWM_MODE_POS)
#define PWM_MODE_INDIVIDUAL         (0x00UL << PWM_MODE_POS)
#define PWM_MODE_SYNC               (0x01UL << PWM_MODE_POS)

#define PWM_SYNC_POS                1U
#define PWM_SYNC_MASK               (0x01UL << PWM_SYNC_POS)
#define PWM_SYNC_DISABLE            (0x00UL << PWM_SYNC_POS)
#define PWM_SYNC_ENABLE             (0x01UL << PWM_SYNC_POS)

#define PWM_IDX_ENABLE_POS          8U
#define PWM_IDX_ENABLE_MASK         (HAL_PWM_IDX_ALL << PWM_IDX_ENABLE_POS)

#define PWM_RAMP_UP_POS             0U
#define PWM_RAMP_UP_MASK            (0xFFUL << PWM_RAMP_UP_POS)

#define PWM_RAMP_DOWN_POS           8U
#define PWM_RAMP_DOWN_MASK          (0xFFUL << PWM_RAMP_DOWN_POS)

#define PWM_PERIOD_POS              16U
#define PWM_PERIOD_MASK             (0xFFUL << PWM_PERIOD_POS)

#define PWM_HOLD_BRIGHT_POS         0U
#define PWM_HOLD_BRIGHT_MASK        (0x01FFUL << PWM_HOLD_BRIGHT_POS)

#define PWM_HOLD_DULL_POS           16U
#define PWM_HOLD_DULL_MASK          (0x01FFUL << PWM_HOLD_DULL_POS)

#define PWM_DUTY_BRIGHT_POS         0U
#define PWM_DUTY_BRIGHT_MASK        (0xFFUL << PWM_DUTY_BRIGHT_POS)

#define PWM_DUTY_DULL_POS           16U
#define PWM_DUTY_DULL_MASK          (0xFFUL << PWM_DUTY_DULL_POS)

// the default value of 32KHz and 22MHz
#define PWM_32KHZ                   32768UL
#define PWM_22MHZ                   XTAL


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list
typedef struct
{
    volatile uint32_t Ctrl;                 /* Offset: 0x000      (R/W) PWM Control Register */
    volatile uint32_t Period[6];            /* Offset: 0x004~0x18 (R/W) PWM Ramp and Period Register */
    volatile uint32_t Hold[6];              /* Offset: 0x01C~0x30 (R/W) PWM Hold Time Register */
    volatile uint32_t Duty[6];              /* Offset: 0x034~0x48 (R/W) PWM Duty Register */
} S_Pwm_Reg_t;


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
RET_DATA uint32_t g_ulHalPwm_32kValue;

RET_DATA T_Hal_Pwm_InitFp Hal_Pwm_Init;
RET_DATA T_Hal_Pwm_EnableFp Hal_Pwm_Enable;
RET_DATA T_Hal_Pwm_SyncEnableFp Hal_Pwm_SyncEnable;
RET_DATA T_Hal_Pwm_DisableFp Hal_Pwm_Disable;
RET_DATA T_Hal_Pwm_ClockSourceSetFp Hal_Pwm_ClockSourceSet;
RET_DATA T_Hal_Pwm_SimpleConfigSetFp Hal_Pwm_SimpleConfigSet;
RET_DATA T_Hal_Pwm_ComplexConfigSetFp Hal_Pwm_ComplexConfigSet;


// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable
static uint32_t g_ulHalPwm_CurrentClock;


// Sec 7: declaration of static function prototype


/***********
C Functions
***********/
// Sec 8: C Functions

/*************************************************************************
* FUNCTION:
*   Hal_Pwm_Init
*
* DESCRIPTION:
*   initial
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Hal_Pwm_Init_impl(void)
{
    Hal_Sys_ApsClkEn(1, APS_CLK_PWM);
    
    // reset the control register
    PWM->Ctrl = 0;

    // set the default clock source to 32KHz
    Hal_Sys_PwmSrcSelect(PWM_CLK_SRC_32KHZ);
    g_ulHalPwm_CurrentClock = g_ulHalPwm_32kValue;
}

/*************************************************************************
* FUNCTION:
*   Hal_Pwm_Enable
*
* DESCRIPTION:
*   enable the index of PWM
*
* PARAMETERS
*   1. ubIdxMask       : [In] the bit mask of index
*
* RETURNS
*   none
*
*************************************************************************/
void Hal_Pwm_Enable_impl(uint8_t ubIdxMask)
{
    uint32_t temp;

    // error handler
    ubIdxMask = ubIdxMask & HAL_PWM_IDX_ALL;
    
    temp = PWM->Ctrl;
    temp |= (ubIdxMask << PWM_IDX_ENABLE_POS);
    PWM->Ctrl = temp;
}

/*************************************************************************
* FUNCTION:
*   Hal_Pwm_SyncEnable
*
* DESCRIPTION:
*   enable the index of PWM synchronously
*
* PARAMETERS
*   1. ubIdxMask       : [In] the bit mask of index
*
* RETURNS
*   none
*
*************************************************************************/
void Hal_Pwm_SyncEnable_impl(uint8_t ubIdxMask)
{
    // error handler
    ubIdxMask = ubIdxMask & HAL_PWM_IDX_ALL;

    // step 1. mode
    PWM->Ctrl = PWM_MODE_SYNC;
    // step 2. index
    PWM->Ctrl = PWM_MODE_SYNC | (ubIdxMask << PWM_IDX_ENABLE_POS);
    // step 3. sync start
    PWM->Ctrl = PWM_MODE_SYNC | PWM_SYNC_ENABLE | (ubIdxMask << PWM_IDX_ENABLE_POS);
}

/*************************************************************************
* FUNCTION:
*   Hal_Pwm_Disable
*
* DESCRIPTION:
*   enable the index of PWM
*
* PARAMETERS
*   1. ubIdxMask       : [In] the bit mask of index
*
* RETURNS
*   none
*
*************************************************************************/
void Hal_Pwm_Disable_impl(uint8_t ubIdxMask)
{
    uint32_t temp;

    // error handler
    ubIdxMask = ubIdxMask & HAL_PWM_IDX_ALL;
    
    temp = PWM->Ctrl;
    temp &= ~(ubIdxMask << PWM_IDX_ENABLE_POS);
    PWM->Ctrl = temp;
}

/*************************************************************************
* FUNCTION:
*   Hal_Pwm_ClockSourceSet
*
* DESCRIPTION:
*   set the PWM clock source
*
* PARAMETERS
*   1. ubSrc       : [In] the type of clock source
*
* RETURNS
*   none
*
*************************************************************************/
void Hal_Pwm_ClockSourceSet_impl(uint8_t ubSrc)
{
    if (HAL_PWM_CLK_32K == ubSrc)
    {
        Hal_Sys_PwmSrcSelect(PWM_CLK_SRC_32KHZ);
        g_ulHalPwm_CurrentClock = g_ulHalPwm_32kValue;
    }
    else if (HAL_PWM_CLK_22M == ubSrc)
    {
        Hal_Sys_PwmSrcSelect(PWM_CLK_SRC_22MHZ);
        g_ulHalPwm_CurrentClock = PWM_22MHZ;
    }
}

/*************************************************************************
* FUNCTION:
*   Hal_Pwm_SimpleConfigSet
*
* DESCRIPTION:
*   set the simple config of PWM
*
* PARAMETERS
*   1. ubIdxMask       : [In] the bit mask of index
*   2. ubDuty          : [In] the percentage of duty
*   3. ulHz            : [In] the frequency (Hz)
*
* RETURNS
*   HAL_PWM_RET_FAIL   : fail
*   HAL_PWM_RET_PASS   : pass
*
*************************************************************************/
uint8_t Hal_Pwm_SimpleConfigSet_impl(uint8_t ubIdxMask, uint8_t ubDuty, uint32_t ulHz)
{
    uint32_t ulTickPerCycle;
    uint32_t ulPeriod;          // the tick count of one clock cycle
    uint32_t ulDutyBright;      // the max tick count of high level in one clock cycle
    uint32_t ulDutyDull;        // the min tick count of high level in one clock cycle
    uint32_t ulRampUp;          // the delta count from dull to bright per clock cycle
    uint32_t ulRampDown;        // the delta count from bright to dull per clock cycle
    uint32_t ulHoldBright;      // the hold times of the bright state
    uint32_t ulHoldDull;        // the hold times the dull state
    
    uint8_t bRet = HAL_PWM_RET_FAIL;
    uint8_t i;

    // error handler
    ubIdxMask = ubIdxMask & HAL_PWM_IDX_ALL;
    if (0 == ubIdxMask)
    {
        goto done;
    }

    // error check
    // the max frequency
    if (ulHz > g_ulHalPwm_CurrentClock)
    {
        goto done;
    }
    // the zero frequency
    if (ulHz == 0)
    {
        goto done;
    }
    // the max duty
    if (ubDuty > 100)
    {
        goto done;
    }

    // compute the tick count of one clock cycle
    ulTickPerCycle = (g_ulHalPwm_CurrentClock + (ulHz / 2)) / ulHz;     // rounding
    // case 1: the period range is enough
    if (ulTickPerCycle <= HAL_PWM_MAX_PERIOD)
    {
        ulPeriod = ulTickPerCycle;
        ulDutyBright = ((ulPeriod * ubDuty) + (100 / 2)) / 100;         // rounding
        ulDutyDull = ulDutyBright;
        ulRampUp = ulDutyBright;
        ulRampDown = 0;
        ulHoldBright = 1;
        ulHoldDull = 1;
    }
    // case 2: the period range is not enough
    else
    {
        ulPeriod = (ulTickPerCycle + (HAL_PWM_MAX_HOLD - 1)) / HAL_PWM_MAX_HOLD;    // ceiling
        // error check period
        // min: can't be zero
        if (ulPeriod < HAL_PWM_MIN_PERIOD)
            ulPeriod = HAL_PWM_MIN_PERIOD;
        // max: overflow
        else if (ulPeriod > HAL_PWM_MAX_PERIOD)
        {
            goto done;
        }
        ulDutyBright = ulPeriod;
        ulDutyDull = 0;
        ulRampUp = ulPeriod;
        ulRampDown = ulPeriod;
        ulHoldBright = (((ulTickPerCycle / ulPeriod) * ubDuty) + (100 / 2)) / 100;  // rounding
        ulHoldDull = (ulTickPerCycle / ulPeriod) - ulHoldBright;
    }

    // set the value
    for (i=0; i<HAL_PWM_MAX; i++)
    {
        if (ubIdxMask & (0x01 << i))
        {
            PWM->Period[i] = (ulPeriod << PWM_PERIOD_POS) | (ulRampDown << PWM_RAMP_DOWN_POS) | (ulRampUp << PWM_RAMP_UP_POS);
            PWM->Hold[i] = (ulHoldDull << PWM_HOLD_DULL_POS) | (ulHoldBright << PWM_HOLD_BRIGHT_POS);
            PWM->Duty[i] = (ulDutyDull << PWM_DUTY_DULL_POS) | (ulDutyBright << PWM_DUTY_BRIGHT_POS);
        }
    }

    bRet = HAL_PWM_RET_PASS;

done:
    return bRet;
}

/*************************************************************************
* FUNCTION:
*   Hal_Pwm_ComplexConfigSet
*
* DESCRIPTION:
*   set the complex config of PWM
*
* PARAMETERS
*   1. ubIdxMask       : [In] the bit mask of index
*   2. tConfig         : [In] the complex config
*
* RETURNS
*   HAL_PWM_RET_FAIL   : fail
*   HAL_PWM_RET_PASS   : pass
*
*************************************************************************/
uint8_t Hal_Pwm_ComplexConfigSet_impl(uint8_t ubIdxMask, S_Hal_Pwm_Config_t tConfig)
{
    uint8_t bRet = HAL_PWM_RET_FAIL;
    uint8_t i;

    // error handler
    ubIdxMask = ubIdxMask & HAL_PWM_IDX_ALL;
    if (0 == ubIdxMask)
    {
        goto done;
    }

    // error check
    // peroid 
    if ((tConfig.ulPeriod < HAL_PWM_MIN_PERIOD) || (tConfig.ulPeriod > HAL_PWM_MAX_PERIOD))
    {
        goto done;
    }
    // duty
    if ((tConfig.ulDutyBright > HAL_PWM_MAX_DUTY) || (tConfig.ulDutyDull > HAL_PWM_MAX_DUTY))
    {
        goto done;
    }
    // ramp
    if ((tConfig.ulRampUp > HAL_PWM_MAX_RAMP) || (tConfig.ulRampDown > HAL_PWM_MAX_RAMP))
    {
        goto done;
    }
    // hold
    if ((tConfig.ulHoldBright < HAL_PWM_MIN_HOLD) || (tConfig.ulHoldBright > HAL_PWM_MAX_HOLD)
        || (tConfig.ulHoldDull < HAL_PWM_MIN_HOLD) || (tConfig.ulHoldDull > HAL_PWM_MAX_HOLD))
    {
        goto done;
    }

    // set the value
    for (i=0; i<HAL_PWM_MAX; i++)
    {
        if (ubIdxMask & (0x01 << i))
        {
            PWM->Period[i] = (tConfig.ulPeriod << PWM_PERIOD_POS) | (tConfig.ulRampDown << PWM_RAMP_DOWN_POS) | (tConfig.ulRampUp << PWM_RAMP_UP_POS);
            PWM->Hold[i] = (tConfig.ulHoldDull << PWM_HOLD_DULL_POS) | (tConfig.ulHoldBright << PWM_HOLD_BRIGHT_POS);
            PWM->Duty[i] = (tConfig.ulDutyDull << PWM_DUTY_DULL_POS) | (tConfig.ulDutyBright << PWM_DUTY_BRIGHT_POS);
        }
    }

    bRet = HAL_PWM_RET_PASS;

done:
    return bRet;
}

/*************************************************************************
* FUNCTION:
*   Hal_Pwm_PreInitCold
*
* DESCRIPTION:
*   the previous initial for cold boot
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Hal_Pwm_PreInitCold(void)
{
    g_ulHalPwm_32kValue = PWM_32KHZ;

    Hal_Pwm_Init = Hal_Pwm_Init_impl;
    Hal_Pwm_Enable = Hal_Pwm_Enable_impl;
    Hal_Pwm_SyncEnable = Hal_Pwm_SyncEnable_impl;
    Hal_Pwm_Disable = Hal_Pwm_Disable_impl;
    Hal_Pwm_ClockSourceSet = Hal_Pwm_ClockSourceSet_impl;
    Hal_Pwm_SimpleConfigSet = Hal_Pwm_SimpleConfigSet_impl;
    Hal_Pwm_ComplexConfigSet = Hal_Pwm_ComplexConfigSet_impl;
}
