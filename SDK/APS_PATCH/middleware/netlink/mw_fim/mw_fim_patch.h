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
 *  @file mw_fim_patch.h
 * 
 *  @brief 
 *  
 ******************************************************************************/

#ifndef _MW_FIM_PATCH_H_
#define _MW_FIM_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "mw_fim.h"
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
// file data (12 bytes)
typedef struct
{
    uint32_t ulFileId;
    uint16_t uwRecIdx;
    uint16_t uwFileSize;
    uint8_t *pubFileData;
} T_MwFimFileData;


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
void MwFim_PreInit_patch(void);

#ifdef __cplusplus
}
#endif
#endif  /* _MW_FIM_PATCH_H_ */
