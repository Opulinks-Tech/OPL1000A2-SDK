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

#ifndef __WIFI_EVENT_H__
#define __WIFI_EVENT_H__

#include <stdint.h>
#include <stdbool.h>

#include "wifi_types.h"

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
typedef wifi_event_t wifi_event_id_t;

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *               Global Variables
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

/** \defgroup WIFI_COMMON_APIs WIFI Common APIs
 */

/** @addtogroup WIFI_COMMON_APIs
 * @{
 */

/**
 * @brief  wifi_event_sta_scan_done_t
 */
typedef struct {
    uint32_t status;                    /**< status of scanning APs */
    uint8_t  number;
    uint8_t  scan_id;
} wifi_event_sta_scan_done_t;

/**
 * @brief wifi_event_sta_connected_t
 */
typedef struct {
    uint8_t ssid[32];                   /**< SSID of connected AP */
    uint8_t ssid_len;                   /**< SSID length of connected AP */
    uint8_t bssid[6];                   /**< BSSID of connected AP*/
    uint8_t channel;                    /**< channel of connected AP*/
    wifi_auth_mode_t authmode;
} wifi_event_sta_connected_t;

/**
 * @brief  wifi_event_sta_disconnected_t
 */
typedef struct {
    uint8_t ssid[32];                   /**< SSID of disconnected AP */
    uint8_t ssid_len;                   /**< SSID length of disconnected AP */
    uint8_t bssid[6];                   /**< BSSID of disconnected AP */
    uint8_t reason;                     /**< reason of disconnection */
} wifi_event_sta_disconnected_t;

typedef struct {
    //tcpip_adapter_ip_info_t ip_info;
    bool ip_changed;
} wifi_event_sta_got_ip_t;

/**
 * @brief  wifi_event_info_t
 */
typedef union
{
    wifi_event_sta_connected_t     connected;          /**< station connected to AP */
    wifi_event_sta_disconnected_t  disconnected;       /**< station disconnected to AP */
    wifi_event_sta_scan_done_t     scan_done;          /**< station scan (APs) done */
    wifi_event_sta_got_ip_t        got_ip;             /**< station got IP, first time got IP or when IP is changed */
} wifi_event_info_t;

/*
typedef struct
{
    wifi_event_id_t     event_id;
    wifi_event_info_t   event_info;
} wifi_event_cb_data_t;
*/

/**
  * @brief  Default event handler for system events
  *
  * This function performs default handling of system events.
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
  * @param[in]  payload: Data block transmitted to event
  * @param[in]  length: The length of the data block
  *
  * @return    0  : success
  * @return    other : failed
  */
int wifi_event_process_handler(wifi_event_t event, uint8_t *payload, uint32_t length);


/**
  * @}
  */


/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __WIFI_EVENT_H__ */
