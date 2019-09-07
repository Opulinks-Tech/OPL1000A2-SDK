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

#ifndef __ALI_HAL_DECL_H__
#define __ALI_HAL_DECL_H__

#include <stdlib.h>
#include <string.h>
#include "cmsis_os.h"
#include "sys_os_config.h"

#include "infra_defs.h"

#ifndef _IN_ 
  #define _IN_ 
#endif
#ifndef _OU_
  #define _OU_ 
#endif

void Ali_Hal_Devive_init(void);

int HAL_SetDeviceName(char *device_name);
int HAL_SetDeviceSecret(char *device_secret);
int HAL_SetProductKey(char *product_key);
int HAL_SetProductSecret(char *product_secret);

int HAL_GetProductKey(char product_key[IOTX_PRODUCT_KEY_LEN + 1]);
int HAL_GetProductSecret(char product_secret[IOTX_PRODUCT_SECRET_LEN+1]);
int HAL_GetDeviceName(char device_name[IOTX_DEVICE_NAME_LEN + 1]);
int HAL_GetDeviceSecret(char device_secret[IOTX_DEVICE_SECRET_LEN+1]);

uint32_t HAL_GetProductId(void);



#endif
