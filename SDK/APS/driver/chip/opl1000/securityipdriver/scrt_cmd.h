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

#ifndef __SCRT_CMD_H__
#define __SCRT_CMD_H__


#include "cmsis_os.h"


#define SCRT_CMD


#ifdef SCRT_CMD

// internal
typedef void (*scrt_sut_task_delete_fp_t)(void);
typedef int (*scrt_sut_task_create_fp_t)(uint8_t bValue);

// external
typedef void (*nl_scrt_cmd_fp_t)(char *sCmd);


// internal
extern os_pthread scrt_sut_task_main_1;
extern os_pthread scrt_sut_task_main_2;
extern os_pthread scrt_sut_task_main_3;
extern os_pthread scrt_sut_task_main_4;
extern scrt_sut_task_delete_fp_t scrt_sut_task_delete;
extern scrt_sut_task_create_fp_t scrt_sut_task_create;

// external
extern nl_scrt_cmd_fp_t nl_scrt_cmd;


void nl_scrt_cmd_func_init(void);

#endif //#ifdef SCRT_CMD

#endif //#ifndef __SCRT_CMD_H__

