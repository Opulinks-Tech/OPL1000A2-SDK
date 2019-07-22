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
#ifndef _LE_MTC_APP_FN_H_
#define _LE_MTC_APP_FN_H_

#include "le_host.h"
#include "le_gap_if.h"
#include "le_gatt_if.h"
#include "le_cm_if.h"


#define INCLUDE_MTC_APP

#ifdef INCLUDE_MTC_APP

// le_mtc_app_db
typedef struct _LE_MTC_GATT_TABLE_T LE_MTC_GATT_TABLE_T;


typedef void (*T_LeMtcAppGattDbInitFp)(void);
typedef LE_MTC_GATT_TABLE_T* (*T_LeMtcAppSvcTblFp)(UINT16 sid);

extern T_LeMtcAppGattDbInitFp LeMtcAppGattDbInit;
extern T_LeMtcAppSvcTblFp LeMtcAppSvcTbl;

void LeMtcAppDbPatch(void);

// le_mtc_app_gatt
typedef LE_GATT_SERVICE_T* (*T_LeMtcAppGetSvcFp)(UINT16 sid);
typedef UINT16 (*T_LeMtcAppGetServiceHandleFp)(UINT16 svcAttId, UINT16 attId);
typedef void (*T_LeMtcAppGattTaskHandlerFp)(TASK task, MESSAGEID id, MESSAGE message);

extern T_LeMtcAppGetSvcFp LeMtcAppGetSvc;
extern T_LeMtcAppGetServiceHandleFp LeMtcAppGetServiceHandle;
extern T_LeMtcAppGattTaskHandlerFp LeMtcAppGattTaskHandler;

void LeMtcAppGattPatch(void);

// le_mtc_app
typedef struct _LE_HOST_MTC_APP_DATA_T LE_HOST_MTC_APP_DATA_T;
typedef struct _LE_MTC_APP_PEER_T LE_MTC_APP_PEER_T;

typedef void (*T_LeMtcAppSetAdvtisingParaFp)(UINT8 type, UINT8 own_addr_type, LE_BT_ADDR_T *peer_addr, UINT8 filter);
typedef LE_MTC_APP_PEER_T* (*T_LeMtcAppGetPeerFp)(UINT16 conn_hdl);
typedef BOOL (*T_LeMtcAppSendDisconnectReqFp)(UINT16 conn_hdl);
typedef LE_HOST_MTC_APP_DATA_T* (*T_LeHostMtcAppFp)(void);
typedef void (*T_LeMtcAppInitFp)(void);

extern T_LeMtcAppSetAdvtisingParaFp LeMtcAppSetAdvtisingPara;
extern T_LeMtcAppGetPeerFp LeMtcAppGetPeer;
extern T_LeMtcAppSendDisconnectReqFp LeMtcAppSendDisconnectReq;
extern T_LeHostMtcAppFp LeHostMtcApp;
extern T_LeMtcAppInitFp LeMtcAppInit;

void LeMtcAppPatch(void);

// le_mtc_app_cmd
typedef void (*T_LeMtcAppProcessCliCmdFp)(void);
typedef void (*T_ParseLeMtcTestCommandFp)(char* pszData, int dataLen);

extern T_LeMtcAppProcessCliCmdFp LeMtcAppProcessCliCmd;
extern T_ParseLeMtcTestCommandFp ParseLeMtcTestCommand;

void LeMtcAppCmdPatch(void);

#endif

#endif
