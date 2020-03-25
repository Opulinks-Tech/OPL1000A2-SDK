/******************************************************************************
*  Copyright 2017 - 2019, Opulinks Technology Ltd.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2019
******************************************************************************/

#ifndef _SYS_COMMON_API_PATCH_H_
#define _SYS_COMMON_API_PATCH_H_

#include "sys_common_api.h"

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

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *               Global Variables
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/

/**
* @brief     Force the VDD_RF (smps_rf) of WiFi lowpwer mode with given setting. Recommand: do it in Sys_ServiceInit
  *
  * @attention 1. API returns false if try to set Configuration which something error
  *
  * @param[in]   level: The level of VDD_RF (smps_rf)
  *              - 0: 1.2V
  *              - 1: 1.3V
  *              - 2: 1.4V
  *              - 3: 1.5V
  *
  * @return    0  : success
  * @return    other : failed
 */
int sys_set_wifi_lowpower_tx_vdd_rf(uint8_t level);

/**
* @brief     Force periodic temperature calibration(TCA) mode for RF.
  *
  * @attention 1. API returns false if try to set Configuration which something error
  *
  * @param[in]   mode: The mode of TCA
  *              - 0: off
  *              - 1: on
  *
  * @return    0  : success
  * @return    other : failed
 */
int sys_set_rf_temp_cal_mode(uint8_t mode);

#ifdef __cplusplus
}
#endif

#endif /* _SYS_COMMON_API_PATCH_H_ */
