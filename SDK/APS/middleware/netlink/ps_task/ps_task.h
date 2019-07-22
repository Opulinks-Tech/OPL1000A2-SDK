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

#include "cmsis_os.h"

typedef TickType_t (*t_ps_adjust_tick_fp)(TickType_t tick);
typedef void (*t_ps_task_create_fp)(void);
typedef void (*t_ps_task_delete_fp)(void);

extern t_ps_adjust_tick_fp ps_adjust_tick;
extern t_ps_task_create_fp ps_task_create;
extern t_ps_task_delete_fp ps_task_delete;

extern uint32_t ps_sleep_time_s;
extern uint8_t ps_warm_boot_pybass_overflow_check;

void ps_pre_init(void);
TickType_t ps_adjust_tick_impl(TickType_t tick);
void ps_task_create_impl(void);
void ps_task_delete_impl(void);
void ps_task(void *argument);
void timer_sleep_test(int sleep_time_s);
