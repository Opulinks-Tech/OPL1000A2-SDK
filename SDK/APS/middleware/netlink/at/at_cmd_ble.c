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
 * @file at_cmd_ble.c
 * @author Michael Liao
 * @date 14 Dec 2017
 * @brief File supports the BLE module AT Commands.
 *
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "os.h"
#include "at_cmd.h"
#include "at_cmd_ble.h"
#include "le_ctrl.h"
#include "at_cmd_common.h"
#include "sys_common_ctrl.h"

static uint8_t at_cmd_UtilStringToHexNum(uint8_t *data, uint16_t len, uint8_t *buf);
static uint8_t at_cmd_UtilGetAddrFromStr(uint8_t *addr_str, uint8_t *addr);

// Refer to static BOOL LeHtcUtilStringToHexNum_Impl(UINT8 *data, UINT16 len, UINT8 *buf)
static uint8_t at_cmd_UtilStringToHexNum(uint8_t *data, uint16_t len, uint8_t *buf)
{
	uint16_t i = 0;
	uint8_t c;
	uint8_t lc;
	uint8_t rc;
    
	if (!data || !buf || !len || (len & 1)) return false;

	while (len)
	{
		c = data[i++];
    
		if ((c >= '0') && (c <= '9'))
			c -= '0';
        else if ((c >= 'a') && (c <= 'f'))
			c = c - 'a' + 10;
        else if ((c >= 'A') && (c <= 'F'))
			c = c - 'A' + 10;
		else
			return false;

		if (i & 1) 
		{
			lc = c;
	        continue;
        }
        else
            rc = c;

		buf[(i >> 1) - 1] = (lc << 4) | rc;

        len -= 2;
    }
	
	return true;
}

// Refer to static BOOL LeCmdAppGetAddrFromStr_Impl(UINT8 *addr_str, BD_ADDR addr)
static uint8_t at_cmd_UtilGetAddrFromStr(uint8_t *addr_str, uint8_t *addr)
{
	uint16_t len = strlen((const char *)addr_str) + 1;
	uint8_t *s = 0;
	uint8_t *e = 0;
	uint8_t *p = addr_str;
    uint8_t addrBuf[2];
	uint8_t c;
	uint8_t count = 0;

	while (len--)
	{
		c = *p;
    
		if (((c >= '0') && (c <= '9')) || 
            ((c >= 'a') && (c <= 'f')) || 
            ((c >= 'A') && (c <= 'F')))
		{
			if (!s)
				s = p;
			else
				e = p;
        }
		else
		{
			if (s)
			{
				uint8_t *a = &addr[count];
                
				if (!e)
				{
					addrBuf[0] = '0';
					addrBuf[1] = *s;
                }
				else if ((e - s) < 2)
				{
					addrBuf[0] = *s;
					addrBuf[1] = *e;
                }
            	else
                    return false;

				if (!at_cmd_UtilStringToHexNum(addrBuf, 2, a)) return false;
				
				count++;

				if (count >= 6) break;
            }
        
			s = e = 0;
        }

        p++;
    }

	if (count == 6)
	{
		return true;
    }

	return false;
}

/*
 * @brief Command at+blemode
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_ble_mode(char *buf, int len, int mode)
{
    char *argv[AT_MAX_CMD_ARGS] = {0};
    int argc = 0;
    int uart_mode = UART1_MODE_DEFAULT;
    extern unsigned int g_uart1_mode;

    at_cmd_buf_to_argc_argv(buf, &argc, argv, AT_MAX_CMD_ARGS);
    
    if(argc > 1) {
        uart_mode = atoi(argv[1]);
        if ((uart_mode >= UART1_MODE_NUM) || (uart_mode < UART1_MODE_AT)) return false;
    }
    
    g_uart1_mode = uart_mode;
    
    switch (uart_mode)
    {
        case UART1_MODE_AT:
            uart1_mode_set_at();
            break;
        case UART1_MODE_BLE_HCI:
            uart1_mode_set_ble_hci();
            break;
/*
        case UART1_MODE_BLE_HOST:
            break;
*/
        case UART1_MODE_OTHERS:
            uart1_mode_set_others();
            break;
        default:
            break;
    }
    
    return true;
}

/*
 * @brief Command at+letest
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_letest(char *buf, int len, int mode)
{
    /* at+letest=251, 1  => 251 octets, 1 sec */

    int iRet = 0;
    int argc = 0;
    char *argv[AT_MAX_CMD_ARGS] = {0};
    uint8_t tx_octets = 0;
    uint8_t duration = 0;

    if(!at_cmd_buf_to_argc_argv(buf, &argc, argv, AT_MAX_CMD_ARGS))
    {
        goto done;
    }
    if(argc != 3)
    {
        at_output("invalid param number\r\n");
        goto done;
    }

    tx_octets = atoi(argv[1]);
    duration = atoi(argv[2]);

    le_ctrl_throughput_test(tx_octets, duration);
    iRet = 1;

done:
    if(!iRet)
    {
        at_output("ERROR\r\n");
    }

    return true;
}

/*
 * @brief Command at+mpbleaddr
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_mp_ble_addr(char *buf, int len, int mode)
{
    int iRet = 0;
    int argc = 0;
    char *argv[AT_MAX_CMD_ARGS] = {0};
    uint8_t i = 0;
    uint8_t type = 0;
    uint8_t bd_addr[6] = {0};

    if (!at_cmd_buf_to_argc_argv(buf, &argc, argv, AT_MAX_CMD_ARGS))
    {
        goto done;
    }
    
    if (!strncmp("?", &(argv[0][12]), 13))
    {
        get_ble_bd_addr(bd_addr);
        at_output("+MPBLEADDR:\"");
        for (i = 0; i < 6; i++)
        {
            if (i == 5)
                at_output("%2X\"\r\n", bd_addr[i]);
            else
                at_output("%2X:", bd_addr[i]);
        }
    }
    else 
    {
        type = atoi(argv[1]);
        
        if (type == 0)
        {
            if (!at_cmd_UtilGetAddrFromStr((uint8_t *)argv[2], bd_addr)) goto done;
        
            set_ble_bd_addr(bd_addr);
        }
        else
        {
            goto done;
        }
    }
    
    iRet = 1;

done:
    if (!iRet)
    {
        at_output("+MPBLEADDR: ERROR\r\n");
    }

    return true;
}
