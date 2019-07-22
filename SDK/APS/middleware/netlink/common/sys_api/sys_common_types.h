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

#ifndef _SYS_COMMON_TYPES_H_
#define _SYS_COMMON_TYPES_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
 *                    Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************
 *                   Enumerations
 ******************************************************/
typedef enum {
    MAC_IFACE_WIFI_STA = 0,    /**< Wi-Fi Station interface */
    MAC_IFACE_WIFI_SOFTAP,     /**< Wi-Fi SoftAP interface (Not support yet) */
    MAC_IFACE_BLE,             /**< BLE interface */
} mac_iface_t;

typedef enum {
    MAC_SOURCE_FROM_OTP = 0,   /**< Mac address source from OTP */
    MAC_SOURCE_FROM_FLASH,     /**< Mac address source from FLASH */
} mac_source_type_t;

/******************************************************
 *                 Type Definitions
 ******************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _SYS_COMMON_TYPES_H_ */
