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


static UINT16 BleAppFindCharCccdHdl(BLE_SERVICE_T *svc, BLE_CHAR_DECL_T *decl)
{
    BLE_ATT_ATTR_T *attr = svc->attr;
    UINT16 s = decl->handle + 1;
    UINT16 e = 0;
    UINT16 i;
    
    if (!svc || !attr || !decl || (svc->attr_count < 4)) return 0;

    if ((s < svc->start_hdl) || (s > svc->end_hdl)) return 0;

    for (i = 1; i < svc->attr_count; i++)
    {
        if (attr[i].handle > s)
        {
            if ((attr[i].format == LE_GATT_UUID16) && (attr[i].uuid[0] == GATT_CHARACTERISTIC_UUID)) break;
        
            if ((attr[i].format == LE_GATT_UUID16) && (attr[i].uuid[0] == GATT_CLIENT_CHAR_CFG_UUID)) 
            {
                e = attr[i].handle;

                break;
            }
        }
    }

    return e;
}

static void BleAppBuildGetNotifyAttrCmdData(UINT8 **buf, UINT8 attr_id, UINT16 len)
{
    UINT8 *p = *buf;

    *p++ = attr_id;

    if (len) 
    {
        MemCopy(p, &len, 2);
        p += 2;
    }

    *buf = p;
}

static void* BleAppAncsNotifySrcFindByUid(BLE_APP_PEER_T *peer, UINT32 uid)
{
    BLE_ANCS_INFO_T *ancs = peer->ancs;
    ANCS_NOTIFY_SRC_DATA_T *ansd = 0;
    UINT16 count;
    UINT16 i;

    if (!ancs) return 0;

    count = LeQueueCount(&ancs->ansl);

    for (i = 0; i < count; i++)
    {
        ansd = LeQueuePeekAny(&ancs->ansl, ansd);

        if (!ansd) break;

        if (ansd->notify_uid == uid) return ansd;
    }

    return 0;
}

static void BleAppAncsHandleNotifySrcData(BLE_APP_PEER_T *peer, UINT16 len, void *data)
{
    if (len == 8)
    {
        UINT8 *p = data;
        UINT8 evtId = p[0];
        UINT8 cateId = p[2];
        UINT32 uid = *((UINT32 *)&p[4]);
        
        if ((cateId == CategoryIDIncomingCall) || (cateId == CategoryIDMissedCall))
        {
            BLE_ANCS_INFO_T *ancs = peer->ancs;
            ANCS_NOTIFY_SRC_DATA_T *ansd = BleAppAncsNotifySrcFindByUid(peer, uid);
        
            if (evtId == EventIDNotificationAdded)
            {
                if (ansd) return;

                ansd = Malloc(sizeof(ANCS_NOTIFY_SRC_DATA_T));

                if (!ansd) return;

                ansd->evt_id = evtId;
                ansd->evt_flag = p[1];
                ansd->category_id = cateId;
                ansd->category_count = p[3];
                ansd->notify_uid = uid;
                ansd->appid_flag = 0;
                
                LeQueuePush(&ancs->ansl, ansd);

                BleAppAncsGetNotifyAttr(peer->conn_hdl, uid, 0xFF);
            }
            else if (evtId == EventIDNotificationModified)
            {
                if (!ansd) return;

                ansd->evt_id = evtId;
                ansd->evt_flag = p[1];
                ansd->category_id = cateId;
                ansd->category_count = p[3];
                ansd->appid_flag = 0;
                
                BleAppAncsGetNotifyAttr(peer->conn_hdl, uid, 0xFF);
            }
            else if (evtId == EventIDNotificationRemoved)
            {
                if (!ansd) return;

                LeQueueDelete(&ancs->ansl, ansd);
            }
        }
    }
}

static void BleAppAncsShowNotifyAttrInfo(UINT16 rsp_len, void *rsp_data)
{
    UINT8 *p = rsp_data;
    UINT16 len = rsp_len;
    UINT8 attrId;
    UINT16 attrLen;
    char *s;
    
    while (len)
    {
        attrId = *p++;
        attrLen = *((UINT16 *)p);
        p += 2;

        s = 0;
    
        switch (attrId)
        {
            case NotificationAttributeIDAppIdentifier:
                s = "AppId";
            break;

            case NotificationAttributeIDTitle:
                s = "Title";
            break;

            case NotificationAttributeIDSubtitle:
                s = "Subtitle";
            break;

            case NotificationAttributeIDMessage:
                s = "Msg";
            break;

            case NotificationAttributeIDMessageSize:
                s = "MsgSize";
            break;

            case NotificationAttributeIDDate:
                s = "Date";
            break;

            case NotificationAttributeIDPositiveActionLabel:
                s = "PositiveLable";
            break;

            case NotificationAttributeIDNegativeActionLabel:
                s = "NegativeLable";
            break;

            default:
            break;
        }

        if (s) BleAppMemDump(s, attrLen, p);

        p += attrLen;
        len -= (3 + attrLen);
    }
}

static void BleAppAncsBuildAppId(BLE_APP_PEER_T *peer, UINT32 uid, UINT16 rsp_len, void *rsp_data)
{
    UINT8 *p = rsp_data;
    UINT16 len = rsp_len;
    UINT8 attrId;
    UINT16 attrLen;
    
    while (len)
    {
        attrId = *p++;
        attrLen = *((UINT16 *)p);
        p += 2;

        switch (attrId)
        {
            case NotificationAttributeIDAppIdentifier:
            {
                ANCS_NOTIFY_SRC_DATA_T *ansd = BleAppAncsNotifySrcFindByUid(peer, uid);

                if (!ansd) return;

                if (attrLen > 64) return;

                ansd->appid_flag = attrLen | 0x0100;

                if (attrLen) MemCopy(ansd->appid, p, attrLen);

                // get display name
                BleAppAncsGetAppAttr(peer->conn_hdl, uid);
            }
            return;

            default:
            break;
        }

        p += attrLen;
        len -= (3 + attrLen);
    }
}

static void BleAppAncsHandleGetNotifyAttrInfo(BLE_APP_PEER_T *peer, UINT16 len, void *data)
{
    BLE_ANCS_INFO_T *ancs = peer->ancs;
    ANCS_GET_NOTI_ATTR_CMD_T *cmd = (ANCS_GET_NOTI_ATTR_CMD_T *)ancs->cmd;    
    UINT16 idx = cmd->chk_idx;
    UINT8 *p = cmd->rsp_buf;

    if (!idx)
    {
        if (p[0] != CommandIDGetNotificationAttributes) return;

        idx++;
        cmd->rsp_len = 0;
    }

    if ((cmd->rsp_len + len) > 256) goto done;

    MemCopy(p + cmd->rsp_len, data, len);
    cmd->rsp_len += len;

    if (idx == 1)
    {
        if (cmd->rsp_len >= 5)
        {
            UINT32 uid = *((UINT32 *)&p[1]);
            ANCS_NOTIFY_SRC_DATA_T *ansd;
        
            if (uid != cmd->notify_uid) 
            {
                cmd->chk_idx = 0;
                cmd->rsp_len = 0;
                LeGapDisconnectReq(peer->conn_hdl);
                return;
            }

            ansd = BleAppAncsNotifySrcFindByUid(peer, uid);

            if (!ansd) goto done;

            idx += 4;
            cmd->chk_idx = idx;
        }
    }

    if (idx >= 5)
    {
        UINT16 remain = cmd->rsp_len - idx;
        UINT8 attrId;
        UINT16 attrLen;

        while (remain >= 3)
        {
            attrId = p[idx];
            attrLen = *((UINT16 *)&p[idx + 1]);

            remain -= 3;

            if (remain >= attrLen)
            {
                cmd->attr_id_flag &= ~(1 << attrId);

                cmd->chk_idx = idx + 3 + attrLen;

                idx = cmd->chk_idx;
                remain -= attrLen;

                if (!cmd->attr_id_flag) break;
            }
            else
                break;
        }
    }

    if (!cmd->attr_id_flag)
    {
        BleAppAncsBuildAppId(peer, cmd->notify_uid, cmd->rsp_len - 5, &cmd->rsp_buf[5]);
        BleAppAncsShowNotifyAttrInfo(cmd->rsp_len - 5, &cmd->rsp_buf[5]);
        
        goto done;
    }

    return;
    
done:
    ancs->cmd = 0;

    if (cmd) FreeMem(cmd);
}

static void BleAppAncsShowAppAttrInfo(UINT16 rsp_len, void *rsp_data)
{
    UINT8 *p = rsp_data;
    UINT16 len = rsp_len;
    UINT8 attrId;
    UINT16 attrLen;
    char *s;

    while (len)
    {
        attrId = *p++;
        attrLen = *((UINT16 *)p);
        p += 2;

        s = 0;
    
        switch (attrId)
        {
            case AppAttributeIDDisplayName:
                s = "DisplayName";
            break;

            default:
            break;
        }

        if (s) BleAppMemDump(s, attrLen, p);

        p += attrLen;
        len -= (3 + attrLen);
    }
}

static void BleAppAncsHandleGetAppAttrInfo(BLE_APP_PEER_T *peer, UINT16 len, void *data)
{
    BLE_ANCS_INFO_T *ancs = peer->ancs;
    ANCS_GET_APP_ATTR_CMD_T *cmd = (ANCS_GET_APP_ATTR_CMD_T *)ancs->cmd;    
    UINT16 idx = cmd->chk_idx;
    UINT8 *p = cmd->rsp_buf;
    ANCS_NOTIFY_SRC_DATA_T *ansd = 0;
    
    if (!idx)
    {
        if (p[0] != CommandIDGetAppAttributes) return;

        idx++;
        cmd->rsp_len = 0;
    }

    if ((cmd->rsp_len + len) > 128) goto done;

    MemCopy(p + cmd->rsp_len, data, len);
    cmd->rsp_len += len;

    if (idx == 1)
    {
        if (cmd->rsp_len >= (cmd->appid_len + 2))
        {
            ansd = BleAppAncsNotifySrcFindByUid(peer, cmd->uid);

            if (!ansd || MemCmp(&p[1], ansd->appid, cmd->appid_len))
            {
                cmd->chk_idx = 0;
                LeGapDisconnectReq(peer->conn_hdl);
                return;
            }

            idx += (cmd->appid_len + 1);
            cmd->chk_idx = idx;
        }
    }

    if (idx >= (cmd->appid_len + 2))
    {
        UINT16 remain = cmd->rsp_len - idx;
        UINT8 attrId;
        UINT16 attrLen;

        while (remain >= 3)
        {
            attrId = p[idx];
            attrLen = *((UINT16 *)&p[idx + 1]);

            remain -= 3;

            if (remain >= attrLen)
            {
                cmd->attr_id_flag &= ~(1 << attrId);

                cmd->chk_idx = idx + 3 + attrLen;

                idx = cmd->chk_idx;
                remain -= attrLen;

                if (!cmd->attr_id_flag) break;
            }
            else
                break;
        }
    }

    if (!cmd->attr_id_flag)
    {
        BleAppAncsShowAppAttrInfo(cmd->rsp_len - cmd->appid_len - 2, &cmd->rsp_buf[cmd->appid_len + 2]);

        if (!ansd) ansd = BleAppAncsNotifySrcFindByUid(peer, cmd->uid);

        if (ansd)
        {
            if ((ansd->category_id == CategoryIDIncomingCall) && (ansd->evt_id == EventIDNotificationAdded))
            {
                BleAppAncsNotifyAction(peer->conn_hdl, ansd->notify_uid, ActionIDPositive);
            }              
        }

        goto done;
    }

    return;
    
done:
    ancs->cmd = 0;

    if (cmd) FreeMem(cmd);
}

static void BleAppAncsHandleDataSrcData(BLE_APP_PEER_T *peer, UINT16 len, void *data)
{
    if (len)
    {
        BLE_ANCS_INFO_T *ancs = peer->ancs;
        ANCS_GET_NOTI_ATTR_CMD_T *cmd;
        
        if (!ancs->cmd) 
        {
            if (!LeQueueCount(&ancs->cmd_list)) return;

            ancs->cmd = LeQueuePop(&ancs->cmd_list);
        }

        cmd = ancs->cmd;

        if (cmd->cmd_id == CommandIDGetNotificationAttributes)
        {
            BleAppAncsHandleGetNotifyAttrInfo(peer, len, data);
        }
        else if (cmd->cmd_id == CommandIDGetAppAttributes)
        {
            BleAppAncsHandleGetAppAttrInfo(peer, len, data);
        }
    }
}

// GATT handler
void BleAppAncsHandleCharDeclInfoInd(LE_GATT_MSG_CHARACTERISTIC_DECL_INFO_IND_T *ind)
{
    BLE_APP_PEER_T *peer = BleAppFindPeer(ind->conn_hdl);
    BLE_ANCS_INFO_T *ancs = peer->ancs;
    BLE_SERVICE_T *svc = ancs->svc;
    ANCS_CHAR_T *cha = 0;

    if (!peer || (peer->action != GATT_ACTION_WAIT_CHAR_INFO)) return;

    if (!ancs || (svc->action != GATT_ACTION_WAIT_CHAR_INFO)) return;

    if ((ind->handle <= svc->start_hdl) || (ind->handle > svc->end_hdl)) return;

    if (ind->format == LE_GATT_UUID16)
    {
        if (ind->uuid[0] == ANCS_NOTIF_SRC_CHAR_UUID16)
        {
            if (!ancs->noti_src)
            {
                cha = PanicUnlessMalloc(sizeof(ANCS_CHAR_T) + sizeof(BLE_CHAR_CCCD_T));
                cha->cccd = (BLE_CHAR_CCCD_T *)((UINT8 *)cha + sizeof(ANCS_CHAR_T));
                ancs->noti_src = cha;
            }
        }
        else if (ind->uuid[0] == ANCS_CTRL_PT_CHAR_UUID16)
        {
            if (!ancs->ctrl_pt)
            {
                cha = PanicUnlessMalloc(sizeof(ANCS_CHAR_T));
                ancs->ctrl_pt = cha;
            }
        }
        else if (ind->uuid[0] == ANCS_DATA_SRC_CHAR_UUID16)
        {
            if (!ancs->data_src)
            {
                cha = PanicUnlessMalloc(sizeof(ANCS_CHAR_T));
                cha->cccd = (BLE_CHAR_CCCD_T *)((UINT8 *)cha + sizeof(ANCS_CHAR_T));
                ancs->data_src = cha;
            }
        }
    }
    else
    {
        if (!ancs->noti_src)
        {
            UINT8 uuid[16] = {ANCS_NOTIF_SRC_CHAR_UUID128};

            if (!MemCmp(ind->uuid, uuid, 16))
            {
                cha = PanicUnlessMalloc(sizeof(ANCS_CHAR_T) + sizeof(BLE_CHAR_CCCD_T));
                cha->cccd = (BLE_CHAR_CCCD_T *)((UINT8 *)cha + sizeof(ANCS_CHAR_T));
                ancs->noti_src = cha;
                goto done;
            }
        }

        if (!ancs->ctrl_pt)
        {
            UINT8 uuid[16] = {ANCS_CTRL_PT_CHAR_UUID128};

            if (!MemCmp(ind->uuid, uuid, 16))
            {
                cha = PanicUnlessMalloc(sizeof(ANCS_CHAR_T));
                ancs->ctrl_pt = cha;
                goto done;
            }
        }

        if (!ancs->data_src)
        {
            UINT8 uuid[16] = {ANCS_DATA_SRC_CHAR_UUID128};

            if (!MemCmp(ind->uuid, uuid, 16))
            {
                cha = PanicUnlessMalloc(sizeof(ANCS_CHAR_T));
                cha->cccd = (BLE_CHAR_CCCD_T *)((UINT8 *)cha + sizeof(ANCS_CHAR_T));
                ancs->data_src = cha;
                goto done;
            }
        }
    }

done:
    if (cha)
    {
        cha->decl.handle = ind->handle;
        cha->decl.property = ind->property;
        cha->decl.val_hdl = ind->val_hdl;
        cha->decl.format = ind->format;

        if (ind->format == LE_GATT_UUID16)
            cha->decl.uuid[0] = ind->uuid[0];
        else
            MemCopy(cha->decl.uuid, ind->uuid, 16);
    }
}

void BleAppAncsHandleFindCharCfm(LE_GATT_MSG_FIND_CHARACTERISTIC_CFM_T *cfm)
{
    BLE_APP_PEER_T *peer = BleAppFindPeer(cfm->conn_hdl);
    BLE_ANCS_INFO_T *ancs = peer->ancs;
    BLE_SERVICE_T *svc = ancs->svc;

    if (!peer || (peer->action != GATT_ACTION_WAIT_CHAR_INFO)) return;

    if (!ancs || (svc->action != GATT_ACTION_WAIT_CHAR_INFO)) return;

    if (ancs->noti_src)
    {
        ancs->noti_src->cccd->handle = BleAppFindCharCccdHdl(svc, &ancs->noti_src->decl);
    }

    if (ancs->data_src)
    {
        ancs->data_src->cccd->handle = BleAppFindCharCccdHdl(svc, &ancs->data_src->decl);
    }

    peer->action = GATT_ACTION_IDLE;
    svc->action = GATT_ACTION_IDLE;

    BleAppAncsRwCccdValReq(peer->conn_hdl, ANCS_WR_DATA_SRC_CCCD_VAL, 1);
}

void BleAppAncsHandleWriteCharValCfm(LE_GATT_MSG_WRITE_CHAR_VALUE_CFM_T *cfm)
{
    BLE_APP_PEER_T *peer = BleAppFindPeer(cfm->conn_hdl);
    BLE_ANCS_INFO_T *ancs = peer->ancs;

    if (cfm->status || cfm->att_err)
    {
        BLE_APP_PRINT("BleAppAncsHandleWriteCharValCfm status = %x at_err = %x\r\n", cfm->status, cfm->att_err);
    }

    if (!peer || !peer->action || !ancs || !peer->rw_curr) return;

    if (peer->action == GATT_ACTION_RW_CCCD_VAL)
    {
        GATT_REQ_HDR_T *hdr = peer->rw_curr;

        if (hdr->op == ANCS_WR_NOTI_SRC_CCCD_VAL)
        {
            if (cfm->status == SYS_ERR_SUCCESS) ancs->noti_src->cccd->config = 1;

            peer->action = GATT_ACTION_IDLE;
        }
        else if (hdr->op == ANCS_WR_DATA_SRC_CCCD_VAL)
        {
            if (cfm->status == SYS_ERR_SUCCESS) ancs->data_src->cccd->config = 1;

            peer->action = GATT_ACTION_IDLE;

            BleAppAncsRwCccdValReq(peer->conn_hdl, ANCS_WR_NOTI_SRC_CCCD_VAL, 1);
        }

        if (peer->action == GATT_ACTION_IDLE)
        {
            FreeMem(hdr);
            peer->rw_curr = 0;
        }
    }
    else if (peer->action == GATT_ACTION_WR_ANCS_CMD)
    {
        GATT_REQ_HDR_T *hdr = peer->rw_curr;

        if (hdr->op == ANCS_WR_CTRL_PT_CMD)
        {
            if (cfm->status == SYS_ERR_SUCCESS)
            {
                ANCS_CMD_HDR_T *cmd = (ANCS_CMD_HDR_T *)peer->rw_curr;

                if ((cmd->cmd_id == CommandIDGetNotificationAttributes) || (cmd->cmd_id == CommandIDGetAppAttributes)) 
                    LeQueuePush(&ancs->cmd_list, hdr);
                else
                    FreeMem(hdr);
            }
            else
                FreeMem(hdr);
        
            peer->rw_curr = 0;
            peer->action = GATT_ACTION_IDLE;
        }
    }

    if (!peer->action) BleAppSendCommonMsg(BLE_APP_MSG_RW_REQ, cfm->conn_hdl, 0);
}

void BleAppAncsHandleCharValInd(LE_GATT_MSG_CHARACTERISTIC_VAL_IND_T *ind)
{
    BLE_APP_PEER_T *peer = BleAppFindPeer(ind->conn_hdl);
    BLE_ANCS_INFO_T *ancs = peer->ancs;

    if (!peer || !peer->action || !ancs || !peer->rw_curr) return;

    if (peer->action == GATT_ACTION_RW_CCCD_VAL)
    {
        GATT_REQ_HDR_T *hdr = peer->rw_curr;

        if (hdr->op == ANCS_RD_NOTI_SRC_CCCD_VAL)
        {
            ancs->noti_src->cccd->config = *((UINT16 *)ind->val);
        }
        else if (hdr->op == ANCS_RD_DATA_SRC_CCCD_VAL)
        {
            ancs->data_src->cccd->config = *((UINT16 *)ind->val);
        }
    }
}

void BleAppAncsHandleReadCharValCfm(LE_GATT_MSG_READ_CHARACTERISTIC_VALUE_CFM_T *cfm)
{
    BLE_APP_PEER_T *peer = BleAppFindPeer(cfm->conn_hdl);
    BLE_ANCS_INFO_T *ancs = peer->ancs;

    if (!peer || !peer->action || !ancs || !peer->rw_curr) return;

    if (peer->action == GATT_ACTION_RW_CCCD_VAL)
    {
        GATT_REQ_HDR_T *hdr = peer->rw_curr;

        if ((hdr->op == ANCS_RD_NOTI_SRC_CCCD_VAL) || (hdr->op == ANCS_RD_DATA_SRC_CCCD_VAL))
        {
            peer->action = GATT_ACTION_IDLE;
            FreeMem(hdr);
            peer->rw_curr = 0;

            BleAppSendCommonMsg(BLE_APP_MSG_RW_REQ, cfm->conn_hdl, 0);
        }
    }
}

void BleAppAncsHandleNotifyInd(LE_GATT_MSG_NOTIFY_IND_T *ind)
{
    BLE_APP_PEER_T *peer = BleAppFindPeer(ind->conn_hdl);
    BLE_ANCS_INFO_T *ancs = peer->ancs;

    if (!peer || !ancs) return;

    if (ancs->noti_src && (ind->handle == ancs->noti_src->decl.val_hdl))
    {
        BleAppMemDump("ANCSN", ind->len, ind->val);

        BleAppAncsHandleNotifySrcData(peer, ind->len, ind->val);
    }
    else if (ancs->data_src && (ind->handle == ancs->data_src->decl.val_hdl))
    {
        BleAppMemDump("ANCSD", ind->len, ind->val);

        BleAppAncsHandleDataSrcData(peer, ind->len, ind->val);
    }
    else
    {
        BleAppMemDump("ANCSO", ind->len, ind->val);
    }
}

void BleAppAncsInfoClean(UINT16 conn_hdl)
{
    BLE_APP_PEER_T *peer = BleAppFindPeer(conn_hdl);
    BLE_ANCS_INFO_T *ancs = peer->ancs;

    if (!peer || !ancs) return;

    if (ancs->noti_src) FreeMem(ancs->noti_src);

    if (ancs->ctrl_pt) FreeMem(ancs->ctrl_pt);

    if (ancs->data_src) FreeMem(ancs->data_src);

    LeQueueBroken(&ancs->ansl);
    LeQueueBroken(&ancs->cmd_list);
    
    if (ancs->cmd) FreeMem(ancs->cmd);

    FreeMem(ancs);
    peer->ancs = 0;
}

void BleAppFindAncsChar(BLE_APP_MSG_COMMON_T *req)
{
    BLE_APP_PEER_T *peer = BleAppFindPeer(req->conn_hdl);
    LE_ERR_STATE rc = SYS_ERR_SUCCESS;
    BLE_SERVICE_T *svc = peer->svc;
    UINT8 ancsSvcUuid[16] = {ANCS_SVC_UUID128};
    UINT16 i;

    if (!peer || peer->action) return;
        
    for (i = 0; i < peer->svc_idx; i++)
    {
        if ((svc[i].format == LE_GATT_UUID128) && !MemCmp(ancsSvcUuid, svc[i].uuid, 16))
        {
            rc = LeGattFindAllCharacteristic(peer->conn_hdl, svc[i].start_hdl + 1, svc[i].end_hdl);

            if (rc == SYS_ERR_SUCCESS) 
            {
                BLE_ANCS_INFO_T *ancs = PanicUnlessMalloc(sizeof(BLE_ANCS_INFO_T));

                peer->action = GATT_ACTION_WAIT_CHAR_INFO;
                peer->ancs = ancs;
                ancs->svc = &svc[i];
                svc[i].action = GATT_ACTION_WAIT_CHAR_INFO;
            }
        
            break;
        }
    }
}

UINT16 BleAppAncsRwCccdValReq(UINT16 conn_hdl, UINT8 op, UINT16 val)
{
    BLE_APP_PEER_T *peer = BleAppFindPeer(conn_hdl);
    ANCS_WR_CCCD_VAL_T *req;

    if (!peer || !peer->ancs) return SYS_ERR_FAIL;
    
    req = Malloc(sizeof(ANCS_WR_CCCD_VAL_T));

    if (!req) return SYS_ERR_OUT_OF_MEM;

    req->hdr.op = op;
    req->val = val;

    LeQueuePush(&peer->rw_req, req);

    BleAppSendCommonMsg(BLE_APP_MSG_RW_REQ, conn_hdl, 0);

    return SYS_ERR_SUCCESS;
}

BOOL BleAppAncsGetNotifyAttr(UINT16 conn_hdl, UINT32 uid, UINT16 attr_id_flag)
{
    BLE_APP_PEER_T *peer = BleAppFindPeer(conn_hdl);
    BLE_ANCS_INFO_T *ancs = peer->ancs;
    ANCS_GET_NOTI_ATTR_CMD_T *ntCmd;
    UINT8 cmd[30];
    UINT8 *p = cmd;
    
    if (!peer || !ancs || !ancs->ctrl_pt) return FALSE;

    *p++ = CommandIDGetNotificationAttributes;
    MemCopy(p, &uid, 4);
    p += 4;

    if (attr_id_flag & (1 << NotificationAttributeIDAppIdentifier)) BleAppBuildGetNotifyAttrCmdData(&p, NotificationAttributeIDAppIdentifier, 0);

    if (attr_id_flag & (1 << NotificationAttributeIDTitle)) BleAppBuildGetNotifyAttrCmdData(&p, NotificationAttributeIDTitle, 30);

    if (attr_id_flag & (1 << NotificationAttributeIDSubtitle)) BleAppBuildGetNotifyAttrCmdData(&p, NotificationAttributeIDSubtitle, 20);

    if (attr_id_flag & (1 << NotificationAttributeIDMessageSize)) BleAppBuildGetNotifyAttrCmdData(&p, NotificationAttributeIDMessageSize, 0);

    if (attr_id_flag & (1 << NotificationAttributeIDMessage)) BleAppBuildGetNotifyAttrCmdData(&p, NotificationAttributeIDMessage, 100);

    if (attr_id_flag & (1 << NotificationAttributeIDDate)) BleAppBuildGetNotifyAttrCmdData(&p, NotificationAttributeIDDate, 0);

    if (attr_id_flag & (1 << NotificationAttributeIDPositiveActionLabel)) BleAppBuildGetNotifyAttrCmdData(&p, NotificationAttributeIDPositiveActionLabel, 0);

    if (attr_id_flag & (1 << NotificationAttributeIDNegativeActionLabel)) BleAppBuildGetNotifyAttrCmdData(&p, NotificationAttributeIDNegativeActionLabel, 0);

    ntCmd = Malloc(sizeof(ANCS_GET_NOTI_ATTR_CMD_T));

    if (!ntCmd) return FALSE;

    ntCmd->hdr.op = ANCS_WR_CTRL_PT_CMD;

    ntCmd->cmd_id = CommandIDGetNotificationAttributes;
    ntCmd->attr_id_flag = attr_id_flag & 0xFF;
    ntCmd->notify_uid = uid;
    ntCmd->chk_idx = 0;
    
    ntCmd->rsp_len = p - cmd;
    MemCopy(ntCmd->rsp_buf, cmd, ntCmd->rsp_len);
    LeQueuePush(&peer->rw_req, ntCmd);
    
    BleAppSendCommonMsg(BLE_APP_MSG_RW_REQ, conn_hdl, 0);

    return TRUE;
}

BOOL BleAppAncsGetAppAttr(UINT16 conn_hdl, UINT32 uid)
{
    BLE_APP_PEER_T *peer = BleAppFindPeer(conn_hdl);
    BLE_ANCS_INFO_T *ancs = peer->ancs;
    ANCS_NOTIFY_SRC_DATA_T *ansd;
    ANCS_GET_APP_ATTR_CMD_T *appCmd;
    UINT8 *p;
    
    if (!peer || !ancs || !ancs->ctrl_pt) return FALSE;

    ansd = BleAppAncsNotifySrcFindByUid(peer, uid);

    if (!ansd) return FALSE;

    if (!(ansd->appid_flag & 0x0100)) return FALSE;

    appCmd = Malloc(sizeof(ANCS_GET_APP_ATTR_CMD_T));

    if (!appCmd) return FALSE;
    
    MemSet(appCmd, 0, sizeof(ANCS_GET_APP_ATTR_CMD_T));

    appCmd->hdr.op = ANCS_WR_CTRL_PT_CMD;
    appCmd->cmd_id = CommandIDGetAppAttributes;
    appCmd->attr_id_flag = 1;
    appCmd->uid = uid;
    appCmd->appid_len = ansd->appid_flag & 0xFF;
    appCmd->chk_idx = 0;
    
    p = appCmd->rsp_buf;

    *p++ = CommandIDGetAppAttributes;
    MemCopy(p, ansd->appid, appCmd->appid_len);
    p += appCmd->appid_len;
    *p++ = 0;
    *p++ = AppAttributeIDDisplayName;
    
    appCmd->rsp_len = p - appCmd->rsp_buf;
    
    LeQueuePush(&peer->rw_req, appCmd);

    BleAppSendCommonMsg(BLE_APP_MSG_RW_REQ, conn_hdl, 0);

    return TRUE;
}

BOOL BleAppAncsNotifyAction(UINT16 conn_hdl, UINT32 uid, UINT8 action)
{
    BLE_APP_PEER_T *peer = BleAppFindPeer(conn_hdl);
    BLE_ANCS_INFO_T *ancs = peer->ancs;
    ANCS_NOTIFY_SRC_DATA_T *ansd;
    ANCS_NOTI_ACTION_CMD_T *notiCmd;
    UINT8 *p;
    
    if (!peer || !ancs || !ancs->ctrl_pt) return FALSE;

    if (action > ActionIDNegative) return FALSE;

    ansd = BleAppAncsNotifySrcFindByUid(peer, uid);

    if (!ansd) return FALSE;

    notiCmd = Malloc(sizeof(ANCS_NOTI_ACTION_CMD_T));

    if (!notiCmd) return FALSE;
    
    MemSet(notiCmd, 0, sizeof(ANCS_NOTI_ACTION_CMD_T));

    notiCmd->hdr.op = ANCS_WR_CTRL_PT_CMD;
    notiCmd->cmd_id = CommandIDPerformNotificationAction;
    notiCmd->uid = uid;
    notiCmd->chk_idx = 0;
    
    p = notiCmd->rsp_buf;

    *p++ = CommandIDPerformNotificationAction;
    MemCopy(p, &uid, 4);
    p += 4;
    *p++ = action;
    
    notiCmd->rsp_len = p - notiCmd->rsp_buf;
    
    LeQueuePush(&peer->rw_req, notiCmd);

    BleAppSendCommonMsg(BLE_APP_MSG_RW_REQ, conn_hdl, 0);

    return TRUE;
}

BOOL BleAppGattAncsRwReq(UINT16 conn_hdl, void *hdr)
{
    BLE_APP_PEER_T *peer = BleAppFindPeer(conn_hdl);
    GATT_REQ_HDR_T *gattRw = hdr;
    BLE_ANCS_INFO_T *ancs = peer->ancs;
    LE_ERR_STATE rc = SYS_ERR_FAIL;

    if (!peer || peer->action || !ancs) return FALSE;

    if (gattRw->op >= ANCS_OP_TOP) return FALSE;
    
    switch (gattRw->op)
    {
        case ANCS_WR_CTRL_PT_CMD:
        {
            ANCS_CHAR_T *cha = ancs->ctrl_pt;

            if (cha && cha->decl.val_hdl)
            {
                ANCS_CMD_HDR_T *cmdHdr = hdr;
                
                if (cmdHdr->cmd_id == CommandIDGetNotificationAttributes)
                {
                    ANCS_GET_NOTI_ATTR_CMD_T *req = hdr;

                    rc = LeGattWriteCharVal(conn_hdl, cha->decl.val_hdl, req->rsp_len, req->rsp_buf);
                }
                else if (cmdHdr->cmd_id == CommandIDGetAppAttributes)
                {
                    ANCS_GET_APP_ATTR_CMD_T *req = hdr;
                
                    rc = LeGattWriteCharVal(conn_hdl, cha->decl.val_hdl, req->rsp_len, req->rsp_buf);
                }
                else if (cmdHdr->cmd_id == CommandIDPerformNotificationAction)
                {
                    ANCS_NOTI_ACTION_CMD_T *req = hdr;
                
                    rc = LeGattWriteCharVal(conn_hdl, cha->decl.val_hdl, req->rsp_len, req->rsp_buf);
                }
            }
        }
        break;
    
        case ANCS_RD_NOTI_SRC_CCCD_VAL:
        {
            ANCS_CHAR_T *cha = ancs->noti_src;
            
            if (cha && cha->cccd->handle) rc = LeGattReadCharValue(conn_hdl, cha->cccd->handle);
        }
        break;

        case ANCS_RD_DATA_SRC_CCCD_VAL:
        {
            ANCS_CHAR_T *cha = ancs->data_src;
            
            if (cha && cha->cccd->handle) rc = LeGattReadCharValue(conn_hdl, cha->cccd->handle);
        }
        break;

        case ANCS_WR_NOTI_SRC_CCCD_VAL:
        {
            ANCS_CHAR_T *cha = ancs->noti_src;
            
            if (cha && cha->cccd->handle)
            {
                ANCS_WR_CCCD_VAL_T *req = hdr;

                rc = LeGattWriteCharVal(conn_hdl, cha->cccd->handle, 2, (UINT8 *)&req->val);
            }
        }
        break;

        case ANCS_WR_DATA_SRC_CCCD_VAL:
        {
            ANCS_CHAR_T *cha = ancs->data_src;
            
            if (cha && cha->cccd->handle)
            {
                ANCS_WR_CCCD_VAL_T *req = hdr;

                rc = LeGattWriteCharVal(conn_hdl, cha->cccd->handle, 2, (UINT8 *)&req->val);
            }
        }
        break;

        default:
        break;
    }

    if (!rc) 
    {
        if (gattRw->op == ANCS_WR_CTRL_PT_CMD)
            peer->action = GATT_ACTION_WR_ANCS_CMD;
        else
            peer->action = GATT_ACTION_RW_CCCD_VAL;
        
        return TRUE;
    }

    return FALSE;
}

