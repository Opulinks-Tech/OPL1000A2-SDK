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


void sys_cfg_pre_init_patch(void);

int sys_cfg_rf_init_patch(T_RfCfg *ptCfg);


#endif //#ifndef __SYS_CFG_PATCH_H__

