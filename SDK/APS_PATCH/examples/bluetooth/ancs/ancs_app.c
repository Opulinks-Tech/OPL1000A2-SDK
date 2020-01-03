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

#include "ble_hci_if.h"
#include "ble_cm_if.h"
#include "ble_smp_if.h"
#include "ble_gap_if.h"
#include "ble_gatt_if.h"
#include "ble_util.h"
#include "ancs_app.h"
#include "ancs_app_gatt.h"
#include "ancs_app_client.h"


static BLE_APP_DATA_T gTheBle;

static UINT8 gBleAdvertData[] = 
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
    0x9,
    GAP_ADTYPE_LOCAL_NAME_COMPLETE,   
    'A', 'N', 'C', 'S',' ', 'A', 'P', 'P'
};

static UINT8 gBleScanRspData[] =
{
    0x9,
    GAP_ADTYPE_LOCAL_NAME_COMPLETE,   
    'A', 'N', 'C', 'S',' ', 'A', 'P', 'P'
};

static void BleAppSetAdvtisingPara(UINT8 type, UINT8 own_addr_type, LE_BT_ADDR_T *peer_addr, UINT8 filter)
{
    LE_GAP_ADVERTISING_PARAM_T para;

    para.interval_min = 0x100;
    para.interval_max = 0x100;
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

static void BleAppSmMsgHandler(TASK task, MESSAGEID id, MESSAGE message)
{
    switch (id)
    {
        case LE_SMP_MSG_PAIRING_ACTION_IND:
        {
            LE_SMP_MSG_PAIRING_ACTION_IND_T *ind = (LE_SMP_MSG_PAIRING_ACTION_IND_T *)message;
            BLE_APP_PEER_T *peer = BleAppFindPeer(ind->conn_hdl);
            
            BLE_APP_PRINT("LE_SMP_MSG_PAIRING_ACTION_IND hdl = %x sc = %d action = %d\r\n", ind->conn_hdl, ind->sc, ind->action);
            
            if (peer) LeSmpSecurityRsp(ind->conn_hdl, TRUE);
        }
        break;

        case LE_SMP_MSG_ENCRYPTION_CHANGE_IND:
        {
            LE_SMP_MSG_ENCRYPTION_CHANGE_IND_T *ind = (LE_SMP_MSG_ENCRYPTION_CHANGE_IND_T *)message;
            BLE_APP_PEER_T *peer = BleAppFindPeer(ind->conn_hdl);
            
            BLE_APP_PRINT("LE_SMP_MSG_ENCRYPTION_CHANGE_IND enable = %d\r\n", ind->enable);

            if (peer) peer->encrypted = ind->enable;
        }
        break;
        
        case LE_SMP_MSG_ENCRYPTION_REFRESH_IND:
        {
            LE_SMP_MSG_ENCRYPTION_REFRESH_IND_T *ind = (LE_SMP_MSG_ENCRYPTION_REFRESH_IND_T *)message;
            BLE_APP_PEER_T *peer = BleAppFindPeer(ind->conn_hdl);
            
            BLE_APP_PRINT("LE_SMP_MSG_ENCRYPTION_REFRESH_IND status = %x\r\n", ind->status);

            if (!peer) return;

            if (ind->status == SYS_ERR_SUCCESS)
            {
                peer->encrypted = TRUE;

                BleAppSendCommonMsg(BLE_APP_MSG_FIND_ANCS_SERVICE, ind->conn_hdl, 1000);
            }
            else
            {
                LeGapDisconnectReq(ind->conn_hdl);
            }
        }
        break;
        
        case LE_SMP_MSG_PAIRING_COMPLETE_IND:
        {
            LE_SMP_MSG_PAIRING_COMPLETE_IND_T *ind = (LE_SMP_MSG_PAIRING_COMPLETE_IND_T *)message;
            BLE_APP_PEER_T *peer = BleAppFindPeer(ind->conn_hdl);

            BLE_APP_PRINT("LE_SMP_MSG_PAIRING_COMPLETE_IND status = %x\r\n", ind->status);

            if (!peer) return;

            if (ind->status == SYS_ERR_SUCCESS)
            {
                peer->paired = TRUE;

                BleAppSendCommonMsg(BLE_APP_MSG_FIND_ANCS_SERVICE, ind->conn_hdl, 100);
            }
            else
            {
                LeGapDisconnectReq(ind->conn_hdl);
            }
        }
        break;

        default:
        break;
    }
}

static void BleAppGattPeerInfoClean(UINT16 conn_hdl)
{
    BLE_APP_DATA_T *app = BleAppGetEntity();
    BLE_APP_PEER_T *peer = BleAppFindPeer(conn_hdl);
    UINT16 i;

    if (!peer) return;

    for (i = 0; i < peer->svc_idx; i++)
    {
        if (peer->svc[i].attr) FreeMem(peer->svc[i].attr);
    }

    if (LeQueueCount(&peer->attrq)) LeQueueBroken(&peer->attrq);
    
    if (LeQueueCount(&peer->rw_req)) LeQueueBroken(&peer->rw_req);

    if (peer->rw_curr) FreeMem(peer->rw_curr);

    if (peer->ancs) BleAppAncsInfoClean(conn_hdl);

    LeQueueDelete(&app->peer, peer);
}

static void* BleAppAddPeer(UINT16 conn_hdl)
{
    BLE_APP_DATA_T *app = BleAppGetEntity();
    UINT16 count = LeQueueCount(&app->peer);
    BLE_APP_PEER_T *peer;

    if (count >= LE_MAX_CONNECTION_COUNT) return 0;

    peer = Malloc(sizeof(BLE_APP_PEER_T));

    if (peer)
    {
        MemSet(peer, 0, sizeof(BLE_APP_PEER_T));

        peer->conn_hdl = conn_hdl;

        LeQueuePush(&app->peer, peer);

        return peer;
    }

    return 0;
}

static void AncsAppDebugConfig(void)
{
    tracer_log_mode_set(2);
    tracer_log_level_set(255, 7);
}

static void BleCmMsgHandler(TASK task, MESSAGEID id, MESSAGE message)
{
    BLE_APP_DATA_T *app = BleAppGetEntity();

    switch (id)
    {
        case LE_CM_MSG_INIT_COMPLETE_CFM:
        {
            AncsAppDebugConfig();
            
            BLE_APP_PRINT("LE_CM_MSG_INIT_COMPLETE_CFM\r\n");
            LeGattInit(&app->task);
            LeSmpInit(&app->task);
            LeGapSetAdvData(sizeof(gBleAdvertData), gBleAdvertData);
            LeSetScanRspData(sizeof(gBleScanRspData), gBleScanRspData);
            BleAppSetAdvtisingPara(LE_HCI_ADV_TYPE_ADV_IND, LE_HCI_OWN_ADDR_PUBLIC, 0, LE_HCI_ADV_FILT_NONE);
        }
        break;
        
        case LE_CM_MSG_SET_ADVERTISING_DATA_CFM:
        {
            BLE_APP_PRINT("LE_CM_MSG_SET_ADVERTISING_DATA_CFM - Status = %x\r\n", ((LE_CM_MSG_SET_ADVERTISING_DATA_CFM_T *)message)->status);
        }
        break;

        case LE_CM_MSG_SET_SCAN_RSP_DATA_CFM:
        {
            BLE_APP_PRINT("LE_CM_MSG_SET_SCAN_RSP_DATA_CFM - Status = %x\r\n", ((LE_CM_MSG_SET_SCAN_RSP_DATA_CFM_T *)message)->status);
            LeSmpSetDefaultConfig(LE_SM_IO_CAP_NO_IO, FALSE, FALSE, TRUE);
        }
        break;

        case LE_CM_MSG_SET_ADVERTISING_PARAMS_CFM:
        {
            BLE_APP_PRINT("LE_CM_MSG_SET_ADVERTISING_PARAMS_CFM - Status = %x\r\n", ((LE_CM_MSG_SET_ADVERTISING_PARAMS_CFM_T *)message)->status);

            app->state = APP_STATE_IDLE;
            LeSendMessageAfter(&app->task, BLE_APP_MSG_ENTER_ADVERTISING, 0, 500);
        }
        break;

        case LE_CM_MSG_ENTER_ADVERTISING_CFM:
        {
            LE_CM_MSG_ENTER_ADVERTISING_CFM_T *cfm = (LE_CM_MSG_ENTER_ADVERTISING_CFM_T *)message;
            
            BLE_APP_PRINT("LE_CM_MSG_ENTER_ADVERTISING_CFM Status = %x\r\n", cfm->status);
            
            if (cfm->status == SYS_ERR_SUCCESS) app->state = APP_STATE_ADVERTISING;
        }
        break;

        case LE_CM_MSG_EXIT_ADVERTISING_CFM:
        {
            LE_CM_MSG_EXIT_ADVERTISING_CFM_T *cfm = (LE_CM_MSG_EXIT_ADVERTISING_CFM_T *)message;
            
            BLE_APP_PRINT("LE_CM_MSG_EXIT_ADVERTISING_CFM Status = %x\r\n", cfm->status);

            if (cfm->status == SYS_ERR_SUCCESS) app->state = APP_STATE_IDLE;
        }
        break;
        
        case LE_CM_CONNECTION_COMPLETE_IND:
        {
            LE_CM_CONNECTION_COMPLETE_IND_T *ind = (LE_CM_CONNECTION_COMPLETE_IND_T *)message;
            BLE_APP_PEER_T *peer = 0;
            
            BLE_APP_PRINT("LE_CM_CONNECTION_COMPLETE_IND status = %x\r\n", ind->status);

            app->state = APP_STATE_IDLE;
            
            if (ind->status == SYS_ERR_SUCCESS) 
            {
                peer = BleAppAddPeer(ind->conn_hdl);

                if (peer)
                {
                    peer->devid = ind->dev_id;
                    peer->bt_addr.type = ind->peer_addr_type;
                    MemCopy(peer->bt_addr.addr, ind->peer_addr, 6);
                    peer->interval = ind->conn_interval;
                    peer->latency = ind->conn_latency;
                    peer->sv_tmo = ind->supervison_timeout;

                    peer->mtu = app->default_mtu;

                    BleAppSendCommonMsg(BLE_APP_MSG_SECURITY, ind->conn_hdl, 100);
                }
            }

            if (!peer && (LeQueueCount(&app->peer) < LE_MAX_CONNECTION_COUNT))
            {
                LeSendMessageAfter(&app->task, BLE_APP_MSG_ENTER_ADVERTISING, 0, 500);
            }
        }
        break;

        case LE_CM_MSG_SIGNAL_UPDATE_REQ:
        {
            LE_CM_MSG_SIGNAL_UPDATE_REQ_T *req = (LE_CM_MSG_SIGNAL_UPDATE_REQ_T *)message;
            BLE_APP_PEER_T *peer = BleAppFindPeer(req->conn_hdl);
            
            if (peer) LeGapConnUpdateResponse(req->conn_hdl, req->identifier, TRUE);
        }
        break;

        case LE_CM_MSG_CONN_PARA_REQ:
        {
            LE_CM_MSG_CONN_PARA_REQ_T *req = (LE_CM_MSG_CONN_PARA_REQ_T *)message;
            BLE_APP_PEER_T *peer = BleAppFindPeer(req->conn_hdl);
            
            if (peer) LeGapConnParaRequestRsp(req->conn_hdl, TRUE);
        }
        break;

        case LE_CM_MSG_CONN_UPDATE_COMPLETE_IND:
        {
            LE_CM_MSG_CONN_UPDATE_COMPLETE_IND_T *ind = (LE_CM_MSG_CONN_UPDATE_COMPLETE_IND_T *)message;
            BLE_APP_PEER_T *peer = BleAppFindPeer(ind->conn_hdl);

            if (peer)
            {
                if (ind->status == SYS_ERR_SUCCESS)
                {
                    peer->interval = ind->interval;
                    peer->latency = ind->latency;
                    peer->sv_tmo = ind->supervision_timeout;
                }
                else
                {
                    LeGapDisconnectReq(ind->conn_hdl);
                }
            }
        }
        break;

        case LE_CM_MSG_SET_DISCONNECT_CFM:
        {
            LE_CM_MSG_SET_DISCONNECT_CFM_T *cfm = (LE_CM_MSG_SET_DISCONNECT_CFM_T *)message;
            BLE_APP_PRINT("LE_CM_MSG_SET_DISCONNECT_CFM conn_hdl = %x status = %x\r\n", cfm->handle, cfm->status);
        }
        break;
        
        case LE_CM_MSG_DISCONNECT_COMPLETE_IND:
        {
            LE_CM_MSG_DISCONNECT_COMPLETE_IND_T *ind = (LE_CM_MSG_DISCONNECT_COMPLETE_IND_T *)message;
            BLE_APP_PEER_T *peer = BleAppFindPeer(ind->conn_hdl);

            BLE_APP_PRINT("LE_CM_MSG_DISCONNECT_COMPLETE_IND conn_hdl = %x status = %x reason = %x \r\n", ind->conn_hdl, ind->status, ind->reason);

            if (peer)
            {
                BleAppGattPeerInfoClean(peer->conn_hdl);
                
                if (app->state == APP_STATE_IDLE) LeSendMessageAfter(&app->task, BLE_APP_MSG_ENTER_ADVERTISING, 0, 100);
            }
        }
        break;

        default:
        break;
    }
}

static void BleAppShowAttrInfo(BLE_APP_PEER_T *peer)
{
    BLE_SERVICE_T *svc = peer->svc;
    UINT16 i, j;

    BLE_APP_PRINT("\r\nHandle    format    uuid\r\n");

    for (i = 0; i < peer->svc_idx; i++)
    {
        if (svc[i].action == GATT_ACTION_IDLE)
        {
#if 0        
            if (svc[i].format == LE_GATT_UUID16)
                BLE_APP_PRINT("  %04x      %d       %04x\r\n", svc[i].start_hdl, svc[i].format, svc[i].uuid[0]);
            else
            {
                BLE_APP_PRINT("  %04x      %d\r\n", svc[i].start_hdl, svc[i].format);

            }
#endif   
            for (j = 0; j < svc[i].attr_count; j++)
            {
                if (svc[i].attr[j].format == LE_GATT_UUID16)
                    BLE_APP_PRINT("  %04x      %d       %04x\r\n", svc[i].attr[j].handle, svc[i].attr[j].format, svc[i].attr[j].uuid[0]);
                else
                {
                    BLE_APP_PRINT("  %04x      %d\r\n", svc[i].attr[j].handle, svc[i].attr[j].format);

                }
            }
           }
    }
}

static void BleAppFindAttr(BLE_APP_MSG_COMMON_T *req)
{
    BLE_APP_PEER_T *peer = BleAppFindPeer(req->conn_hdl);

    if (peer && !peer->action)
    {
        BLE_SERVICE_T *svc = peer->svc;
        LE_ERR_STATE rc = SYS_ERR_SUCCESS;
        UINT16 i;

        for (i = 0; i < BLE_SERVICE_COUNT; i++)
        {
            if (svc[i].action != GATT_ACTION_FIND_ATTR) continue;
            
            rc = LeGattFindAllCharDescriptor(peer->conn_hdl, svc[i].start_hdl, svc[i].end_hdl);

            if (rc == SYS_ERR_SUCCESS) 
            {
                peer->action = GATT_ACTION_WAIT_ATTR_INFO;
                peer->curr_svc = &svc[i];
                svc[i].action = GATT_ACTION_WAIT_ATTR_INFO;
                svc[i].attr_count = 0;

                return;
            }
        }

        peer->curr_svc = 0;
        BleAppShowAttrInfo(peer);

        // All attributes is found and find ANCS char
        BleAppSendCommonMsg(BLE_APP_MSG_FIND_ANCS_CHAR, peer->conn_hdl, 0);
    }
}

static void BleAppGattHandleRwReq(BLE_APP_MSG_COMMON_T *req)
{
    BLE_APP_PEER_T *peer = BleAppFindPeer(req->conn_hdl);
    GATT_REQ_HDR_T *hdr;

    if (!peer || peer->action) return;

    if (peer->rw_curr) return;

    hdr = LeQueuePop(&peer->rw_req);

    if (!hdr) return;

    if (BleAppGattAncsRwReq(peer->conn_hdl, hdr)) peer->rw_curr = hdr;

    if (!peer->rw_curr)
    {
        FreeMem(hdr);

        if (LeQueueCount(&peer->rw_req)) BleAppSendCommonMsg(BLE_APP_MSG_RW_REQ, peer->conn_hdl, 0);

        BLE_APP_PRINT("BleAppGattRwReq fail op = %d\r\n", hdr->op);
    }
}

static void BleAppMsgHandler(TASK task, MESSAGEID id, MESSAGE message)
{
    BLE_APP_DATA_T *app = BleAppGetEntity();

    switch (id)
    {
        case BLE_APP_MSG_INITIALIZING:
            LeCmInit(&app->task);
        break;

        case BLE_APP_MSG_ENTER_ADVERTISING:
        {
            LE_ERR_STATE rc = LeGapAdvertisingEnable(TRUE);
            
            if (rc != SYS_ERR_SUCCESS) 
            {
                BLE_APP_PRINT("BLE_APP_MSG_ENTER_ADVERTISING fail rc = %x\r\n", rc);
            }
        }
        break;

        case BLE_APP_MSG_EXIT_ADVERTISING:
        {
            LE_ERR_STATE rc = LeGapAdvertisingEnable(FALSE);
            
            if (rc != SYS_ERR_SUCCESS) 
            {
                BLE_APP_PRINT("BLE_APP_MSG_EXIT_ADVERTISING fail rc = %x\r\n", rc);
            }
        }
        break;

        case BLE_APP_MSG_SECURITY:
        {
            BLE_APP_MSG_COMMON_T *req = (BLE_APP_MSG_COMMON_T *)message;
            UINT16 rc = LeSmpSecurityReq(req->conn_hdl);

            if (rc != SYS_ERR_SUCCESS)
            {
                BLE_APP_PRINT("BLE_APP_MSG_REQUEST_PAIR fail rc = %x\r\n", rc);
            }
        }
        break;

        case BLE_APP_MSG_FIND_PRIMARY_SERVICE:
        {
            BLE_APP_MSG_COMMON_T *req = (BLE_APP_MSG_COMMON_T *)message;
            BLE_APP_PEER_T *peer = BleAppFindPeer(req->conn_hdl);
            LE_ERR_STATE rc;
            
            if (!peer || peer->action) return;

            rc = LeGattFindAllPrimaryService(req->conn_hdl);

            if (rc == SYS_ERR_SUCCESS)
            {
                peer->action = GATT_ACTION_FIND_PRIMARY_SERVICE;
            }
            else
            {
                BLE_APP_PRINT("BLE_APP_MSG_FIND_PRIMARY_SERVICE fail rc = %x\r\n", rc);
            }
        }
        break;

        case BLE_APP_MSG_FIND_ANCS_SERVICE:
        {
            BLE_APP_MSG_COMMON_T *req = (BLE_APP_MSG_COMMON_T *)message;
            BLE_APP_PEER_T *peer = BleAppFindPeer(req->conn_hdl);
            
            if (peer && !peer->action)
            {
                UINT8 svcUuid[16] = {ANCS_SVC_UUID128};
                LE_ERR_STATE rc = LeGattFindPrimaryServiceByUuid(req->conn_hdl, LE_GATT_UUID128, (UINT16 *)svcUuid);
                
                if (rc == SYS_ERR_SUCCESS) 
                {
                    peer->action = GATT_ACTION_FIND_ANCS_SERVICE;
                }
                else
                {
                    BLE_APP_PRINT("BLE_APP_MSG_FIND_ANCS_SERVICE fail rc = %x\r\n", rc);
                }
            }
        }
        break;

        case BLE_APP_MSG_FIND_ATTR:
            BleAppFindAttr((BLE_APP_MSG_COMMON_T *)message);
        break;

        case BLE_APP_MSG_FIND_ANCS_CHAR:
            BleAppFindAncsChar((BLE_APP_MSG_COMMON_T *)message);
        break;

        case BLE_APP_MSG_RW_REQ:
            BleAppGattHandleRwReq((BLE_APP_MSG_COMMON_T *)message);
        break;
        
        default:
        break;
    }
}

static void BleAppTaskHandler(TASK task, MESSAGEID id, MESSAGE message)
{
    if ((id >= LE_GATT_MSG_BASE) && (id < LE_GATT_MSG_TOP))
    {
        BleAppGattMsgHandler(task, id, message);
    }
    else if ((id >= BLE_APP_MSG_INITIALIZING) && (id < BLE_APP_MSG_TOP))
    {
        BleAppMsgHandler(task, id, message);
    }
    else if ((id >= LE_SMP_MSG_BASE) && (id < LE_SMP_MSG_TOP))
    {
        BleAppSmMsgHandler(task, id, message);
    }
    else if ((id >= LE_CM_MSG_BASE) && (id < LE_CM_MSG_TOP))
    {
        BleCmMsgHandler(task, id, message);
    }
}

BLE_APP_DATA_T* BleAppGetEntity(void)
{
    return &gTheBle;
}

void BleAppInit(void)
{
    BLE_APP_DATA_T *app = BleAppGetEntity();

    MemSet(app, 0, sizeof(BLE_APP_DATA_T));

    app->state = APP_STATE_INIT;
    app->default_mtu = 23;
    
    app->min_itvl = DEFAULT_DESIRED_MIN_CONN_INTERVAL;
    app->max_itvl = DEFAULT_DESIRED_MAX_CONN_INTERVAL;
    app->latency  = DEFAULT_DESIRED_SLAVE_LATENCY;
    app->sv_tmo   = DEFAULT_DESIRED_SUPERVERSION_TIMEOUT;

    LeHostCreateTask(&app->task, BleAppTaskHandler);
    LeSendMessage(&app->task, BLE_APP_MSG_INITIALIZING, 0);
}

void* BleAppFindPeer(UINT16 conn_hdl)
{
    BLE_APP_DATA_T *app = BleAppGetEntity();
    BLE_APP_PEER_T *peer = 0;
    UINT16 count = LeQueueCount(&app->peer);
    UINT16 i;
    
    for (i = 0; i < count; i++)
    {
        peer = LeQueuePeekAny(&app->peer, peer);

        if (!peer) break;

        if (peer->conn_hdl == conn_hdl) return peer;
    }

    return 0;
}

void BleAppSendCommonMsg(UINT32 msgId, UINT16 conn_hdl, UINT32 delay)
{
    BLE_APP_MSG_COMMON_T *msg = Malloc(sizeof(BLE_APP_MSG_COMMON_T));

    if (msg)
    {
        BLE_APP_DATA_T *app = BleAppGetEntity();
        
        msg->conn_hdl = conn_hdl;

        if (delay)
            LeSendMessageAfter(&app->task, msgId, msg, delay);
        else
            LeSendMessage(&app->task, msgId, msg);
    }
}

void BleAppMemDump(char *s, UINT16 len, void *data)
{
    UINT16 i;
    UINT8 *p = data;

    if (s)
    {
        BLE_APP_PRINT("\r\n%s:<", s);
    }
    
    for (i = 0; i < len; i++)
    {
        if (i < len - 1)
        {
            BLE_APP_PRINT("%02x ", p[i]);
        }
        else
        {
            BLE_APP_PRINT("%02x", p[i]);
        }
    }

    if (s)
    {
        BLE_APP_PRINT(">\r\n");
    }
    else
    {
        BLE_APP_PRINT("\r\n");
    }
}

