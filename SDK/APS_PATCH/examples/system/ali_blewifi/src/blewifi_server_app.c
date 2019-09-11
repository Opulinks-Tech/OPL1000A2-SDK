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

#include "mw_fim_default_group11_project.h"

#ifdef ALI_BLE_WIFI_PROVISION
#include "core.h"
#include "breeze_export.h"
#include "breeze_hal_ble.h"
#include "breeze_hal_sec.h"
#include "extcmd.h"
#include "utils.h"
#include "awss_notify.h"
#include "awss_cmp.h"
#include "cmsis_os.h"
#include "ble_gatt_if.h"
#include "transport.h"

UINT8 g_icnt=0;
#endif

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
#ifdef ALI_BLE_WIFI_PROVISION
static void BleWifi_Ble_AppMsgHandler_AliSendRandom(TASK task, MESSAGEID id, MESSAGE message);
static void BleWifi_Ble_AppMsgHandler_AliSendDeviceName(TASK task, MESSAGEID id, MESSAGE message);
static void BleWifi_Ble_AppMsgHandler_AliSendDeviceSecret(TASK task, MESSAGEID id, MESSAGE message);
static void BleWifi_Ble_AppMsgHandler_AliSendHiClient(TASK task, MESSAGEID id, MESSAGE message);
#endif
static T_BleWifi_Ble_MsgHandlerTbl gBleAppMsgHandlerTbl[] =
{
    {BLEWIFI_APP_MSG_INITIALIZING,              BleWifi_Ble_AppMsgHandler_Initializing},
    {BLEWIFI_APP_MSG_ENTER_ADVERTISING,         BleWifi_Ble_AppMsgHandler_EnterAdvertising},
    {BLEWIFI_APP_MSG_EXIT_ADVERTISING,          BleWifi_Ble_AppMsgHandler_ExitAdvertising},
    {BLEWIFI_APP_MSG_CHANGE_ADVERTISING_TIME,   BleWifi_Ble_AppMsgHandler_ChangeAdvertisingTime},
    {BLEWIFI_APP_MSG_SEND_DATA,                 BleWifi_Ble_AppMsgHandler_SendData},
    {BLEWIFI_APP_MSG_SEND_TO_PEER,              BleWifi_Ble_AppMsgHandler_SendToPeer},
    {BLEWIFI_APP_MSG_SEND_TO_PEER_CFM,          BleWifi_Ble_AppMsgHandler_SendToPeerCfm},
#ifdef ALI_BLE_WIFI_PROVISION		
    {BLEWIFI_APP_MSG_ALI_SEND_RANDOM,           BleWifi_Ble_AppMsgHandler_AliSendRandom},
    {BLEWIFI_APP_MSG_ALI_SEND_DEVICE_NAME,      BleWifi_Ble_AppMsgHandler_AliSendDeviceName},
    {BLEWIFI_APP_MSG_ALI_SEND_DEVICE_SECRET,    BleWifi_Ble_AppMsgHandler_AliSendDeviceSecret},
    {BLEWIFI_APP_MSG_ALI_SEND_HICLIENT,         BleWifi_Ble_AppMsgHandler_AliSendHiClient},
 #endif

    {0xFFFFFFFF,                                NULL}
};
#ifdef ALI_BLE_WIFI_PROVISION		
ali_init_t init_ali;
extern auth_t g_auth;
extern breeze_dev_info_t dinfo;

uint32_t tx_func_indicate(uint8_t cmd, uint8_t *p_data, uint16_t length)
{
	reset_tx();
    return transport_tx(TX_INDICATION, cmd, p_data, length, LeGattCharValIndicate, gTheBle.conn_hdl, gTheBle.ali_indhdl);
}

uint32_t tx_func_notify(uint8_t cmd, uint8_t *p_data, uint16_t length)
{
	reset_tx();
    return transport_tx(TX_NOTIFICATION, cmd, p_data, length, LeGattCharValNotify, gTheBle.conn_hdl, gTheBle.ali_notihdl);
}

#endif
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
#ifndef ALI_BLE_WIFI_PROVISION	
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
#endif


#ifdef ALI_BLE_WIFI_PROVISION		

#define MAX_ADV_DATA_LEN 16
#define BT_DATA_FLAGS                   0x01 /* AD flags */
#define BT_DATA_UUID16_SOME             0x02 /* 16-bit UUID, more available */
#define BT_DATA_UUID16_ALL              0x03 /* 16-bit UUID, all listed */
#define BT_DATA_UUID32_SOME             0x04 /* 32-bit UUID, more available */
#define BT_DATA_UUID32_ALL              0x05 /* 32-bit UUID, all listed */
#define BT_DATA_UUID128_SOME            0x06 /* 128-bit UUID, more available */
#define BT_DATA_UUID128_ALL             0x07 /* 128-bit UUID, all listed */
#define BT_DATA_NAME_SHORTENED          0x08 /* Shortened name */
#define BT_DATA_NAME_COMPLETE           0x09 /* Complete name */

#define AD_LEN 3
#define SD_LEN 1

#define FMSK_BLUETOOTH_VER_Pos 0
#define FMSK_OTA_Pos 2
#define FMSK_SECURITY_Pos 3
#define FMSK_SECRET_TYPE_Pos 4
#define FMSK_SIGNED_ADV_Pos 5

#define BZ_PROTOCOL_ID 0x05
#define MAC_ASCII_LEN 6

struct bt_data {
	uint8_t type;
	uint8_t data_len;
	const uint8_t *data;
};

#define MAX_ADV_DATA_LEN 16


extern core_t g_core;
extern void create_bz_adv_data(uint32_t model_id, uint8_t *mac_bin);

int ble_advertising_start(ais_adv_init_t *adv)
{
//    int            err=0;
    uint8_t        flag = 0, srv[] = { 0xb3, 0xfe }, ad_len = AD_LEN;//, sd_len = SD_LEN;
    static struct bt_data ad[AD_LEN];
    static struct bt_data sd[SD_LEN];
		
    memset(ad,0,sizeof(struct bt_data)*AD_LEN);

	

    if (adv->flag & AIS_AD_GENERAL) {
        flag |= GAP_ADTYPE_FLAGS_GENERAL;
    }
    if (adv->flag & AIS_AD_NO_BREDR) {
        flag |= GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED;
    }
 /*   if (!flag) {
        BZ_LOG_E("Invalid adv flag");
        return AIS_ERR_INVALID_ADV_DATA;
    }
*/
	

    ad[0].type     = BT_DATA_FLAGS;
    ad[0].data     = &flag;
    ad[0].data_len = 1;

    ad[1].type     = BT_DATA_UUID16_ALL;
    ad[1].data     = srv;
    ad[1].data_len = sizeof(srv);

    if (adv->vdata.len != 0) {
        ad[2].type     = 0xFF;
        ad[2].data_len = adv->vdata.len;
        ad[2].data     = adv->vdata.data;
    } else {
        ad_len--;
    }


    UINT8 bleAdvertData[] ={
    0x02,
    BT_DATA_FLAGS,
    0x06 ,
    0x03,
    BT_DATA_UUID16_ALL,
    0xb3, 0xfe,
    adv->vdata.len+1,
    0xFF,
    };
    
    uint8_t ubLen = sizeof(bleAdvertData);
    gTheBle.adv_data.len = ubLen;
    MemCopy(gTheBle.adv_data.buf, bleAdvertData, ubLen);
    MemCopy(gTheBle.adv_data.buf+ubLen, adv->vdata.data,adv->vdata.len);
    gTheBle.adv_data.len+=adv->vdata.len;
    
        
    printf("ALi BLE ADV data:\n");
    for(int i=0;i<gTheBle.adv_data.len;i++)
    {
        printf("%02x", gTheBle.adv_data.buf[i]);
    }
    printf("\n");
    
    switch (adv->name.ntype) {
        case AIS_ADV_NAME_SHORT:
            sd[0].type = BT_DATA_NAME_SHORTENED;
            break;
        case AIS_ADV_NAME_FULL:
            sd[0].type = BT_DATA_NAME_COMPLETE;
            break;
        default:
//           BZ_LOG_E("Invalid adv name type.");
            return 114;
    }
    
/*   if (adv->name.name == NULL) {
        BZ_LOG_E("Invalid adv name.");
        return AIS_ERR_INVALID_ADV_DATA;
    }
*/
    sd[0].data     = (const uint8_t*)adv->name.name;
    sd[0].data_len = strlen(adv->name.name);

    gTheBle.scn_data.len = sd[0].data_len + 2;
    gTheBle.scn_data.buf[0] = gTheBle.scn_data.len - 1;
    gTheBle.scn_data.buf[1] = sd[0].type;
    MemCopy(gTheBle.scn_data.buf + 2, sd[0].data, sd[0].data_len);

		
    printf("ALi BLE SCN data:\n");
    for(int i=0;i<gTheBle.scn_data.len;i++)
    {
        printf("%02x", gTheBle.scn_data.buf[i]);
    }
    printf("\n");
	
	/*

    err = bt_le_adv_start(BT_LE_ADV_CONN, ad, ad_len, sd, sd_len);
    if (err) {
        BZ_LOG_E("Failed to start adv (err %d)", err);
        return AIS_ERR_ADV_FAIL;
    }
*/
    return 0;
}

#endif



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
#ifdef ALI_BLE_WIFI_PROVISION
    ais_adv_init_t adv_data = {
        .flag = (ais_adv_flag_t)(AIS_AD_GENERAL | AIS_AD_NO_BREDR),
        .name = { .ntype = AIS_ADV_NAME_FULL, .name = "AZ" },
    };

	/* device key may be NULL */
    if (dinfo.device_name != NULL) {
    init_ali.device_key.length = strlen(dinfo.device_name);
    init_ali.device_key.p_data = (uint8_t*)dinfo.device_name;
            g_auth.key_len = strlen(dinfo.device_name);
    if(g_auth.key_len > 0){
        memcpy(g_auth.key, dinfo.device_name, g_auth.key_len);
    }
//  printf("Device key=%s\r\n", g_auth.key);
//  printf("Device key=%s\r\n", init_ali.device_key.p_data );
    } else {
        init_ali.device_key.length = 0;
    }
	
    /* device secret may be NULL */
    if (dinfo.device_secret != NULL) {
    init_ali.secret.length = strlen(dinfo.device_secret);
    init_ali.secret.p_data = (uint8_t *)dinfo.device_secret;
            
    } else {
    init_ali.secret.length = 0;
    }


//  g_dev_conf->product_secret_len = strlen(dinfo.product_secret);
//  memcpy(g_dev_conf->product_secret, dinfo.product_secret, strlen(dinfo.product_secret));
    
//  init_ali.event_handler = event_handler;
    init_ali.model_id = dinfo.product_id;

    init_ali.product_key.p_data = (uint8_t *)dinfo.product_key;
    init_ali.product_key.length = strlen(dinfo.product_key);;

    init_ali.product_secret.p_data = (uint8_t *)dinfo.product_secret;
    init_ali.product_secret.length = strlen(dinfo.product_secret);
    init_ali.transport_timeout     = BZ_TRANSPORT_TIMEOUT;
    init_ali.max_mtu               = BZ_MAX_SUPPORTED_MTU;
//    init_ali.user_adv_data         = user_adv.data;
//    init_ali.user_adv_len          = user_adv.len;

#ifdef ALI_OPL_DBG		 
    printf("product secret len: %d\r\n", strlen(dinfo.product_secret));
    printf("product secret: %s\r\n", init_ali.product_secret.p_data);
    printf("product key len: %d\r\n", init_ali.product_key.length);
    printf("product key: %s\r\n", init_ali.product_key.p_data);
    printf("device key len: %d\r\n", init_ali.device_key.length);
    printf("device key: %s\r\n", init_ali.device_key.p_data);
    printf("device secret len: %d\r\n", init_ali.secret.length);
    printf("device secret: %s\r\n", init_ali.secret.p_data);
    printf("product id: %d\r\n", init_ali.model_id);
#endif	
	transport_init((ali_init_t const *)&init_ali);
	uint32_t model_id=init_ali.model_id;
	unsigned char mac_be[6]={0x78,0x78,0x78,0x78,0x78,0x78};
	LeGapGetBdAddr(mac_be);
	create_bz_adv_data(model_id, mac_be);
    adv_data.vdata.len = sizeof(adv_data.vdata.data);
    if (get_bz_adv_data(adv_data.vdata.data, &(adv_data.vdata.len))) {
//      BREEZE_LOG_ERR("%s %d fail.\r\n", __func__, __LINE__);
//      err = AIS_ERR_INVALID_ADV_DATA;
        goto exit;
   }

/*
	memcpy(gTheBle.adv_data.buf + gTheBle.adv_data.len,adv_data.vdata.data, adv_data.vdata.len);
    gTheBle.adv_data.len+=adv_data.vdata.len;
*/
	
	
	ble_advertising_start(&adv_data);

//	memcpy(adv_data.vdata.data + adv_data.vdata.len,
 //                      g_ali_init->user_adv_data, g_ali_init->user_adv_len);
 //               adv_data.vdata.len += g_ali_init->user_adv_len;
#endif	
    LeGapSetAdvData(gTheBle.adv_data.len, gTheBle.adv_data.buf);
#ifdef ALI_BLE_WIFI_PROVISION
exit:
    return;
#endif	
}

static void BleWifi_Ble_SetScanData(void)
{
#ifndef ALI_BLE_WIFI_PROVISION	
    T_MwFim_GP11_BleDeviceName tBleDeviceName;    
    uint8_t ubLen;
    BOOL isOk = FALSE;

    // get the settings of BLE device name
    if (MW_FIM_OK != MwFim_FileRead(MW_FIM_IDX_GP11_PROJECT_BLE_DEVICE_NAME, 0, MW_FIM_GP11_BLE_DEVICE_NAME_SIZE, (uint8_t*)&tBleDeviceName))
    {
        // if fail, get the default value
        memcpy(&tBleDeviceName, &g_tMwFimDefaultGp11BleDeviceName, MW_FIM_GP11_BLE_DEVICE_NAME_SIZE);
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
#endif
    LeSetScanRspData(gTheBle.scn_data.len, gTheBle.scn_data.buf);
    BleWifi_Ble_AppUpdateDevName(gTheBle.scn_data.buf + 2, gTheBle.scn_data.len - 2);
}

static void BleWifi_Ble_CmMsgHandler_InitCompleteCfm(TASK task, MESSAGEID id, MESSAGE message)
{
    T_MwFim_GP11_BleAdvInterval tBleAdvInterval;

    // !!! after LeCmInit
    BLEWIFI_INFO("APP-LE_CM_MSG_INIT_COMPLETE_CFM\r\n");

    // get the settings of BLE advertisement interval
    if (MW_FIM_OK != MwFim_FileRead(MW_FIM_IDX_GP11_PROJECT_BLE_ADV_INTERVAL, 0, MW_FIM_GP11_BLE_ADV_INTERVAL_SIZE, (uint8_t*)&tBleAdvInterval))
    {
        // if fail, get the default value
        memcpy(&tBleAdvInterval, &g_tMwFimDefaultGp11BleAdvInterval, MW_FIM_GP11_BLE_ADV_INTERVAL_SIZE);
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
#ifdef ALI_BLE_WIFI_PROVISION
        BleWifi_Ctrl_EventStatusSet(BLEWIFI_CTRL_EVENT_BIT_LINK_CONN, false);
        transport_reset();
        ali_auth_init(&init_ali, tx_func_indicate);
        extcmd_init(&init_ali, tx_func_indicate);
        auth_connected();
        g_icnt=0;
#endif
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
#ifdef ALI_BLE_WIFI_PROVISION
     transport_reset();
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

#ifdef ALI_BLE_WIFI_PROVISION
static void BleWifi_Ble_AppMsgHandler_AliSendRandom(TASK task, MESSAGEID id, MESSAGE message)
{
    if (gTheBle.state == APP_STATE_CONNECTED)
    {
        g_auth.state = AUTH_STATE_SVC_ENABLED;
//        printf("send_random=%s\r\n", g_auth.ikm + g_auth.ikm_len);
        reset_tx();
        transport_tx(TX_INDICATION, BZ_CMD_AUTH_RAND, g_auth.ikm + g_auth.ikm_len, RANDOM_SEQ_LEN, LeGattCharValIndicate, gTheBle.conn_hdl, gTheBle.ali_indhdl);
        auth_tx_done();
    }
}
static void BleWifi_Ble_AppMsgHandler_AliSendDeviceName(TASK task, MESSAGEID id, MESSAGE message)
{
    if (gTheBle.state == APP_STATE_CONNECTED)
    {
        reset_tx();
//        printf("send Device key: %s\r\n", g_auth.key);
        transport_tx(TX_NOTIFICATION, BZ_CMD_AUTH_KEY, g_auth.key, strlen((const char*)g_auth.key), LeGattCharValNotify, gTheBle.conn_hdl, gTheBle.ali_notihdl);
//        auth_tx_done();
    }
}
static void BleWifi_Ble_AppMsgHandler_AliSendDeviceSecret(TASK task, MESSAGEID id, MESSAGE message)
{
    if (gTheBle.state == APP_STATE_CONNECTED)
    {
        reset_tx();            	
//        printf("send Device secret: %s\r\n", g_auth.ikm + g_auth.ikm_len);
//        transport_tx(TX_INDICATION, BZ_CMD_AUTH_RAND,  g_auth.ikm + g_auth.ikm_len, RANDOM_SEQ_LEN, LeGattCharValIndicate, gTheBle.conn_hdl, gTheBle.ali_indhdl);
//        auth_tx_done();
    }
}
static void BleWifi_Ble_AppMsgHandler_AliSendHiClient(TASK task, MESSAGEID id, MESSAGE message)
{
    if (gTheBle.state == APP_STATE_CONNECTED)
    {
        BLEWIFI_MESSAGE_T *p_data = (BLEWIFI_MESSAGE_T *)message;
        transport_rx(p_data->data, p_data->len);
    }
}
#endif
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
