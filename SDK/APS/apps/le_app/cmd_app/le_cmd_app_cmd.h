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

#ifndef _LE_CMD_APP_CMD_H_
#define _LE_CMD_APP_CMD_H_

#include "le.h"
#include "le_misc_patch.h"

#include "le_cmd_app_fn.h"

#ifdef INCLUDE_CMD_APP

#define LE_AT_CMD_TYPE_INVALID			0
#define LE_AT_CMD_TYPE_TEST				1
#define LE_AT_CMD_TYPE_INQUIRY			2
#define LE_AT_CMD_TYPE_SET				3
#define LE_AT_CMD_TYPE_RUN				4

#define LE_OCCUPY_UART_ID_NONE			0
#define LE_OCCUPY_UART_ID_NTY			1
#define LE_OCCUPY_UART_ID_IND			2
#define LE_OCCUPY_UART_ID_WR			3
#define LE_OCCUPY_UART_ID_CHG			4


#define LE_AT_CMD_BLE_INIT				"bleinit"
#define LE_AT_CMD_BLE_ADDR				"bleaddr"
#define LE_AT_CMD_BLE_NAME				"blename"
#define LE_AT_CMD_BLE_SCAN_RSP_DATA		"blescanrspdata"
#define LE_AT_CMD_BLE_ADV_PARAM			"bleadvparam"
#define LE_AT_CMD_BLE_ADV_DATA			"bleadvdata"
#define LE_AT_CMD_BLE_ADV_START			"bleadvstart"
#define LE_AT_CMD_BLE_ADV_STOP			"bleadvstop"
#define LE_AT_CMD_BLE_CONN_PARAM		"bleconnparam"
#define LE_AT_CMD_BLE_CONN				"bleconn"
#define LE_AT_CMD_BLE_DISCONN			"bledisconn"
#define LE_AT_CMD_BLE_DATALEN			"bledatalen"
#define LE_AT_CMD_BLE_CFG_MTU			"blecfgmtu"
#define LE_AT_CMD_BLE_GATTS_SRV_CREAT	"blegattssrvcre"
#define LE_AT_CMD_BLE_GATTS_SRV_START	"blegattssrvstart"
#define LE_AT_CMD_BLE_GATTS_SRV_STOP	"blegattssrvstop"
#define LE_AT_CMD_BLE_GATTS_SRV			"blegattssrv"
#define LE_AT_CMD_BLE_GATTS_CHAR		"blegattschar"
#define LE_AT_CMD_BLE_GATTS_NTFY		"blegattsntfy"
#define LE_AT_CMD_BLE_GATTS_IND			"blegattsind"
#define LE_AT_CMD_BLE_GATTS_SET_ATTR	"blegattssetattr"
#define LE_AT_CMD_BLE_GATTC_PRIM_SRV	"blegattcprimsrv"
#define LE_AT_CMD_BLE_GATTC_INCL_SRV	"blegattcinclsrv"
#define LE_AT_CMD_BLE_GATTC_CHAR		"blegattcchar"
#define LE_AT_CMD_BLE_GATTC_CHAR_READ	"blegattcrd"
#define LE_AT_CMD_BLE_GATTC_CHAR_WRITE	"blegattcwr"
#define LE_AT_CMD_BLE_PHY_SET			"blephyset"
#define LE_AT_CMD_BLE_PHY_READ			"blephyread"

#ifdef LE_CMD_APP_OPTION_CMD
#define LE_AT_CMD_BLE_CHANGE_CTRLER		"blectrl"
#define LE_AT_CMD_BLE_DEBUG_SWITCH		"bledbgsw"
#define LE_AT_CMD_BLE_UART_SWITCH		"bleuartsw"
#ifdef LE_ACTION_FOR_DEVELOP
#define LE_AT_CMD_BLE_AUTO_TEST_SWITCH	"bleautotest"
#endif
#endif
#ifdef LE_DB_FROM_INTERNAL
#define LE_AT_CMD_BLE_INTERNAL_DB		"bletestitd"
#endif
#define LE_AT_CMD_ID_BASE		0x1000

enum
{
	LE_AT_CMD_ID_BLE_INIT = LE_AT_CMD_ID_BASE,
	LE_AT_CMD_ID_BLE_ADDR,
	LE_AT_CMD_ID_BLE_NAME,
	LE_AT_CMD_ID_BLE_SCAN_RSP_DATA,
	LE_AT_CMD_ID_BLE_ADV_PARAM,
	LE_AT_CMD_ID_BLE_ADV_DATA,
	LE_AT_CMD_ID_BLE_ADV_START,
	LE_AT_CMD_ID_BLE_ADV_STOP,
	LE_AT_CMD_ID_BLE_CONN_PARAM,
	//LE_AT_CMD_ID_BLE_CONN,
	LE_AT_CMD_ID_BLE_DISCONN,
	LE_AT_CMD_ID_BLE_DATALEN,
	LE_AT_CMD_ID_BLE_CFG_MTU,
	LE_AT_CMD_ID_BLE_GATTS_SRV_CREAT,
	LE_AT_CMD_ID_BLE_GATTS_SRV_START,
	LE_AT_CMD_ID_BLE_GATTS_SRV_STOP,
	LE_AT_CMD_ID_BLE_GATTS_SRV,
	LE_AT_CMD_ID_BLE_GATTS_CHAR,
	LE_AT_CMD_ID_BLE_GATTS_NTFY,
	LE_AT_CMD_ID_BLE_GATTS_IND,
	LE_AT_CMD_ID_BLE_GATTS_SET_ATTR,
	LE_AT_CMD_ID_BLE_GATTC_PRIM_SRV,
	LE_AT_CMD_ID_BLE_GATTC_INCL_SRV,
	LE_AT_CMD_ID_BLE_GATTC_CHAR,
	LE_AT_CMD_ID_BLE_GATTC_CHAR_READ,
	LE_AT_CMD_ID_BLE_GATTC_CHAR_WRITE,
	LE_AT_CMD_ID_BLE_PHY_SET,
	LE_AT_CMD_ID_BLE_PHY_READ,
#ifdef LE_CMD_APP_OPTION_CMD
	LE_AT_CMD_ID_BLE_CHANGE_CTRLER,
	LE_AT_CMD_ID_BLE_DEBUG_SWITCH,
	LE_AT_CMD_ID_BLE_UART_SWITCH,
#ifdef LE_ACTION_FOR_DEVELOP
	LE_AT_CMD_ID_BLE_AUTO_TEST_SWITCH,
#endif
#endif
#ifdef LE_DB_FROM_INTERNAL
	LE_AT_CMD_ID_BLE_INTERNAL_DB,
#endif
    LE_AT_CMD_ID_TOP
};

typedef struct _LE_HOST_AT_CMD_T
{
	UINT8		handle[8];
    UINT16		id;
    UINT8		type;
    UINT8		*str;
	UINT16		len;
	UINT8		*data;
} LE_HOST_AT_CMD_T;


#define LeCmdAppNumToDecStr(x, y) LeHtcUtilNumToDecStr(x, y)
#define LeCmdAppNumToStr(x, y, z) LeHtcUtilOctetNumToHexStr(x, y, z)


BOOL LeCmdAppProcessAtCmd(char* pszData, int dataLen);

BOOL LeCmdAppProcessSendData(char* pszData, int dataLen);

#else

#define LeCmdAppProcessAtCmd(x, y)		(FALSE)
#define LeCmdAppProcessSendData(x, y)
#endif

#endif
