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

#ifndef __SYS_CFG_H__
#define __SYS_CFG_H__


typedef enum
{
    SYS_CFG_CLK_22_MHZ = 0,
    SYS_CFG_CLK_44_MHZ,
    SYS_CFG_CLK_78_MHZ,
    SYS_CFG_CLK_81_MHZ,
    SYS_CFG_CLK_84_MHZ,
    SYS_CFG_CLK_87_MHZ,
    SYS_CFG_CLK_90_MHZ,
    SYS_CFG_CLK_93_MHZ,
    SYS_CFG_CLK_97_MHZ,
    SYS_CFG_CLK_101_MHZ,
    SYS_CFG_CLK_106_MHZ,
    SYS_CFG_CLK_110_MHZ,
    SYS_CFG_CLK_116_MHZ,
    SYS_CFG_CLK_122_MHZ,
    SYS_CFG_CLK_128_MHZ,
    SYS_CFG_CLK_135_MHZ,
    SYS_CFG_CLK_143_MHZ,
    SYS_CFG_CLK_176_MHZ,

    SYS_CFG_CLK_MAX
} T_SysCfgClkIdx;

typedef struct
{
    uint8_t u8HighPwrStatus;
} T_RfCfg;

typedef struct
{
    uint8_t u8WifiOnly;
} T_WifiCfg;

typedef struct
{
    uint8_t u8Enable;
    uint8_t u8DataLen;
    uint8_t u8aData[2];
} T_TsCfg;

// internal
typedef int (*T_SysCfgCommFp)(void);
typedef int (*T_SysCfgM0SetFp)(void *pCfg);
typedef int (*T_SysCfgClkSrcGetFp)(uint32_t u32ClkIdx, uint32_t *pu32ClkSrc);

// external
typedef int (*T_SysCfgInitFp)(uint8_t u8SetM0);
typedef int (*T_SysCfgSetFp)(void *pCfg, uint8_t u8SetM0);
typedef int (*T_SysCfgGetFp)(void *pCfg);
typedef int (*T_SysCfgClkSetFp)(T_SysCfgClkIdx tClkIdx);


extern T_SysCfgCommFp sys_cfg_init;
extern T_SysCfgSetFp sys_cfg_rf_set;
extern T_SysCfgGetFp sys_cfg_rf_get;
extern T_SysCfgSetFp sys_cfg_wifi_set;
extern T_SysCfgGetFp sys_cfg_wifi_get;
extern T_SysCfgCommFp sys_cfg_rsp_done;
extern T_SysCfgCommFp sys_cfg_clk_init;
extern T_SysCfgClkSetFp sys_cfg_clk_set;

void sys_cfg_pre_init(void);


#endif //#ifndef __SYS_CFG_H__

