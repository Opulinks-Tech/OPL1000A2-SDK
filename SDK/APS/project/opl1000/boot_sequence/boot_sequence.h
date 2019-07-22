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
#ifndef _BOOT_SEQUENCE_H_
#define _BOOT_SEQUENCE_H_

#include <stdint.h>

void Boot_Sequence(void);
void Boot_PatchFuncInit(void);
uint32_t Boot_CheckWarmBoot(void);
uint32_t Boot_SendMultiData(char *data);

void Boot_WarmBootHook_impl(void);
void Boot_CheckBootstrap_impl(void);
void Boot_FTTestMode_impl(void);
void Boot_BusyLoop_impl(void);
void Boot_BeforeApplyPatch_impl(void);
void Boot_PrepareM0PatchOK_impl(void);
void Boot_PreInitWarm_impl(void);
void Boot_MainPatch_impl(void);

typedef void (*T_Boot_WarmBootHookFp)(void);
typedef void (*T_Boot_CheckBootstrapFp)(void);
typedef void (*T_Boot_FTTestModeFp)(void);
typedef void (*T_Boot_BusyLoopFp)(void);
typedef void (*T_Boot_BeforeApplyPatchFp)(void);
typedef void (*T_Boot_PrepareM0PatchOKFp)(void);
typedef void (*T_Boot_PreInitWarmFp)(void);
typedef void (*T_Boot_MainPatchFp)(void);

extern T_Boot_WarmBootHookFp Boot_WarmBootHook;
extern T_Boot_CheckBootstrapFp Boot_CheckBootstrap;
extern T_Boot_FTTestModeFp Boot_FTTestMode;
extern T_Boot_BusyLoopFp Boot_BusyLoop;
extern T_Boot_BeforeApplyPatchFp Boot_BeforeApplyPatch;
extern T_Boot_PrepareM0PatchOKFp Boot_PrepareM0PatchOK;
extern T_Boot_PreInitWarmFp Boot_PreInitWarm;
extern T_Boot_MainPatchFp Boot_MainPatch;

#if defined (__RTL_SIMULATION__)
#define DEBUG_ADDR      0x70000000
#endif

#define BOOT_UART_TIMEOUT   1000

#endif /* _BOOT_SEQUENCE_H_ */
