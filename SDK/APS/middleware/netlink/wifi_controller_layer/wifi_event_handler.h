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

#ifndef __WIFI_EVENT_HANDLER_API_H__
#define __WIFI_EVENT_HANDLER_API_H__

#include <stdint.h>
#include <stdbool.h>

#include "wifi_api.h"

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
 *               Function Declarations
 ******************************************************/

/** \defgroup WIFI_APIs WIFI APIs
  * @brief WIFI APIs 
  */

/** @addtogroup WIFI_APIs
  * @{
  */


/**
 * @brief  instaall default event handler for wifi event (internal use)
 */
void wifi_install_default_event_handlers(void);

/**
  * @brief register wifi event handelrt (internal use)
  *
  * @param[in] idx: one of the enums of bt_scan_mode_t
  * @param[in] handler: the Wi-Fi event handler
  *
  * @return    0  : success
  * @return    other : failed
  */
int wifi_register_event_handler(wifi_event_t idx, wifi_event_handler_t handler);

/**
  * @brief  Default event handler for system events
  *
  * This function performs default handling of system events.
  * When using event_loop APIs, it is called automatically before invoking the user-provided
  * callback function.
  *
  * Applications which implement a custom event loop must call this function
  * as part of event processing.
  *
  * @param[in]  event: event type  Set the event type,Options are 
  *             - WIFI_EVENT_INIT_COMPLETE
  *             - WIFI_EVENT_SCAN_COMPLETE
  *             - WIFI_EVENT_STA_START
  *             - WIFI_EVENT_STA_STOP
  *             - WIFI_EVENT_STA_CONNECTED
  *             - WIFI_EVENT_STA_DISCONNECTED
  *             - WIFI_EVENT_STA_CONNECTION_FAILED
  *             - WIFI_EVENT_STA_GOT_IP
  *
  * @param[in]  payload: Data block that transmitted to event
  * @param[in]  length: The length of data block
  *
  * @return    0  : success
  * @return    other : failed
  */
int wifi_event_process_handler(wifi_event_t event, uint8_t *payload, uint32_t length);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __WIFI_EVENT_HANDLER_API_H__ */
