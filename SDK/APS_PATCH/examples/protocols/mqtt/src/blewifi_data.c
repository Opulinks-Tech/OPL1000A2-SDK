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

/**
 * @file blewifi_data.c
 * @author Michael Liao
 * @date 20 Feb 2018
 * @brief File creates the wifible_data task architecture.
 *
 */

#include "sys_os_config.h"
#include "blewifi_app.h"
#include "blewifi_data.h"
#include "blewifi_ctrl.h"
#include "blewifi_server_app.h"
#include "blewifi_wifi_api.h"
#include "blewifi_ble_api.h"
#include "wifi_api.h"
#include "lwip/netif.h"
#include "mw_ota.h"
#include "blewifi_ctrl_http_ota.h"
#include "hal_auxadc_patch.h"
#include "hal_system.h"
#include "mw_fim_default_group03.h"
#include "mw_fim_default_group03_patch.h"
#include "at_cmd_common.h"

#define HI_UINT16(a) (((a) >> 8) & 0xFF)
#define LO_UINT16(a) ((a) & 0xFF)

typedef struct {
    uint16_t total_len;
    uint16_t remain;
    uint16_t offset;
    uint8_t *aggr_buf;
} blewifi_rx_packet_t;

blewifi_rx_packet_t g_rx_packet = {0};

static void BleWifi_Ble_ProtocolHandler_Scan(uint16_t type, uint8_t *data, int len);
static void BleWifi_Ble_ProtocolHandler_Connect(uint16_t type, uint8_t *data, int len);
static void BleWifi_Ble_ProtocolHandler_Disconnect(uint16_t type, uint8_t *data, int len);
static void BleWifi_Ble_ProtocolHandler_Reconnect(uint16_t type, uint8_t *data, int len);
static void BleWifi_Ble_ProtocolHandler_ReadDeviceInfo(uint16_t type, uint8_t *data, int len);
static void BleWifi_Ble_ProtocolHandler_WriteDeviceInfo(uint16_t type, uint8_t *data, int len);
static void BleWifi_Ble_ProtocolHandler_WifiStatus(uint16_t type, uint8_t *data, int len);
static void BleWifi_Ble_ProtocolHandler_Reset(uint16_t type, uint8_t *data, int len);

#if (BLE_OTA_FUNCTION_EN == 1)
static void BleWifi_Ble_ProtocolHandler_OtaVersion(uint16_t type, uint8_t *data, int len);
static void BleWifi_Ble_ProtocolHandler_OtaUpgrade(uint16_t type, uint8_t *data, int len);
static void BleWifi_Ble_ProtocolHandler_OtaRaw(uint16_t type, uint8_t *data, int len);
static void BleWifi_Ble_ProtocolHandler_OtaEnd(uint16_t type, uint8_t *data, int len);
#endif

#if (WIFI_OTA_FUNCTION_EN == 1)
static void BleWifi_Ble_ProtocolHandler_HttpOtaTrig(uint16_t type, uint8_t *data, int len);
static void BleWifi_Ble_ProtocolHandler_HttpOtaDeviceVersion(uint16_t type, uint8_t *data, int len);
static void BleWifi_Ble_ProtocolHandler_HttpOtaServerVersion(uint16_t type, uint8_t *data, int len);
#endif

static void BleWifi_Ble_ProtocolHandler_MpCalVbat(uint16_t type, uint8_t *data, int len);
static void BleWifi_Ble_ProtocolHandler_MpCalIoVoltage(uint16_t type, uint8_t *data, int len);
static void BleWifi_Ble_ProtocolHandler_MpCalTmpr(uint16_t type, uint8_t *data, int len);
static void BleWifi_Ble_ProtocolHandler_MpSysModeWrite(uint16_t type, uint8_t *data, int len);
static void BleWifi_Ble_ProtocolHandler_MpSysModeRead(uint16_t type, uint8_t *data, int len);
static void BleWifi_Ble_ProtocolHandler_EngSysReset(uint16_t type, uint8_t *data, int len);
static void BleWifi_Ble_ProtocolHandler_EngWifiMacWrite(uint16_t type, uint8_t *data, int len);
static void BleWifi_Ble_ProtocolHandler_EngWifiMacRead(uint16_t type, uint8_t *data, int len);
static void BleWifi_Ble_ProtocolHandler_EngBleMacWrite(uint16_t type, uint8_t *data, int len);
static void BleWifi_Ble_ProtocolHandler_EngBleMacRead(uint16_t type, uint8_t *data, int len);
static void BleWifi_Ble_ProtocolHandler_EngBleCmd(uint16_t type, uint8_t *data, int len);
static T_BleWifi_Ble_ProtocolHandlerTbl g_tBleProtocolHandlerTbl[] =
{
    {BLEWIFI_REQ_SCAN,                      BleWifi_Ble_ProtocolHandler_Scan},
    {BLEWIFI_REQ_CONNECT,                   BleWifi_Ble_ProtocolHandler_Connect},
    {BLEWIFI_REQ_DISCONNECT,                BleWifi_Ble_ProtocolHandler_Disconnect},
    {BLEWIFI_REQ_RECONNECT,                 BleWifi_Ble_ProtocolHandler_Reconnect},
    {BLEWIFI_REQ_READ_DEVICE_INFO,          BleWifi_Ble_ProtocolHandler_ReadDeviceInfo},
    {BLEWIFI_REQ_WRITE_DEVICE_INFO,         BleWifi_Ble_ProtocolHandler_WriteDeviceInfo},
    {BLEWIFI_REQ_WIFI_STATUS,               BleWifi_Ble_ProtocolHandler_WifiStatus},
    {BLEWIFI_REQ_RESET,                     BleWifi_Ble_ProtocolHandler_Reset},

#if (BLE_OTA_FUNCTION_EN == 1)
    {BLEWIFI_REQ_OTA_VERSION,               BleWifi_Ble_ProtocolHandler_OtaVersion},
    {BLEWIFI_REQ_OTA_UPGRADE,               BleWifi_Ble_ProtocolHandler_OtaUpgrade},
    {BLEWIFI_REQ_OTA_RAW,                   BleWifi_Ble_ProtocolHandler_OtaRaw},
    {BLEWIFI_REQ_OTA_END,                   BleWifi_Ble_ProtocolHandler_OtaEnd},
#endif

#if (WIFI_OTA_FUNCTION_EN == 1)
    {BLEWIFI_REQ_HTTP_OTA_TRIG,             BleWifi_Ble_ProtocolHandler_HttpOtaTrig},
    {BLEWIFI_REQ_HTTP_OTA_DEVICE_VERSION,   BleWifi_Ble_ProtocolHandler_HttpOtaDeviceVersion},
    {BLEWIFI_REQ_HTTP_OTA_SERVER_VERSION,   BleWifi_Ble_ProtocolHandler_HttpOtaServerVersion},
#endif

    {BLEWIFI_REQ_MP_CAL_VBAT,               BleWifi_Ble_ProtocolHandler_MpCalVbat},
    {BLEWIFI_REQ_MP_CAL_IO_VOLTAGE,         BleWifi_Ble_ProtocolHandler_MpCalIoVoltage},
    {BLEWIFI_REQ_MP_CAL_TMPR,               BleWifi_Ble_ProtocolHandler_MpCalTmpr},
    {BLEWIFI_REQ_MP_SYS_MODE_WRITE,         BleWifi_Ble_ProtocolHandler_MpSysModeWrite},
    {BLEWIFI_REQ_MP_SYS_MODE_READ,          BleWifi_Ble_ProtocolHandler_MpSysModeRead},
    
    {BLEWIFI_REQ_ENG_SYS_RESET,             BleWifi_Ble_ProtocolHandler_EngSysReset},
    {BLEWIFI_REQ_ENG_WIFI_MAC_WRITE,        BleWifi_Ble_ProtocolHandler_EngWifiMacWrite},
    {BLEWIFI_REQ_ENG_WIFI_MAC_READ,         BleWifi_Ble_ProtocolHandler_EngWifiMacRead},
    {BLEWIFI_REQ_ENG_BLE_MAC_WRITE,         BleWifi_Ble_ProtocolHandler_EngBleMacWrite},
    {BLEWIFI_REQ_ENG_BLE_MAC_READ,          BleWifi_Ble_ProtocolHandler_EngBleMacRead},
    {BLEWIFI_REQ_ENG_BLE_CMD,               BleWifi_Ble_ProtocolHandler_EngBleCmd},
    
    {0xFFFFFFFF,                            NULL}
};

#if (BLE_OTA_FUNCTION_EN == 1)
static void BleWifi_OtaSendVersionRsp(uint8_t status, uint16_t pid, uint16_t cid, uint16_t fid)
{
	uint8_t data[7];
	uint8_t *p = (uint8_t *)data;

	*p++ = status;
	*p++ = LO_UINT16(pid);
	*p++ = HI_UINT16(pid);
	*p++ = LO_UINT16(cid);
	*p++ = HI_UINT16(cid);
	*p++ = LO_UINT16(fid);
	*p++ = HI_UINT16(fid);

	BleWifi_Ble_DataSendEncap(BLEWIFI_RSP_OTA_VERSION, data, 7);
}

static void BleWifi_OtaSendUpgradeRsp(uint8_t status)
{
	BleWifi_Ble_DataSendEncap(BLEWIFI_RSP_OTA_UPGRADE, &status, 1);
}

static void BleWifi_OtaSendEndRsp(uint8_t status, uint8_t stop)
{
	BleWifi_Ble_DataSendEncap(BLEWIFI_RSP_OTA_END, &status, 1);

    if (stop)
    {
        if (gTheOta)
        {
            if (status != BLEWIFI_OTA_SUCCESS)
                MwOta_DataGiveUp();
            free(gTheOta);
            gTheOta = 0;

            if (status != BLEWIFI_OTA_SUCCESS)
                BleWifi_Ctrl_MsgSend(BLEWIFI_CTRL_MSG_OTHER_OTA_OFF_FAIL, NULL, 0);
            else
                BleWifi_Ctrl_MsgSend(BLEWIFI_CTRL_MSG_OTHER_OTA_OFF, NULL, 0);
        }
    }
}

static void BleWifi_HandleOtaVersionReq(uint8_t *data, int len)
{
	uint16_t pid;
	uint16_t cid;
	uint16_t fid;
	uint8_t state = MwOta_VersionGet(&pid, &cid, &fid);
    
	BLEWIFI_INFO("BLEWIFI: BLEWIFI_REQ_OTA_VERSION\r\n");

	if (state != MW_OTA_OK) 
		BleWifi_OtaSendVersionRsp(BLEWIFI_OTA_ERR_HW_FAILURE, 0, 0, 0);
	else
		BleWifi_OtaSendVersionRsp(BLEWIFI_OTA_SUCCESS, pid, cid, fid);
}

static uint8_t BleWifi_MwOtaPrepare(uint16_t uwProjectId, uint16_t uwChipId, uint16_t uwFirmwareId, uint32_t ulImageSize, uint32_t ulImageSum)
{
	uint8_t state = MW_OTA_OK;

	state = MwOta_Prepare(uwProjectId, uwChipId, uwFirmwareId, ulImageSize, ulImageSum);
	return state;
}

static uint8_t BleWifi_MwOtaDatain(uint8_t *pubAddr, uint32_t ulSize)
{
	uint8_t state = MW_OTA_OK;

	state = MwOta_DataIn(pubAddr, ulSize);
	return state;
}

static uint8_t BleWifi_MwOtaDataFinish(void)
{
	uint8_t state = MW_OTA_OK;

	state = MwOta_DataFinish();
	return state;
}

static void BleWifi_HandleOtaUpgradeReq(uint8_t *data, int len)
{
	blewifi_ota_t *ota = gTheOta;
	uint8_t state = MW_OTA_OK;

	BLEWIFI_INFO("BLEWIFI: BLEWIFI_REQ_OTA_UPGRADE\r\n");

	if (len != 26)
	{
		BleWifi_OtaSendUpgradeRsp(BLEWIFI_OTA_ERR_INVALID_LEN);
		return;
	}

	if (ota)
	{
		BleWifi_OtaSendUpgradeRsp(BLEWIFI_OTA_ERR_IN_PROGRESS);
		return;
	}

	ota = malloc(sizeof(blewifi_ota_t));

	if (ota)
	{
		T_MwOtaFlashHeader *ota_hdr= (T_MwOtaFlashHeader*) &data[2];
		
		ota->pkt_idx = 0;
		ota->idx     = 0;		
        ota->rx_pkt  = *(uint16_t *)&data[0];
        ota->proj_id = ota_hdr->uwProjectId;
        ota->chip_id = ota_hdr->uwChipId;
        ota->fw_id   = ota_hdr->uwFirmwareId;
        ota->total   = ota_hdr->ulImageSize;
        ota->chksum  = ota_hdr->ulImageSum;		
		ota->curr 	 = 0;

		state = BleWifi_MwOtaPrepare(ota->proj_id, ota->chip_id, ota->fw_id, ota->total, ota->chksum);

        if (state == MW_OTA_OK) 
        {
	        BleWifi_OtaSendUpgradeRsp(BLEWIFI_OTA_SUCCESS);
	        gTheOta = ota;

	        BleWifi_Ctrl_MsgSend(BLEWIFI_CTRL_MSG_OTHER_OTA_ON, NULL, 0);
        }
        else
            BleWifi_OtaSendEndRsp(BLEWIFI_OTA_ERR_HW_FAILURE, TRUE);
    }
	else
	{
		BleWifi_OtaSendUpgradeRsp(BLEWIFI_OTA_ERR_MEM_CAPACITY_EXCEED);
	}
}

static uint32_t BleWifi_OtaAdd(uint8_t *data, int len)
{
	uint16_t i;
	uint32_t sum = 0;

	for (i = 0; i < len; i++)
    {
		sum += data[i];
    }

    return sum;
}

static void BleWifi_HandleOtaRawReq(uint8_t *data, int len)
{
	blewifi_ota_t *ota = gTheOta;
	uint8_t state = MW_OTA_OK;

	BLEWIFI_INFO("BLEWIFI: BLEWIFI_REQ_OTA_RAW\r\n");

	if (!ota)
	{
		BleWifi_OtaSendEndRsp(BLEWIFI_OTA_ERR_NOT_ACTIVE, FALSE);
        goto err;
	}

	if ((ota->curr + len) > ota->total)
	{
		BleWifi_OtaSendEndRsp(BLEWIFI_OTA_ERR_INVALID_LEN, TRUE);
		goto err;
    }

	ota->pkt_idx++;
	ota->curr += len;
	ota->curr_chksum += BleWifi_OtaAdd(data, len);

	if ((ota->idx + len) >= 256)
	{
		UINT16 total = ota->idx + len;
		UINT8 *s = data;
		UINT8 *e = data + len;
		UINT16 cpyLen = 256 - ota->idx;

		if (ota->idx)
		{
			MemCopy(&ota->buf[ota->idx], s, cpyLen);
			s += cpyLen;
			total -= 256;
			ota->idx = 0;

			state = BleWifi_MwOtaDatain(ota->buf, 256);
		}

		if (state == MW_OTA_OK)
		{
			while (total >= 256)
			{
				state = BleWifi_MwOtaDatain(s, 256);
				s += 256;
				total -= 256;

				if (state != MW_OTA_OK) break;
			}

			if (state == MW_OTA_OK)
			{
				MemCopy(ota->buf, s, e - s);
				ota->idx = e - s;

				if ((ota->curr == ota->total) && ota->idx)
				{
					state = BleWifi_MwOtaDatain(ota->buf, ota->idx);
				}
			}
		}
	}
	else
	{
		MemCopy(&ota->buf[ota->idx], data, len);
		ota->idx += len;

		if ((ota->curr == ota->total) && ota->idx)
		{
			state = BleWifi_MwOtaDatain(ota->buf, ota->idx);
		}
	}

	if (state == MW_OTA_OK)
	{
		if (ota->rx_pkt && (ota->pkt_idx >= ota->rx_pkt))
		{
	        BleWifi_Ble_DataSendEncap(BLEWIFI_RSP_OTA_RAW, 0, 0);
	    		ota->pkt_idx = 0;
    }
  }
    else
		BleWifi_OtaSendEndRsp(BLEWIFI_OTA_ERR_HW_FAILURE, TRUE);

err:
	return;
}

static void BleWifi_HandleOtaEndReq(uint8_t *data, int len)
{
	blewifi_ota_t *ota = gTheOta;
	uint8_t status = data[0];

	BLEWIFI_INFO("BLEWIFI: BLEWIFI_REQ_OTA_END\r\n");

	if (!ota)
	{
		BleWifi_OtaSendEndRsp(BLEWIFI_OTA_ERR_NOT_ACTIVE, FALSE);
        goto err;
    }

		if (status == BLEWIFI_OTA_SUCCESS)
		{
		if (ota->curr == ota->total)
				{
					if (BleWifi_MwOtaDataFinish() == MW_OTA_OK)
						BleWifi_OtaSendEndRsp(BLEWIFI_OTA_SUCCESS, TRUE);
                    else
						BleWifi_OtaSendEndRsp(BLEWIFI_OTA_ERR_CHECKSUM, TRUE);
	            }
	            else
				{
					BleWifi_OtaSendEndRsp(BLEWIFI_OTA_ERR_INVALID_LEN, TRUE);
	            }
	        }
			else
			{
		if (ota) MwOta_DataGiveUp();

			// APP stop OTA
			BleWifi_OtaSendEndRsp(BLEWIFI_OTA_SUCCESS, TRUE);
		}

err:
	return;
}
#endif /* #if (BLE_OTA_FUNCTION_EN == 1) */

#if (WIFI_OTA_FUNCTION_EN == 1)
void BleWifi_Wifi_OtaTrigReq(uint8_t *data)
{
    // data length = string length + 1 (\n)
    blewifi_ctrl_http_ota_msg_send(BLEWIFI_CTRL_HTTP_OTA_MSG_TRIG, data, strlen((char*)data) + 1);
}

void BleWifi_Wifi_OtaTrigRsp(uint8_t status)
{
    BleWifi_Ble_DataSendEncap(BLEWIFI_RSP_HTTP_OTA_TRIG, &status, 1);
}

void BleWifi_Wifi_OtaDeviceVersionReq(void)        
{
    blewifi_ctrl_http_ota_msg_send(BLEWIFI_CTRL_HTTP_OTA_MSG_DEVICE_VERSION, NULL, 0);
}

void BleWifi_Wifi_OtaDeviceVersionRsp(uint16_t fid)        
{
    uint8_t data[2];
    uint8_t *p = (uint8_t *)data;

    *p++ = LO_UINT16(fid);
    *p++ = HI_UINT16(fid);

    BleWifi_Ble_DataSendEncap(BLEWIFI_RSP_HTTP_OTA_DEVICE_VERSION, data, 2);
}

void BleWifi_Wifi_OtaServerVersionReq(void)
{
    blewifi_ctrl_http_ota_msg_send(BLEWIFI_CTRL_HTTP_OTA_MSG_SERVER_VERSION, NULL, 0);
}

void BleWifi_Wifi_OtaServerVersionRsp(uint16_t fid)
{
    uint8_t data[2];
    uint8_t *p = (uint8_t *)data;

    *p++ = LO_UINT16(fid);
    *p++ = HI_UINT16(fid);

    BleWifi_Ble_DataSendEncap(BLEWIFI_RSP_HTTP_OTA_SERVER_VERSION, data, 2);
}
#endif /* #if (WIFI_OTA_FUNCTION_EN == 1) */

static void BleWifi_MP_CalVbat(uint8_t *data, int len)
{
    float fTargetVbat;

    memcpy(&fTargetVbat, &data[0], 4);
    Hal_Aux_VbatCalibration(fTargetVbat);
    BleWifi_Ble_SendResponse(BLEWIFI_RSP_MP_CAL_VBAT, 0);
}

static void BleWifi_MP_CalIoVoltage(uint8_t *data, int len)
{
    float fTargetIoVoltage;
    uint8_t ubGpioIdx;

    memcpy(&ubGpioIdx, &data[0], 1);
    memcpy(&fTargetIoVoltage, &data[1], 4);
    Hal_Aux_IoVoltageCalibration(ubGpioIdx, fTargetIoVoltage);
    BleWifi_Ble_SendResponse(BLEWIFI_RSP_MP_CAL_IO_VOLTAGE, 0);
}

static void BleWifi_MP_CalTmpr(uint8_t *data, int len)
{
    BleWifi_Ble_SendResponse(BLEWIFI_RSP_MP_CAL_TMPR, 0);
}

static void BleWifi_MP_SysModeWrite(uint8_t *data, int len)
{
    T_MwFim_SysMode tSysMode;
    
    // set the settings of system mode
    tSysMode.ubSysMode = data[0];
    if (tSysMode.ubSysMode < MW_FIM_SYS_MODE_MAX)
    {
        if (MW_FIM_OK == MwFim_FileWrite(MW_FIM_IDX_GP03_PATCH_SYS_MODE, 0, MW_FIM_SYS_MODE_SIZE, (uint8_t*)&tSysMode))
        {
            BleWifi_Ctrl_SysModeSet(tSysMode.ubSysMode);
            BleWifi_Ble_SendResponse(BLEWIFI_RSP_MP_SYS_MODE_WRITE, 0);
            return;
        }
    }

    BleWifi_Ble_SendResponse(BLEWIFI_RSP_MP_SYS_MODE_WRITE, 1);
}

static void BleWifi_MP_SysModeRead(uint8_t *data, int len)
{
    uint8_t ubSysMode;

    ubSysMode = BleWifi_Ctrl_SysModeGet();
    BleWifi_Ble_SendResponse(BLEWIFI_RSP_MP_SYS_MODE_READ, ubSysMode);
}

static void BleWifi_Eng_SysReset(uint8_t *data, int len)
{
    BleWifi_Ble_SendResponse(BLEWIFI_RSP_ENG_SYS_RESET, 0);

    // wait the BLE response, then reset the system
    osDelay(3000);
    Hal_Sys_SwResetAll();
}

static void BleWifi_Eng_BleCmd(uint8_t *data, int len)
{
    msg_print_uart1("+BLE:%s\r\n", data);
    BleWifi_Ble_SendResponse(BLEWIFI_RSP_ENG_BLE_CMD, 0);
}

static void BleWifi_Ble_ProtocolHandler_Scan(uint16_t type, uint8_t *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: Recv BLEWIFI_REQ_SCAN \r\n");
    BleWifi_Wifi_DoScan(data, len);
}

static void BleWifi_Ble_ProtocolHandler_Connect(uint16_t type, uint8_t *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: Recv BLEWIFI_REQ_CONNECT \r\n");
    BleWifi_Wifi_DoConnect(data, len);
}

static void BleWifi_Ble_ProtocolHandler_Disconnect(uint16_t type, uint8_t *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: Recv BLEWIFI_REQ_DISCONNECT \r\n");
    BleWifi_Wifi_DoDisconnect();
}

static void BleWifi_Ble_ProtocolHandler_Reconnect(uint16_t type, uint8_t *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: Recv BLEWIFI_REQ_RECONNECT \r\n");
}

static void BleWifi_Ble_ProtocolHandler_ReadDeviceInfo(uint16_t type, uint8_t *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: Recv BLEWIFI_REQ_READ_DEVICE_INFO \r\n");
    BleWifi_Wifi_ReadDeviceInfo();
}

static void BleWifi_Ble_ProtocolHandler_WriteDeviceInfo(uint16_t type, uint8_t *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: Recv BLEWIFI_REQ_WRITE_DEVICE_INFO \r\n");
    BleWifi_Wifi_WriteDeviceInfo(data, len);
}

static void BleWifi_Ble_ProtocolHandler_WifiStatus(uint16_t type, uint8_t *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: Recv BLEWIFI_REQ_WIFI_STATUS \r\n");
    BleWifi_Wifi_SendStatusInfo(BLEWIFI_RSP_WIFI_STATUS);
}

static void BleWifi_Ble_ProtocolHandler_Reset(uint16_t type, uint8_t *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: Recv BLEWIFI_REQ_RESET \r\n");
    BleWifi_Wifi_ResetRecord();
}

#if (BLE_OTA_FUNCTION_EN == 1)
static void BleWifi_Ble_ProtocolHandler_OtaVersion(uint16_t type, uint8_t *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: Recv BLEWIFI_REQ_OTA_VERSION \r\n");
    BleWifi_HandleOtaVersionReq(data, len);
}

static void BleWifi_Ble_ProtocolHandler_OtaUpgrade(uint16_t type, uint8_t *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: Recv BLEWIFI_REQ_OTA_UPGRADE \r\n");
    BleWifi_HandleOtaUpgradeReq(data, len);
}

static void BleWifi_Ble_ProtocolHandler_OtaRaw(uint16_t type, uint8_t *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: Recv BLEWIFI_REQ_OTA_RAW \r\n");
    BleWifi_HandleOtaRawReq(data, len);
}

static void BleWifi_Ble_ProtocolHandler_OtaEnd(uint16_t type, uint8_t *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: Recv BLEWIFI_REQ_OTA_END \r\n");
    BleWifi_HandleOtaEndReq(data, len);
}
#endif

#if (WIFI_OTA_FUNCTION_EN == 1)
static void BleWifi_Ble_ProtocolHandler_HttpOtaTrig(uint16_t type, uint8_t *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: Recv BLEWIFI_REQ_HTTP_OTA_TRIG \r\n");
    BleWifi_Wifi_OtaTrigReq(WIFI_OTA_HTTP_URL);
}

static void BleWifi_Ble_ProtocolHandler_HttpOtaDeviceVersion(uint16_t type, uint8_t *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: Recv BLEWIFI_REQ_HTTP_OTA_DEVICE_VERSION \r\n");
    BleWifi_Wifi_OtaDeviceVersionReq();
}

static void BleWifi_Ble_ProtocolHandler_HttpOtaServerVersion(uint16_t type, uint8_t *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: Recv BLEWIFI_REQ_HTTP_OTA_SERVER_VERSION \r\n");
    BleWifi_Wifi_OtaServerVersionReq();
}
#endif

static void BleWifi_Ble_ProtocolHandler_MpCalVbat(uint16_t type, uint8_t *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: Recv BLEWIFI_REQ_MP_CAL_VBAT \r\n");
    BleWifi_MP_CalVbat(data, len);
}

static void BleWifi_Ble_ProtocolHandler_MpCalIoVoltage(uint16_t type, uint8_t *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: Recv BLEWIFI_REQ_MP_CAL_IO_VOLTAGE \r\n");
    BleWifi_MP_CalIoVoltage(data, len);
}

static void BleWifi_Ble_ProtocolHandler_MpCalTmpr(uint16_t type, uint8_t *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: Recv BLEWIFI_REQ_MP_CAL_TMPR \r\n");
    BleWifi_MP_CalTmpr(data, len);
}

static void BleWifi_Ble_ProtocolHandler_MpSysModeWrite(uint16_t type, uint8_t *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: Recv BLEWIFI_REQ_MP_SYS_MODE_WRITE \r\n");
    BleWifi_MP_SysModeWrite(data, len);
}

static void BleWifi_Ble_ProtocolHandler_MpSysModeRead(uint16_t type, uint8_t *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: Recv BLEWIFI_REQ_MP_SYS_MODE_READ \r\n");
    BleWifi_MP_SysModeRead(data, len);
}

static void BleWifi_Ble_ProtocolHandler_EngSysReset(uint16_t type, uint8_t *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: Recv BLEWIFI_REQ_ENG_SYS_RESET \r\n");
    BleWifi_Eng_SysReset(data, len);
}

static void BleWifi_Ble_ProtocolHandler_EngWifiMacWrite(uint16_t type, uint8_t *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: Recv BLEWIFI_REQ_ENG_WIFI_MAC_WRITE \r\n");
    BleWifi_Wifi_MacAddrWrite(data, len);
}

static void BleWifi_Ble_ProtocolHandler_EngWifiMacRead(uint16_t type, uint8_t *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: Recv BLEWIFI_REQ_ENG_WIFI_MAC_READ \r\n");
    BleWifi_Wifi_MacAddrRead(data, len);
}

static void BleWifi_Ble_ProtocolHandler_EngBleMacWrite(uint16_t type, uint8_t *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: Recv BLEWIFI_REQ_ENG_BLE_MAC_WRITE \r\n");
    BleWifi_Ble_MacAddrWrite(data, len);
}

static void BleWifi_Ble_ProtocolHandler_EngBleMacRead(uint16_t type, uint8_t *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: Recv BLEWIFI_REQ_ENG_BLE_MAC_READ \r\n");
    BleWifi_Ble_MacAddrRead(data, len);
}

static void BleWifi_Ble_ProtocolHandler_EngBleCmd(uint16_t type, uint8_t *data, int len)
{
    BLEWIFI_INFO("BLEWIFI: Recv BLEWIFI_REQ_ENG_BLE_CMD \r\n");
    BleWifi_Eng_BleCmd(data, len);
}

// it is used in the ctrl task
void BleWifi_Ble_ProtocolHandler(uint16_t type, uint8_t *data, int len)
{
    uint32_t i = 0;

    while (g_tBleProtocolHandlerTbl[i].ulEventId != 0xFFFFFFFF)
    {
        // match
        if (g_tBleProtocolHandlerTbl[i].ulEventId == type)
        {
            g_tBleProtocolHandlerTbl[i].fpFunc(type, data, len);
            break;
        }

        i++;
    }

    // not match
    if (g_tBleProtocolHandlerTbl[i].ulEventId == 0xFFFFFFFF)
    {
    }
}

// it is used in the ctrl task
void BleWifi_Ble_DataRecvHandler(uint8_t *data, int data_len)
{
    blewifi_hdr_t *hdr = NULL;
    int hdr_len = sizeof(blewifi_hdr_t);

    /* 1.aggregate fragment data packet, only first frag packet has header */
    /* 2.handle blewifi data packet, if data frame is aggregated completely */
    if (g_rx_packet.offset == 0)
    {
        hdr = (blewifi_hdr_t*)data;
        g_rx_packet.total_len = hdr->data_len + hdr_len;
        g_rx_packet.remain = g_rx_packet.total_len;
        g_rx_packet.aggr_buf = malloc(g_rx_packet.total_len);

        if (g_rx_packet.aggr_buf == NULL) {
           BLEWIFI_ERROR("%s no mem, len %d\n", __func__, g_rx_packet.total_len);
           return;
        }
    }

    // error handle
    // if the size is overflow, don't copy the whole data
    if (data_len > g_rx_packet.remain)
        data_len = g_rx_packet.remain;

    memcpy(g_rx_packet.aggr_buf + g_rx_packet.offset, data, data_len);
    g_rx_packet.offset += data_len;
    g_rx_packet.remain -= data_len;

    /* no frag or last frag packet */
    if (g_rx_packet.remain == 0)
    {
        hdr = (blewifi_hdr_t*)g_rx_packet.aggr_buf;
        BleWifi_Ble_ProtocolHandler(hdr->type, g_rx_packet.aggr_buf + hdr_len,  (g_rx_packet.total_len - hdr_len));
        g_rx_packet.offset = 0;
        g_rx_packet.remain = 0;
        free(g_rx_packet.aggr_buf);
        g_rx_packet.aggr_buf = NULL;
    }
}

void BleWifi_Ble_DataSendEncap(uint16_t type_id, uint8_t *data, int total_data_len)
{
    blewifi_hdr_t *hdr = NULL;
    int remain_len = total_data_len;

    /* 1.fragment data packet to fit MTU size */

    /* 2.Pack blewifi header */
    hdr = malloc(sizeof(blewifi_hdr_t) + remain_len);
    if (hdr == NULL)
    {
        BLEWIFI_ERROR("BLEWIFI: memory alloc fail\r\n");
        return;
    }

    hdr->type = type_id;
    hdr->data_len = remain_len;
    if (hdr->data_len)
        memcpy(hdr->data, data, hdr->data_len);

    BLEWIFI_DUMP("[BLEWIFI]:out packet", (uint8_t*)hdr, (hdr->data_len + sizeof(blewifi_hdr_t)));

    /* 3.send app data to BLE stack */
    BleWifi_Ble_SendAppMsgToBle(BLEWIFI_APP_MSG_SEND_DATA, (hdr->data_len + sizeof(blewifi_hdr_t)), (uint8_t *)hdr);

    free(hdr);
}

void BleWifi_Ble_SendResponse(uint16_t type_id, uint8_t status)
{
    BleWifi_Ble_DataSendEncap(type_id, &status, 1);
}
