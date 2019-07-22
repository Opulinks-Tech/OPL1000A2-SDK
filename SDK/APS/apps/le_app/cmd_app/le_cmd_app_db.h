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

#ifndef _LE_CMD_APP_DB_H_
#define _LE_CMD_APP_DB_H_

#include "le.h"
#include "le_host.h"
#include "le_gatt_if.h"


#ifdef INCLUDE_CMD_APP



#pragma pack(push, 1)

typedef struct
{
	UINT32				magic_num;
	UINT32				total_len;
	UINT16				total_item;
} LE_GATT_DB_HDR_T;

typedef struct
{
	UINT16				len;
	UINT16				index;
	UINT8				uuid_len;
    UINT16				uuid;
    UINT16				permit;
	UINT16				max_len;
	UINT16				curr_len;
    UINT8				val[1];
} LE_GATT_DB_ITEM_MBR_16_T;

typedef struct
{
	UINT16				len;
	UINT16				index;
	UINT8				uuid_len;
    UINT16				uuid[8];
    UINT16				permit;
	UINT16				max_len;
	UINT16				curr_len;
    UINT8				val[1];
} LE_GATT_DB_ITEM_MBR_128_T;

#pragma pack(pop)

typedef union
{
	LE_GATT_DB_ITEM_MBR_16_T	mbr16;
	LE_GATT_DB_ITEM_MBR_128_T	mbr128;
} LE_GATT_DB_ITEM_MBR_T;

typedef struct
{
	UINT16				handle;
	UINT8				format;         
	UINT16 				*pUuid;
	UINT16				permit;   
    UINT16				maxLen;
    UINT16				len;
	UINT8 				*pVal; 
} LE_GATT_DB_ATTR_T;

typedef struct _LE_GATT_DB_SVC_T
{
    LE_GATT_DB_ATTR_T	*attr;
	UINT16				start_idx;
	UINT16				count;
} LE_GATT_DB_SVC_T;

typedef struct _LE_GATT_DB_T
{
	LE_GATT_DB_HDR_T	hdr;
    UINT8				*raw;
	UINT16				need_buf;
	UINT8				*buf;
    
    LE_GATT_DB_ATTR_T	*attr;
	LE_GATT_DB_SVC_T	*svc;
	UINT16				svc_count;

    LE_GATT_SERVICE_T   **svc_id;
} LE_GATT_DB_T;


#ifdef LE_DB_FROM_INTERNAL

typedef struct _LE_GATT_TABLE_T
{
	LE_GATT_ATTR_T		*tbl;
	UINT16				count;
} LE_GATT_TABLE_T;


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

	GAP_IDX_TX_POWER_LEVEL_CHAR,
	GAP_IDX_TX_POWER_LEVEL_VAL,

	GAP_IDX_PRIVACY_FLAG_CHAR,
	GAP_IDX_PRIVACY_FLAG_VAL,
	GAP_IDX_RECONN_ADDR_CHAR,
	GAP_IDX_RECONN_ADDR_VAL,

	GAP_IDX_APPEARANCE_CHAR,
	GAP_IDX_APPEARANCE_VAL,

	GAP_IDX_CONN_PARAM_CHAR,
	GAP_IDX_CONN_PARAM_VAL,




    GAP_IDX_TOP
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
    BPS_IDX_SVC,

    BPS_IDX_BP_MEAS_CHAR,
    BPS_IDX_BP_MEAS_VAL,
    BPS_IDX_BP_MEAS_IND_CFG,

    BPS_IDX_BP_FEATURE_CHAR,
    BPS_IDX_BP_FEATURE_VAL,

    BPS_IDX_INTM_CUFF_PRESS_CHAR,
    BPS_IDX_INTM_CUFF_PRESS_VAL,
    BPS_IDX_INTM_CUFF_PRESS_NTF_CFG,

    BPS_IDX_PTS_TEST_ITEM_CHAR,
    BPS_IDX_PTS_TEST_ITEM_VAL,
    BPS_IDX_PTS_TEST_ITEM2_CHAR,
    BPS_IDX_PTS_TEST_ITEM2_VAL,
    BPS_IDX_PTS_TEST_ITEM2_USER_DESC,
    BPS_IDX_PTS_TEST_ITEM3_CHAR,
    BPS_IDX_PTS_TEST_ITEM3_VAL,
    BPS_IDX_PTS_TEST_ITEM3_USER_DESC,
    BPS_IDX_PTS_TEST_ITEM3_EXTEND_PROP_DESC,
    BPS_IDX_PTS_TEST_SERVER_CONFIG_DESC,

    BPS_IDX_PTS_TEST_ITEM4_CHAR,
    BPS_IDX_PTS_TEST_ITEM4_VAL,
    BPS_IDX_PTS_TEST_ITEM4_PRESENT_FORMAT,
    BPS_IDX_PTS_TEST_ITEM4_PRESENT_FORMAT1,
    BPS_IDX_PTS_TEST_ITEM4_AGGREGATE_FORMAT,

    BPS_IDX_TOP
};

enum
{
    HOGPD_IDX_SVC,

    // Included Service
    HOGPD_IDX_INCL_SVC,

    // HID Information
    HOGPD_IDX_HID_INFO_CHAR,
    HOGPD_IDX_HID_INFO_VAL,

    // HID Control Point
    HOGPD_IDX_HID_CTNL_PT_CHAR,
    HOGPD_IDX_HID_CTNL_PT_VAL,

    // Report Map
    HOGPD_IDX_REPORT_MAP_CHAR,
    HOGPD_IDX_REPORT_MAP_VAL,
    HOGPD_IDX_REPORT_MAP_EXT_REP_REF,

    // Protocol Mode
    HOGPD_IDX_PROTO_MODE_CHAR,
    HOGPD_IDX_PROTO_MODE_VAL,

    // Boot Keyboard Input Report
    HOGPD_IDX_BOOT_KB_IN_REPORT_CHAR,
    HOGPD_IDX_BOOT_KB_IN_REPORT_VAL,
    HOGPD_IDX_BOOT_KB_IN_REPORT_NTF_CFG,

    // Boot Keyboard Output Report
    HOGPD_IDX_BOOT_KB_OUT_REPORT_CHAR,
    HOGPD_IDX_BOOT_KB_OUT_REPORT_VAL,

    // Boot Mouse Input Report
    HOGPD_IDX_BOOT_MOUSE_IN_REPORT_CHAR,
    HOGPD_IDX_BOOT_MOUSE_IN_REPORT_VAL,
    HOGPD_IDX_BOOT_MOUSE_IN_REPORT_NTF_CFG,

    // Report
    HOGPD_IDX_REPORT_CHAR,
    HOGPD_IDX_REPORT_VAL,
    HOGPD_IDX_REPORT_REP_REF,
    HOGPD_IDX_REPORT_NTF_CFG,

    HOGPD_IDX_NB,
};
#endif

#endif

#endif
