/******************************************************************************
*  Copyright 2019, Netlink Communication Corp.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Netlink Communication Corp. (C) 2019
******************************************************************************/

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
//#include "le_cmd_app_cmd.h"
#include "controller_wifi_com.h"

#include "at_cmd_common_patch.h"
#include "at_cmd_data_process_patch.h"
#include "agent_patch.h"

at_trans_ring_buf_cfg_t at_ring_buf;

extern TimerHandle_t at_trans_timer;

/*
 * @brief Global variable msg_print_uart1 retention attribute segment
 *
 */
extern msg_print_uart1_fp_t msg_print_uart1;

/*
 * @brief Global variable uart1_mode_get retention attribute segment
 *
 */
extern uart1_mode_get_fp_t uart1_mode_get;

/*
 * @brief Global variable uart1_mode_set retention attribute segment
 *
 */
extern uart1_mode_set_fp_t uart1_mode_set;

/*
 * @brief Global variable uart1_mode_set_at retention attribute segment
 *
 */
extern uart1_mode_set_at_fp_t uart1_mode_set_at;

/*
 * @brief Global variable uart1_mode_set_ble_hci retention attribute segment
 *
 */
extern uart1_mode_set_ble_hci_fp_t uart1_mode_set_ble_hci;

/*
 * @brief Global variable uart1_mode_set_ble_host retention attribute segment
 *
 */
extern uart1_mode_set_ble_host_fp_t uart1_mode_set_ble_host;

/*
 * @brief Global variable uart1_mode_set_others retention attribute segment
 *
 */
extern uart1_mode_set_others_fp_t uart1_mode_set_others;

/*
 * @brief Global variable uart1_mode_set_default retention attribute segment
 *
 */
extern uart1_mode_set_default_fp_t uart1_mode_set_default;

/*
 * @brief Global variable uart1_rx_int_do_at retention attribute segment
 *
 */
extern uart1_rx_int_do_at_fp_t uart1_rx_int_do_at;

/*
 * @brief Global variable uart1_rx_int_do_ble_hci retention attribute segment
 *
 */
extern uart1_rx_int_do_ble_hci_fp_t uart1_rx_int_do_ble_hci;

/*
 * @brief Global variable uart1_rx_int_do_ble_host retention attribute segment
 *
 */
extern uart1_rx_int_do_ble_host_fp_t uart1_rx_int_do_ble_host;

/*
 * @brief Global variable uart1_rx_int_handler_default retention attribute segment
 *
 */
extern uart1_rx_int_handler_default_fp_t uart1_rx_int_handler_default;

/*
 * @brief Global variable uart1_rx_int_do_others retention attribute segment
 *
 */
extern uart1_rx_int_do_others_fp_t uart1_rx_int_do_others;

/*
 * @brief Global variable at_clear_uart_buffer retention attribute segment
 *
 */
extern at_clear_uart_buffer_fp_t at_clear_uart_buffer;

/*
 * @brief Global variable g_uart1_mode retention attribute segment
 *
 */
extern unsigned int g_uart1_mode;

/*
 * @brief Global variable at_rx_buf retention attribute segment
 *
 */
extern at_uart_buffer_t at_rx_buf;

/*
 * @brief Global variable uart1_rx_int_at_data_receive_ble retention attribute segment
 *
 */
extern uart1_rx_int_at_data_receive_ble_fp_t uart1_rx_int_at_data_receive_ble;

/*
 * @brief Global variable uart1_rx_int_at_data_receive_tcpip retention attribute segment
 *
 */
extern uart1_rx_int_at_data_receive_tcpip_fp_t uart1_rx_int_at_data_receive_tcpip;


/*
 * @brief Global variable at_cmd_switch_uart1_dbguart retention attribute segment
 *
 */
extern at_cmd_switch_uart1_dbguart_fp_t at_cmd_switch_uart1_dbguart;



/*
 * @brief Global variable set_echo_on retention attribute segment
 *
 */
extern set_echo_on_fp_t set_echo_on;

/*
 * @brief Global variable at_echo_on retention attribute segment
 *
 */
extern int at_echo_on;

/*
 * @brief Global variable at_cmd_get_para_as_digital retention attribute segment
 *
 */
extern at_cmd_get_para_as_digital_fp_t at_cmd_get_para_as_digital;


extern at_io01_uart_mode_set_fp at_io01_uart_mode_set;


/* IO 0/1 Uart mode */
extern E_IO01_UART_MODE g_eIO01UartMode;

extern struct wpa_supplicant *wpa_s;

extern uint8_t at_state;
extern uint8_t *pDataLine;
extern uint8_t  at_data_line[AT_DATA_LEN_MAX];
extern uint16_t at_send_len;
extern uint8_t sending_id;

RET_DATA uart1_rx_int_at_data_receive_app_fp_t uart1_rx_int_at_data_receive_app;

/*
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
*/

void uart1_rx_int_do_at_patch(uint32_t u32Data)
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
                if(at_echo_on)
                {
                    Hal_Uart_DataSend(UART_IDX_1, u8Buff);
                }

                // 0x0A:LF, 0x0D:CR. "ENTER" key. Windows:CR+LF, Linux:CR and Mac:LF
                p->buf[ p->in++ ] = u8Buff;
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

                    if(at_echo_on)
                    {
                        Hal_Uart_DataSend(UART_IDX_1, 0x08);
                        Hal_Uart_DataSend(UART_IDX_1, 0x20);
                        Hal_Uart_DataSend(UART_IDX_1, 0x08);
                    }
                }
            }
            else
            {
                // Others
                p->buf[ p->in ] = u8Buff;

                if(at_echo_on)
                {
                    Hal_Uart_DataSend(UART_IDX_1, p->buf[p->in]);
                }

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

            case LOCK_OTHERS:
            case LOCK_APP:
                uart1_rx_int_at_data_receive_app(u32Data);
                break;

            default:
                break;
        }
    }
}

void uart1_rx_int_at_data_receive_app_patch(uint32_t u32Data)
{
    extern uint32_t g_u32AtAppDataMax;
    extern uint32_t g_u32AtAppDataCurr;
    extern uint32_t g_u32AtAppDataOffset;

    uint8_t u8Send = 0;
    uint8_t u8Done = 0;

    at_data_line[g_u32AtAppDataOffset++] = (u32Data & 0xFF);
    g_u32AtAppDataCurr += 1;

    if(g_u32AtAppDataCurr >= g_u32AtAppDataMax)
    {
        u8Send = 1;
        u8Done = 1;
    }

    if(g_u32AtAppDataOffset >= AT_DATA_LEN_MAX)
    {
        u8Send = 1;
    }

    if(u8Send)
    {
        T_AgentMsgPatch tMsg = {0};

        tMsg.u32Type = AGENT_MSG_UART1_PATCH;
        tMsg.u32ParamLen = g_u32AtAppDataOffset;
        tMsg.pu8Param = at_data_line;

        if(agent_msg_send((T_AgentMsg *)&tMsg))
        {
            data_process_unlock();
            goto done;
        }

        g_u32AtAppDataOffset = 0;

        if(u8Done)
        {
            data_process_unlock();
        }
    }

done:
    return;
}

void msg_print_uart1_patch(char *fmt,...)
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
            Hal_Uart_DataSend(UART_IDX_1, *pt++);
        }

    	va_end(ap);
	}
}

/*
 * @brief UART1 RX Data Handler of TCP/IP (Reserved)
 *
 * @param [in] u32Data 4bytes data from UART1 interrupt
 */
void uart1_rx_int_at_data_receive_tcpip_patch(uint32_t u32Data)
{
    int send_len = 0;
    bool sendex_flag = FALSE;
    at_socket_t *link = at_link_get_id(sending_id);

    send_len = data_process_data_len_get();

    if (at_state == AT_STATE_SENDING_RECV)
    {
        *pDataLine = (u32Data & 0xFF);

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
     * interval between each packet, and a maximum of
     * 2048 bytes per packet.
     *
     * Baurd Rate       Sec/Byte     Input Bytes In 20 ms
     *  9600            0.001042           19.2
     *  115200          0.000087           230.4
     *  230400          0.000043           460.8
     *  460800          0.000022           921.6
     */
    else if (at_state == AT_STATE_TRANSMIT)
    {
        if (RB_IS_FULL(at_ring_buf) == true) {
            at_uart1_printf((char *)"\r\nError...\r\n");
            return;
        }

        RB_PUT_VAL(at_ring_buf, (u32Data & 0xFF))
    }
    else //AT_STATE_SENDING_DATA
    {
        at_uart1_printf((char *)"\r\nbusy p...\r\n");
    }
}

void at_cmd_common_func_init_patch(void)
{
    // init at transparen ring buffer
    memset(&at_ring_buf, 0, sizeof(at_trans_ring_buf_cfg_t));
    at_ring_buf.num  = AT_DATA_LEN_MAX;
    at_ring_buf.mask = at_ring_buf.num - 1;
    at_ring_buf.pbuf = &at_data_line[0];

    uart1_rx_int_do_at = uart1_rx_int_do_at_patch;
    uart1_rx_int_at_data_receive_app = uart1_rx_int_at_data_receive_app_patch;
    msg_print_uart1 = msg_print_uart1_patch;
    uart1_rx_int_at_data_receive_tcpip  = uart1_rx_int_at_data_receive_tcpip_patch;
}
