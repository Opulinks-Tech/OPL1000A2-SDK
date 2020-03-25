/* *****************************************************************************
 *  Copyright 2019, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2019
 *
 *******************************************************************************
 *
 *  @file at_cmd_common_patch.h
 * 
 *  @brief 
 *  
 ******************************************************************************/

#ifndef _AT_CMD_COMMON_PATCH_H_
#define _AT_CMD_COMMON_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "at_cmd_common.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */


/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */
typedef void (*uart1_rx_int_at_data_receive_app_fp_t)(uint32_t u32Data);
/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */
extern uart1_rx_int_at_data_receive_app_fp_t uart1_rx_int_at_data_receive_app;
/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */
void at_cmd_common_func_init_patch(void);
void at_uart1_write_to_uart_normal(char *buf, int len);

#ifdef __cplusplus
}
#endif
#endif  /* _AT_CMD_COMMON_PATCH_H_ */
