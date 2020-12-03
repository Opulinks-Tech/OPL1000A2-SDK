/* *****************************************************************************
 *  Copyright 2019, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2020
 *
 *******************************************************************************
 *
 * @file os_partition_pool_patch.h 
 * 
 * @brief Patch for PMP 
 *
 *  
 ******************************************************************************/

#ifndef _OS_PARTITION_POOL_PATCH_H_
#define _OS_PARTITION_POOL_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */

#include "os_partition_pool.h"
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
typedef void (*vPartitionPoolReachThresholdHook_fp)(void);


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

void vPartitionPoolPatchInit(void);
void vPartitionPoolDetectStart(uint32_t u32Threshold, uint32_t u32PeriodMs, vPartitionPoolReachThresholdHook_fp pFunc);
void vPartitionPoolDetectStop(void);
void vVmpInfoDump(void);


#ifdef __cplusplus
}
#endif
#endif  /* _OS_PARTITION_POOL_PATCH_H_ */
