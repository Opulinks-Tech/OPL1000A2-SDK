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

#ifndef _PINMUX_I2C__H_
#define _PINMUX_I2C__H_

#include "OPL1000_pin_mux_define.h"
#include "Hal_pinmux_common.h"

/**
 * @brief  Pinmux I2C slave init    
 *
 * @param[in] the struct of I2C   
 *           
 * @return - 0: successful
 *         - other: failed        
 */
extern ERR_CODE Hal_Pinmux_I2c_Slave_Init(T_OPL1000_I2c *periph);

/**
 * @brief  Pinmux I2C master init    
 *
 * @param[in] the struct of I2C
 *            
 * @return - 0: successful
 *         - other: failed   
 */
extern ERR_CODE Hal_Pinmux_I2C_Master_init(T_OPL1000_I2c *i2c);


#endif


