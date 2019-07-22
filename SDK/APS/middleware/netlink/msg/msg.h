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

#ifndef __MSG_H__
#define __MSG_H__

#include <stdio.h>
#include <stdbool.h>

#define TRACER

#ifdef TRACER

//#define TRACER_SUT

#include "opulinks.h"


#define TRACER_ISR_NAME_PREFIX      "opl_isr_"

#define TRACER_ISR_HANDLE_MASK      0xFF

#define TRACER_INT_TASK_NUM_MAX     32
#define TRACER_EXT_TASK_NUM_MAX     32

#define TRACER_MSG_MAX_SIZE         256 // include '\0'
#define TRACER_QUEUE_SIZE           80 // must <= TRACER_MSG_MAX_SIZE
#define TRACER_QUEUE_NUM            64

#define TRACER_TASK_NUM_MAX         32
#define TRACER_TASK_IDX_MAX         255

#define TRACER_TASK_NAME_LEN        16 // include '\0'
#define TRACER_TASK_STACK_SIZE_MIN  128 // number of uint32_t

#define LOG_HIGH_LEVEL              0x04
#define LOG_MED_LEVEL               0x02
#define LOG_LOW_LEVEL               0x01
#define LOG_NONE_LEVEL              0x00
#define LOG_ALL_LEVEL               (LOG_HIGH_LEVEL | LOG_MED_LEVEL | LOG_LOW_LEVEL)

#define TRACER_DBG(...)


typedef enum
{
    TRACER_TYPE_LOG = 0,
    TRACER_TYPE_CLI,

    TRACER_TYPE_OPT_ADD,

    TRACER_TYPE_MAX
} T_TracerType;

typedef enum
{
    TRACER_MODE_DISABLE = 0,
    TRACER_MODE_NORMAL,
    TRACER_MODE_DRCT,

    TRACER_MODE_MAX
} T_TracerMode;

typedef struct
{
    uint32_t dwHandle;
    uint8_t bLevel;
    uint8_t bUsed;
} T_TracerOpt;

typedef struct
{
    uint8_t bType;      // log/cli
    uint8_t bLevel;

    uint32_t dwHandle;  // task handle or ISR index
} T_TracerInfo;

typedef struct
{
    T_TracerInfo tInfo;
    char baBuf[4];
} T_TracerCb;

typedef struct
{
    T_TracerCb *ptCb;
} T_TracerMsg;

typedef enum
{
    TRACER_TASK_TYPE_INTERNAL = 0,
    TRACER_TASK_TYPE_APP,

    TRACER_TASK_TYPE_MAX
} T_TracerTaskType;

typedef struct
{
    char baName[TRACER_TASK_NAME_LEN];
    uint8_t bLevel;
    uint8_t bStatus;
    uint8_t baPadding[2];
} T_TracerTaskInfo;

typedef struct
{
    uint8_t bMode;
    uint8_t bExtTaskDefLevel;
    uint8_t bNameDisplay;
    uint8_t bPadding;
    int iPriority;
    uint32_t dwStackSize;
    uint32_t dwQueueNum;
    uint32_t dwQueueSize;
} T_TracerCfg;

typedef void (*T_TracerCommonFp)(void);
typedef int (*T_TracerLogLevelSetFp)(uint8_t bIdx, uint8_t bLevel);
typedef void (*T_TracerOptSetFp)(uint8_t bMode);
typedef uint8_t (*T_TracerOptGetFp)(void);
typedef void (*T_TracerPrioritySetFp)(int iPriority);
typedef void (*T_TracerNameDisplayFp)(uint8_t bDisplay);
typedef int (*T_TracerPrintfFp)(const char *sFmt, ...);
typedef int (*T_TracerMsgFp)(uint8_t bType, uint8_t bLevel, char *sFmt, ...);

typedef void (*T_TracerProcFp)(char *sString);
typedef void (*T_TracerMsgFreeFp)(T_TracerMsg *ptMsg);
typedef void (*T_TracerOptEntryAddFp)(uint32_t dwHandle, uint8_t bLevel);
typedef void (*T_TracerNameGetFp)(uint32_t dwHandle, char *baName, uint32_t dwSize);
typedef void (*T_TracerTaskMainFp)(void *pParam);
typedef uint32_t (*T_TracerHandleGetFp)(uint8_t *pbIsr);
typedef int (*T_TracerLevelGetFp)(uint32_t dwHandle, uint8_t *pbLevel);
typedef void (*T_TracerStringSendFp)(char *sString);
typedef T_TracerTaskInfo *(*T_TracerTaskInfoGetFp)(char *baName, T_TracerTaskInfo *taTaskInfo, uint8_t bTaskNum);
typedef int (*T_TracerDefLevelFp)(uint8_t bType, uint8_t bLevel);
typedef void (*T_TracerCmdFp)(char *sCmd);
typedef int (*T_TracerTaskCfgSaveFp)(void);
typedef int (*T_TracerTaskInfoSaveFp)(uint8_t bIdx);


#define tracer_log(level, args...)          tracer_msg(TRACER_TYPE_LOG, level, args)
//#define tracer_cli(level, args...)          tracer_msg(TRACER_TYPE_CLI, level, args)
#define tracer_cli(level, args...)          tracer_drct_printf(args)

#define printf(args...)                     tracer_msg(TRACER_TYPE_LOG, LOG_HIGH_LEVEL, args)
#define msg_print                           tracer_log
#define log_print(level, tag, fmt, args...)      tracer_log(level, "[%s]:"fmt, tag, ##args)

extern T_TracerCommonFp tracer_init;
extern T_TracerLogLevelSetFp tracer_log_level_set;
extern T_TracerOptSetFp tracer_log_mode_set;
extern T_TracerOptGetFp tracer_log_mode_get;
extern T_TracerPrioritySetFp tracer_priority_set;
extern T_TracerCommonFp tracer_dump;
extern T_TracerNameDisplayFp tracer_name_display;
extern T_TracerPrintfFp tracer_drct_printf;
extern T_TracerMsgFp tracer_msg;
extern T_TracerTaskInfoGetFp tracer_task_info_get;
extern T_TracerCommonFp tracer_load;
extern T_TracerDefLevelFp tracer_def_level_set;
extern T_TracerCmdFp tracer_cmd;

void Tracer_PreInit(void);

#ifdef TRACER_SUT
int tracer_sut_task_create(uint8_t bEnableCliAt);
#endif //#ifdef TRACER_SUT

#endif //#ifdef TRACER

#endif /*__MSG_H__*/
