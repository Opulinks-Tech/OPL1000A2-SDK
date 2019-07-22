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

#ifndef _ANCS_APP_H_
#define _ANCS_APP_H_

#include "ble.h"
#include "ble_msg.h"
#include "ble_queue.h"
#include "msg.h"


#define ENABLE_APP_DEBUG

#ifdef ENABLE_APP_DEBUG
#define BLE_APP_PRINT		printf
#else
#define BLE_APP_PRINT(...)
#endif

#define DEFAULT_DESIRED_MIN_CONN_INTERVAL     	100
#define DEFAULT_DESIRED_MAX_CONN_INTERVAL     	200
#define DEFAULT_DESIRED_SLAVE_LATENCY			0
#define DEFAULT_DESIRED_SUPERVERSION_TIMEOUT   	500

#define BLE_SERVICE_COUNT		5

enum
{
	BLE_APP_MSG_INITIALIZING = 1,
	BLE_APP_MSG_ENTER_ADVERTISING,
	BLE_APP_MSG_EXIT_ADVERTISING,
	BLE_APP_MSG_SECURITY,

	BLE_APP_MSG_FIND_PRIMARY_SERVICE,
	
	BLE_APP_MSG_FIND_ANCS_SERVICE,
	BLE_APP_MSG_FIND_ATTR,
	BLE_APP_MSG_FIND_ANCS_CHAR,

	BLE_APP_MSG_RW_REQ,

	BLE_APP_MSG_TOP
};

enum
{
	APP_STATE_INIT,
	APP_STATE_IDLE,
	APP_STATE_ADVERTISING
};

enum
{
	GATT_ACTION_IDLE,
	GATT_ACTION_FIND_PRIMARY_SERVICE,
	GATT_ACTION_FIND_ANCS_SERVICE,
	GATT_ACTION_FIND_ATTR,
	GATT_ACTION_WAIT_ATTR_INFO,
	GATT_ACTION_WAIT_CHAR_INFO,
	GATT_ACTION_RW_CCCD_VAL,
	GATT_ACTION_WR_ANCS_CMD
};

typedef struct
{
	UINT16		conn_hdl;
} BLE_APP_MSG_COMMON_T;

typedef struct
{
	UINT16			phandle[4];
	UINT8			op;
} GATT_REQ_HDR_T;

typedef struct
{
	UINT16			phandle[4];
	UINT16			handle;
	UINT8			format;         
	UINT16			uuid[8];
} BLE_ATTRQ_T;

typedef struct
{
	UINT16			handle;
	UINT8			format;         
	UINT16			uuid[8];
} BLE_ATT_ATTR_T;

typedef struct
{
	UINT16			start_hdl;
	UINT16			end_hdl;
    UINT8			action;
	UINT8			format;         
	UINT16			uuid[8];
    UINT16			attr_count;
	BLE_ATT_ATTR_T	*attr;
} BLE_SERVICE_T;

typedef struct
{
	UINT16			handle;
	UINT16			val_hdl;
	UINT8			property;
	UINT8			format;
    UINT16			dummy;
	UINT16			uuid[8];
} BLE_CHAR_DECL_T;

typedef struct
{
	UINT16			handle;                    
	UINT16			config;                   
} BLE_CHAR_CCCD_T;

typedef struct
{
	UINT16			phandle[4];
	UINT16			conn_hdl;
	UINT16			devid;
    UINT16			interval;
    UINT16			latency;
    UINT16			sv_tmo;
	UINT16			mtu;
	LE_BT_ADDR_T	bt_addr;
	BOOL			encrypted;
    BOOL			paired;

    UINT8			action;
    BLE_SERVICE_T   svc[BLE_SERVICE_COUNT];
	UINT8			svc_idx;
	BLE_SERVICE_T	*curr_svc;
	LE_QLIST_T		attrq;
    LE_QLIST_T		rw_req;
    GATT_REQ_HDR_T	*rw_curr;
	void			*ancs;
} BLE_APP_PEER_T;

typedef struct
{
	TASKPACK		task;
	UINT16			state;
    UINT16			default_mtu;
    UINT16			min_itvl;
    UINT16			max_itvl;
    UINT16			latency;
    UINT16			sv_tmo;
	LE_QLIST_T		peer;
} BLE_APP_DATA_T;


void BleAppInit(void);

BLE_APP_DATA_T* BleAppGetEntity(void);

void* BleAppFindPeer(UINT16 conn_hdl);

void BleAppSendCommonMsg(UINT32 msgId, UINT16 conn_hdl, UINT32 delay);

void BleAppMemDump(char *s, UINT16 len, void *data);

#endif
