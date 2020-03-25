/* *****************************************************************************
 *  Copyright 2019, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2018
 *
 *******************************************************************************
 *
 *  @file opl1000_it_patch.h
 * 
 *  @brief 
 *  
 ******************************************************************************/

#ifndef _OPL1000_IT_PATCH_H_
#define _OPL1000_IT_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "opl1000_it.h"
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


typedef void (*T_InterruptHandler)(void);

/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */

extern T_InterruptHandler IPC0_IRQHandler_Entry;
extern T_InterruptHandler IPC1_IRQHandler_Entry;
extern T_InterruptHandler IPC2_IRQHandler_Entry;
extern T_InterruptHandler IPC3_IRQHandler_Entry;
extern T_InterruptHandler GPIO_IRQHandler_Entry;
extern T_InterruptHandler SCRT_IRQHandler_Entry;
extern T_InterruptHandler MSQ_IRQHandler_Entry;
extern T_InterruptHandler UARTDBG_IRQHandler_Entry;
extern T_InterruptHandler UART0_IRQHandler_Entry;
extern T_InterruptHandler UART1_IRQHandler_Entry;
extern T_InterruptHandler I2C_IRQHandler_Entry;
extern T_InterruptHandler SPI0_IRQHandler_Entry;
extern T_InterruptHandler SPI1_IRQHandler_Entry;
extern T_InterruptHandler SPI2_IRQHandler_Entry;
extern T_InterruptHandler TIM0_IRQHandler_Entry;
extern T_InterruptHandler TIM1_IRQHandler_Entry;
extern T_InterruptHandler WDT_IRQHandler_Entry;
extern T_InterruptHandler JTAG_IRQHandler_Entry;
extern T_InterruptHandler DMA_IRQHandler_Entry;

/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */

void ISR_Pre_PatchInit(void);
#ifdef __cplusplus
}
#endif
#endif  /* _OPL1000_IT_PATCH_H_ */
