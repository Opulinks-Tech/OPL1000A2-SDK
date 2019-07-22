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
/**
 ****************************************************************************** 
 * @file opulinks.h
 * @author Opulinks Software Team
 * @version V0.0.1
 * @date 14-September-2018
 * @brief 
 */
#ifndef __OPULINKS_H
#define __OPULINKS_H


/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup Opulinks
  * @{
  */
 

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup Lib_cfg
  * @{
  */
/**
  * @brief CMSIS Device version number V5.0.2
  */
#define __OPULINKS_CMSIS_VER_MAIN   (0x05) /*!< [31:24] main version */
#define __OPULINKS_CMSIS_VER_SUB1   (0x00) /*!< [23:16] sub1 version */
#define __OPULINKS_CMSIS_VER_SUB2   (0x02) /*!< [15:8]  sub2 version */
#define __OPULINKS_CMSIS_VER_RC     (0x00) /*!< [7:0]  release candidate */ 
#define __OPULINKS_CMSIS_VER        ( (__OPULINKS_CMSIS_VER_MAIN << 24) |\
                                      (__OPULINKS_CMSIS_VER_SUB1 << 16) |\
                                      (__OPULINKS_CMSIS_VER_SUB2 << 8 ) |\
                                      (__OPULINKS_CMSIS_VER_RC))
/** @}
  * //Lib_cfg
  */

  
/** @addtogroup Device_Inc
  * @{
  */  
#if defined(OPL1000_APS) || defined(OPL1000_MSQ)
    #include "opl1000.h"
#elif defined(OPL2000_APS)
    #include "opl2000.h"
#elif defined(OPL2000_MSQ)
    #include "opl2000_msq.h"
#else
    #error "Wrong device definition"
#endif
/** @}
  * //Device_Inc
  */
  

/** @}
  * //Opulinks
  */

/** @}
  * //CMSIS
  */

  
#ifdef __cplusplus
}
#endif

#endif  /* __OPULINKS_H */
