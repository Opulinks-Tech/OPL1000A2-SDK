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
/**
 * @file at_cmd_sys.c
 * @author Michael Liao
 * @date 14 Dec 2017
 * @brief File supports the System module AT Commands.
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "cmsis_os.h"
#include "at_cmd.h"
#include "at_cmd_sys.h"
#include "at_cmd_common.h"
#include "at_cmd_data_process.h"
#include "hal_tick.h"
#include "hal_system.h"
#include "hal_uart.h"
#include "hal_dbg_uart.h"
#include "svn_rev.h"
#include "mw_fim.h"
#include "mw_fim_default_group01.h"
#include "mw_fim_default_group02.h"
#include "mw_fim_default_group03.h"
#include "ps_patch.h"
#include "sys_common_api.h"
#include "sys_common_types.h"
#include "hal_flash.h"
#include "at_cmd_task.h"
#include "hal_pin.h"
#include "hal_pin_def.h"
#include "mw_ota.h"

#define CMD_TOKEN_SIZE          16
#define AT_CMD_SYS_WAIT_TIME    1000   // ms

//#define AT_FLASH_CHECK_BEFORE_WRITE
//#define AT_DEBUG
//#define AT_LOG                      msg_print_uart1
#define AT_LOG(...)

#define AT_FLASH_READ_START         0x00000000
#define AT_FLASH_READ_END           0x00100000
#define AT_FLASH_WRITE_START        0x00000000
#define AT_FLASH_WRITE_END          0x00100000
#define AT_FLASH_WRITE_ARGS_MAX     ((AT_RBUF_SIZE - 18 - 1) / 2) // (AT_RBUF_SIZE - length of "at+writeflash=x,yy") / 2
#define AT_FLASH_BUF_SIZE           32

extern uint32_t g_u32FlashReadStart;
extern uint32_t g_u32FlashReadEnd;
extern uint32_t g_u32FlashWriteStart;
extern uint32_t g_u32FlashWriteEnd;

extern size_t xFreeBytesRemaining;

extern void at_cmd_sys_gslp_wakeup_callback(PS_WAKEUP_TYPE type);
extern void at_cmd_sys_gslp_io_callback(E_GpioIdx_t eIdx);

/*
 * @brief Command at+gmr
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_sys_gmr_patch(char *buf, int len, int mode)
{
    if (AT_CMD_MODE_EXECUTION == mode)
    {
        msg_print_uart1("\r\n");

        msg_print_uart1("AT version info: %s \r\n", AT_VER);
        msg_print_uart1("SDK version info: %d \r\n", SVN_REVISION);
        msg_print_uart1("Compile time: %s \r\n", SVN_TIME_NOW);

        msg_print_uart1("\r\nOK\r\n");
    }

    return true;
}

/*
 * @brief Command at+gslp
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_sys_gslp_patch(char *buf, int len, int mode)
{
    int argc = 0;
    char *argv[AT_MAX_CMD_ARGS] = {0};

    at_cmd_buf_to_argc_argv(buf, &argc, argv, AT_MAX_CMD_ARGS);

	switch (mode)		
	{
		case AT_CMD_MODE_SET:
		{
			int sleep_duration_ms = atoi(argv[1]);
			int num = atoi(argv[2]);

			if (argc == 3)
            {
                Hal_Pin_ConfigSet(num, PIN_TYPE_GPIO_INPUT, PIN_DRIVING_FLOAT);
                ps_set_wakeup_io((E_GpioIdx_t) num, 1, INT_TYPE_LEVEL, 0, at_cmd_sys_gslp_io_callback);
            }
			ps_set_wakeup_cb(at_cmd_sys_gslp_wakeup_callback);
			ps_timer_sleep(sleep_duration_ms);

			msg_print_uart1("\r\nOK\r\n");
			break;
		}

		default:
			msg_print_uart1("\r\ndefault\r\n");
			break;
	}

    return true;
}

/*
 * @brief Command at+restore
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_sys_restore_patch(char *buf, int len, int mode)
{
    if (AT_CMD_MODE_EXECUTION == mode)
    {
        MwFim_FileWriteDefault(MW_FIM_IDX_GP03_LE_CFG, 0);
    }

    extern int at_cmd_sys_restore(char *buf, int len, int mode);
    return at_cmd_sys_restore(buf, len, mode);
}

/*
 * @brief Command at+sleep
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_sys_sleep_patch(char *buf, int len, int mode)
{
    int argc = 0;
    char *argv[AT_MAX_CMD_ARGS] = {0};

    at_cmd_buf_to_argc_argv(buf, &argc, argv, AT_MAX_CMD_ARGS);

	switch (mode)		
	{
		case AT_CMD_MODE_SET:
		{
			int slp_mode = atoi(argv[1]);
			int p1 = atoi(argv[2]);
			int p2 = atoi(argv[3]);

			switch (slp_mode)
			{
				case 0:
					ps_smart_sleep(0);
					msg_print_uart1("\r\nOK\r\n");
					break;

				case 1:
					if (argc == 3)
                    {
                        Hal_Pin_ConfigSet(p1, PIN_TYPE_GPIO_INPUT, PIN_DRIVING_FLOAT);
                        ps_set_wakeup_io((E_GpioIdx_t) p1, 1, INT_TYPE_LEVEL, 0, at_cmd_sys_gslp_io_callback);
                    }
                    ps_set_wakeup_cb(at_cmd_sys_gslp_wakeup_callback);
					ps_smart_sleep(1);
					msg_print_uart1("\r\nOK\r\n");
					break;

				case 2:
					if (argc == 4)
                    {
                        Hal_Pin_ConfigSet(p2, PIN_TYPE_GPIO_INPUT, PIN_DRIVING_FLOAT);
                        ps_set_wakeup_io((E_GpioIdx_t) p2, 1, INT_TYPE_LEVEL, 0, at_cmd_sys_gslp_io_callback);
                    }
					ps_set_wakeup_cb(at_cmd_sys_gslp_wakeup_callback);
					ps_timer_sleep(p1);
					msg_print_uart1("\r\nOK\r\n");
					break;

				case 3:
					if (argc == 3)
                    {
                        Hal_Pin_ConfigSet(p1, PIN_TYPE_GPIO_INPUT, PIN_DRIVING_FLOAT);
                        ps_set_wakeup_io((E_GpioIdx_t) p1, 1, INT_TYPE_LEVEL, 0, NULL);
                    }
					ps_deep_sleep();
					msg_print_uart1("\r\nOK\r\n");
					break;

				default:
					msg_print_uart1("\r\nERROR\r\n");
					break;
			}
			break;
		}
		default:
			msg_print_uart1("\r\nERROR\r\n");
			break;
	}

    return true;
}

/*
 * @brief Command at+fwver
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_sys_fwver_patch(char *buf, int len, int mode)
{
	uint16_t uwProjectId = 0;
    uint16_t uwChipId = 0;
    uint16_t uwFirmwareId = 0;

    switch (mode)
    {
        case AT_CMD_MODE_READ:
            if (MW_OTA_OK == MwOta_VersionGet(&uwProjectId, &uwChipId, &uwFirmwareId))
            {
                msg_print_uart1("ProjectId : %u\r\n", uwProjectId);
                msg_print_uart1("ChipId    : %u\r\n", uwChipId);
                msg_print_uart1("FwId      : %u\r\n", uwFirmwareId);
                msg_print_uart1("OK\r\n");
            }
            else
            {
                msg_print_uart1("ERROR\r\n");
            }
            break;

        default:
            msg_print_uart1("ERROR\r\n");
            break;
    }
    
    return true;
}

int at_cmd_at_slp_tmr_patch(char *buf, int len, int mode)
{
    uint32_t duration_ms = 1000;

    if (mode == AT_CMD_MODE_SET)
    {
        int argc = 0;
        char *argv[AT_MAX_CMD_ARGS] = {0};
        at_cmd_buf_to_argc_argv(buf, &argc, argv, AT_MAX_CMD_ARGS);

        duration_ms = atoi(argv[1]);
        if (duration_ms <= 0)
            duration_ms = 1;
    }

    double xtal_freq = ps_32k_xtal_measure(duration_ms);

    msg_print_uart1("\r\n32K XTAL Freq: %.2f\n\r", xtal_freq);
    return true;
}

/**
  * @brief AT Command Table for System Module
  *
  */
extern at_command_t gAtCmdTbl_Sys[32];

/*
 * @brief Global variable g_AtCmdTbl_Sys_Ptr retention attribute segment
 *
 */
extern at_command_t *g_AtCmdTbl_Sys_Ptr;

/*
 * @brief AT Command Interface Initialization for System modules
 *
 */
void at_cmd_sys_func_init_patch(void)
{
#ifdef SDK_LITE
	extern void at_cmd_sys_func_init(void);

	at_cmd_sys_func_init();

    gAtCmdTbl_Sys[1].cmd_handle = at_cmd_sys_gmr_patch;
    gAtCmdTbl_Sys[3].cmd_handle = at_cmd_sys_restore_patch;
    gAtCmdTbl_Sys[15].cmd_handle = at_cmd_sys_fwver_patch;
#else
    /** Command Table (System) */
    g_AtCmdTbl_Sys_Ptr          = gAtCmdTbl_Sys;

    gAtCmdTbl_Sys[1].cmd_handle = at_cmd_sys_gmr_patch;
    gAtCmdTbl_Sys[2].cmd_handle = at_cmd_sys_gslp_patch;
    gAtCmdTbl_Sys[3].cmd_handle = at_cmd_sys_restore_patch;
    gAtCmdTbl_Sys[7].cmd_handle = at_cmd_sys_sleep_patch;
    gAtCmdTbl_Sys[15].cmd_handle = at_cmd_sys_fwver_patch;
    gAtCmdTbl_Sys[30].cmd_handle = at_cmd_at_slp_tmr_patch;

    g_u32FlashReadStart         = AT_FLASH_READ_START;
    g_u32FlashReadEnd           = AT_FLASH_READ_END;
    g_u32FlashWriteStart        = AT_FLASH_WRITE_START;
    g_u32FlashWriteEnd          = AT_FLASH_WRITE_END;
#endif
}
