/**************************************************************************//**
 * @file     ARMCM3.h
 * @brief    CMSIS Core Peripheral Access Layer Header File for
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

#ifndef ARMCM3_H
#define ARMCM3_H

#ifdef __cplusplus
extern "C" {
#endif


/* -------------------------  Interrupt Number Definition  ------------------------ */

typedef enum IRQn
{
/* -------------------  Cortex-M3 Processor Exceptions Numbers  ------------------- */
  NonMaskableInt_IRQn           = -14,      /*!<  2 Non Maskable Interrupt          */
  HardFault_IRQn                = -13,      /*!<  3 HardFault Interrupt             */
  MemoryManagement_IRQn         = -12,      /*!<  4 Memory Management Interrupt     */
  BusFault_IRQn                 = -11,      /*!<  5 Bus Fault Interrupt             */
  UsageFault_IRQn               = -10,      /*!<  6 Usage Fault Interrupt           */
  SVCall_IRQn                   =  -5,      /*!< 11 SV Call Interrupt               */
  DebugMonitor_IRQn             =  -4,      /*!< 12 Debug Monitor Interrupt         */
  PendSV_IRQn                   =  -2,      /*!< 14 Pend SV Interrupt               */
  SysTick_IRQn                  =  -1,      /*!< 15 System Tick Interrupt           */

/* ----------------------  ARMCM3 Specific Interrupt Numbers  --------------------- */
    IPC0_IRQn                   = 0,
    IPC1_IRQn                   = 1,
    IPC2_IRQn                   = 2,
    IPC3_IRQn                   = 3,
    GPIO_IRQn                   = 4,
    SCRT_IRQn                   = 5,
    MSQ2APS_IRQn                = 6,
    UARTDBG_IRQn                = 7,
    UART0_IRQn                  = 8,
    UART1_IRQn                  = 9,
    I2C_IRQn                    = 10,
    SPI0_IRQn                   = 11,
    SPI1_IRQn                   = 12,
    SPI2_IRQn                   = 13,
    TIM0_IRQn                   = 14,
    TIM1_IRQn                   = 15,
    WDT_IRQn                    = 16,
    JTAG_IRQn                   = 17,       /*!< JTAG Debug Port Interrupt          */
    DMA_IRQn                    = 18,       /*!< APS DMA Interrupt                  */
    IRQn_MAX
} IRQn_Type;

/* --------  Configuration of the Cortex-M3 Processor and Core Peripherals  ------- */
#define __CM3_REV                 0x0201U   /* Core revision r2p1 */
#define __MPU_PRESENT             0U        /* MPU present */
#define __NVIC_PRIO_BITS          4U        /* Number of Bits used for Priority Levels */
#define __Vendor_SysTickConfig    0U        /* Set to 1 if different SysTick Config is used */

#include "core_cm3.h"                       /* Processor and core peripherals */
#include "system_ARMCM3.h"                  /* System Header */

#define IRQ_PRIORITY_IPC0       0x04        /* will be left shift 4 bits */
#define IRQ_PRIORITY_IPC1       0x04
#define IRQ_PRIORITY_IPC2       0x04
#define IRQ_PRIORITY_IPC3       0x04
#define IRQ_PRIORITY_GPIO       0x0A
#define IRQ_PRIORITY_SCRT       0x0A
#define IRQ_PRIORITY_MSQ        0x0A
#define IRQ_PRIORITY_UARTDBG    0x0A
#define IRQ_PRIORITY_UART0      0x0A
#define IRQ_PRIORITY_UART1      0x0A
#define IRQ_PRIORITY_I2C        0x08
#define IRQ_PRIORITY_SPI0       0x0A
#define IRQ_PRIORITY_SPI1       0x0A
#define IRQ_PRIORITY_SPI2       0x0A
#define IRQ_PRIORITY_TIM0       0x0A
#define IRQ_PRIORITY_TIM1       0x0A
#define IRQ_PRIORITY_WDT        0x02
#define IRQ_PRIORITY_JTAG       0x0A
#define IRQ_PRIORITY_DMA        0x0A

#define IRQ_PRIORITY_SYS        0x10         /* directly value, it is used for critical section in FreeRTOS */
#define IRQ_PRIORITY_TICK       0xF0         /* directly value, it is used for sys tick in FreeRTOS */


/* ================================================================================ */
/* ================              Peripheral memory map             ================ */
/* ================================================================================ */
#define APS_PERIPH_BASE         ((uint32_t)0x30000000)
#define MSQ_PERIPH_BASE         ((uint32_t)0x40000000)

/* APS (M3)*/
#define UART_DBG_BASE           (APS_PERIPH_BASE + 0x1000)
#define OTP_BASE                (APS_PERIPH_BASE + 0x2000)
#define DMA_BASE                (APS_PERIPH_BASE + 0x3000)
#define UART0_BASE              (APS_PERIPH_BASE + 0x4000)
#define UART1_BASE              (APS_PERIPH_BASE + 0x5000)
#define I2C_BASE                (APS_PERIPH_BASE + 0x6000)
#define SPI0_BASE               (APS_PERIPH_BASE + 0x7000)
#define SPI1_BASE               (APS_PERIPH_BASE + 0x8000)
#define SPI2_BASE               (APS_PERIPH_BASE + 0x9000)
#define TIM0_BASE               (APS_PERIPH_BASE + 0xA000)
#define TIM1_BASE               (APS_PERIPH_BASE + 0xB000)
#define WDT_BASE                (APS_PERIPH_BASE + 0xC000)
#define VIC_BASE                (APS_PERIPH_BASE + 0xD000)
#define SYS_BASE                (APS_PERIPH_BASE + 0xE000)
#define PWM_BASE                (APS_PERIPH_BASE + 0xF000)

/* MSQ (M0) */
#define AOS_BASE                (MSQ_PERIPH_BASE + 0x1000)
#define RF_BASE                 (MSQ_PERIPH_BASE + 0x9000)


#ifdef __cplusplus
}
#endif

#endif  /* ARMCM3_H */
