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

#ifndef __IOT_RB_DATA_H__
#define __IOT_RB_DATA_H__

#include <stdint.h>
#include <stdbool.h>

#define IOT_RB_DATA_OK      0
#define IOT_RB_DATA_FAIL    1

#define IOT_RB_COUNT        (32)

typedef struct{
    uint8_t ubLightSwitch;
    uint8_t ubaReserved[3];
} IoT_Properity_t;

typedef struct
{
    uint32_t ulReadIdx;
    uint32_t ulWriteIdx;
    IoT_Properity_t taProperity[IOT_RB_COUNT];
} IoT_Ring_buffer_t;

void IoT_Ring_Buffer_Init(void);
uint8_t IoT_Ring_Buffer_Push(IoT_Properity_t *ptProperity);
uint8_t IoT_Ring_Buffer_Pop(IoT_Properity_t *ptProperity);
uint8_t IoT_Ring_Buffer_CheckEmpty(void);
void IoT_Ring_Buffer_ResetBuffer(void);
uint8_t IoT_Ring_Buffer_ReadIdxUpdate(void);

#endif // __IOT_RB_DATA_H__

