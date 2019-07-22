/******************************************************************************
*  Copyright 2019, Opulinks Technology Ltd.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2019
******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "at_cmd_app.h"
#include "at_cmd_common.h"
#include "at_cmd_nvm.h"
#include "at_cmd_task.h"
#include "at_cmd_tcpip.h"
#include "at_cmd_task_patch.h"
#include "sys_os_config.h"

extern uint32_t at_task_stack_size;
extern int at_task_priority;
extern osThreadId AtTaskHandle;
extern osPoolId AtMemPoolId;
extern osMessageQId xAtQueue;
extern const osPoolDef_t os_pool_def_atMemPool;
extern at_nvm_trans_config_t gTransCfg;
extern osThreadId at_app_task_id;
extern uint8_t g_wifi_init_mode;
extern volatile bool at_ip_mode; // 0: normal transmission mode. 1:transparent transmission

RET_DATA at_cmd_crlf_term_set_fp_t at_cmd_crlf_term_set;
RET_DATA at_cmd_crlf_term_get_fp_t at_cmd_crlf_term_get;
RET_DATA at_term_char_remove_fp_t at_term_char_remove;

volatile uint8_t g_u8AtCrLfTerm = 0;
char *g_sAtPendBuf = NULL;


void at_blewifi_auto_trans_init_patch(void)
{
    uint8_t at_blewifi_mode = 0;
    
    at_cmd_nvm_trans_config_get(&gTransCfg);
    at_cmd_nvm_cw_ble_wifi_mode_get(&at_blewifi_mode);
    
    if (at_blewifi_mode == true) {
        g_wifi_init_mode = 4;

        at_cmd_crlf_term_set(1); // Enable CR-LF termination for BLE/WiFi AT commands
        
        // 1. AT BleWifi init function
        if (at_blewifi_init()) {
            tracer_log(LOG_HIGH_LEVEL, "at_blewifi_init not yet\n");
            return;
        }
        
        // 2. Create a task for transparent if link was saved.
        if (gTransCfg.enable == true) {
            at_ip_mode = true;
            at_trans_save_link_task_create();
        }
    } else {
        /* Initial AT transparent if link was saved. */
        if (gTransCfg.enable == true) {
            at_ip_mode = true;
            at_wifi_net_task_init();
        }
    }
}

void at_module_init_patch(uint32_t netconn_max, const char *custom_version)
{
    osMessageQDef_t at_queue_def;
    osThreadDef_t at_task_def;
    osSemaphoreDef_t tSemDef = {0};

    /** create task */
    at_task_def.name = OS_TASK_NAME_AT;
    at_task_def.stacksize = at_task_stack_size;
    at_task_def.tpriority = (osPriority)at_task_priority;
    at_task_def.pthread = at_task;
    AtTaskHandle = osThreadCreate(&at_task_def, (void *)AtTaskHandle);

    if(AtTaskHandle == NULL)
    {
        tracer_log(LOG_HIGH_LEVEL, "create thread fail \r\n");
        msg_print_uart1("create thread fail \r\n");
    }

    /** create memory pool */
    AtMemPoolId = osPoolCreate (osPool(atMemPool)); /** create Mem Pool */
    if (!AtMemPoolId)
    {
        tracer_log(LOG_HIGH_LEVEL, "AT Task Mem Pool create Fail \r\n"); /** MemPool object not created, handle failure */
        msg_print_uart1("AT Task Mem Pool create Fail \r\n");
    }

    /** create message queue */
    at_queue_def.item_sz = sizeof( xATMessage );
    at_queue_def.queue_sz = AT_QUEUE_SIZE;
    xAtQueue = osMessageCreate(&at_queue_def, AtTaskHandle);
    if(xAtQueue == NULL)
    {
        tracer_log(LOG_HIGH_LEVEL, "create queue fail \r\n");
        msg_print_uart1("create queue fail \r\n");
    }
    
    /* Create semaphore for switch UART */    
    g_tSwitchuartSem = osSemaphoreCreate(&tSemDef, 1);
    if (g_tSwitchuartSem == NULL)
    {
        tracer_log(LOG_HIGH_LEVEL, "create semaphore fail \r\n");
        msg_print_uart1("create swUART sema fail \r\n");
    }

    //move from sys_init
    uart1_mode_set_default();

    uart1_mode_set_at();

    //at tcpip structure initial
    at_link_init(AT_LINK_MAX_NUM);
}

int at_wifi_net_task_init_patch(void)
{
    if (at_app_task_id == NULL) {
        at_cmd_wifi_hook();
        return 0;
    }
    else {
        msg_print_uart1("AT Wifi and TCPIP initialize already \r\n");
        return -1;
    }
}

void at_task_patch(void *pvParameters)
{
    osEvent rxEvent;
    xATMessage *rxMsg;
    at_uart_buffer_t *pData;

    tracer_log(LOG_HIGH_LEVEL, "AT task is created successfully! \r\n");
    msg_print_uart1("\r\n>");

    at_blewifi_auto_trans_init();
    
    for(;;)
    {
        /** wait event */
        rxEvent = osMessageGet(xAtQueue, osWaitForever);
        if(rxEvent.status != osEventMessage) continue;

        rxMsg = (xATMessage *) rxEvent.value.p;

        if(rxMsg->event == AT_UART1_EVENT)
        {
            pData = (at_uart_buffer_t *)rxMsg->pcMessage;

            if(g_sAtPendBuf)
            {
                if(pData->buf[0] == 0x0A)
                {
                    at_term_char_remove(g_sAtPendBuf);
                    at_task_cmd_process(g_sAtPendBuf, strlen(g_sAtPendBuf));
                }
                else if(pData->buf[pData->in - 1] == 0x0D)
                {
                    msg_print_uart1("\r\nLF not found. Discard pending command.\r\n");
                    msg_print_uart1("ERROR\r\n");

                    if(strlen(g_sAtPendBuf) < pData->in)
                    {
                        free(g_sAtPendBuf);

                        g_sAtPendBuf = (char *)malloc(pData->in + 1);

                        if(!g_sAtPendBuf)
                        {
                            goto done;
                        }
                    }
                        
                    memcpy(g_sAtPendBuf, pData->buf, pData->in + 1);
                    goto done;
                }
                else
                {
                    msg_print_uart1("\r\nUnexpected begining[%02X]. Discard pending and current commands.\r\n", pData->buf[0]);
                    msg_print_uart1("ERROR\r\n");
                }

                free(g_sAtPendBuf);
                g_sAtPendBuf = NULL;
            }
            else
            {
                if(at_cmd_crlf_term_get())
                {
                    if(pData->buf[pData->in - 1] == 0x0D)
                    {
                        g_sAtPendBuf = (char *)malloc(pData->in + 1);
                        
                        if(g_sAtPendBuf)
                        {
                            memcpy(g_sAtPendBuf, pData->buf, pData->in + 1);
                        }
                    }

                    goto done;
                }
                
                at_term_char_remove(pData->buf);
                at_task_cmd_process(pData->buf, strlen(pData->buf));
            }
        }

    done:
        if(rxMsg->pcMessage != NULL) free(rxMsg->pcMessage);
        osPoolFree (AtMemPoolId, rxMsg);
    }
}

void at_cmd_crlf_term_set_patch(uint8_t u8Enable)
{
    if(u8Enable != g_u8AtCrLfTerm)
    {
        g_u8AtCrLfTerm = u8Enable;
    }
    
    return;
}

uint8_t at_cmd_crlf_term_get_patch(void)
{
    return g_u8AtCrLfTerm;
}

void at_term_char_remove_patch(char *sBuf)
{
    int iLen = strlen(sBuf);
    int i = 0;

    if(!iLen)
    {
        goto done;
    }

    for(i = iLen - 1; i >= 0; i--)
    {
        if((sBuf[i] == 0x0D) || (sBuf[i] == 0x0A))
        {
            sBuf[i] = 0x00;
        }
        else
        {
            break;
        }
    }

done:
    return;
}

void at_task_func_init_patch(void)
{
    at_blewifi_auto_trans_init       = at_blewifi_auto_trans_init_patch;
    at_module_init                   = at_module_init_patch;
    at_task                          = at_task_patch;
    at_cmd_crlf_term_set             = at_cmd_crlf_term_set_patch;
    at_cmd_crlf_term_get             = at_cmd_crlf_term_get_patch;
    at_term_char_remove              = at_term_char_remove_patch;
}
