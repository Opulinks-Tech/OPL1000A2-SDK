/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

#include "ble_hci_if.h"
#include "ble_cm_if.h"
#include "ble_smp_if.h"
#include "ble_gap_if.h"
#include "ble_gatt_if.h"

#include "blewifi_common.h"
#include "blewifi_configuration.h"
#include "blewifi_server_app.h"
#include "blewifi_server_app_gatt.h"
#include "blewifi_ctrl.h"

#include "mw_fim_default_group08.h"
#include "mw_fim_default_group08_project.h"

extern LE_ERR_STATE LeGapGetBdAddr(BD_ADDR addr);
extern T_BleWifi_Ble_MsgHandlerTbl gBleGattMsgHandlerTbl[];

static BLE_APP_DATA_T gTheBle;
static BLE_ADV_TIME_T gTheBleAdvTime;

static void BleWifi_Ble_SmMsgHandler_PairingActionInd(TASK task, MESSAGEID id, MESSAGE message);
static void BleWifi_Ble_SmMsgHandler_EncryptionChangeInd(TASK task, MESSAGEID id, MESSAGE message);
static void BleWifi_Ble_SmMsgHandler_EncryptionRefreshInd(TASK task, MESSAGEID id, MESSAGE message);
static void BleWifi_Ble_SmMsgHandler_PairingCompleteInd(TASK task, MESSAGEID id, MESSAGE message);
static T_BleWifi_Ble_MsgHandlerTbl gBleSmMsgHandlerTbl[] =
{
    {LE_SMP_MSG_PAIRING_ACTION_IND,     BleWifi_Ble_SmMsgHandler_PairingActionInd},
    {LE_SMP_MSG_ENCRYPTION_CHANGE_IND,  BleWifi_Ble_SmMsgHandler_EncryptionChangeInd},
    {LE_SMP_MSG_ENCRYPTION_REFRESH_IND, BleWifi_Ble_SmMsgHandler_EncryptionRefreshInd},
    {LE_SMP_MSG_PAIRING_COMPLETE_IND,   BleWifi_Ble_SmMsgHandler_PairingCompleteInd},
    
    {0xFFFFFFFF,                        NULL}
};

static void BleWifi_Ble_CmMsgHandler_InitCompleteCfm(TASK task, MESSAGEID id, MESSAGE message);
static void BleWifi_Ble_CmMsgHandler_SetAdvertisingDataCfm(TASK task, MESSAGEID id, MESSAGE message);
static void BleWifi_Ble_CmMsgHandler_SetScanRspDataCfm(TASK task, MESSAGEID id, MESSAGE message);
static void BleWifi_Ble_CmMsgHandler_SetAdvertisingParamsCfm(TASK task, MESSAGEID id, MESSAGE message);
static void BleWifi_Ble_CmMsgHandler_EnterAdvertisingCfm(TASK task, MESSAGEID id, MESSAGE message);
static void BleWifi_Ble_CmMsgHandler_ExitAdvertisingCfm(TASK task, MESSAGEID id, MESSAGE message);
static void BleWifi_Ble_CmMsgHandler_ConnectionCompleteInd(TASK task, MESSAGEID id, MESSAGE message);
static void BleWifi_Ble_CmMsgHandler_SignalUpdateReq(TASK task, MESSAGEID id, MESSAGE message);
static void BleWifi_Ble_CmMsgHandler_ConnParaReq(TASK task, MESSAGEID id, MESSAGE message);
static void BleWifi_Ble_CmMsgHandler_ConnUpdateCompleteInd(TASK task, MESSAGEID id, MESSAGE message);
static void BleWifi_Ble_CmMsgHandler_SetDisconnectCfm(TASK task, MESSAGEID id, MESSAGE message);
static void BleWifi_Ble_CmMsgHandler_DisconnectCompleteInd(TASK task, MESSAGEID id, MESSAGE message);
static T_BleWifi_Ble_MsgHandlerTbl gBleCmMsgHandlerTbl[] =
{
    {LE_CM_MSG_INIT_COMPLETE_CFM,           BleWifi_Ble_CmMsgHandler_InitCompleteCfm},
    {LE_CM_MSG_SET_ADVERTISING_DATA_CFM,    BleWifi_Ble_CmMsgHandler_SetAdvertisingDataCfm},
    {LE_CM_MSG_SET_SCAN_RSP_DATA_CFM,       BleWifi_Ble_CmMsgHandler_SetScanRspDataCfm},
    {LE_CM_MSG_SET_ADVERTISING_PARAMS_CFM,  BleWifi_Ble_CmMsgHandler_SetAdvertisingParamsCfm},
    {LE_CM_MSG_ENTER_ADVERTISING_CFM,       BleWifi_Ble_CmMsgHandler_EnterAdvertisingCfm},
    {LE_CM_MSG_EXIT_ADVERTISING_CFM,        BleWifi_Ble_CmMsgHandler_ExitAdvertisingCfm},
    {LE_CM_CONNECTION_COMPLETE_IND,         BleWifi_Ble_CmMsgHandler_ConnectionCompleteInd},
    {LE_CM_MSG_SIGNAL_UPDATE_REQ,           BleWifi_Ble_CmMsgHandler_SignalUpdateReq},
    {LE_CM_MSG_CONN_PARA_REQ,               BleWifi_Ble_CmMsgHandler_ConnParaReq},
    {LE_CM_MSG_CONN_UPDATE_COMPLETE_IND,    BleWifi_Ble_CmMsgHandler_ConnUpdateCompleteInd},
    {LE_CM_MSG_SET_DISCONNECT_CFM,          BleWifi_Ble_CmMsgHandler_SetDisconnectCfm},
    {LE_CM_MSG_DISCONNECT_COMPLETE_IND,     BleWifi_Ble_CmMsgHandler_DisconnectCompleteInd},
    
    {0xFFFFFFFF,                            NULL}
};

static void BleWifi_Ble_AppMsgHandler_Initializing(TASK task, MESSAGEID id, MESSAGE message);
static void BleWifi_Ble_AppMsgHandler_EnterAdvertising(TASK task, MESSAGEID id, MESSAGE message);
static void BleWifi_Ble_AppMsgHandler_ExitAdvertising(TASK task, MESSAGEID id, MESSAGE message);
static void BleWifi_Ble_AppMsgHandler_ChangeAdvertisingTime(TASK task, MESSAGEID id, MESSAGE message);
static void BleWifi_Ble_AppMsgHandler_SendData(TASK task, MESSAGEID id, MESSAGE message);
static void BleWifi_Ble_AppMsgHandler_SendToPeer(TASK task, MESSAGEID id, MESSAGE message);
static void BleWifi_Ble_AppMsgHandler_SendToPeerCfm(TASK task, MESSAGEID id, MESSAGE message);
static T_BleWifi_Ble_MsgHandlerTbl gBleAppMsgHandlerTbl[] =
{
    {BLEWIFI_APP_MSG_INITIALIZING,              BleWifi_Ble_AppMsgHandler_Initializing},
    {BLEWIFI_APP_MSG_ENTER_ADVERTISING,         BleWifi_Ble_AppMsgHandler_EnterAdvertising},
    {BLEWIFI_APP_MSG_EXIT_ADVERTISING,          BleWifi_Ble_AppMsgHandler_ExitAdvertising},
    {BLEWIFI_APP_MSG_CHANGE_ADVERTISING_TIME,   BleWifi_Ble_AppMsgHandler_ChangeAdvertisingTime},
    {BLEWIFI_APP_MSG_SEND_DATA,                 BleWifi_Ble_AppMsgHandler_SendData},
    {BLEWIFI_APP_MSG_SEND_TO_PEER,              BleWifi_Ble_AppMsgHandler_SendToPeer},
    {BLEWIFI_APP_MSG_SEND_TO_PEER_CFM,          BleWifi_Ble_AppMsgHandler_SendToPeerCfm},
    
    {0xFFFFFFFF,                                NULL}
};

static void BleWifi_Ble_SmMsgHandler_PairingActionInd(TASK task, MESSAGEID id, MESSAGE message)
{
    LE_SMP_MSG_PAIRING_ACTION_IND_T *ind = (LE_SMP_MSG_PAIRING_ACTION_IND_T *)message;
    BLEWIFI_INFO("APP-LE_SMP_MSG_PAIRING_ACTION_IND hdl = %x sc = %d action = %d\r\n", ind->conn_hdl, ind->sc, ind->action);
    
    LeSmpSecurityRsp(ind->conn_hdl, TRUE);
}

static void BleWifi_Ble_SmMsgHandler_EncryptionChangeInd(TASK task, MESSAGEID id, MESSAGE message)
{
    LE_SMP_MSG_ENCRYPTION_CHANGE_IND_T *ind = (LE_SMP_MSG_ENCRYPTION_CHANGE_IND_T *)message;
    
    BLEWIFI_INFO("APP-LE_SMP_MSG_ENCRYPTION_CHANGE_IND enable = %d\r\n", ind->enable);
    gTheBle.encrypted = ind->enable;
}

static void BleWifi_Ble_SmMsgHandler_EncryptionRefreshInd(TASK task, MESSAGEID id, MESSAGE message)
{
#ifdef BLEWIFI_SHOW_INFO
    LE_SMP_MSG_ENCRYPTION_REFRESH_IND_T *ind = (LE_SMP_MSG_ENCRYPTION_REFRESH_IND_T *)message;
    BLEWIFI_INFO("APP-LE_SMP_MSG_ENCRYPTION_REFRESH_IND status = %x\r\n", ind->status);
#endif
}

static void BleWifi_Ble_SmMsgHandler_PairingCompleteInd(TASK task, MESSAGEID id, MESSAGE message)
{
    LE_SMP_MSG_PAIRING_COMPLETE_IND_T *ind = (LE_SMP_MSG_PAIRING_COMPLETE_IND_T *)message;
    
    BLEWIFI_INFO("APP-LE_SMP_MSG_PAIRING_COMPLETE_IND status = %x\r\n", ind->status);
    
    if (ind->status == SYS_ERR_SUCCESS)
    {
        gTheBle.paired = TRUE;
    }
}

static void BleWifi_Ble_SetAdvtisingPara(UINT8 type, UINT8 own_addr_type, LE_BT_ADDR_T *peer_addr, UINT8 filter, UINT16 interval_min, UINT16 interval_max)
{
	LE_GAP_ADVERTISING_PARAM_T para;

	para.interval_min = interval_min;
	para.interval_max = interval_max;
	para.type = type;
	para.own_addr_type = own_addr_type;

	if (peer_addr)
    {
	    para.peer_addr_type = peer_addr->type;
        MemCopy(para.peer_addr, peer_addr->addr, 6);
    }
    else
    {
	    para.peer_addr_type = LE_HCI_ADV_PEER_ADDR_PUBLIC;
		MemSet(para.peer_addr, 0, 6);
    }

	para.channel_map = 0x7;
    para.filter_policy = filter;

	LeGapSetAdvParameter(&para);
}

static void BleWifi_UtilHexToStr(void *data, UINT16 len, UINT8 **p)
{
	UINT8 t[] = "0123456789ABCDEF";
	UINT8 *num = data;
	UINT8 *buf = *p;
    UINT16 i = 0;
    
	while (len--)
	{
		buf[i << 1] = t[num[i] >> 4];
		buf[(i << 1) + 1] = t[num[i] & 0xf];
		i++;
    }

    *p += (i << 1);
}

static void BleWifi_Ble_SetAdvData(void)
{
    uint8_t ubLen;
	UINT8 bleAdvertData[] =
	{
        0x02,
        GAP_ADTYPE_FLAGS,
        GAP_ADTYPE_FLAGS_GENERAL | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,
        // connection interval range
        0x05,
        GAP_ADTYPE_SLAVE_CONN_INTERVAL_RANGE,
        UINT16_LO(DEFAULT_DESIRED_MIN_CONN_INTERVAL),
        UINT16_HI(DEFAULT_DESIRED_MIN_CONN_INTERVAL),
        UINT16_LO(DEFAULT_DESIRED_MAX_CONN_INTERVAL),
        UINT16_HI(DEFAULT_DESIRED_MAX_CONN_INTERVAL),
        0x02,
        GAP_ADTYPE_POWER_LEVEL,
        0,
        0x11,
        GAP_ADTYPE_128BIT_COMPLETE,
        0xFB, 0x34, 0x9B, 0x5F, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0xAA, 0xAA, 0x00, 0x00
	};

    // error handle
	ubLen = sizeof(bleAdvertData);
	if (ubLen > BLE_ADV_SCAN_BUF_SIZE)
	    ubLen = BLE_ADV_SCAN_BUF_SIZE;
	gTheBle.adv_data.len = ubLen;
	MemCopy(gTheBle.adv_data.buf, bleAdvertData, gTheBle.adv_data.len);
    LeGapSetAdvData(gTheBle.adv_data.len, gTheBle.adv_data.buf);
}

static void BleWifi_Ble_SetScanData(void)
{
    T_MwFim_GP08_BleDeviceName tBleDeviceName;
    uint8_t ubLen;
    BOOL isOk = FALSE;

    // get the settings of BLE device name
	if (MW_FIM_OK != MwFim_FileRead(MW_FIM_IDX_GP08_PROJECT_BLE_DEVICE_NAME, 0, MW_FIM_GP08_BLE_DEVICE_NAME_SIZE, (uint8_t*)&tBleDeviceName))
    {
        // if fail, get the default value
        memcpy(&tBleDeviceName, &g_tMwFimDefaultGp08BleDeviceName, MW_FIM_GP08_BLE_DEVICE_NAME_SIZE);
    }
    
    if (tBleDeviceName.ubNameMethod == 1)
    {
    	BD_ADDR addr;

    	if (LeGapGetBdAddr(addr) == SYS_ERR_SUCCESS)
    	{
    		UINT8 *p = gTheBle.scn_data.buf;
    		UINT16 i = tBleDeviceName.ubNamePostfixMacCount;

    		// error handle, the mac address length
    		if (i > 6)
    		    i = 6;
            
    		*p++ = 0x10;
    		*p++ = GAP_ADTYPE_LOCAL_NAME_COMPLETE;
            // error handle
            // !!! if i = 4, the other char are 12 bytes (i*3)
            ubLen = strlen((const char *)(tBleDeviceName.ubaNamePrefix));
    		if (ubLen > (BLE_ADV_SCAN_BUF_SIZE - 2 - (i*3)))
    	        ubLen = BLE_ADV_SCAN_BUF_SIZE - 2 - (i*3);
    		MemCopy(p, tBleDeviceName.ubaNamePrefix, ubLen);
    		p += ubLen;

            if (i > 0)
            {
        		while (i--)
        		{
                    BleWifi_UtilHexToStr(&addr[i], 1, &p);
        			*p++ = ':';
                }

                gTheBle.scn_data.len = p - gTheBle.scn_data.buf - 1;    // remove the last char ":"
            }
            else
            {
                gTheBle.scn_data.len = p - gTheBle.scn_data.buf;
            }

            gTheBle.scn_data.buf[0] = gTheBle.scn_data.len - 1;     // update the total length

            isOk = TRUE;
        }
    }
    else if (tBleDeviceName.ubNameMethod == 2)
    {
        // error handle
        ubLen = strlen((const char *)(tBleDeviceName.ubaNameFull));
        if (ubLen > (BLE_ADV_SCAN_BUF_SIZE - 2))
            ubLen = (BLE_ADV_SCAN_BUF_SIZE - 2);
    	gTheBle.scn_data.len = ubLen + 2;
        gTheBle.scn_data.buf[0] = gTheBle.scn_data.len - 1;
        gTheBle.scn_data.buf[1] = GAP_ADTYPE_LOCAL_NAME_COMPLETE;
        MemCopy(gTheBle.scn_data.buf + 2, tBleDeviceName.ubaNameFull, ubLen);

        isOk = TRUE;
    }

    // error handle to give the default value
    if (isOk != TRUE)
    {
        // error handle
        ubLen = strlen("OPL_Device");
        if (ubLen > (BLE_ADV_SCAN_BUF_SIZE - 2))
            ubLen = (BLE_ADV_SCAN_BUF_SIZE - 2);
        gTheBle.scn_data.len = ubLen + 2;
        gTheBle.scn_data.buf[0] = gTheBle.scn_data.len - 1;
        gTheBle.scn_data.buf[1] = GAP_ADTYPE_LOCAL_NAME_COMPLETE;
        MemCopy(gTheBle.scn_data.buf + 2, "OPL_Device", ubLen);
    }

    LeSetScanRspData(gTheBle.scn_data.len, gTheBle.scn_data.buf);
    BleWifi_Ble_AppUpdateDevName(gTheBle.scn_data.buf + 2, gTheBle.scn_data.len - 2);
}

static void BleWifi_Ble_CmMsgHandler_InitCompleteCfm(TASK task, MESSAGEID id, MESSAGE message)
{
    T_MwFim_GP08_BleAdvInterval tBleAdvInterval;

    // !!! after LeCmInit
    BLEWIFI_INFO("APP-LE_CM_MSG_INIT_COMPLETE_CFM\r\n");

    // get the settings of BLE advertisement interval
	if (MW_FIM_OK != MwFim_FileRead(MW_FIM_IDX_GP08_PROJECT_BLE_ADV_INTERVAL, 0, MW_FIM_GP08_BLE_ADV_INTERVAL_SIZE, (uint8_t*)&tBleAdvInterval))
    {
        // if fail, get the default value
        memcpy(&tBleAdvInterval, &g_tMwFimDefaultGp08BleAdvInterval, MW_FIM_GP08_BLE_ADV_INTERVAL_SIZE);
    }
    
    LeGattInit(&gTheBle.task);
    LeSmpInit(&gTheBle.task);
    LeSmpSetDefaultConfig(LE_SM_IO_CAP_NO_IO, FALSE, FALSE, TRUE);
    
    BleWifi_Ble_SetAdvData();
    BleWifi_Ble_SetScanData();
    BleWifi_Ble_SetAdvtisingPara(LE_HCI_ADV_TYPE_ADV_IND,
                                 LE_HCI_OWN_ADDR_PUBLIC,
                                 0,
                                 LE_HCI_ADV_FILT_NONE,
                                 tBleAdvInterval.uwIntervalMin,
                                 tBleAdvInterval.uwIntervalMax);
}

static void BleWifi_Ble_CmMsgHandler_SetAdvertisingDataCfm(TASK task, MESSAGEID id, MESSAGE message)
{
    BLEWIFI_INFO("APP-LE_CM_MSG_SET_ADVERTISING_DATA_CFM - Status = %x\r\n", ((LE_CM_MSG_SET_ADVERTISING_DATA_CFM_T *)message)->status);
}

static void BleWifi_Ble_CmMsgHandler_SetScanRspDataCfm(TASK task, MESSAGEID id, MESSAGE message)
{
    BLEWIFI_INFO("APP-LE_CM_MSG_SET_SCAN_RSP_DATA_CFM - Status = %x\r\n", ((LE_CM_MSG_SET_SCAN_RSP_DATA_CFM_T *)message)->status);
}

static void BleWifi_Ble_CmMsgHandler_SetAdvertisingParamsCfm(TASK task, MESSAGEID id, MESSAGE message)
{
    BLEWIFI_INFO("APP-LE_CM_MSG_SET_ADVERTISING_PARAMS_CFM - Status = %x\r\n", ((LE_CM_MSG_SET_ADVERTISING_PARAMS_CFM_T *)message)->status);
    
    // !!! Init complete
    if (gTheBle.state == APP_STATE_INIT)
    {
        // send the message to the application task (ctrl task)
        gTheBle.state = APP_STATE_IDLE;
        BleWifi_Ctrl_MsgSend(BLEWIFI_CTRL_MSG_BLE_INIT_COMPLETE, NULL, 0);
    }
    // !!! Change the time of advertising
    else if (gTheBle.state == APP_STATE_ADVERTISING_TIME_CHANGE)
    {
        BleWifi_Ble_SendAppMsgToBle(BLEWIFI_APP_MSG_ENTER_ADVERTISING, 0, NULL);
    }
}

static void BleWifi_Ble_CmMsgHandler_EnterAdvertisingCfm(TASK task, MESSAGEID id, MESSAGE message)
{
    LE_CM_MSG_ENTER_ADVERTISING_CFM_T *cfm = (LE_CM_MSG_ENTER_ADVERTISING_CFM_T *)message;
    BLEWIFI_INFO("APP-LE_CM_MSG_ENTER_ADVERTISING_CFM Status = %x\r\n", cfm->status);
    
    if (cfm->status == SYS_ERR_SUCCESS)
    {
        // !!! Change the time of advertising
        if (gTheBle.state == APP_STATE_ADVERTISING_TIME_CHANGE)
        {
            gTheBle.state = APP_STATE_ADVERTISING;
            BleWifi_Ctrl_MsgSend(BLEWIFI_CTRL_MSG_BLE_ADVERTISING_TIME_CHANGE_CFM, NULL, 0);
        }
        else
        {
            gTheBle.state = APP_STATE_ADVERTISING;
            BleWifi_Ctrl_MsgSend(BLEWIFI_CTRL_MSG_BLE_ADVERTISING_CFM, NULL, 0);
        }
    }
}

static void BleWifi_Ble_CmMsgHandler_ExitAdvertisingCfm(TASK task, MESSAGEID id, MESSAGE message)
{
    LE_CM_MSG_EXIT_ADVERTISING_CFM_T *cfm = (LE_CM_MSG_EXIT_ADVERTISING_CFM_T *)message;
    BLEWIFI_INFO("APP-LE_CM_MSG_EXIT_ADVERTISING_CFM Status = %x\r\n", cfm->status);
    
    if (cfm->status == SYS_ERR_SUCCESS)
    {
        // !!! Change the time of advertising
        if (gTheBle.state == APP_STATE_ADVERTISING_TIME_CHANGE)
        {
            BleWifi_Ble_SetAdvtisingPara(LE_HCI_ADV_TYPE_ADV_IND,
                                         LE_HCI_OWN_ADDR_PUBLIC,
                                         0,
                                         LE_HCI_ADV_FILT_NONE,
                                         gTheBleAdvTime.interval_min,
                                         gTheBleAdvTime.interval_max);
        }
        else
        {
            gTheBle.state = APP_STATE_IDLE;
            BleWifi_Ctrl_MsgSend(BLEWIFI_CTRL_MSG_BLE_ADVERTISING_EXIT_CFM, NULL, 0);
        }
    }
}

static void BleWifi_Ble_CmMsgHandler_ConnectionCompleteInd(TASK task, MESSAGEID id, MESSAGE message)
{
    LE_CM_CONNECTION_COMPLETE_IND_T *ind = (LE_CM_CONNECTION_COMPLETE_IND_T *)message;
    BLEWIFI_INFO("APP-LE_CM_CONNECTION_COMPLETE_IND status = %x\r\n", ind->status);
    
    if (ind->status == SYS_ERR_SUCCESS)
    {
        gTheBle.state = APP_STATE_CONNECTED;
    
        gTheBle.conn_hdl = ind->conn_hdl;
        gTheBle.bt_addr.type = ind->peer_addr_type;
        MemCopy(gTheBle.bt_addr.addr, ind->peer_addr, 6);
    
        gTheBle.max_itvl = ind->conn_interval;
        gTheBle.latency = ind->conn_latency;
        gTheBle.sv_tmo = ind->supervison_timeout;
    
        BleWifi_Ble_GattIndicateServiceChange(ind->conn_hdl);
        BleWifi_Ctrl_MsgSend(BLEWIFI_CTRL_MSG_BLE_CONNECTION_COMPLETE, NULL, 0);
    }
    else
    {
        gTheBle.state = APP_STATE_IDLE;
        BleWifi_Ctrl_MsgSend(BLEWIFI_CTRL_MSG_BLE_CONNECTION_FAIL, NULL, 0);
    }
}

static void BleWifi_Ble_CmMsgHandler_SignalUpdateReq(TASK task, MESSAGEID id, MESSAGE message)
{
    LE_CM_MSG_SIGNAL_UPDATE_REQ_T *req = (LE_CM_MSG_SIGNAL_UPDATE_REQ_T *)message;
    BLEWIFI_INFO("APP-LE_CM_MSG_SIGNAL_UPDATE_REQ identifier = %d\r\n", req->identifier);
    BLEWIFI_INFO("    min = %x max = %x latency = %x timeout = %x\r\n", req->interval_min, req->interval_max, req->slave_latency, req->timeout_multiplier);
    LeGapConnUpdateResponse(req->conn_hdl, req->identifier, TRUE);
}

static void BleWifi_Ble_CmMsgHandler_ConnParaReq(TASK task, MESSAGEID id, MESSAGE message)
{
    LE_CM_MSG_CONN_PARA_REQ_T *req = (LE_CM_MSG_CONN_PARA_REQ_T *)message;
    BLEWIFI_INFO("APP-LE_CM_MSG_CONN_PARA_REQ min = %x max = %x latency = %x timeout = %x\r\n", req->itv_min, req->itv_max, req->latency, req->sv_tmo);
    LeGapConnParaRequestRsp(req->conn_hdl, TRUE);
}

static void BleWifi_Ble_CmMsgHandler_ConnUpdateCompleteInd(TASK task, MESSAGEID id, MESSAGE message)
{
    LE_CM_MSG_CONN_UPDATE_COMPLETE_IND_T *ind = (LE_CM_MSG_CONN_UPDATE_COMPLETE_IND_T *)message;
    BLEWIFI_INFO("APP-LE_CM_MSG_CONN_UPDATE_COMPLETE_IND status = %x, itv = %x, latency = %x svt = %x\r\n", ind->status, ind->interval, ind->latency, ind->supervision_timeout);
    
    if (ind->status == SYS_ERR_SUCCESS)
    {
        gTheBle.max_itvl = ind->interval;
        gTheBle.latency = ind->latency;
        gTheBle.sv_tmo = ind->supervision_timeout;
    }
    else
    {
        LeGapDisconnectReq(ind->conn_hdl);
    }
}

static void BleWifi_Ble_CmMsgHandler_SetDisconnectCfm(TASK task, MESSAGEID id, MESSAGE message)
{
    // !!! after LeGapDisconnectReq
    // if the disconnection is finished, "LE_CM_MSG_DISCONNECT_COMPLETE_IND" will be executed
#ifdef BLEWIFI_SHOW_INFO
    LE_CM_MSG_SET_DISCONNECT_CFM_T *cfm = (LE_CM_MSG_SET_DISCONNECT_CFM_T *)message;
    BLEWIFI_INFO("APP-LE_CM_MSG_SET_DISCONNECT_CFM conn_hdl = %x status = %x\r\n", cfm->handle, cfm->status);
#endif
}

static void BleWifi_Ble_CmMsgHandler_DisconnectCompleteInd(TASK task, MESSAGEID id, MESSAGE message)
{
    // !!! [Device] after LeGapDisconnectReq
    // !!! [Peer] request the disconnection
    BLEWIFI_DATA_OUT_STORE_T *s = &gTheBle.store;
#ifdef BLEWIFI_SHOW_INFO
    LE_CM_MSG_DISCONNECT_COMPLETE_IND_T *ind = (LE_CM_MSG_DISCONNECT_COMPLETE_IND_T *)message;
    BLEWIFI_INFO("APP-LE_CM_MSG_DISCONNECT_COMPLETE_IND conn_hdl = %x status = %x reason = %x \r\n", ind->conn_hdl, ind->status, ind->reason);
#endif

    gTheBle.state = APP_STATE_IDLE;
    s->pidx = s->ridx;
    s->sending = 0;
    BleWifi_Ctrl_MsgSend(BLEWIFI_CTRL_MSG_BLE_DISCONNECT, NULL, 0);
}

static void BleWifi_Ble_CopyToBuf(UINT16 len, UINT8 *data)
{
	if (gTheBle.state != APP_STATE_CONNECTED) return;

	if (len && data)
	{
		BLEWIFI_DATA_OUT_STORE_T *s = &gTheBle.store;
		UINT16 ridx = s->ridx;
        UINT16 remain = 0;
		UINT16 copyLen;

		if (len >= LE_GATT_DATA_OUT_BUF_SIZE) return;

		if ((ridx + len) > LE_GATT_DATA_OUT_BUF_SIZE) remain = ridx + len - LE_GATT_DATA_OUT_BUF_SIZE;

        copyLen = len - remain;

		MemCopy(&s->buf[ridx], data, copyLen);

		if (remain) MemCopy(s->buf, &data[copyLen], remain);

		ridx = (ridx + len) & (LE_GATT_DATA_OUT_BUF_SIZE - 1);

		s->ridx = ridx;

        BleWifi_Ble_SendAppMsgToBle(BLEWIFI_APP_MSG_SEND_TO_PEER, 0, NULL);
    }
}

static void BleWifi_Ble_SendToPeer(void)
{
	BLEWIFI_DATA_OUT_STORE_T *s = &gTheBle.store;

	if (s->sending < 4)
	{
		LE_ERR_STATE status;
		UINT16 ridx = s->ridx;
		UINT16 pidx = s->pidx;
		UINT16 sendLen;

		while (pidx != ridx)
		{
			if (pidx > ridx)
				sendLen = LE_GATT_DATA_OUT_BUF_SIZE - pidx;
			else
				sendLen = ridx - pidx;

			if (sendLen > (gTheBle.curr_mtu - 3)) sendLen = gTheBle.curr_mtu - 3;

		    status = LeGattCharValNotify(gTheBle.conn_hdl, s->send_hdl, sendLen, s->buf + pidx);

	    	BLEWIFI_INFO("APP-BleWifi_Ble_SendToPeer pidx = %d ridx = %d status = %x sending = %d\r\n", pidx, ridx, status, s->sending);

			if (status != SYS_ERR_SUCCESS)
			{
				s->pidx = s->ridx;
                s->sending = 0;
                return;
		    }
			else
			{
				pidx = (pidx + sendLen) & (LE_GATT_DATA_OUT_BUF_SIZE - 1);
				s->sending++;
            }

            if (s->sending == 4) break;
	    }

		s->pidx = pidx;
    }
}

static void BleWifi_Ble_AppMsgHandler_Initializing(TASK task, MESSAGEID id, MESSAGE message)
{
    /* BLE Init Step 1: Do BLE initialization first */
    LeCmInit(&gTheBle.task);
}

static void BleWifi_Ble_AppMsgHandler_EnterAdvertising(TASK task, MESSAGEID id, MESSAGE message)
{
    LE_ERR_STATE rc = LeGapAdvertisingEnable(TRUE);
    
    if (rc != SYS_ERR_SUCCESS)
    {
        BLEWIFI_INFO("APP-BLEWIFI_APP_MSG_ENTER_ADVERTISING fail rc = %x\r\n", rc);
    }
}

static void BleWifi_Ble_AppMsgHandler_ExitAdvertising(TASK task, MESSAGEID id, MESSAGE message)
{
    LE_ERR_STATE rc = LeGapAdvertisingEnable(FALSE);
    
    if (rc != SYS_ERR_SUCCESS)
    {
        BLEWIFI_INFO("APP-BLEWIFI_APP_MSG_EXIT_ADVERTISING fail rc = %x\r\n", rc);
    }
}

static void BleWifi_Ble_AppMsgHandler_ChangeAdvertisingTime(TASK task, MESSAGEID id, MESSAGE message)
{
    BLEWIFI_MESSAGE_T *adv_time = (BLEWIFI_MESSAGE_T *)message;
    
    memcpy(&gTheBleAdvTime, adv_time->data, sizeof(BLE_ADV_TIME_T));
    
    if (gTheBle.state == APP_STATE_ADVERTISING)
    {
        gTheBle.state = APP_STATE_ADVERTISING_TIME_CHANGE;
        BleWifi_Ble_SendAppMsgToBle(BLEWIFI_APP_MSG_EXIT_ADVERTISING, 0, NULL);
    }
    else
    {
        BleWifi_Ble_SetAdvtisingPara(LE_HCI_ADV_TYPE_ADV_IND,
                                     LE_HCI_OWN_ADDR_PUBLIC,
                                     0,
                                     LE_HCI_ADV_FILT_NONE,
                                     gTheBleAdvTime.interval_min,
                                     gTheBleAdvTime.interval_max);
    }
}

static void BleWifi_Ble_AppMsgHandler_SendData(TASK task, MESSAGEID id, MESSAGE message)
{
    if (gTheBle.state == APP_STATE_CONNECTED)
    {
        BLEWIFI_MESSAGE_T *wifi_data = (BLEWIFI_MESSAGE_T *)message;
    
        // copy data to buffer
        BleWifi_Ble_CopyToBuf(wifi_data->len, wifi_data->data);
    }
}

static void BleWifi_Ble_AppMsgHandler_SendToPeer(TASK task, MESSAGEID id, MESSAGE message)
{
    if (gTheBle.state == APP_STATE_CONNECTED)
    {
        // send data from buffer to peer
        BleWifi_Ble_SendToPeer();
    }
}

static void BleWifi_Ble_AppMsgHandler_SendToPeerCfm(TASK task, MESSAGEID id, MESSAGE message)
{
    if (gTheBle.state == APP_STATE_CONNECTED)
    {
        BLEWIFI_DATA_OUT_STORE_T *s = &gTheBle.store;
    
        if (s->sending) 
            s->sending--;
    
        // trigger the next "send data from buffer to peer"
        BleWifi_Ble_SendAppMsgToBle(BLEWIFI_APP_MSG_SEND_TO_PEER, 0, NULL);
    }
}

static void BleWifi_Ble_MsgHandler(T_BleWifi_Ble_MsgHandlerTbl tHanderTbl[], TASK task, MESSAGEID id, MESSAGE message)
{
    uint32_t i = 0;

    while (tHanderTbl[i].ulEventId != 0xFFFFFFFF)
    {
        // match
        if (tHanderTbl[i].ulEventId == id)
        {
            tHanderTbl[i].fpFunc(task, id, message);
            break;
        }

        i++;
    }

    // not match
    if (tHanderTbl[i].ulEventId == 0xFFFFFFFF)
    {
    }
}

static void BleWifi_Ble_TaskHandler(TASK task, MESSAGEID id, MESSAGE message)
{
	if ((id >= LE_GATT_MSG_BASE) && (id < LE_GATT_MSG_TOP))
	{
		BleWifi_Ble_MsgHandler(gBleGattMsgHandlerTbl, task, id, message);
    }
    else if ((id >= LE_SMP_MSG_BASE) && (id < LE_SMP_MSG_TOP))
	{
        BleWifi_Ble_MsgHandler(gBleSmMsgHandlerTbl, task, id, message);
    }
	else if ((id >= LE_CM_MSG_BASE) && (id < LE_CM_MSG_TOP))
	{
        BleWifi_Ble_MsgHandler(gBleCmMsgHandlerTbl, task, id, message);
    }
    else if ((id >= BLEWIFI_APP_MSG_BASE) && (id < BLEWIFI_APP_MSG_TOP))
    {
        BleWifi_Ble_MsgHandler(gBleAppMsgHandlerTbl, task, id, message);
    }
}

BLE_APP_DATA_T* BleWifi_Ble_GetEntity(void)
{
	return &gTheBle;
}

UINT16 BleWifi_Ble_GetBufFreeSize(void)
{
	UINT16 freeSize;
	BLEWIFI_DATA_OUT_STORE_T *s = &gTheBle.store;
	UINT16 ridx = s->ridx;
	UINT16 pidx = s->pidx;

	if (pidx == ridx)
		freeSize = LE_GATT_DATA_OUT_BUF_SIZE - 1;
	else if (pidx > ridx)
		freeSize = pidx - ridx - 1;
	else
		freeSize = (LE_GATT_DATA_OUT_BUF_SIZE - 1) - ridx + pidx;

	return freeSize;
}

void BleWifi_Ble_SendAppMsgToBle(UINT32 id, UINT16 len, void *data)
{
	if ((id >= BLEWIFI_APP_MSG_BASE) && (id < BLEWIFI_APP_MSG_TOP))
	{
		void *p = 0;

		if (len)
        {
			MESSAGE_DATA_BULID(BLEWIFI_MESSAGE, len);

			msg->len = len;
			msg->data = MESSAGE_OFFSET(BLEWIFI_MESSAGE);
			MemCopy(msg->data, data, len);
			p = msg;
        }

	    LeSendMessage(&gTheBle.task, id, p);
    }
}

void BleWifi_Ble_ServerAppInit(void)
{
    BLEWIFI_INFO("APP-BleWifi_Ble_ServerAppInit\r\n");
    
	MemSet(&gTheBle, 0, sizeof(gTheBle));
	MemSet(&gTheBleAdvTime, 0, sizeof(gTheBleAdvTime));

	gTheBle.state = APP_STATE_INIT;
	gTheBle.curr_mtu = 23;

	gTheBle.min_itvl = DEFAULT_DESIRED_MIN_CONN_INTERVAL;
	gTheBle.max_itvl = DEFAULT_DESIRED_MAX_CONN_INTERVAL;
	gTheBle.latency  = DEFAULT_DESIRED_SLAVE_LATENCY;
	gTheBle.sv_tmo   = DEFAULT_DESIRED_SUPERVERSION_TIMEOUT;

    LeHostCreateTask(&gTheBle.task, BleWifi_Ble_TaskHandler);
	BleWifi_Ble_SendAppMsgToBle(BLEWIFI_APP_MSG_INITIALIZING, 0, NULL);
}
