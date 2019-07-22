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

#ifndef _BLE_MSG_H_
#define _BLE_MSG_H_

#include "ble.h"
#include "ble_util.h"

/** \defgroup BLE_ALL_APIs BLE ALL APIs
 * @brief BLE ALL APIs
 */

/** @addtogroup BLE_ALL_APIs
 * @{
 */

/** \defgroup BLE_MSG_APIs BLE MSG APIs
 */

/** @addtogroup BLE_MSG_APIs
 * @{
 */


#define T_SEC(s)    ((UINT32)((s) * (UINT32)1000))
#define T_MIN(m)    ((UINT32)((m) * (UINT32)1000 * (UINT32)60))
#define T_HOUR(h)   ((UINT32)((h) * (UINT32)1000 * (UINT32)60) * (UINT32)60)


#define LE_HCI_MSG_BASE				0x1000
#define LE_CM_MSG_BASE				0x1100
#define LE_L2CAP_MSG_BASE			0x1200
#define LE_SMP_MSG_BASE				0x1300
#define LE_ATT_MSG_BASE				0x1400
#define LE_GATT_MSG_BASE			0x1500
#define LE_SYS_MSG_BASE				0x8000

/**
 * @brief  BLE system message id
 */
enum
{
	LE_SYS_MSG_BUF_OVERFLOW = (LE_SYS_MSG_BASE + 1),   /**< message buffer overflow */
	LE_SYS_MSG_TOP                                     /**< top of system message id */
};


#define MESSAGE_BULID(M) M##_T *msg = PanicUnlessMalloc(sizeof(M##_T))
#define MESSAGE_DATA_BULID(M, S) M##_T *msg = PanicUnlessMalloc(sizeof(M##_T) + S)
#define MESSAGE_ALLOCATE(M, S) PanicUnlessMalloc(sizeof(M##_T) + S)
#define MESSAGE_OFFSET(M) ((UINT8 *)msg + sizeof(M##_T))


typedef void **TASKPACK;
typedef TASKPACK * Task;
typedef void const * MsgData;
typedef const UINT8 * MsgLock;

typedef Task    TASK;
typedef MsgData MESSAGE;
typedef MsgLock MSGLOCK;
typedef UINT16  MESSAGEID;
typedef UINT16  MSGSUBID;
typedef UINT32  MSGTIMER;

typedef void (*TASKHANDLER)(Task, UINT16, MsgData);

typedef struct
{
	UINT16			conn_hdl;                          /**< connection handle */

} LE_SYS_MSG_BUF_OVERFLOW_T;


/**
 * @brief    Create BLE task.
 *
 * @param    task  the reference of BLE task.
 * @param    hdl   callback handle of BLE task.
 *
 * @return   TRUE is success, FALSE is failed.
 */
BOOL LeHostCreateTask(TASK task, TASKHANDLER hdl);

/**
 * @brief    Send message to BLE task.
 *
 * @param    task   reference of BLE task.
 * @param    msgId  message ID.
 * @param    msg    message.
 *
 * @return   None.
 */
void LeSendMessage(TASK task, MESSAGEID msgId, MESSAGE msg);

/**
 * @brief    Delay, then send message to BLE task.
 *
 * @param    task   reference of BLE task.
 * @param    msgId  message ID.
 * @param    msg    message.
 * @param    delay  delay time, ms.
 *
 * @return   None.
 */
void LeSendMessageAfter(TASK task, MESSAGEID msgId, MESSAGE msg, UINT32 delay);

/**
 * @brief    Cancel the first message in queue.
 *
 * @param    task  task.
 * @param    id    message id.
 *
 * @return   True is ok, false is error.
 */
BOOL LeCancelFirstMessage(TASK task, MESSAGEID id);

/**
 * @brief    Cancel all message in queue.
 *
 * @param    task  task.
 * @param    id    message id.
 *
 * @return   0 is ok, others is error.
 */
UINT16 LeCancelAllMessage(TASK task, MESSAGEID id);

/**
 * @brief    Send message until lock is 0.
 *
 * @param    task  the task of recvice message.
 * @param    id    message id.
 * @param    msg   message.
 * @param    lock  lock number.
 *
 * @return   None.
 */
void LeSendMessageUnlock(TASK task, MESSAGEID id, MESSAGE msg, MSGLOCK lock);

/**
 * @brief    Send sub message.
 *
 * @param    task  the task of recvice message.
 * @param    msgId  message id.
 * @param    subId  sub message id.
 * @param    msg    message.
 *
 * @return   None.
 */
void LeSendSubMessage(TASK task, MESSAGEID msgId, MSGSUBID subId, MESSAGE msg);

/**
 * @brief    Delay, then send sub message.
 *
 * @param    task  the task of recvice message.
 * @param    msgId  message id.
 * @param    subId  sub message id.
 * @param    msg    message.
 * @param    delay  delay time.
 *
 * @return   None.
 */
void LeSendSubMessageAfter(TASK task, MESSAGEID msgId, MSGSUBID subId, MESSAGE msg, UINT32 delay);

/**
 * @brief    Send sub message until lock is 0.
 *
 * @param    task  the task of recvice message.
 * @param    id     message id.
 * @param    subId  sub message id.
 * @param    msg    message.
 * @param    lock   lock number.
 *
 * @return   None.
 */
void LeSendSubMessageUnlock(TASK task, MESSAGEID id, MSGSUBID subId, MESSAGE msg, MSGLOCK lock);

/**
 * @brief    Cancel the first sub message in queue.
 *
 * @param    task  the task of recvice message.
 * @param    id     message id.
 * @param    subId  sub message id.
 *
 * @return   True is ok, false is error.
 */
BOOL LeCancelFirstSubMessage(TASK task, MESSAGEID id, MSGSUBID subId);

/**
 * @brief    Cancel all sub message in queue.
 *
 * @param    task  the task of recvice message.
 * @param    id     message id.
 * @param    subId  sub message id.
 *
 * @return   0 is ok, others is error.
 */
UINT16 LeCancelAllSubMessage(TASK task, MESSAGEID id, MSGSUBID subId);

/**
 * @brief    Get sub message id.
 *
 * @param    sub message id.
 *
 * @return   0 is ok, others is error.
 */
UINT16 LeGetSubMsgId(UINT16 *s);

/**
 * @brief    message loop run.
 *
 * @return   None.
 */
void LeHostMessageLoop(void);

/**
 * @}
 */

/**
 * @}
 */

#endif
