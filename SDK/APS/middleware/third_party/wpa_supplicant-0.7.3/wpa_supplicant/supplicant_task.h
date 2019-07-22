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
#ifndef __SUPPLICANT_TASK_H__
#define __SUPPLICANT_TASK_H__

#include "cmsis_os.h"
#include "task.h"
#include "driver.h"

#define SUPP_QUEUE_SIZE					(50)

typedef struct
{
    uint32_t event;
	uint32_t length;
	uint8_t *pcMessage;
} xSupplicantMessage_t;

extern osMessageQId xSupplicantQueue;
extern osThreadId SupplicantTaskHandle;

#if 0
void supplicant_task_create(void);
void supplicant_task_init(void);
void supplicant_task( void *pvParameters );
osStatus supplicant_task_send(xSupplicantMessage_t txMsg);
void supplicant_task_evt_handle(uint32_t evt_type);
#else
typedef void (*supplicant_task_create_fp_t)(void);
typedef void (*supplicant_task_init_fp_t)(void);
typedef void (*supplicant_task_fp_t)( void *pvParameters );
typedef osStatus (*supplicant_task_send_fp_t)(xSupplicantMessage_t txMsg);
typedef void (*supplicant_task_evt_handle_fp_t)(uint32_t evt_type);
extern supplicant_task_create_fp_t supplicant_task_create;
extern supplicant_task_init_fp_t supplicant_task_init;
extern supplicant_task_fp_t supplicant_task;
extern supplicant_task_send_fp_t supplicant_task_send;
extern supplicant_task_evt_handle_fp_t supplicant_task_evt_handle;
#endif

/*
   Interface Initialization: Supplicant Task
 */
void wpa_supplicant_task_func_init(void);

#endif //__SUPPLICANT_TASK_H__
