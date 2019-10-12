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

#ifndef __IOT_DATA_H__
#define __IOT_DATA_H__

#include <stdint.h>
#include <stdbool.h>




typedef enum iot_data_tx_msg_type
{
    IOT_DATA_TX_MSG_DATA_POST = 0,

    IOT_DATA_TX_MSG_NUM
} iot_data_tx_msg_type_e;

typedef struct
{
    uint32_t event;
    uint32_t length;
    uint8_t ucaMessage[];
} xIotDataMessage_t;

typedef void (*T_IoT_Data_EvtHandler_Fp)(uint32_t evt_type, void *data, int len);
typedef struct
{
    uint32_t ulEventId;
    T_IoT_Data_EvtHandler_Fp fpFunc;
} T_IoT_Data_EvtHandlerTbl;

#if (IOT_DEVICE_DATA_TX_EN == 1)
int Iot_Data_TxTask_MsgSend(int msg_type, uint8_t *data, int data_len);
#endif  // end of #if (IOT_DEVICE_DATA_TX_EN == 1)

#if (IOT_DEVICE_DATA_TX_EN == 1) || (IOT_DEVICE_DATA_RX_EN == 1)
void Iot_Data_Init(void);
#endif

#endif  // end of __IOT_DATA_H__
