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
 *  @file cmsis_os_patch.h
 *
 *  @brief
 *
 ******************************************************************************/

#ifndef _CMSIS_OS_PATCH_H_
#define _CMSIS_OS_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "cmsis_os.h"
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




typedef struct {
    uint32_t    pmp_buf_sz;    /* 0 as no fail case */
    void        *pmp_buf;
    uint32_t    pmp_instance_id;
    uint32_t    vmp_buf_sz;    /* 0 as no fail case */
    void        *vmp_buf;
    uint32_t    vmp_instance_id;
    void        *unknown_buf;   /* Cannot find correct MagicNum */
} osMemPoolFailLog_t;

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

void osPatchInit(void);


#ifdef __cplusplus
}
#endif
#endif  /* _CMSIS_OS_PATCH_H_ */
