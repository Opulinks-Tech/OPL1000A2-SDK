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

#ifndef _BLE_H_
#define _BLE_H_

#include "ble_type_define.h"
#include "ble_err.h"


#define LE_CONN_HANDLE_TOP				0xF00

#define LE_GATT_UUID16					1
#define LE_GATT_UUID128					2

#define LE_MAX_CONNECTION_COUNT			8

#define LE_CONN_ROLE_MASTER				0
#define LE_CONN_ROLE_SLAVE				1

#define LE_ATT_DEFAULT_MTU          	23
#define LE_ATT_MAX_MTU          		247
#define LE_ATT_ATTR_MAX_LEN             512

/* Address types */
#define LE_ADDR_TYPE_PUBLIC            (0)
#define LE_ADDR_TYPE_RANDOM            (1)
#define LE_ADDR_TYPE_RPA_PUB_DEFAULT   (2)
#define LE_ADDR_TYPE_RPA_RND_DEFAULT   (3)


#define LE_BD_ADDR_LEN 		   		   (6)


typedef UINT8 BD_ADDR[LE_BD_ADDR_LEN];

typedef struct
{
	UINT8		type;              /**< address type */
	BD_ADDR		addr;              /**< address */
} LE_BT_ADDR_T;

typedef struct
{
    UINT16 		itv_min;           /**< mininum connection interval */ 
    UINT16		itv_max;           /**< maxinum connection interval */
    UINT16		latency;           /**< slave latency */
    UINT16		sv_timeout;        /**< supervision timeout */
} LE_CONN_PARA_T;

#endif
