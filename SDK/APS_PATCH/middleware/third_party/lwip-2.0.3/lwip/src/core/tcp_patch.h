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
 *  @file tcp_patch.h
 * 
 *  @brief 
 *  
 ******************************************************************************/

#ifndef _TCP_PATCH_H_
#define _TCP_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "lwip/opt.h"

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
#if OPL_LWIP
/*
 * Set the number of checking threshold to kill the oldest connection 
 * that is in TIME_WAIT/FIN-WAIT1/FIN-WAIT2/CLOSING/LAST ACK state.
 * This invoke MUST before LWIP initialize. The Default value is MEMP_NUM_TCP_PCB.
 * Please do not invoke this function if you don't know have knowledge of LWIP configuration. 
 */
int tcp_kill_pcb_check_threshold_set(uint8_t num);
#endif

#ifdef __cplusplus
}
#endif
#endif  /* _TCP_PATCH_H_ */
