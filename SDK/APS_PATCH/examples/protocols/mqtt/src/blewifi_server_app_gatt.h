/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

#ifndef _BLEWIFI_SERVER_APP_GATT_H_
#define _BLEWIFI_SERVER_APP_GATT_H_

#include "ble_msg.h"


enum
{
    GATT_IDX_SVC,
        
	GATT_IDX_SERVICE_CHANGE_CHAR,
	GATT_IDX_SERVICE_CHANGE_VAL,
	GATT_IDX_SERVICE_CHANGE_CFG,

    GATT_IDX_TOP
};

enum
{
    GAP_IDX_SVC,

	GAP_IDX_DEVICE_NAME_CHAR,
	GAP_IDX_DEVICE_NAME_VAL,

	GAP_IDX_APPEARANCE_CHAR,
	GAP_IDX_APPEARANCE_VAL,

	GAP_IDX_CONN_PARAM_CHAR,
	GAP_IDX_CONN_PARAM_VAL,

    GAP_IDX_TOP
};

enum
{
    BWP_IDX_SVC,

	BWP_IDX_DATA_IN_CHAR,
	BWP_IDX_DATA_IN_VAL,

	BWP_IDX_DATA_OUT_CHAR,
	BWP_IDX_DATA_OUT_VAL,
	BWP_IDX_DATA_OUT_CFG,
	
    BWP_IDX_TOP
};

// BLE GAP Device Name (default)
#define BLEWIFI_BLE_GAP_DEVICE_NAME     "BLEWIFI APP"

typedef void (*T_BleWifi_Ble_MsgHandler_Fp)(TASK task, MESSAGEID id, MESSAGE message);
typedef struct
{
    uint32_t ulEventId;
    T_BleWifi_Ble_MsgHandler_Fp fpFunc;
} T_BleWifi_Ble_MsgHandlerTbl;

void BleWifi_Ble_ServerAppGattMsgHandler(TASK task, MESSAGEID id, MESSAGE message);

void BleWifi_Ble_GattIndicateServiceChange(UINT16 conn_hdl);

void BleWifi_Ble_AppUpdateDevName(UINT8 *name, UINT16 len);

#endif
