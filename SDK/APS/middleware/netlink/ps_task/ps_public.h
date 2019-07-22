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

#include "hal_vic.h"

#ifndef __PS_PUBLIC_H__
#define __PS_PUBLIC_H__

typedef enum
{
	PS_WAKEUP_TYPE_IO,		/**< Wakeup by external IO of pre-defined */
	PS_WAKEUP_TYPE_TIMEOUT  /**< Wakeup by timer timeout */

} PS_WAKEUP_TYPE;

typedef void (*PS_WAKEUP_CALLBACK)(PS_WAKEUP_TYPE);

/*
* @brief Configure external input for system wakeup from sleep
*
* @param [in] num The number of external input used for wakeup
*
* @param [in] enable Set whether enable the external wake-up mechanism
*
* @param [in] type The trigger type for external input
*
* @param [in] invert Set whether invert the triggered signal
*
* @param [in] callback The ISR which be invoked at IO wake-up happened
*
*/
void ps_set_wakeup_io(E_GpioIdx_t num, int enable, E_ItrType_t type, int invert, T_Gpio_CallBack callback);


/*
* @brief Set callback witch invoked at system wakeup
*
* @param [in] callback User defined wakeup callback function
*
*/
void ps_set_wakeup_cb(PS_WAKEUP_CALLBACK callback);

/*
* @brief Enable Deep Sleep: system will go sleep until external input triggered
*
*/
void ps_deep_sleep(void);

/*
* @brief Enable Timer Sleep: system will go sleep until timer timeout or external input triggered
*
* @param [in] sleep_duration_ms Timer timeout value unit in millisecond
*
*/
void ps_timer_sleep(uint32_t sleep_duration_ms);

/*
* @brief Enable Smart Sleep: system will go sleep automatically if system in idle during connection time. Smart sleep disabled when external input triggered.<br/>
* Define that what causes "if system in idle during connection time"<br/>
*   - WiFi connection is exists<br/>
*   - BLE advertising is enabled<br/>
*	- BLE connection is exists<br/>
*
* @param [in] enable Configure whether smart sleep is enabled
*
*/
void ps_smart_sleep(int enable);

#endif
