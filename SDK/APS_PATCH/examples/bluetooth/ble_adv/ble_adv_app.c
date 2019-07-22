#include "ble_adv_app.h"
#include "ble_hci_if.h"
#include "ble_cm_if.h"
#include "ble_gap_if.h"
#include "ble_util.h"
#include "msg.h"


#define ENABLE_APP_DEBUG


#ifdef ENABLE_APP_DEBUG

#define BLE_APP_PRINT		printf

#else

#define BLE_APP_PRINT(...)

#endif

// Maximum connection interval (units of 1.25ms) if automatic parameter update request is enabled
#define DEFAULT_DESIRED_MAX_CONN_INTERVAL     1600
// Minimum connection interval (units of 1.25ms) if automatic parameter update request is enabled
#define DEFAULT_DESIRED_MIN_CONN_INTERVAL     200


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
	0
};

static UINT8 gBleScanRspData[] =
{
	0x0f,
	GAP_ADTYPE_LOCAL_NAME_COMPLETE,   
	'N', 'L', '1', '0', '0', '0', ' ', 'A', 'D', 'V', ' ', 'A', 'P', 'P'
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

static void BleAppCmMsgHandler(TASK task, MESSAGEID id, MESSAGE message)
{
	switch (id)
	{
		case LE_CM_MSG_INIT_COMPLETE_CFM:
        {
			BLE_APP_PRINT("LE_CM_MSG_INIT_COMPLETE_CFM\r\n");
            LeGapSetAdvData(sizeof(gBleAdvertData), gBleAdvertData);
            LeSetScanRspData(sizeof(gBleScanRspData), gBleScanRspData);
            BleAppSetAdvtisingPara(LE_HCI_ADV_TYPE_ADV_NONCONN_IND, LE_HCI_OWN_ADDR_PUBLIC, 0, LE_HCI_ADV_FILT_NONE);

            gTheBle.state = APP_STATE_IDLE;
        }
		break;
        
		case LE_CM_MSG_SET_ADVERTISING_DATA_CFM:
			BLE_APP_PRINT("LE_CM_MSG_SET_ADVERTISING_DATA_CFM - Status = %x\r\n", ((LE_CM_MSG_SET_ADVERTISING_DATA_CFM_T *)message)->status);
		break;

		case LE_CM_MSG_SET_SCAN_RSP_DATA_CFM:
        {
			BLE_APP_PRINT("LE_CM_MSG_SET_SCAN_RSP_DATA_CFM - Status = %x\r\n", ((LE_CM_MSG_SET_SCAN_RSP_DATA_CFM_T *)message)->status);
            BleAppSetAdvtisingPara(LE_HCI_ADV_TYPE_ADV_NONCONN_IND, LE_HCI_OWN_ADDR_PUBLIC, 0, LE_HCI_ADV_FILT_NONE);
        }
		break;

		case LE_CM_MSG_SET_ADVERTISING_PARAMS_CFM:
        {
			BLE_APP_PRINT("LE_CM_MSG_SET_ADVERTISING_PARAMS_CFM - Status = %x\r\n", ((LE_CM_MSG_SET_ADVERTISING_PARAMS_CFM_T *)message)->status);
            LeSendMessageAfter(&gTheBle.task, BLE_APP_MSG_ENTER_ADVERTISING, 0, 500);
        }
        break;

		case LE_CM_MSG_ENTER_ADVERTISING_CFM:
		{
			LE_CM_MSG_ENTER_ADVERTISING_CFM_T *cfm = (LE_CM_MSG_ENTER_ADVERTISING_CFM_T *)message;
			BLE_APP_PRINT("LE_CM_MSG_ENTER_ADVERTISING_CFM Status = %x\r\n", cfm->status);
            
			if (cfm->status == SYS_ERR_SUCCESS)
			{
				gTheBle.state = APP_STATE_ADVERTISING;

                LeSendMessageAfter(&gTheBle.task, BLE_APP_MSG_EXIT_ADVERTISING, 0, 29000);
            }
        }
		break;

		case LE_CM_MSG_EXIT_ADVERTISING_CFM:
		{
			LE_CM_MSG_EXIT_ADVERTISING_CFM_T *cfm = (LE_CM_MSG_EXIT_ADVERTISING_CFM_T *)message;
            BLE_APP_PRINT("LE_CM_MSG_EXIT_ADVERTISING_CFM Status = %x\r\n", cfm->status);
            
			if (cfm->status == SYS_ERR_SUCCESS)
			{
				gTheBle.state = APP_STATE_IDLE;

                LeSendMessageAfter(&gTheBle.task, BLE_APP_MSG_ENTER_ADVERTISING, 0, 1000);
            }
        }
		break;
        
		default:
        break;
    }
}

static void BleAppMsgHandler(TASK task, MESSAGEID id, MESSAGE message)
{
	BLE_APP_PRINT("BleAppMsgHandler id = %x\r\n", id);
	switch (id)
	{
		case BLE_APP_MSG_INITIALIZING:
        {
			LeCmInit(&gTheBle.task);
        }
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

		default:
		break;
    }
}

static void BleAppTaskHandler(TASK task, MESSAGEID id, MESSAGE message)
{
	if ((id >= LE_CM_MSG_BASE) && (id < LE_CM_MSG_TOP))
	{
		BleAppCmMsgHandler(task, id, message);
    }
    else if ((id >= BLE_APP_MSG_INITIALIZING) && (id < BLE_APP_MSG_TOP))
    {
		BleAppMsgHandler(task, id, message);
    }
}

void BleAppInit(void)
{
	gTheBle.state = APP_STATE_INIT;
	LeHostCreateTask(&gTheBle.task, BleAppTaskHandler);
	LeSendMessage(&gTheBle.task, BLE_APP_MSG_INITIALIZING, 0);
	printf("OPL1000 BLE init completed. \r\n");
}

