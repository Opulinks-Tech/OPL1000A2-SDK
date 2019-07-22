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

//#include "hal_i2c.h"
#include "Hal_pinmux_i2c.h"


ERR_CODE Hal_Pinmux_I2C_Master_init(T_OPL1000_I2c *i2c)
{
    
    Hal_PinMux_OutPutConfig(Hal_Get_Point(i2c->scl,I2C_SCL_OUT_E));
    Hal_PinMux_OutPutConfig(Hal_Get_Point(i2c->sda,I2C_SDA_OUT_E));
    Hal_PinMux_PullDownConfig(Hal_Get_Point(i2c->scl,I2C_SCL_IN_E).io);
    Hal_PinMux_PullDownConfig(Hal_Get_Point(i2c->sda,I2C_SDA_IN_E).io);
    Hal_PinMux_InPutConfig(Hal_Get_Point(i2c->scl,I2C_SCL_OUT_E));
    Hal_PinMux_InPutConfig(Hal_Get_Point(i2c->sda,I2C_SDA_OUT_E));
    
    // init I2C
    Hal_I2c_MasterInit(i2c->addr,i2c->type);

    // set the target address
    Hal_I2c_TargetAddrSet(i2c->address);
      
    return RET_SUCCESS;
}


ERR_CODE Hal_Pinmux_I2c_Slave_Init(T_OPL1000_I2c *i2c)
{
    Hal_PinMux_OutPutConfig(Hal_Get_Point(i2c->scl,I2C_SCL_OUT_E));
    Hal_PinMux_OutPutConfig(Hal_Get_Point(i2c->sda,I2C_SDA_OUT_E));

    Hal_PinMux_InPutConfig(Hal_Get_Point(i2c->scl,I2C_SCL_IN_E));
    Hal_PinMux_InPutConfig(Hal_Get_Point(i2c->sda,I2C_SDA_IN_E));  
    
    Hal_PinMux_PullUpConfig(Hal_Get_Point(i2c->scl,I2C_SCL_IN_E).io);
    Hal_PinMux_PullUpConfig(Hal_Get_Point(i2c->sda,I2C_SDA_IN_E).io);
    
    // init I2C
    Hal_I2c_SlaveInit(i2c->addr, i2c->address);

    
    return RET_SUCCESS;
}



