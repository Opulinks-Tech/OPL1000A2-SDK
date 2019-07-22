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
 * @file hal_vic_patch.c
 * 
 * @brief Patch for VIC API patch
 *  
 *******************************************************************************/

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "hal_vic_patch.h"
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
extern void Hal_Vic_GpioPinmux_impl(E_GpioIdx_t eIdx, E_GpioDirection_t tDirection, E_GpioLevel_t tOutputLevel);
void Hal_Vic_GpioPinmux_patch(E_GpioIdx_t eIdx, E_GpioDirection_t tDirection, E_GpioLevel_t tOutputLevel);
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

void Hal_Vic_PatchInit(void)
{
    Hal_Vic_GpioPinmux = Hal_Vic_GpioPinmux_patch;
}

/*
 *************************************************************************
 *                          Private Functions
 *************************************************************************
 */


/** 
 * @brief set the pinmux to input or output
 *
 * @param eIdx [in] Index of call-back GPIO
 * @param tDirection [in] the GPIO direction
 * @param tOutputLevel [in] the output level
 */
void Hal_Vic_GpioPinmux_patch(E_GpioIdx_t eIdx, E_GpioDirection_t tDirection, E_GpioLevel_t tOutputLevel)
{
    Hal_Gpio_SleepIoAutoCtrlSet(eIdx, SLEEP_OUTPUT_DISABLE);
    Hal_Vic_GpioPinmux_impl(eIdx, tDirection, tOutputLevel);
}



