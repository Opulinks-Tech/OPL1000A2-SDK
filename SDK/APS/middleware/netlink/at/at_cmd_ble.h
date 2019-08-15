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

#ifndef __AT_CMD_BLE_H__
#define __AT_CMD_BLE_H__
/**
 * @file at_cmd_ble.h
 * @author Michael Liao
 * @date 14 Dec 2017
 * @brief File containing declaration of at_cmd_ble api & definition of structure for reference.
 *
 */

/*
 * @brief AT Command Interface Initialization for BLE module
 *
 */
//void at_cmd_ble_func_init(void);

//void at_cmd_ble_func_init(void);
int at_cmd_ble_mode(char *buf, int len, int mode);
int at_cmd_letest(char *buf, int len, int mode);
int at_cmd_mp_ble_addr(char *buf, int len, int mode);

#endif //__AT_CMD_BLE_H__

