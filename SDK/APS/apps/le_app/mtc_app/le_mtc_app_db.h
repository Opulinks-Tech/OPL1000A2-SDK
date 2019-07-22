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
#ifndef _LE_MTC_APP_DB_H_
#define _LE_MTC_APP_DB_H_

#include "le_mtc_app_fn.h"

#ifdef INCLUDE_MTC_APP

enum
{
	LE_MTC_SVC_ID_GAP,
	LE_MTC_SVC_ID_GATT,
};

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

    GAP_IDX_SPPS_IN_CHAR,
    GAP_IDX_SPPS_IN_VAL,

    GAP_IDX_SPPS_OUT_CHAR,
    GAP_IDX_SPPS_OUT_VAL,
    GAP_IDX_SPPS_OUT_CFG,

	GAP_IDX_DEVICE_NAME_CHAR,
	GAP_IDX_DEVICE_NAME_VAL,

 	GAP_IDX_CONN_PARAM_CHAR,
	GAP_IDX_CONN_PARAM_VAL,

    GAP_IDX_TOP
};


typedef struct _LE_MTC_GATT_TABLE_T
{
	LE_GATT_ATTR_T		*tbl;
	UINT16				count;
} LE_MTC_GATT_TABLE_T;

#endif

#endif
