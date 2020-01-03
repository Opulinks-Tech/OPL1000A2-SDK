/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include "iot_rb_data.h"

IoT_Ring_buffer_t g_tRBData;

void IoT_Ring_Buffer_Init(void)
{
    IoT_Ring_Buffer_ResetBuffer();
}

uint8_t IoT_Ring_Buffer_Push(IoT_Properity_t *ptProperity)
{
    uint32_t ulWriteNext;
    
    if (ptProperity == NULL)
        return IOT_RB_DATA_FAIL;

    // full, ulWriteIdx + 1 == ulReadIdx
    ulWriteNext = (g_tRBData.ulWriteIdx + 1) % IOT_RB_COUNT;
    
    // Read index alway prior to write index
    if (ulWriteNext == g_tRBData.ulReadIdx)
    {
        // discard the oldest data, and read index move forware one step.
        g_tRBData.ulReadIdx = (g_tRBData.ulReadIdx + 1) % IOT_RB_COUNT;
    }

    // update the temperature data to write index
    memcpy(&(g_tRBData.taProperity[g_tRBData.ulWriteIdx]), ptProperity, sizeof(IoT_Properity_t));
    g_tRBData.ulWriteIdx = ulWriteNext;

    return IOT_RB_DATA_OK;
}

uint8_t IoT_Ring_Buffer_Pop(IoT_Properity_t *ptProperity)
{
    uint8_t bRet = IOT_RB_DATA_FAIL;
    
    if (ptProperity == NULL)
        return IOT_RB_DATA_FAIL;

    // empty, ulWriteIdx == ulReadIdx
    if (g_tRBData.ulWriteIdx == g_tRBData.ulReadIdx)
        goto done;

    memcpy(ptProperity, &(g_tRBData.taProperity[g_tRBData.ulReadIdx]), sizeof(IoT_Properity_t));
    
    bRet = IOT_RB_DATA_OK;

done:
    return bRet;
}

uint8_t IoT_Ring_Buffer_CheckEmpty(void)
{
    // empty, ulWriteIdx == ulReadIdx
    if (g_tRBData.ulWriteIdx == g_tRBData.ulReadIdx)
        return IOT_RB_DATA_OK;
    
    return IOT_RB_DATA_FAIL;
}

void IoT_Ring_Buffer_ResetBuffer(void)
{
    g_tRBData.ulReadIdx = 0;
    g_tRBData.ulWriteIdx = 0;    
}

uint8_t IoT_Ring_Buffer_ReadIdxUpdate(void)
{
    g_tRBData.ulReadIdx = (g_tRBData.ulReadIdx + 1) % IOT_RB_COUNT;

    return IOT_RB_DATA_OK;
}

