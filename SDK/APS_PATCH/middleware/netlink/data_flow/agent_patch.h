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
 *  @file agent_patch.h
 * 
 *  @brief 
 *  
 ******************************************************************************/

#ifndef __AGENT_PATCH_H__
#define __AGENT_PATCH_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "agent.h"
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
typedef enum
{
    AGENT_MSG_M0_PATCH = 0,
    AGENT_MSG_UART1_PATCH,

    AGENT_MSG_MAX_PATCH
} T_AgentMsgTypePatch;

typedef struct
{
    uint32_t u32Type;   // T_AgentMsgTypePatch
    uint32_t u32ParamLen;
    uint8_t *pu8Param;
} T_AgentMsgPatch;

typedef struct
{
    uint32_t u32Type;   // T_AgentMsgTypePatch
    uint32_t u32ParamLen;
    uint8_t pu8Param[];
} T_AgentMsgInt;

typedef int (*T_AgentDataHandleFp)(uint32_t u32Type, uint8_t *u8aData, uint32_t u32DataLen, void *pParam);
typedef void (*T_AgentDataHandleRegFp)(T_AgentDataHandleFp fpHandle, void *pParam);
/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */
extern T_AgentDataHandleRegFp agent_data_handle_reg;
/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */
void agent_pre_init_patch(void);

#ifdef __cplusplus
}
#endif
#endif /* __AGENT_PATCH_H__ */
