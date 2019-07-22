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

#ifndef __EVENT_LOOP_H__
#define __EVENT_LOOP_H__

#include <stdint.h>
#include <stdbool.h>
#include "sys_os_config.h"
#include "wifi_event.h"

#ifdef __cplusplus
extern "C" {
#endif

#define WIFI_EVENT_LOOP_QUEUE_SIZE  5
#define WIFI_EVENT_LOOP_TASK_PRIORITY			OS_TASK_PRIORITY_EVENT_LOOP
#define WIFI_EVENT_LOOP_TASK_STACK_SIZE			OS_TASK_STACK_SIZE_EVENT_LOOP

/** \defgroup WIFI_APIs WIFI APIs
  * @brief WIFI APIs 
  */

/** @addtogroup WIFI_APIs
  * @{
  */

/** \defgroup WIFI_COMMON_APIs WIFI Common APIs
 */

/** @addtogroup WIFI_COMMON_APIs
 * @{
 */


/**
 * @brief Send information to event by event_msg_t
 */
typedef struct
{
  /** event type */
    uint32_t event;
  /** Packet length */
	uint32_t length;
  /** event parament */
	uint8_t *param;
} event_msg_t;


/**
  * @brief  Application specified event callback function
  *
  */
typedef int (*wifi_event_cb_t)(wifi_event_id_t event, void *data, uint16_t length);


/**
  * @brief  Event Loop Initialization
  *         Create the event handler and call back funtion
  *
  * @param  cb : application specified event callback 
  *
  * @return    0  : success
  * @return    other : failed
 */
int  wifi_event_loop_init(wifi_event_cb_t cb);

/**
  * @brief  Set application specified event callback function
  *
  * @attention 1. If cb is NULL, means application does not need to handle
  *               If cb is not NULL, it will be called when an event is received and 
  *               after the default event callback is completed
  *
  * @param  wifi_event_cb_t cb : callback
  * @param  void *ctx : reserved for user
  *
  */
void wifi_event_loop_set_cb(wifi_event_cb_t cb, void *ctx);

/**
  * @brief  Send an event to event task
  * @attention 1. Other task/modules, such as the TCPIP module, can call this API to send an event to event task
  * @param  event_msg_t * msg: Send information to event by msg
  *
  * @return    0  : success
  * @return    other : failed
*/
int  wifi_event_loop_send(event_msg_t *msg);

/**
  * @}
  */


/**
  * @}
  */


#ifdef __cplusplus
}
#endif

#endif /* __EVENT_LOOP_H__ */
