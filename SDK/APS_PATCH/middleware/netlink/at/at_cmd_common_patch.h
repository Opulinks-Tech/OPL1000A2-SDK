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
typedef struct {
    uint32_t write_index;
    uint32_t read_index;
    uint32_t num;
    uint32_t mask;
    uint8_t *pbuf;
} at_trans_ring_buf_cfg_t;

#define RB_GET_NUM(st) \
    ((st).num)

#define RB_GET_MASK(st) \
    ((st).mask)

#define RB_IS_FULL(st) \
    (((uint32_t)((st).write_index - (st).read_index)) >= RB_GET_NUM((st)))

#define RB_GET_COUNT(st) \
    (((uint32_t)((st).write_index) - (st).read_index)) & RB_GET_MASK((st)))

#define RB_PUT_VAL(st, value) \
    { \
        (st).pbuf[(st).write_index & RB_GET_MASK((st))] = (value); \
        ++(st).write_index; \
    }

#define RB_GET_VAL(st, value) \
    { \
        (value) = (st).pbuf[(st).read_index & RB_GET_MASK((st))] ; \
        ++(st).read_index; \
    }
        
#define RB_GET_READ_INDEX(st) \
    ((st).read_index)

#define RB_GET_WRITE_INDEX(st) \
    ((st).write_index)

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
