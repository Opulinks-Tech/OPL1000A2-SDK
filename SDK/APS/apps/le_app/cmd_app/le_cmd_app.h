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

#ifndef _LE_CMD_APP_H_
#define _LE_CMD_APP_H_

#include "le.h"
#include "le_host.h"
#include "le_gap_if.h"
#include "le_gatt_if.h"
#include "le_smp.h"

#include "le_cmd_app_cmd.h"
#include "le_cmd_app_fn.h"

#ifdef INCLUDE_CMD_APP

#define LE_APP_ROLE_GATT_CLIENT			1
#define LE_APP_ROLE_GATT_SERVER			2

#define LE_APP_MAX_CONNECTION			8


#define HI_UINT16(a) (((a) >> 8) & 0xFF)
#define LO_UINT16(a) ((a) & 0xFF)


enum
{
	LE_APP_USR_MSG_DISCONNECTION = LE_APP_USR_MSG_BASE,

	LE_APP_USR_MSG_BUILD_GATT_DB,
	LE_APP_USR_MSG_START_GATT_DB,
	LE_APP_USR_MSG_SHOW_GATT_SVC,
	LE_APP_USR_MSG_SHOW_GATT_CHAR,
	LE_APP_USR_MSG_START_ADV,
	LE_APP_USR_MSG_SEND_GATT_NOTIFY,

    
	LE_APP_USR_MSG_TOP
};

enum
{
	LE_APP_STATE_UNINIT,
	LE_APP_STATE_IDLE 					= (1 << 0),
	LE_APP_STATE_ADVERTISING 			= (1 << 1),
	LE_APP_STATE_SCANNING 				= (1 << 2),
	LE_APP_STATE_CONNECTING 			= (1 << 3),
	LE_APP_STATE_CONNECTED 				= (1 << 4),
};

enum
{
	LE_APP_ACTION_START_ADVERTISING 	= (1 << 0),
	LE_APP_ACTION_END_ADVERTISING 		= (1 << 1),
	LE_APP_ACTION_START_CONNECTING 		= (1 << 2),
	LE_APP_ACTION_CANCEL_CONNECTING 	= (1 << 3),
	LE_APP_ACTION_START_SCANNING 		= (1 << 4),
	LE_APP_ACTION_END_SCANNING          = (1 << 5),
	
	LE_APP_ACTION_FIND_ALL_SERVICE      = (1 << 6),
};

enum
{
	LE_PEER_ACTION_FIND_ALL_SERVICE = 1,
	LE_PEER_ACTION_FIND_INCL_SERVICE,
	LE_PEER_ACTION_FIND_ALL_CHAR,
	LE_PEER_ACTION_READ_LONG_CHAR_VAL,
	LE_PEER_ACTION_WRITE_CHAR_VAL,


};


typedef struct _LE_APP_GATT_CHAR_T
{
	LE_GATT_ATTR_T		*attr;
    LE_GATT_ATTR_T		*desc;
	UINT16				count;
} LE_APP_GATT_CHAR_T;

typedef struct _LE_APP_GATT_SVC_T
{
	LE_GATT_SERVICE_T	*id;
    LE_APP_GATT_CHAR_T	*chara;
	UINT16				count;
    UINT8				type;
    BOOL				start;
} LE_APP_GATT_SVC_T;

typedef struct
{
    UINT16				conn_hdl;
    UINT16				handle;
    UINT16				len;
    UINT16				idx;
    UINT8				type;
    UINT8				prop;
    LE_GATT_SERVICE_T 	*svc;
	UINT8				*buf;
} LE_APP_GATT_SEND_T;

typedef struct _LE_APP_GATT_SVR_T
{
	LE_APP_GATT_SVC_T	*svc;
	UINT16				count;
    BOOL				init;
} LE_APP_GATT_SVR_T;

typedef struct _LE_APP_PEER_ATTR_T
{
	UINT16				handle;
    BOOL				valid;
	UINT8				type;
	UINT16				uuid[8];
} LE_APP_PEER_ATTR_T;

typedef struct _LE_APP_PEER_CHAR_T
{
	LE_APP_PEER_ATTR_T	*attr;
	LE_APP_PEER_ATTR_T	*desc;
    UINT16				count;
	UINT8				property;
    UINT16				val_hdl;
	UINT16				start_hdl;
    UINT16				end_hdl;
} LE_APP_PEER_CHAR_T;

typedef struct _LE_APP_PEER_SVC_T
{
	UINT16				start_hdl;
    UINT16				end_hdl;
    UINT16				type;
	UINT8				format;
	UINT16				uuid[8];

	LE_APP_PEER_ATTR_T	*attr;
	UINT16				attr_count;
	LE_APP_PEER_ATTR_T	*incl;
	UINT16				incl_count;
    LE_APP_PEER_CHAR_T	*chara;
	UINT16				count;
} LE_APP_PEER_SVC_T;

typedef struct
{
	UINT16				handle[4];
	LE_APP_PEER_ATTR_T	*attr;
	UINT16				count;
} LE_APP_PEER_ATTR_ITEM_T;

typedef struct
{
	UINT16				handle[4];
	UINT16				svc_idx;
	LE_APP_PEER_SVC_T	*svc;
} LE_APP_PEER_INC_T;

typedef struct _LE_CMD_APP_PEER_T
{
	UINT16						conn_hdl;
	UINT16						mtu;
    UINT8						idx;
    UINT8						valid;
    UINT8						bondid;
	LE_BT_ADDR_T				bd_addr;
    UINT16						itv;
    UINT16						latency;      
    UINT16						sv_tmo;

    UINT16						action;
    UINT16						find;
    UINT8						*wrd_buf;
    UINT16						wrd_len;
	LE_APP_PEER_ATTR_T			*attr;
	LE_APP_PEER_SVC_T			*svc;
	UINT16						attr_count;
	UINT16						svc_count;

    LE_QLIST_T					atti;
	LE_APP_PEER_ATTR_ITEM_T 	*curr_atti;
    
    LE_QLIST_T					srv;
    LE_QLIST_T					inc;

    BOOL						discon;
    UINT16						svr_ind;
} LE_CMD_APP_PEER_T;

typedef struct _LE_HOST_CMD_APP_DATA_T
{
	TASKPACK							task;
	UINT16								state;
    UINT16								action;

	LE_APP_GATT_SVR_T					server;
	LE_APP_GATT_SEND_T					*send;

	UINT8								init_role;
#ifdef LE_ACTION_FOR_DEVELOP
	UINT8								adv_data[31];
	UINT8								adv_data_size;
	UINT8								scan_data[31];
	UINT8								scan_data_size;
#endif
	BD_ADDR								pub_addr;
	BD_ADDR								sta_addr;
	UINT8								sta_addr_change;

	//UINT8								*dev_name;
	//UINT16							name_len;
	LE_GATT_SERVICE_T 					*name_svc;
	UINT16 								name_attrid;



	LE_GAP_ADVERTISING_PARAM_T			adv_para;
	UINT8								adv_para_change;

	LE_CONN_PARA_T 						conn_param;

	
	LE_CMD_APP_PEER_T					peer[LE_APP_MAX_CONNECTION];
	UINT8								peer_count;

	// function
	T_LeCmdAppSendAtCmdAckFp					fnSendCmdAck;
	T_LeCmdAppGetPeerByHdlFp					fnGetPeerByHdl;
	T_LeCmdAppBuildGattTableFp					fnBuildGattTable;
	T_LeCmdAppFreePeerGattDbFp					fnFreePeerGattDb;
	T_LeCmdAppClearSendFlagFp					fnClearSendFlag;

	// state
	T_LeCmdAppShowStateConnectedFp				fnShowConnected;


	// rsp
	T_LeCmdAppSendAtCmdRspFp					fnSendAtCmdRsp;
    T_LeCmdAppSendBleDisconnRspFp				fnSendDisconnRsp;
	T_LeCmdAppSendBleGattcPrimSrvRspFp			fnSendGattcPrimSrvRsp;
	T_LeCmdAppSendBleGattcNoPrimSrvRspFp		fnSendGattcNoPrimSrvRsp;
	T_LeCmdAppSendBleGattcInclSrvRspFp			fnSendGattcInclSrvRsp;
	T_LeCmdAppSendBleGattcCharInfoRspFp			fnSendGattcCharInfoRsp;
	T_LeCmdAppSendBleGattcDescInfoRspFp			fnSendGattcDescInfoRsp;
	T_LeCmdAppSendBleGattcCharReadRspFp			fnSendGattcCharReadRsp;
    
} LE_HOST_CMD_APP_DATA_T;


#endif

#endif
