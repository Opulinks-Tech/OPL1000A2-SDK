/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

/******************************************************************************
*  Filename:
*  ---------
*  mw_ota.c
*
*  Project:
*  --------
*  OPL1000 Project - the Over The Air (OTA) implement file
*
*  Description:
*  ------------
*  This implement file is include the Over The Air (OTA) function and api.
*
*  Author:
*  -------
*  Jeff Kuo
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include <stdlib.h>
#include <string.h>
#include "mw_ota.h"
#include "boot_sequence.h"
#include "hal_flash.h"
#include "hal_flash_internal.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define MW_OTA_DATA_BUFFER_SIZE     256


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
RET_DATA T_MwOtaLayoutInfo g_tMwOtaLayoutInfo;          // the layout information
RET_DATA uint8_t g_ubMwOtaBoot;                         // is the 2nd boot loader or not

RET_DATA int8_t g_bMwOtaImageIdx;                       // 0, 1, or MW_OTA_IDX_INVALID
RET_DATA T_MwOtaFlashHeader g_tMwOtaCurrentHeaderInfo;  // the current information of header

RET_DATA uint8_t g_ubMwOtaPrepareStatus;                // 0: not ready, 1: ready
RET_DATA T_MwOtaFlashHeader g_tMwOtaPrepareHeaderInfo;  // the prepare information of header
RET_DATA uint32_t g_ulMwOtaPrepareHeaderAddr;           // the start address of header
RET_DATA uint32_t g_ulMwOtaPrepareImageAddr;            // the start address of image
RET_DATA uint32_t g_ulMwOtaPrepareWriteAddr;            // write the image from the address
RET_DATA uint32_t g_ulMwOtaPrepareWriteSize;            // the size of the written data

// the function pointer of flash
RET_DATA T_Hal_Flash_4KSectorAddrErase g_tMwOtaEraseFunc;
RET_DATA T_Hal_Flash_AddrProgram g_tMwOtaWriteFunc;
RET_DATA T_Hal_Flash_AddrRead g_tMwOtaReadFunc;


// Sec 5: declaration of global function prototype
RET_DATA T_MwOta_Init_Fp MwOta_Init;
RET_DATA T_MwOta_VersionGet_Fp MwOta_VersionGet;
RET_DATA T_MwOta_Prepare_Fp MwOta_Prepare;
RET_DATA T_MwOta_DataIn_Fp MwOta_DataIn;
RET_DATA T_MwOta_DataFinish_Fp MwOta_DataFinish;
RET_DATA T_MwOta_DataGiveUp_Fp MwOta_DataGiveUp;

RET_DATA T_MwOta_CurrentIdxFake_Fp MwOta_CurrentIdxFake;            // for upgrade in the 2nd boot loader
RET_DATA T_MwOta_BootAddrGet_Fp MwOta_BootAddrGet;                  // for load image from flash in the 2nd boot loader

// internal part
RET_DATA T_MwOta_HeaderSeqNumGet_Fp MwOta_HeaderSeqNumGet;
RET_DATA T_MwOta_ImageCheckSumLocal_Fp MwOta_ImageCheckSumLocal;    // use the local buffer
RET_DATA T_MwOta_ImageCheckSumAlloc_Fp MwOta_ImageCheckSumAlloc;    // use the alloc buffer
RET_DATA T_MwOta_ImageCheckSumCompute_Fp MwOta_ImageCheckSumCompute;


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


/***********
C Functions
***********/
// Sec 8: C Functions

/*************************************************************************
* FUNCTION:
*   MwOta_Init
*
* DESCRIPTION:
*   OTA init
*
* PARAMETERS
*   1. ptInfo : [In] the layout information
*   2. ubBoot : [In] is 2nd boot loader or not
*
* RETURNS
*   MW_OTA_OK   : successful
*   MW_OTA_FAIL : fail
*
*************************************************************************/
uint8_t MwOta_Init_impl(T_MwOtaLayoutInfo *ptInfo, uint8_t ubBoot)
{
    T_MwOtaFlashHeader taHeader[2];
    uint16_t uwaSeqNum[2];
    
    // store "is the 2nd boot loader or not"
    g_ubMwOtaBoot = ubBoot;
    
    // for cold boot only
    if (0 != Boot_CheckWarmBoot())
        return MW_OTA_FAIL;
    
    // before init
    g_bMwOtaImageIdx = MW_OTA_IDX_INVALID;
    
    // check the flash state
    if (SUPPORTED_FLASH != Hal_Flash_Check(SPI_IDX_0))
    {
        printf("To check the flash for MW_OTA is fail.\n");
        return MW_OTA_FAIL;
    }
    
    // store the layout information
    memcpy(&g_tMwOtaLayoutInfo, ptInfo, sizeof(T_MwOtaLayoutInfo));
    
    // store the function pointer of flash
    if (g_ubMwOtaBoot == 1)
    {
        g_tMwOtaEraseFunc = Hal_Flash_4KSectorAddrErase_Internal;
        g_tMwOtaWriteFunc = Hal_Flash_AddrProgram_Internal;
        g_tMwOtaReadFunc = Hal_Flash_AddrRead_Internal;
    }
    else
    {
        g_tMwOtaEraseFunc = Hal_Flash_4KSectorAddrErase;
        g_tMwOtaWriteFunc = Hal_Flash_AddrProgram;
        g_tMwOtaReadFunc = Hal_Flash_AddrRead;
    }
    
    // check which is the latest image
    // read the header information
    if (0 != g_tMwOtaReadFunc(SPI_IDX_0, g_tMwOtaLayoutInfo.ulaHeaderAddr[0], 0, sizeof(T_MwOtaFlashHeader), (uint8_t*)&taHeader[0]))
    {
        printf("To read the 1st header of MW_OTA is fail.\n");
        return MW_OTA_FAIL;
    }
    if (0 != g_tMwOtaReadFunc(SPI_IDX_0, g_tMwOtaLayoutInfo.ulaHeaderAddr[1], 0, sizeof(T_MwOtaFlashHeader), (uint8_t*)&taHeader[1]))
    {
        printf("To read the 2nd header of MW_OTA is fail.\n");
        return MW_OTA_FAIL;
    }
    // compare the sequence number
    uwaSeqNum[0] = MwOta_HeaderSeqNumGet(&taHeader[0]);
    uwaSeqNum[1] = MwOta_HeaderSeqNumGet(&taHeader[1]);
    if ((uwaSeqNum[0] == MW_OTA_SEQ_INVALID) && (uwaSeqNum[1] == MW_OTA_SEQ_INVALID))
    {
        // give the init sequence number, it will be used later.
        if (g_ubMwOtaBoot == 1)
            g_tMwOtaCurrentHeaderInfo.uwSeqNum = MW_OTA_SEQ_END;
        
        printf("There is no any OTA image.\n");
        if (g_ubMwOtaBoot == 1)
            return MW_OTA_OK;
        else
            return MW_OTA_FAIL;
    }
    else if (uwaSeqNum[0] == MW_OTA_SEQ_INVALID)
    {
        g_bMwOtaImageIdx = 1;
    }
    else if (uwaSeqNum[1] == MW_OTA_SEQ_INVALID)
    {
        g_bMwOtaImageIdx = 0;
    }
    else
    {
        if (uwaSeqNum[0] >= uwaSeqNum[1])
        {
            g_bMwOtaImageIdx = 0;
        }
        else if ((uwaSeqNum[0] == MW_OTA_SEQ_START) && (uwaSeqNum[1] == MW_OTA_SEQ_END))
        {
            g_bMwOtaImageIdx = 0;
        }
        else
        {
            g_bMwOtaImageIdx = 1;
        }
    }
    
    // copy the header information
    memcpy(&g_tMwOtaCurrentHeaderInfo, &taHeader[g_bMwOtaImageIdx], sizeof(T_MwOtaFlashHeader));
    
    // the prepare status
    g_ubMwOtaPrepareStatus = MW_OTA_PREPARE_NONE;
    
    printf("This image is from MW_OTA [%u].\n", g_bMwOtaImageIdx);
    return MW_OTA_OK;
}

/*************************************************************************
* FUNCTION:
*   MwOta_VersionGet
*
* DESCRIPTION:
*   get the version information
*
* PARAMETERS
*   1. puwProjectId  : [Out] the pointer of project ID
*   2. puwChipId     : [Out] the pointer of chip ID
*   3. puwFirmwareId : [Out] the pointer of firmware ID
*
* RETURNS
*   MW_OTA_OK   : successful
*   MW_OTA_FAIL : fail
*
*************************************************************************/
uint8_t MwOta_VersionGet_impl(uint16_t *puwProjectId, uint16_t *puwChipId, uint16_t *puwFirmwareId)
{
    // error check
    if (g_bMwOtaImageIdx == MW_OTA_IDX_INVALID)
        return MW_OTA_FAIL;
    
    *puwProjectId = g_tMwOtaCurrentHeaderInfo.uwProjectId;
    *puwChipId = g_tMwOtaCurrentHeaderInfo.uwChipId;
    *puwFirmwareId = g_tMwOtaCurrentHeaderInfo.uwFirmwareId;
    
    return MW_OTA_OK;
}

/*************************************************************************
* FUNCTION:
*   MwOta_Prepare
*
* DESCRIPTION:
*   set the prepare version and image size
*
* PARAMETERS
*   1. uwProjectId  : [In] the project ID
*   2. uwChipId     : [In] the chip ID
*   3. uwFirmwareId : [In] the firmware ID
*   4. ulImageSize  : [In] the image size
*   5. ulImageSum   : [In] the check sum of image
*
* RETURNS
*   MW_OTA_OK   : successful
*   MW_OTA_FAIL : fail
*
*************************************************************************/
uint8_t MwOta_Prepare_impl(uint16_t uwProjectId, uint16_t uwChipId, uint16_t uwFirmwareId, uint32_t ulImageSize, uint32_t ulImageSum)
{
    uint32_t ulHeaderSum;
    uint32_t ulSize;
    uint8_t *pubData;
    
    uint32_t i;
    
    // error check
    if (g_bMwOtaImageIdx == MW_OTA_IDX_INVALID)
        return MW_OTA_FAIL;
    
    // check thee image size
    if (ulImageSize > g_tMwOtaLayoutInfo.ulImageSize)
        return MW_OTA_FAIL;
    
    // prepare the information of OTA header
    g_tMwOtaPrepareHeaderInfo.ulSignature = MW_OTA_SIGNATURE;
    g_tMwOtaPrepareHeaderInfo.uwProjectId = uwProjectId;
    g_tMwOtaPrepareHeaderInfo.uwChipId = uwChipId;
    g_tMwOtaPrepareHeaderInfo.uwFirmwareId = uwFirmwareId;
    g_tMwOtaPrepareHeaderInfo.ulImageSize = ulImageSize;
    g_tMwOtaPrepareHeaderInfo.ulImageSum = ulImageSum;

    // update the sequence number
    if (g_tMwOtaCurrentHeaderInfo.uwSeqNum == MW_OTA_SEQ_END)
        g_tMwOtaPrepareHeaderInfo.uwSeqNum = MW_OTA_SEQ_START;
    else
        g_tMwOtaPrepareHeaderInfo.uwSeqNum = g_tMwOtaCurrentHeaderInfo.uwSeqNum + 1;

    // compute the check sum
    ulHeaderSum = 0;
    ulSize = sizeof(T_MwOtaFlashHeader) - 4;    // without the size of check sum
    pubData = (uint8_t*)&g_tMwOtaPrepareHeaderInfo;
    for (i=0; i<ulSize; i++)
    {
        ulHeaderSum += pubData[i];
    }
    g_tMwOtaPrepareHeaderInfo.ulHeaderSum = ulHeaderSum;
    
    // the prepare address of header and image
    if (g_bMwOtaImageIdx == 0)
    {
        g_ulMwOtaPrepareHeaderAddr = g_tMwOtaLayoutInfo.ulaHeaderAddr[1];
        g_ulMwOtaPrepareImageAddr = g_tMwOtaLayoutInfo.ulaImageAddr[1];
        g_ulMwOtaPrepareWriteAddr = g_tMwOtaLayoutInfo.ulaImageAddr[1];
    }
    else
    {
        g_ulMwOtaPrepareHeaderAddr = g_tMwOtaLayoutInfo.ulaHeaderAddr[0];
        g_ulMwOtaPrepareImageAddr = g_tMwOtaLayoutInfo.ulaImageAddr[0];
        g_ulMwOtaPrepareWriteAddr = g_tMwOtaLayoutInfo.ulaImageAddr[0];
    }
    // the size of the written data
    g_ulMwOtaPrepareWriteSize = 0;
    
    // erase the flash sector
    // header
    if (0 != g_tMwOtaEraseFunc(SPI_IDX_0, g_ulMwOtaPrepareHeaderAddr))
    {
        printf("To erase the header [%u] of MW_OTA is fail.\n", g_ulMwOtaPrepareHeaderAddr);
        return MW_OTA_FAIL;
    }
    // image
    for (i=0; i<g_tMwOtaLayoutInfo.ulImageSize; i+= 0x1000)
    {
        if (0 != g_tMwOtaEraseFunc(SPI_IDX_0, g_ulMwOtaPrepareImageAddr + i))
        {
            printf("To erase the image [%u] of MW_OTA is fail.\n", g_ulMwOtaPrepareImageAddr + i);
            return MW_OTA_FAIL;
        }
    }
    
    // update the prepare status 
    g_ubMwOtaPrepareStatus = MW_OTA_PREPARE_READY;
    
    return MW_OTA_OK;
}

/*************************************************************************
* FUNCTION:
*   MwOta_DataIn
*
* DESCRIPTION:
*   write the image data into the flash
*
* PARAMETERS
*   1. pubAddr  : [In] the pointer of data buffer
*   2. ulSize   : [In] the buffer size
*
* RETURNS
*   MW_OTA_OK   : successful
*   MW_OTA_FAIL : fail
*
*************************************************************************/
uint8_t MwOta_DataIn_impl(uint8_t *pubAddr, uint32_t ulSize)
{
    // error check
    if (g_bMwOtaImageIdx == MW_OTA_IDX_INVALID)
        return MW_OTA_FAIL;
    
    // error check
    if (g_ubMwOtaPrepareStatus != MW_OTA_PREPARE_READY)
        return MW_OTA_FAIL;
    
    // check the image size
    if ((g_ulMwOtaPrepareWriteSize + ulSize) > g_tMwOtaPrepareHeaderInfo.ulImageSize)
        return MW_OTA_FAIL;
    
    // write the image data
    if (0 != g_tMwOtaWriteFunc(SPI_IDX_0, g_ulMwOtaPrepareWriteAddr, 0, ulSize, pubAddr))
    {
        printf("To write the image [%u] of MW_OTA is fail.\n", g_ulMwOtaPrepareWriteAddr);
        return MW_OTA_FAIL;
    }
    
    // update the information
    g_ulMwOtaPrepareWriteAddr += ulSize;
    g_ulMwOtaPrepareWriteSize += ulSize;
    
    return MW_OTA_OK;
}

/*************************************************************************
* FUNCTION:
*   MwOta_DataFinish
*
* DESCRIPTION:
*   finish the image upgrade
*
* PARAMETERS
*   none
*
* RETURNS
*   MW_OTA_OK   : successful
*   MW_OTA_FAIL : fail
*
*************************************************************************/
uint8_t MwOta_DataFinish_impl(void)
{
    uint32_t ulCheckSum;

    // error check
    if (g_bMwOtaImageIdx == MW_OTA_IDX_INVALID)
        return MW_OTA_FAIL;
    
    // error check
    if (g_ubMwOtaPrepareStatus != MW_OTA_PREPARE_READY)
        return MW_OTA_FAIL;
    
    // check the image size
    if (g_ulMwOtaPrepareWriteSize != g_tMwOtaPrepareHeaderInfo.ulImageSize)
        return MW_OTA_FAIL;
    
    // read the image data and compute the check sum
    if (g_ubMwOtaBoot == 1)
        ulCheckSum = MwOta_ImageCheckSumLocal();
    else
        ulCheckSum = MwOta_ImageCheckSumAlloc();
    
    // compare the 32 bits check sum
    if (ulCheckSum != g_tMwOtaPrepareHeaderInfo.ulImageSum)
        return MW_OTA_FAIL;
    
    // write the header information
    if (0 != g_tMwOtaWriteFunc(SPI_IDX_0, g_ulMwOtaPrepareHeaderAddr, 0, sizeof(T_MwOtaFlashHeader), (uint8_t*)&g_tMwOtaPrepareHeaderInfo))
    {
        printf("To write the header [%u] of MW_OTA is fail.\n", g_ulMwOtaPrepareHeaderAddr);
        return MW_OTA_FAIL;
    }
    
    return MW_OTA_OK;
}

/*************************************************************************
* FUNCTION:
*   MwOta_DataGiveUp
*
* DESCRIPTION:
*   give up the image upgrade
*
* PARAMETERS
*   none
*
* RETURNS
*   MW_OTA_OK   : successful
*   MW_OTA_FAIL : fail
*
*************************************************************************/
uint8_t MwOta_DataGiveUp_impl(void)
{
    // error check
    if (g_bMwOtaImageIdx == MW_OTA_IDX_INVALID)
        return MW_OTA_FAIL;
    
    // error check
    if (g_ubMwOtaPrepareStatus != MW_OTA_PREPARE_READY)
        return MW_OTA_FAIL;
    
    g_ubMwOtaPrepareStatus = MW_OTA_PREPARE_NONE;
    
    return MW_OTA_OK;
}

/*************************************************************************
* FUNCTION:
*   MwOta_CurrentIdxFake
*
* DESCRIPTION:
*   fake the current image index for boot only
*
* PARAMETERS
*   none
*
* RETURNS
*   MW_OTA_OK   : successful
*   MW_OTA_FAIL : fail
*
*************************************************************************/
uint8_t MwOta_CurrentIdxFake_impl(void)
{
    // for boot only
    if (g_ubMwOtaBoot != 1)
        return MW_OTA_FAIL;
    
    // fake the current image index
    // if the original index is invalid, set to the 2nd image
    if (g_bMwOtaImageIdx == MW_OTA_IDX_INVALID)
        g_bMwOtaImageIdx = 1;
    
    return MW_OTA_OK;
}

/*************************************************************************
* FUNCTION:
*   MwOta_BootAddrGet
*
* DESCRIPTION:
*   get the start address to load the image from flash
*
* PARAMETERS
*   1. pulImageAddr : [Out] the start address
*
* RETURNS
*   MW_OTA_OK   : successful
*   MW_OTA_FAIL : fail
*
*************************************************************************/
uint8_t MwOta_BootAddrGet_impl(uint32_t *pulImageAddr)
{
    T_MwOtaFlashHeader taHeader[2];
    uint16_t uwaSeqNum[2];
    
    // check which is the latest image
    // read the header information
    if (0 != g_tMwOtaReadFunc(SPI_IDX_0, g_tMwOtaLayoutInfo.ulaHeaderAddr[0], 0, sizeof(T_MwOtaFlashHeader), (uint8_t*)&taHeader[0]))
    {
        printf("To read the 1st header of MW_OTA is fail.\n");
        return MW_OTA_FAIL;
    }
    if (0 != g_tMwOtaReadFunc(SPI_IDX_0, g_tMwOtaLayoutInfo.ulaHeaderAddr[1], 0, sizeof(T_MwOtaFlashHeader), (uint8_t*)&taHeader[1]))
    {
        printf("To read the 2nd header of MW_OTA is fail.\n");
        return MW_OTA_FAIL;
    }
    // compare the sequence number
    uwaSeqNum[0] = MwOta_HeaderSeqNumGet(&taHeader[0]);
    uwaSeqNum[1] = MwOta_HeaderSeqNumGet(&taHeader[1]);
    if ((uwaSeqNum[0] == MW_OTA_SEQ_INVALID) && (uwaSeqNum[1] == MW_OTA_SEQ_INVALID))
    {
        printf("There is no any OTA image.\n");
        return MW_OTA_FAIL;
    }
    else if (uwaSeqNum[0] == MW_OTA_SEQ_INVALID)
    {
        *pulImageAddr = g_tMwOtaLayoutInfo.ulaImageAddr[1];
    }
    else if (uwaSeqNum[1] == MW_OTA_SEQ_INVALID)
    {
        *pulImageAddr = g_tMwOtaLayoutInfo.ulaImageAddr[0];
    }
    else
    {
        if (uwaSeqNum[0] >= uwaSeqNum[1])
        {
            *pulImageAddr = g_tMwOtaLayoutInfo.ulaImageAddr[0];
        }
        else if ((uwaSeqNum[0] == MW_OTA_SEQ_START) && (uwaSeqNum[1] == MW_OTA_SEQ_END))
        {
            *pulImageAddr = g_tMwOtaLayoutInfo.ulaImageAddr[0];
        }
        else
        {
            *pulImageAddr = g_tMwOtaLayoutInfo.ulaImageAddr[1];
        }
    }
    
    return MW_OTA_OK;
}

/*************************************************************************
* FUNCTION:
*   MwOta_HeaderSeqNumGet
*
* DESCRIPTION:
*   get the sequence number
*
* PARAMETERS
*   1. ptHeader : [In] the pointer of header
*
* RETURNS
*   MW_OTA_SEQ_INVALID : fail, it is an invalid header
*   n                  : successful, it is the sequence number
*
*************************************************************************/
uint16_t MwOta_HeaderSeqNumGet_impl(T_MwOtaFlashHeader *ptHeader)
{
    uint32_t ulHeaderSum;
    uint32_t ulSize;
    uint8_t *pubData;
    
    uint32_t i;
    
    // check the signature
    if (MW_OTA_SIGNATURE != ptHeader->ulSignature)
        return MW_OTA_SEQ_INVALID;
    
    // compute the check sum
    ulHeaderSum = 0;
    ulSize = sizeof(T_MwOtaFlashHeader) - 4;    // without the size of check sum
    pubData = (uint8_t*)ptHeader;
    for (i=0; i<ulSize; i++)
    {
        ulHeaderSum += pubData[i];
    }
    // check the check sum
    if (ulHeaderSum != ptHeader->ulHeaderSum)
        return MW_OTA_SEQ_INVALID;
    
    return ptHeader->uwSeqNum;
}

/*************************************************************************
* FUNCTION:
*   MwOta_ImageCheckSumLocal
*
* DESCRIPTION:
*   use the local buffer to compute the image check sum
*
* PARAMETERS
*   none
*
* RETURNS
*   check sum
*
*************************************************************************/
uint32_t MwOta_ImageCheckSumLocal_impl(void)
{
    uint8_t ubaData[MW_OTA_DATA_BUFFER_SIZE];
    
    return MwOta_ImageCheckSumCompute(ubaData);
}

/*************************************************************************
* FUNCTION:
*   MwOta_ImageCheckSumAlloc
*
* DESCRIPTION:
*   use the alloc buffer to compute the image check sum
*
* PARAMETERS
*   none
*
* RETURNS
*   0         : fail
*   check sum : successful
*
*************************************************************************/
uint32_t MwOta_ImageCheckSumAlloc_impl(void)
{
    uint32_t ulCheckSum = 0;
    uint8_t *pubData;
    
    // alloc buffer
    pubData = malloc(MW_OTA_DATA_BUFFER_SIZE);
    if (pubData == NULL)
    {
        printf("To alloc buffer [%u] of MW_OTA is fail.\n", MW_OTA_DATA_BUFFER_SIZE);
        goto done;
    }
    
    // compute the check sum
    ulCheckSum = MwOta_ImageCheckSumCompute(pubData);
    
    // release buffer
    free(pubData);

done:
    return ulCheckSum;
}

/*************************************************************************
* FUNCTION:
*   MwOta_ImageCheckSumCompute
*
* DESCRIPTION:
*   compute the image check sum
*
* PARAMETERS
*   1. ubaData : [In] the buffer
*
* RETURNS
*   0         : fail
*   check sum : successful
*
*************************************************************************/
uint32_t MwOta_ImageCheckSumCompute_impl(uint8_t ubaData[])
{
    uint32_t ulTotalSize;
    uint32_t ulOffset;
    uint32_t ulSize;
    uint32_t ulCheckSum;
    
    uint32_t i;
    
    ulTotalSize = g_ulMwOtaPrepareWriteSize;
    ulOffset = 0;
    ulCheckSum = 0;
    while (ulTotalSize)
    {
        if (ulTotalSize >= MW_OTA_DATA_BUFFER_SIZE)
            ulSize = MW_OTA_DATA_BUFFER_SIZE;
        else
            ulSize = ulTotalSize;
        
        if (0 != g_tMwOtaReadFunc(SPI_IDX_0, g_ulMwOtaPrepareImageAddr + ulOffset, 0, ulSize, ubaData))
        {
            printf("To read the image [%u] of MW_OTA is fail.\n", g_ulMwOtaPrepareImageAddr + ulOffset);
            return 0;
        }
        
        for (i=0; i<ulSize; i++)
        {
            ulCheckSum += ubaData[i];
        }
        
        ulOffset += ulSize;
        ulTotalSize -= ulSize;
    }
    
    return ulCheckSum;
}

/*************************************************************************
* FUNCTION:
*   MwOta_PreInitCold
*
* DESCRIPTION:
*   the previous initial for cold boot
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void MwOta_PreInitCold(void)
{
    MwOta_Init = MwOta_Init_impl;
    MwOta_VersionGet = MwOta_VersionGet_impl;
    MwOta_Prepare = MwOta_Prepare_impl;
    MwOta_DataIn = MwOta_DataIn_impl;
    MwOta_DataFinish = MwOta_DataFinish_impl;
    MwOta_DataGiveUp = MwOta_DataGiveUp_impl;
    
    MwOta_CurrentIdxFake = MwOta_CurrentIdxFake_impl;
    MwOta_BootAddrGet = MwOta_BootAddrGet_impl;
    
    MwOta_HeaderSeqNumGet = MwOta_HeaderSeqNumGet_impl;
    MwOta_ImageCheckSumLocal = MwOta_ImageCheckSumLocal_impl;
    MwOta_ImageCheckSumAlloc = MwOta_ImageCheckSumAlloc_impl;
    MwOta_ImageCheckSumCompute = MwOta_ImageCheckSumCompute_impl;
}
