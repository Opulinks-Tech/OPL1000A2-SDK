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

#ifndef _BLE_ADV_APP_H_
#define _BLE_ADV_APP_H_

#include "ble.h"
#include "ble_msg.h"


enum
{
    APP_STATE_INIT,
    APP_STATE_IDLE,
    APP_STATE_ADVERTISING,

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
    TASKPACK        task;
    UINT16            state;

} BLE_APP_DATA_T;


void BleAppInit(void);

#endif
