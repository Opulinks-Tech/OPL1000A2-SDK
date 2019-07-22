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

#ifndef __AT_CMD_DATA_PROCESS_H__
#define __AT_CMD_DATA_PROCESS_H__
/**
 * @file at_cmd_data_process.h
 * @author Michael Liao
 * @date 21 Mar 2018
 * @brief File containing declaration of at data processing api & definition of structure for reference.
 *
 */

#define LOCK_NONE    0x00000000U
#define LOCK_WIFI    0x00000001U
#define LOCK_BLE     0x00000002U
#define LOCK_TCPIP   0x00000004U
#define LOCK_RF      0x00000008U
#define LOCK_PIP     0x00000010U
#define LOCK_OTHERS  0x00000020U

#define PREFIX_AT_CMD_READ "?"
#define PREFIX_AT_CMD_EXE ""
#define PREFIX_ATCMD_SET "="
#define PREFIX_AT_CMD_TEST "=?"

#define AT_TCP_TRANS_LOCK_ID 0xFFFFFFFF

typedef enum {
    AT_CMD_MODE_READ,       /**< Read mode command, such as "AT+CMD?". */
    AT_CMD_MODE_EXECUTION,  /**< Active mode command, such as "AT+CMD". */
    AT_CMD_MODE_SET,        /**< Execute mode command, such as "AT+CMD=<op>". */
    AT_CMD_MODE_TESTING,    /**< Test mode command, such as "AT+CMD=?". */
    AT_CMD_MODE_INVALID     /**< The input command doesn't belong to any of the four types. */
} at_cmd_mode_t;

void data_process_init(void);
int data_process_wifi_parse(char *pbuf, int len, int mode);
int data_process_wifi(char *pbuf, int len, int mode);
int data_process_ble(char *pbuf, int len, int mode);
int data_process_tcpip(char *pbuf, int len, int mode);
int data_process_sys(char *pbuf, int len, int mode);
int data_process_rf(char *pbuf, int len, int mode);
int data_process_pip(char *pbuf, int len, int mode);
int data_process_others(char *pbuf, int len, int mode);
int data_process_cmd_mode(char *pbuf);
int data_process_handler(char *pbuf, int len);
void data_process_func_init(void);
void data_process_lock(int module, int data_len);
void data_process_unlock(void);
int data_process_lock_get(void);
int data_process_data_len_get(void);

#endif

