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
 * @file at_cmd_common.c
 * @author Michael Liao
 * @date 14 Dec 2017
 * @brief File contains the AT Command common api.
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdarg.h>
#include "opulinks.h"
#include "os.h"
#include "at_cmd.h"
#include "at_cmd_sys.h"
#include "at_cmd_wifi.h"
#include "at_cmd_ble.h"
#include "at_cmd_tcpip.h"
#include "at_cmd_rf.h"
#include "at_cmd_pip.h"
#include "at_cmd_others.h"
#include "at_cmd_task.h"
#include "le_ctrl.h"
#include "hal_pin.h"
#include "hal_pin_def.h"
#include "hal_pin_config.h"
#include "at_cmd_common.h"
#include "hal_dbg_uart.h"
#include "hal_uart.h"
#if defined(__AT_CMD_TASK__)
#include "at_cmd_task.h"
#endif
#include "at_cmd_data_process.h"
#include "wpa_supplicant_i.h"
#include "le_cmd_app_cmd.h"
#include "controller_wifi_com.h"


/*
 * @brief Global variable msg_print_uart1 retention attribute segment
 *
 */
RET_DATA msg_print_uart1_fp_t msg_print_uart1;

/*
 * @brief Global variable uart1_mode_get retention attribute segment
 *
 */
RET_DATA uart1_mode_get_fp_t uart1_mode_get;

/*
 * @brief Global variable uart1_mode_set retention attribute segment
 *
 */
RET_DATA uart1_mode_set_fp_t uart1_mode_set;

/*
 * @brief Global variable uart1_mode_set_at retention attribute segment
 *
 */
RET_DATA uart1_mode_set_at_fp_t uart1_mode_set_at;

/*
 * @brief Global variable uart1_mode_set_ble_hci retention attribute segment
 *
 */
RET_DATA uart1_mode_set_ble_hci_fp_t uart1_mode_set_ble_hci;

/*
 * @brief Global variable uart1_mode_set_ble_host retention attribute segment
 *
 */
RET_DATA uart1_mode_set_ble_host_fp_t uart1_mode_set_ble_host;

/*
 * @brief Global variable uart1_mode_set_others retention attribute segment
 *
 */
RET_DATA uart1_mode_set_others_fp_t uart1_mode_set_others;

/*
 * @brief Global variable uart1_mode_set_default retention attribute segment
 *
 */
RET_DATA uart1_mode_set_default_fp_t uart1_mode_set_default;

/*
 * @brief Global variable uart1_rx_int_do_at retention attribute segment
 *
 */
RET_DATA uart1_rx_int_do_at_fp_t uart1_rx_int_do_at;

/*
 * @brief Global variable uart1_rx_int_do_ble_hci retention attribute segment
 *
 */
RET_DATA uart1_rx_int_do_ble_hci_fp_t uart1_rx_int_do_ble_hci;

/*
 * @brief Global variable uart1_rx_int_do_ble_host retention attribute segment
 *
 */
RET_DATA uart1_rx_int_do_ble_host_fp_t uart1_rx_int_do_ble_host;

/*
 * @brief Global variable uart1_rx_int_handler_default retention attribute segment
 *
 */
RET_DATA uart1_rx_int_handler_default_fp_t uart1_rx_int_handler_default;

/*
 * @brief Global variable uart1_rx_int_do_others retention attribute segment
 *
 */
RET_DATA uart1_rx_int_do_others_fp_t uart1_rx_int_do_others;

/*
 * @brief Global variable at_clear_uart_buffer retention attribute segment
 *
 */
RET_DATA at_clear_uart_buffer_fp_t at_clear_uart_buffer;

/*
 * @brief Global variable g_uart1_mode retention attribute segment
 *
 */
RET_DATA unsigned int g_uart1_mode;

/*
 * @brief Global variable at_rx_buf retention attribute segment
 *
 */
RET_DATA at_uart_buffer_t at_rx_buf;

/*
 * @brief Global variable uart1_rx_int_at_data_receive_ble retention attribute segment
 *
 */
RET_DATA uart1_rx_int_at_data_receive_ble_fp_t uart1_rx_int_at_data_receive_ble;

/*
 * @brief Global variable uart1_rx_int_at_data_receive_tcpip retention attribute segment
 *
 */
RET_DATA uart1_rx_int_at_data_receive_tcpip_fp_t uart1_rx_int_at_data_receive_tcpip;


/*
 * @brief Global variable at_cmd_switch_uart1_dbguart retention attribute segment
 *
 */
RET_DATA at_cmd_switch_uart1_dbguart_fp_t at_cmd_switch_uart1_dbguart;



/*
 * @brief Global variable set_echo_on retention attribute segment
 *
 */
RET_DATA set_echo_on_fp_t set_echo_on;

/*
 * @brief Global variable at_echo_on retention attribute segment
 *
 */
RET_DATA int at_echo_on;

/*
 * @brief Global variable at_cmd_get_para_as_digital retention attribute segment
 *
 */
RET_DATA at_cmd_get_para_as_digital_fp_t at_cmd_get_para_as_digital;


RET_DATA at_io01_uart_mode_set_fp at_io01_uart_mode_set;


/* IO 0/1 Uart mode */
E_IO01_UART_MODE g_eIO01UartMode;

extern struct wpa_supplicant *wpa_s;

uint8_t at_state = AT_STATE_IDLE;
uint8_t *pDataLine;
uint8_t  at_data_line[AT_DATA_LEN_MAX];
extern uint16_t at_send_len;
extern uint8_t sending_id;

//at global variables
static char *at_result_string[AT_RESULT_CODE_MAX] = {
    "\r\nOK\r\n",         //AT_RESULT_CODE_OK         = 0x00,
    "\r\nERROR\r\n",      //AT_RESULT_CODE_ERROR      = 0x01,
    "\r\nERROR\r\n",      //AT_RESULT_CODE_FAIL       = 0x02,
    "\r\nSEND OK\r\n",    //AT_RESULT_CODE_SEND_OK    = 0x03,
    "\r\nSEND FAIL\r\n",  //AT_RESULT_CODE_SEND_FAIL  = 0x04,
    NULL,
    NULL,
};

/*
 * @brief Print Message to UART1
 *
 * @param [in] fmt message
 */
void msg_print_uart1_impl(char *fmt,...)
{
	va_list ap;
    char string[256];
    char *pt;

	{
		va_start(ap,fmt);
    	vsprintf(string,fmt,ap);

	    pt = &string[0];
        while(*pt != '\0')
        {
            if(*pt == '\n') {
                Hal_Uart_DataSend(UART_IDX_1, '\r');
            }
            Hal_Uart_DataSend(UART_IDX_1, *pt++);
        }

    	va_end(ap);
	}
}

/*
 * @brief Get UART1 mode
 *
 * @return int Current UART1 mode
 */
int uart1_mode_get_impl(void)
{
    return g_uart1_mode;
}

/*
 * @brief UART1 Mode Setting's Handler
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int uart1_mode_set_impl(int argc, char *argv[])
{
    int mode = UART1_MODE_DEFAULT;

    if(argc > 1) {
        mode = atoi(argv[1]);
        if ((mode >= UART1_MODE_NUM) || (mode < UART1_MODE_AT)) return false;
    }
    g_uart1_mode = mode;

    switch (mode) {
    	case UART1_MODE_AT:
            /** Do something when set AT mode */
            uart1_mode_set_at();
    		break;
        case UART1_MODE_BLE_HCI:
            /** Do something when set BLE HCI mode */
            uart1_mode_set_default();
            uart1_mode_set_ble_hci();
    		break;
        case UART1_MODE_OTHERS:
            /** Do something when set Others mode */
            uart1_mode_set_others();
            break;
    	default:
    		break;
	}

    return true;
}

/*
 * @brief UART1 AT Mode's Configuration
 *
 */
void uart1_mode_set_at_impl(void)
{
    /** Nothing to do */
}

/*
 * @brief UART1 BLE HCI Mode's Configuration
 *
 */
void uart1_mode_set_ble_hci_impl(void)
{
    le_ctrl_data.is_uart_hci_mode = (g_uart1_mode == UART1_MODE_BLE_HCI);
}

/*
 * @brief UART1 BLE HOST Mode's Configuration ()
 *
 */
void uart1_mode_set_ble_host_impl(void)
{
    /** Reserved */
}

/*
 * @brief UART1 BLE HCI Mode's Configuration
 *
 */
void uart1_mode_set_others_impl(void)
{
    /** Reserved */
}

/*
 * @brief Configure the UART1 to default UART AT Mode
 *
 */
void uart1_mode_set_default_impl(void)
{
    Hal_Uart_RxCallBackFuncSet(UART_IDX_1, uart1_rx_int_handler_default);
    Hal_Uart_RxIntEn(UART_IDX_1, 1);
}

#if 0
/*
 * @brief AT mode handler
 *
 * @param [in] u32Data 4bytes data from UART1 interrupt
 */
void uart1_rx_int_do_at_impl(uint32_t u32Data)
{
    at_uart_buffer_t *p;
    xATMessage txMsg;

    p = &at_rx_buf;
    if ((p->in & ~(AT_RBUF_SIZE-1)) == 0)
    {
        if((u32Data & 0xFF) == 0x0D)
        {
            p->buf [p->in & (AT_RBUF_SIZE-1)] = 0x00;

            /** send message */
            txMsg.pcMessage = (char *)p;
            txMsg.length = sizeof(at_uart_buffer_t);
            txMsg.event = AT_UART1_EVENT;

            /** Post the byte. */
            at_task_send( txMsg );
        }
        else if((u32Data & 0xFF) == 0x08)
        {
            if (p->in > 0)
            {
                p->in--;
                p->buf[p->in & (AT_RBUF_SIZE-1)] = 0x00;
                Hal_Uart_DataSend(UART_IDX_1, 0x08);
                Hal_Uart_DataSend(UART_IDX_1, 0x20);
                Hal_Uart_DataSend(UART_IDX_1, 0x08);
            }
        }
        else
        {
            p->buf [p->in & (AT_RBUF_SIZE-1)] = (u32Data & 0xFF);
            Hal_Uart_DataSend(UART_IDX_1, p->buf [p->in]);
            p->in++;
        }
    }
    else
    {
        p->in = 0;
        p->buf [p->in & (AT_RBUF_SIZE-1)] = (u32Data & 0xFF);
        Hal_Uart_DataSend(UART_IDX_1, p->buf [p->in]);
    }

}
#endif

/*
 * @brief UART1 RX Data Handler of BLE (Reserved)
 *
 * @param [in] u32Data 4bytes data from UART1 interrupt
 */
void uart1_rx_int_at_data_receive_ble_impl(uint32_t u32Data)
{
	LeCmdAppProcessSendData((char *)&u32Data, 1);
}

/*
 * @brief UART1 RX Data Handler of TCP/IP (Reserved)
 *
 * @param [in] u32Data 4bytes data from UART1 interrupt
 */
void uart1_rx_int_at_data_receive_tcpip_impl(uint32_t u32Data)
{
    int send_len = 0;
    bool sendex_flag = FALSE;
    at_socket_t *link = at_link_get_id(sending_id);

    send_len = data_process_data_len_get();

    *pDataLine = (u32Data & 0xFF);

    if (at_state == AT_STATE_SENDING_RECV)
    {
        //if not transparent transmission mode, display back
        //if(( (u32Data & 0xFF) != '\n') && (at_echo_on))
        {
            Hal_Uart_DataSend(UART_IDX_1, (u32Data & 0xFF));
        }

        if (link->send_mode == AT_SEND_MODE_SENDEX)
        {
            uint32_t index = 0;
            for (index = 0; index < pDataLine - at_data_line; index++)
            {
                if ((at_data_line[index] == '\\') && (at_data_line[index + 1] == '0'))
                {
                    at_data_line[index] = '\0';
                    sendex_flag = TRUE;
                    send_len = index ;
                    break;
                }
            }
        }

        if ((pDataLine >= &at_data_line[send_len - 1]) || (pDataLine >= &at_data_line[AT_DATA_LEN_MAX - 1]) || (sendex_flag == TRUE))
        {
            at_event_msg_t msg = {0};

            msg.event = AT_DATA_TX_EVENT;
            msg.length = send_len;
            msg.param = at_data_line;
            at_data_task_send(&msg);
            at_state = AT_STATE_SENDING_DATA;
            pDataLine = at_data_line;
            sendex_flag = FALSE;
        }
        else
        {
            pDataLine++;
        }
    }
    /* Enter transparent transmission, with a 20-ms
       interval between each packet, and a maximum of
       2048 bytes per packet. */
    else if (at_state == AT_STATE_TRANSMIT)
    {
        static uint32_t data_len;
        pDataLine++;
        
        data_len = pDataLine - at_data_line;
        if (data_len >= AT_DATA_LEN_MAX)
        {
            pDataLine = at_data_line; //Point to index 0
            
            at_event_msg_t msg = {0};
            msg.event = AT_DATA_TX_EVENT;
            msg.length = AT_DATA_LEN_MAX;
            msg.param = at_data_line;
            at_data_task_send(&msg);
        }
    }
    else //AT_STATE_SENDING_DATA
    {
        at_uart1_printf((char *)"\r\nbusy p...\r\n");
    }
}

void uart1_rx_int_do_at_impl(uint32_t u32Data)
{
    at_uart_buffer_t *p;
    xATMessage txMsg;
    int lock = data_process_lock_get();

    p = &at_rx_buf;

    //command input for all modules
    if (lock == LOCK_NONE)
    {
        if( p->in < (AT_RBUF_SIZE-1) ) // one reserved for '\0'
        {
            uint8_t u8Buff = 0;
            u8Buff = u32Data & 0xFF;

            if( (u8Buff == 0x0A) || (u8Buff == 0x0D) )
            {
                // 0x0A:LF, 0x0D:CR. "ENTER" key. Windows:CR+LF, Linux:CR and Mac:LF
                p->buf[ p->in ] = 0x00;

                /** send message */
                txMsg.pcMessage = (char *)p;
                txMsg.length = sizeof(at_uart_buffer_t);
                txMsg.event = AT_UART1_EVENT;

                /** Post the byte. */
                at_task_send( txMsg );
                at_clear_uart_buffer();
            }
            else if(u8Buff == 0x08)
            {
                // back space
                if (p->in > 0)
                {
                    p->in--;
                    p->buf[ p->in ] = 0x00;
                    Hal_Uart_DataSend(UART_IDX_1, 0x08);
                    Hal_Uart_DataSend(UART_IDX_1, 0x20);
                    Hal_Uart_DataSend(UART_IDX_1, 0x08);
                }
            }
            else
            {
                // Others 
                p->buf[ p->in ] = u8Buff;
                if(at_echo_on) 
                    Hal_Uart_DataSend(UART_IDX_1, p->buf[p->in]);
                p->in++;
            }
        }
        // if overflow, no action.
    }

    //data input for BLE or TCP/IP module
    else
    {
        //Set handler of BLE or TCP/IP module
        switch(lock)
        {
            case LOCK_BLE:
                uart1_rx_int_at_data_receive_ble(u32Data);
                break;

            case LOCK_TCPIP:
                uart1_rx_int_at_data_receive_tcpip(u32Data);
                break;

            default:
                break;
        }
    }
}
/*
 * @brief BLE HCI mode handler
 *
 * @param [in] u32Data 4bytes data from UART1 interrupt
 */
void uart1_rx_int_do_ble_hci_impl(uint32_t u32Data)
{
    le_ctrl_hci_uart_input(u32Data);
}

/*
 * @brief BLE HOST mode handler (Reserved)
 *
 * @param [in] u32Data 4bytes data from UART1 interrupt
 */
void uart1_rx_int_do_ble_host_impl(uint32_t u32Data)
{
    /** Reserved */
}

/*
 * @brief Others mode handler (Reserved)
 *
 * @param [in] u32Data 4bytes data from UART1 interrupt
 */
void uart1_rx_int_do_others_impl(uint32_t u32Data)
{
    /** Reserved */
}

/*
 * @brief UART1 RX Interrupt default handler
 *
 * @param [in] u32Data 4bytes data from UART1 interrupt
 */
void uart1_rx_int_handler_default_impl(uint32_t u32Data)
{
    int mode;

    mode = uart1_mode_get();
    switch (mode) {
    	case UART1_MODE_AT:
            /** Do AT CMD */
            uart1_rx_int_do_at(u32Data);
    		break;
        case UART1_MODE_BLE_HCI:
            /** Do BLE HCI */
            uart1_rx_int_do_ble_hci(u32Data);
    		break;
        case UART1_MODE_BLE_HOST:
            /** Do BLE HOST (Reserved) */
            //uart1_rx_int_do_ble_host(u32Data);
    		break;
        case UART1_MODE_OTHERS:
            /** Do Others (Reserved) */
            uart1_rx_int_do_others(u32Data);
            break;
    	default:
            /** Do AT CMD */
            uart1_rx_int_do_at(u32Data);
    		break;
	}
}

/*
 * @brief buffer flag clear
 *
 */
void at_clear_uart_buffer_impl(void)
{
    at_rx_buf.in = 0;
}

char *at_cmd_param_trim(char *sParam)
{
    char *sRet = NULL;
    int iLen = strlen(sParam);

    if(iLen == 0) goto done;

    if(sParam[0] == '"')
    {
        if(iLen < 2)
        {
            printf("[%s %d] invalid param[%s]\n", __func__, __LINE__, sParam);
            goto done;
        }

        if(sParam[iLen - 1] != '"')
        {
            printf("[%s %d] invalid param[%s]\n", __func__, __LINE__, sParam);
            goto done;
        }

        sParam[0] = 0;
        sParam[iLen - 1] = 0;
        sRet = &(sParam[1]);
    }
    else
    {
        if(sParam[iLen - 1] == '"')
        {
            printf("[%s %d] invalid param[%s]\n", __func__, __LINE__, sParam);
            goto done;
        }

        sRet = sParam;
    }

done:
    return sRet;
}

int at_cmd_buf_to_argc_argv(char *pbuf, int *argc, char *argv[], int iArgvNum)
{
    int count = 0;
    char *p = NULL;
    char *pTrim = NULL;
    char *pSavedPtr = NULL;

    if((!pbuf) || (!argc) || (!argv) || (!iArgvNum))
    {
        return 0;
    }

    /** Get the first word */
    p = strtok_r(pbuf, "=", &pSavedPtr);
    argv[count] = p;
    //msg_print_uart1("\r\n at_cmd_buf_to_argc_argv, argv[%d]:%s ", count, argv[count]);
    count++;

    while ((p = strtok_r(NULL, ",", &pSavedPtr)) != NULL)
    {
        if(count >= iArgvNum)
        {
            break;
        }

        //argv[count] = p;
        //msg_print_uart1("\r\n at_cmd_buf_to_argc_argv, argv[%d]:%s ", count, argv[count]);
        pTrim = at_cmd_param_trim(p);
        if(pTrim == NULL)
        {
            argv[count] = NULL;
            return false;
        }
        argv[count] = pTrim;
        count++;
    }
    *argc = count;

    return true;
}

void at_uart1_write_buffer(char *buf, int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        if(buf[i] == '\n') {
            Hal_Uart_DataSend(UART_IDX_1, '\r');
        }
        Hal_Uart_DataSend(UART_IDX_1, buf[i]);
    }
}

void at_uart1_printf(char *str)
{
    if (str == NULL) {
        return;
    }
    at_uart1_write_buffer(str, strlen(str));
}

void at_response_result(uint8_t result_code)
{
    if (result_code < AT_RESULT_CODE_MAX) {
        at_uart1_printf(at_result_string[result_code]);
    }
}

void at_cmd_switch_uart1_dbguart_impl(void)
{
    osSemaphoreWait(g_tSwitchuartSem, osWaitForever);

#if defined(OPL1000_APS)
    if (g_eIO01UartMode == IO01_UART_MODE_AT)
    {
        Hal_Pin_ConfigSet(0, PIN_TYPE_UART_APS_TX, PIN_DRIVING_FLOAT);
        Hal_Pin_ConfigSet(1, PIN_TYPE_UART_APS_RX, PIN_DRIVING_FLOAT);

        Hal_Pin_ConfigSet(8, PIN_TYPE_UART1_TX, PIN_DRIVING_FLOAT);
        Hal_Pin_ConfigSet(9, PIN_TYPE_UART1_RX, PIN_DRIVING_HIGH);
    }
    else
    {
        Hal_Pin_ConfigSet(8, PIN_TYPE_UART_APS_TX, PIN_DRIVING_FLOAT);
        Hal_Pin_ConfigSet(9, PIN_TYPE_UART_APS_RX, PIN_DRIVING_HIGH);
     
        Hal_Pin_ConfigSet(0, PIN_TYPE_UART1_TX, PIN_DRIVING_FLOAT);
        Hal_Pin_ConfigSet(1, PIN_TYPE_UART1_RX, PIN_DRIVING_FLOAT);
    }
#elif defined(OPL2000_APS)
    if (g_eIO01UartMode == IO01_UART_MODE_AT)
    {   /* Switch 0, 2 to debug UART */
        Hal_Pin_Config(PIN_TYPE_APS_UART_TXD_IO0);
        Hal_Pin_Config(PIN_TYPE_APS_UART_RXD_IO2|PIN_INMODE_IO2_PULL_UP);

        Hal_Pin_Config(PIN_TYPE_UART1_TXD_IO5);
        Hal_Pin_Config(PIN_TYPE_UART1_RXD_IO1|PIN_INMODE_IO1_PULL_UP);
    }
    else
    {   /* Switch 0, 2 to AT UART */
        Hal_Pin_Config(PIN_TYPE_APS_UART_TXD_IO5);
        Hal_Pin_Config(PIN_TYPE_APS_UART_RXD_IO1|PIN_INMODE_IO1_PULL_UP);
     
        Hal_Pin_Config(PIN_TYPE_UART1_TXD_IO0);
        Hal_Pin_Config(PIN_TYPE_UART1_RXD_IO2|PIN_INMODE_IO2_PULL_UP);
    }
#else
    #error "Wrong definition"
#endif
    g_eIO01UartMode = (E_IO01_UART_MODE)!g_eIO01UartMode;
    osSemaphoreRelease(g_tSwitchuartSem);    
}

void at_io01_uart_mode_set_impl(E_IO01_UART_MODE eMode)
{
    g_eIO01UartMode = eMode;
}


void set_echo_on_impl(int on)
{
    int enable = false;
    if(on) enable = true;
    at_echo_on = enable;
}

int wpas_get_state(void)
{
    if (wpa_s == NULL) return 0;
    return ((int)wpa_s->wpa_state);
}

int wpas_get_assoc_freq(void)
{
    if (wpa_s == NULL) return 0;
    return get_channel_from_bss_info();
}

/*
 * @brief parser argument to integer type.
 *
 * @param [in] *para char type
 *
 * @param [out] *result return integer type
 *
 * @return 0 fail 1 success
 */
int at_cmd_get_para_as_digital_impl(char *para, int *result)
{
    char *ptr = NULL;
    int ret = -1;
    int value = 0;
    int8_t sign = 1;
    uint8_t base = 10;
    uint8_t ch = 0;
    uint32_t cutoff = 0;
    int cutlim = 0;
    
    ptr = para;
    
    if (*ptr == '\0') {
        goto exit;
    }

    if (*ptr == '0') {
        if (strlen((char *)ptr) >= 2) {
            if (ptr[1] == 'x' || ptr[1] == 'X') {
                base = 16;
                ptr += 2;
            }
        }
    }
    else if (*ptr == '-') {
        sign = -1;
        ptr++;
    }
    
    cutoff = (unsigned long)0xffffffff / (unsigned long)base;
    cutlim = (unsigned long)0xffffffff % (unsigned long)base;
    
    while (*ptr != '\0') {
        if ((*ptr >= '0') && (*ptr <= '9')) {
            ch = *ptr - '0';
        } else if ((*ptr >= 'a') && (*ptr <= 'f')) {
            ch = *ptr - 'a' + 10;
        } else if((*ptr >= 'A') && (*ptr <= 'F')) {
            ch = *ptr - 'A' + 10;
        } else {
            goto exit;
        }
        
        if (ch >= base) {
            goto exit;
        }
        
        if ((value > cutoff) || (value == cutoff && ch > cutlim)) {
            goto exit;
        }
        
        //printf("value=%d, base=%d, ch=%d\n", value, base, ch);
        
        value = value * base + ch;
        ptr++;
    }
    
    *result = value * sign;
    
    ret = 0;
exit:
    return ret;
}

/*
 * @brief AT Common Interface Initialization (AT Common)
 *
 */
void at_cmd_common_func_init(void)
{
    memset(&at_rx_buf, 0, sizeof(at_uart_buffer_t));

    /** Functions: AT Common */
    msg_print_uart1              = msg_print_uart1_impl;
    uart1_mode_get               = uart1_mode_get_impl;
    uart1_mode_set               = uart1_mode_set_impl;
    uart1_mode_set_at            = uart1_mode_set_at_impl;
    uart1_mode_set_ble_hci       = uart1_mode_set_ble_hci_impl;
    uart1_mode_set_ble_host      = uart1_mode_set_ble_host_impl;
    uart1_mode_set_others        = uart1_mode_set_others_impl;
    uart1_mode_set_default       = uart1_mode_set_default_impl;
    //uart1_rx_int_do_at         = uart1_rx_int_do_at_impl;
    uart1_rx_int_do_ble_hci      = uart1_rx_int_do_ble_hci_impl;
    uart1_rx_int_do_ble_host     = uart1_rx_int_do_ble_host_impl;
    uart1_rx_int_do_others       = uart1_rx_int_do_others_impl;
    uart1_rx_int_handler_default = uart1_rx_int_handler_default_impl;
    at_clear_uart_buffer         = at_clear_uart_buffer_impl;

    uart1_rx_int_do_at           = uart1_rx_int_do_at_impl;
    uart1_rx_int_at_data_receive_ble   = uart1_rx_int_at_data_receive_ble_impl;
    uart1_rx_int_at_data_receive_tcpip = uart1_rx_int_at_data_receive_tcpip_impl;

    at_cmd_switch_uart1_dbguart  = at_cmd_switch_uart1_dbguart_impl;
    
    at_echo_on = true;
    set_echo_on                  = set_echo_on_impl;
    at_io01_uart_mode_set        = at_io01_uart_mode_set_impl;
    
    at_cmd_get_para_as_digital   = at_cmd_get_para_as_digital_impl;
    
    at_io01_uart_mode_set(HAL_PIN_0_1_UART_MODE_DEF);
}

