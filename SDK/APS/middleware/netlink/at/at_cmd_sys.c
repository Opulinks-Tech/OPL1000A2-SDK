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
#include "ps.h"
#include "sys_common_api.h"
#include "sys_common_types.h"
#include "hal_flash.h"
#include "at_cmd_task.h"
#include "hal_pin.h"
#include "hal_pin_def.h"

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

RET_DATA uint32_t g_u32FlashReadStart;
RET_DATA uint32_t g_u32FlashReadEnd;
RET_DATA uint32_t g_u32FlashWriteStart;
RET_DATA uint32_t g_u32FlashWriteEnd;

extern size_t xFreeBytesRemaining;

/*
 * @brief parse the token
 *
 * @param [in] the input string
 *
 * @param [in] argc count of parameters
 *
 * @param [out] argv parameters array
 *
 * @return none
 *
 */
void Cmd_TokenParse(char* pszData, uint8_t* pubCount, uint8_t* pubaToken[])
{
    uint8_t ucaSeps[] = "?=, \t\r\n";         // ?, =, space, tab, new line
    uint8_t i = 0;
    char *pSavedPtr = NULL;

    // get the first token
    pubaToken[0] = (uint8_t*)strtok_r(pszData, (const char*)ucaSeps, &pSavedPtr);

    // get the other token
    if (NULL != pubaToken[0])
    {
        for (i=1; i<CMD_TOKEN_SIZE; i++)
        {
            pubaToken[i] = (uint8_t*)strtok_r(NULL, (const char*)ucaSeps, &pSavedPtr);
            if (NULL == pubaToken[i])
                break;
        }
    }

    // update the token count
    *pubCount = i;
}

/*
 * @brief it is a busy wait
 *
 * @param [in] none
 *
 * @param [in] none
 *
 * @return none
 *
 */
void at_cmd_sys_internal_wait(uint32_t u32MilliSec)
{
    uint32_t u32TimeOutTick = 0;
    uint32_t u32TimeOutStart = 0;

    if(u32MilliSec >= Hal_Tick_MilliSecMax())
        u32TimeOutTick = Hal_Tick_MilliSecMax() * Hal_Tick_PerMilliSec();
    else
        u32TimeOutTick = u32MilliSec * Hal_Tick_PerMilliSec();

    u32TimeOutStart = Hal_Tick_Diff(0);
    while (Hal_Tick_Diff(u32TimeOutStart) < u32TimeOutTick)
    {
        ;
    }
}

/*
 * @brief parse the config of UART
 *
 * @param [in] the input string
 *
 * @param [in] the length of input string
 *
 * @param [out] the uart configuration
 *
 * @return 0 fail 1 success
 *
 */
uint8_t at_cmd_sys_internal_uart_config_parse(char *buf, int len, T_HalUartConfig *ptUartConfig)
{
    uint8_t ubCount;
    uint8_t* pubaToken[CMD_TOKEN_SIZE];
    uint32_t ulToken;

    // pre-parser the input string
    Cmd_TokenParse(buf, &ubCount, pubaToken);

    // AT+UART_CUR=<baudrate>,<databits>,<stopbits>,<parity>,<flow control>
    if (6 != ubCount)
        return 0;

    // baudrate
    ulToken = strtoul((const char*)pubaToken[1], NULL, 0);
    if ((ulToken < 80) || (ulToken > 1000000))
        return 0;
    ptUartConfig->ulBuadrate = ulToken;

    // databits
    ulToken = strtoul((const char*)pubaToken[2], NULL, 0);
    switch (ulToken)
    {
        case 5:
            ptUartConfig->ubDataBit = DATA_BIT_5;
            break;
        case 6:
            ptUartConfig->ubDataBit = DATA_BIT_6;
            break;
        case 7:
            ptUartConfig->ubDataBit = DATA_BIT_7;
            break;
        case 8:
            ptUartConfig->ubDataBit = DATA_BIT_8;
            break;

        default:
            return 0;
            //break;
    }

    // stopbits
    ulToken = strtoul((const char*)pubaToken[3], NULL, 0);
    switch (ulToken)
    {
        case 1:
            ptUartConfig->ubStopBit = STOP_BIT_1;
            break;
        // 1.5 stopbits is only for 5 databits
        case 2:
            ptUartConfig->ubStopBit = STOP_BIT_1P5;
            if (ptUartConfig->ubDataBit != DATA_BIT_5)
                return 0;
            break;
        case 3:
            ptUartConfig->ubStopBit = STOP_BIT_2;
            break;

        default:
            return 0;
            //break;
    }

    // parity
    ulToken = strtoul((const char*)pubaToken[4], NULL, 0);
    switch (ulToken)
    {
        case 0:
            ptUartConfig->ubParity = PARITY_NONE;
            break;
        case 1:
            ptUartConfig->ubParity = PARITY_ODD;
            break;
        case 2:
            ptUartConfig->ubParity = PARITY_EVEN;
            break;

        default:
            return 0;
            //break;
    }

    // flow control
    ulToken = strtoul((const char*)pubaToken[5], NULL, 0);
    switch (ulToken)
    {
        case 0:
            ptUartConfig->ubFlowCtrl = 0;
            break;
        case 3:
            ptUartConfig->ubFlowCtrl = 1;
            break;

        default:
            return 0;
            //break;
    }

    return 1;
}

/*
 * @brief output the config of UART
 *
 * @param [out] the uart configuration
 *
 * @return 0 fail 1 success
 *
 */
uint8_t at_cmd_sys_internal_uart_config_output(T_HalUartConfig *ptUartConfig, uint8_t ubCurrent)
{
    uint32_t ulBuadrate;
    uint8_t ubDataBit;
    uint8_t ubStopBit;
    uint8_t ubParity;
    uint8_t ubFlowCtrl;

    // +UART_DEF:<baudrate>,<databits>,<stopbits>,<parity>,<flow control>

    // baudrate
    ulBuadrate = ptUartConfig->ulBuadrate;

    // databits
    switch (ptUartConfig->ubDataBit)
    {
        case DATA_BIT_5:
            ubDataBit = 5;
            break;
        case DATA_BIT_6:
            ubDataBit = 6;
            break;
        case DATA_BIT_7:
            ubDataBit = 7;
            break;
        case DATA_BIT_8:
            ubDataBit = 8;
            break;

        default:
            return 0;
            //break;
    }

    // stopbits
    switch (ptUartConfig->ubStopBit)
    {
        case STOP_BIT_1:
            ubStopBit = 1;
            break;
        case STOP_BIT_1P5:
            ubStopBit = 2;
            break;
        case STOP_BIT_2:
            ubStopBit = 3;
            break;

        default:
            return 0;
            //break;
    }

    // parity
    switch (ptUartConfig->ubParity)
    {
        case PARITY_NONE:
            ubParity = 0;
            break;
        case PARITY_ODD:
            ubParity = 1;
            break;
        case PARITY_EVEN:
            ubParity = 2;
            break;

        default:
            return 0;
            //break;
    }

    // flow control
    switch (ptUartConfig->ubFlowCtrl)
    {
        case 0:
            ubFlowCtrl = 0;
            break;
        case 1:
            ubFlowCtrl = 3;
            break;

        default:
            return 0;
            //break;
    }

    msg_print_uart1("\r\n");

    if (ubCurrent == 1)
        msg_print_uart1("+UART_CUR:%u,%u,%u,%u,%u\r\n", ulBuadrate, ubDataBit, ubStopBit, ubParity, ubFlowCtrl);
    else
        msg_print_uart1("+UART_DEF:%u,%u,%u,%u,%u\r\n", ulBuadrate, ubDataBit, ubStopBit, ubParity, ubFlowCtrl);

    msg_print_uart1("\r\nOK\r\n");

    return 1;
}

/*
 * @brief Command at+rst
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_sys_rst(char *buf, int len, int mode)
{
    if (AT_CMD_MODE_EXECUTION == mode)
    {
        msg_print_uart1("\r\nOK\r\n");

        // wait the output of Uart
        at_cmd_sys_internal_wait(AT_CMD_SYS_WAIT_TIME);

        // reset all
        Hal_Sys_SwResetAll();
        while(1);
    }

    return true;
}

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
int at_cmd_sys_gmr(char *buf, int len, int mode)
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

void at_cmd_sys_gslp_wakeup_callback(PS_WAKEUP_TYPE type)
{
	msg_print_uart1("\r\nAT Wakeup, Type: %s\r\n", type == PS_WAKEUP_TYPE_IO ? "IO" : "TIMEOUT");
}

void at_cmd_sys_gslp_io_callback(E_GpioIdx_t eIdx)
{
    Hal_Vic_GpioIntEn(eIdx, 0);
    ps_smart_sleep(0);
	msg_print_uart1("AT IO ISR invoked, io_num: %d\r\n", eIdx);
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
int at_cmd_sys_gslp(char *buf, int len, int mode)
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
                Hal_Pin_ConfigSet(num, PIN_TYPE_GPIO_INPUT, PIN_DRIVING_HIGH);
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
int at_cmd_sys_restore(char *buf, int len, int mode)
{
    int i; //I0060 0000628
    
    if (AT_CMD_MODE_EXECUTION == mode)
    {
        // reset the config of UART0 / UART1
        MwFim_FileWriteDefault(MW_FIM_IDX_GP01_UART_CFG, 0);
        MwFim_FileWriteDefault(MW_FIM_IDX_GP01_UART_CFG, 1);

        // reset Auto connect configuartion and Info
        MwFim_FileWriteDefault(MW_FIM_IDX_WIFI_AUTO_CONN_MODE, 0);
        MwFim_FileWriteDefault(MW_FIM_IDX_WIFI_AUTO_CONN_AP_NUM, 0);
        MwFim_FileWriteDefault(MW_FIM_IDX_WIFI_AUTO_CONN_CFG, 0);
        
		//I0060 0000628_add_beg
        for(i=0; i<MAX_NUM_OF_AUTO_CONNECT; i++) {
            MwFim_FileWriteDefault(MW_FIM_IDX_WIFI_AUTO_CONN_AP_INFO, i);
        }
		//I0060 0000628_add_end

        // rest STA information
        MwFim_FileWriteDefault(MW_FIM_IDX_GP03_STA_MAC_ADDR, 0);
        MwFim_FileWriteDefault(MW_FIM_IDX_STA_SKIP_DTIM, 0);

        // Mac address source
        MwFim_FileWriteDefault(MW_FIM_IDX_GP03_MAC_ADDR_WIFI_STA_SRC, 0);
        MwFim_FileWriteDefault(MW_FIM_IDX_GP03_MAC_ADDR_WIFI_SOFTAP_SRC, 0);
        MwFim_FileWriteDefault(MW_FIM_IDX_GP03_MAC_ADDR_BLE_SRC, 0);
        
        // RF power
        MwFim_FileWriteDefault(MW_FIM_IDX_GP01_RF_CFG, 0);

        // DHCP ARP
        MwFim_FileWriteDefault(MW_FIM_IDX_DHCP_ARP_CHK, 0);
        
        // Mac data rate
        MwFim_FileWriteDefault(MW_FIM_IDX_MAC_TX_DATA_RATE, 0);
        
        msg_print_uart1("\r\nOK\r\n");

        // wait the output of Uart
        at_cmd_sys_internal_wait(AT_CMD_SYS_WAIT_TIME);

        // reset all
        Hal_Sys_SwResetAll();
        while(1);
    }

    return true;
}

/*
 * @brief Command at+uart
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_sys_uart(char *buf, int len, int mode)
{
    return true;
}

/*
 * @brief Command at+uartcur
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_sys_uartcur(char *buf, int len, int mode)
{
    T_HalUartConfig tUartConfig;

    if (AT_CMD_MODE_READ == mode)
    {
        if (0 == Hal_Uart_ConfigGet(UART_IDX_1, &tUartConfig))
        {
            at_cmd_sys_internal_uart_config_output(&tUartConfig, 1);
        }
    }
    else if (AT_CMD_MODE_SET == mode)
    {
        if (1 == at_cmd_sys_internal_uart_config_parse(buf, len, &tUartConfig))
        {
            msg_print_uart1("\r\nOK\r\n");

            // wait the output of Uart
            at_cmd_sys_internal_wait(AT_CMD_SYS_WAIT_TIME);

            // set the new config
            Hal_Uart_ConfigSet(UART_IDX_1, &tUartConfig);
        }
    }

    return true;
}

/*
 * @brief Command at+uartdef
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_sys_uartdef(char *buf, int len, int mode)
{
    T_HalUartConfig tUartConfig;

    if (AT_CMD_MODE_READ == mode)
    {
        if (0 == Hal_Uart_ConfigGet(UART_IDX_1, &tUartConfig))
        {
            at_cmd_sys_internal_uart_config_output(&tUartConfig, 0);
        }
    }
    else if (AT_CMD_MODE_SET == mode)
    {
        if (1 == at_cmd_sys_internal_uart_config_parse(buf, len, &tUartConfig))
        {
            if (MW_FIM_OK == MwFim_FileWrite(MW_FIM_IDX_GP01_UART_CFG, 1, MW_FIM_UART_CFG_SIZE, (uint8_t*)&tUartConfig))
            {
                msg_print_uart1("\r\nOK\r\n");

                // wait the output of Uart
                at_cmd_sys_internal_wait(AT_CMD_SYS_WAIT_TIME);

                // set the new config
                Hal_Uart_ConfigSet(UART_IDX_1, &tUartConfig);
            }
        }
    }

    return true;
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
int at_cmd_sys_sleep(char *buf, int len, int mode)
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
                        Hal_Pin_ConfigSet(p1, PIN_TYPE_GPIO_INPUT, PIN_DRIVING_HIGH);
                        ps_set_wakeup_io((E_GpioIdx_t) p1, 1, INT_TYPE_LEVEL, 0, at_cmd_sys_gslp_io_callback);
                    }
                    ps_set_wakeup_cb(at_cmd_sys_gslp_wakeup_callback);
					ps_smart_sleep(1);
					msg_print_uart1("\r\nOK\r\n");
					break;

				case 2:
					if (argc == 4)
                    {
                        Hal_Pin_ConfigSet(p1, PIN_TYPE_GPIO_INPUT, PIN_DRIVING_HIGH);
                        ps_set_wakeup_io((E_GpioIdx_t) p2, 1, INT_TYPE_LEVEL, 0, at_cmd_sys_gslp_io_callback);
                    }
					ps_set_wakeup_cb(at_cmd_sys_gslp_wakeup_callback);
					ps_timer_sleep(p1);
					msg_print_uart1("\r\nOK\r\n");
					break;

				case 3:
					if (argc == 3)
                    {
                        Hal_Pin_ConfigSet(p1, PIN_TYPE_GPIO_INPUT, PIN_DRIVING_HIGH);
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
 * @brief Command at+ram
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_sys_ram(char *buf, int len, int mode)
{
    if (AT_CMD_MODE_READ == mode)
    {
        msg_print_uart1("+SYSRAM:%u\r\n", xFreeBytesRemaining);

        msg_print_uart1("\r\nOK\r\n");
    }
    return true;
}

void sys_cmd_reg(int argc, char *argv[])
{
    uint32_t addr, val, old;

    if (strcmp(argv[1], "r") == 0 && argc >=3)
    {
        addr = strtoul(argv[2], NULL, 16);
        addr &= 0xFFFFFFFC;

        if (argc > 3)
        {
            uint32_t length;

            at_output("\nStart address: 0x%08X\n", addr);
            length = strtoul(argv[3], NULL, 0);
            for (uint32_t i=0; i<length; i++)
            {
                if ((i&0x3) == 0x0)
                    at_output("\n%08X:", addr+i*4);
                at_output("    %08X", reg_read(addr+i*4));
            }
            at_output("\n");
        }
        else
        {
            val = reg_read(addr);
            at_output("0x%X: 0x%08X\n", addr, val);
        }
    }
    else if (strcmp(argv[1], "w") == 0 && argc == 4)
    {
        addr = strtoul(argv[2], NULL, 16);
        addr &= 0xFFFFFFFC;
        val = strtoul(argv[3], NULL, 16);
        old = reg_read(addr);
        reg_write(addr, val);
        val = reg_read(addr);

        at_output("REG:0x%08X: 0x%08X, (old:0x%08X)\n", addr, val, old);
    }
    else
    {
        at_output("Usage:");
        at_output("\treg r <addr> [<length>]\r\n");
        at_output("\treg w <addr> <value>\r\n");
    }
}

/*
 * @brief Command at+reg
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_sys_reg(char *buf, int len, int mode)
{
#if 1
    int iRet = 0;
    char *argv[AT_MAX_CMD_ARGS] = {0};
    int argc = 0;

    if(!at_cmd_buf_to_argc_argv(buf, &argc, argv, AT_MAX_CMD_ARGS))
    {
        goto done;
    }

    if((argc < 1) || (argc > AT_MAX_CMD_ARGS))
    {
        at_output("invalid param number\r\n");
        goto done;
    }

    sys_cmd_reg(argc, argv);

    iRet = 1;

done:
    if(!iRet)
    {
        at_output("ERROR\r\n");
    }
    else
    {
        at_output("OK\r\n");
    }

    return iRet;
#else
    /** at+reg=0x40009040 --> 2 */
    /** at+reg=0x40009040,0x3F4154 --> 3 */
    volatile uint32_t u32Value = 0;

    switch(argc)
    {
        case 2: /** Read */
            u32Value = reg_read(strtol(argv[1], NULL, 16));
            printf("%d\r\n", u32Value);
            msg_print_uart1("%d\r\n", u32Value);
            break;
        case 3: /** Write */
            reg_write(strtol(argv[1], NULL, 16), strtol(argv[2], NULL, 16));
            break;
        default:
            printf("UnKnow command");
            msg_print_uart1("UnKnow command");
            break;
    }

    printf("ok\r\n");
    msg_print_uart1("ok\r\n");
    return true;
#endif
}

/*
 * @brief Command at+start
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_sys_start(char *buf, int len, int mode)
{
    return true;
}

/*
 * @brief Command at+end
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_sys_end(char *buf, int len, int mode)
{
    return true;
}

/*
 * @brief Command at+rsv
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_sys_rsv(char *buf, int len, int mode)
{
    return true;
}

/*
 * @brief Command at+auto_recording_on
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_sys_auto_recording_on(char *buf, int len, int mode)
{
    return true;
}

/*
 * @brief Command at+auto_recording_off
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_sys_auto_recording_off(char *buf, int len, int mode)
{
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
int at_cmd_sys_fwver(char *buf, int len, int mode)
{
    return true;
}

/*
 * @brief Command at+mcupwrsave
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_sys_mcupwrsave(char *buf, int len, int mode)
{
    return true;
}

/*
 * @brief Command at+mcustandby
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_sys_mcustandby(char *buf, int len, int mode)
{
    return true;
}

/*
 * @brief Command at+ota_svr_start
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_sys_ota_svr_start(char *buf, int len, int mode)
{
    return true;
}

/*
 * @brief Command at+ota_svr_stop
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_sys_ota_svr_stop(char *buf, int len, int mode)
{
    return true;
}

/*
 * @brief Command at+dbguart
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_sys_dbg_uart_ctrl(char *buf, int len, int mode)
{
    uint8_t ubCount;
    uint8_t* pubaToken[CMD_TOKEN_SIZE];
    uint32_t ulToken;

    // pre-parser the input string
    Cmd_TokenParse(buf, &ubCount, pubaToken);
    
    if (AT_CMD_MODE_SET == mode)
    {
        // AT+DBGUART=<enable/disable>
        if (2 == ubCount)
        {
            ulToken = strtoul((const char*)pubaToken[1], NULL, 0);
            if (ulToken == 1)
            {
                Hal_DbgUart_RxIntEn(1);
                
                msg_print_uart1("\r\nOK\r\n");
            }
            else if (ulToken == 0)
            {
                Hal_DbgUart_RxIntEn(0);
                
                msg_print_uart1("\r\nOK\r\n");
            }
        }
    }

    return true;
}

/*
 * @brief Command at+download
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
extern uint32_t Boot_CheckUartBehavior(void);
int at_cmd_sys_download(char *buf, int len, int mode)
{
    if (AT_CMD_MODE_EXECUTION == mode)
    {
        msg_print_uart1("\r\nOK\r\n");

        // wait the output of Uart
        at_cmd_sys_internal_wait(AT_CMD_SYS_WAIT_TIME);
        
        osEnterCritical();
        Hal_SysPinMuxDownloadInit();
        Boot_CheckUartBehavior();
        osExitCritical();
        
        // wait the output of Uart
        at_cmd_sys_internal_wait(AT_CMD_SYS_WAIT_TIME);

        // reset all
        Hal_Sys_SwResetAll();
        while(1);
    }

    return true;
}

/*
 * @brief Command at+macaddrdef
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_sys_mac_addr_def(char *buf, int len, int mode)
{
    int argc = 0;
    char *argv[AT_MAX_CMD_ARGS] = {0};
    int ret;
    u8 ret_st = true;
    
    at_cmd_buf_to_argc_argv(buf, &argc, argv, AT_MAX_CMD_ARGS);
    
    switch(mode) {
        case AT_CMD_MODE_READ:
        {
            u8 sta_type, ble_type;
            
            ret = mac_addr_get_config_source(MAC_IFACE_WIFI_STA, (mac_source_type_t *)&sta_type);
            if (ret != 0) {
                ret_st = false;
                goto done;
            }
            
            ret = mac_addr_get_config_source(MAC_IFACE_BLE, (mac_source_type_t *)&ble_type);
            if (ret != 0) {
                ret_st = false;
                goto done;
            }
            
            msg_print_uart1("\r\n+MACADDRDEF:%d,%d,%d,%d\r\n", MAC_IFACE_WIFI_STA, sta_type,
                             MAC_IFACE_BLE - 1, ble_type);
        }
            break;
        case AT_CMD_MODE_SET:
        {
            u8 iface, type;
            if (argc != 3) {
                ret_st = false;
                goto done;
            }
            
            iface = atoi(argv[1]);
            type = atoi(argv[2]);
            
            //Skip SoftAP mode since we have not support it.
            if (iface > MAC_IFACE_BLE - 1) {
                ret_st = false;
                goto done;
            }
            
            if (type > MAC_SOURCE_FROM_FLASH) {
                ret_st = false;
                goto done;
            }

            //AT command : 0 = STA, 1 = BLE
            if (iface == 0) {
                ret = mac_addr_set_config_source(MAC_IFACE_WIFI_STA, (mac_source_type_t)type);
            }
            else if (iface == 1) {
                ret = mac_addr_set_config_source(MAC_IFACE_BLE, (mac_source_type_t)type);
            }
            
            if (ret != 0) {
                ret_st = false;
                goto done;
            }
        }
            break;
        default:
            ret_st = false;
            break;
    }

done:
    if (ret_st)
        msg_print_uart1("\r\nOK\r\n");
    else 
        msg_print_uart1("\r\nError\r\n");
    
    return ret_st;
}

int at_cmd_sys_rf_hp(char *buf, int len, int mode)
{
    int argc = 0;
    char *argv[AT_MAX_CMD_ARGS] = {0};
    int ret;
    u8 ret_st = true;
    
    at_cmd_buf_to_argc_argv(buf, &argc, argv, AT_MAX_CMD_ARGS);
    
    switch(mode) {
        case AT_CMD_MODE_READ:
        {
            u8 level = 0;
            sys_get_config_rf_power_level(&level); // I0055 0000831, change type
            msg_print_uart1("\r\n+RFHP:%d\r\n", level);
        }
            break;
        case AT_CMD_MODE_SET:
        {
            u8 level = 0;
            
            if (argc != 2) {
                ret_st = false;
                goto done;
            }
            
            level = atoi(argv[1]);
            
            ret = sys_set_config_rf_power_level(level); // I0055 0000831, change type
            if (ret != 0) {
                ret_st = false;
                goto done;
            }
        }
            break;
        default:
            ret_st = false;
            break;
    }

done:
    if (ret_st)
        msg_print_uart1("\r\nOK\r\n");
    else 
        msg_print_uart1("\r\nError\r\n");
    
    return ret_st;
}

int at_cmd_sys_read_flash(char *buf, int len, int mode)
{
    int iRet = 0;
    int argc = 0;
    char *argv[AT_MAX_CMD_ARGS] = {0};
    
    if(!at_cmd_buf_to_argc_argv(buf, &argc, argv, AT_MAX_CMD_ARGS))
    {
        goto done;
    }

    if(argc < 3)
    {
        AT_LOG("invalid param number\r\n");
        goto done;
    }
    
    switch(mode)
    {
        case AT_CMD_MODE_SET:
        {
            E_SpiIdx_t u32SpiIdx = SPI_IDX_0;
            uint32_t u32Addr = (uint32_t)strtoul(argv[1], NULL, 16);
            uint32_t u32Size = (uint32_t)strtoul(argv[2], NULL, 0);
            uint32_t u32End = 0;
            uint32_t i = 0;
            uint8_t u8aReadBuf[AT_FLASH_BUF_SIZE] = {0};
            uint32_t u32BufSize = AT_FLASH_BUF_SIZE;
            uint32_t u32Offset = 0;

            if((u32Addr < g_u32FlashReadStart) || (u32Addr > g_u32FlashReadEnd))
            {
                AT_LOG("invalid address[%#x]\r\n", u32Addr);
                goto done;
            }

            u32End = u32Addr + u32Size - 1;

            if((u32Size == 0) || (u32End >= g_u32FlashReadEnd))
            {
                AT_LOG("invalid size[%d]\r\n", u32Size);
                goto done;
            }

            while(u32Offset < u32Size)
            {
                uint32_t u32ProcSize = 0;

                if(u32Offset + u32BufSize < u32Size)
                {
                    u32ProcSize = u32BufSize;
                }
                else
                {
                    u32ProcSize = u32Size - u32Offset;
                }

                AT_LOG("\r\nread_addr[%08X] offset[%08X] size[%u]\r\n", u32Addr, u32Offset, u32ProcSize);

                if(Hal_Flash_AddrRead(u32SpiIdx, u32Addr + u32Offset, 0, u32ProcSize, u8aReadBuf))
                {
                    AT_LOG("Hal_Flash_AddrRead fail\r\n");
                    goto done;
                }

                for(i = 0; i < u32ProcSize; i++)
                {
                    if((u32Offset) || (i))
                    {
                        msg_print_uart1(",");
                    }
    
                    msg_print_uart1("%02X", u8aReadBuf[i]);
                }

                u32Offset += u32ProcSize;
            }

            msg_print_uart1("\r\n");
            break;
        }

        default:
            goto done;
    }

    iRet = 1;

done:
    if(iRet)
    {
        msg_print_uart1("OK\r\n");
    }
    else
    {
        msg_print_uart1("ERROR\r\n");
    }
    
    return iRet;
}

int at_cmd_sys_write_flash(char *buf, int len, int mode)
{
    int iRet = 0;
    int argc = 0;
    char **argv = NULL;
    uint32_t u32ArgBufSize = AT_FLASH_WRITE_ARGS_MAX * sizeof(char *);
    uint8_t *baArgBuf = NULL;

    baArgBuf = (uint8_t *)malloc(u32ArgBufSize);

    if(!baArgBuf)
    {
        AT_LOG("malloc fail\r\n");
        goto done;
    }

    argv = (char **)baArgBuf;
    
    if(!at_cmd_buf_to_argc_argv(buf, &argc, argv, AT_FLASH_WRITE_ARGS_MAX))
    {
        goto done;
    }

    if(argc < 4)
    {
        AT_LOG("invalid param number\r\n");
        goto done;
    }
    
    switch(mode)
    {
        case AT_CMD_MODE_SET:
        {
            E_SpiIdx_t u32SpiIdx = SPI_IDX_0;
            uint32_t u32Addr = (uint32_t)strtoul(argv[1], NULL, 16);
            uint32_t u32Size = (uint32_t)strtoul(argv[2], NULL, 0);
            uint32_t u32End = 0;
            uint32_t i = 0;
            uint8_t u8aWriteBuf[AT_FLASH_BUF_SIZE] = {0};
            uint8_t u8aReadBuf[AT_FLASH_BUF_SIZE] = {0};
            uint32_t u32BufSize = AT_FLASH_BUF_SIZE;
            uint32_t u32Offset = 0;
            uint8_t u8ValueArgOffset = 3; // skip at+writeflash, address and size

        #ifdef AT_FLASH_CHECK_BEFORE_WRITE
            uint8_t u8NeedErase = 0;
            uint32_t u32EraseStart = 0;
            uint32_t u32EraseEnd = 0;
        #endif

            #ifdef AT_DEBUG
            uint8_t u8Dump = 0;
            #endif

            if((u32Addr < g_u32FlashWriteStart) || (u32Addr >= g_u32FlashWriteEnd))
            {
                AT_LOG("invalid address[%#x]\r\n", u32Addr);
                goto done;
            }

            u32End = u32Addr + u32Size - 1;

            if((u32Size == 0) || (u32End >= g_u32FlashWriteEnd))
            {
                AT_LOG("invalid size[%d]\r\n", u32Size);
                goto done;
            }

        #ifdef AT_FLASH_CHECK_BEFORE_WRITE
            // read and check
            while(u32Offset < u32Size)
            {
                uint32_t u32ProcSize = 0;

                #ifdef AT_DEBUG
                u8Dump = 0;
                #endif

                if(u32Offset + u32BufSize < u32Size)
                {
                    u32ProcSize = u32BufSize;
                }
                else
                {
                    u32ProcSize = u32Size - u32Offset;
                }

                #ifdef AT_DEBUG
                if((!u32Offset) || ((u32Offset + u32ProcSize) >= u32Size))
                {
                    u8Dump = 1;
                    AT_LOG("\r\nread_addr[%08X] offset[%08X] size[%u]\r\n", u32Addr, u32Offset, u32ProcSize);
                }
                #endif

                if(Hal_Flash_AddrRead(u32SpiIdx, u32Addr + u32Offset, 0, u32ProcSize, u8aReadBuf))
                {
                    AT_LOG("Hal_Flash_AddrRead fail\r\n");
                    goto done;
                }

                #ifdef AT_DEBUG
                if(u8Dump)
                {
                    AT_LOG("check: ");
                }
                #endif

                for(i = 0; i < u32ProcSize; i++)
                {
                    #ifdef AT_DEBUG
                    if(u8Dump)
                    {
                        AT_LOG(", %02X", u8aReadBuf[i]);
                    }
                    #endif

                    if(u8aReadBuf[i] != 0xFF)
                    {
                        if(u8NeedErase)
                        {
                            u32EraseEnd = u32Addr + u32Offset + i;
                        }
                        else
                        {
                            u32EraseStart = u32Addr + u32Offset + i;
                            u32EraseEnd = u32EraseStart;
                            u8NeedErase = 1;
                        }
                    }
                }

                u32Offset += u32ProcSize;
            }

            #ifdef AT_DEBUG
            if(u8Dump)
            {
                AT_LOG("\r\n");
            }
            #endif

            // erase
            if(u8NeedErase)
            {
                uint32_t u32EraseUnit = 0x1000; // 4K

                u32EraseStart &= ~(u32EraseUnit - 1);
                u32EraseEnd &= ~(u32EraseUnit - 1);

                while(u32EraseStart <= u32EraseEnd)
                {
                    AT_LOG("\r\nerase_addr[%08X]\r\n", u32EraseStart);

                    if(Hal_Flash_4KSectorAddrErase(u32SpiIdx, u32EraseStart))
                    {
                        AT_LOG("Hal_Flash_4KSectorAddrErase fail\r\n");
                        goto done;
                    }

                    u32EraseStart += u32EraseUnit;
                }
            }

            u32Offset = 0;
        #endif //#ifdef AT_FLASH_CHECK_BEFORE_WRITE

            // write
            while(u32Offset < u32Size)
            {
                uint32_t u32ProcSize = 0;

                #ifdef AT_DEBUG
                u8Dump = 0;
                #endif

                if(u32Offset + u32BufSize < u32Size)
                {
                    u32ProcSize = u32BufSize;
                }
                else
                {
                    u32ProcSize = u32Size - u32Offset;
                }

                for(i = 0; i < u32ProcSize; i++)
                {
                    uint32_t u32ArgIdx = u8ValueArgOffset + u32Offset + i;

                    if(u32ArgIdx >= argc)
                    {
                        u32ArgIdx = argc - 1;
                    }

                    u8aWriteBuf[i] = (uint8_t)strtoul(argv[u32ArgIdx], NULL, 16);
                }

                #ifdef AT_DEBUG
                if((!u32Offset) || ((u32Offset + u32ProcSize) >= u32Size))
                {
                    u8Dump = 1;
                    AT_LOG("\r\nwrite_addr[%08X] offset[%08X] size[%u]\r\n", u32Addr, u32Offset, u32ProcSize);
                }
                #endif

                if(Hal_Flash_AddrProgram(u32SpiIdx, u32Addr + u32Offset, 0, u32ProcSize, u8aWriteBuf))
                {
                    AT_LOG("Hal_Flash_AddrProgram fail\r\n");
                    goto done;
                }

                if(Hal_Flash_AddrRead(u32SpiIdx, u32Addr + u32Offset, 0, u32ProcSize, u8aReadBuf))
                {
                    AT_LOG("Hal_Flash_AddrRead fail\r\n");
                    goto done;
                }

                if(memcmp(u8aReadBuf, u8aWriteBuf, u32ProcSize))
                {
                    AT_LOG("data not matched\r\n");
                    goto done;
                }

                u32Offset += u32ProcSize;
            }

            #ifdef AT_DEBUG
            if(u8Dump)
            {
                AT_LOG("\r\n");
            }
            #endif
            
            break;
        }

        default:
            goto done;
    }

    iRet = 1;

done:
    if(baArgBuf)
    {
        free(baArgBuf);
    }

    if(iRet)
    {
        msg_print_uart1("OK\r\n");
    }
    else
    {
        msg_print_uart1("ERROR\r\n");
    }
    
    return iRet;
}

int at_cmd_sys_erase_flash(char *buf, int len, int mode)
{
    int iRet = 0;
    int argc = 0;
    char *argv[AT_MAX_CMD_ARGS] = {0};
    
    if(!at_cmd_buf_to_argc_argv(buf, &argc, argv, AT_MAX_CMD_ARGS))
    {
        goto done;
    }

    if(argc < 3)
    {
        AT_LOG("invalid param number\r\n");
        goto done;
    }
    
    switch(mode)
    {
        case AT_CMD_MODE_SET:
        {
            E_SpiIdx_t u32SpiIdx = SPI_IDX_0;
            uint32_t u32Addr = (uint32_t)strtoul(argv[1], NULL, 16);
            uint32_t u32SectorNum = (uint32_t)strtoul(argv[2], NULL, 0);
            uint32_t u32EraseUnit = 0x1000; // 4K
            uint32_t u32EraseStart = 0;
            uint32_t u32EraseEnd = 0;

            if((u32Addr < g_u32FlashWriteStart) || (u32Addr >= g_u32FlashWriteEnd))
            {
                AT_LOG("invalid address[%#x]\r\n", u32Addr);
                goto done;
            }

            if(u32Addr & (u32EraseUnit - 1))
            {
                AT_LOG("invalid sector start address[%#x]\r\n", u32Addr);
                goto done;
            }

            u32EraseStart = u32Addr;
            u32EraseEnd = u32EraseStart + u32EraseUnit * u32SectorNum - 1;

            if((u32SectorNum == 0) || (u32EraseEnd >= g_u32FlashWriteEnd))
            {
                AT_LOG("invalid sector number[%d]\r\n", u32SectorNum);
                goto done;
            }

            while(u32EraseStart <= u32EraseEnd)
            {
                AT_LOG("\r\nerase_addr[%08X]\r\n", u32EraseStart);

                if(Hal_Flash_4KSectorAddrErase(u32SpiIdx, u32EraseStart))
                {
                    AT_LOG("Hal_Flash_4KSectorAddrErase fail\r\n");
                    goto done;
                }

                u32EraseStart += u32EraseUnit;
            }

            break;
        }

        default:
            goto done;
    }

    iRet = 1;

done:
    if(iRet)
    {
        msg_print_uart1("OK\r\n");
    }
    else
    {
        msg_print_uart1("ERROR\r\n");
    }
    
    return iRet;
}


int at_cmd_at_switch_to_dbg(char *buf, int len, int mode)
{
    msg_print_uart1("\r\n");
    tracer_drct_printf("\r\n");
    
    at_cmd_switch_uart1_dbguart();
    
    /* Make uart host buffer clean */
    Hal_Uart_DataSend(UART_IDX_1, 0);
    Hal_DbgUart_DataSend(0);
    
    msg_print_uart1("\r\nSwitch: AT UART\r\n>");
    tracer_drct_printf("\r\nSwitch: Dbg UART\r\n>");
        
    return true;
}

int at_cmd_sys_mp_rst(char *buf, int len, int mode)
{
    return at_cmd_sys_rst(buf, len, mode);
}

/*
 * @brief  Sample code to do system test
 *
 * @return 0 fail 1 success
 *
 */
int _at_cmd_sys_sample(void)
{
    return true;
}

int at_cmd_sys_mode(char *buf, int len, int mode)
{
    T_MwFim_SysMode tSysMode;

    uint8_t ubCount;
    uint8_t* pubaToken[CMD_TOKEN_SIZE];

    if (AT_CMD_MODE_READ == mode)
    {
        msg_print_uart1("\r\n");

        // get the settings of system mode
        if (MW_FIM_OK != MwFim_FileRead(MW_FIM_IDX_GP03_SYS_MODE, 0, MW_FIM_SYS_MODE_SIZE, (uint8_t*)&tSysMode))
        {
            // if fail, get the default value
            memcpy(&tSysMode, &g_tMwFimDefaultSysMode, MW_FIM_SYS_MODE_SIZE);
        }

        msg_print_uart1("System Mode: %d \r\n", tSysMode.ubSysMode);

        msg_print_uart1("\r\nOK\r\n");
    }
    else if (AT_CMD_MODE_SET == mode)
    {
        msg_print_uart1("\r\n");

        // pre-parser the input string
        Cmd_TokenParse(buf, &ubCount, pubaToken);

        // set the settings of system mode
        tSysMode.ubSysMode = strtoul((const char*)pubaToken[1], NULL, 0);
        if (tSysMode.ubSysMode < MW_FIM_SYS_MODE_MAX)
        {
            if (MW_FIM_OK == MwFim_FileWrite(MW_FIM_IDX_GP03_SYS_MODE, 0, MW_FIM_SYS_MODE_SIZE, (uint8_t*)&tSysMode))
            {
                msg_print_uart1("\r\nOK\r\n");
            }
        }
    }

    return true;
}

int at_cmd_at_slp_tmr(char *buf, int len, int mode)
{
    uint32_t u32TickStart = 0;
    uint32_t u32TickDiff = 0;
    uint64_t u64SlpTmrStart = 0;
    uint64_t u64SlpTmrEnd = 0;

    Hal_Tick_Init();
    
    u64SlpTmrStart = Hal_Sys_SleepTimerGet();
    u32TickStart = Hal_Tick_Diff( 0 );
    while( u32TickDiff < ( 1000* Hal_Tick_PerMilliSec() ) )
    {
        // busy wait here
        u32TickDiff = Hal_Tick_Diff( u32TickStart );
    }
    u64SlpTmrEnd = Hal_Sys_SleepTimerGet();
    
    msg_print_uart1("\r\n32K XTAL Freq: %lld\n\r", u64SlpTmrEnd - u64SlpTmrStart);
    tracer_drct_printf("\r\n32K XTAL Freq: %lld\n\r", u64SlpTmrEnd - u64SlpTmrStart);
    return true;
}

/**
  * @brief AT Command Table for System Module
  *
  */
at_command_t gAtCmdTbl_Sys[] =
{
    { "at+rst",                 at_cmd_sys_rst,           "Restart module" },
    { "at+gmr",                 at_cmd_sys_gmr,           "View version info" },
    { "at+gslp",                at_cmd_sys_gslp,          "Enter deep-sleep mode" },
    { "at+restore",             at_cmd_sys_restore,       "Factory Reset" },
    { "at+uart_cur",            at_cmd_sys_uartcur,       "UART current configuration show" },
    { "at+uart_def",            at_cmd_sys_uartdef,       "UART default configuration, save to flash" },
    { "at+uart",                at_cmd_sys_uart,          "UART configuration" },
    { "at+sleep",               at_cmd_sys_sleep,         "Sleep mode" },
    { "at+sysram",              at_cmd_sys_ram,           "RAM information" },
    { "at+reg",                 at_cmd_sys_reg,           "Register Access" },
    { "at+sysinit",             at_cmd_sys_start,         "Sys Start" },    //Back Door
    { "at+sysend",              at_cmd_sys_end,           "Sys End" },      //Back Door
    { "at+sysrsv",              at_cmd_sys_rsv,           "Sys Reserved" }, //Back Door
    { "at+auto_recording_on",   at_cmd_sys_auto_recording_on,  "Save Wi-Fi/BLE status" },
    { "at+auto_recording_off",  at_cmd_sys_auto_recording_off, "Turn off auto_recording" },
    { "at+fwver",               at_cmd_sys_fwver,         "Show FW Version" },
    { "at+mcupwrsave",          at_cmd_sys_mcupwrsave,    "Enter Power save mode" },
    { "at+mcustandby",          at_cmd_sys_mcustandby,    "Enter Standby mode" },
    { "at+ota_svr_start",       at_cmd_sys_ota_svr_start, "Open OTA FW Upgrade(HTTP) function" },
    { "at+ota_svr_stop",        at_cmd_sys_ota_svr_stop,  "Close OTA FW Upgrade(HTTP) function" },
    { "at+dbguart",             at_cmd_sys_dbg_uart_ctrl, "Enable/Disable the Rx part of debug UART" },
    { "at+download",            at_cmd_sys_download,      "Download the patch image" },
    { "at+macaddrdef",          at_cmd_sys_mac_addr_def,  "Default mac address from OTP or others storage" },
    { "at+rfhp",                at_cmd_sys_rf_hp,         "Set RF power"},
    { "at+readflash",           at_cmd_sys_read_flash,    "Read flash" },
    { "at+writeflash",          at_cmd_sys_write_flash,   "Write flash" },
    { "at+eraseflash",          at_cmd_sys_erase_flash,   "Erase flash" },
    { "at+switchdbg",           at_cmd_at_switch_to_dbg,  "AT switch to Debug UART" },
    { "at+mprst",               at_cmd_sys_mp_rst,        "Restart module (MP usage)" },
    { "at+sysmode",             at_cmd_sys_mode,          "Set the system mode"},
    { "at+slptmr",              at_cmd_at_slp_tmr,        "Got measured 32K XTAL freq"},
    { NULL,                     NULL,                     NULL},
};

/*
 * @brief Global variable g_AtCmdTbl_Sys_Ptr retention attribute segment
 *
 */
RET_DATA at_command_t *g_AtCmdTbl_Sys_Ptr;

/*
 * @brief AT Command Interface Initialization for System modules
 *
 */
void at_cmd_sys_func_init(void)
{
    /** Command Table (System) */
    g_AtCmdTbl_Sys_Ptr      = gAtCmdTbl_Sys;

    g_u32FlashReadStart     = AT_FLASH_READ_START;
    g_u32FlashReadEnd       = AT_FLASH_READ_END;
    g_u32FlashWriteStart    = AT_FLASH_WRITE_START;
    g_u32FlashWriteEnd      = AT_FLASH_WRITE_END;
}

