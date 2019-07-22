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

#ifndef __WIFI_EVENT_HANDLER_IF_API_H__
#define __WIFI_EVENT_HANDLER_IF_API_H__

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
int wifi_event_sta_connected_handler(wifi_event_t event, uint8_t *data, uint32_t length);
int wifi_event_sta_disconnected_handler(wifi_event_t event, uint8_t *data, uint32_t length);
int wifi_set_event_handler(wifi_event_t event_id, wifi_event_handler_t handler_func);

/* Rom patch interface */

/* Private API fucntion used in wifi_event_handler.c*/
typedef int (*wifi_event_sta_connected_handler_fp_t)(wifi_event_t event, uint8_t *data, uint32_t length);
typedef int (*wifi_event_sta_disconnected_handler_fp_t)(wifi_event_t event, uint8_t *data, uint32_t length);
typedef int (*wifi_set_event_handler_fp_t)(wifi_event_t event_id, wifi_event_handler_t handler_func);


/*Public API fucntion in wifi_event_handler.c*/
typedef void (*wifi_install_default_event_handlers_fp_t)(void);
typedef int  (*wifi_register_event_handler_fp_t)(wifi_event_t idx, wifi_event_handler_t handler);
typedef int  (*wifi_event_process_handler_fp_t)(wifi_event_t event, uint8_t *payload, uint32_t length);



extern wifi_event_sta_connected_handler_fp_t    wifi_event_sta_connected_handler_api;
extern wifi_event_sta_disconnected_handler_fp_t wifi_event_sta_disconnected_handler_api;
extern wifi_set_event_handler_fp_t              wifi_set_event_handler_api;
extern wifi_install_default_event_handlers_fp_t wifi_install_default_event_handlers_api;
extern wifi_register_event_handler_fp_t         wifi_register_event_handler_api;
extern wifi_event_process_handler_fp_t          wifi_event_process_handler_api;



/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __WIFI_EVENT_HANDLER_API_H__ */
