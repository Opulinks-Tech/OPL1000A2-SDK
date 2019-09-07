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
#include "ali_hal_decl.h"
#include "mw_fim_default_group12_project.h"

#define __DEMO__

T_MwFim_GP12_AliyunDevice g_AliyunDevice;

void Ali_Hal_Devive_init(){
	
    // get the settings of power saving
    if (MW_FIM_OK != MwFim_FileRead(MW_FIM_IDX_GP12_PROJECT_ALIYUN_DEVICE, 0, MW_FIM_GP12_ALIYUN_DEVICE_SIZE, (uint8_t*)&g_AliyunDevice))
    {
        // if fail, get the default value
        memcpy(&g_AliyunDevice, &g_tMwFimDefaultGp12AliyunDevice, MW_FIM_GP12_ALIYUN_DEVICE_SIZE);
    }
}


int HAL_SetProductKey(_IN_ char *product_key)
{
    int len = strlen(product_key);
#ifdef __DEMO__
    if (len > IOTX_PRODUCT_KEY_LEN) {
        return -1;
    }
    memset(g_AliyunDevice.ubaProductKey, 0x0, IOTX_PRODUCT_KEY_LEN + 1);
    strncpy(g_AliyunDevice.ubaProductKey, product_key, len);
		
    //MwFim_FileWrite(MW_FIM_IDX_GP12_PROJECT_ALIYUN_DEVICE, 0, MW_FIM_GP12_ALIYUN_DEVICE_SIZE, (uint8_t*)&g_AliyunDevice);
#endif
    return len;
}


int HAL_SetDeviceName(_IN_ char *device_name)
{
    int len = strlen(device_name);
#ifdef __DEMO__
    if (len > IOTX_DEVICE_NAME_LEN) {
        return -1;
    }
    memset(g_AliyunDevice.ubaDeviceName, 0x0, IOTX_DEVICE_NAME_LEN + 1);
    strncpy(g_AliyunDevice.ubaDeviceName, device_name, len);
    //MwFim_FileWrite(MW_FIM_IDX_GP12_PROJECT_ALIYUN_DEVICE, 0, MW_FIM_GP12_ALIYUN_DEVICE_SIZE, (uint8_t*)&g_AliyunDevice);
#endif
    return len;
}


int HAL_SetDeviceSecret(_IN_ char *device_secret)
{
    int len = strlen(device_secret);
#ifdef __DEMO__
    if (len > IOTX_DEVICE_SECRET_LEN) {
        return -1;
    }
    memset(g_AliyunDevice.ubaDeviceSecret, 0x0, IOTX_DEVICE_SECRET_LEN + 1);
    strncpy(g_AliyunDevice.ubaDeviceSecret, device_secret, len);
    //MwFim_FileWrite(MW_FIM_IDX_GP12_PROJECT_ALIYUN_DEVICE, 0, MW_FIM_GP12_ALIYUN_DEVICE_SIZE, (uint8_t*)&g_AliyunDevice);
#endif
    return len;
}


int HAL_SetProductSecret(_IN_ char *product_secret)
{
    int len = strlen(product_secret);
#ifdef __DEMO__
    if (len > IOTX_PRODUCT_SECRET_LEN) {
        return -1;
    }
    memset(g_AliyunDevice.ubaProductSecret, 0x0, IOTX_PRODUCT_SECRET_LEN + 1);
    strncpy(g_AliyunDevice.ubaProductSecret, product_secret, len);
    //MwFim_FileWrite(MW_FIM_IDX_GP12_PROJECT_ALIYUN_DEVICE, 0, MW_FIM_GP12_ALIYUN_DEVICE_SIZE, (uint8_t*)&g_AliyunDevice);
#endif
    return len;
}

int HAL_GetProductKey(_OU_ char *product_key)
{
    int len = strlen(g_AliyunDevice.ubaProductKey);
    memset(product_key, 0x0, IOTX_PRODUCT_KEY_LEN);

#ifdef __DEMO__
    strncpy(product_key, g_AliyunDevice.ubaProductKey, len);
#endif

    return len;
}

int HAL_GetProductSecret(_OU_ char *product_secret)
{
    int len = strlen(g_AliyunDevice.ubaProductSecret);
    memset(product_secret, 0x0, IOTX_PRODUCT_SECRET_LEN);

#ifdef __DEMO__
    strncpy(product_secret, g_AliyunDevice.ubaProductSecret, len);
#endif

    return len;
}

int HAL_GetDeviceName(_OU_ char *device_name)
{
    int len = strlen(g_AliyunDevice.ubaDeviceName);
    memset(device_name, 0x0, IOTX_DEVICE_NAME_LEN);

#ifdef __DEMO__
    strncpy(device_name, g_AliyunDevice.ubaDeviceName, len);
#endif

    return strlen(device_name);
}

int HAL_GetDeviceSecret(_OU_ char *device_secret)
{
    int len = strlen(g_AliyunDevice.ubaDeviceSecret);
    memset(device_secret, 0x0, IOTX_DEVICE_SECRET_LEN);

#ifdef __DEMO__
    strncpy(device_secret, g_AliyunDevice.ubaDeviceSecret, len);
#endif

    return len;
}

uint32_t HAL_GetProductId()
{
	return g_AliyunDevice.ulProductId;
}
