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

#ifndef __SYS_CFG_PATCH_H__
#define __SYS_CFG_PATCH_H__


#include "sys_cfg.h"


typedef enum
{
    SYS_CFG_AUXADC_VAL_GET = 0,

    SYS_CFG_AUXADC_MAX
} T_AuxAdcType;

typedef struct
{
    uint8_t u8MaxRetry;
    uint8_t u8HighPriority;
} T_AuxAdcCfg;

typedef struct
{
    uint8_t u8Type;
    uint8_t u8Src;
    uint8_t u8GpioIdx;
    uint8_t u8AdcAlwaysOn;

    uint32_t u32RepeatCnt;
    uint32_t u32PuEnDelay_Us;
} T_AuxAdcReq;

typedef struct
{
    uint8_t u8Type;
    uint8_t u8aRsv[3];
    uint32_t u32Value;
} T_AuxAdcRsp;

typedef int (*T_SysCfgReqFp)(void *pReq, void *pRsp);
typedef int (*T_SysCfgRspFp)(uint8_t u8Statue, void *pRsp);


extern T_SysCfgM0SetFp sys_cfg_auxadc_set;
extern T_SysCfgReqFp sys_cfg_auxadc_req;
extern T_SysCfgRspFp sys_cfg_auxadc_rsp;

void sys_cfg_pre_init_patch(void);

int sys_cfg_rf_init_patch(T_RfCfg *ptCfg);


#endif //#ifndef __SYS_CFG_PATCH_H__

