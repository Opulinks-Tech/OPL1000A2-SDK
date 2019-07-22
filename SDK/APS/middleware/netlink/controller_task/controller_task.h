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

#ifndef __CONTROLLER_TASK_H__
#define __CONTROLLER_TASK_H__

#include "sys_os_config.h"

#define	CONTROLLER_QUEUE_SIZE				40
#define	CONTROLLER_QUEUE_BLE_SIZE			20
#define CONTROLLER_TASK_PRIORITY			OS_TASK_PRIORITY_CONTROLLER
#define CONTROLLER_TASK_STACK_SIZE			OS_TASK_STACK_SIZE_CONTROLLER

#define CONTROLLER_MESSAGE_TYPE_WIFI_CMD	0
#define CONTROLLER_MESSAGE_TYPE_WIFI_EVT	1
#define CONTROLLER_MESSAGE_TYPE_BLE_TX		2
#define CONTROLLER_MESSAGE_TYPE_BLE_RX		3

#include "cmsis_os.h"

typedef enum {
	CTRL_TaskStatusTransmitReady,
	CTRL_TaskStatusBufferEmpty
} CTRL_TaskStatus;

extern osMessageQId controller_queue_ble_id;

void controller_task_func_init(void);

void controller_task_create_impl(void);

void controller_task(void *argument);

void CTRL_TaskSendHciPacketFromShm_impl(int indicator, void *data, int *ipc_ret_opcode);

CTRL_TaskStatus CTRL_TaskSendHciPacket_impl(void *data, int length);

/*	This is a non-blocking function,
	delay or yield, if return without CTRL_TaskStatusTransmitReady

	Sample below:
		while (1) {
			status = CTRL_TaskReceiveHciPacket(data, &len);
			if (status == CTRL_TaskStatusTransmitReady)
				// processing hci event or acl data
			else
				// delay or yield
		}	*/
CTRL_TaskStatus CTRL_TaskReceiveHciPacket_impl(void *data, int *length);

CTRL_TaskStatus CTRL_TaskSendWifiCmd_impl(void *data);

CTRL_TaskStatus CTRL_TaskSendWifiEvt_impl(void *data);

typedef void (*controller_task_create_fp)(void);
typedef void (*CTRL_TaskSendHciPacketFromShm_fp)(int indicator, void *data, int *ipc_ret_opcode);
typedef CTRL_TaskStatus (*CTRL_TaskSendHciPacket_fp)(void *data, int length);
typedef CTRL_TaskStatus (*CTRL_TaskReceiveHciPacket_fp)(void *data, int *length);
typedef CTRL_TaskStatus (*CTRL_TaskSendWifiCmd_fp)(void *data);
typedef CTRL_TaskStatus (*CTRL_TaskSendWifiEvt_fp)(void *data);

extern controller_task_create_fp controller_task_create;
extern CTRL_TaskSendHciPacketFromShm_fp CTRL_TaskSendHciPacketFromShm;
extern CTRL_TaskSendHciPacket_fp CTRL_TaskSendHciPacket;
extern CTRL_TaskReceiveHciPacket_fp CTRL_TaskReceiveHciPacket;
extern CTRL_TaskSendWifiCmd_fp CTRL_TaskSendWifiCmd;
extern CTRL_TaskSendWifiEvt_fp CTRL_TaskSendWifiEvt;

#endif
