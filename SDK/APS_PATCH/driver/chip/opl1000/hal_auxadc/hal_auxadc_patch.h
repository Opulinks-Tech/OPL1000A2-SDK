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
 *  @file hal_auxadc_patch.h
 * 
 *  @brief 
 *  
 ******************************************************************************/

#ifndef _HAL_AUXADC_PATCH_H_
#define _HAL_AUXADC_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "hal_auxadc.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
#define DEFAULT_AUXADC_AVG_CNT             30
#define DEFAULT_AUXADC_PU_EN_DELAY_US      0
#define DEFAULT_AUXADC_PU_EN_DELAY_US_MAX  5000 /* 5 ms */

/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */
// source type
typedef enum
{
    //HAL_AUX_SRC_GPIO = 0,
    //HAL_AUX_SRC_VBAT,
    //HAL_AUX_SRC_LDO_VCO,
    //HAL_AUX_SRC_LDO_RF,
    //HAL_AUX_SRC_TEMP_SEN,
    //HAL_AUX_SRC_HPBG_REF,
    //HAL_AUX_SRC_LPBG_REF,
    //HAL_AUX_SRC_PMU_SF,
    
    HAL_AUX_SRC_VSS = HAL_AUX_SRC_MAX, // New source
    HAL_AUX_SRC_MAX_PATCH
} E_HalAux_Src_Patch_t;

typedef enum
{
    HAL_AUX_CAL_PTS_L = 0,
    HAL_AUX_CAL_PTS_H,
    HAL_AUX_CAL_PTS_NUM
} E_HalAux_CalIdx_t;

typedef struct _sAuxadcCal
{
    uint16_t u16MiniVolt;
    uint16_t u16RawData;
}S_AuxadcCal_t;

typedef struct _sAuxadcCalTable
{
    uint32_t      u32Header;    // Reserved for future used
    S_AuxadcCal_t stIntSrc[ HAL_AUX_CAL_PTS_NUM ];
}S_AuxadcCalTable_t;

/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */
extern char *pAuxadcSrcName[ HAL_AUX_SRC_MAX_PATCH ];
extern S_AuxadcCalTable_t sAuxadcCalTable;

extern uint32_t g_ulHalAux_AverageCount;
extern uint8_t g_ubHalAux_AdcAlwaysOn;
extern uint32_t g_ulHalAux_PuEnDelay_Us;
/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */
/* API */
void Hal_Aux_AdcCal_Init( void );

uint32_t Hal_Aux_AdcCal_LoadDef( void );
uint32_t Hal_Aux_AdcCal_LoadOtp( void );
uint32_t Hal_Aux_AdcCal_LoadFlash( void );
uint32_t Hal_Aux_AdcCal_EraseFlash( void );
uint32_t Hal_Aux_AdcCal_StoreFlash( void );

// API changed, extra "uint8_t u8PtsIdx"
uint32_t Hal_Aux_AdcVbatInCal(uint16_t u16MiniVolt, uint8_t u8PtsIdx);
uint32_t Hal_Aux_AdcGpioInCal(uint8_t u8GpioIdx, uint16_t u16MiniVolt, uint8_t u8PtsIdx);

uint8_t Hal_Aux_AdcRawData_Get( E_HalAux_Src_Patch_t tSrc, uint8_t ubGpioIdx, uint32_t *pulRaw);
float Hal_Aux_AdcMiniVolt_Convert(uint32_t u32RawData);
uint8_t Hal_Aux_AdcMiniVolt_Get( E_HalAux_Src_Patch_t tSrc, uint8_t ubGpioIdx, float *pfVbat);
uint8_t Hal_Aux_AdcConvValue_Get( E_HalAux_Src_Patch_t tSrc, uint8_t ubGpioIdx, uint32_t *pulValue);
uint8_t Hal_Aux_AdcConvValue_Get_v2( E_HalAux_Src_Patch_t tSrc, uint8_t ubGpioIdx, uint32_t ulAvgCnt, uint32_t ulDelayUs, uint32_t *pulValue);

void Hal_Aux_PatchInit(void);

#ifdef __cplusplus
}
#endif
#endif  /* _HAL_AUXADC_PATCH_H_ */
