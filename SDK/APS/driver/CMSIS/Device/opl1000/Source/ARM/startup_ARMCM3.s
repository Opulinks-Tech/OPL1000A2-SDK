;/**************************************************************************//**
; * @file     startup_ARMCM3.s
; * @brief    CMSIS Core Device Startup File for
; *           ARMCM3 Device Series
; * @version  V5.00
; * @date     02. March 2016
; ******************************************************************************/
;/*
; * Copyright (c) 2009-2016 ARM Limited. All rights reserved.
; *
; * SPDX-License-Identifier: Apache-2.0
; *
; * Licensed under the Apache License, Version 2.0 (the License); you may
; * not use this file except in compliance with the License.
; * You may obtain a copy of the License at
; *
; * www.apache.org/licenses/LICENSE-2.0
; *
; * Unless required by applicable law or agreed to in writing, software
; * distributed under the License is distributed on an AS IS BASIS, WITHOUT
; * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; * See the License for the specific language governing permissions and
; * limitations under the License.
; */

;/*
;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
;*/


; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00001000

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000020

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

                IMPORT  IPC0_IRQHandler
                IMPORT  IPC1_IRQHandler
                IMPORT  IPC2_IRQHandler
                IMPORT  IPC3_IRQHandler
                IMPORT  GPIO_IRQHandler
                IMPORT  SCRT_IRQHandler
                IMPORT  MSQ_IRQHandler
                IMPORT  UARTDBG_IRQHandler
                IMPORT  UART0_IRQHandler
                IMPORT  UART1_IRQHandler
                IMPORT  I2C_IRQHandler
                IMPORT  SPI0_IRQHandler
                IMPORT  SPI1_IRQHandler
                IMPORT  SPI2_IRQHandler
                IMPORT  TIM0_IRQHandler
                IMPORT  TIM1_IRQHandler
                IMPORT  WDT_IRQHandler
                IMPORT  JTAG_IRQHandler
                IMPORT  DMA_IRQHandler

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     MemManage_Handler         ; MPU Fault Handler
                DCD     BusFault_Handler          ; Bus Fault Handler
                DCD     UsageFault_Handler        ; Usage Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     DebugMon_Handler          ; Debug Monitor Handler
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                DCD     IPC0_IRQHandler           ;  0:  IPC0
                DCD     IPC1_IRQHandler           ;  1:  IPC1
                DCD     IPC2_IRQHandler           ;  2:  IPC2
                DCD     IPC3_IRQHandler           ;  3:  IPC3
                DCD     GPIO_IRQHandler           ;  4:  PMU GPIO Int:  
                DCD     SCRT_IRQHandler           ;  5:  SCRT:  
                DCD     MSQ_IRQHandler            ;  6:  MSQ2APS
                DCD     UARTDBG_IRQHandler        ;  7:  UARTDBG
                DCD     UART0_IRQHandler          ;  8:  UART0 
                DCD     UART1_IRQHandler          ;  9:  UART1 
                DCD     I2C_IRQHandler            ; 10: I2C
                DCD     SPI0_IRQHandler           ; 11: SPI0
                DCD     SPI1_IRQHandler           ; 12: SPI1
                DCD     SPI2_IRQHandler           ; 13: SPI2
                DCD     TIM0_IRQHandler           ; 14: Timer0
                DCD     TIM1_IRQHandler           ; 15: Timer1
                DCD     WDT_IRQHandler            ; 16: Watchdog
                DCD     JTAG_IRQHandler           ; 17: JTAG debug
                DCD     DMA_IRQHandler            ; 18: DMA
                DCD     0                         ; 19: Reserved
                DCD     0                         ; 20: Reserved
                DCD     0                         ; 21: Reserved
                DCD     0                         ; 22: Reserved
                DCD     0                         ; 23: Reserved
                DCD     0                         ; 24: Reserved
                DCD     0                         ; 25: Reserved
                DCD     0                         ; 26: Reserved
                DCD     0                         ; 27: Reserved
                DCD     0                         ; 28: Reserved - CPU FPGA CLCD
                DCD     0                         ; 29: Reserved - CPU FPGA
                DCD     0                         ; 30: Reserved
                DCD     0                         ; 31: Reserved
__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  Boot_CheckStrapEarly
                IMPORT  Boot_CheckWarmBoot
                IMPORT  SystemInit
                IMPORT  __main
                IMPORT  Boot_MainPatch
                
                LDR     R0, = 0x0
                LDR     R1, = 0x0
                LDR     R2, = 0x0
                LDR     R3, = 0x0
                LDR     R4, = 0x0
                LDR     R5, = 0x0
                LDR     R6, = 0x0
                LDR     R7, = 0x0

                MOV     R8, R0
                MOV     R9, R0
                MOV     R10, R0
                MOV     R11, R0
                MOV     R12, R0

                MOV     R14, R0
                
                LDR     R0, =Boot_CheckStrapEarly
                BLX     R0

                LDR     R0, =Boot_CheckWarmBoot
                BLX     R0
                CMP     R0, #1
                BEQ     WarmBoot

                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
WarmBoot
                LDR     R0, =Boot_MainPatch
                LDR     R0, [R0]
                BX      R0
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler         [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler          [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler        [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler          [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC
                B       .

                ENDP


                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap

__user_initial_stackheap PROC
                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR
                ENDP

                ALIGN

                ENDIF


                END
