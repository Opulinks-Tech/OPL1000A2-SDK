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

#ifndef __DIAG_TASK_H__
#define __DIAG_TASK_H__

/* Scheduler includes. */
#include "cmsis_os.h"

#define DIAG_UART_EVENT             (0x0001)
#define DIAG_TEST_EVENT             (0x0002)

#define DIAG_QUEUE_SIZE             (5)
#define RBUF_SIZE                   128

/* For user command response.
 * When user command found and finished, return CMD_FINISHED to stop searching diag_task commands.
 * Or return CMD_CONTINUE to go on searching diag_task commands and run. */
typedef enum
{
    CMD_FINISHED=0,
    CMD_CONTINUE=1,
}E_CLI_CMD_PROC;

typedef struct
{
    unsigned char in;                                // Next In Index
    unsigned char out;                               // Next Out Index
    char buf[RBUF_SIZE];                             // Buffer
} uart_buffer_t;

typedef struct
{
	unsigned int event;
	unsigned int length;
	char *pcMessage;
} xDiagMessage;

/* ----------------------------------------------------------------------- */

#define	MAX_CMD_BUFFER_SIZE		24
#define MAX_CMD_NAME_SIZE		4
#define MAX_CMD_ARGV_SIZE		16

typedef struct
{
	uint8_t	cmd;
} mtsk_cmd, *pmtsk_cmd;

typedef struct
{
	char	CmdName[MAX_CMD_NAME_SIZE];
	uint8_t	Argc;
	uint8_t	Argv[MAX_CMD_ARGV_SIZE];
	uint8_t	IsCmdFlag;
} cmd_package_t;

typedef void Cmd_t(uint8_t argc, uint8_t *argv);

typedef struct
{
	char	CmdName[MAX_CMD_NAME_SIZE];
	Cmd_t	*fun;
} cmd_table_t;

typedef void (*cli_cmd_handle)(void);
typedef struct nl_cli_cmd
{
    const char      *cmd;
    cli_cmd_handle  cmd_handle;
    //const char      *cmd_usage;
} nl_cli_cmd_t;

extern osMessageQId xDiagQueue;
extern osThreadId DiagTaskHandle;

void uartdbg_rx_int_handler(uint32_t u32Data);


/**
 * @brief User CLI command in DbgUart.
 *        OPL1000 DbgUart command parser is after this function
 * @param pbuf [in] The input buffer pointer.
 * @param len [in] The length of input buffer.
 * @return Command searching status.
 * @retval CMD_FINISHED - Search command success. CLI command finished here.
 * @retval CMD_CONTINUE - Search command fail. After this function, it will run OPL1000 CLI commands.
 */
typedef E_CLI_CMD_PROC(*T_Cli_UserCmdProcess_fp)(char *pbuf, int len);
typedef void (*Diag_UserCmdSet_fp_t)(T_Cli_UserCmdProcess_fp ptUserCmdFunc);
typedef void (*diag_task_create_fp_t)(void);
typedef osStatus (*diag_task_send_fp_t)(xDiagMessage txMsg);
typedef void (*diag_task_fp_t)( void *pvParameters );
typedef void (*CmdProcess_fp_t)(char *pbuf, int len);
typedef void (*ParseWPACommand_fp_t)(char* pszData);
typedef void (*ParseLWIPCommand_fp_t)(char* pszData);
typedef void (*ParsePINGCommand_fp_t)(char* pszData);
typedef void (*ParseWIFICommand_fp_t)(char* pszData);
#ifdef __PMP_ENABLE__
typedef void (*ParsePmpCommand_fp_t)(char* pszData);
#endif // #__PMP_ENABLE__
//#ifdef ENHANCE_IPC
typedef void (*ParseIpcCommand_fp_t)(char *sCmd);
//#endif // #ENHANCE_IPC
typedef void (*ParseTracerCommand_fp_t)(char *sCmd);
typedef void (*ParseSYSCommand_fp_t)(char* pszData);
typedef void (*ParseDownloadCommand_fp_t)(char *sCmd);
typedef void (*ParseAtVersionCommand_fp_t)(char *sCmd);
typedef void (*ParseSimpleAtCommand_fp_t)(char *sCmd);
typedef void (*ParseLeCfgCommand_fp_t)(char *sCmd);
typedef void (*ParseLeTestCommand_fp_t)(char *sCmd);
typedef void (*ParseUnknownCommand_fp_t)(char *sCmd);
typedef void (*ParseSwitchAT_DBGCommand_fp_t)(char *sCmd);
typedef void (*ParseRegCtrlCommand_fp_t)(char *sCmd);

typedef uint32_t (*ParseParam_fp_t)(char *sCmd, char **ppbParam, uint32_t dwNum);

extern diag_task_create_fp_t diag_task_create;
extern diag_task_send_fp_t diag_task_send;
extern diag_task_fp_t diag_task;
extern CmdProcess_fp_t CmdProcess;
extern ParseWPACommand_fp_t ParseWPACommand;
extern ParseLWIPCommand_fp_t ParseLWIPCommand;
extern ParsePINGCommand_fp_t ParsePINGCommand;
extern ParseWIFICommand_fp_t ParseWIFICommand;
#ifdef __PMP_ENABLE__
extern ParsePmpCommand_fp_t ParsePmpCommand;
#endif // #__PMP_ENABLE__
//#ifdef ENHANCE_IPC
extern ParseIpcCommand_fp_t ParseIpcCommand;
//#endif // #ENHANCE_IPC
extern ParseTracerCommand_fp_t ParseTracerCommand;
extern ParseSYSCommand_fp_t ParseSYSCommand;
extern ParseDownloadCommand_fp_t ParseDownloadCommand;
extern ParseAtVersionCommand_fp_t ParseAtVersionCommand;
extern ParseSimpleAtCommand_fp_t ParseSimpleAtCommand;
extern ParseLeCfgCommand_fp_t ParseLeCfgCommand;
extern ParseLeTestCommand_fp_t ParseLeTestCommand;
extern ParseUnknownCommand_fp_t ParseUnknownCommand;
extern ParseSwitchAT_DBGCommand_fp_t ParseSwitchAT_DBGCommand;
extern ParseRegCtrlCommand_fp_t ParseRegCtrlCommand;

extern ParseParam_fp_t ParseParam;
extern Diag_UserCmdSet_fp_t Diag_UserCmdSet;

/*
   Interface Initialization: DIAG TASK
 */
void diag_task_func_init(void);
void Diag_UserCmdSet_impl(T_Cli_UserCmdProcess_fp ptUserCmdFunc);


#endif
