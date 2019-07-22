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

/******************************************************************************
*  Filename:
*  ---------
*  mw_fim.c
*
*  Project:
*  --------
*  OPL1000 Project - the Flash Item Management (FIM) implement file
*
*  Description:
*  ------------
*  This implement file is include the Flash Item Management (FIM) function and api.
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
#include <string.h>
#include "mw_fim.h"
#include "mw_fim_internal.h"
#include "mw_fim_default.h"
#include "hal_flash.h"
#include "cmsis_os.h"
#include "boot_sequence.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define MW_FIM_SIGNATURE_GROUP      0x67726F70  // grop
#define MW_FIM_SIGNATURE_FILE       0x46494C45  // FILE

#define MW_FIM_VER_GROUP_SEQUENCE_MIN       0x01
#define MW_FIM_VER_GROUP_SEQUENCE_MAX       0xFE
#define MW_FIM_VER_GROUP_SEQUENCE_OVER      0xFF

#define MW_FIM_DATA_SIZE_MIN        8   // used by check the offset of free space
#define MW_FIM_DATA_BUFFER_SIZE     64  // used for swap behavior

#define MW_FIM_SPACE_FULL           0x7FFFFFFF  // can't use 0xFFFFFFFF, it will be overflow in some case


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
RET_DATA T_MwFim_FlashLayoutUpdate_Fp MwFim_FlashLayoutUpdate;


// Sec 5: declaration of global function prototype
RET_DATA T_MwFim_Init_Fp MwFim_Init;
RET_DATA T_MwFim_FileRead_Fp MwFim_FileRead;
RET_DATA T_MwFim_FileWrite_Fp MwFim_FileWrite;
RET_DATA T_MwFim_FileWriteDefault_Fp MwFim_FileWriteDefault;
RET_DATA T_MwFim_FileDelete_Fp MwFim_FileDelete;

RET_DATA T_MwFim_ZoneInfoUpdate_Fp MwFim_ZoneInfoUpdate;
RET_DATA T_MwFim_GroupInfoUpdate_Fp MwFim_GroupInfoUpdate;
RET_DATA T_MwFim_GroupVersionUpdate_Fp MwFim_GroupVersionUpdate;


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable
RET_DATA uint8_t g_ubMwFimInit;
RET_DATA osSemaphoreId g_tMwFimSemaphoreId;
RET_DATA T_MwFimGroupStatus g_taMwFimGroupStatusTable[MW_FIM_ZONE_MAX][MW_FIM_GROUP_MAX];


// Sec 7: declaration of static function prototype
RET_DATA T_MwFim_FileWriteDo_Fp MwFim_FileWriteDo;
RET_DATA T_MwFim_GroupSizeCheck_Fp MwFim_GroupSizeCheck;
RET_DATA T_MwFim_GroupStatusFill_Fp MwFim_GroupStatusFill;
RET_DATA T_MwFim_GroupHeaderCheck_Fp MwFim_GroupHeaderCheck;
RET_DATA T_MwFim_GroupHeaderVerify_Fp MwFim_GroupHeaderVerify;
RET_DATA T_MwFim_GroupCheckSumVerify_Fp MwFim_GroupCheckSumVerify;
RET_DATA T_MwFim_GroupStatusRemap_Fp MwFim_GroupStatusRemap;
RET_DATA T_MwFim_EmptyBlockFind_Fp MwFim_EmptyBlockFind;
RET_DATA T_MwFim_GroupBlockParser_Fp MwFim_GroupBlockParser;
RET_DATA T_MwFim_FileHeaderVerify_Fp MwFim_FileHeaderVerify;
RET_DATA T_MwFim_FreeOffsetVerify_Fp MwFim_FreeOffsetVerify;
RET_DATA T_MwFim_FileTableSearch_Fp MwFim_FileTableSearch;
RET_DATA T_MwFim_GroupSwap_Fp MwFim_GroupSwap;
RET_DATA T_MwFim_FileDataDefaultFill_Fp MwFim_FileDataDefaultFill;


/***********
C Functions
***********/
// Sec 8: C Functions

/*************************************************************************
* FUNCTION:
*   MwFim_Init
*
* DESCRIPTION:
*   FIM init
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void MwFim_Init_impl(void)
{
    osSemaphoreDef_t tSemaphoreDef;

    // 0. check for cold boot only
    if (0 != Boot_CheckWarmBoot())
        return;
    
    // 1. before init
    g_ubMwFimInit = 0;
    
    // 2. check the flash state
    if (SUPPORTED_FLASH != Hal_Flash_Check(SPI_IDX_0))
    {
        printf("To check the flash for MW_FIM is fail.\n");
        return;
    }
    
    // 3. update the flash layout
    if (MwFim_FlashLayoutUpdate != NULL)
        MwFim_FlashLayoutUpdate();
    
    // 4. check the size of group is overflow or not
    MwFim_GroupSizeCheck();
    
    // 5. fill the group status
    MwFim_GroupStatusFill();
    
    // 6. parse the block to fill the free offset of group and the information of file
    MwFim_GroupBlockParser();
    
    // 7. fill the default value of file if the data is not exist
    MwFim_FileDataDefaultFill();
    
    // 8. create the semaphore
    tSemaphoreDef.dummy = 0;                            // reserved, it is no used
    g_tMwFimSemaphoreId = osSemaphoreCreate(&tSemaphoreDef, 1);
    if (g_tMwFimSemaphoreId == NULL)
    {
        printf("To create the semaphore for MW_FIM is fail.\n");
        return;
    }
    
    // 9. after init
    g_ubMwFimInit = 1;
    
    printf("The init of MW_FIM is done.\n");
}

/*************************************************************************
* FUNCTION:
*   MwFim_FileRead
*
* DESCRIPTION:
*   read the file data from flash
*
* PARAMETERS
*   1. ulFileId    : [In] the file ID
*   2. uwRecIdx    : [In] the index of record
*   3. uwFileSize  : [In] the data size of file
*   4. pubFileData : [Out] the pointer of file data
*
* RETURNS
*   1. MW_FIM_OK   : success
*   2. MW_FIM_FAIL : fail
*
*************************************************************************/
uint8_t MwFim_FileRead_impl(uint32_t ulFileId, uint16_t uwRecIdx, uint16_t uwFileSize, uint8_t *pubFileData)
{
    T_MwFimFileInfo *ptFileTable;
    uint8_t ubRet = MW_FIM_FAIL;
    
    // check init
    if (g_ubMwFimInit != 1)
        return ubRet;
    
    // wait the semaphore
    osSemaphoreWait(g_tMwFimSemaphoreId, osWaitForever);

    // search the file table by file ID
    if (MW_FIM_OK != MwFim_FileTableSearch(ulFileId, &ptFileTable))
        goto done;

    // check the index of record
    if (uwRecIdx >= ptFileTable->uwRecordMax)
        goto done;

    // check the size of file
    if (uwFileSize != ptFileTable->uwDataSize)
        goto done;
    
    // check the data is exist or not
    if (ptFileTable->pulDataAddr[uwRecIdx] == 0xFFFFFFFF)
        goto done;
    
    // read the file data from flash
    if (0 != Hal_Flash_AddrRead(SPI_IDX_0, ptFileTable->pulDataAddr[uwRecIdx], 0, uwFileSize, pubFileData))
    {
        //printf("FIM: #01\n");
        goto done;
    }

    ubRet = MW_FIM_OK;
    
done:
    // release the semaphore
    osSemaphoreRelease(g_tMwFimSemaphoreId);
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*   MwFim_FileWrite
*
* DESCRIPTION:
*   write the file data into flash
*
* PARAMETERS
*   1. ulFileId    : [In] the file ID
*   2. uwRecIdx    : [In] the index of record
*   3. uwFileSize  : [In] the data size of file
*   4. pubFileData : [In] the pointer of file data
*
* RETURNS
*   1. MW_FIM_OK   : success
*   2. MW_FIM_FAIL : fail
*
*************************************************************************/
uint8_t MwFim_FileWrite_impl(uint32_t ulFileId, uint16_t uwRecIdx, uint16_t uwFileSize, uint8_t *pubFileData)
{
    uint8_t ubRet = MW_FIM_FAIL;

    // check init
    if (g_ubMwFimInit != 1)
        return ubRet;
    
    // wait the semaphore
    osSemaphoreWait(g_tMwFimSemaphoreId, osWaitForever);
    
    // do the behavior
    if (MW_FIM_OK != MwFim_FileWriteDo(ulFileId, uwRecIdx, uwFileSize, pubFileData))
        goto done;
    
    ubRet = MW_FIM_OK;
    
done:
    // release the semaphore
    osSemaphoreRelease(g_tMwFimSemaphoreId);
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*   MwFim_FileWriteDefault
*
* DESCRIPTION:
*   write the default file data into flash
*
* PARAMETERS
*   1. ulFileId    : [In] the file ID
*   2. uwRecIdx    : [In] the index of record
*
* RETURNS
*   1. MW_FIM_OK   : success
*   2. MW_FIM_FAIL : fail
*
*************************************************************************/
uint8_t MwFim_FileWriteDefault_impl(uint32_t ulFileId, uint16_t uwRecIdx)
{
    T_MwFimFileInfo *ptFileTable;
    uint8_t ubRet = MW_FIM_FAIL;
    
    // check init
    if (g_ubMwFimInit != 1)
        return ubRet;
    
    // wait the semaphore
    osSemaphoreWait(g_tMwFimSemaphoreId, osWaitForever);
    
    // search the file table by file ID
    if (MW_FIM_OK != MwFim_FileTableSearch(ulFileId, &ptFileTable))
        goto done;
    
    // write the default value
    if (MW_FIM_OK != MwFim_FileWriteDo(ulFileId, uwRecIdx, ptFileTable->uwDataSize, ptFileTable->pubDefaultValue))
        goto done;
    
    ubRet = MW_FIM_OK;
    
done:
    // release the semaphore
    osSemaphoreRelease(g_tMwFimSemaphoreId);
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*   MwFim_FileDelete
*
* DESCRIPTION:
*   delete the file data
*
* PARAMETERS
*   1. ulFileId    : [In] the file ID
*   2. uwRecIdx    : [In] the index of record
*
* RETURNS
*   1. MW_FIM_OK   : success
*   2. MW_FIM_FAIL : fail
*
*************************************************************************/
uint8_t MwFim_FileDelete_impl(uint32_t ulFileId, uint16_t uwRecIdx)
{
    T_MwFimFileInfo *ptFileTable;
    uint8_t ubRet = MW_FIM_FAIL;
    
    uint32_t ulZoneIdx;
    uint32_t ulGroupIdx;
    
    // check init
    if (g_ubMwFimInit != 1)
        return ubRet;
    
    // wait the semaphore
    osSemaphoreWait(g_tMwFimSemaphoreId, osWaitForever);
    
    // search the file table by file ID
    if (MW_FIM_OK != MwFim_FileTableSearch(ulFileId, &ptFileTable))
        goto done;
    
    // check the index of record
    if (uwRecIdx >= ptFileTable->uwRecordMax)
        goto done;
    
    // get the zone and group index
    ulZoneIdx = (ulFileId >> 24) & 0xFF;
    ulGroupIdx = (ulFileId >> 16) & 0xFF;
    
    // reset the data address of record
    ptFileTable->pulDataAddr[uwRecIdx] = 0xFFFFFFFF;
        
    // do the swap behavior
    if (MW_FIM_OK != MwFim_GroupSwap(ulZoneIdx, ulGroupIdx))
        goto done;
    
    ubRet = MW_FIM_OK;
    
done:
    // release the semaphore
    osSemaphoreRelease(g_tMwFimSemaphoreId);
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*   MwFim_ZoneInfoUpdate
*
* DESCRIPTION:
*   update the zone info
*
* PARAMETERS
*   1. ulZoneIdx    : [In] the index of zone
*   2. ptZoneInfo   : [In] the pointer of zone info
*
* RETURNS
*   1. MW_FIM_OK   : success
*   2. MW_FIM_FAIL : fail
*
*************************************************************************/
uint8_t MwFim_ZoneInfoUpdate_impl(uint32_t ulZoneIdx, T_MwFimZoneInfo *ptZoneInfo)
{
    uint8_t ubRet = MW_FIM_FAIL;
    
    // check the data of zone info
    if (ptZoneInfo == NULL)
        goto done;
    
    // check the index of zone
    if (ulZoneIdx >= MW_FIM_ZONE_MAX)
        goto done;
    
    // update the data of zone info
    memcpy(&(g_taMwFimZoneInfoTable[ulZoneIdx]), ptZoneInfo, sizeof(T_MwFimZoneInfo));
    
    ubRet = MW_FIM_OK;
    
done:
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*   MwFim_GroupInfoUpdate
*
* DESCRIPTION:
*   update the group info
*
* PARAMETERS
*   1. ulZoneIdx    : [In] the index of zone
*   2. ulGroupIdx   : [In] the index of group
*   3. ptGroupInfo  : [In] the pointer of group info
*
* RETURNS
*   1. MW_FIM_OK   : success
*   2. MW_FIM_FAIL : fail
*
*************************************************************************/
uint8_t MwFim_GroupInfoUpdate_impl(uint32_t ulZoneIdx, uint32_t ulGroupIdx, T_MwFimFileInfo *ptGroupInfo)
{
    uint8_t ubRet = MW_FIM_FAIL;
    
    // check the data of group info
    if (ptGroupInfo == NULL)
        goto done;
    
    // check the index of zone
    if (ulZoneIdx >= MW_FIM_ZONE_MAX)
        goto done;
    
    // check the index of group
    if (ulGroupIdx >= MW_FIM_GROUP_MAX)
        goto done;
    
    // update the pointer of group info
    g_ptaMwFimGroupInfoTable[ulZoneIdx][ulGroupIdx] = ptGroupInfo;
    
    ubRet = MW_FIM_OK;
    
done:
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*   MwFim_GroupVersionUpdate
*
* DESCRIPTION:
*   update the group version
*
* PARAMETERS
*   1. ulZoneIdx    : [In] the index of zone
*   2. ulGroupIdx   : [In] the index of group
*   3. ubVersion    : [In] the version of group
*
* RETURNS
*   1. MW_FIM_OK   : success
*   2. MW_FIM_FAIL : fail
*
*************************************************************************/
uint8_t MwFim_GroupVersionUpdate_impl(uint32_t ulZoneIdx, uint32_t ulGroupIdx, uint8_t ubVersion)
{
    uint8_t ubRet = MW_FIM_FAIL;
    
    // check the index of zone
    if (ulZoneIdx >= MW_FIM_ZONE_MAX)
        goto done;
    
    // check the index of group
    if (ulGroupIdx >= MW_FIM_GROUP_MAX)
        goto done;
    
    // update the version of group
    g_taMwFimZoneInfoTable[ulZoneIdx].pubVerTable[ulGroupIdx] = ubVersion;

    ubRet = MW_FIM_OK;
    
done:
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*   MwFim_FileWriteDo
*
* DESCRIPTION:
*   do "write the file data into flash"
*
* PARAMETERS
*   1. ulFileId    : [In] the file ID
*   2. uwRecIdx    : [In] the index of record
*   3. uwFileSize  : [In] the data size of file
*   4. pubFileData : [In] the pointer of file data
*
* RETURNS
*   1. MW_FIM_OK   : success
*   2. MW_FIM_FAIL : fail
*
*************************************************************************/
uint8_t MwFim_FileWriteDo_impl(uint32_t ulFileId, uint16_t uwRecIdx, uint16_t uwFileSize, uint8_t *pubFileData)
{
    T_MwFimFileInfo *ptFileTable;
    uint8_t ubRet = MW_FIM_FAIL;

    T_MwFimFileHeader tFileHeader;
    uint32_t ulFreeAddr;
    
    uint32_t ulZoneIdx;
    uint32_t ulGroupIdx;
    
    // search the file table by file ID
    if (MW_FIM_OK != MwFim_FileTableSearch(ulFileId, &ptFileTable))
        goto done;

    // check the index of record
    if (uwRecIdx >= ptFileTable->uwRecordMax)
        goto done;

    // check the size of file
    if (uwFileSize != ptFileTable->uwDataSize)
        goto done;
    
    // get the zone and group index
    ulZoneIdx = (ulFileId >> 24) & 0xFF;
    ulGroupIdx = (ulFileId >> 16) & 0xFF;
    
    // check the size is enough or not
    // if not enough, do the swap behavior
    if ((g_taMwFimGroupStatusTable[ulZoneIdx][ulGroupIdx].ulFreeOffset + sizeof(T_MwFimFileHeader) + uwFileSize) > g_taMwFimZoneInfoTable[ulZoneIdx].ulBlockSize)
    {
        // reset the data address of record
        // disable it: if device was reset between MwFim_GroupSwap and Hal_Flash_AddrProgram, the record will be reset to default.
        //ptFileTable->pulDataAddr[uwRecIdx] = 0xFFFFFFFF;
        
        // do the swap behavior
        if (MW_FIM_OK != MwFim_GroupSwap(ulZoneIdx, ulGroupIdx))
            goto done;
    }
    
    // get the start address of block
    ulFreeAddr = g_taMwFimZoneInfoTable[ulZoneIdx].ulBaseAddr 
                 + g_taMwFimZoneInfoTable[ulZoneIdx].ulBlockSize * g_taMwFimGroupStatusTable[ulZoneIdx][ulGroupIdx].ubBlockIdx
                 + g_taMwFimGroupStatusTable[ulZoneIdx][ulGroupIdx].ulFreeOffset;

    // write the file data
    if (0 != Hal_Flash_AddrProgram(SPI_IDX_0, ulFreeAddr + sizeof(T_MwFimFileHeader), 0, uwFileSize, pubFileData))
    {
        //printf("FIM: #02\n");
        goto done;
    }

    // fill the file header
    tFileHeader.ulSignature = MW_FIM_SIGNATURE_FILE;
    tFileHeader.ulFileId = ulFileId;
    tFileHeader.uwRecordIdx = uwRecIdx;
    tFileHeader.uwDataSize = uwFileSize;
    
    // write the file header
    if (0 != Hal_Flash_AddrProgram(SPI_IDX_0, ulFreeAddr, 0, sizeof(T_MwFimFileHeader), (uint8_t*)&tFileHeader))
    {
        //printf("FIM: #03\n");
        goto done;
    }
    
    // update the information of file
    ptFileTable->pulDataAddr[uwRecIdx] = ulFreeAddr + sizeof(T_MwFimFileHeader);
    // update the information of group
    g_taMwFimGroupStatusTable[ulZoneIdx][ulGroupIdx].ulFreeOffset = g_taMwFimGroupStatusTable[ulZoneIdx][ulGroupIdx].ulFreeOffset
                                                                    + sizeof(T_MwFimFileHeader)
                                                                    + uwFileSize;
    
    ubRet = MW_FIM_OK;
    
done:
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*   MwFim_GroupSizeCheck
*
* DESCRIPTION:
*   check the size of group is overflow or not
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void MwFim_GroupSizeCheck_impl(void)
{
    T_MwFimFileInfo *ptFileTable;
    uint32_t ulSize;
    
    uint32_t ulPrefix;
    
    uint32_t ulZoneIdx;
    uint32_t ulGroupIdx;
    uint32_t i;
    
    for (ulZoneIdx=0; ulZoneIdx<MW_FIM_ZONE_MAX; ulZoneIdx++)
    {
        for (ulGroupIdx=0; ulGroupIdx<g_taMwFimZoneInfoTable[ulZoneIdx].ulBlockNum; ulGroupIdx++)
        {
            // compute the size of group
            ptFileTable = g_ptaMwFimGroupInfoTable[ulZoneIdx][ulGroupIdx];
            i = 0;
            ulSize = sizeof(T_MwFimGroupHeader);
            ulPrefix = ((ulZoneIdx & 0xFF) << 24) + ((ulGroupIdx & 0xFF) << 16);
            while ((ptFileTable[i].ulFileId & 0xFFFF0000) == ulPrefix)
            {
                ulSize = ulSize + (sizeof(T_MwFimFileHeader) + ptFileTable[i].uwDataSize) * ptFileTable[i].uwRecordMax;
                
                i++;
                // error handle for the end element is exist or not
                if (i >= 0x10000)
                {
                    printf("FIM: the end element of Zone[%u] Group[%u] is not exist.\n", ulZoneIdx, ulGroupIdx);
                    printf("The system is stuck now.\n");
                    while(1);
                }
            }
            
            // check the size of group is overflow or not
            if (ulSize > g_taMwFimZoneInfoTable[ulZoneIdx].ulBlockSize)
            {
                printf("FIM: the size of Zone[%u] Group[%u] is overflow.\n", ulZoneIdx, ulGroupIdx);
                printf("The system is stuck now.\n");
                while(1);
            }
        } // for (ulGroupIdx=0; ulGroupIdx<MW_FIM_GROUP_MAX; ulGroupIdx++)
    } // for (ulZoneIdx=0; ulZoneIdx<MW_FIM_ZONE_MAX; ulZoneIdx++)
}

/*************************************************************************
* FUNCTION:
*   MwFim_GroupStatusFill
*
* DESCRIPTION:
*   fill the group status
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void MwFim_GroupStatusFill_impl(void)
{
    uint8_t ubaBlockUsed[MW_FIM_GROUP_MAX];     // the block is used by which group
    uint32_t ulZoneIdx;
    
    // 0. reset the group status
    memset(g_taMwFimGroupStatusTable, 0xFF, sizeof(T_MwFimGroupStatus) * MW_FIM_ZONE_MAX * MW_FIM_GROUP_MAX);

    for (ulZoneIdx=0; ulZoneIdx<MW_FIM_ZONE_MAX; ulZoneIdx++)
    {
        // check the block number if the zone is empty
        if (0 == g_taMwFimZoneInfoTable[ulZoneIdx].ulBlockNum)
            continue;
        
        // 1. read the current status from flash
        MwFim_GroupHeaderCheck(ulZoneIdx, ubaBlockUsed);

        // 2. remap the block index if the group is not in flash
        MwFim_GroupStatusRemap(ulZoneIdx, ubaBlockUsed);
    }
}

/*************************************************************************
* FUNCTION:
*   MwFim_GroupHeaderCheck
*
* DESCRIPTION:
*   check the group header
*
* PARAMETERS
*   1. ulZoneIdx    : [In] the zone index
*   2. ubaBlockUsed : [Out] the usage of each block
*
* RETURNS
*   none
*
*************************************************************************/
void MwFim_GroupHeaderCheck_impl(uint32_t ulZoneIdx, uint8_t ubaBlockUsed[MW_FIM_GROUP_MAX])
{
    T_MwFimGroupHeader tGroupHeader;
    uint32_t ulBlockAddr;

    uint32_t ulBlockIdx;
    uint32_t ulOldIdx;
    
    // 1. reset the block used status
    memset(ubaBlockUsed, 0xFF, sizeof(uint8_t) * MW_FIM_GROUP_MAX);

    // 2. read the information from each block
    for (ulBlockIdx=0; ulBlockIdx<g_taMwFimZoneInfoTable[ulZoneIdx].ulBlockNum; ulBlockIdx++)
    {
        // read the group header
        ulBlockAddr = g_taMwFimZoneInfoTable[ulZoneIdx].ulBaseAddr + g_taMwFimZoneInfoTable[ulZoneIdx].ulBlockSize * ulBlockIdx;
        if (0 != Hal_Flash_AddrRead(SPI_IDX_0, ulBlockAddr, 0, sizeof(T_MwFimGroupHeader), (uint8_t*)&tGroupHeader))
        {
            //printf("FIM: #04\n");
            continue;
        }
        
        // verify the header information of group
        if (MW_FIM_OK != MwFim_GroupHeaderVerify(ulZoneIdx, &tGroupHeader))
        {
            // erase the block
            if (0 != Hal_Flash_4KSectorAddrErase(SPI_IDX_0, ulBlockAddr))
            {
                //printf("FIM: #05\n");
            }
            continue;
        }

        // it is never used
        if (g_taMwFimGroupStatusTable[ulZoneIdx][tGroupHeader.ubGroupIdx].ubBlockIdx == 0xFF)
        {
            ubaBlockUsed[ulBlockIdx] = tGroupHeader.ubGroupIdx;
            g_taMwFimGroupStatusTable[ulZoneIdx][tGroupHeader.ubGroupIdx].ubMajorVersion = tGroupHeader.ubMajorVersion;
            g_taMwFimGroupStatusTable[ulZoneIdx][tGroupHeader.ubGroupIdx].ubMinorVersion = tGroupHeader.ubMinorVersion;
            g_taMwFimGroupStatusTable[ulZoneIdx][tGroupHeader.ubGroupIdx].ubBlockIdx = ulBlockIdx;
        }
        // the group index is the same as the previous block 
        else
        {
            // compare the minor version
            // the special case 1, swap it
            if ((tGroupHeader.ubMinorVersion == MW_FIM_VER_GROUP_SEQUENCE_MIN)
                && (g_taMwFimGroupStatusTable[ulZoneIdx][tGroupHeader.ubGroupIdx].ubMinorVersion == MW_FIM_VER_GROUP_SEQUENCE_MAX))
            {
                // erase the old block
                ulOldIdx = g_taMwFimGroupStatusTable[ulZoneIdx][tGroupHeader.ubGroupIdx].ubBlockIdx;
                ubaBlockUsed[ulOldIdx] = 0xFF;
                if (0 != Hal_Flash_4KSectorAddrErase(SPI_IDX_0, g_taMwFimZoneInfoTable[ulZoneIdx].ulBaseAddr + g_taMwFimZoneInfoTable[ulZoneIdx].ulBlockSize * ulOldIdx))
                {
                    //printf("FIM: #06\n");
                }
                
                // update the new group status
                ubaBlockUsed[ulBlockIdx] = tGroupHeader.ubGroupIdx;
                g_taMwFimGroupStatusTable[ulZoneIdx][tGroupHeader.ubGroupIdx].ubMajorVersion = tGroupHeader.ubMajorVersion;
                g_taMwFimGroupStatusTable[ulZoneIdx][tGroupHeader.ubGroupIdx].ubMinorVersion = tGroupHeader.ubMinorVersion;
                g_taMwFimGroupStatusTable[ulZoneIdx][tGroupHeader.ubGroupIdx].ubBlockIdx = ulBlockIdx;
            }
            // the special case 2, erase the old version
            else if ((tGroupHeader.ubMinorVersion == MW_FIM_VER_GROUP_SEQUENCE_MAX)
                && (g_taMwFimGroupStatusTable[ulZoneIdx][tGroupHeader.ubGroupIdx].ubMinorVersion == MW_FIM_VER_GROUP_SEQUENCE_MIN))
            {
                // erase the block
                if (0 != Hal_Flash_4KSectorAddrErase(SPI_IDX_0, ulBlockAddr))
                {
                    //printf("FIM: #07\n");
                }
            }
            // the normal case, swap it
            else if (tGroupHeader.ubMinorVersion > g_taMwFimGroupStatusTable[ulZoneIdx][tGroupHeader.ubGroupIdx].ubMinorVersion)
            {
                // erase the old block
                ulOldIdx = g_taMwFimGroupStatusTable[ulZoneIdx][tGroupHeader.ubGroupIdx].ubBlockIdx;
                ubaBlockUsed[ulOldIdx] = 0xFF;
                if (0 != Hal_Flash_4KSectorAddrErase(SPI_IDX_0, g_taMwFimZoneInfoTable[ulZoneIdx].ulBaseAddr + g_taMwFimZoneInfoTable[ulZoneIdx].ulBlockSize * ulOldIdx))
                {
                    //printf("FIM: #08\n");
                }
                
                // update the new group status
                ubaBlockUsed[ulBlockIdx] = tGroupHeader.ubGroupIdx;
                g_taMwFimGroupStatusTable[ulZoneIdx][tGroupHeader.ubGroupIdx].ubMajorVersion = tGroupHeader.ubMajorVersion;
                g_taMwFimGroupStatusTable[ulZoneIdx][tGroupHeader.ubGroupIdx].ubMinorVersion = tGroupHeader.ubMinorVersion;
                g_taMwFimGroupStatusTable[ulZoneIdx][tGroupHeader.ubGroupIdx].ubBlockIdx = ulBlockIdx;
            }
            // the others, erase the old version
            else
            {
                // erase the block
                if (0 != Hal_Flash_4KSectorAddrErase(SPI_IDX_0, ulBlockAddr))
                {
                    //printf("FIM: #09\n");
                }
            }
        }
    } // 2. read the information from each block
}

/*************************************************************************
* FUNCTION:
*   MwFim_GroupHeaderVerify
*
* DESCRIPTION:
*   verify the header information of group
*
* PARAMETERS
*   1. ulZoneIdx     : [In] the zone index
*   2. ptGroupHeader : [In] the group header
*
* RETURNS
*   1. MW_FIM_OK   : success
*   2. MW_FIM_FAIL : fail
*
*************************************************************************/
uint8_t MwFim_GroupHeaderVerify_impl(uint32_t ulZoneIdx, T_MwFimGroupHeader *ptGroupHeader)
{
    uint8_t ubRet = MW_FIM_FAIL;

    // check the signature
    if (ptGroupHeader->ulSignature != MW_FIM_SIGNATURE_GROUP)
        goto done;

    // verify the check sum
    if (MW_FIM_OK != MwFim_GroupCheckSumVerify(ptGroupHeader))
        goto done;

    // verify the major version
    if (ptGroupHeader->ubMajorVersion != g_taMwFimZoneInfoTable[ulZoneIdx].pubVerTable[ptGroupHeader->ubGroupIdx])
        goto done;
    
    ubRet = MW_FIM_OK;

done:    
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*   MwFim_GroupCheckSumVerify
*
* DESCRIPTION:
*   verify the check sum of group header
*
* PARAMETERS
*   1. ptGroupHeader : [In] the group header
*
* RETURNS
*   1. MW_FIM_OK   : success
*   2. MW_FIM_FAIL : fail
*
*************************************************************************/
uint8_t MwFim_GroupCheckSumVerify_impl(T_MwFimGroupHeader *ptGroupHeader)
{
    uint32_t ulCheckSum;
    uint8_t* pubData;
    uint8_t ubDataSize;
    uint8_t ubRet = MW_FIM_FAIL;
    
    uint32_t i;
    
    // 1. compute the check sum
    ulCheckSum = 0;
    pubData = (uint8_t*)ptGroupHeader;
    ubDataSize = sizeof(T_MwFimGroupHeader) - 4;        // 4 bytes check sum
    for (i=0; i<ubDataSize; i++)
    {
        ulCheckSum = ulCheckSum + pubData[i];
    }
    
    // 2. compare the check sum
    if (ulCheckSum == ptGroupHeader->ulCheckSum)
        ubRet = MW_FIM_OK;
    
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*   MwFim_GroupStatusRemap
*
* DESCRIPTION:
*   remap the block index if the group is not in flash
*
* PARAMETERS
*   1. ulZoneIdx    : [In] the zone index
*   2. ubaBlockUsed : [Out] the usage of each block
*
* RETURNS
*   none
*
*************************************************************************/
void MwFim_GroupStatusRemap_impl(uint32_t ulZoneIdx, uint8_t ubaBlockUsed[MW_FIM_GROUP_MAX])
{
    T_MwFimGroupHeader tGroupHeader;
    uint32_t ulEmptyIdx;
    uint32_t ulBlockAddr;
    uint8_t* pubData;
    uint8_t ubDataSize;
    
    uint32_t ulGroupIdx;
    uint32_t i;
    
    // remap the status for each group
    for (ulGroupIdx=0; ulGroupIdx<g_taMwFimZoneInfoTable[ulZoneIdx].ulBlockNum; ulGroupIdx++)
    {
        // the group is not in flash
        if (g_taMwFimGroupStatusTable[ulZoneIdx][ulGroupIdx].ubBlockIdx == 0xFF)
        {
            if (MW_FIM_OK == MwFim_EmptyBlockFind(ulZoneIdx, ubaBlockUsed, &ulEmptyIdx))
            {
                ubaBlockUsed[ulEmptyIdx] = ulGroupIdx;
                g_taMwFimGroupStatusTable[ulZoneIdx][ulGroupIdx].ubMajorVersion = g_taMwFimZoneInfoTable[ulZoneIdx].pubVerTable[ulGroupIdx];
                g_taMwFimGroupStatusTable[ulZoneIdx][ulGroupIdx].ubMinorVersion = MW_FIM_VER_GROUP_SEQUENCE_MIN;
                g_taMwFimGroupStatusTable[ulZoneIdx][ulGroupIdx].ubBlockIdx = ulEmptyIdx;
                
                // if it is not swap block
                if (ulGroupIdx != 0)
                {
                    // fill the group header
                    tGroupHeader.ulSignature = MW_FIM_SIGNATURE_GROUP;
                    tGroupHeader.ubZoneIdx = ulZoneIdx;
                    tGroupHeader.ubGroupIdx = ulGroupIdx;
                    tGroupHeader.ubMajorVersion = g_taMwFimGroupStatusTable[ulZoneIdx][ulGroupIdx].ubMajorVersion;
                    tGroupHeader.ubMinorVersion = g_taMwFimGroupStatusTable[ulZoneIdx][ulGroupIdx].ubMinorVersion;
                    tGroupHeader.ulCheckSum = 0;
                    pubData = (uint8_t*)&tGroupHeader;
                    ubDataSize = sizeof(T_MwFimGroupHeader) - 4;        // 4 bytes check sum
                    for (i=0; i<ubDataSize; i++)
                    {
                        tGroupHeader.ulCheckSum = tGroupHeader.ulCheckSum + pubData[i];
                    }
                    
                    // write the block
                    ulBlockAddr = g_taMwFimZoneInfoTable[ulZoneIdx].ulBaseAddr + g_taMwFimZoneInfoTable[ulZoneIdx].ulBlockSize * ulEmptyIdx;
                    if (0 != Hal_Flash_AddrProgram(SPI_IDX_0, ulBlockAddr, 0, sizeof(T_MwFimGroupHeader), (uint8_t*)&tGroupHeader))
                    {
                        //printf("FIM: #10\n");
                    }
                }
            }
        } // the group is not in flash
    }
}

/*************************************************************************
* FUNCTION:
*   MwFim_EmptyBlockFind
*
* DESCRIPTION:
*   find an empty block
*
* PARAMETERS
*   1. ulZoneIdx    : [In] the zone index
*   2. ubaBlockUsed : [In] the usage of each block
*   3. pulIndex     : [Out] the empty index
*
* RETURNS
*   1. MW_FIM_OK   : success
*   2. MW_FIM_FAIL : fail
*
*************************************************************************/
uint8_t MwFim_EmptyBlockFind_impl(uint32_t ulZoneIdx, uint8_t ubaBlockUsed[MW_FIM_GROUP_MAX], uint32_t *pulIndex)
{
    uint8_t ubRet = MW_FIM_FAIL;
    uint32_t ulBlockIdx;
    
    // find which block is not used
    for (ulBlockIdx=0; ulBlockIdx<g_taMwFimZoneInfoTable[ulZoneIdx].ulBlockNum; ulBlockIdx++)
    {
        if (ubaBlockUsed[ulBlockIdx] == 0xFF)
        {
            *pulIndex = ulBlockIdx;
            ubRet = MW_FIM_OK;
            
            break;
        }
    }
    
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*   MwFim_GroupBlockParser
*
* DESCRIPTION:
*   parse the block to fill the free offset of group and the information of file
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void MwFim_GroupBlockParser_impl(void)
{
    T_MwFimGroupHeader tGroupHeader;
    uint32_t ulBlockAddr;
    
    T_MwFimFileInfo *ptFileTable;
    T_MwFimFileHeader tFileHeader;
    uint32_t ulDataOffset;
    uint32_t ulIndex;
    
    uint32_t ulPrefix;

    uint32_t ulZoneIdx;
    uint32_t ulGroupIdx;
    uint32_t i;

    for (ulZoneIdx=0; ulZoneIdx<MW_FIM_ZONE_MAX; ulZoneIdx++)
    {
        // check the block number if the zone is empty
        if (0 == g_taMwFimZoneInfoTable[ulZoneIdx].ulBlockNum)
            continue;
        
        // the index 0 is the swap group, don't need to parse it
        for (ulGroupIdx=1; ulGroupIdx<g_taMwFimZoneInfoTable[ulZoneIdx].ulBlockNum; ulGroupIdx++)
        {
            // reset the address of file
            ptFileTable = g_ptaMwFimGroupInfoTable[ulZoneIdx][ulGroupIdx];
            i = 0;
            ulPrefix = ((ulZoneIdx & 0xFF) << 24) + ((ulGroupIdx & 0xFF) << 16);
            while ((ptFileTable[i].ulFileId & 0xFFFF0000) == ulPrefix)
            {
                memset(ptFileTable[i].pulDataAddr, 0xFF, sizeof(uint32_t) * ptFileTable[i].uwRecordMax);
                i++;
                if (i >= 0x10000)
                {
                    //printf("FIM: #11\n");
                    break;
                }
            }

            // read the group header
            ulBlockAddr = g_taMwFimZoneInfoTable[ulZoneIdx].ulBaseAddr 
                          + g_taMwFimZoneInfoTable[ulZoneIdx].ulBlockSize * g_taMwFimGroupStatusTable[ulZoneIdx][ulGroupIdx].ubBlockIdx;
            if (0 != Hal_Flash_AddrRead(SPI_IDX_0, ulBlockAddr, 0, sizeof(T_MwFimGroupHeader), (uint8_t*)&tGroupHeader))
            {
                //printf("FIM: #12\n");
                continue;
            }
            
            // verify the header information of group
            if (MW_FIM_OK != MwFim_GroupHeaderVerify(ulZoneIdx, &tGroupHeader))
                continue;
            
            // parse the header information of file
            ulDataOffset = sizeof(T_MwFimGroupHeader);
            while (0 == Hal_Flash_AddrRead(SPI_IDX_0, ulBlockAddr + ulDataOffset, 0, sizeof(T_MwFimFileHeader), (uint8_t*)&tFileHeader))
            {
                // verify the header information of file, and get the index from the file table
                ulIndex = MwFim_FileHeaderVerify(ptFileTable, &tFileHeader);
                if (ulIndex == 0xFFFFFFFF)
                    break;
                
                // update the address of file data
                ptFileTable[ulIndex].pulDataAddr[tFileHeader.uwRecordIdx] = ulBlockAddr + ulDataOffset + sizeof(T_MwFimFileHeader);
                
                // the next file
                ulDataOffset = ulDataOffset + sizeof(T_MwFimFileHeader) + tFileHeader.uwDataSize;
                
                // the end of block
                if ((ulDataOffset + sizeof(T_MwFimFileHeader)) >= g_taMwFimZoneInfoTable[ulZoneIdx].ulBlockSize)
                    break;
            }
            
            // fill the offset of free space
            g_taMwFimGroupStatusTable[ulZoneIdx][ulGroupIdx].ulFreeOffset = MwFim_FreeOffsetVerify(ulZoneIdx, ulGroupIdx, ulDataOffset);
        } // for (ulGroupIdx=1; ulGroupIdx<g_taMwFimZoneInfoTable[ulZoneIdx].ulBlockNum; ulGroupIdx++)
    } // for (ulZoneIdx=0; ulZoneIdx<MW_FIM_ZONE_MAX; ulZoneIdx++)
}

/*************************************************************************
* FUNCTION:
*   MwFim_FileHeaderVerify
*
* DESCRIPTION:
*   verify the file header and return the index of file if found
*
* PARAMETERS
*   1. ptFileTable  : [In] the information table of file
*   2. ptFileHeader : [In] the header data of file
*
* RETURNS
*   1. index      : the index in the information table of file
*   2. 0xFFFFFFFF : fail
*
*************************************************************************/
uint32_t MwFim_FileHeaderVerify_impl(T_MwFimFileInfo *ptFileTable, T_MwFimFileHeader *ptFileHeader)
{
    uint32_t ulFileIdx = 0xFFFFFFFF;
    
    uint32_t ulPrefix;
    
    uint32_t ulZoneIdx;
    uint32_t ulGroupIdx;
    uint32_t i;

    // check the signature
    if (ptFileHeader->ulSignature != MW_FIM_SIGNATURE_FILE)
        goto done;
    
    // get the zone and group index
    ulZoneIdx = (ptFileHeader->ulFileId >> 24) & 0xFF;
    ulGroupIdx = (ptFileHeader->ulFileId >> 16) & 0xFF;
    
    // find the index by file ID
    i = 0;
    ulPrefix = ((ulZoneIdx & 0xFF) << 24) + ((ulGroupIdx & 0xFF) << 16);
    while ((ptFileTable[i].ulFileId & 0xFFFF0000) == ulPrefix)
    {
        if (ptFileTable[i].ulFileId == ptFileHeader->ulFileId)
        {
            ulFileIdx = i;
            break;
        }
        
        i++;
        if (i >= 0x10000)
        {
            //printf("FIM: #13\n");
            break;
        }
    }
    if (ulFileIdx == 0xFFFFFFFF)
        goto done;
    
    // check the index of record
    if (ptFileHeader->uwRecordIdx >= ptFileTable[ulFileIdx].uwRecordMax)
    {
        ulFileIdx = 0xFFFFFFFF;
        goto done;
    }

    // check the size of record
    if (ptFileHeader->uwDataSize != ptFileTable[ulFileIdx].uwDataSize)
    {
        ulFileIdx = 0xFFFFFFFF;
        goto done;
    }

done:    
    return ulFileIdx;
}

/*************************************************************************
* FUNCTION:
*   MwFim_FreeOffsetVerify
*
* DESCRIPTION:
*   verify the offset of free space
*
* PARAMETERS
*   1. ulZoneIdx    : [In] the zone index
*   2. ulGroupIdx   : [In] the group index
*   3. ulDataOffset : [In] the current offset
*
* RETURNS
*   1. offset     : success
*   2. 0xFFFFFFFF : fail
*
*************************************************************************/
uint32_t MwFim_FreeOffsetVerify_impl(uint32_t ulZoneIdx, uint32_t ulGroupIdx, uint32_t ulDataOffset)
{
    uint8_t ubaBuffer[sizeof(T_MwFimFileHeader) + MW_FIM_DATA_SIZE_MIN];
    uint32_t ulBufferSize;
    uint32_t ulBlockAddr;
    uint32_t ulFreeOffset = MW_FIM_SPACE_FULL;
    
    uint32_t i;
    
    // the end of block
    ulBufferSize = sizeof(T_MwFimFileHeader) + MW_FIM_DATA_SIZE_MIN;
    if ((ulDataOffset + ulBufferSize) >= g_taMwFimZoneInfoTable[ulZoneIdx].ulBlockSize)
        goto done;
    
    // get the start address of block
    ulBlockAddr = g_taMwFimZoneInfoTable[ulZoneIdx].ulBaseAddr 
                  + g_taMwFimZoneInfoTable[ulZoneIdx].ulBlockSize * g_taMwFimGroupStatusTable[ulZoneIdx][ulGroupIdx].ubBlockIdx;

    // verify if the space is free
    // !!!
    // 1. check if each value in data buffer is 0xFF
    // 2. the size is FileHeader + MW_FIM_DATA_SIZE_MIN
    if (0 != Hal_Flash_AddrRead(SPI_IDX_0, ulBlockAddr + ulDataOffset, 0, ulBufferSize, ubaBuffer))
    {
        //printf("FIM: #14\n");
        goto done;
    }
    
    // check the value is 0xFF or not
    for (i=0; i<ulBufferSize; i++)
    {
        if (ubaBuffer[i] != 0xFF)
            goto done;
    }

    // it is free
    ulFreeOffset = ulDataOffset;
    
done:
    return ulFreeOffset;
}

/*************************************************************************
* FUNCTION:
*   MwFim_FileTableSearch
*
* DESCRIPTION:
*   search the file table by file ID
*
* PARAMETERS
*   1. ulFileId     : [In] the file ID
*   2. pptFileTable : [Out] the pointer of file table
*
* RETURNS
*   1. MW_FIM_OK   : success
*   2. MW_FIM_FAIL : fail
*
*************************************************************************/
uint8_t MwFim_FileTableSearch_impl(uint32_t ulFileId, T_MwFimFileInfo **pptFileTable)
{
    T_MwFimFileInfo *ptFileTable;
    uint32_t ulFileIdx = 0xFFFFFFFF;
    uint8_t ubRet = MW_FIM_FAIL;
    
    uint32_t ulPrefix;
    
    uint32_t ulZoneIdx;
    uint32_t ulGroupIdx;
    uint32_t i;
    
    // get the zone and group index
    ulZoneIdx = (ulFileId >> 24) & 0xFF;
    ulGroupIdx = (ulFileId >> 16) & 0xFF;
    
    // get the group information table
    ptFileTable = g_ptaMwFimGroupInfoTable[ulZoneIdx][ulGroupIdx];
    
    // find the index by file ID
    i = 0;
    ulPrefix = ((ulZoneIdx & 0xFF) << 24) + ((ulGroupIdx & 0xFF) << 16);
    while ((ptFileTable[i].ulFileId & 0xFFFF0000) == ulPrefix)
    {
        if (ptFileTable[i].ulFileId == ulFileId)
        {
            ulFileIdx = i;
            break;
        }
        
        i++;
        if (i >= 0x10000)
        {
            //printf("FIM: #15\n");
            break;
        }
    }
    if (ulFileIdx == 0xFFFFFFFF)
        goto done;
    
    *pptFileTable = &(ptFileTable[ulFileIdx]);
    ubRet = MW_FIM_OK;

done:    
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*   MwFim_GroupSwap
*
* DESCRIPTION:
*   swap the current group
*
* PARAMETERS
*   1. ulZoneIdx    : [In] the zone index
*   2. ulGroupIdx   : [In] the group index
*
* RETURNS
*   1. MW_FIM_OK   : success
*   2. MW_FIM_FAIL : fail
*
*************************************************************************/
uint8_t MwFim_GroupSwap_impl(uint32_t ulZoneIdx, uint32_t ulGroupIdx)
{
    T_MwFimGroupHeader tGroupHeader;
    T_MwFimFileHeader tFileHeader;
    T_MwFimFileInfo *ptFileTable;
    uint32_t ulBlockAddr;
    uint32_t ulFreeOffset;
    uint8_t ubTargetBlockIdx;
    uint8_t ubRet = MW_FIM_FAIL;
    
    uint32_t ulPrefix;
    
    uint8_t ubaDataBuffer[MW_FIM_DATA_BUFFER_SIZE];
    uint32_t ulDataSize;
    uint32_t ulDataOffset;
    uint32_t ulDataHandleSize;
    
    uint8_t* pubData;
    uint8_t ubDataSize;
    uint8_t ubMinorVersion;
    
    uint32_t i, j;
    
    // read the original group header
    ulBlockAddr = g_taMwFimZoneInfoTable[ulZoneIdx].ulBaseAddr 
                  + g_taMwFimZoneInfoTable[ulZoneIdx].ulBlockSize * g_taMwFimGroupStatusTable[ulZoneIdx][ulGroupIdx].ubBlockIdx;
    if (0 != Hal_Flash_AddrRead(SPI_IDX_0, ulBlockAddr, 0, sizeof(T_MwFimGroupHeader), (uint8_t*)&tGroupHeader))
    {
        //printf("FIM: #16\n");
        goto done;
    }
    
    // verify the header information of group
    if (MW_FIM_OK != MwFim_GroupHeaderVerify(ulZoneIdx, &tGroupHeader))
        goto done;
    
    // get the target block index from swap group
    ubTargetBlockIdx = g_taMwFimGroupStatusTable[ulZoneIdx][0].ubBlockIdx;
    
    // get the start address of target block and set the offset of free space
    ulBlockAddr = g_taMwFimZoneInfoTable[ulZoneIdx].ulBaseAddr + g_taMwFimZoneInfoTable[ulZoneIdx].ulBlockSize * ubTargetBlockIdx;
    ulFreeOffset = sizeof(T_MwFimGroupHeader);
    
    // copy the file data from the original group to swap group
    ptFileTable = g_ptaMwFimGroupInfoTable[ulZoneIdx][ulGroupIdx];
    i = 0;
    ulPrefix = ((ulZoneIdx & 0xFF) << 24) + ((ulGroupIdx & 0xFF) << 16);
    while ((ptFileTable[i].ulFileId & 0xFFFF0000) == ulPrefix)
    {
        for (j=0; j<ptFileTable[i].uwRecordMax; j++)
        {
            // check the file data is exist or not
            if (ptFileTable[i].pulDataAddr[j] != 0xFFFFFFFF)
            {
                // 1. write the file header
                tFileHeader.ulSignature = MW_FIM_SIGNATURE_FILE;
                tFileHeader.ulFileId = ptFileTable[i].ulFileId;
                tFileHeader.uwRecordIdx = j;
                tFileHeader.uwDataSize = ptFileTable[i].uwDataSize;
                if (0 != Hal_Flash_AddrProgram(SPI_IDX_0, ulBlockAddr + ulFreeOffset, 0, sizeof(T_MwFimFileHeader), (uint8_t*)&tFileHeader))
                {
                    //printf("FIM: #17\n");
                    goto done;
                }
                
                // update the information
                ulFreeOffset = ulFreeOffset + sizeof(T_MwFimFileHeader);
                
                // 2. write the file data
                ulDataSize = ptFileTable[i].uwDataSize;
                ulDataOffset = 0;
                while (ulDataSize > 0)
                {
                    // decide the handle size for this loop
                    if (ulDataSize >= MW_FIM_DATA_BUFFER_SIZE)
                        ulDataHandleSize = MW_FIM_DATA_BUFFER_SIZE;
                    else
                        ulDataHandleSize = ulDataSize;
                    
                    // read from the original address
                    if (0 != Hal_Flash_AddrRead(SPI_IDX_0, ptFileTable[i].pulDataAddr[j] + ulDataOffset, 0, ulDataHandleSize, ubaDataBuffer))
                    {
                        //printf("FIM: #18\n");
                        goto done;
                    }
                    
                    // write into the target block
                    if (0 != Hal_Flash_AddrProgram(SPI_IDX_0, ulBlockAddr + ulFreeOffset + ulDataOffset, 0, ulDataHandleSize, ubaDataBuffer))
                    {
                        //printf("FIM: #19\n");
                        goto done;
                    }
                    
                    // update the information
                    ulDataSize = ulDataSize - ulDataHandleSize;
                    ulDataOffset = ulDataOffset + ulDataHandleSize;
                }
                
                // update the address of file
                ptFileTable[i].pulDataAddr[j] = ulBlockAddr + ulFreeOffset;
                
                // update the information
                ulFreeOffset = ulFreeOffset + ptFileTable[i].uwDataSize;
            } // if (ptFileTable[i].pulDataAddr[j] != 0xFFFFFFFF)
        } // for (j=0; j<ptFileTable[i].uwRecordMax; j++)
        
        i++;
        if (i >= 0x10000)
        {
            //printf("FIM: #20\n");
            break;
        }
    } // while ((ptFileTable[i].ulFileId & 0xFFFF0000) == ulPrefix)
    
    // 3. write the group header
    // fill the group header
    tGroupHeader.ulSignature = MW_FIM_SIGNATURE_GROUP;
    tGroupHeader.ubZoneIdx = ulZoneIdx;
    tGroupHeader.ubGroupIdx = ulGroupIdx;
    tGroupHeader.ubMajorVersion = g_taMwFimGroupStatusTable[ulZoneIdx][ulGroupIdx].ubMajorVersion;
    ubMinorVersion = g_taMwFimGroupStatusTable[ulZoneIdx][ulGroupIdx].ubMinorVersion + 1;
    if (ubMinorVersion >= MW_FIM_VER_GROUP_SEQUENCE_OVER)
        ubMinorVersion = MW_FIM_VER_GROUP_SEQUENCE_MIN;
    tGroupHeader.ubMinorVersion = ubMinorVersion;
    tGroupHeader.ulCheckSum = 0;
    pubData = (uint8_t*)&tGroupHeader;
    ubDataSize = sizeof(T_MwFimGroupHeader) - 4;        // 4 bytes check sum
    for (i=0; i<ubDataSize; i++)
    {
        tGroupHeader.ulCheckSum = tGroupHeader.ulCheckSum + pubData[i];
    }
    // write the block
    if (0 != Hal_Flash_AddrProgram(SPI_IDX_0, ulBlockAddr, 0, sizeof(T_MwFimGroupHeader), (uint8_t*)&tGroupHeader))
    {
        //printf("FIM: #21\n");
        goto done;
    }
    
    // 4. update the information of group
    // swap
    g_taMwFimGroupStatusTable[ulZoneIdx][0].ubBlockIdx = g_taMwFimGroupStatusTable[ulZoneIdx][ulGroupIdx].ubBlockIdx;
    // the current group
    g_taMwFimGroupStatusTable[ulZoneIdx][ulGroupIdx].ulFreeOffset = ulFreeOffset;
    g_taMwFimGroupStatusTable[ulZoneIdx][ulGroupIdx].ubMinorVersion = ubMinorVersion;
    g_taMwFimGroupStatusTable[ulZoneIdx][ulGroupIdx].ubBlockIdx = ubTargetBlockIdx;
    
    // 5. erase the swap group
    ulBlockAddr = g_taMwFimZoneInfoTable[ulZoneIdx].ulBaseAddr 
                  + g_taMwFimZoneInfoTable[ulZoneIdx].ulBlockSize * g_taMwFimGroupStatusTable[ulZoneIdx][0].ubBlockIdx;
    Hal_Flash_4KSectorAddrErase(SPI_IDX_0, ulBlockAddr);
    
    ubRet = MW_FIM_OK;

done:    
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*   MwFim_FileDataDefaultFill
*
* DESCRIPTION:
*   fill the default value of file if the data is not exist
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void MwFim_FileDataDefaultFill_impl(void)
{
    T_MwFimFileInfo *ptFileTable;
    
    uint32_t ulPrefix;
    
    uint32_t ulZoneIdx;
    uint32_t ulGroupIdx;
    uint32_t i, j;
    
    for (ulZoneIdx=0; ulZoneIdx<MW_FIM_ZONE_MAX; ulZoneIdx++)
    {
        for (ulGroupIdx=0; ulGroupIdx<g_taMwFimZoneInfoTable[ulZoneIdx].ulBlockNum; ulGroupIdx++)
        {
            ptFileTable = g_ptaMwFimGroupInfoTable[ulZoneIdx][ulGroupIdx];
            i = 0;
            ulPrefix = ((ulZoneIdx & 0xFF) << 24) + ((ulGroupIdx & 0xFF) << 16);
            while ((ptFileTable[i].ulFileId & 0xFFFF0000) == ulPrefix)
            {
                // check the default value is exist or not
                if (ptFileTable[i].pubDefaultValue != NULL)
                {
                    for (j=0; j<ptFileTable[i].uwRecordMax; j++)
                    {
                        // check the record is exist or not
                        if (ptFileTable[i].pulDataAddr[j] != 0xFFFFFFFF)
                            continue;

                        // write the default value
                        if (MW_FIM_OK != MwFim_FileWriteDo(ptFileTable[i].ulFileId, j, ptFileTable[i].uwDataSize, ptFileTable[i].pubDefaultValue))
                        {
                            //printf("FIM: #22\n");
                        }
                    }
                }
                
                i++;
                // error handle for the end element is exist or not
                if (i >= 0x10000)
                {
                    //printf("FIM: #23\n");
                    break;
                }
            }
        } // for (ulGroupIdx=0; ulGroupIdx<MW_FIM_GROUP_MAX; ulGroupIdx++)
    } // for (ulZoneIdx=0; ulZoneIdx<MW_FIM_ZONE_MAX; ulZoneIdx++)
}

//I0051 0001076_add_begin
/*************************************************************************
* FUNCTION:
*   MwFim_Dummy
*
* DESCRIPTION:
*   dummy function
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void MwFim_Dummy(void)
{
    return;
}
//I0051 0001076_add_end

/*************************************************************************
* FUNCTION:
*   MwFim_PreInitCold
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
void MwFim_PreInitCold(void)
{
    MwFim_FlashLayoutUpdate = NULL;
    
    g_ubMwFimInit = 0;
    g_tMwFimSemaphoreId = NULL;
    memset(g_taMwFimGroupStatusTable, 0xFF, sizeof(T_MwFimGroupStatus) * MW_FIM_ZONE_MAX * MW_FIM_GROUP_MAX);

    MwFim_Init              = MwFim_Init_impl;
    MwFim_FileRead          = MwFim_FileRead_impl;
    MwFim_FileWrite         = MwFim_FileWrite_impl;
    MwFim_FileWriteDefault  = MwFim_FileWriteDefault_impl;
    MwFim_FileDelete        = MwFim_FileDelete_impl;

    MwFim_ZoneInfoUpdate     = MwFim_ZoneInfoUpdate_impl;
    MwFim_GroupInfoUpdate    = MwFim_GroupInfoUpdate_impl;
    MwFim_GroupVersionUpdate = MwFim_GroupVersionUpdate_impl;

    // internal api
    MwFim_FileWriteDo           = MwFim_FileWriteDo_impl;
    MwFim_GroupSizeCheck        = MwFim_GroupSizeCheck_impl;
    MwFim_GroupStatusFill       = MwFim_GroupStatusFill_impl;
    MwFim_GroupHeaderCheck      = MwFim_GroupHeaderCheck_impl;
    MwFim_GroupHeaderVerify     = MwFim_GroupHeaderVerify_impl;
    MwFim_GroupCheckSumVerify   = MwFim_GroupCheckSumVerify_impl;
    MwFim_GroupStatusRemap      = MwFim_GroupStatusRemap_impl;
    MwFim_EmptyBlockFind        = MwFim_EmptyBlockFind_impl;
    MwFim_GroupBlockParser      = MwFim_GroupBlockParser_impl;
    MwFim_FileHeaderVerify      = MwFim_FileHeaderVerify_impl;
    MwFim_FreeOffsetVerify      = MwFim_FreeOffsetVerify_impl;
    MwFim_FileTableSearch       = MwFim_FileTableSearch_impl;
    MwFim_GroupSwap             = MwFim_GroupSwap_impl;
    //I0051 0001076 MwFim_FileDataDefaultFill   = MwFim_FileDataDefaultFill_impl;
    MwFim_FileDataDefaultFill   = MwFim_Dummy; //I0051 0001076
}
