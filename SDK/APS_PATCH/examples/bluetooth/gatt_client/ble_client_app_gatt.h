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

#ifndef _BLE_SERVER_APP_GATT_H_
#define _BLE_SERVER_APP_GATT_H_

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
    BPS_IDX_SVC,

    BPS_IDX_BP_MEAS_CHAR,
    BPS_IDX_BP_MEAS_VAL,
    BPS_IDX_BP_MEAS_IND_CFG,

    BPS_IDX_INTM_CUFF_PRESS_CHAR,
    BPS_IDX_INTM_CUFF_PRESS_VAL,
    BPS_IDX_INTM_CUFF_PRESS_NTF_CFG,

    BPS_IDX_BP_FEATURE_CHAR,
    BPS_IDX_BP_FEATURE_VAL,

    BPS_IDX_TOP
};

void BleAppGattMsgHandler(TASK task, MESSAGEID id, MESSAGE message);

#endif
