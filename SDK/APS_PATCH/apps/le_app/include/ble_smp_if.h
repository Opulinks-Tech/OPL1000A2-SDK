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

#ifndef _BLE_SMP_IF_H_
#define _BLE_SMP_IF_H_

#include "ble.h"
#include "ble_msg.h"

/** \defgroup BLE_ALL_APIs BLE ALL APIs
 * @brief BLE ALL APIs
 */

/** @addtogroup BLE_ALL_APIs
 * @{
 */

/** \defgroup BLE_SMP_APIs BLE SMP APIs
 */

/** @addtogroup BLE_SMP_APIs
 * @{
 */


#define LE_MAX_BOND_COUNT  8


/* IO Capability */
#define LE_SM_IO_CAP_DISP_ONLY                 		0x00    /**< display only */
#define LE_SM_IO_CAP_DISP_YES_NO               		0x01    /**< display + yes or no */
#define LE_SM_IO_CAP_KEYBOARD_ONLY             		0x02    /**< keyboard only */
#define LE_SM_IO_CAP_NO_IO                     		0x03    /**< no input and output */
#define LE_SM_IO_CAP_KEYBOARD_DISP             		0x04    /**< display + keyboard */

#define LE_SM_PAIR_OOB_NO                      		0x00
#define LE_SM_PAIR_OOB_YES                     		0x01
#define LE_SM_PAIR_MITM_NO                      	0x00
#define LE_SM_PAIR_MITM_YES                     	0x01
#define LE_SM_PAIR_SC_NO                      		0x00
#define LE_SM_PAIR_SC_YES                     		0x01

/**
 * @brief  BLE SMP message id
 */
enum
{
	LE_SMP_MSG_SLAVE_SECURITY_REQUEST_IND = LE_SMP_MSG_BASE,  /**< slave security request */

	LE_SMP_MSG_PAIRING_ACTION_IND,                            /**< pairing action indication */
	LE_SMP_MSG_PASSKEY_DISPLAY_IND,                           /**< passkey display indication */
	LE_SMP_MSG_PASSKEY_INPUT_IND,                             /**< passkey input indication */
	LE_SMP_MSG_OOB_DATA_REQUEST_IND,                          /**< OOB date request indication */
	LE_SMP_MSG_SC_OOB_DATA_REQUEST_IND,                       /**< SC OOB data request indication */
	LE_SMP_MSG_USER_CONFIRM_IND,                              /**< user confirm indication */

	LE_SMP_MSG_ENCRYPTION_CHANGE_IND,                         /**< encryption change indication */
	LE_SMP_MSG_ENCRYPTION_REFRESH_IND,                        /**< encryption refresh indication */
	LE_SMP_MSG_PAIRING_COMPLETE_IND,                          /**< pairing complete indication */
    
	LE_SMP_LONG_TERM_KEY_REQ,                                 /**< long term key request */
	LE_SMP_KEYS_IND,                                          /**< keys indication */

    LE_SMP_MSG_TOP                                            /**< top of SMP message id*/
};

enum
{
	LE_SMP_PAIR_JUST_WORK,                             /**< just work */
	LE_SMP_PAIR_OOB,                                   /**< out of band */
	LE_SMP_PAIR_PASSKEY_INPUT,                         /**< passkey entry */
	LE_SMP_PAIR_DISPLAY,                               /**< display */
	LE_SMP_PAIR_NUM_COMPARE                            /**< number compare */
};

typedef struct
{
    UINT16				conn_hdl;                      /**< connection handle */
	UINT8				bondable;                      /**< bonding */
	UINT8				mitm;                          /**< MITM */
	UINT8				sc;                            /**< secure connection */
	UINT8				keypress;                      /**< keypress status */
} LE_SMP_MSG_SLAVE_SECURITY_REQUEST_IND_T;

typedef struct
{
    UINT16				conn_hdl;                      /**< connection handle */
	UINT8				sc;                            /**< secure connection */
	UINT8				action;                        /**< refer to LE_SM_IO_CAP_* in ble_smp_if.h */
    BOOL				lost_bond;                     /**< remote lost bond */
} LE_SMP_MSG_PAIRING_ACTION_IND_T;

typedef struct
{
    UINT16				conn_hdl;                      /**< connection handle */
	UINT32				passkey;                       /**< passkey */
} LE_SMP_MSG_PASSKEY_DISPLAY_IND_T;

typedef struct
{
    UINT16				conn_hdl;                       /**< connection handle */
	UINT32				confirm_num;                   /**< confirm number */
} LE_SMP_MSG_USER_CONFIRM_IND_T;

typedef struct
{
    UINT16				conn_hdl;                      /**< connection handle */
} LE_SMP_MSG_PASSKEY_INPUT_IND_T;

typedef struct
{
    UINT16				conn_hdl;                      /**< connection handle */
} LE_SMP_MSG_OOB_DATA_REQUEST_IND_T;

typedef struct
{
    UINT16				conn_hdl;                      /**< connection handle */
} LE_SMP_MSG_SC_OOB_DATA_REQUEST_IND_T;

typedef struct
{
    UINT16				conn_hdl;                      /**< connection handle */
    BOOL				enable;                        /**< enable or disable */
} LE_SMP_MSG_ENCRYPTION_CHANGE_IND_T;

typedef struct
{
    UINT16				conn_hdl;                      /**< connection handle */
    UINT16				status;                        /**< refer to LE_ERR_STATE in ble_err.h */
} LE_SMP_MSG_ENCRYPTION_REFRESH_IND_T;

typedef struct
{
    UINT16				conn_hdl;                      /**< connection handle */
    UINT16				status;                        /**< refer to LE_ERR_STATE in ble_err.h */
    UINT8				authenticated;                 /**< authenticated */
    UINT8				sc;                            /**< secure connection */
    UINT8				bonded;                        /**< bonded */
    LE_BT_ADDR_T		peer_id_addr;                  /**< peer device address */
} LE_SMP_MSG_PAIRING_COMPLETE_IND_T;

typedef struct
{
	UINT8				rand[16];                      /**< random data */
	UINT8				confirm[16];                   /**< confirm data */
} LE_SMP_SC_OOB_DATA_T;


/**
 * @brief    BLE SMP Module Init.
 *
 * @param    appTask  the reference of BLE task.
 *
 * @return   None.
 */
void LeSmpInit(TASK appTask);

/**
 * @brief    BLE SMP security request.
 *
 * @param    conn_hdl  connection handle.
 * @param    accept    TRUE is accept, FALSE is not.
 *
 * @return   0 is Ok, others refer to SMP_ERR_* in ble_err.h.
 */
UINT16 LeSmpSecurityRsp(UINT16 conn_hdl, BOOL accept);

/**
 * @brief    BLE SMP security request.
 *
 * @param    conn_hdl  connection handle.
 *
 * @return   0 is Ok, others refer to SMP_ERR_* in ble_err.h.
 */
UINT16 LeSmpSecurityReq(UINT16 conn_hdl);

/**
 * @brief    SMP OOB present.
 *
 * @param    conn_hdl  connection handle.
 * @param    oob_present  present or not.
 *
 * @return   0 is Ok, others refer to SMP_ERR_* in ble_err.h.
 */
UINT16 LeSmpOobPresent(UINT16 conn_hdl, BOOL oob_present);

/**
 * @brief    Input passkey.
 *
 * @param    conn_hdl  connection handle.
 * @param    passkey   passkey.
 *
 * @return   None.
 */
void LeSmpPasskeyInput(UINT16 conn_hdl, UINT32 passkey);

/**
 * @brief    SMP OOB authenticate data response.
 *
 * @param    conn_hdl  connection handle.
 * @param    data      response data.
 * @param    len       data length.
 *
 * @return   None.
 */
void LeSmpOobAuthDataRsp(UINT16 conn_hdl, UINT8 *data, UINT16 len);

/**
 * @brief    SMP secure connection OOB compute confirm value.
 *
 * @param    rand     random data.
 * @param    confirm  confirm data.
 *
 * @return   0 is Ok, others refer to SMP_ERR_* in ble_err.h.
 */
UINT16 LeSmpScOobComputeConfirmVal(UINT8 *rand, UINT8 *confirm);

/**
 * @brief    OOB data response.
 *
 * @param    conn_hdl  connection handld.
 * @param    our_rand  our random data.
 * @param    peer      peer OOB data.
 *
 * @return   None.
 */
void LeSmpScOobDataRsp(UINT16 conn_hdl, UINT8 *our_rand, LE_SMP_SC_OOB_DATA_T *peer);

/**
 * @brief    User confirm response.
 *
 * @param    conn_hdl  connection handle.
 * @param    accept    yes or no.
 *
 * @return   0 is Ok, others refer to SMP_ERR_* in ble_err.h.
 */
UINT16 LeSmpUserConfirmRsp(UINT16 conn_hdl, BOOL accept);

/**
 * @brief    Set default configure for pairing.
 *
 * @param    iocap  IO capability.
 * @param    mitm   TRUE is MITM protected, FALSE is not.
 * @param    sc     TRUE is request BLE secure connection pairing, FALSE is not.
 * @param    bond   TRUE: bonding, FALSE: no bonding.
 *
 * @return   0 is Ok, others refer to SMP_ERR_* in ble_err.h.
 */
UINT16 LeSmpSetDefaultConfig(UINT8 iocap, BOOL mitm, BOOL sc, BOOL bond);

/**
 * @}
 */
UINT8 LeSmpGetBondIdFromAddr(LE_BT_ADDR_T *peer_addr);
/**
 * @}
 */

#endif
