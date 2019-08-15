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
 * @file at_cmd_task.h
 * @author Michael Liao
 * @date 14 Dec 2017
 * @brief File containing declaration of at_cmd_task api & definition of structure for reference.
 *
 */

#ifndef __AT_CMD_TASK_H__
#define __AT_CMD_TASK_H__

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "cmsis_os.h"

/**
 * @brief Event 1
 *
 */
#ifndef AT_COMMAND_EVENT
#define AT_COMMAND_EVENT        0x01
#endif

/**
 * @brief Event 2
 *
 */
#ifndef AT_COMMAND1_EVENT
#define AT_COMMAND1_EVENT       0x02
#endif

/**
 * @brief AT Event 1
 *
 */
#ifndef AT_UART_EVENT
#define AT_UART_EVENT           (0x0001)
#endif

/**
 * @brief AT Event 2
 *
 */
#ifndef AT_TEST_EVENT
#define AT_TEST_EVENT           (0x0002)
#endif

/**
 * @brief AT Event 3
 *
 */
#ifndef AT_UART1_EVENT
#define AT_UART1_EVENT          (0x0003)
#endif

/**
 * @brief Queue Size of AT Command Task
 *
 */
#ifndef AT_QUEUE_SIZE
#define AT_QUEUE_SIZE           (5)
#endif

/**
 * @brief RBuffer Size of AT Command Task
 *
 */
#ifndef AT_RBUF_SIZE
#define AT_RBUF_SIZE            256
#endif

/**
 * @brief Buffer Size of AT Command Task
 *
 */
#ifndef MAX_AT_CMD_BUFFER_SIZE
#define	MAX_AT_CMD_BUFFER_SIZE  24
#endif

/**
 * @brief Size of AT Command Name
 *
 */
#ifndef MAX_AT_CMD_NAME_SIZE
#define MAX_AT_CMD_NAME_SIZE    4
#endif

/**
 * @brief How many arguments we allow
 *
 */
#ifndef MAX_AT_CMD_ARGV_SIZE
#define MAX_AT_CMD_ARGV_SIZE    16
#endif


/**
 * @brief UART1 Buffer for Received Message
 *
 */
typedef struct
{
    unsigned char in;       /**< Next In Index. */
    unsigned char out;      /**< Next Out Index. */
    char buf[AT_RBUF_SIZE]; /**< Buffer. */
} at_uart_buffer_t;

/**
 * @brief Message Structure for AT CMD Task
 *
 */
typedef struct
{
	unsigned int event;  /**< Event Type. */
	unsigned int length; /**< Event Length. */
	char *pcMessage;     /**< Event Message. */
} xATMessage;


extern osSemaphoreId g_tSwitchuartSem;

/**
 * @brief Function Pointer Type for API at_task_cmd_process
 *
 */
typedef void (*at_task_cmd_process_fp_t)(char *pbuf, int len);

/**
 * @brief Function Pointer Type for API at_task_create_semaphore
 *
 */
typedef void (*at_task_create_semaphore_fp_t)(void);

/**
 * @brief Function Pointer Type for API at_semaphore_release
 *
 */
typedef void (*at_semaphore_release_fp_t)(void);

/**
 * @brief Function Pointer Type for API at_task_create
 *
 */
typedef void (*at_task_create_fp_t)(void);

/**
 * @brief Function Pointer Type for API at_task_send
 *
 */
typedef osStatus (*at_task_send_fp_t)(xATMessage txMsg);

/**
 * @brief Function Pointer Type for API at_task
 *
 */
typedef void (*at_task_fp_t)(void *pvParameters);

/**
 * @brief Function Pointer Type for API at_module_init
 *
 */
typedef void (*at_module_init_fp_t)(uint32_t netconn_max, const char *custom_version);

/**
 * @brief Function Pointer Type for API at_wifi_net_task_init
 *
 */
typedef int (*at_wifi_net_task_init_fp_t) (void);

/**
 * @brief Function Pointer Type for API at_blewifi_auto_trans_init
 *
 */
typedef void (*at_blewifi_auto_trans_init_fp_t)(void);

/**
 * @brief Extern Function at_task_cmd_process
 *
 */
extern at_task_cmd_process_fp_t at_task_cmd_process;

/**
 * @brief Extern Function at_task_create_semaphore
 *
 */
extern at_task_create_semaphore_fp_t at_task_create_semaphore;

/**
 * @brief Extern Function at_semaphore_release
 *
 */
extern at_semaphore_release_fp_t at_semaphore_release;

/**
 * @brief Extern Function at_task_create
 *
 */
extern at_task_create_fp_t at_task_create;

/**
 * @brief Extern Function at_task_send
 *
 */
extern at_task_send_fp_t at_task_send;

/**
 * @brief Extern Function at_task
 *
 */
extern at_task_fp_t at_task;


/**
 * @brief Extern Function at_task
 *
 */
extern at_module_init_fp_t at_module_init;

/**
 * @brief Extern Function at_wifi_net_task_init
 *
 */
extern at_wifi_net_task_init_fp_t at_wifi_net_task_init;

/**
 * @brief Extern Function at_blewifi_auto_trans_init
 *
 */
extern at_blewifi_auto_trans_init_fp_t at_blewifi_auto_trans_init;

/*
 * @brief AT Command Interface Initialization for AT Command Task
 *
 */
void at_task_func_init(void);

#endif //__AT_CMD_TASK_H__

