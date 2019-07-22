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
 * @file at_cmd_pip.c
 * @author Michael Liao
 * @date 14 Dec 2017
 * @brief File supports the Peripherial module AT Commands.
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "os.h"
#include "at_cmd.h"
#include "at_cmd_pip.h"
#include "at_cmd_common.h"

/*
 * @brief Command at+pipfun1
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_pip_fun1(char *buf, int len, int mode)
{
    return true;
}

/*
 * @brief Command at+piprsv
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_pip_rsv(char *buf, int len, int mode)
{
    return true;
}

/*
 * @brief Command at+spi_master_init
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_pip_spi_master_init(char *buf, int len, int mode)
{
    return true;
}

/*
 * @brief Command at+spi_master_stop
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_pip_spi_master_stop(char *buf, int len, int mode)
{
    return true;
}

/*
 * @brief Command at+spi_master_get_data
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_pip_spi_master_get_data(char *buf, int len, int mode)
{
    return true;
}

/*
 * @brief Command at+spi_master_buf_show
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_pip_spi_master_buf_show(char *buf, int len, int mode)
{
    return true;
}

/*
 * @brief Command at+spi_slave_init
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_pip_spi_slave_init(char *buf, int len, int mode)
{
    return true;
}

/*
 * @brief Command at+spi_slave_stop
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_pip_spi_slave_stop(char *buf, int len, int mode)
{
    return true;
}

/*
 * @brief Command at+spi_slave_buf_status
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_pip_spi_slave_buf_status(char *buf, int len, int mode)
{
    return true;
}

/*
 * @brief Command at+spi_slave_buf_show
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_pip_spi_slave_buf_show(char *buf, int len, int mode)
{
    return true;
}

/*
 * @brief Command at+at_cmd_pip_uart_cts_rts_enable
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_pip_uart_cts_rts_enable(char *buf, int len, int mode)
{
    return true;
}

/*
 * @brief Command at+uart_cts_rts_disable
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_pip_uart_cts_rts_disable(char *buf, int len, int mode)
{
    return true;
}

/*
 * @brief Command at+at_cmd_pip_uart_start
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_pip_uart_start(char *buf, int len, int mode)
{
    return true;
}

/*
 * @brief Command at+at_cmd_pip_uart_stop
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_pip_uart_stop(char *buf, int len, int mode)
{
    return true;
}

/*
 * @brief Command at+at_cmd_pip_uart_write
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_pip_uart_write(char *buf, int len, int mode)
{
    return true;
}

/**
  * @brief AT Command Table for Peripherial IP Module
  *
  */
at_command_t gAtCmdTbl_Pip[] =
{
    { "at+pipfun1",              at_cmd_pip_fun1,                   "Perpherial IP function 1" },
    { "at+piprsv",               at_cmd_pip_rsv,                    "Perpherial IP Reserved" },
    { "at+spi_master_init",      at_cmd_pip_spi_master_init,        "Open SPI Master and initialize buffer" },
    { "at+spi_master_stop",      at_cmd_pip_spi_master_stop,        "Stop SPI Master" },
    { "at+spi_master_get_data",  at_cmd_pip_spi_master_get_data,    "Get data from SPI Slave" },
    { "at+spi_master_buf_show",  at_cmd_pip_spi_master_buf_show,    "Show SPI Master Buffer" },
    { "at+spi_slave_init",       at_cmd_pip_spi_slave_init,         "Open SPI Slave" },
    { "at+spi_slave_stop",       at_cmd_pip_spi_slave_stop,         "Stop SPI Slave" },
    { "at+spi_slave_buf_status", at_cmd_pip_spi_slave_buf_status,   "Show SPI Slave Buffer Status" },
    { "at+spi_slave_buf_show",   at_cmd_pip_spi_slave_buf_show,     "Show SPI Slave Buffer" },
    { "at+uart_cts_rts_enable",  at_cmd_pip_uart_cts_rts_enable,    "Enable UART CTS/RTS" },
    { "at+uart_cts_rts_disable", at_cmd_pip_uart_cts_rts_disable,   "Disable UART CTS/RTS" },
    { "at+uart_start",           at_cmd_pip_uart_start,             "Start UART interface" },
    { "at+uart_stop",            at_cmd_pip_uart_stop,              "Stop UART interface" },
    { "at+uart_write",           at_cmd_pip_uart_write,             "Send data to UART interface" },
    { NULL,                      NULL,                               NULL},
};

/*
 * @brief Global variable g_AtCmdTbl_Pip_Ptr retention attribute segment
 *
 */
RET_DATA at_command_t *g_AtCmdTbl_Pip_Ptr;

/*
 * @brief AT Command Interface Initialization for Peripherial IP modules
 *
 */
void at_cmd_pip_func_init(void)
{
    /** Command Table (PIP) */
    g_AtCmdTbl_Pip_Ptr = gAtCmdTbl_Pip;
}

