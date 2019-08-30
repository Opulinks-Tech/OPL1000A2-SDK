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

/**
 * @file blewifi_ctrl_http_ota.h
 * @author Luke Fang
 * @date 11 Sep 2018
 * @brief File includes the function declaration of blewifi ctrl http ota task.
 *
 */

#ifndef _BLEWIFI_CTRL_HTTP_OTA_H_
#define _BLEWIFI_CTRL_HTTP_OTA_H_

#if (WIFI_OTA_FUNCTION_EN == 1)
#define BLEWIFI_CTRL_HTTP_OTA_QUEUE_SIZE         (20)

typedef enum blewifi_wifi_ota_type_id
{
    BLEWIFI_WIFI_OTA_SUCCESS,
    BLEWIFI_WIFI_OTA_FAILURE,
} blewifi_wifi_ota_type_id_e;

typedef enum blewifi_ctrl_http_ota_msg_type
{
    /* BLE Trigger */
    BLEWIFI_CTRL_HTTP_OTA_MSG_TRIG = 0,    //TEST
    BLEWIFI_CTRL_HTTP_OTA_MSG_DEVICE_VERSION,
    BLEWIFI_CTRL_HTTP_OTA_MSG_SERVER_VERSION,
    
    BLEWIFI_CTRL_HTTP_OTA_MSG_NUM
} blewifi_ctrl_http_ota_msg_type_e;

typedef struct
{
    uint32_t event;
    uint32_t length;
    uint8_t ucaMessage[];
} xBleWifiCtrlHttpOtaMessage_t;


/**
  * @brief This function send data to blewifi task
  *
  * @param[in] message type
  * @param[in] payload data
  * @param[in] payload data length
  * @return
  *    -  0 : succeed
  *    - -1: faild
  */
int blewifi_ctrl_http_ota_msg_send(int msg_type, uint8_t *data, int data_len);

/**
  * @brief This function create control task
  *
  */
void blewifi_ctrl_http_ota_task_create(void);

#endif /* #if (WIFI_OTA_FUNCTION_EN == 1) */
#endif /* _BLEWIFI_CTRL_HTTP_OTA_H_ */

