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
#include "ble_queue.h"
#include "msg.h"


#define ENABLE_APP_DEBUG

#ifdef ENABLE_APP_DEBUG
#define BLE_APP_PRINT        printf
#else
#define BLE_APP_PRINT(...)
#endif

// Minimum connection interval (units of 1.25ms) if automatic parameter update request is enabled
#define DEFAULT_DESIRED_MIN_CONN_INTERVAL         100
// Maximum connection interval (units of 1.25ms) if automatic parameter update request is enabled
#define DEFAULT_DESIRED_MAX_CONN_INTERVAL         200
// Maximum connection interval (units of 1.25ms) if automatic parameter update request is enabled
#define DEFAULT_DESIRED_SLAVE_LATENCY            0
// Maximum connection interval (units of 1.25ms) if automatic parameter update request is enabled
#define DEFAULT_DESIRED_SUPERVERSION_TIMEOUT       500

#define BLE_SERVICE_COUNT        5


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
    BLE_APP_MSG_FIND_ALL_SERVICE,
    BLE_APP_MSG_FIND_ATTR,
    

    BLE_APP_MSG_TOP
};

enum
{
    GATT_ACTION_FIND_ATTR = 1,
    GATT_ACTION_WAIT_INFO,
    GATT_ACTION_FINISH,
    

};


typedef struct
{
    UINT16            phandle[4];
    UINT16            handle;
    UINT8            format;         
    UINT16            uuid[8];
} BLE_ATTRQ_T;

typedef struct
{
    UINT16            handle;
    UINT16            permit;   
    UINT8            format;         
    UINT16            uuid[8];
    UINT16            len;
    UINT8            *val; 
} BLE_ATT_ATTR_T;

typedef struct
{
    UINT16            start_hdl;
    UINT16            end_hdl;
    UINT8            action;
    UINT8            format;         
    UINT16            uuid[8];
    UINT16            attr_count;
    BLE_ATT_ATTR_T    *attr;
} BLE_SERVICE_T;

typedef struct
{
    TASKPACK        task;
    UINT16            state;
    UINT16            conn_hdl;
    LE_BT_ADDR_T    bt_addr;

    UINT16            curr_mtu;
    BOOL            encrypted;
    BOOL            paired;

    UINT16            min_itvl;
    UINT16            max_itvl;
    UINT16            latency;
    UINT16            sv_tmo;

    LE_QLIST_T        attrq;
    BLE_SERVICE_T    *curr_svc;
    UINT8            svc_idx;
    BLE_SERVICE_T   svc[BLE_SERVICE_COUNT];
} BLE_APP_DATA_T;


void BleAppInit(void);

BLE_APP_DATA_T* BleAppGetEntity(void);

#endif
