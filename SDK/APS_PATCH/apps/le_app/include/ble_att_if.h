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

#ifndef _BLE_ATT_IF_H_
#define _BLE_ATT_IF_H_


#define LE_ATT_INVALID_HANDLE			   0
#define LE_ATT_VALID_HANDLE_MAX			   0xFFFF

#define LE_ATT_ERR_INVALID_HANDLE          0x01
#define LE_ATT_ERR_READ_NOT_PERMITTED      0x02
#define LE_ATT_ERR_WRITE_NOT_PERMITTED     0x03
#define LE_ATT_ERR_INVALID_PDU             0x04
#define LE_ATT_ERR_INSUFFICIENT_AUTHEN     0x05
#define LE_ATT_ERR_REQ_NOT_SUPPORTED       0x06
#define LE_ATT_ERR_INVALID_OFFSET          0x07
#define LE_ATT_ERR_INSUFFICIENT_AUTHOR     0x08
#define LE_ATT_ERR_PREPARE_QUEUE_FULL      0x09
#define LE_ATT_ERR_ATTR_NOT_FOUND          0x0A
#define LE_ATT_ERR_ATTR_NOT_LONG           0x0B
#define LE_ATT_ERR_INSUFFICIENT_KEY_SZ     0x0C
#define LE_ATT_ERR_INVALID_ATTR_VALUE_LEN  0x0D
#define LE_ATT_ERR_UNLIKELY                0x0E
#define LE_ATT_ERR_INSUFFICIENT_ENC        0x0F
#define LE_ATT_ERR_UNSUPPORTED_GROUP       0x10
#define LE_ATT_ERR_INSUFFICIENT_RES        0x11


#define LE_ATT_OP_ERROR_RSP                0x01
#define LE_ATT_OP_EXCHANGE_MTU_REQ         0x02
#define LE_ATT_OP_EXCHANGE_MTU_RSP         0x03
#define LE_ATT_OP_FIND_INFO_REQ            0x04
#define LE_ATT_OP_FIND_INFO_RSP            0x05
#define LE_ATT_OP_FIND_TYPE_VALUE_REQ      0x06
#define LE_ATT_OP_FIND_TYPE_VALUE_RSP      0x07
#define LE_ATT_OP_READ_TYPE_REQ            0x08
#define LE_ATT_OP_READ_TYPE_RSP            0x09
#define LE_ATT_OP_READ_REQ                 0x0A
#define LE_ATT_OP_READ_RSP                 0x0B
#define LE_ATT_OP_READ_BLOB_REQ            0x0C
#define LE_ATT_OP_READ_BLOB_RSP            0x0D
#define LE_ATT_OP_READ_MULTI_REQ           0x0E
#define LE_ATT_OP_READ_MULTI_RSP           0x0F
#define LE_ATT_OP_READ_GROUP_TYPE_REQ      0x10
#define LE_ATT_OP_READ_GROUP_TYPE_RSP      0x11
#define LE_ATT_OP_WRITE_REQ                0x12
#define LE_ATT_OP_WRITE_RSP                0x13
#define LE_ATT_OP_WRITE_CMD                0x52
#define LE_ATT_OP_SIGNED_WRITE_CMD         0xD2
#define LE_ATT_OP_PREPARE_WRITE_REQ        0x16
#define LE_ATT_OP_PREPARE_WRITE_RSP        0x17
#define LE_ATT_OP_EXECUTE_WRITE_REQ        0x18
#define LE_ATT_OP_EXECUTE_WRITE_RSP        0x19

#define LE_ATT_OP_NOTIFY_REQ               0x1B
#define LE_ATT_OP_INDICATE_REQ             0x1D
#define LE_ATT_OP_INDICATE_RSP             0x1E

#endif
