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

#ifndef _BLE_GATT_IF_H_
#define _BLE_GATT_IF_H_

#include "ble.h"
#include "ble_msg.h"
#include "ble_att_if.h"

/** \defgroup BLE_ALL_APIs BLE ALL APIs
 * @brief BLE ALL APIs
 */

/** @addtogroup BLE_ALL_APIs
 * @{
 */

/** \defgroup BLE_GATT_APIs BLE GATT APIs
 */

/** @addtogroup BLE_GATT_APIs
 * @{
 */


/**
 * @brief  BLE GATT message id
 */
enum
{
	LE_GATT_MSG_INIT_CFM = LE_GATT_MSG_BASE,           /**< initialize confirm message */
	LE_GATT_MSG_EXCHANGE_MTU_IND,                      /**< exchange MTU indication */
	LE_GATT_MSG_EXCHANGE_MTU_CFM,                      /**< exchange MTU confirm */
	LE_GATT_MSG_ACCESS_READ_IND,                       /**< access read indication */
	LE_GATT_MSG_ACCESS_WRITE_IND,                      /**< access write indication */

	LE_GATT_MSG_SERVICE_INFO_IND,                      /**< service infomation indication */
	LE_GATT_MSG_FIND_ALL_PRIMARY_SERVICE_CFM,          /**< find all primary service confirm */
	LE_GATT_MSG_FIND_PRIMARY_SERVICE_BY_UUID_CFM,      /**< find primary service by UUID fonfirm */
	LE_GATT_MSG_INCLUDE_SERVICE_INFO_IND,              /**< include service infomation */
	LE_GATT_MSG_FIND_INCLUDED_SERVICE_CFM,             /**< find include service confirm */
	
	LE_GATT_MSG_CHARACTERISTIC_DECL_INFO_IND,          /**< characteristic declaration info indication */
	LE_GATT_MSG_FIND_CHARACTERISTIC_CFM,               /**< find characteristic confirm */

	LE_GATT_MSG_CHAR_DESCRIPTOR_INFO_IND,              /**< characteristic descriptor info indication */
	LE_GATT_MSG_FIND_ALL_CHAR_DESC_CFM,                /**< find all characteristic descriptors confirm */

	LE_GATT_MSG_CHARACTERISTIC_VAL_IND,                /**< characteristic value, indication message */
	LE_GATT_MSG_READ_CHARACTERISTIC_VALUE_CFM,         /**< read characteristic value, confirm message */
	LE_GATT_MSG_READ_CHAR_VAL_BY_UUID_CFM,             /**< read characteristic value by UUID confirm message */
	LE_GATT_MSG_READ_LONG_CHAR_VAL_CFM,                /**< read long characteristic value confirm mesage */
	
	LE_GATT_MSG_READ_MULTIPLE_CHAR_VAL_CFM,            /**< read multiple characteristic value confirm  */

	LE_GATT_MSG_WRITE_CHAR_VALUE_CFM,                  /**< write characteristic value confirm */
	LE_GATT_MSG_WRITE_LONG_CHAR_VALUE_CFM,             /**< write long characteristic value confirm */
	LE_GATT_MSG_WRITE_CHAR_VAL_RELIABLE_CFM,           /**< write characteristic value reliable confirm */

	LE_GATT_MSG_PREPARE_WRITE_RELIABLE_CFM,            /**< prepare write reliable confirm */
	LE_GATT_MSG_EXECUTE_WRITE_RELIABLE_CFM,            /**< execute write reliable confirm */

	LE_GATT_MSG_WRITE_NO_RSP_CFM,                      /**< write no response confirm */
	LE_GATT_MSG_SIGNED_WRITE_CFM,                      /**< signed write confirm */
	LE_GATT_MSG_NOTIFY_IND,                            /**< notify indication */
	LE_GATT_MSG_NOTIFY_CFM,                            /**< notify confirm */
	LE_GATT_MSG_INDICATE_IND,                          /**< indicate indication*/
	LE_GATT_MSG_CONFIRMATION_CFM,                      /**< confirmation confirm  */

	LE_GATT_MSG_OPERATION_TIMEOUT,                     /**< operation timeout */
	LE_GATT_MSG_SIGN_RESOLUTION_FAIL,                  /**< sign resolution fail */
	
	LE_GATT_MSG_TOP                                    /**< top of GATT message id */
};


#define LE_ATT_UUID_SIZE						   2

#define GATT_PRIMARY_SERVICE_UUID                  0x2800
#define GATT_SECONDARY_SERVICE_UUID                0x2801
#define GATT_INCLUDE_UUID						   0x2802
#define GATT_CHARACTERISTIC_UUID				   0x2803

#define GATT_CHAR_EXT_PROPS_UUID                   0x2900
#define GATT_CHAR_USER_DESC_UUID				   0x2901
#define GATT_CLIENT_CHAR_CFG_UUID				   0x2902
#define GATT_SERV_CHAR_CFG_UUID				       0x2903
#define GATT_CHAR_FORMAT_UUID					   0x2904
#define GATT_CHAR_AGG_FORMAT_UUID				   0x2905
#define GATT_VALID_RANGE_UUID					   0x2906
#define GATT_EXT_REPORT_REF_UUID				   0x2907
#define GATT_REPORT_REF_UUID					   0x2908


extern const UINT16 gcPrimaryServiceUuid;
extern const UINT16 gcSecondaryServiceUuid;
extern const UINT16 gcIncludeUuid;
extern const UINT16 gcCharacteristicUuid;

extern const UINT16 gcCharExtPropUuid;
extern const UINT16 gcCharUserDescUuid;
extern const UINT16 gcClientCharConfigUuid;
extern const UINT16 gcServerCharConfigUuid;
extern const UINT16 gcCharFormatUuid;
extern const UINT16 gcCharAggregateUuid;
extern const UINT16 gcValidRangeUuid;
extern const UINT16 gcExtReportRefUuid;
extern const UINT16 gcReportRefUuid;


#define LE_GATT_PERMIT_READ                 (0x0001)
#define LE_GATT_PERMIT_WRITE                (0x0002)
#define LE_GATT_PERMIT_AUTHOR_READ          (0x0004)
#define LE_GATT_PERMIT_AUTHOR_WRITE         (0x0008)
#define LE_GATT_PERMIT_ENCRYPT_READ         (0x0010)
#define LE_GATT_PERMIT_ENCRYPT_WRITE        (0x0020)
#define LE_GATT_PERMIT_AUTHEN_READ          (0x0040)
#define LE_GATT_PERMIT_AUTHEN_WRITE         (0x0080)
#define LE_GATT_PERMIT_SC_AUTHEN_READ       (0x0100)
#define LE_GATT_PERMIT_SC_AUTHEN_WRITE      (0x0200)

#define LE_GATT_PERMIT_READABLE			    (LE_GATT_PERMIT_READ  | LE_GATT_PERMIT_AUTHEN_READ  | LE_GATT_PERMIT_AUTHOR_READ  | LE_GATT_PERMIT_ENCRYPT_READ  | LE_GATT_PERMIT_SC_AUTHEN_READ)
#define LE_GATT_PERMIT_WRITABLE			    (LE_GATT_PERMIT_WRITE | LE_GATT_PERMIT_AUTHEN_WRITE | LE_GATT_PERMIT_AUTHOR_WRITE | LE_GATT_PERMIT_ENCRYPT_WRITE | LE_GATT_PERMIT_SC_AUTHEN_WRITE)


#define LE_GATT_PERM_NONE                   (0x00)
#define LE_GATT_PERM_READ					(0x1<<1)
#define LE_GATT_PERM_WRITE_CMD              (0x1<<2)
#define LE_GATT_PERM_WRITE_REQ              (0x1<<3)
#define LE_GATT_PERM_AUTH_READABLE          (0x1<<4)
#define LE_GATT_PERM_RELIABLE_WRITE         (0x1<<5)
#define LE_GATT_PERM_AUTH_WRITABLE          (0x1<<6)

/// Characteristic Properties Bit
#define LE_GATT_CHAR_PROP_BCAST             0x01
#define LE_GATT_CHAR_PROP_RD                0x02
#define LE_GATT_CHAR_PROP_WR_NO_RESP        0x04
#define LE_GATT_CHAR_PROP_WR                0x08
#define LE_GATT_CHAR_PROP_NTF               0x10
#define LE_GATT_CHAR_PROP_IND               0x20
#define LE_GATT_CHAR_PROP_AUTH              0x40
#define LE_GATT_CHAR_PROP_EXT_PROP          0x80

#define LE_GATT_EXT_PROP_RELIABLE_WR		0x0001
#define LE_GATT_EXT_PROP_WR_AUX				0x0002

#define LE_GATT_FLAG_WRITE_REQ				0x00
#define LE_GATT_FLAG_WRITE_CMD				0x01
#define LE_GATT_FLAG_PREPARE_WRITE			0x02

#define LE_GATT_CLIENT_CFG_NOTIFICATION		0x01
#define LE_GATT_CLIENT_CFG_INDICATION		0x02


#define PRIMARY_SERVICE_DECL_UUID16(pUuid)  						{0, LE_GATT_UUID16,  (UINT16 *)&gcPrimaryServiceUuid,   LE_GATT_PERMIT_READ,   0,   2, (UINT8 *)(pUuid)}
#define PRIMARY_SERVICE_DECL_UUID128(pUuid)							{0, LE_GATT_UUID16,  (UINT16 *)&gcPrimaryServiceUuid,   LE_GATT_PERMIT_READ,   0,  16, (UINT8 *)(pUuid)}
#define SECONDARY_SERVICE_DECL_UUID16(pUuid)  						{0, LE_GATT_UUID16,  (UINT16 *)&gcSecondaryServiceUuid, LE_GATT_PERMIT_READ,   0,   2, (UINT8 *)(pUuid)}
#define SECONDARY_SERVICE_DECL_UUID128(pUuid)						{0, LE_GATT_UUID16,  (UINT16 *)&gcSecondaryServiceUuid, LE_GATT_PERMIT_READ,   0,  16, (UINT8 *)(pUuid)}
#define INCLUDE_DECL_UUINT16(pVal)  								{0, LE_GATT_UUID16,  (UINT16 *)&gcIncludeUuid, 		    LE_GATT_PERMIT_READ,   0,   6, (UINT8 *)(pVal)}
#define INCLUDE_DECL_UUID128(pVal)									{0, LE_GATT_UUID16,  (UINT16 *)&gcIncludeUuid, 		    LE_GATT_PERMIT_READ,   0,   4, (UINT8 *)(pVal)}

#define CHARACTERISTIC_DECL_UUID16(pVal) 							{0, LE_GATT_UUID16,  (UINT16 *)&gcCharacteristicUuid,   LE_GATT_PERMIT_READ,   0,   5, (UINT8 *)(pVal)}
#define CHARACTERISTIC_DECL_UUID128(pVal) 							{0, LE_GATT_UUID16,  (UINT16 *)&gcCharacteristicUuid,   LE_GATT_PERMIT_READ,   0,  19, (UINT8 *)(pVal)}
#define CHARACTERISTIC_UUID16(pUuid, permit, maxLen, len, pVal) 	{0, LE_GATT_UUID16,  (UINT16 *)pUuid,                   permit,           maxLen, len, (UINT8 *)(pVal)}
#define CHARACTERISTIC_UUID128(pUuid, permit, maxLen, len, pVal) 	{0, LE_GATT_UUID128, (UINT16 *)pUuid,                   permit,           maxLen, len, (UINT8 *)(pVal)}

#define CHAR_EXT_PROP_DESCRIPTOR(pVal)								{0, LE_GATT_UUID16,  (UINT16 *)&gcCharExtPropUuid,      LE_GATT_PERMIT_READ,   0,   2, (UINT8 *)(pVal)}
#define CHAR_USER_DESC_DESCRIPTOR(permit, maxLen, len, pVal)		{0, LE_GATT_UUID16,  (UINT16 *)&gcCharUserDescUuid,     permit,           maxLen, len, (UINT8 *)(pVal)}
#define CHAR_CLIENT_CONFIG_DESCRIPTOR(permit, pVal)					{0, LE_GATT_UUID16,  (UINT16 *)&gcClientCharConfigUuid, LE_GATT_PERMIT_READ | permit,  0,   2, (UINT8 *)(pVal)}
#define CHAR_SERVER_CONFIG_DESCRIPTOR(permit, pVal)					{0, LE_GATT_UUID16,  (UINT16 *)&gcServerCharConfigUuid, LE_GATT_PERMIT_READ | permit,  0,   2, (UINT8 *)(pVal)}
#define CHAR_PRESENT_FORMAT_DESCRIPTOR(pVal)						{0, LE_GATT_UUID16,  (UINT16 *)&gcCharFormatUuid,       LE_GATT_PERMIT_READ,   0,   7, (UINT8 *)(pVal)}
#define CHAR_AGGREGATE_DESCRIPTOR(len, pVal)						{0, LE_GATT_UUID16,  (UINT16 *)&gcCharAggregateUuid,    LE_GATT_PERMIT_READ,   0, len, (UINT8 *)(pVal)}

#define INCLUDE_DECL_UUID16_ATTR_VAL(uuid)							{0, 0, 0, 0, UINT16_LO(uuid), UINT16_HI(uuid)}
#define INCLUDE_DECL_UUID128_ATTR_VAL()								{0, 0, 0, 0}

#define CHAR_DECL_UUID16_ATTR_VAL(prop, type)						{(prop), 0, 0, UINT16_LO(type), UINT16_HI(type)}


typedef struct
{
	UINT16					handle;                    /**< handle */
	UINT8					format;                    /**< UUID type */
	UINT16 * const			pUuid;                     /**< UUID */
	UINT16					permit;                    /**< permit */
	UINT16					maxLen;				       /**< maxinum value length */
	UINT16					len;					   /**< value length */
	UINT8 * const			pVal;					   /**< value */
} LE_GATT_ATTR_T;

typedef struct
{
	UINT16					svc_id;                    /**< service ID */
	UINT16					startHdl;                  /**< start handle */
	UINT16					endHdl;                    /**< end handle */
	LE_GATT_ATTR_T			*pAttr;                    /**< pointer attribute table */
} LE_GATT_SERVICE_T;

typedef struct
{
	UINT16					conn_hdl;                  /**< connection handle */
	UINT16					devid;                     /**< device index */
	UINT16					handle;                    /**< attribute handle */
	UINT16					offset;                    /**< attribute handle value */
} LE_GATT_MSG_ACCESS_READ_IND_T;

typedef struct
{
	UINT16					conn_hdl;                  /**< connection handle */
	UINT16					devid;                     /**< device ID */
	UINT8					flag;                      /**< refer to LE_GATT_FLAG_* in ble_gatt_if.h */
	UINT16					handle;                    /**< attribute handle */
	UINT16					offset;                    /**< attribute handle value */
	UINT16					len;                       /**< length written */
	UINT8					*pVal;                     /**< value written */
} LE_GATT_MSG_ACCESS_WRITE_IND_T;

typedef struct
{
	UINT16					conn_hdl;                  /**< connection handle */
	UINT16					devid;                     /**< device ID */
	UINT16					client_rx_mtu;             /**< client receive MTU */
} LE_GATT_MSG_EXCHANGE_MTU_IND_T;

typedef struct
{
	UINT16					conn_hdl;                  /**< connection handle */
	UINT16					devid;                     /**< device ID */
	UINT16					current_rx_mtu;            /**< current receive MTU */
} LE_GATT_MSG_EXCHANGE_MTU_CFM_T;

typedef struct
{
	UINT16					conn_hdl;                  /**< connection handle */
	UINT16					devid;                     /**< device ID */
	UINT16					start_hdl;                 /**< start handle */
	UINT16					end_hdl;                   /**< end handle */
	UINT8					format;                    /**< UUID type */
	UINT16					uuid[8];                   /**< UUID */
} LE_GATT_MSG_SERVICE_INFO_IND_T;

typedef struct
{
	UINT16					conn_hdl;                  /**< connection handle */
	UINT16					devid;					   /**< device ID */
	UINT16					handle;                    /**< include servie handle */
	UINT16					start_hdl;				   /**< start handle */
	UINT16					end_hdl;				   /**< end handle */
	UINT8					format;                    /**< UUID type */
	UINT16					uuid[8];                   /**< UUID */
} LE_GATT_MSG_INCLUDE_SERVICE_INFO_IND_T;

typedef struct
{
	UINT16					conn_hdl;                  /**< connection handle */
	UINT16					devid;                     /**< device ID */
	UINT16					status;                    /**< refer to LE_ERR_STATE in ble_err.h */
	UINT8					att_err;                   /**< 0 is ok, others refer to LE_ATT_ERR_* in ble_att_if.h */
	UINT16					handle;
} LE_GATT_MSG_FIND_ALL_PRIMARY_SERVICE_CFM_T;

typedef struct
{
	UINT16					conn_hdl;                  /**< connection handle */
	UINT16					devid;                     /**< device ID */
	UINT16					status;                    /**< refer to LE_ERR_STATE in ble_err.h */
	UINT8					att_err;                   /**< 0 is ok, others refer to LE_ATT_ERR_* in ble_att_if.h */
	UINT16					handle;                    /**< service start handle */
} LE_GATT_MSG_FIND_PRIMARY_SERVICE_BY_UUID_CFM_T;

typedef struct
{
	UINT16					conn_hdl;                  /**< connection handle */
	UINT16					devid;                     /**< device ID */
	UINT16					status;                    /**< refer to LE_ERR_STATE in ble_err.h */
	UINT8					att_err;                   /**< 0 is ok, others refer to LE_ATT_ERR_* in ble_att_if.h */
	UINT16					handle;                    /**< include service start handle */
} LE_GATT_MSG_FIND_INCLUDED_SERVICE_CFM_T;

typedef struct
{
	UINT16					conn_hdl;                  /**< connection handle */
	UINT16					devid;                     /**< device ID */
	UINT16					handle;                    /**< characteristic declaration handle */
	UINT8					property;                  /**< property */
	UINT16					val_hdl;                   /**< characteristic value handle */
	UINT8					format;                    /**< UUID type */
	UINT16					uuid[8];                   /**< UUID */
} LE_GATT_MSG_CHARACTERISTIC_DECL_INFO_IND_T;

typedef struct
{
	UINT16					conn_hdl;                  /**< connection handle */
	UINT16					devid;                     /**< device ID */
	UINT16					status;                    /**< refer to LE_ERR_STATE in ble_err.h */
	UINT8					att_err;                   /**< 0 is ok, others refer to LE_ATT_ERR_* in ble_att_if.h */
	UINT16					handle;                    /**< characteristic descriptor handle */
} LE_GATT_MSG_FIND_CHARACTERISTIC_CFM_T;

typedef struct
{
	UINT16					conn_hdl;                  /**< connection handle */
	UINT16					devid;                     /**< device ID */
	UINT16					handle;                    /**< characteristic descriptor handle */
	UINT8					format;                    /**< UUID type */
	UINT16					uuid[8];                   /**< UUID */
} LE_GATT_MSG_CHAR_DESCRIPTOR_INFO_IND_T;

typedef struct
{
	UINT16					conn_hdl;                  /**< connection handle */
	UINT16					devid;                     /**< device ID */
	UINT16					status;                    /**< refer to LE_ERR_STATE in ble_err.h */
	UINT8					att_err;                   /**< 0 is ok, others refer to LE_ATT_ERR_* in ble_att_if.h */
	UINT16					handle;                    /**< characteristic descriptor handle */
} LE_GATT_MSG_FIND_ALL_CHAR_DESC_CFM_T;

typedef struct
{
	UINT16					conn_hdl;                  /**< connection handle */
	UINT16					devid;                     /**< device ID */
	UINT8					att_err;                   /**< 0 is ok, others refer to LE_ATT_ERR_* in ble_att_if.h */
	UINT16					handle;                    /**< characteristic value handle */
	UINT16					offset;                    /**< value position offset */
	UINT16					len;                       /**< value length */
	UINT8					*val;                      /**< value */
} LE_GATT_MSG_CHARACTERISTIC_VAL_IND_T;

typedef struct
{
	UINT16					conn_hdl;                  /**< connection handle */
	UINT16					devid;                     /**< device ID */
	UINT16					status;                    /**< refer to LE_ERR_STATE in ble_err.h */
	UINT8					att_err;                   /**< 0 is ok, others refer to LE_ATT_ERR_* in ble_att_if.h */
	UINT16					handle;                    /**< characteristic value handle */
} LE_GATT_MSG_READ_CHARACTERISTIC_VALUE_CFM_T;

typedef struct
{
	UINT16					conn_hdl;                  /**< connection handle */
	UINT16					devid;                     /**< device ID */
	UINT16					status;                    /**< refer to LE_ERR_STATE in ble_err.h */
	UINT8					att_err;                   /**< 0 is ok, others refer to LE_ATT_ERR_* in ble_att_if.h */
	UINT16					handle;                    /**< characteristic value handle */
} LE_GATT_MSG_READ_CHAR_VAL_BY_UUID_CFM_T;

typedef struct
{
	UINT16					conn_hdl;                  /**< connection handle */
	UINT16					devid;                     /**< device ID */
	UINT16					status;                    /**< refer to LE_ERR_STATE in ble_err.h */
	UINT8					att_err;                   /**< 0 is ok, others refer to LE_ATT_ERR_* in ble_att_if.h */
	UINT16					handle;                    /**< characteristic value handle */
} LE_GATT_MSG_READ_LONG_CHAR_VAL_CFM_T;

typedef struct
{
	UINT16					conn_hdl;                  /**< connection handle */
	UINT16					devid;                     /**< device ID */
	UINT16					status;                    /**< refer to LE_ERR_STATE in ble_err.h */
	UINT8					att_err;                   /**< 0 is ok, others refer to LE_ATT_ERR_* in ble_att_if.h */
	UINT16					err_hdl;                   /**< TBD */
	UINT16					len;                       /**< value length */
	UINT8					*val;                      /**< value */
} LE_GATT_MSG_READ_MULTIPLE_CHAR_VAL_CFM_T;

typedef struct
{
	UINT16					conn_hdl;                  /**< connection handle */
	UINT16					devid;                     /**< device ID */
	UINT16					status;                    /**< refer to LE_ERR_STATE in ble_err.h */
	UINT8					att_err;                   /**< 0 is ok, others refer to LE_ATT_ERR_* in ble_att_if.h */
	UINT16					handle;                    /**< attribute handle */
} LE_GATT_MSG_WRITE_CHAR_VALUE_CFM_T;

typedef struct
{
	UINT16					conn_hdl;                  /**< connection handle */
	UINT16					devid;                     /**< device ID */
	UINT16					status;                    /**< refer to LE_ERR_STATE in ble_err.h */
	UINT8					att_err;                   /**< 0 is ok, others refer to LE_ATT_ERR_* in ble_att_if.h */
	UINT16					handle;                    /**< characteristic value handle */
} LE_GATT_MSG_WRITE_LONG_CHAR_VALUE_CFM_T;

typedef struct
{
	UINT16					conn_hdl;                  /**< connection handle */
	UINT16					devid;                     /**< device ID */
	UINT16					status;                    /**< refer to LE_ERR_STATE in ble_err.h */
	UINT8					att_err;                   /**< 0 is ok, others refer to LE_ATT_ERR_* in ble_att_if.h */
	UINT16					handle;                    /**< characteristic value handle */
} LE_GATT_MSG_WRITE_CHAR_VAL_RELIABLE_CFM_T;

typedef struct
{
	UINT16					conn_hdl;                  /**< connection handle */
	UINT16					devid;                     /**< device ID */
	UINT16					status;                    /**< refer to LE_ERR_STATE in ble_err.h */
	UINT8					att_err;                   /**< 0 is ok, others refer to LE_ATT_ERR_* in ble_att_if.h */
	UINT16					handle;                    /**< attribute handle */
} LE_GATT_MSG_PREPARE_WRITE_RELIABLE_CFM_T;

typedef struct
{
	UINT16					conn_hdl;                  /**< connection handle */
	UINT16					devid;                     /**< device ID */
	UINT16					status;                    /**< refer to LE_ERR_STATE in ble_err.h */
	UINT8					att_err;                   /**< 0 is ok, others refer to LE_ATT_ERR_* in ble_att_if.h */
	UINT16					err_hdl;                   /**< TBD */
} LE_GATT_MSG_EXECUTE_WRITE_RELIABLE_CFM_T;

typedef struct
{
	UINT16					conn_hdl;                  /**< connection handle */
	UINT16					devid;                     /**< device ID */
	UINT16					handle;                    /**< attribute handle */
	UINT16					len;                       /**< value length */
	UINT8					*val;                      /**< value */
} LE_GATT_MSG_NOTIFY_IND_T;

typedef struct
{
	UINT16					conn_hdl;                  /**< connection handle */
	UINT16					devid;                     /**< device ID */
	UINT16					handle;                    /**< attribute handle */
	UINT16					len;                       /**< value length */
	UINT8					*val;                      /**< value */
} LE_GATT_MSG_INDICATE_IND_T;

typedef struct
{
	UINT16					conn_hdl;                  /**< connection handle */
	UINT16					devid;                     /**< device ID */
	UINT16					handle;                    /**< attribute handle */
} LE_GATT_MSG_CONFIRMATION_CFM_T;

typedef struct
{
	UINT16					conn_hdl;                  /**< connection handle */
	UINT16					devid;                     /**< device ID */
	UINT8					att_op;                    /**< refer to LE_ATT_OP_* in ble_att_if.h */
} LE_GATT_MSG_OPERATION_TIMEOUT_T;

typedef struct
{
	UINT16					conn_hdl;                  /**< connection handle */
	UINT16					devid;                     /**< device ID */
	UINT16					handle;                    /**< attribute handle */
	UINT16					status;                    /**< refer to LE_ERR_STATE in ble_err.h */
} LE_GATT_MSG_WRITE_NO_RSP_CFM_T;

typedef struct
{
	UINT16					conn_hdl;                  /**< connection handle */
	UINT16					devid;                     /**< device ID */
	UINT16					handle;                    /**< attribute handle */
	UINT16					status;                    /**< refer to LE_ERR_STATE in ble_err.h */
} LE_GATT_MSG_NOTIFY_CFM_T;

typedef struct
{
	UINT16					conn_hdl;                  /**< connection handle */
	UINT16					devid;                     /**< device ID */
	UINT16					handle;                    /**< attribute handle */
	UINT16					status;                    /**< refer to LE_ERR_STATE in ble_err.h */
} LE_GATT_MSG_SIGNED_WRITE_CFM_T;

/**
 * @brief    BLE Gatt module init.
 *
 * @param    appTask  the reference of BLE task.
 *
 * @return   None.
 */
void LeGattInit(TASK appTask);

/**
 * @brief    Called to register a service.
 *
 * @param    attrTable  service attribute table.
 * @param    numAttr    the attribute number of service.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_GATT_SERVICE_T* LeGattRegisterService(LE_GATT_ATTR_T *attrTable, UINT16 numAttr);

/**
 * @brief    Called to register an include service.
 *
 * @param    inc_hdl    include service handle.
 * @param	 start_hdl  start handle.
 * @param	 end_hdl    end handle.
 * @param	 uuid       include service UUID.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGattRegisterIncludeService(UINT16 inc_hdl, UINT16 start_hdl, UINT16 end_hdl, UINT16 uuid);

/**
 * @brief    Get attribute handle.
 *
 * @param    svc     service.
 * @param    attrId  attribute index of service.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
UINT16 LeGattGetAttrHandle(LE_GATT_SERVICE_T *svc, UINT16 attrId);

/**
 * @brief    Get the length of attribute value.
 *
 * @param    svc     service.
 * @param    attrId  attribute index of service.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
UINT16 LeGattGetAttrValLen(LE_GATT_SERVICE_T *svc, UINT16 attrId);

/**
 * @brief    Get the max length of attribute value.
 *
 * @param    svc     service.
 * @param    attrId  attribute index of service.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
UINT16 LeGattGetAttrValMaxLen(LE_GATT_SERVICE_T *svc, UINT16 attrId);

/**
 * @brief    Get attribute value.
 *
 * @param      svc     service.
 * @param      attrId  attribute index of service.
 * @param[out] len     attribute value length.
 * @param[out] val     attribute value.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGattGetAttrVal(LE_GATT_SERVICE_T *svc, UINT16 attrId, UINT16 *len, void *val);

/**
 * @brief    Change attribute value.
 *
 * @param      svc     service.
 * @param      attrId  attribute index of service.
 * @param[in]  len     attribute value length.
 * @param[in]  val     attribute value.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGattChangeAttrVal(LE_GATT_SERVICE_T *svc, UINT16 attrId, UINT16 len, void *val);

/**
 * @brief    Modify attribute value.
 *
 * @param    svc     servie.
 * @param    attrId  attribute index of service.
 * @param    offset  modify offset.
 * @param    len     modify length.
 * @param    val     modify value.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGattModifyAttrVal(LE_GATT_SERVICE_T *svc, UINT16 attrId, UINT16 offset, UINT16 len, void *val);

/**
 * @brief    Gatt access read response.
 *
 * @param    conn_hdl  connection handle.
 * @param    handle    attribute handle.
 * @param    att_err   0 is OK, others refer to LE_ATT_ERR_* in ble_att_if.h.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGattAccessReadRsp(UINT16 conn_hdl, UINT16 handle, UINT8 att_err);

/**
 * @brief    Gatt access write response.
 *
 * @param    conn_hdl  connection handle.
 * @param    method    refer to LE_GATT_FLAG_* in ble_gatt_if.h
 * @param    handle    attribute handle.
 * @param    att_err   0 is OK, others refer to LE_ATT_ERR_* in ble_att_if.h.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGattAccessWriteRsp(UINT16 conn_hdl, UINT8 method, UINT16 handle, UINT8 att_err);

/**
 * @brief    Stop current procedure.
 *
 * @param    conn_hdl  connection handle.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
void LeGattStopCurrentProcedure(UINT16 conn_hdl);

/**
 * @brief    Exchange MTU request.
 *
 * @param    conn_hdl  connection handle.
 * @param    mtu       MTU.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGattExchangeMtuReq(UINT16 conn_hdl, UINT16 mtu);

/**
 * @brief    Exchange MTU response.
 *
 * @param    conn_hdl  connection handle.
 * @param    mtu       MTU.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGattExchangeMtuRsp(UINT16 conn_hdl, UINT16 mtu);

/**
 * @brief    Find all primary service.
 *
 * @param    conn_hdl  connection handle.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGattFindAllPrimaryService(UINT16 conn_hdl);

/**
 * @brief    Find primary service by UUID.
 *
 * @param    conn_hdl  connection handle.
 * @param    format    UUID type.
 * @param    uuid      UUID.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGattFindPrimaryServiceByUuid(UINT16 conn_hdl, UINT8 format, UINT16 *uuid);

/**
 * @brief    Find include service.
 *
 * @param    conn_hdl   connection handle.
 * @param    start_hdl  start handle.
 * @param    end_hdl    end handle.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGattFindIncludedService(UINT16 conn_hdl, UINT16 start_hdl, UINT16 end_hdl);

/**
 * @brief    Find all characteristic.
 *
 * @param    conn_hdl   connection handle.
 * @param    start_hdl  start handle.
 * @param    end_hdl    end handle.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGattFindAllCharacteristic(UINT16 conn_hdl, UINT16 start_hdl, UINT16 end_hdl);

/**
 * @brief    Find characteristic by UUID.
 *
 * @param    conn_hdl   connection handle.
 * @param    start_hdl  start handle.
 * @param    end_hdl    end handle.
 * @param    format     UUID type.
 * @param    uuid       UUID.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGattFindCharacteristicByUuid(UINT16 conn_hdl, UINT16 start_hdl, UINT16 end_hdl, UINT8 format, UINT16 *uuid);

/**
 * @brief    Find all characteristic description.
 *
 * @param    conn_hdl   connection handle.
 * @param    start_hdl  start handle.
 * @param    end_hdl    end handle.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGattFindAllCharDescriptor(UINT16 conn_hdl, UINT16 start_hdl, UINT16 end_hdl);

/**
 * @brief    Read a characteristic value.
 *
 * @param    conn_hdl  connection handle.
 * @param    handle    characteristic value handle.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGattReadCharValue(UINT16 conn_hdl, UINT16 handle);

/**
 * @brief    Read a characteristic value by UUID.
 *
 * @param    conn_hdl   connection handle.
 * @param    start_hdl  start handle.
 * @param    end_hdl    end handle.
 * @param    format     UUID type.
 * @param    uuid       UUID.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGattReadCharValByUuid(UINT16 conn_hdl, UINT16 start_hdl, UINT16 end_hdl, UINT8 format, UINT16 *uuid);

/**
 * @brief    Read a long characteristic value.
 *
 * @param    conn_hdl  connection handle.
 * @param    handle    characteristic value handle.
 * @param    offset    characteristic value offset.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGattReadLongCharVal(UINT16 conn_hdl, UINT16 handle, UINT16 offset);

/**
 * @brief    Read Multiple characteristic values.
 *
 * @param    conn_hdl  connection handle.
 * @param    count     handle count.
 * @param    handle    handle table.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGattReadMultipleCharVal(UINT16 conn_hdl, UINT16 count, UINT16 *handle);

/**
 * @brief    Write characteristic value.
 *
 * @param    conn_hdl  connection handle.
 * @param    handle    characteristic value handle.
 * @param    len       length of the data to be written.
 * @param    val       the value to be written.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGattWriteCharVal(UINT16 conn_hdl, UINT16 handle, UINT16 len, UINT8 *val);

/**
 * @brief    Write without response.
 *
 * @param    conn_hdl  connection handle.
 * @param    handle    characteristic value handle.
 * @param    len       length of the data to be written.
 * @param    val       the value to be written.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGattWriteNoRsp(UINT16 conn_hdl, UINT16 handle, UINT16 len, UINT8 *val);

/**
 * @brief    Signed write without response.
 *
 * @param    conn_hdl  connection handle.
 * @param    handle    characteristic value handle.
 * @param    len       length of the data to be written.
 * @param    val       the value to be written.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGattSignedWriteNoRsp(UINT16 conn_hdl, UINT16 handle, UINT16 len, UINT8 *val);

/**
 * @brief    Write long characteristic value.
 *
 * @param    conn_hdl  connection handle.
 * @param    handle    characteristic value handle.
 * @param    offset    value position offset.
 * @param    len       length of the data to be written.
 * @param    val       the value to be written.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGattWriteLongCharVal(UINT16 conn_hdl, UINT16 handle, UINT16 offset, UINT16 len, UINT8 *val);

/**
 * @brief    Write characteristic value reliable.
 *
 * @param    conn_hdl  connection handle.
 * @param    handle    characteristic value handle.
 * @param    offset    offset written.
 * @param    len       length written.
 * @param    val       value.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGattWriteCharValReliable(UINT16 conn_hdl, UINT16 handle, UINT16 offset, UINT16 len, UINT8 *val);

/**
 * @brief    Prepare write characteristic value request.
 *
 * @param    conn_hdl  connection handle.
 * @param    handle    characteristic value handle.
 * @param    offset    offset written.
 * @param    len       length written.
 * @param    val       value.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGattPrepareWriteCharValReliable(UINT16 conn_hdl, UINT16 handle, UINT16 offset, UINT16 len, UINT8 *val);

/**
 * @brief    Execute write characteristic value request.
 *
 * @param    conn_hdl  connection handle.
 * @param    yesno     execute write or not.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGattExecuteWriteCharValReliable(UINT16 conn_hdl, BOOL yesno);

/**
 * @brief    Prepare write characteristic value response.
 *
 * @param    conn_hdl  connection handle.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGattCharValConfirmation(UINT16 conn_hdl);

/**
 * @brief    Gatt characteristic value notification.
 *
 * @param    conn_hdl  connection handle.
 * @param    hdl       characteristic value handle.
 * @param    len       value length.
 * @param    pval      value.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGattCharValNotify(UINT16 conn_hdl, UINT16 hdl, UINT16 len, UINT8 *pval);

/**
 * @brief    Gatt characteristic value indication.
 *
 * @param    conn_hdl  connection handle.
 * @param    hdl       characteristic value handle.
 * @param    len       value length.
 * @param    pval      value.
 *
 * @return
 *         - SYS_ERR_SUCCESS: success.
 *         - others: refer to error code in ble_err.h.
 */
LE_ERR_STATE LeGattCharValIndicate(UINT16 conn_hdl, UINT16 hdl, UINT16 len, UINT8 *pval);

/**
 * @}
 */

/**
 * @}
 */

#endif
