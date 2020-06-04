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

#include "mw_fim_patch.h"
#include "hal_vic.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define MW_FIM_SIGNATURE_GROUP      0x67726F70  // grop
#define MW_FIM_SIGNATURE_FILE       0x46494C45  // FILE

#define MW_FIM_VER_GROUP_SEQUENCE_MIN       0x01
#define MW_FIM_VER_GROUP_SEQUENCE_MAX       0xFE
#define MW_FIM_VER_GROUP_SEQUENCE_OVER      0xFF

#define MW_FIM_DATA_SIZE_MIN        8   // used by check the offset of free space
#define MW_FIM_DATA_BUFFER_SIZE     64  // used for swap behavior

#define MW_FIM_SPACE_FULL           0x7FFFFFFF  // can't use 0xFFFFFFFF, it will be overflow in some case

#define MW_FIM_SWAP_DATA_DELETE     0xFFFFFFFF  // the data is not exist or want to delete it when do swap
#define MW_FIM_SWAP_DATA_REPLACE    0xFFFFFFFD  // replace the current data when do swap

#define MW_FIM_FLASH_BLOCK_SIZE     0x1000      // 4KB
#define MW_FIM_FLASH_BLOCK_MASK     0x0FFF      // 4KB Mask

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern T_MwFim_FlashLayoutUpdate_Fp MwFim_FlashLayoutUpdate;


// Sec 5: declaration of global function prototype
extern T_MwFim_Init_Fp MwFim_Init;
extern T_MwFim_FileRead_Fp MwFim_FileRead;
extern T_MwFim_FileWrite_Fp MwFim_FileWrite;
extern T_MwFim_FileWriteDefault_Fp MwFim_FileWriteDefault;
extern T_MwFim_FileDelete_Fp MwFim_FileDelete;

extern T_MwFim_ZoneInfoUpdate_Fp MwFim_ZoneInfoUpdate;
extern T_MwFim_GroupInfoUpdate_Fp MwFim_GroupInfoUpdate;
extern T_MwFim_GroupVersionUpdate_Fp MwFim_GroupVersionUpdate;


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable
extern uint8_t g_ubMwFimInit;
extern osSemaphoreId g_tMwFimSemaphoreId;
extern T_MwFimGroupStatus g_taMwFimGroupStatusTable[MW_FIM_ZONE_MAX][MW_FIM_GROUP_MAX];


// Sec 7: declaration of static function prototype
extern T_MwFim_FileWriteDo_Fp MwFim_FileWriteDo;
extern T_MwFim_GroupSizeCheck_Fp MwFim_GroupSizeCheck;
extern T_MwFim_GroupStatusFill_Fp MwFim_GroupStatusFill;
extern T_MwFim_GroupHeaderCheck_Fp MwFim_GroupHeaderCheck;
extern T_MwFim_GroupHeaderVerify_Fp MwFim_GroupHeaderVerify;
extern T_MwFim_GroupCheckSumVerify_Fp MwFim_GroupCheckSumVerify;
extern T_MwFim_GroupStatusRemap_Fp MwFim_GroupStatusRemap;
extern T_MwFim_EmptyBlockFind_Fp MwFim_EmptyBlockFind;
extern T_MwFim_GroupBlockParser_Fp MwFim_GroupBlockParser;
extern T_MwFim_FileHeaderVerify_Fp MwFim_FileHeaderVerify;
extern T_MwFim_FreeOffsetVerify_Fp MwFim_FreeOffsetVerify;
extern T_MwFim_FileTableSearch_Fp MwFim_FileTableSearch;
extern T_MwFim_GroupSwap_Fp MwFim_GroupSwap;
extern T_MwFim_FileDataDefaultFill_Fp MwFim_FileDataDefaultFill;

uint8_t MwFim_GroupSwap_patch(uint32_t ulZoneIdx, uint32_t ulGroupIdx, T_MwFimFileData *ptReplaceData);
uint8_t MwFim_GroupBlockErase(uint32_t ulStartAddr, uint32_t ulGroupSize);

extern void MwFim_FileDataDefaultFill_impl(void);


/***********
C Functions
***********/
// Sec 8: C Functions

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
uint8_t MwFim_FileWriteDo_patch(uint32_t ulFileId, uint16_t uwRecIdx, uint16_t uwFileSize, uint8_t *pubFileData)
{
    T_MwFimFileInfo *ptFileTable;
    uint8_t ubRet = MW_FIM_FAIL;

    T_MwFimFileHeader tFileHeader;
    uint32_t ulFreeAddr;
    
    uint32_t ulZoneIdx;
    uint32_t ulGroupIdx;

    T_MwFimFileData tReplaceData;
    
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
        // replace the file data
        ptFileTable->pulDataAddr[uwRecIdx] = MW_FIM_SWAP_DATA_REPLACE;
        
        // do the swap behavior
        tReplaceData.ulFileId = ulFileId;
        tReplaceData.uwRecIdx = uwRecIdx;
        tReplaceData.uwFileSize = uwFileSize;
        tReplaceData.pubFileData = pubFileData;
        if (MW_FIM_OK != MwFim_GroupSwap_patch(ulZoneIdx, ulGroupIdx, &tReplaceData))
            goto done;
    }
    else
    {
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
    }
    
    ubRet = MW_FIM_OK;
    
done:
    return ubRet;
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
void MwFim_GroupHeaderCheck_patch(uint32_t ulZoneIdx, uint8_t ubaBlockUsed[MW_FIM_GROUP_MAX])
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
            if (MW_FIM_OK != MwFim_GroupBlockErase(ulBlockAddr, g_taMwFimZoneInfoTable[ulZoneIdx].ulBlockSize))
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
                if (MW_FIM_OK != MwFim_GroupBlockErase(g_taMwFimZoneInfoTable[ulZoneIdx].ulBaseAddr + g_taMwFimZoneInfoTable[ulZoneIdx].ulBlockSize * ulOldIdx, g_taMwFimZoneInfoTable[ulZoneIdx].ulBlockSize))
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
                if (MW_FIM_OK != MwFim_GroupBlockErase(ulBlockAddr, g_taMwFimZoneInfoTable[ulZoneIdx].ulBlockSize))
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
                if (MW_FIM_OK != MwFim_GroupBlockErase(g_taMwFimZoneInfoTable[ulZoneIdx].ulBaseAddr + g_taMwFimZoneInfoTable[ulZoneIdx].ulBlockSize * ulOldIdx, g_taMwFimZoneInfoTable[ulZoneIdx].ulBlockSize))
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
                if (MW_FIM_OK != MwFim_GroupBlockErase(ulBlockAddr, g_taMwFimZoneInfoTable[ulZoneIdx].ulBlockSize))
                {
                    //printf("FIM: #09\n");
                }
            }
        }
    } // 2. read the information from each block
}

/*************************************************************************
* FUNCTION:
*   MwFim_GroupSwap
*
* DESCRIPTION:
*   swap the current group and replace one file data (optional)
*
* PARAMETERS
*   1. ulZoneIdx     : [In] the zone index
*   2. ulGroupIdx    : [In] the group index
*   3. ptReplaceData : [In] the file data for the replacement
*
* RETURNS
*   1. MW_FIM_OK   : success
*   2. MW_FIM_FAIL : fail
*
*************************************************************************/
uint8_t MwFim_GroupSwap_patch(uint32_t ulZoneIdx, uint32_t ulGroupIdx, T_MwFimFileData *ptReplaceData)
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
            if (ptFileTable[i].pulDataAddr[j] != MW_FIM_SWAP_DATA_DELETE)
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
                // 2-1. replace the file data
                if (ptFileTable[i].pulDataAddr[j] == MW_FIM_SWAP_DATA_REPLACE)
                {
                    if (ptReplaceData->pubFileData != NULL)
                    {
                        // write into the target block
                        if (0 != Hal_Flash_AddrProgram(SPI_IDX_0, ulBlockAddr + ulFreeOffset, 0, ptFileTable[i].uwDataSize, ptReplaceData->pubFileData))
                        {
                            //printf("FIM: #19\n");
                            goto done;
                        }
                    }
                }
                // 2-2. write the original data
                else
                {
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
                } // if (ptFileTable[i].pulDataAddr[j] == MW_FIM_SWAP_DATA_REPLACE)
                
                // update the address of file
                ptFileTable[i].pulDataAddr[j] = ulBlockAddr + ulFreeOffset;
                
                // update the information
                ulFreeOffset = ulFreeOffset + ptFileTable[i].uwDataSize;
            } // if (ptFileTable[i].pulDataAddr[j] != MW_FIM_SWAP_DATA_DELETE)
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
    if (MW_FIM_OK != MwFim_GroupBlockErase(ulBlockAddr, g_taMwFimZoneInfoTable[ulZoneIdx].ulBlockSize))
        goto done;
    
    ubRet = MW_FIM_OK;

done:    
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*   MwFim_GroupBlockErase
*
* DESCRIPTION:
*   erase the flash of group block
*
* PARAMETERS
*   1. ulStartAddr : [In] the start address
*   2. ulGroupSize : [In] the group size
*
* RETURNS
*   1. MW_FIM_OK   : success
*   2. MW_FIM_FAIL : fail
*
*************************************************************************/
uint8_t MwFim_GroupBlockErase(uint32_t ulStartAddr, uint32_t ulGroupSize)
{
    uint8_t ubRet = MW_FIM_FAIL;
    uint32_t ulCurrentAddr;
    uint32_t ulRestSize;

    // check the start address
    if (MW_FIM_FLASH_BLOCK_MASK & ulStartAddr)
        goto done;

    // check the group size
    if (MW_FIM_FLASH_BLOCK_MASK & ulGroupSize)
        goto done;

    // erase the group block
    ulCurrentAddr = ulStartAddr;
    ulRestSize = ulGroupSize;
    while (ulRestSize > 0)
    {
        if (0 != Hal_Flash_4KSectorAddrErase(SPI_IDX_0, ulCurrentAddr))
            goto done;

        ulCurrentAddr = ulCurrentAddr + MW_FIM_FLASH_BLOCK_SIZE;
        ulRestSize = ulRestSize - MW_FIM_FLASH_BLOCK_SIZE;
    }

    ubRet = MW_FIM_OK;

done:    
    return ubRet;    
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
uint8_t MwFim_FileRead_patch(uint32_t ulFileId, uint16_t uwRecIdx, uint16_t uwFileSize, uint8_t *pubFileData)
{
    T_MwFimFileInfo *ptFileTable;
    uint8_t ubRet = MW_FIM_FAIL;
    
    // check init
    if (g_ubMwFimInit != 1)
        return ubRet;
    
    // wait the semaphore
    osSemaphoreWait(g_tMwFimSemaphoreId, osWaitForever);

    Hal_Vic_IntMask(GPIO_IRQn, 1);

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
    Hal_Vic_IntMask(GPIO_IRQn, 0);

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
uint8_t MwFim_FileWrite_patch(uint32_t ulFileId, uint16_t uwRecIdx, uint16_t uwFileSize, uint8_t *pubFileData)
{
    uint8_t ubRet = MW_FIM_FAIL;

    // check init
    if (g_ubMwFimInit != 1)
        return ubRet;
    
    // wait the semaphore
    osSemaphoreWait(g_tMwFimSemaphoreId, osWaitForever);

    Hal_Vic_IntMask(GPIO_IRQn, 1);
    
    // do the behavior
    if (MW_FIM_OK != MwFim_FileWriteDo(ulFileId, uwRecIdx, uwFileSize, pubFileData))
        goto done;
    
    ubRet = MW_FIM_OK;
    
done:
    Hal_Vic_IntMask(GPIO_IRQn, 0);

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
uint8_t MwFim_FileWriteDefault_patch(uint32_t ulFileId, uint16_t uwRecIdx)
{
    T_MwFimFileInfo *ptFileTable;
    uint8_t ubRet = MW_FIM_FAIL;
    
    // check init
    if (g_ubMwFimInit != 1)
        return ubRet;
    
    // wait the semaphore
    osSemaphoreWait(g_tMwFimSemaphoreId, osWaitForever);

    Hal_Vic_IntMask(GPIO_IRQn, 1);
    
    // search the file table by file ID
    if (MW_FIM_OK != MwFim_FileTableSearch(ulFileId, &ptFileTable))
        goto done;
    
    // write the default value
    if (MW_FIM_OK != MwFim_FileWriteDo(ulFileId, uwRecIdx, ptFileTable->uwDataSize, ptFileTable->pubDefaultValue))
        goto done;
    
    ubRet = MW_FIM_OK;
    
done:
    Hal_Vic_IntMask(GPIO_IRQn, 0);

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
uint8_t MwFim_FileDelete_patch(uint32_t ulFileId, uint16_t uwRecIdx)
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

    Hal_Vic_IntMask(GPIO_IRQn, 1);
    
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
    Hal_Vic_IntMask(GPIO_IRQn, 0);

    // release the semaphore
    osSemaphoreRelease(g_tMwFimSemaphoreId);
    return ubRet;
}

void MwFim_PreInit_patch(void)
{
    MwFim_FileDataDefaultFill   = MwFim_FileDataDefaultFill_impl;
    MwFim_FileWriteDo           = MwFim_FileWriteDo_patch;
    MwFim_GroupHeaderCheck      = MwFim_GroupHeaderCheck_patch;
    MwFim_FileRead              = MwFim_FileRead_patch;
    MwFim_FileWrite             = MwFim_FileWrite_patch;
    MwFim_FileWriteDefault      = MwFim_FileWriteDefault_patch;
    MwFim_FileDelete            = MwFim_FileDelete_patch;
}

