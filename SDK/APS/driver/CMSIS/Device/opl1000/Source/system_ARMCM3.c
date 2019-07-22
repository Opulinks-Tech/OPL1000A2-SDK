/**************************************************************************//**
 * @file     system_ARMCM3.c
 * @brief    CMSIS Device System Source File for
 *           ARMCM3 Device Series
 * @version  V5.00
 * @date     07. September 2016
 ******************************************************************************/
/*
 * Copyright (c) 2009-2016 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "opl1000.h"
#include "hal_tick.h"

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
#define  SYSTEM_CLOCK    XTAL


/*----------------------------------------------------------------------------
  System Core Clock Variable
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock = SYSTEM_CLOCK;
uint32_t SystemCoreDivFactor = 16;


/*----------------------------------------------------------------------------
  System Core Clock update function
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate (void)
{
    /*
    * Note: It is CMSIS standard. see http://www.keil.com/pack/doc/CMSIS/Core/html/group__system__init__gr.html 
    * By definition:
    *     Updates the variable SystemCoreClock and must be called whenever the core clock is changed during program execution. 
    *     The function evaluates the clock register settings and calculates the current core clock. 
    */
    // No need do anything in our device...
}

uint32_t SystemCoreClockGet (void)
{
    return SystemCoreClock*SystemCoreDivFactor/16;
}

void SystemCoreClockSet (uint32_t clkHz)
{
    SystemCoreClock = clkHz;
}

void SystemCoreClockDivFactorSet (uint32_t factor)
{
    SystemCoreDivFactor = factor;
}

/*----------------------------------------------------------------------------
  System initialization function
 *----------------------------------------------------------------------------*/
void SystemInit (void)
{
    SystemCoreClock = SYSTEM_CLOCK;
    Hal_Tick_Init();
}
