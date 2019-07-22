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

#ifndef __EVENT_LOOP_IF_H__
#define __EVENT_LOOP_IF_H__

#include <stdint.h>
#include <stdbool.h>
#include "sys_os_config.h"
#include "wifi_event.h"
#include "event_loop.h"

#ifdef __cplusplus
extern "C" {
#endif

int  event_task_post(event_msg_t *msg, uint32_t timeout);
int  wifi_event_post_to_user(wifi_event_id_t event, void *data, uint16_t length);
void wifi_event_loop_task(void *arg);


/*private API fucntion in event_loop.c*/
typedef int  (*event_task_post_fp_t)(event_msg_t *msg, uint32_t timeout);
typedef int  (*wifi_event_post_to_user_fp_t)(wifi_event_id_t event, void *data, uint16_t length);
typedef void (*wifi_event_loop_task_fp_t)(void *arg);

/*Public API fucntion in event_loop.c*/
typedef int  (*wifi_event_loop_init_fp_t)(wifi_event_cb_t cb);
typedef void (*wifi_event_loop_set_cb_fp_t)(wifi_event_cb_t cb, void *ctx);
typedef int  (*wifi_event_loop_send_fp_t)(event_msg_t *msg);


extern event_task_post_fp_t         event_task_post_api;
extern wifi_event_post_to_user_fp_t wifi_event_post_to_user_api;
extern wifi_event_loop_task_fp_t    wifi_event_loop_task_api;

extern wifi_event_loop_init_fp_t    wifi_event_loop_init_api;
extern wifi_event_loop_set_cb_fp_t  wifi_event_loop_set_cb_api;
extern wifi_event_loop_send_fp_t    wifi_event_loop_send_api;


#ifdef __cplusplus
}
#endif

#endif /* __EVENT_LOOP_IF_H__ */
