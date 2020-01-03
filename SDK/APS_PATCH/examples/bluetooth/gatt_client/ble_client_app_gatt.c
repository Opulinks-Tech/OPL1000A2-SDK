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

#include "ble_client_app.h"
#include "ble_client_app_gatt.h"
#include "ble_gatt_if.h"
#include "ble_uuid.h"
























void BleAppGattMsgHandler(TASK task, MESSAGEID id, MESSAGE message)
{
    BLE_APP_DATA_T *theApp = BleAppGetEntity();

    switch (id)
    {
        case LE_GATT_MSG_INIT_CFM:
        {
            BLE_APP_PRINT("LE_GATT_MSG_INIT_CFM\r\n");
        }
        break;

        case LE_GATT_MSG_EXCHANGE_MTU_IND:
        {
            LE_GATT_MSG_EXCHANGE_MTU_IND_T *ind = (LE_GATT_MSG_EXCHANGE_MTU_IND_T *)message;
            BLE_APP_PRINT("LE_GATT_MSG_EXCHANGE_MTU_IND client mtu = %d\r\n", ind->client_rx_mtu);
            LeGattExchangeMtuRsp(ind->conn_hdl, LE_ATT_MAX_MTU);
        }
        break;

        case LE_GATT_MSG_EXCHANGE_MTU_CFM:
        {
            LE_GATT_MSG_EXCHANGE_MTU_CFM_T *cfm = (LE_GATT_MSG_EXCHANGE_MTU_CFM_T *)message;
            BLE_APP_PRINT("LE_GATT_MSG_EXCHANGE_MTU_CFM curr mtu = %d\r\n", cfm->current_rx_mtu);
            BleAppGetEntity()->curr_mtu = cfm->current_rx_mtu;
        }
        break;

        case LE_GATT_MSG_SERVICE_INFO_IND:
        {
            LE_GATT_MSG_SERVICE_INFO_IND_T *ind = (LE_GATT_MSG_SERVICE_INFO_IND_T *)message;
            BLE_APP_PRINT("LE_GATT_MSG_SERVICE_INFO_IND uuid = %04x start = %x end = %x\r\n", ind->uuid[0], ind->start_hdl, ind->end_hdl);

            if (theApp->svc_idx < BLE_SERVICE_COUNT)
            {
                BLE_SERVICE_T *svc = &theApp->svc[theApp->svc_idx];
                theApp->svc_idx++;

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
        break;

        case LE_GATT_MSG_FIND_ALL_PRIMARY_SERVICE_CFM:
        {
            BLE_APP_PRINT("LE_GATT_MSG_FIND_ALL_PRIMARY_SERVICE_CFM\r\n");
            LeSendMessage(&BleAppGetEntity()->task, BLE_APP_MSG_FIND_ATTR, 0);
        }
        break;

        case LE_GATT_MSG_CHAR_DESCRIPTOR_INFO_IND:
        {
            LE_GATT_MSG_CHAR_DESCRIPTOR_INFO_IND_T *ind = (LE_GATT_MSG_CHAR_DESCRIPTOR_INFO_IND_T *)message;
            BLE_SERVICE_T *svc = BleAppGetEntity()->curr_svc;
            BLE_ATTRQ_T *attr = PanicUnlessMalloc(sizeof(BLE_ATTRQ_T));

            if (!svc) return;

            if (svc->action != GATT_ACTION_WAIT_INFO) return;

            if ((ind->handle <= svc->start_hdl) || (ind->handle > svc->end_hdl)) return;

            attr->handle = ind->handle;
            attr->format = ind->format;

            if (ind->format == LE_GATT_UUID16)
                attr->uuid[0] = ind->uuid[0];
            else
                MemCopy(attr->uuid, ind->uuid, 16);

            LeQueuePush(&BleAppGetEntity()->attrq, attr);
        }
        break;
        
        case LE_GATT_MSG_FIND_ALL_CHAR_DESC_CFM:
        {
            LE_GATT_MSG_FIND_ALL_CHAR_DESC_CFM_T *cfm = (LE_GATT_MSG_FIND_ALL_CHAR_DESC_CFM_T *)message;
            BLE_SERVICE_T *svc = BleAppGetEntity()->curr_svc;
            UINT16 count = LeQueueCount(&BleAppGetEntity()->attrq);
            BLE_APP_PRINT("LE_GATT_MSG_FIND_ALL_CHAR_DESC_CFM cid = %x attErr = %x count = %d\r\n", cfm->conn_hdl, cfm->att_err, count);

            if (count)
            {
                BLE_ATT_ATTR_T *attr = PanicUnlessMalloc(sizeof(BLE_ATT_ATTR_T) * count);
                BLE_ATTRQ_T *attrm;
                UINT16 idx = 0;
                
                svc->attr_count = count;
                svc->attr = attr;

                while (count--)
                {
                    attrm = LeQueuePop(&BleAppGetEntity()->attrq);

                    if (attrm)
                    {
                        attr[idx].handle = attrm->handle;
                        attr[idx].format = attrm->format;

                        if (attrm->format == LE_GATT_UUID16)
                            attr[idx].uuid[0] = attrm->uuid[0];
                        else
                            MemCopy(attr[idx].uuid, attrm->uuid, 16);

                        FreeMem(attrm);
                        idx++;
                    }
                    else
                        break;
                }
            }

            svc->action = GATT_ACTION_FINISH;
            LeSendMessage(&BleAppGetEntity()->task, BLE_APP_MSG_FIND_ATTR, 0);
        }
        break;

        case LE_GATT_MSG_OPERATION_TIMEOUT:
        {
            LE_GATT_MSG_OPERATION_TIMEOUT_T *ind = (LE_GATT_MSG_OPERATION_TIMEOUT_T *)message;
            BLE_APP_PRINT("LE_GATT_MSG_OPERATION_TIMEOUT op = %x\r\n", ind->att_op);
        }
        break;

        default:
        break;
    }
}

