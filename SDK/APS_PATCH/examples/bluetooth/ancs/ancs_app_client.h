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

#ifndef _ANCS_APP_CLIENT_H_
#define _ANCS_APP_CLIENT_H_

#include "ancs_app.h"


// ANCS Service: 7905F431-B5CE-4E99-A40F-4B1E122D00D0
#define ANCS_SVC_UUID128 				0xD0, 0x00, 0x2D, 0x12, 0x1E, 0x4B, 0x0F, 0xA4, 0x99, 0x4E, 0xCE, 0xB5, 0x31, 0xF4, 0x05, 0x79
// Notification Source: UUID 9FBF120D-6301-42D9-8C58-25E699A21DBD (notifiable)
#define ANCS_NOTIF_SRC_CHAR_UUID128 	0xBD, 0x1D, 0xA2, 0x99, 0xE6, 0x25, 0x58, 0x8C, 0xD9, 0x42, 0x01, 0x63, 0x0D, 0x12, 0xBF, 0x9F
// Control point: UUID 69D1D8F3-45E1-49A8-9821-9BBDFDAAD9D9 (writeable with response)
#define ANCS_CTRL_PT_CHAR_UUID128 		0xD9, 0xD9, 0xAA, 0xFD, 0xBD, 0x9B, 0x21, 0x98, 0xA8, 0x49, 0xE1, 0x45, 0xF3, 0xD8, 0xD1, 0x69
// Data Source: UUID 22EAC6E9-24D6-4BB5-BE44-B36ACE7C7BFB (notifiable)
#define ANCS_DATA_SRC_CHAR_UUID128 		0xFB, 0x7B, 0x7C, 0xCE, 0x6A, 0xB3, 0x44, 0xBE, 0xB5, 0x4B, 0xD6, 0x24, 0xE9, 0xC6, 0xEA, 0x22

#define ANCS_SVC_UUID16      			0x00D0
#define ANCS_NOTIF_SRC_CHAR_UUID16      0x1DBD
#define ANCS_CTRL_PT_CHAR_UUID16        0xD9D9
#define ANCS_DATA_SRC_CHAR_UUID16       0x7BFB


// CategoryID Values
#define CategoryIDOther					0
#define CategoryIDIncomingCall			1
#define CategoryIDMissedCall			2
#define CategoryIDVoicemail				3
#define CategoryIDSocial				4
#define CategoryIDSchedule				5
#define CategoryIDEmail					6
#define CategoryIDNews					7
#define CategoryIDHealthAndFitness		8
#define CategoryIDBusinessAndFinance	9
#define CategoryIDLocation				10
#define CategoryIDEntertainment			11

// EventID Values
#define EventIDNotificationAdded		0
#define EventIDNotificationModified		1
#define EventIDNotificationRemoved		2

// EventFlags
#define EventFlagSilent             	0x01
#define EventFlagImportant          	0x02
#define EventFlagPreExisting        	0x04
#define EventFlagPositiveAction     	0x08
#define EventFlagNegativeAction     	0x10

// CommandID Values
#define CommandIDGetNotificationAttributes      0
#define CommandIDGetAppAttributes               1
#define CommandIDPerformNotificationAction      2

#define ActionIDPositive        0
#define ActionIDNegative        1

// NotificationAttributeID Values
#define NotificationAttributeIDAppIdentifier            0
#define NotificationAttributeIDTitle                    1       // (Needs to be followed by a 2-bytes max length parameter)
#define NotificationAttributeIDSubtitle                 2       // (Needs to be followed by a 2-bytes max length parameter)
#define NotificationAttributeIDMessage                  3       // (Needs to be followed by a 2-bytes max length parameter)
#define NotificationAttributeIDMessageSize              4 
#define NotificationAttributeIDDate                     5 
#define NotificationAttributeIDPositiveActionLabel      6 
#define NotificationAttributeIDNegativeActionLabel      7

// AppAttributeID Values
#define AppAttributeIDDisplayName						0

enum
{
	ANCS_RD_NOTI_SRC_CCCD_VAL,
	ANCS_RD_DATA_SRC_CCCD_VAL,
	ANCS_WR_NOTI_SRC_CCCD_VAL,
	ANCS_WR_DATA_SRC_CCCD_VAL,
	ANCS_WR_CTRL_PT_CMD,

    ANCS_OP_TOP
};


typedef struct
{
	GATT_REQ_HDR_T	hdr;
	UINT16			val;
} ANCS_WR_CCCD_VAL_T;

typedef struct
{
	GATT_REQ_HDR_T	hdr;
	UINT8			cmd_id;
} ANCS_CMD_HDR_T;

typedef struct
{
	GATT_REQ_HDR_T	hdr;
	UINT8			cmd_id;
	UINT16			attr_id_flag;
    UINT32			notify_uid;

	UINT16			chk_idx;
	UINT16			rsp_len;
    UINT8			rsp_buf[256];
} ANCS_GET_NOTI_ATTR_CMD_T;

typedef struct
{
	GATT_REQ_HDR_T	hdr;
	UINT8			cmd_id;
	UINT16			attr_id_flag;
    UINT32			uid;
	UINT16			appid_len;
    
	UINT16			chk_idx;
	UINT16			rsp_len;
    UINT8			rsp_buf[128];
} ANCS_GET_APP_ATTR_CMD_T;

typedef struct
{
	GATT_REQ_HDR_T	hdr;
	UINT8			cmd_id;
    UINT32			uid;
    
	UINT16			chk_idx;
	UINT16			rsp_len;
    UINT8			rsp_buf[32];
} ANCS_NOTI_ACTION_CMD_T;

typedef struct
{
	UINT16			phandle[4];
	UINT8			evt_id;
	UINT8			evt_flag;
	UINT8			category_id;
	UINT8			category_count;
    UINT32			notify_uid;
    UINT16			appid_flag;
    UINT8			appid[64];
} ANCS_NOTIFY_SRC_DATA_T;

typedef struct
{
	BLE_CHAR_DECL_T	decl;
    BLE_CHAR_CCCD_T	*cccd;
} ANCS_CHAR_T;

typedef struct
{
	BLE_SERVICE_T	*svc;
	ANCS_CHAR_T		*noti_src;
	ANCS_CHAR_T		*ctrl_pt;
	ANCS_CHAR_T		*data_src;

    LE_QLIST_T		ansl;
	LE_QLIST_T		cmd_list;
    void			*cmd;
} BLE_ANCS_INFO_T;


void BleAppFindAncsChar(BLE_APP_MSG_COMMON_T *req);

UINT16 BleAppAncsRwCccdValReq(UINT16 conn_hdl, UINT8 op, UINT16 val);

void BleAppAncsInfoClean(UINT16 conn_hdl);

void BleAppAncsHandleNotifyInd(LE_GATT_MSG_NOTIFY_IND_T *ind);

void BleAppAncsHandleReadCharValCfm(LE_GATT_MSG_READ_CHARACTERISTIC_VALUE_CFM_T *cfm);

void BleAppAncsHandleCharValInd(LE_GATT_MSG_CHARACTERISTIC_VAL_IND_T *ind);

void BleAppAncsHandleWriteCharValCfm(LE_GATT_MSG_WRITE_CHAR_VALUE_CFM_T *ind);

void BleAppAncsHandleFindCharCfm(LE_GATT_MSG_FIND_CHARACTERISTIC_CFM_T *cfm);

void BleAppAncsHandleCharDeclInfoInd(LE_GATT_MSG_CHARACTERISTIC_DECL_INFO_IND_T *ind);

BOOL BleAppAncsGetNotifyAttr(UINT16 conn_hdl, UINT32 uid, UINT16 attr_id_flag);

BOOL BleAppAncsGetAppAttr(UINT16 conn_hdl, UINT32 uid);

BOOL BleAppAncsNotifyAction(UINT16 conn_hdl, UINT32 uid, UINT8 action);

BOOL BleAppGattAncsRwReq(UINT16 conn_hdl, void *hdr);

#endif
