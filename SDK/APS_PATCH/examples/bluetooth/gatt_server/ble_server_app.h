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

#ifndef _BLE_SERVER_APP_H_
#define _BLE_SERVER_APP_H_

#include "ble.h"
#include "ble_msg.h"
#include "msg.h"


#define ENABLE_APP_DEBUG

#ifdef ENABLE_APP_DEBUG
#define BLE_APP_PRINT		printf
#else
#define BLE_APP_PRINT(...)
#endif

// Minimum connection interval (units of 1.25ms) if automatic parameter update request is enabled
#define DEFAULT_DESIRED_MIN_CONN_INTERVAL     	100
// Maximum connection interval (units of 1.25ms) if automatic parameter update request is enabled
#define DEFAULT_DESIRED_MAX_CONN_INTERVAL     	200
// Maximum connection interval (units of 1.25ms) if automatic parameter update request is enabled
#define DEFAULT_DESIRED_SLAVE_LATENCY			0
// Maximum connection interval (units of 1.25ms) if automatic parameter update request is enabled
#define DEFAULT_DESIRED_SUPERVERSION_TIMEOUT   	500


enum
{
	APP_STATE_INIT,
	APP_STATE_IDLE,
	APP_STATE_ADVERTISING,
	APP_STATE_CONNECTED,

};

enum
{
	BLE_APP_MSG_INITIALIZING = 1,
	BLE_APP_MSG_ENTER_ADVERTISING,
	BLE_APP_MSG_EXIT_ADVERTISING,


	BLE_APP_MSG_TOP
};


typedef struct
{
	TASKPACK		task;
	UINT16			state;
	UINT16			conn_hdl;
	LE_BT_ADDR_T	bt_addr;

	UINT16			curr_mtu;
	BOOL			encrypted;
    BOOL			paired;

    UINT16			min_itvl;
    UINT16			max_itvl;
    UINT16			latency;
    UINT16			sv_tmo;
} BLE_APP_DATA_T;


void BleAppInit(void);

BLE_APP_DATA_T* BleAppGetEntity(void);

#endif
