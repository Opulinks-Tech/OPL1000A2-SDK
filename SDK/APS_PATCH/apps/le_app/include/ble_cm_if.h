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

#ifndef _LE_CM_IF_H_
#define _LE_CM_IF_H_

#include "ble.h"
#include "ble_msg.h"

/** \defgroup BLE_ALL_APIs BLE ALL APIs
 * @brief BLE ALL APIs
 */

/** @addtogroup BLE_ALL_APIs
 * @{
 */

/** \defgroup BLE_CM_APIs BLE CM APIs
 */

/** @addtogroup BLE_CM_APIs
 * @{
 */


/**
 * @brief  BLE connection management message id
 */
enum
{
	LE_CM_MSG_INIT_COMPLETE_CFM = LE_CM_MSG_BASE,      /**< initialize complete */

	LE_CM_MSG_SET_DISCONNECT_CFM,                      /**< set disconnect confirm */
	LE_CM_MSG_DISCONNECT_COMPLETE_IND,                 /**< disconnect complete indication */

	LE_CM_MSG_SET_ADVERTISING_DATA_CFM,                /**< set advertising data confirm */
	LE_CM_MSG_SET_SCAN_RSP_DATA_CFM,                   /**< set scan response data confirm */
	LE_CM_MSG_SET_ADVERTISING_PARAMS_CFM,              /**< set advertising parameters confirm */
	LE_CM_MSG_ENTER_ADVERTISING_CFM,                   /**< enter advertising confirm */
	LE_CM_MSG_EXIT_ADVERTISING_CFM,                    /**< exit advertising confirm */

	LE_CM_MSG_SET_SCAN_PARAMS_CFM,                     /**< set scan parameters confirm */
	LE_CM_MSG_ENTER_SCANNING_CFM,                      /**< enter scanning confirm */
	LE_CM_MSG_EXIT_SCANNING_CFM,                       /**< exit scanning confirm */
    
	LE_CM_MSG_CREATE_CONNECTION_CFM,                   /**< create connection confirm */
	LE_CM_MSG_CANCEL_CONNECTION_CFM,                   /**< cancel connection confirm */

	LE_CM_MSG_READ_TX_POWER_CFM,                       /**< read tx power confirm */
	LE_CM_MSG_READ_BD_ADDR_CFM,                        /**< read device address confirm */
    
	LE_CM_MSG_READ_RSSI_CFM,                           /**< read RSSI confirm */
	LE_CM_MSG_SET_RANDOM_ADDRESS_CFM,                  /**< set random address confirm */
	LE_CM_MSG_READ_ADV_TX_POWER_CFM,                   /**< read advertising tx power confirm */

	LE_CM_MSG_READ_WHITE_LIST_SIZE_CFM,                /**< read whitelist size confirm */
	LE_CM_MSG_CLEAR_WHITE_LIST_CFM,                    /**< clear whitelist confirm */
    LE_CM_MSG_ADD_TO_WHITE_LIST_CFM,                   /**< add to whitelist confirm */
	LE_CM_MSG_REMOVE_FROM_WHITE_LIST_CFM,              /**< remove from whitelist confirm */

	LE_CM_MSG_SET_CHANNEL_MAP_CFM,                     /**< set channel map confirm */
	LE_CM_MSG_READ_CHANNEL_MAP_CFM,                    /**< read channel map confirm */

	LE_CM_MSG_SET_DATA_LENGTH_CFM,                     /**< set data length confirm */
	LE_CM_MSG_DATA_LEN_CHANGE_IND,                     /**< data length change indication */

	LE_CM_MSG_ADD_TO_RESOLVING_LIST_CFM,               /**< add to resolving list confirm */
	LE_CM_MSG_REMOVE_FROM_RESOLVING_LIST_CFM,          /**< remove from resolving list confirm */
	LE_CM_MSG_CLEAR_RESOLVING_LIST_CFM,                /**< clear resolving list confirm */
	LE_CM_MSG_READ_RESOLVING_LIST_SIZE_CFM,            /**< read resolving list size confirm */
	LE_CM_MSG_SET_RPA_TIMEOUT_CFM,                     /**< set resolving private address timeout confirm */

#if 0
	LE_CM_READ_AUTH_PAYLOAD_TIMEOUT_CFM,
	LE_CM_WRITE_AUTH_PAYLOAD_TIMEOUT_CFM,
#endif

    LE_CM_MSG_SIGNAL_UPDATE_REQ,                       /**< signal update request */
#if 0
    LE_CM_MSG_SIGNAL_UPDATE_RSP,
#endif
    LE_CM_MSG_CONN_UPDATE_COMPLETE_IND,                /**< connection update complete indication */
    LE_CM_MSG_CONN_PARA_REQ,                           /**< connection parameters request */

	LE_CM_MSG_ENCRYPTION_CHANGE_IND,                   /**< encryption change indication */
	LE_CM_MSG_ENCRYPTION_REFRESH_IND,                  /**< encryption refresh indication */
	LE_CM_MSG_LTK_REQ_IND,                             /**< long term key indication */

	LE_CM_MSG_ADVERTISE_REPORT_IND,                    /**< advertising report indication */
	LE_CM_MSG_DIRECT_ADV_REPORT_IND,                   /**< direct advertising report indication */
    LE_CM_CONNECTION_COMPLETE_IND,                     /**< connection complete indication */

	//LE_CM_READ_PEER_RESOLVABLE_ADDRESS_CFM,
	LE_CM_MSG_READ_LOCAL_RPA_CFM,                      /**< read local resolving private address confirm */
	
	//LE_CM_SET_ADDRESS_RESOLUTION_ENABLE_CFM,

	LE_CM_MSG_READ_PHY_CFM,
	LE_CM_MSG_SET_DEFAULT_PHY_CFM,
	LE_CM_MSG_SET_PHY_CFM,
	LE_CM_MSG_PHY_UPDATE_COMPLETE_IND,

    LE_CM_MSG_TOP                                      /**< top of CM message id */
};


typedef struct
{
	UINT16				status;                        /**< refer to LE_ERR_STATE in ble_err.h */
} LE_CM_REQ_STATUS_T;


typedef LE_CM_REQ_STATUS_T LE_CM_MSG_SET_RANDOM_ADDRESS_CFM_T;

typedef LE_CM_REQ_STATUS_T LE_CM_MSG_SET_ADVERTISING_DATA_CFM_T;
typedef LE_CM_REQ_STATUS_T LE_CM_MSG_SET_ADVERTISING_PARAMS_CFM_T;
typedef LE_CM_REQ_STATUS_T LE_CM_MSG_ENTER_ADVERTISING_CFM_T;
typedef LE_CM_REQ_STATUS_T LE_CM_MSG_EXIT_ADVERTISING_CFM_T;

typedef LE_CM_REQ_STATUS_T LE_CM_MSG_SET_SCAN_RSP_DATA_CFM_T;
typedef LE_CM_REQ_STATUS_T LE_CM_MSG_SET_SCAN_PARAMS_CFM_T;
typedef LE_CM_REQ_STATUS_T LE_CM_MSG_ENTER_SCANNING_CFM_T;
typedef LE_CM_REQ_STATUS_T LE_CM_MSG_EXIT_SCANNING_CFM_T;

typedef LE_CM_REQ_STATUS_T LE_CM_MSG_CREATE_CONNECTION_CFM_T;
typedef LE_CM_REQ_STATUS_T LE_CM_MSG_CANCEL_CONNECTION_CFM_T;

typedef LE_CM_REQ_STATUS_T LE_CM_MSG_CLEAR_WHITE_LIST_CFM_T;
typedef LE_CM_REQ_STATUS_T LE_CM_MSG_ADD_TO_WHITE_LIST_CFM_T;
typedef LE_CM_REQ_STATUS_T LE_CM_MSG_REMOVE_FROM_WHITE_LIST_CFM_T;
typedef LE_CM_REQ_STATUS_T LE_CM_MSG_SET_CHANNEL_MAP_CFM_T;

typedef LE_CM_REQ_STATUS_T LE_CM_MSG_ADD_TO_RESOLVING_LIST_CFM_T;
typedef LE_CM_REQ_STATUS_T LE_CM_MSG_REMOVE_FROM_RESOLVING_LIST_CFM_T;
typedef LE_CM_REQ_STATUS_T LE_CM_MSG_CLEAR_RESOLVING_LIST_CFM_T;

typedef LE_CM_REQ_STATUS_T LE_CM_MSG_SET_RPA_TIMEOUT_CFM_T;

typedef LE_CM_REQ_STATUS_T LE_CM_MSG_SET_DEFAULT_PHY_CFM_T;


typedef struct
{
	UINT16				status;                        /**< refer to LE_ERR_STATE in ble_err.h */
} LE_CM_MSG_INIT_COMPLETE_CFM_T;

typedef struct
{
	UINT16				status;                        /**< refer to LE_ERR_STATE in ble_err.h */
    UINT16				handle;                        /**< connection handle */
} LE_CM_MSG_SET_DISCONNECT_CFM_T;

typedef struct
{
	UINT16				status;                        /**< refer to LE_ERR_STATE in ble_err.h */
    UINT16				conn_hdl;                      /**< connection handle */
    INT8				tx_power;                      /**< tx power */
} LE_CM_MSG_READ_TX_POWER_CFM_T;

typedef struct
{
	UINT16				status;                        /**< refer to LE_ERR_STATE in ble_err.h */
    BD_ADDR				bd_addr;                       /**< */
} LE_CM_MSG_READ_BD_ADDR_CFM_T;

typedef struct
{
	UINT16				status;                        /**< refer to LE_ERR_STATE in ble_err.h */
    UINT16				conn_hdl;                      /**< connection handle */
    INT8				rssi;                          /**< RSSI */
} LE_CM_MSG_READ_RSSI_CFM_T;

typedef struct
{
	UINT16				status;                        /**< refer to LE_ERR_STATE in ble_err.h */
    INT8				pwr_level;                     /**< power level */
} LE_CM_MSG_READ_ADV_TX_POWER_CFM_T;

typedef struct
{
	UINT16				status;                        /**< refer to LE_ERR_STATE in ble_err.h */
    UINT8				size;                          /**< white list size */
} LE_CM_MSG_READ_WHITE_LIST_SIZE_CFM_T;

typedef struct
{
	UINT16				status;                        /**< refer to LE_ERR_STATE in ble_err.h */
    UINT16				conn_hdl;                      /**< connection handle */
    UINT8				ch_map[5];                     /**< channel map */
} LE_CM_MSG_READ_CHANNEL_MAP_CFM_T;

typedef struct
{
	UINT16				status;                        /**< refer to LE_ERR_STATE in ble_err.h */
    UINT16				conn_hdl;                      /**< connection handle */
} LE_CM_MSG_SET_DATA_LENGTH_CFM_T;

typedef struct
{
    UINT16				conn_hdl;                      /**< connection handle */
    UINT16				max_tx_octets;                 /**< connMaxTxOctets */
    UINT16				max_tx_time;                   /**< connMaxTxTime */
    UINT16				max_rx_octets;                 /**< connMaxRxOctets */
    UINT16				max_rx_time;                   /**< connMaxRxTime */
} LE_CM_MSG_DATA_LEN_CHANGE_IND_T;

typedef struct
{
	UINT16				status;                        /**< refer to LE_ERR_STATE in ble_err.h */
    UINT8				size;                          /**< resolving list size */
} LE_CM_MSG_READ_RESOLVING_LIST_SIZE_CFM_T;

typedef struct
{
	UINT16				conn_hdl;                      /**< connection handle */
	UINT16				status;                        /**< refer to LE_ERR_STATE in ble_err.h */
	UINT8				reason;                        /**< disconnect reason */
} LE_CM_MSG_DISCONNECT_COMPLETE_IND_T;

typedef struct
{
	UINT8				event_type;
	UINT8				addr_type;                     /**< address type */
	BD_ADDR				addr;                          /**< address */
    INT8				rssi;                          /**< RSSI */
    UINT8				len;                           /**< */
    UINT8				data[1];                       /**< */
} LE_CM_MSG_ADVERTISE_REPORT_IND_T;

typedef struct
{
	UINT8				peer_addr_type;                /**< peer address type */
	BD_ADDR				peer_addr;                     /**< peer address */
	UINT8				direct_addr_type;              /**< direct address type */
	BD_ADDR				direct_addr;                   /**< direct address */
    INT8				rssi;                          /**< RSSI */
} LE_CM_MSG_DIRECT_ADV_REPORT_IND_T;

typedef struct
{
	UINT16				status;                        /**< refer to LE_ERR_STATE in ble_err.h */
	UINT16				conn_hdl;                      /**< connection handle */
    UINT16				dev_id;                        /**< device ID */
	UINT8				role;                          /**< master or slave */
	UINT8				peer_addr_type;                /**< peer address type */
	BD_ADDR				peer_addr;                     /**< perr address */
	UINT16				conn_interval;                 /**< connection interval */
	UINT16				conn_latency;                  /**< connection latency */
	UINT16				supervison_timeout;            /**< supervision timeout */
} LE_CM_CONNECTION_COMPLETE_IND_T;

typedef struct
{
	UINT16				status;                        /**< refer to LE_ERR_STATE in ble_err.h */
	UINT16				conn_hdl;                      /**< connection handle */
    UINT16				devid;                         /**< device ID */
    BOOL				enabled;                       /**< enable or disable */
} LE_CM_MSG_ENCRYPTION_REFRESH_IND_T;

typedef struct
{
	UINT16				status;                        /**< refer to LE_ERR_STATE in ble_err.h */
	UINT16				conn_hdl;                      /**< connection handle */
    UINT16				devid;                         /**< device ID */
	UINT8				enabled;
} LE_CM_MSG_ENCRYPTION_CHANGE_IND_T;

typedef struct
{
	UINT16				conn_hdl;                      /**< connection handle */
    UINT16				devid;                         /**< device ID */
    UINT8				rand[8];                       /**< */
	UINT16				ediv;                          /**< */
} LE_CM_MSG_LTK_REQ_IND_T;

typedef struct
{
	UINT16				conn_hdl;                      /**< connection handle */
	UINT16				identifier;                    /**< */
    UINT16 				interval_min;                  /**< mininum connection interval */
    UINT16 				interval_max;                  /**< maxinum connection interval */
    UINT16 				slave_latency;                 /**< slave latency */
    UINT32 				timeout_multiplier;            /**< */
} LE_CM_MSG_SIGNAL_UPDATE_REQ_T;

#if 0
typedef struct
{
	UINT16				conn_hdl;                      /**< connection handle */
	BOOL				accept;                        /**< accept */
} LE_CM_MSG_SIGNAL_UPDATE_RSP_T;
#endif

typedef struct
{
	UINT16				conn_hdl;                      /**< connection handle */
    UINT16				status;                        /**< refer to LE_ERR_STATE in ble_err.h */
    UINT16 				interval;                      /**< connection interval */
    UINT16 				latency;                       /**< slave letency */
    UINT32 				supervision_timeout;           /**< supervision timeout */
} LE_CM_MSG_CONN_UPDATE_COMPLETE_IND_T;

typedef struct
{
	UINT16				conn_hdl;                      /**< connection handle */
	UINT16				itv_min;                       /**< mininum connection interval */
    UINT16 				itv_max;                       /**< maxinum connection interval */
    UINT16 				latency;                       /**< slave latency */
    UINT32 				sv_tmo;                        /**< supervision timeout */
} LE_CM_MSG_CONN_PARA_REQ_T;

typedef struct
{
	UINT16				status;
    UINT16				conn_hdl;
    UINT8				tx_phy;
    UINT8				rx_phy;
} LE_CM_MSG_READ_PHY_CFM_T;

typedef struct
{
	UINT16				status;
    UINT16				conn_hdl;
} LE_CM_MSG_SET_PHY_CFM_T;

typedef LE_CM_MSG_READ_PHY_CFM_T LE_CM_MSG_PHY_UPDATE_COMPLETE_IND_T;

/** 
 * @brief   BLE Connection Management Module Init.
 *
 * @param   the reference of BLE task.
 *
 * @return  None.
 */
void LeCmInit(TASK appTask);

/**
 * @}
 */

/**
 * @}
 */

#endif
