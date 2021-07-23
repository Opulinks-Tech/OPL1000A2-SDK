/* *****************************************************************************
 *  Copyright 2019, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2018
 *
 *******************************************************************************
 *
 *  @file ps_patch.h
 * 
 *  @brief 
 *  
 ******************************************************************************/

#ifndef _PS_PATCH_H_
#define _PS_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "ps.h"
#include "ps_task.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */


/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */

typedef struct
{
	uint32_t last_status;

	t_ps_proc_ctrl proc_ctrl;
	t_ps_busy_flag busy_flag;
	t_ps_snap_time snap_time;

	uint64_t wakeup_time;
	uint32_t sleep_min_limit;
	uint32_t wakeup_cost_gain;
	uint32_t wakeup_cost;
	uint8_t wakeup_cost_precise;

	uint8_t app_apply_sleep;
	uint8_t app_active_sleep;
	
	// patch for public
	uint8_t xtal_ready_cost;
	uint8_t app_apply_deep;

    uint32_t app_timer_sleep_duration;

    // for IO detect
    uint32_t app_io_time;
    uint8_t app_io_exist;
    uint8_t gpio_lock;
    volatile uint8_t lock_synchronized;
    volatile uint8_t is_insomnia;

    double tick_32k;

} t_ps_conf_patch;


/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */


/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */

void ps_patch_init(void);

int ps_sleep_patch(void);
uint32_t ps_synchronize_timers_patch(void);
void ps_update_io_time(void);
void ps_lock_gpio_apply(void);
void ps_enable_timer_sleep_patch(uint32_t sleep_duration_ms);
TickType_t ps_adjust_tick_patch(TickType_t tick);
double ps_32k_xtal_measure(uint32_t duration_ms);
void ps_set_busy_flag(uint8_t u8Idx, uint8_t u8Value);

void ps_task_create_patch(void);

#ifdef __cplusplus
}
#endif
#endif  /* _PS_PATCH_H_ */
