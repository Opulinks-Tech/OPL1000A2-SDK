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

#ifndef __ALI_LINKKITSDK_DECL_H__
#define __ALI_LINKKITSDK_DECL_H__

#include <stdlib.h>
#include <string.h>
#include "cmsis_os.h"
#include "sys_os_config.h"


#include "dev_sign_api.h"
#include "mqtt_api.h"
#include "dev_model_api.h"
#include "dm_wrapper.h"

#include "kvmgr.h"
#include "kv.h"
#include "awss_notify.h"
#include "cJSON.h"

#include "ali_hal_decl.h"
#include "iot_rb_data.h"




void *HAL_Malloc(uint32_t size);
void HAL_Free(void *ptr);
uint64_t HAL_UptimeMs(void);
int HAL_Snprintf(char *str, const int len, const char *fmt, ...);


typedef struct {
    int master_devid;
    int cloud_connected;
    int master_initialized;
} user_example_ctx_t;

#if USE_CUSTOME_DOMAIN
    #define CUSTOME_DOMAIN_MQTT     "iot-as-mqtt.cn-shanghai.aliyuncs.com"
    #define CUSTOME_DOMAIN_HTTP     "iot-auth.cn-shanghai.aliyuncs.com"
#endif

#define USER_EXAMPLE_YIELD_TIMEOUT_MS (1000) //Kevin add it

#define EXAMPLE_TRACE(...)                               \
do {                                                     \
    HAL_Printf("\033[1;32;40m%s.%d: ", __func__, __LINE__);  \
    HAL_Printf(__VA_ARGS__);                                 \
    HAL_Printf("\033[0m\r\n");                                   \
} while (0)



int ali_linkkit_init(user_example_ctx_t *user_example_ctx);

void IOT_Linkkit_Tx(void);


#endif
