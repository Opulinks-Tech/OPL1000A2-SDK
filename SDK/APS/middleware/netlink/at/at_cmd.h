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
 * @file at_cmd.h
 * @author Michael Liao
 * @date 14 Dec 2017
 * @brief File containing declaration of at_cmd api & definition of structure for reference.
 *
 */

#ifndef __AT_CMD_H__
#define __AT_CMD_H__

#define AT_MAX_CMD_ARGS            10

/**
 * @brief Function Pointer Type for API at_cmd_parse
 *
 */
typedef int (*at_cmd_parse_fp_t)(char *pbuf);

/**
 * @brief Function Pointer Type for API at_cmd_handler
 *
 */
typedef int (*at_cmd_handler_fp_t)(char *buf, int len, int mode);

/**
 * @brief Function Pointer Type for API at_cmd_extend
 *
 */
typedef int (*at_cmd_extend_fp_t)(char *buf, int len, int mode);

/**
 * @brief Function Pointer Type for API at_cmd_ext_tbl_reg
 *
 */
typedef int (*at_cmd_ext_tbl_reg_fp_t)(void *ptr);

/*
 * @brief Extern Function at_cmd_parse
 *
 */
extern at_cmd_parse_fp_t at_cmd_parse;

/*
 * @brief Extern Function at_cmd_handler
 *
 */
extern at_cmd_handler_fp_t at_cmd_handler;

/*
 * @brief Extern Function at_cmd_extend
 *
 */
//extern at_cmd_extend_fp_t at_cmd_extend;

/*
 * @brief Extern Function at_cmd_ext_tbl_reg
 *
 */
extern at_cmd_ext_tbl_reg_fp_t at_cmd_ext_tbl_reg;

/**
 * @brief AT Command Interface Initialization Entry Point
 *
 */
void at_cmd_func_init(void);

/**
 * @brief AT Command Structure
 *
 */
typedef struct at_command {
    const char *cmd; /**< Command String. */
    at_cmd_handler_fp_t cmd_handle; /**< Command Handler. */
    const char *cmd_usage; /**< Command Description. */
}at_command_t;

typedef struct S_CmdTblLst
{
    at_command_t *taCmdTbl;
    struct S_CmdTblLst *ptNext;
} T_CmdTblLst;


extern T_CmdTblLst g_tAtCmdExtTblLst;

#endif //__AT_CMD_H__
