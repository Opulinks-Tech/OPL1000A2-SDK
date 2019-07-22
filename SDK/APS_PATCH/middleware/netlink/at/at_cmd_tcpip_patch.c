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
#include <time.h>
#include "cmsis_os.h"
#include "at_cmd.h"
#include "at_cmd_common.h"
#include "at_cmd_tcpip.h"
#include "at_cmd_nvm.h"
#include "at_cmd_data_process.h"
#include "hal_wdt.h"
#include "sys_os_config.h"

#include "at_cmd_tcpip_patch.h"

extern int at_trans_client_init_impl(at_nvm_trans_config_t *trans_cfg);
extern int at_cmd_tcpip_savetranslink(char *buf, int len, int mode);
extern int at_cmd_tcpip_cipserver(char *buf, int len, int mode);
extern int at_cmd_tcpip_cipstart(char *buf, int len, int mode);
extern void at_create_tcpip_tx_task_impl(void);

/******************************************************
 *                      Macros
 ******************************************************/
//#define AT_CMD_TCPIP_DBG

#ifdef AT_CMD_TCPIP_DBG
    #define AT_LOG                      printf(("[AT]: "fmt"\r\n"), ##arg)
#else
    #define AT_LOG(...)
#endif

#define AT_LOGI(fmt,arg...)             printf(("[AT]: "fmt"\r\n"), ##arg)

/******************************************************
 *               Variable Definitions
 ******************************************************/
extern osThreadId   at_tx_task_id;
extern osPoolId     at_tx_task_pool_id;
extern osMessageQId at_tx_task_queue_id;
extern at_command_t *g_AtCmdTbl_Tcpip_Ptr;
extern uint32_t at_tx_data_task_stack_size;
extern volatile bool at_ipMux;
extern volatile bool at_ip_mode; // 0: normal transmission mode. 1:transparent transmission
extern int at_tx_data_task_priority;
extern const osPoolDef_t os_pool_def_at_tx_task_pool;
extern int at_netconn_max_num;
extern volatile uint16_t server_timeover;
extern uint8_t g_server_mode;
extern uint32_t g_server_port;
extern int tcp_server_socket;

/******************************************************
 *               Function Definitions
 ******************************************************/
static void at_server_timeout_handler_patch(void)
{
    uint8_t loop = 0;
    at_socket_t *link = NULL;
    //char resp_buf[32];
    
    for (loop = 0; loop < at_netconn_max_num; loop++) {
        link = at_link_get_id(loop);
        if ((link->sock >= 0) && (link->terminal_type == AT_REMOTE_CLIENT)) {
            link->server_timeout++;

            if (link->server_timeout >= server_timeover) {
                at_socket_client_cleanup_task(link);
                //at_sprintf(resp_buf, "%d,CLOSED\r\n", loop);
                //msg_print_uart1(resp_buf);
                msg_print_uart1("%d,CLOSED\r\n", loop);
            }
        }
    }
    AT_LOGI("at_server_timeout_handler\r\n");
}

static void at_socket_process_task_patch(void *arg)
{
    at_socket_t* plink = (at_socket_t*)arg;
    bool link_count_flag = false;

    if (plink == NULL) {
        plink->task_handle = NULL;
        vTaskDelete(NULL);
    }

    //xSemaphoreTake(plink->sema,0);
    if ((plink->link_type == AT_LINK_TYPE_TCP) || (plink->link_type == AT_LINK_TYPE_SSL)) {
        link_count_flag = true;
        at_update_link_count(1);
    }

    for(;;)
    {
        if ((plink->link_state == AT_LINK_DISCONNECTED) || (plink->link_state == AT_LINK_DISCONNECTING)) {
            if (at_ip_mode == true) {
                if (at_create_tcp_client_trans(plink) < 0) {
                    vTaskDelay(g_at_trans_recreate_ms);
                    continue;
                }
            }
            else {
                break;
            }
        }

        at_process_recv_socket(plink);
        Hal_Wdt_Clear();
    }

    if (link_count_flag) {
        at_update_link_count(-1);
    }

    if (plink->recv_buf) {
        free(plink->recv_buf);
        plink->recv_buf = NULL;
        AT_LOGI("plink->recv_buf=%p\r\n", plink->recv_buf);
    }

    AT_LOGI("socket recv delete\r\n");

    if(plink->link_state == AT_LINK_DISCONNECTING) {
        //xSemaphoreGive(plink->sema);
        plink->link_state = AT_LINK_DISCONNECTED;
    }

    plink->task_handle = NULL;
    vTaskDelete(NULL);
}


int at_trans_client_init_patch(at_nvm_trans_config_t *trans_cfg)
{
    if (at_trans_client_init_impl(trans_cfg) == AT_RESULT_CODE_OK) {
        at_create_tcpip_data_task();
        return AT_RESULT_CODE_OK;
    }
    else {
        return AT_RESULT_CODE_ERROR;
    }
}
 
void at_create_tcpip_tx_task_patch(void)
{
    if (at_tx_task_id != NULL)
        return;
    at_create_tcpip_tx_task_impl();
}

/*
 * @brief Command at+cipstart
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_tcpip_cipstart_patch(char *buf, int len, int mode)
{
    char *argv[AT_MAX_CMD_ARGS] = {0};
    int argc = 0;
    uint8_t linkID;
    at_socket_t *link;
    char *tmp_buf = NULL;
    
    tmp_buf = malloc(len);
    if (tmp_buf == NULL) {
        goto exit;
    }
    memcpy(tmp_buf, buf, len);
    tmp_buf[len] = '\0';
    
    at_cmd_tcpip_cipstart(buf, len, mode);
    
    if (!at_cmd_buf_to_argc_argv(tmp_buf, &argc, argv, AT_MAX_CMD_ARGS))
        goto exit;

    if (at_ipMux)
        linkID = atoi(argv[1]);
    else
        linkID = 0;
    
    link = at_link_get_id(linkID);
    if (link->sock >= 0) {
        at_create_tcpip_data_task();
    }

exit:
    if (tmp_buf) free(tmp_buf);
    return true;
}

/*
 * @brief Command at+cipserver
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_tcpip_cipserver_patch(char *buf, int len, int mode)
{
    at_cmd_tcpip_cipserver(buf, len, mode);
    if (tcp_server_socket >= 0) {
        at_create_tcpip_data_task();
    }
    return true;
}

/*
 * @brief Command at+savetranslink
 *
 * @param [in] argc count of parameters
 *
 * @param [in] argv parameters array
 *
 * @return 0 fail 1 success
 *
 */
int at_cmd_tcpip_savetranslink_patch(char *buf, int len, int mode)
{
    int enable = 0;
    int argc = 0;
    char *argv[AT_MAX_CMD_ARGS] = {0};
    char *tmp_buf = NULL;
    
    if (mode == AT_CMD_MODE_SET) {
        tmp_buf = malloc(len + 1);
        if (tmp_buf == NULL) {
            goto exit;
        }
        memcpy(tmp_buf, buf, len);
        tmp_buf[len] = '\0';
        
        if (!at_cmd_buf_to_argc_argv(tmp_buf, &argc, argv, AT_MAX_CMD_ARGS)) {
            goto exit;
        }
        
        if (at_cmd_get_para_as_digital(argv[1], &enable) != 0) {
            at_response_result(AT_RESULT_CODE_ERROR);
            goto exit;
        }
        
        if (enable > 1 || enable < 0) {
            at_response_result(AT_RESULT_CODE_ERROR);
            goto exit;
        }
        
        at_cmd_tcpip_savetranslink(buf, len, mode);
    }
    
exit:
    if (tmp_buf) free(tmp_buf);
    return true;
}

/*-------------------------------------------------------------------------------------
 * Interface assignment
 *------------------------------------------------------------------------------------*/
void at_cmd_tcpip_api_preinit_patch(void)
{
    at_trans_client_init           = at_trans_client_init_patch;
    at_create_tcpip_tx_task        = at_create_tcpip_tx_task_patch;
    at_server_timeout_handler      = at_server_timeout_handler_patch;
    at_socket_process_task         = at_socket_process_task_patch;
    
    g_AtCmdTbl_Tcpip_Ptr[2].cmd_handle   = at_cmd_tcpip_cipstart_patch;
    g_AtCmdTbl_Tcpip_Ptr[8].cmd_handle   = at_cmd_tcpip_cipserver_patch;
    g_AtCmdTbl_Tcpip_Ptr[10].cmd_handle  = at_cmd_tcpip_savetranslink_patch;
}
