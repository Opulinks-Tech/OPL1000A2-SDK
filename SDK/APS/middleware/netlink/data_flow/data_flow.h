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

#ifndef __DATA_FLOW_H__
#define __DATA_FLOW_H__

#include "ipc.h"

#define TX_TEST_CASE 0

#define DATA_FLOW_TYPE_START_BIT    24
#define DATA_FLOW_IND_BIT           27
#define DATA_FLOW_TYPE_MASK         0x07000000
#define DATA_FLOW_ADDR_MASK         0xf0ffffff

typedef enum{
SYSTEM_CMD_EVT = 0,
BLE_HCI_CMD_EVT = 1,
WIFI_AT_CMD_EVT = 2,
RF_CMD_EVT = 3,
DIAG_CMD_EVT = 4
} t_Source_Index;

typedef enum{
APS_2_MSQ = 0,
MSQ_2_APS = 1,
DIRECTION = 2
} t_Data_Direction;

typedef enum{
COMMAND_QUEUE     = 0,
EVENT_QUEUE       = 1,
DATA_OUT_1_QUEUE  = 2,
DATA_IN_1_QUEUE   = 3,
DATA_OUT_2_QUEUE  = 4,
DATA_IN_2_QUEUE   = 5,
DATA_OUT_3_QUEUE  = 6,
DATA_IN_3_QUEUE	  = 7, 
DATA_OUT_4_QUEUE  = 8,
DATA_IN_4_QUEUE   = 9,
DATA_OUT_5_QUEUE  = 10,
DATA_IN_5_QUEUE   = 11,
DATA_OUT_6_QUEUE  = 12,
DATA_IN_6_QUEUE   = 13,
DATA_OUT_7_QUEUE  = 14,
DATA_IN_7_QUEUE   = 15,
DATA_OUT_8_QUEUE  = 16,
DATA_IN_8_QUEUE   = 17,
TOTAL_QUEUE       = 18
} t_Queue_type;

uint8_t free_Tx_buf(uint32_t tmp_buf);
void release_Tx_entry(uint8_t which_queue);
uint8_t *alloc_Tx_buf(void);
void init_data_flow(void);
uint8_t is_empty(uint32_t *p_queue, uint8_t index);
uint32_t *get_queue(uint8_t which_queue);
uint8_t APS_dispatch_queue(uint32_t target_addr);
uint8_t APS_write_Tx_queue(uint8_t which_queue, uint8_t *data);
uint8_t APS_send_command(uint8_t access_user, uint8_t *data);
void APS_send_command_EX(uint8_t *data, uint32_t length, uint8_t source);
uint32_t APS_read_Rx_queue(uint8_t which_queue);
void APS_free_Rx_buf(uint8_t which_queue);
void APS_rev_event(void);
void APS_rev_data(void);
void data_flow_task(void);

typedef int (*T_IpcTestProcFp)(void* pdata);

extern T_IpcTestProcFp ipc_rf_at_process_M0ToM3;
extern T_IpcHandleFp ipc_evt_handle;
extern T_IpcHandleFp ipc_ble_evt_handle;
extern T_IpcHandleFp ipc_ble_rx_handle;
extern T_IpcHandleFp ipc_wifi_evt_handle;
extern T_IpcHandleFp ipc_wifi_aps_rx_handle;
extern T_IpcHandleFp ipc_m0_msg_handle;

int ipc_evt_handle_impl(uint32_t dwType, uint32_t dwIdx, void *pBuf, uint32_t dwBufSize);
int ipc_ble_evt_handle_impl(uint32_t dwType, uint32_t dwIdx, void *pBuf, uint32_t dwBufSize);
int ipc_ble_rx_handle_impl(uint32_t dwType, uint32_t dwIdx, void *pBuf, uint32_t dwBufSize);
int ipc_wifi_evt_handle_impl(uint32_t dwType, uint32_t dwIdx, void *pBuf, uint32_t dwBufSize);
int ipc_wifi_aps_rx_handle_impl(uint32_t dwType, uint32_t dwIdx, void *pBuf, uint32_t dwBufSize);
int ipc_rf_at_process_M0ToM3_impl(void* pdata);
int ipc_m0_msg_handle_impl(uint32_t dwType, uint32_t dwIdx, void *pBuf, uint32_t dwBufSize);

#endif

