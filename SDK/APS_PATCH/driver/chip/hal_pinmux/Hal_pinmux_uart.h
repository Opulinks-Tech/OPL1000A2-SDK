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

#ifndef _PINMUX_UART__H_
#define _PINMUX_UART__H_

#include "OPL1000_pin_mux_define.h"
#include "Hal_pinmux_common.h"
#include "hal_uart.h"

/**
 * @brief  Pinmux uart init    
 *
 * @param[in] the struct of uart
 *         
 * @return - 0: successful
 *         - other: failed        
 */
ERR_CODE Hal_PinMux_Uart_Init(T_OPL1000_uart *uart);

#endif

