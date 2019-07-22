/* *****************************************************************************
 *  Copyright 2019, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2018
 *
 *******************************************************************************
 *
 *  @file le_ctrl_patch.h
 * 
 *  @brief 
 *  
 ******************************************************************************/

#ifndef _LE_CTRL_PATCH_H_
#define _LE_CTRL_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "le_ctrl.h"
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



/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */


/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */
// le_ctrl_func_map
void le_ctrl_pre_patch_init(void);

// le_ctrl_hci_uart_tl

// le_ctrl_data

// le_ctrl_enc

// le_ctrl_hci
void le_ctrl_init_patch(void);

// le_ctrl_hci_handler

// le_ctrl_fim

// le_ctrl_cmd



#ifdef __cplusplus
}
#endif
#endif  /* _LE_CTRL_PATCH_H_ */
