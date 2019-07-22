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

#ifndef _BLE_GAP_IF_H_
#define _BLE_GAP_IF_H_

#include "ble.h"

/** \defgroup BLE_ALL_APIs BLE ALL APIs
 * @brief BLE ALL APIs
 */

/** @addtogroup BLE_ALL_APIs
 * @{
 */

/** \defgroup BLE_GAP_APIs BLE GAP APIs
 */

/** @addtogroup BLE_GAP_APIs
 * @{
 */


#define LE_GAP_ADV_MAX_SIZE						31


#define GAP_ADTYPE_FLAGS                        0x01 
#define GAP_ADTYPE_16BIT_MORE                   0x02 
#define GAP_ADTYPE_16BIT_COMPLETE               0x03 
#define GAP_ADTYPE_32BIT_MORE                   0x04 
#define GAP_ADTYPE_32BIT_COMPLETE               0x05 
#define GAP_ADTYPE_128BIT_MORE                  0x06 
#define GAP_ADTYPE_128BIT_COMPLETE              0x07 
#define GAP_ADTYPE_LOCAL_NAME_SHORT             0x08 
#define GAP_ADTYPE_LOCAL_NAME_COMPLETE          0x09 
#define GAP_ADTYPE_POWER_LEVEL                  0x0A 
#define GAP_ADTYPE_OOB_CLASS_OF_DEVICE          0x0D 
#define GAP_ADTYPE_OOB_SIMPLE_PAIRING_HASHC     0x0E 
#define GAP_ADTYPE_OOB_SIMPLE_PAIRING_RANDR     0x0F 
#define GAP_ADTYPE_SM_TK                        0x10 
#define GAP_ADTYPE_SM_OOB_FLAG                  0x11 
#define GAP_ADTYPE_SLAVE_CONN_INTERVAL_RANGE    0x12 
#define GAP_ADTYPE_SIGNED_DATA                  0x13 
#define GAP_ADTYPE_SERVICES_LIST_16BIT          0x14 
#define GAP_ADTYPE_SERVICES_LIST_128BIT         0x15 
#define GAP_ADTYPE_SERVICE_DATA                 0x16 
#define GAP_ADTYPE_PUBLIC_TARGET_ADDR           0x17 
#define GAP_ADTYPE_RANDOM_TARGET_ADDR           0x18 
#define GAP_ADTYPE_APPEARANCE                   0x19 
#define GAP_ADTYPE_ADV_INTERVAL                 0x1A 
#define GAP_ADTYPE_LE_BD_ADDR                   0x1B 
#define GAP_ADTYPE_LE_ROLE                      0x1C 
#define GAP_ADTYPE_SIMPLE_PAIRING_HASHC_256     0x1D 
#define GAP_ADTYPE_SIMPLE_PAIRING_RANDR_256     0x1E 
#define GAP_ADTYPE_SERVICE_DATA_32BIT           0x20 
#define GAP_ADTYPE_SERVICE_DATA_128BIT          0x21 
#define GAP_ADTYPE_3D_INFO_DATA                 0x3D 
#define GAP_ADTYPE_MANUFACTURER_SPECIFIC        0xFF 

#define GAP_ADTYPE_FLAGS_LIMITED                0x01 
#define GAP_ADTYPE_FLAGS_GENERAL                0x02 
#define GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED    0x04 

#define GAPBOND_PAIRING_MODE_NO_PAIRING         0x00 
#define GAPBOND_PAIRING_MODE_WAIT_FOR_REQ       0x01 
#define GAPBOND_PAIRING_MODE_INITIATE           0x02 

#define GAPBOND_IO_CAP_DISPLAY_ONLY             0x00 
#define GAPBOND_IO_CAP_DISPLAY_YES_NO           0x01 
#define GAPBOND_IO_CAP_KEYBOARD_ONLY            0x02 
#define GAPBOND_IO_CAP_NO_INPUT_NO_OUTPUT       0x03 
#define GAPBOND_IO_CAP_KEYBOARD_DISPLAY         0x04 

#define GAP_SCAN_TYPE_PASSIVE					0
#define GAP_SCAN_TYPE_ACTIVE					1

#define GAP_PUBLIC_ADDR							0
#define GAP_RAND_ADDR_STATIC					1
#define GAP_RAND_ADDR_NRPA						2
#define GAP_RAND_ADDR_RPA						3

#define GAP_TX_PWR_CURR_VAL						0
#define GAP_TX_PWR_MAX_VAL						1


typedef struct
{
	UINT16			interval_min;            /**< mininum advertising interval */
	UINT16			interval_max;            /**< maxinum advertising interval */
	UINT8			type;                    /**< advertising type */
	UINT8			own_addr_type;           /**< owner address type */
	UINT8			peer_addr_type;          /**< peer address type */
	BD_ADDR			peer_addr;               /**< peer address */
	UINT8			channel_map;             /**< advertising channel map */
	UINT8			filter_policy;           /**< advertising filter policy */
} LE_GAP_ADVERTISING_PARAM_T;

typedef struct
{
	UINT8			type;                    /**< scan type */
	UINT16			interval;                /**< scan interval */
	UINT16			window;                  /**< scan window */
	UINT8			own_addr_type;           /**< owner address type */
    UINT8			filter_policy;           /**< scan filter policy */
} LE_GAP_SCAN_PARAM_T;

typedef struct
{
	UINT16			interval_min;            /**< mininum connection interval */
	UINT16			interval_max;			 /**< maxinum connection interval */
	UINT16			latency;				 /**< slave latency */
	UINT16			supervision_timeout;	 /**< supervision timeout for the LE Link */
} LE_GAP_CONN_PARAM_T;


/**
 * @brief    Disconnect the physical connection.
 *
 * @param    conn_hdl  connection handle.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGapDisconnectReq(UINT16 conn_hdl);

/**
 * @brief    Called to set ADV data.
 *
 * @param    len   ADV data length.
 * @param    data  ADV data.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGapSetAdvData(UINT8 len, UINT8 *data);

/**
 * @brief    Called to set scan response data.
 *
 * @param    len   scan response data length.
 * @param    data  scan response data.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeSetScanRspData(UINT8 len, UINT8 *data);

/**
 * @brief    Called to set ADV parameters.
 *
 * @param    params  advertising params.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGapSetAdvParameter(LE_GAP_ADVERTISING_PARAM_T *params);

/**
 * @brief    Called to generation random address.
 *
 * @param    type  address type.
 * @param    addr  address.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGapGenRandAddr(UINT8 type, BD_ADDR addr);

/**
 * @brief    Called to set static address.
 *
 * @param    addr  the static address which should be set.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGapSetStaticAddr(BD_ADDR addr);

/**
 * @brief    Called to set random address.
 *
 * @param    addr  the random address which should be set.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGapSetRandAddr(BD_ADDR addr);

/**
 * @brief    Enable or disable advertising function.
 *
 * @param    start  TRUE is enable, FALSE is disable.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGapAdvertisingEnable(BOOL start);

/**
 * @brief    Called to set scan parameters.
 *
 * @param    params  scan parameters.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeSetScanParameter(LE_GAP_SCAN_PARAM_T *params);

/**
 * @brief    Request scanning start.
 *
 * @param    start   TRUE is start, FALSE is not.
 * @param    filter  scan policy, refer to LE_HCI_SCAN_FILT_* in ble_hci_if.h
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGapScanningReq(BOOL start, BOOL filter);

/**
 * @brief    Called to set connection parameters.
 *
 * @param    interval_min         mininum connection interval.
 * @param    interval_max         maxinum connection interval.
 * @param    slave_letency        slave letency.
 * @param    supervision_timeout  supervison timeout.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGapSetConnParameter(UINT16 interval_min, UINT16 interval_max, UINT16 slave_latency, UINT16 supervision_timeout);

/**
 * @brief    Cancel connect request.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGapConnectCancelReq(void);

/**
 * @brief    Central connect request.
 *
 * @param    taddr          advertisers device address.
 * @param    own_addr_type  owner address type.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGapCentralConnectReq(LE_BT_ADDR_T *taddr, UINT8 own_addr_type);

/**
 * @brief    Read whitelist size in the controller.
 */
void LeGapReadWhiteListSize(void);

/**
 * @brief    Clear whitelist in the controller.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGapClearWhiteList(void);

/**
 * @brief    Add device to whitelist.
 *
 * @param    bt_addr  BT device address.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGapAddToWhiteList(LE_BT_ADDR_T *bt_addr);

/**
 * @brief    Remove device from whitelist.
 *
 * @param    bt_addr  BT device address.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGapRemoveFromWhiteList(LE_BT_ADDR_T *bt_addr);

/**
 * @brief    Read tx power value for the specified connection.
 *
 * @param    conn_hdl  connection handle.
 * @param    type      current tx power, or maxinum tx power. Don't support.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGapReadTxPower(UINT16 conn_hdl, UINT8 type);

/**
 * @brief    Read RSSI value from controller.
 *
 * @param    conn_hdl  connection handle.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGapReadRssi(UINT16 conn_hdl);

/**
 * @brief    Read the resolving-list size in the controller.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
void LeGapReadResolvingListSize(void);

/**
 * @brief    Clear the resolving-list in the controller.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGapClearResolvingList(void);

/**
 * @brief    Add device to resolving-list.
 *
 * @param    bt_addr  BT device address.
 * @param    irk      IRK, Identity Resolving Key
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGapAddToResolvingList(LE_BT_ADDR_T *bt_addr, UINT8 *irk);

/**
 * @brief    Remove device from resolving-list.
 *
 * @param    bt_addr  BT device address.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGapRemoveFromWhiteList(LE_BT_ADDR_T *bt_addr);

/**
 * @brief    Read ADV channel txpower.
 */
void LeGapReadAdvChannelTxPower(void);

/**
 * @brief    Central set data channel.
 *
 * @param    ch  data channel.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGapCentralSetDataChannel(UINT8 *ch);

/**
 * @brief    Read channel map.
 *
 * @param    conn_hdl  connection handle.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGapReadChannelMap(UINT16 conn_hdl);

/**
 * @brief    Set data channel PDU length.
 *
 * @param    tx_octets  the maximum number of octets in the Payload field 
 *                      that the local device will send to the remote device.
 * @param    tx_time    the maximum number of microseconds that the local device 
 *                      will take to transmit a PDU to the remote device.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGapSetDataChannelPduLen(UINT16 conn_hdl, UINT16 tx_octets, UINT16 tx_time);

/**
 * @brief    Set resolvable private address timeout.
 *
 * @param    timeout  RPA_Timeout, measured in seconds.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGapSetRpaTimeout(UINT16 timeout);

/**
 * @brief    Get owner device address.
 */
void LeGapGetBtAddr(void);

/**
 * @brief    Connection parameters update response.
 *
 * @param    conn_hdl    connection handle.
 * @param    identifier  TBD
 * @param    accept      accept request, or not.
 *
 * @return   None.
 */
void LeGapConnUpdateResponse(UINT16 conn_hdl, UINT8 identifier, BOOL accept);

/**
 * @brief    Connection parameters update request.
 *
 * @param    conn_hdl  connection handle.
 * @param    para      update connection parameters.
 *
 * @return   None.
 */
LE_ERR_STATE LeGapConnUpdateRequest(UINT16 conn_hdl, LE_CONN_PARA_T *para);

/**
 * @brief    Connection parameters request response.
 *
 * @param    conn_hdl  connection handle.
 * @param    accept    TRUE is accept, FALSE is not.
 *
 * @return   None.
 */
void LeGapConnParaRequestRsp(UINT16 conn_hdl, BOOL accept);

LE_ERR_STATE LeGapReadPhy(UINT16 conn_hdl);

LE_ERR_STATE LeGapSetDefaultPhy(UINT8 tx, UINT8 rx);

LE_ERR_STATE LeGapSetPhy(UINT16 conn_hdl, UINT8 tx, UINT8 rx, UINT16 option);

/**
 * @}
 */

/**
 * @}
 */

#endif
