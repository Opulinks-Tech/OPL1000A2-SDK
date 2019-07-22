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

#ifndef _LE_CMD_APP_FN_H_
#define _LE_CMD_APP_FN_H_

#include "le_host.h"
#include "le_gap_if.h"
#include "le_gatt_if.h"
#include "le_cm_if.h"

#define INCLUDE_CMD_APP

#ifdef INCLUDE_CMD_APP

#define LE_ACTION_FOR_DEVELOP
#define LE_DB_FROM_INTERNAL
#define VER_AO_ROM
#define LE_CMD_APP_OPTION_CMD


#define LE_HTC_SUCCESS			0


// le_cmd_app
typedef struct _LE_CMD_APP_PEER_T LE_CMD_APP_PEER_T;
typedef struct _LE_HOST_CMD_APP_DATA_T LE_HOST_CMD_APP_DATA_T;


typedef void* (*T_LeCmdAppGetPeerByHdlFp)(UINT16 conn_hdl);
typedef void (*T_LeCmdAppFreePeerGattDbFp)(LE_CMD_APP_PEER_T *peer);
typedef void (*T_LeCmdAppClearSendFlagFp)(void);

typedef void (*T_LeCmdAppInitFp)(void);
typedef LE_HOST_CMD_APP_DATA_T* (*T_LeHostCmdAppFp)(void);
#ifdef LE_ACTION_FOR_DEVELOP
typedef void (*T_LeCmdAppEnableAutoTestFp)(void);
#endif
 
extern T_LeCmdAppInitFp LeCmdAppInit;
extern T_LeHostCmdAppFp LeHostCmdApp;
#ifdef LE_ACTION_FOR_DEVELOP
extern T_LeCmdAppEnableAutoTestFp LeCmdAppEnableAutoTest;
#endif

void LeCmdAppFnAssign(void);

void LeCmdAppPatch(void);

// le_cmd_app_gatt
typedef LE_ERR_STATE (*T_LeCmdAppBuildGattTableFp)(void);

void LeCmdAppGattFuncAssign(void);

// le_cmd_app_cmd
typedef struct _LE_APP_PEER_SVC_T LE_APP_PEER_SVC_T;
typedef struct _LE_APP_PEER_CHAR_T LE_APP_PEER_CHAR_T;
typedef struct _LE_APP_PEER_ATTR_T LE_APP_PEER_ATTR_T;
typedef struct _LE_APP_GATT_SVR_T LE_APP_GATT_SVR_T;
typedef struct _LE_APP_GATT_SVC_T LE_APP_GATT_SVC_T;
typedef struct _LE_APP_GATT_CHAR_T LE_APP_GATT_CHAR_T;
typedef struct _LE_HOST_AT_CMD_T LE_HOST_AT_CMD_T;


typedef void (*T_LeCmdAppSendAtCmdRspFp)(char *fmt, ...);
typedef void (*T_LeCmdAppShowStateConnectedFp)(UINT8 conn_idx, UINT8 addr_type, BD_ADDR addr);
typedef void (*T_LeCmdAppSendAtCmdAckFp)(void *str, UINT16 err);
typedef void (*T_LeCmdAppSendBleGattcPrimSrvRspFp)(UINT16 conn_idx, UINT16 srv_idx, UINT8 format, UINT16 *uuid, UINT8 srv_type);
typedef void (*T_LeCmdAppSendBleGattcNoPrimSrvRspFp)(UINT16 conn_idx);
typedef void (*T_LeCmdAppSendBleGattcInclSrvRspFp)(LE_CMD_APP_PEER_T *peer, UINT16 srv_idx, UINT8 format, UINT16 *incl_uuid, UINT8 incl_type);
typedef void (*T_LeCmdAppSendBleGattcCharInfoRspFp)(UINT16 conn_idx, UINT16 srv_idx, UINT16 char_idx, UINT8 format, UINT16 *uuid, UINT8 char_prop);
typedef void (*T_LeCmdAppSendBleGattcDescInfoRspFp)(UINT16 conn_idx, UINT16 srv_idx, UINT16 char_idx, UINT16 desc_idx, UINT8 format, UINT16 *uuid);
typedef void (*T_LeCmdAppSendBleGattcCharReadRspFp)(UINT16 conn_idx, UINT16 len, UINT8 *val);
typedef void (*T_LeCmdAppSendBleDisconnRspFp)(UINT16 conn_idx, BD_ADDR addr);


typedef void (*T_ParseLeCmdTestCommandFp)(char* pszData, int dataLen);
typedef void (*T_LeCmdAppProcessCliCmdFp)(void);

extern T_LeCmdAppProcessCliCmdFp LeCmdAppProcessCliCmd;
extern T_ParseLeCmdTestCommandFp ParseLeCmdTestCommand;

void LeCmdAppCmdFuncAssign(void);

void LeCmdAppCmdPatch(void);

// le_cmd_app_db
typedef LE_ERR_STATE (*T_LeGattDbBuildServiceDbFp)(UINT16 *count);
typedef LE_GATT_SERVICE_T* (*T_LeGattDbGetServiceIdFp)(UINT16 idx);
#ifdef LE_DB_FROM_INTERNAL
typedef void (*T_LeGattDbEnableInternalDbFp)(void);
#endif

extern T_LeGattDbBuildServiceDbFp LeGattDbBuildServiceDb;
extern T_LeGattDbGetServiceIdFp LeGattDbGetServiceId;
#ifdef LE_DB_FROM_INTERNAL
extern T_LeGattDbEnableInternalDbFp LeGattDbEnableInternalDb;
#endif

void LeCmdAppDbPatch(void);

// le_cmd_app_gatt
typedef void (*T_LeCmdAppGattTaskHandlerFp)(TASK task, MESSAGEID id, MESSAGE message);

extern T_LeCmdAppGattTaskHandlerFp LeCmdAppGattTaskHandler;

void LeCmdAppGattFuncAssign(void);

void LeCmdAppGattPatch(void);

#endif

#endif
