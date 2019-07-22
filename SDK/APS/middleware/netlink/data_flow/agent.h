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

#ifndef __AGENT_H__
#define __AGENT_H__


typedef enum
{
    AGENT_MSG_M0 = 0,

    AGENT_MSG_MAX
} T_AgentMsgType;

typedef struct
{
    uint32_t u32Type;   // T_AgentMsgType
    void *pParam;

    // Todo
    
} T_AgentMsg;

typedef enum
{
    M3_MSG_RSP = 0,
    M3_MSG_RF_CFG_SET,
    M3_MSG_WIFI_CFG_SET,
    M3_MSG_TS_CFG_SET,

    M3_MSG_MAX
} T_M3MsgType;

typedef enum
{
    M0_MSG_RSP = 0,
    M0_MSG_RF_CFG_WRITE,
    M0_MSG_RF_CFG_READ,

    M0_MSG_MAX
} T_M0MsgType;

typedef struct
{
    uint8_t u8Type;     // T_M3MsgType
    uint8_t u8Req;
    uint8_t u8Status;
    uint8_t u8Padding;

    uint8_t u8aData[252];
} T_M3CommMsg;

typedef struct
{
    uint8_t u8Type;     // T_M0MsgType
    uint8_t u8Req;
    uint8_t u8Status;
    uint8_t u8Padding;

    uint8_t u8aData[252];
} T_M0CommMsg;


// internal
typedef void (*T_AgentTaskMainFp)(void *pParam);
typedef void (*T_AgentMsgFreeFp)(T_AgentMsg *ptMsg);

// external
typedef void (*T_AgengCommFp)(void);
typedef int (*T_AgengMsgSendFp)(T_AgentMsg *ptMsg);


extern T_AgengCommFp agent_init;
extern T_AgengMsgSendFp agent_msg_send;

void agent_pre_init(void);


#endif //#ifndef __AGENT_H__

