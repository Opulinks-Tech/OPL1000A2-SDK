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
#ifndef _LE_PTS_APP_FN_H_
#define _LE_PTS_APP_FN_H_

#include "le_host.h"
#include "le_gap_if.h"
#include "le_gatt_if.h"
#include "le_cm_if.h"


#define INCLUDE_PTS_APP

#ifdef INCLUDE_PTS_APP

// le_pts_app_db
typedef struct _LE_PTS_GATT_TABLE_T LE_PTS_GATT_TABLE_T;

typedef void (*T_LePtsAppGattDbInitFp)(void);
typedef LE_PTS_GATT_TABLE_T* (*T_LePtsAppSvcTblFp)(UINT16 sid);

extern T_LePtsAppGattDbInitFp LePtsAppGattDbInit;
extern T_LePtsAppSvcTblFp LePtsAppSvcTbl;

void LePtsAppDbPatch(void);

// le_pts_app_gatt
typedef LE_GATT_SERVICE_T* (*T_LePtsAppGetSvcFp)(UINT16 sid);
typedef UINT16 (*T_LePtsAppGetServiceHandleFp)(UINT16 svcAttId, UINT16 attId);
typedef void (*T_LePtsAppGattTaskHandlerFp)(TASK task, MESSAGEID id, MESSAGE message);

extern T_LePtsAppGetSvcFp LePtsAppGetSvc;
extern T_LePtsAppGetServiceHandleFp LePtsAppGetServiceHandle;
extern T_LePtsAppGattTaskHandlerFp LePtsAppGattTaskHandler;

void LePtsAppGattPatch(void);

// le_pts_app
typedef struct _LE_HOST_PTS_APP_DATA_T LE_HOST_PTS_APP_DATA_T;

typedef LE_HOST_PTS_APP_DATA_T* (*T_LeHostPtsAppFp)(void);
typedef void (*T_LePtsAppInitFp)(void);
typedef BOOL (*T_LePtsAppSetGetPtsAddrFp)(BOOL set, LE_BT_ADDR_T *pts_addr);
typedef BOOL (*T_LePtsAppSetDirectAdvtisingParaFp)(LE_BT_ADDR_T *own_rnd_addr, LE_BT_ADDR_T *peer_addr);
typedef void (*T_LePtsAppSetAdvtisingParaFp)(UINT8 type, UINT8 own_addr_type, LE_BT_ADDR_T *peer_addr, UINT8 filter);

extern T_LeHostPtsAppFp LeHostPtsApp;
extern T_LePtsAppInitFp LePtsAppInit;
extern T_LePtsAppSetGetPtsAddrFp LePtsAppSetGetPtsAddr;
extern T_LePtsAppSetDirectAdvtisingParaFp LePtsAppSetDirectAdvtisingPara;
extern T_LePtsAppSetAdvtisingParaFp LePtsAppSetAdvtisingPara;

void LePtsAppPatch(void);

// le_pts_app_cmd
typedef void (*T_LePtsAppProcessCliCmdFp)(void);
typedef void (*T_ParseLePtsTestCommandFp)(char* pszData, int dataLen);

extern T_LePtsAppProcessCliCmdFp LePtsAppProcessCliCmd;
extern T_ParseLePtsTestCommandFp ParseLePtsTestCommand;

void LePtsAppCmdPatch(void);

#endif

#endif
