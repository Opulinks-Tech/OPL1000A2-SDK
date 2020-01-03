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

#include "ble_gap_if.h"
#include "ble_gatt_if.h"
#include "ble_uuid.h"
#include "ancs_app.h"
#include "ancs_app_gatt.h"
#include "ancs_app_client.h"


static void BleAppGattHandleServiceInfoInd(LE_GATT_MSG_SERVICE_INFO_IND_T *ind)
{
    BLE_APP_PEER_T *peer = BleAppFindPeer(ind->conn_hdl);

    BLE_APP_PRINT("LE_GATT_MSG_SERVICE_INFO_IND uuid = %04x start = %x end = %x\r\n", ind->uuid[0], ind->start_hdl, ind->end_hdl);

    if (!peer) return;

    if ((peer->action != GATT_ACTION_FIND_PRIMARY_SERVICE) && (peer->action != GATT_ACTION_FIND_ANCS_SERVICE)) return;

    if (peer->svc_idx < BLE_SERVICE_COUNT)
    {
        BLE_SERVICE_T *svc = &peer->svc[peer->svc_idx];
        
        peer->svc_idx++;

        svc->start_hdl = ind->start_hdl;
        svc->end_hdl = ind->end_hdl;
        svc->action = GATT_ACTION_FIND_ATTR;
        svc->format = ind->format;

        if (ind->format == LE_GATT_UUID16)
            svc->uuid[0] = ind->uuid[0];
        else
            MemCopy(svc->uuid, ind->uuid, 16);

        svc->attr = 0;
    }
}

static void BleAppGattHandleFindServiceCfm(UINT16 conn_hdl)
{
    BLE_APP_PEER_T *peer = BleAppFindPeer(conn_hdl);

    if (!peer) return;

    if ((peer->action != GATT_ACTION_FIND_PRIMARY_SERVICE) && (peer->action != GATT_ACTION_FIND_ANCS_SERVICE)) return;

    if (peer)
    {
        peer->action = GATT_ACTION_IDLE;
        BleAppSendCommonMsg(BLE_APP_MSG_FIND_ATTR, conn_hdl, 0);
    }
}

static void BleAppGattHandleCharDescInfoInd(LE_GATT_MSG_CHAR_DESCRIPTOR_INFO_IND_T *ind)
{
    BLE_APP_PEER_T *peer = BleAppFindPeer(ind->conn_hdl);
    BLE_SERVICE_T *svc = peer->curr_svc;
    BLE_ATTRQ_T *attr;

    if (!peer || (peer->action != GATT_ACTION_WAIT_ATTR_INFO)) return;

    if (!svc || (svc->action != GATT_ACTION_WAIT_ATTR_INFO)) return;

    if ((ind->handle < svc->start_hdl) || (ind->handle > svc->end_hdl)) return;

    attr = PanicUnlessMalloc(sizeof(BLE_ATTRQ_T));

    attr->handle = ind->handle;
    attr->format = ind->format;

    if (ind->format == LE_GATT_UUID16)
        attr->uuid[0] = ind->uuid[0];
    else
        MemCopy(attr->uuid, ind->uuid, 16);

    LeQueuePush(&peer->attrq, attr);
}

static void BleAppGattHandleFindCharDescCfm(LE_GATT_MSG_FIND_ALL_CHAR_DESC_CFM_T *cfm)
{
    BLE_APP_PEER_T *peer = BleAppFindPeer(cfm->conn_hdl);
    BLE_SERVICE_T *svc = peer->curr_svc;
    UINT16 count = LeQueueCount(&peer->attrq);

    if (!peer || (peer->action != GATT_ACTION_WAIT_ATTR_INFO)) return;

    if (!svc || (svc->action != GATT_ACTION_WAIT_ATTR_INFO)) 
    {
        if (count) LeQueueBroken(&peer->attrq);

        count = 0;
    }

    if (count)
    {
        BLE_ATT_ATTR_T *attr = PanicUnlessMalloc(sizeof(BLE_ATT_ATTR_T) * count);
        BLE_ATTRQ_T *attrm;
        UINT16 idx = 0;

        svc->attr_count = 0;
        svc->attr = attr;

        while (count--)
        {
            attrm = LeQueuePop(&peer->attrq);

            if (!attrm) break;

            svc->attr_count++;

            attr[idx].handle = attrm->handle;
            attr[idx].format = attrm->format;

            if (attrm->format == LE_GATT_UUID16)
                attr[idx].uuid[0] = attrm->uuid[0];
            else
                MemCopy(attr[idx].uuid, attrm->uuid, 16);

            FreeMem(attrm);
            idx++;
        }

        if (count) LeQueueBroken(&peer->attrq);
    }

    if (svc) 
    {
        peer->action = GATT_ACTION_IDLE;
        svc->action = GATT_ACTION_IDLE;

        BleAppSendCommonMsg(BLE_APP_MSG_FIND_ATTR, peer->conn_hdl, 0);
    }
}

static void BleAppGattHandleCharDeclInfoInd(LE_GATT_MSG_CHARACTERISTIC_DECL_INFO_IND_T *ind)
{
    BleAppAncsHandleCharDeclInfoInd(ind);
}

static void BleAppGattHandleFindCharCfm(LE_GATT_MSG_FIND_CHARACTERISTIC_CFM_T *cfm)
{
    BleAppAncsHandleFindCharCfm(cfm);
}

static void BleAppGattHandleWriteCharValCfm(LE_GATT_MSG_WRITE_CHAR_VALUE_CFM_T *cfm)
{
    BleAppAncsHandleWriteCharValCfm(cfm);
}

static void BleAppGattHandleCharValInd(LE_GATT_MSG_CHARACTERISTIC_VAL_IND_T *ind)
{
    BleAppAncsHandleCharValInd(ind);
}

static void BleAppGattHandleReadCharValCfm(LE_GATT_MSG_READ_CHARACTERISTIC_VALUE_CFM_T *cfm)
{
    BleAppAncsHandleReadCharValCfm(cfm);
}

static void BleAppGattHandleNotifyInd(LE_GATT_MSG_NOTIFY_IND_T *ind)
{
    BleAppAncsHandleNotifyInd(ind);
}

void BleAppGattMsgHandler(TASK task, MESSAGEID id, MESSAGE message)
{
    BLE_APP_DATA_T *app = BleAppGetEntity();

    switch (id)
    {
        case LE_GATT_MSG_NOTIFY_IND:
            BleAppGattHandleNotifyInd((LE_GATT_MSG_NOTIFY_IND_T *)message);
        break;
        
        case LE_GATT_MSG_INIT_CFM:
        {
            BLE_APP_PRINT("LE_GATT_MSG_INIT_CFM\r\n");
        }
        break;

        case LE_GATT_MSG_EXCHANGE_MTU_IND:
        {
            LE_GATT_MSG_EXCHANGE_MTU_IND_T *ind = (LE_GATT_MSG_EXCHANGE_MTU_IND_T *)message;
            BLE_APP_PEER_T *peer = BleAppFindPeer(ind->conn_hdl);
            
            BLE_APP_PRINT("LE_GATT_MSG_EXCHANGE_MTU_IND client mtu = %d\r\n", ind->client_rx_mtu);
            
            if (peer) LeGattExchangeMtuRsp(ind->conn_hdl, LE_ATT_MAX_MTU);
        }
        break;

        case LE_GATT_MSG_EXCHANGE_MTU_CFM:
        {
            LE_GATT_MSG_EXCHANGE_MTU_CFM_T *cfm = (LE_GATT_MSG_EXCHANGE_MTU_CFM_T *)message;
            BLE_APP_PEER_T *peer = BleAppFindPeer(cfm->conn_hdl);
            
            BLE_APP_PRINT("LE_GATT_MSG_EXCHANGE_MTU_CFM curr mtu = %d\r\n", cfm->current_rx_mtu);

            if (peer) peer->mtu = cfm->current_rx_mtu;
        }
        break;

        case LE_GATT_MSG_SERVICE_INFO_IND:
            BleAppGattHandleServiceInfoInd((LE_GATT_MSG_SERVICE_INFO_IND_T *)message);
        break;

        case LE_GATT_MSG_FIND_ALL_PRIMARY_SERVICE_CFM:
        case LE_GATT_MSG_FIND_PRIMARY_SERVICE_BY_UUID_CFM:
            BleAppGattHandleFindServiceCfm(((LE_GATT_MSG_FIND_ALL_PRIMARY_SERVICE_CFM_T *)message)->conn_hdl);
        break;

        case LE_GATT_MSG_CHAR_DESCRIPTOR_INFO_IND:
            BleAppGattHandleCharDescInfoInd((LE_GATT_MSG_CHAR_DESCRIPTOR_INFO_IND_T *)message);
        break;
        
        case LE_GATT_MSG_FIND_ALL_CHAR_DESC_CFM:
            BleAppGattHandleFindCharDescCfm((LE_GATT_MSG_FIND_ALL_CHAR_DESC_CFM_T *)message);
        break;

        case LE_GATT_MSG_CHARACTERISTIC_DECL_INFO_IND:
            BleAppGattHandleCharDeclInfoInd((LE_GATT_MSG_CHARACTERISTIC_DECL_INFO_IND_T *)message);
        break;

        case LE_GATT_MSG_FIND_CHARACTERISTIC_CFM:
            BleAppGattHandleFindCharCfm((LE_GATT_MSG_FIND_CHARACTERISTIC_CFM_T *)message);
        break;

        case LE_GATT_MSG_CHARACTERISTIC_VAL_IND:
            BleAppGattHandleCharValInd((LE_GATT_MSG_CHARACTERISTIC_VAL_IND_T *)message);
        break;

        case LE_GATT_MSG_READ_CHARACTERISTIC_VALUE_CFM:
            BleAppGattHandleReadCharValCfm((LE_GATT_MSG_READ_CHARACTERISTIC_VALUE_CFM_T *)message);
        break;

        case LE_GATT_MSG_WRITE_CHAR_VALUE_CFM:
            BleAppGattHandleWriteCharValCfm((LE_GATT_MSG_WRITE_CHAR_VALUE_CFM_T *)message);
        break;
        
        case LE_GATT_MSG_OPERATION_TIMEOUT:
        {
            LE_GATT_MSG_OPERATION_TIMEOUT_T *ind = (LE_GATT_MSG_OPERATION_TIMEOUT_T *)message;
            BLE_APP_PEER_T *peer = BleAppFindPeer(ind->conn_hdl);
            
            BLE_APP_PRINT("LE_GATT_MSG_OPERATION_TIMEOUT op = %x\r\n", ind->att_op);
            
            if (peer) LeGapDisconnectReq(peer->conn_hdl);
        }
        break;

        default:
        break;
    }
}

