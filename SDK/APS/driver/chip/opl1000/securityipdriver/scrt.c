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

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "cmsis_os.h"
#include "opulinks.h"
#include "msg.h"
#include "scrt.h"
#include "ipc.h"
#include "scrt_cmd.h"
#include "hal_system.h"


// idx: T_ScrtMbIdx
#define SCRT_BASE_ADDR(idx)             (SCRT0_BASE_ADDR + ((idx & 0x03) << 10))

#define SCRT_CTRL_WRITE_MSK(idx)        (0x01 << (idx * 4))
#define SCRT_CTRL_READ_MSK(idx)         (0x02 << (idx * 4))
#define SCRT_CTRL_LINK_MSK(idx)         (0x04 << (idx * 4))
#define SCRT_CTRL_UNLINK_MSK(idx)       (0x08 << (idx * 4))
#define SCRT_STATUS_WRITE_MSK           SCRT_CTRL_WRITE_MSK
#define SCRT_STATUS_READ_MSK            SCRT_CTRL_READ_MSK
#define SCRT_STATUS_LINK_MSK            SCRT_CTRL_LINK_MSK
#define SCRT_STATUS_AVAIL_MSK           SCRT_CTRL_UNLINK_MSK

#define SCRT_STAT_CTRL_ADDR             (SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_MAILBOX_STAT)
#define SCRT_OTP_STATUS_ADDR            (0x30002400)

#define SCRT_ID                         0x4F5A3647
#define SCRT_PRIV_KEY_LEN               32
#define SCRT_PUB_KEY_VEC_LEN            32
#define SCRT_KEY_PARAM_FMT_LEN          224
#define SCRT_PRIV_KEY_FMT_LEN           36
#define SCRT_PUB_KEY_FMT_LEN            72
#define SCRT_SHARED_SRCRET_LEN          32

#define SCRT_WAIT_RES_SEM               osWaitForever
#define SCRT_WAIT_ECDH_SEM              osWaitForever

#define SCRT_WAIT_RSP_CNT               (20000000)


//#define SCRT_ACCESS_SEM

#ifdef SCRT_ACCESS_SEM
    #define SCRT_WAIT_ACCESS_SEM        (2000) // ms
#else
    #define SCRT_WAIT_RES               10 // ms
    #define SCRT_WAIT_RES_MAX_CNT       200
#endif

#define SCRT_MALLOC                 malloc
#define SCRT_FREE                   free

#define SCRT_DEBUG

#ifdef SCRT_DEBUG
    #define SCRT_LOGI(...)
    //#define SCRT_LOGI(args...)      tracer_log(LOG_LOW_LEVEL, args)
    #define SCRT_LOGE(args...)      tracer_log(LOG_HIGH_LEVEL, args)
#else
    #define SCRT_LOGI(...)
    #define SCRT_LOGE(...)
#endif

//#define SCRT_CHECK

#ifdef SCRT_CHECK
    #define SCRT_ASSERT(a)    \
    if(!(a))    \
    {     \
        tracer_drct_printf("SCRT_ASSERT FAILED '" #a "' %s %s:%d\n", __func__, __FILE__, __LINE__);   \
        while(1);    \
    }
#else
    #define SCRT_ASSERT(...)
#endif

#define SCRT_IP_CLK_MSK     (0x00000001)
#define SCRT_OTP_CLK_MSK    (0x00000002)


typedef enum
{
    SCRT_MB_IDX_0 = 0,
    SCRT_MB_IDX_1,
    SCRT_MB_IDX_2,
    SCRT_MB_IDX_3,

    SCRT_MB_IDX_MAX
} T_ScrtMbIdx;

typedef enum
{
    SCRT_TOKEN_ID_RESET = 0xE000,
    SCRT_TOKEN_ID_TRNG_CFG = 0xE010,
    SCRT_TOKEN_ID_ASSET_LOAD = 0xE020,
    SCRT_TOKEN_ID_ASSET_CREATE_KEY_PARAM = 0xE030,
    SCRT_TOKEN_ID_ASSET_CREATE_PRIV_KEY = 0xE040,
    SCRT_TOKEN_ID_ASSET_CREATE_PUB_KEY = 0xE050,
    SCRT_TOKEN_ID_ASSET_CREATE_SHARED_SECRET = 0xE060,
    SCRT_TOKEN_ID_SHARED_SECRET_GEN = 0xE070,
    SCRT_TOKEN_ID_PUB_DATA_READ = 0xE080,
    SCRT_TOKEN_ID_ASSET_DELETE = 0xE090,
    SCRT_TOKEN_ID_KEY_PAIR_GEN = 0xE0A0,
    SCRT_TOKEN_ID_AES_CCM_ENCRYPT = 0xE0B0,
    SCRT_TOKEN_ID_AES_CCM_DECRYPT = 0xE0C0,
    SCRT_TOKEN_ID_HMAC_SHA_1 = 0xE0D0,
    SCRT_TOKEN_ID_AES_ECB = 0xE0E0,
    SCRT_TOKEN_ID_AES_CMAC = 0xE0F0,
    SCRT_TOKEN_ID_SHA = 0xE100,

    SCRT_TOKEN_ID_MAX = 0xEFFF
} T_ScrtTokenId;

typedef struct
{
    uint8_t u8Used;
    uint8_t u8MbIdx;
} T_ScrtRes;


// internal
RET_DATA nl_scrt_common_fp_t scrt_param_init;
RET_DATA nl_scrt_common_fp_t scrt_mb_init;
RET_DATA scrt_cmd_fp_t scrt_trng_init;
RET_DATA scrt_cmd_fp_t scrt_eng_init;
RET_DATA scrt_status_chk_fp_t scrt_status_chk;
RET_DATA scrt_asset_create_fp_t scrt_ecdh_key_param_init;
RET_DATA scrt_asset_create_fp_t scrt_ecdh_priv_key_asset_create;
RET_DATA scrt_asset_create_fp_t scrt_ecdh_pub_key_asset_create;
RET_DATA scrt_asset_create_fp_t scrt_ecdh_shared_secret_asset_create;
RET_DATA scrt_asset_delete_fp_t scrt_asset_delete;
RET_DATA scrt_asset_load_fp_t scrt_asset_load;
RET_DATA scrt_asset_get_fp_t scrt_asset_get;
//RET_DATA scrt_cmd_fp_t scrt_reset;
//RET_DATA nl_scrt_common_fp_t scrt_internal_reset;
RET_DATA scrt_cmd_fp_t scrt_link;

#ifdef SCRT_ENABLE_UNLINK
RET_DATA scrt_cmd_fp_t scrt_unlink;
#endif

#ifdef SCRT_ACCESS_SEM
RET_DATA nl_scrt_common_fp_t scrt_access_lock;
RET_DATA nl_scrt_common_fp_t scrt_access_unlock;
#endif

RET_DATA nl_scrt_common_fp_t scrt_res_lock;
RET_DATA nl_scrt_common_fp_t scrt_res_unlock;
RET_DATA nl_scrt_common_fp_t scrt_ecdh_lock;
RET_DATA nl_scrt_common_fp_t scrt_ecdh_unlock;
RET_DATA nl_scrt_common_fp_t scrt_sem_create;
RET_DATA scrt_res_alloc_fp_t scrt_res_alloc;
RET_DATA scrt_res_free_fp_t scrt_res_free;
RET_DATA scrt_clk_enable_fp_t scrt_clk_enable;

// external
RET_DATA nl_scrt_common_fp_t nl_scrt_Init;
RET_DATA nl_aes_ccm_fp_t nl_aes_ccm;
RET_DATA nl_scrt_aes_ccm_fp_t nl_scrt_aes_ccm;
RET_DATA nl_hmac_sha_1_fp_t nl_hmac_sha_1;
RET_DATA nl_scrt_aes_ecb_fp_t nl_scrt_aes_ecb;
RET_DATA nl_scrt_common_fp_t nl_scrt_otp_status_get;
RET_DATA nl_scrt_ecdh_key_pair_gen_fp_t nl_scrt_ecdh_key_pair_gen;
RET_DATA nl_scrt_ecdh_dhkey_gen_fp_t nl_scrt_ecdh_dhkey_gen;
RET_DATA nl_scrt_key_delete_fp_t nl_scrt_key_delete;
RET_DATA nl_scrt_aes_cmac_fp_t nl_scrt_aes_cmac;
//RET_DATA nl_scrt_hmac_sha_1_step_fp_t nl_scrt_hmac_sha_1_step;
RET_DATA nl_scrt_sha_fp_t nl_scrt_sha;

T_ScrtRes g_tScrtRes[SCRT_MB_IDX_MAX] = {0};
osSemaphoreId g_tScrtResSem = NULL;
osSemaphoreId g_tScrtEcdhSem = NULL;

#ifdef SCRT_ACCESS_SEM
osSemaphoreId g_tScrtAccessSem = NULL;
#endif

//uint8_t g_u8ScrtOtpReady = 0;
//uint32_t g_u32ScrtKeyParamAsId = 0;
uint32_t g_u32ScrtWaitResCnt = 0;
uint32_t g_u32ScrtWaitRspCnt = 0;

uint32_t g_u32aScrtKeyParamFmt[] __attribute__((aligned(16))) = 
{
    0x07000100, 
    // Curve_P
    0xffffffff, 0xffffffff, 0xffffffff, 0x00000000, 
    0x00000000, 0x00000000, 0x00000001, 0xffffffff, 

    0x07010100,
    0xfffffffc, 0xffffffff, 0xffffffff, 0x00000000, 
    0x00000000, 0x00000000, 0x00000001, 0xffffffff, 

    0x07020100,
    // Curve_B
    0x27d2604b, 0x3bce3c3e, 0xcc53b0f6, 0x651d06b0, 
    0x769886bc, 0xb3ebbd55, 0xaa3a93e7, 0x5ac635d8, 

    0x07030100,
    // Curve_N
    0xfc632551, 0xf3b9cac2, 0xa7179e84, 0xbce6faad,
    0xffffffff, 0xffffffff, 0x00000000, 0xffffffff,

    0x07040100, 
    // Curve_G1
    0xd898c296, 0xf4a13945, 0x2deb33a0, 0x77037d81, 
    0x63a440f2, 0xf8bce6e5, 0xe12c4247, 0x6b17d1f2, 

    0x07050100,
    // Curve_G2
    0x37bf51f5, 0xcbb64068, 0x6b315ece, 0x2bce3357, 
    0x7c0f9e16, 0x8ee7eb4a, 0xfe1a7f9b, 0x4fe342e2, 

    0x07060001, 
    0x00000001
};


int scrt_status_chk_impl(uint32_t u32Mask, uint32_t u32Value)
{
    int iRet = -1;
    volatile uint32_t *pu32Status = (uint32_t *)SCRT_STAT_CTRL_ADDR;
    uint32_t u32Cnt = 0;

    while(u32Cnt < SCRT_WAIT_RSP_CNT)
    {
        if((*pu32Status & u32Mask) == u32Value)
        {
            iRet = 0;
            break;
        }

        ++u32Cnt;
    }

    if(u32Cnt > g_u32ScrtWaitRspCnt)
    {
        g_u32ScrtWaitRspCnt = u32Cnt;
    }

    return iRet;
}

int scrt_ecdh_priv_key_asset_create_impl(uint8_t u8Idx, uint32_t *pu32PrivKeyAsId)
{
    int iRet = -1;
    volatile uint32_t *u32aBase = (uint32_t *)SCRT_BASE_ADDR(u8Idx);
    volatile uint32_t *u32aStatus = (uint32_t *)SCRT_STAT_CTRL_ADDR;
    uint16_t u16TokenId = SCRT_TOKEN_ID_ASSET_CREATE_PRIV_KEY + u8Idx;
    uint32_t u32aOutput[2] = {0};

    // Asset Create (for private key): start
    u32aBase[0] = 0x17000000 | u16TokenId;
    u32aBase[1] = SCRT_ID;
    u32aBase[2] = 0x00000000;
    u32aBase[3] = 0x00004000;
    u32aBase[4] = 0x00000024;
    u32aBase[5] = 0x00000000;
    u32aBase[6] = 0x00000000;

    *u32aStatus = SCRT_CTRL_WRITE_MSK(u8Idx);
    // Asset Create (for private key): end

    // write done and ready to read
    if(scrt_status_chk(SCRT_STATUS_WRITE_MSK(u8Idx) | SCRT_STATUS_READ_MSK(u8Idx), 
                       SCRT_STATUS_READ_MSK(u8Idx)))
    {
        SCRT_LOGE("[%s %d] scrt_status_chk fail\n", __func__, __LINE__);

        #ifdef SCRT_CHECK
        SCRT_ASSERT(0);
        #else
        goto done;
        #endif
    }

    // Asset Create (for private key) output: start
    memcpy(u32aOutput, (void *)u32aBase, 8);

    *u32aStatus = SCRT_CTRL_READ_MSK(u8Idx);

    if(u32aOutput[0] != u16TokenId)
    {
        SCRT_LOGE("[%s %d] invalid output[%08X] for SCRT_TOKEN_ID_PRIV_KEY[%08X]\n", __func__, __LINE__, u32aOutput[0], u16TokenId);
        goto done;
    }

    *pu32PrivKeyAsId = u32aOutput[1];
    // Asset Create (for private key) output: end

    iRet = 0;

done:
    return iRet;
}

int scrt_ecdh_pub_key_asset_create_impl(uint8_t u8Idx, uint32_t *pu32PubKeyAsId)
{
    int iRet = -1;
    volatile uint32_t *u32aBase = (uint32_t *)SCRT_BASE_ADDR(u8Idx);
    volatile uint32_t *u32aStatus = (uint32_t *)SCRT_STAT_CTRL_ADDR;
    uint16_t u16TokenId = SCRT_TOKEN_ID_ASSET_CREATE_PUB_KEY + u8Idx;
    uint32_t u32aOutput[2] = {0};

    // Asset Create (for public key): start
    u32aBase[0] = 0x17000000 | u16TokenId;
    u32aBase[1] = SCRT_ID;
    u32aBase[2] = 0x80000000;
    u32aBase[3] = 0x00004000;
    u32aBase[4] = 0x00000048;
    u32aBase[5] = 0x00000000;
    u32aBase[6] = 0x00000000;

    *u32aStatus = SCRT_CTRL_WRITE_MSK(u8Idx);
    // Asset Create (for public key): end

    // write done and ready to read
    if(scrt_status_chk(SCRT_STATUS_WRITE_MSK(u8Idx) | SCRT_STATUS_READ_MSK(u8Idx), 
                       SCRT_STATUS_READ_MSK(u8Idx)))
    {
        SCRT_LOGE("[%s %d] scrt_status_chk fail\n", __func__, __LINE__);

        #ifdef SCRT_CHECK
        SCRT_ASSERT(0);
        #else
        goto done;
        #endif
    }

    // Asset Create (for public key) output: start
    memcpy(u32aOutput, (void *)u32aBase, 8);

    *u32aStatus = SCRT_CTRL_READ_MSK(u8Idx);

    if(u32aOutput[0] != u16TokenId)
    {
        SCRT_LOGE("[%s %d] invalid output[%08X] for SCRT_TOKEN_ID_PUB_KEY[%08X]\n", __func__, __LINE__, u32aOutput[0], u16TokenId);
        goto done;
    }

    *pu32PubKeyAsId = u32aOutput[1];
    // Asset Create (for public key) output: end

    iRet = 0;

done:
    return iRet;
}

int scrt_ecdh_shared_secret_asset_create_impl(uint8_t u8Idx, uint32_t *pu32SharedSecretAsId)
{
    int iRet = -1;
    volatile uint32_t *u32aBase = (uint32_t *)SCRT_BASE_ADDR(u8Idx);
    volatile uint32_t *u32aStatus = (uint32_t *)SCRT_STAT_CTRL_ADDR;
    uint16_t u16TokenId = SCRT_TOKEN_ID_ASSET_CREATE_SHARED_SECRET + u8Idx;
    uint32_t u32aOutput[2] = {0};

    // Asset Create (for shared secret): start
    u32aBase[0] = 0x17000000 | u16TokenId;
    u32aBase[1] = SCRT_ID;

    // PUBLIC-DATA
    u32aBase[2] = 0x00000000;
    u32aBase[3] = 0x10000000;

    // AES-CMAC
    //u32aBase[2] = 0x00000120;
    //u32aBase[3] = 0x00000008;

    // SHA-256
    //u32aBase[2] = 0x00000004;
    //u32aBase[3] = 0x00000008;

    u32aBase[4] = 0x00000020;
    u32aBase[5] = 0x00000000;
    u32aBase[6] = 0x00000000;

    *u32aStatus = SCRT_CTRL_WRITE_MSK(u8Idx);
    // Asset Create (for shared secret): end

    // write done and ready to read
    if(scrt_status_chk(SCRT_STATUS_WRITE_MSK(u8Idx) | SCRT_STATUS_READ_MSK(u8Idx), 
                       SCRT_STATUS_READ_MSK(u8Idx)))
    {
        SCRT_LOGE("[%s %d] scrt_status_chk fail\n", __func__, __LINE__);

        #ifdef SCRT_CHECK
        SCRT_ASSERT(0);
        #else
        goto done;
        #endif
    }

    // Asset Create (for shared secret) output: start
    memcpy(u32aOutput, (void *)u32aBase, 8);

    *u32aStatus = SCRT_CTRL_READ_MSK(u8Idx);

    if(u32aOutput[0] != u16TokenId)
    {
        SCRT_LOGE("[%s %d] invalid output[%08X] for SCRT_TOKEN_ID_SHARED_SECRET[%08X]\n", __func__, __LINE__, u32aOutput[0], u16TokenId);
        goto done;
    }

    *pu32SharedSecretAsId = u32aOutput[1];
    // Asset Create (for shared secret) output: end

    iRet = 0;

done:
    return iRet;
}

int scrt_asset_delete_impl(uint8_t u8Idx, uint32_t u32AsId)
{
    int iRet = -1;
    volatile uint32_t *u32aBase = (uint32_t *)SCRT_BASE_ADDR(u8Idx);
    volatile uint32_t *u32aStatus = (uint32_t *)SCRT_STAT_CTRL_ADDR;
    uint32_t u32Output = 0;
    uint16_t u16TokenId = SCRT_TOKEN_ID_ASSET_DELETE + u8Idx;

    SCRT_LOGI("[%s %d] u32AsId[%#x]\n", __func__, __LINE__, u32AsId);

    if(!u32AsId)
    {
        iRet = 0;
        goto done;
    }

    // Asset Delete: start
    u32aBase[0] = 0x37000000 | u16TokenId;
    u32aBase[1] = SCRT_ID;
    u32aBase[2] = u32AsId;

    *u32aStatus = SCRT_CTRL_WRITE_MSK(u8Idx);
    // Asset Delete: end

    // write done and ready to read
    if(scrt_status_chk(SCRT_STATUS_WRITE_MSK(u8Idx) | SCRT_STATUS_READ_MSK(u8Idx), 
                       SCRT_STATUS_READ_MSK(u8Idx)))
    {
        SCRT_LOGE("[%s %d] scrt_status_chk fail\n", __func__, __LINE__);

        #ifdef SCRT_CHECK
        SCRT_ASSERT(0);
        #else
        goto done;
        #endif
    }

    // Asset Delete output: start
    u32Output = u32aBase[0];

    *u32aStatus = SCRT_CTRL_READ_MSK(u8Idx);

    if(u32Output != u16TokenId)
    {
        SCRT_LOGE("[%s %d] invalid output[%08X] for SCRT_TOKEN_ID_ASSET_DELETE[%08X]\n", __func__, __LINE__, u32Output, u16TokenId);
        goto done;
    }
    // Asset Delete output: end

    iRet = 0;

done:
    return iRet;
}

int scrt_asset_load_impl(uint8_t u8Idx, uint32_t u32AsId, uint8_t *u8aAddr, uint16_t u16Len)
{
    int iRet = -1;
    volatile uint32_t *u32aBase = (uint32_t *)SCRT_BASE_ADDR(u8Idx);
    volatile uint32_t *u32aStatus = (uint32_t *)SCRT_STAT_CTRL_ADDR;
    uint32_t u32Output = 0;
    uint16_t u16TokenId = SCRT_TOKEN_ID_ASSET_LOAD + u8Idx;

    // Asset Load: start
    u32aBase[0] = 0x27000000 | u16TokenId;
    u32aBase[1] = SCRT_ID;
    u32aBase[2] = u32AsId;
    u32aBase[3] = 0x08000000 | (u16Len & 0x3FF);
    u32aBase[4] = (uint32_t)u8aAddr;
    u32aBase[5] = 0x00000000;
    u32aBase[6] = 0x00000000;
    u32aBase[7] = 0x00000000;
    u32aBase[8] = 0x00000000;
    u32aBase[9] = 0x00000000;

    *u32aStatus = SCRT_CTRL_WRITE_MSK(u8Idx);
    // Asset Load: end

    // write done and ready to read
    if(scrt_status_chk(SCRT_STATUS_WRITE_MSK(u8Idx) | SCRT_STATUS_READ_MSK(u8Idx), 
                       SCRT_STATUS_READ_MSK(u8Idx)))
    {
        SCRT_LOGE("[%s %d] scrt_status_chk fail\n", __func__, __LINE__);

        #ifdef SCRT_CHECK
        SCRT_ASSERT(0);
        #else
        goto done;
        #endif
    }

    // Asset Load output: start
    u32Output = u32aBase[0];

    *u32aStatus = SCRT_CTRL_READ_MSK(u8Idx);

    if(u32Output != u16TokenId)
    {
        SCRT_LOGE("[%s %d] invalid output[%08X] for u16TokenId[%#x]\n", __func__, __LINE__, u32Output, u16TokenId);
        goto done;
    }
    // Asset Load output: end

    iRet = 0;

done:
    return iRet;
}

int scrt_asset_get_impl(uint8_t u8Idx, uint32_t u32AsId, uint8_t *u8aBuf, uint32_t u32BufLen, uint32_t u32OutputLen)
{
    int iRet = -1;
    volatile uint32_t *u32aBase = (uint32_t *)SCRT_BASE_ADDR(u8Idx);
    volatile uint32_t *u32aStatus = (uint32_t *)SCRT_STAT_CTRL_ADDR;
    uint32_t u32Output = 0;
    uint32_t u32OutputTokenId = 0;
    uint16_t u16TokenId = SCRT_TOKEN_ID_PUB_DATA_READ + u8Idx;

    // Public Data Read: start
    u32aBase[0] = 0x47040000 | u16TokenId;
    u32aBase[1] = SCRT_ID;
    u32aBase[2] = u32AsId;
    u32aBase[3] = u32BufLen & 0x000003FF;
    u32aBase[4] = (uint32_t)u8aBuf;
    u32aBase[5] = 0x00000000;

    *u32aStatus = SCRT_CTRL_WRITE_MSK(u8Idx);
    // Public Data Read: end

    // write done and ready to read
    if(scrt_status_chk(SCRT_STATUS_WRITE_MSK(u8Idx) | SCRT_STATUS_READ_MSK(u8Idx), 
                       SCRT_STATUS_READ_MSK(u8Idx)))
    {
        SCRT_LOGE("[%s %d] scrt_status_chk fail\n", __func__, __LINE__);

        #ifdef SCRT_CHECK
        SCRT_ASSERT(0);
        #else
        goto done;
        #endif
    }

    // Public Data Read output: start
    u32Output = u32aBase[0];

    *u32aStatus = SCRT_CTRL_READ_MSK(u8Idx);

    if(u32Output != u16TokenId)
    {
        SCRT_LOGE("[%s %d] invalid output[%08X] for SCRT_TOKEN_ID_PUB_DATA_READ[%08X]\n", __func__, __LINE__, u32Output, u16TokenId);
        goto done;
    }

    memcpy(&u32OutputTokenId, &(u8aBuf[u32OutputLen]), 4);

    SCRT_LOGI("[%s %d] output token id[%08X] SCRT_TOKEN_ID_PUB_DATA_READ[%08X]\n", __func__, __LINE__, 
              u32OutputTokenId, u16TokenId);

    if(u32OutputTokenId != u16TokenId)
    {
        SCRT_LOGE("[%s %d] output token id[%08X] != SCRT_TOKEN_ID_PUB_DATA_READ[%08X]\n", __func__, __LINE__, 
                  u32OutputTokenId, u16TokenId);
        goto done;
    }
    // Public Data Read output: end

    iRet = 0;

done:
    return iRet;
}
/*
int scrt_reset_impl(uint8_t u8Idx)
{
    int iRet = -1;
    volatile uint32_t *u32aBase = (uint32_t *)SCRT_BASE_ADDR(u8Idx);
    volatile uint32_t *u32aStatus = (uint32_t *)SCRT_STAT_CTRL_ADDR;
    uint32_t u32Output = 0;
    uint16_t u16TokenId = SCRT_TOKEN_ID_RESET + u8Idx;

    // Reset: start
    u32aBase[0] = 0x2F020000 | u16TokenId;
    u32aBase[1] = SCRT_ID;

    *u32aStatus = SCRT_CTRL_WRITE_MSK(u8Idx);
    // Reset: end

    // write done and ready to read
    if(scrt_status_chk(SCRT_STATUS_WRITE_MSK(u8Idx) | SCRT_STATUS_READ_MSK(u8Idx), 
                       SCRT_STATUS_READ_MSK(u8Idx)))
    {
        SCRT_LOGE("[%s %d] scrt_status_chk fail\n", __func__, __LINE__);

        #ifdef SCRT_CHECK
        SCRT_ASSERT(0);
        #else
        goto done;
        #endif
    }

    // Reset output: start
    u32Output = u32aBase[0];

    *u32aStatus = SCRT_CTRL_READ_MSK(u8Idx);

    if(u32Output != u16TokenId)
    {
        SCRT_LOGE("[%s %d] invalid output[%08X] for SCRT_TOKEN_ID_RESET[%08X]\n", __func__, __LINE__, u32Output, u16TokenId);
        goto done;
    }
    // Reset output: end

    iRet = 0;

done:

    return iRet;
}

int scrt_internal_reset_impl(void)
{
    int iRet = -1;
    volatile uint32_t *u32Status = (uint32_t *)SCRT_STAT_CTRL_ADDR;
    uint32_t u32Mask = 0;
    uint8_t i = 0;

    for(i = 0; i < SCRT_MB_IDX_MAX; i++)
    {
        *u32Status = SCRT_CTRL_LINK_MSK(i);
        u32Mask = SCRT_STATUS_LINK_MSK(i);

        if(scrt_status_chk(u32Mask, SCRT_STATUS_LINK_MSK(i)))
        {
            SCRT_LOGE("[%s %d] scrt_status_chk fail\n", __func__, __LINE__);
        }

        *u32Status = SCRT_CTRL_READ_MSK(i);

        u32Mask = SCRT_STATUS_WRITE_MSK(i) | SCRT_STATUS_READ_MSK (i) | SCRT_STATUS_LINK_MSK(i) | SCRT_STATUS_AVAIL_MSK(i);

        if((*u32Status & u32Mask) == SCRT_STATUS_LINK_MSK(i))
        {
            if(!scrt_reset(i))
            {
                SCRT_LOGI("[%s %d] scrt_reset success, mb_id[%d]\n", __func__, __LINE__, i);
                iRet = 0;
                break;
            }
            else
            {
                SCRT_LOGE("[%s %d] scrt_reset fail, mb_id[%d]\n", __func__, __LINE__, i);
            }
        }
    }

    if(!iRet)
    {
        for(i = 0; i < SCRT_MB_IDX_MAX; i++)
        {
            *u32Status = SCRT_CTRL_UNLINK_MSK(i);
        }
    }
    
    return iRet;
}
*/
int scrt_link_impl(uint8_t u8Idx)
{
    int iRet = -1;
    volatile uint32_t *u32Status = (uint32_t *)SCRT_STAT_CTRL_ADDR;
    uint32_t u32Mask = SCRT_STATUS_WRITE_MSK(u8Idx) | SCRT_STATUS_READ_MSK (u8Idx) | SCRT_STATUS_LINK_MSK(u8Idx) | SCRT_STATUS_AVAIL_MSK(u8Idx);

    #ifdef SCRT_ENABLE_UNLINK
    if((*u32Status & u32Mask) != SCRT_STATUS_AVAIL_MSK(u8Idx))
    {
        SCRT_LOGE("[%s %d] idx[%d] s[%08X] not ready to link\n", __func__, __LINE__, u8Idx, *u32Status);
        //goto done;
    }
    #endif

    *u32Status = SCRT_CTRL_LINK_MSK(u8Idx);

    if(scrt_status_chk(u32Mask, SCRT_STATUS_LINK_MSK(u8Idx)))
    {
        SCRT_LOGE("[%s %d] scrt_status_chk fail\n", __func__, __LINE__);
        goto done;
    }

    iRet = 0;

done:
    return iRet;
}

#ifdef SCRT_ENABLE_UNLINK
int scrt_unlink_impl(uint8_t u8Idx)
{
    int iRet = -1;
    volatile uint32_t *u32Status = (uint32_t *)SCRT_STAT_CTRL_ADDR;
    uint32_t u32Mask = SCRT_STATUS_LINK_MSK(u8Idx) | SCRT_STATUS_AVAIL_MSK(u8Idx);

    if((*u32Status & u32Mask) == SCRT_STATUS_AVAIL_MSK(u8Idx))
    {
        SCRT_LOGE("[%s %d] idx[%d] s[%08X] already available\n", __func__, __LINE__, u8Idx, *u32Status);
        iRet = 0;
        goto done;
    }

    *u32Status = SCRT_CTRL_UNLINK_MSK(u8Idx);

    if(scrt_status_chk(u32Mask, SCRT_STATUS_AVAIL_MSK(u8Idx)))
    {
        SCRT_LOGE("[%s %d] scrt_status_chk fail\n", __func__, __LINE__);
        goto done;
    }

    iRet = 0;

done:
    return iRet;
}
#endif

int scrt_mb_init_impl(void)
{
    int iRet = -1;
    volatile uint32_t *u32Status = (uint32_t *)SCRT_STAT_CTRL_ADDR;
    uint8_t u8AvailCnt = 0;
    uint8_t i = 0;

    //for(i = 0; i < SCRT_MB_IDX_MAX; i++)
    for(i = SCRT_MB_IDX_0; i < SCRT_MB_IDX_MAX; i++)
    {
        uint32_t u32Mask = SCRT_STATUS_WRITE_MSK(i) | SCRT_STATUS_READ_MSK (i) | SCRT_STATUS_LINK_MSK(i) | SCRT_STATUS_AVAIL_MSK(i);

        if((*u32Status & u32Mask) != SCRT_STATUS_AVAIL_MSK(i))
        {
            /*
            if(scrt_internal_reset())
            {
                SCRT_LOGE("[%s %d] scrt_internal_reset fail, please reset device\n", __func__, __LINE__);

                #ifdef SCRT_CHECK
                SCRT_ASSERT(0);
                #else
                goto done;
                #endif
            }

            break;
            */
            if(!Hal_Sys_ApsModuleRst(APS_RST_SCRT))
            {
                iRet = 0;
                break;
            }

            SCRT_LOGE("[%s %d] Hal_Sys_ApsModuleRst(ASP_RST_SCRT) fail\n", __func__, __LINE__);
        }
        else
        {
            ++u8AvailCnt;
        }
    }

    if(u8AvailCnt == SCRT_MB_IDX_MAX)
    {
        iRet = 0;
    }

    #ifdef SCRT_PRE_LINK
    //for(i = 0; i < SCRT_MB_IDX_MAX; i++)
    for(i = SCRT_MB_IDX_1; i < SCRT_MB_IDX_MAX; i++)
    {
        if(scrt_link(i))
        {
            SCRT_LOGE("[%s %d] scrt_link fail, please reset device\n", __func__, __LINE__);

            #ifdef SCRT_CHECK
            SCRT_ASSERT(0);
            #else
            //goto done;
            iRet = -1;
            #endif
        }
    }
    #endif

/*
    iRet = 0;

#ifdef SCRT_CHECK
#else
done:
#endif
*/
    return iRet;
}

void scrt_clk_enable_impl(uint8_t u8Enable, uint32_t u32Msk)
{
    if(u32Msk & SCRT_IP_CLK_MSK)
    {
        Hal_Sys_ApsClkEn(u8Enable, APS_CLK_SCRT);
    }

    if(u32Msk & SCRT_OTP_CLK_MSK)
    {
        Hal_Sys_ApsClkEn(u8Enable, APS_CLK_OTP);
    }

    return;
}

#ifdef SCRT_ACCESS_SEM
int scrt_access_lock_impl(void)
{
    int iRet = -1;
    int iStatus = 0;

    if(!g_tScrtAccessSem)
    {
        SCRT_LOGE("[%s %d] sem is null\n", __func__, __LINE__);
        goto done;
    }

    iStatus = osSemaphoreWait(g_tScrtAccessSem, SCRT_WAIT_ACCESS_SEM);

    if(iStatus != osOK)
    {
        SCRT_LOGE("[%s %d] osSemaphoreWait fail, status[%d]\n", __func__, __LINE__, iStatus);
        goto done;
    }

    iRet = 0;

done:
    return iRet;
}

int scrt_access_unlock_impl(void)
{
    int iRet = -1;

    if(!g_tScrtAccessSem)
    {
        SCRT_LOGE("[%s %d] sem is null\n", __func__, __LINE__);
        goto done;
    }
    
    if(osSemaphoreRelease(g_tScrtAccessSem) != osOK)
    {
        SCRT_LOGE("[%s %d] osSemaphoreWait fail\n", __func__, __LINE__);
        goto done;
    }

    iRet = 0;

done:
    return iRet;
}
#endif //#ifdef SCRT_ACCESS_SEM

int scrt_res_lock_impl(void)
{
    int iRet = -1;

    if(!g_tScrtResSem)
    {
        SCRT_LOGE("[%s %d] sem is null\n", __func__, __LINE__);
        goto done;
    }
    
    if(osSemaphoreWait(g_tScrtResSem, SCRT_WAIT_RES_SEM) != osOK)
    {
        SCRT_LOGE("[%s %d] osSemaphoreWait fail\n", __func__, __LINE__);
        goto done;
    }

    iRet = 0;

done:
    return iRet;
}

int scrt_res_unlock_impl(void)
{
    int iRet = -1;

    if(!g_tScrtResSem)
    {
        SCRT_LOGE("[%s %d] sem is null\n", __func__, __LINE__);
        goto done;
    }
    
    if(osSemaphoreRelease(g_tScrtResSem) != osOK)
    {
        SCRT_LOGE("[%s %d] osSemaphoreWait fail\n", __func__, __LINE__);
        goto done;
    }

    iRet = 0;

done:
    return iRet;
}

int scrt_ecdh_lock_impl(void)
{
    int iRet = -1;

    if(!g_tScrtEcdhSem)
    {
        SCRT_LOGE("[%s %d] sem is null\n", __func__, __LINE__);
        goto done;
    }
    
    if(osSemaphoreWait(g_tScrtEcdhSem, SCRT_WAIT_ECDH_SEM) != osOK)
    {
        SCRT_LOGE("[%s %d] osSemaphoreWait fail\n", __func__, __LINE__);
        goto done;
    }

    iRet = 0;

done:
    return iRet;
}

int scrt_ecdh_unlock_impl(void)
{
    int iRet = -1;

    if(!g_tScrtEcdhSem)
    {
        SCRT_LOGE("[%s %d] sem is null\n", __func__, __LINE__);
        goto done;
    }
    
    if(osSemaphoreRelease(g_tScrtEcdhSem) != osOK)
    {
        SCRT_LOGE("[%s %d] osSemaphoreWait fail\n", __func__, __LINE__);
        goto done;
    }

    iRet = 0;

done:
    return iRet;
}

int scrt_sem_create_impl(void)
{
    int iRet = -1;
    osSemaphoreDef_t tSemDef = {0};

    #ifdef SCRT_ACCESS_SEM
    if(g_tScrtAccessSem == NULL)
    {
        g_tScrtAccessSem = osSemaphoreCreate(&tSemDef, SCRT_MB_IDX_MAX);
    
        if(g_tScrtAccessSem == NULL)
        {
            SCRT_LOGE("[%s %d] osSemaphoreCreate fail\n", __func__, __LINE__);
        }
    }
    #endif

    if(g_tScrtResSem == NULL)
    {
        g_tScrtResSem = osSemaphoreCreate(&tSemDef, 1);
    
        if(g_tScrtResSem == NULL)
        {
            SCRT_LOGE("[%s %d] osSemaphoreCreate fail\n", __func__, __LINE__);
        }
    }

    if(g_tScrtEcdhSem == NULL)
    {
        g_tScrtEcdhSem = osSemaphoreCreate(&tSemDef, 1);
    
        if(g_tScrtEcdhSem == NULL)
        {
            SCRT_LOGE("[%s %d] osSemaphoreCreate fail\n", __func__, __LINE__);
        }
    }

    iRet = 0;

    return iRet;
}

#ifdef SCRT_ACCESS_SEM
uint8_t scrt_res_alloc_impl(void)
{
    uint8_t u8Idx = SCRT_MB_IDX_MAX;
    uint8_t u8ResDone = 0;
    uint8_t i = 0;

    #ifdef SCRT_ACCESS_SEM
    if(scrt_access_lock())
    {
        SCRT_LOGE("[%s %d] scrt_access_lock fail\n", __func__, __LINE__);
        goto done;
    }
    #endif

    if(scrt_res_lock())
    {
        SCRT_LOGE("[%s %d] scrt_res_lock fail\n", __func__, __LINE__);
        goto done;
    }

    u8ResDone = 1;

    // reserve SCRT_MB_IDX_0 for M0 usage
    for(i = SCRT_MB_IDX_1; i < SCRT_MB_IDX_MAX; i++)
    {
        if(g_tScrtRes[i].u8Used == 0)
        {
            scrt_clk_enable(1, (SCRT_OTP_CLK_MSK | SCRT_IP_CLK_MSK));

            g_tScrtRes[i].u8Used = 1;
            u8Idx = i;
            break;
        }
    }

    SCRT_ASSERT(u8Idx <= SCRT_MB_IDX_MAX);

done:
    if(u8ResDone)
    {
        if(scrt_res_unlock())
        {
            SCRT_LOGE("[%s %d] scrt_res_unlock fail\n", __func__, __LINE__);

            SCRT_ASSERT(0);
        }
    }

    return u8Idx;
}
#else
uint8_t scrt_res_alloc_impl(void)
{
    uint8_t u8Idx = SCRT_MB_IDX_MAX;
    uint8_t i = 0;
    uint32_t u32Cnt = 0;

    while(u32Cnt < SCRT_WAIT_RES_MAX_CNT)
    {
        if(scrt_res_lock())
        {
            SCRT_LOGE("[%s %d] scrt_res_lock fail\n", __func__, __LINE__);
            break;
        }

        // reserve SCRT_MB_IDX_0 for M0 usage
        for(i = SCRT_MB_IDX_1; i < SCRT_MB_IDX_MAX; i++)
        {
            if(g_tScrtRes[i].u8Used == 0)
            {
                scrt_clk_enable(1, (SCRT_OTP_CLK_MSK | SCRT_IP_CLK_MSK));

                g_tScrtRes[i].u8Used = 1;
                u8Idx = i;
                break;
            }
        }

        if(scrt_res_unlock())
        {
            SCRT_LOGE("[%s %d] scrt_res_unlock fail\n", __func__, __LINE__);
            break;
        }

        if(u8Idx != SCRT_MB_IDX_MAX)
        {
            break;
        }

        ++u32Cnt;
        osDelay(SCRT_WAIT_RES);
    }

    if(u32Cnt >g_u32ScrtWaitResCnt)
    {
        g_u32ScrtWaitResCnt = u32Cnt;
    }

    return u8Idx;
}
#endif //#ifdef SCRT_ACCESS_SEM

void scrt_res_free_impl(uint8_t u8Idx)
{
    //uint8_t u8ResDone = 0;

    if(u8Idx >= SCRT_MB_IDX_MAX)
    {
        SCRT_LOGI("[%s %d] invalid res_id[%d]\n", __func__, __LINE__, u8Idx);
        goto done;
    }
    /*
    if(scrt_res_lock())
    {
        SCRT_LOGE("[%s %d] scrt_res_lock fail\n", __func__, __LINE__);
        goto done;
    }
    
    u8ResDone = 1;
    */
    g_tScrtRes[u8Idx].u8Used = 0;

done:
    /*
    if(u8ResDone)
    {
        if(scrt_res_unlock())
        {
            SCRT_LOGE("[%s %d] scrt_res_unlock fail\n", __func__, __LINE__);

            SCRT_ASSERT(0);
        }
    }
    */
    #ifdef SCRT_ACCESS_SEM
    if(scrt_access_unlock())
    {
        SCRT_LOGE("[%s %d] scrt_access_unlock fail\n", __func__, __LINE__);

        SCRT_ASSERT(0);
    }
    #endif

    return;
}

int scrt_trng_init_impl(uint8_t u8Idx)
{
    int iRet = -1;
    volatile uint32_t *u32aBase = (uint32_t *)SCRT_BASE_ADDR(u8Idx);
    volatile uint32_t *u32aStatus = (uint32_t *)SCRT_STAT_CTRL_ADDR;
    uint32_t u32Output = 0;
    uint16_t u16TokenId = 0;

    SCRT_LOGI("[%s %d] u32aBase at[%08X]\n", __func__, __LINE__, u32aBase);

    if(!nl_scrt_otp_status_get())
    {
        SCRT_LOGE("[%s %d] OTP not ready\n", __func__, __LINE__);
        goto done;
    }

    u16TokenId = SCRT_TOKEN_ID_TRNG_CFG + u8Idx;

    // TRNG configure: start
    u32aBase[0] = 0x14000000 | u16TokenId;
    u32aBase[1] = SCRT_ID;
    u32aBase[2] = 0x00000001;
    u32aBase[3] = 0x00030101;

    *u32aStatus = SCRT_CTRL_WRITE_MSK(u8Idx);
    // TRNG configure: end

    // write done and ready to read
    if(scrt_status_chk(SCRT_STATUS_WRITE_MSK(u8Idx) | SCRT_STATUS_READ_MSK(u8Idx), 
                       SCRT_STATUS_READ_MSK(u8Idx)))
    {
        SCRT_LOGE("[%s %d] scrt_status_chk fail\n", __func__, __LINE__);

        #ifdef SCRT_CHECK
        SCRT_ASSERT(0);
        #else
        goto done;
        #endif
    }

    // TRNG configure output: start
    u32Output = u32aBase[0];

    *u32aStatus = SCRT_CTRL_READ_MSK(u8Idx);

    if(u32Output != u16TokenId)
    {
        SCRT_LOGE("[%s %d] invalid output[%08X] for SCRT_TOKEN_ID_TRNG_CFG[%08X]\n", __func__, __LINE__, u32Output, u16TokenId);
        goto done;
    }
    // TRNG configure output: end

    iRet = 0;

done:
    return iRet;
}

int scrt_ecdh_key_param_init_impl(uint8_t u8Idx, uint32_t *pu32KeyParamAsId)
{
    int iRet = -1;
    volatile uint32_t *u32aBase = (uint32_t *)SCRT_BASE_ADDR(u8Idx);
    volatile uint32_t *u32aStatus = (uint32_t *)SCRT_STAT_CTRL_ADDR;
    uint32_t u32aOutput[2] = {0};
    uint16_t u16TokenId = 0;

    SCRT_LOGI("[%s %d] u32aBase at[%08X]\n", __func__, __LINE__, u32aBase);

    u16TokenId = SCRT_TOKEN_ID_ASSET_CREATE_KEY_PARAM + u8Idx;

    // Asset Create (for key param): start
    u32aBase[0] = 0x17000000 | u16TokenId;
    u32aBase[1] = SCRT_ID;
    u32aBase[2] = 0x00000000;
    u32aBase[3] = 0x00008000;
    u32aBase[4] = 0x000000E0;
    u32aBase[5] = 0x00000000;
    u32aBase[6] = 0x00000000;

    *u32aStatus = SCRT_CTRL_WRITE_MSK(u8Idx);
    // Asset Create (for key param): end

    // write done and ready to read
    if(scrt_status_chk(SCRT_STATUS_WRITE_MSK(u8Idx) | SCRT_STATUS_READ_MSK(u8Idx), 
                       SCRT_STATUS_READ_MSK(u8Idx)))
    {
        SCRT_LOGE("[%s %d] scrt_status_chk fail\n", __func__, __LINE__);

        #ifdef SCRT_CHECK
        SCRT_ASSERT(0);
        #else
        goto done;
        #endif
    }

    // Asset Create (for key param) output: start
    memcpy(u32aOutput, (void *)u32aBase, 8);

    *u32aStatus = SCRT_CTRL_READ_MSK(u8Idx);

    if(u32aOutput[0] != u16TokenId)
    {
        SCRT_LOGE("[%s %d] invalid output[%08X] for SCRT_TOKEN_ID_PUB_KEY_PARAM\n", __func__, __LINE__, u32aOutput[0], u16TokenId);
        goto done;
    }

    *pu32KeyParamAsId = u32aOutput[1];
    // Asset Create (for key param) output: end

    SCRT_LOGI("[%s %d] load key param\n", __func__, __LINE__);
    
    if(scrt_asset_load(u8Idx, *pu32KeyParamAsId, (uint8_t *)g_u32aScrtKeyParamFmt, SCRT_KEY_PARAM_FMT_LEN))
    {
        SCRT_LOGE("[%s %d] scrt_asset_load fail for key param\n", __func__, __LINE__);
        goto done;
    }

    iRet = 0;

done:
    return iRet;
}

int scrt_eng_init_impl(uint8_t u8Idx)
{
    int iRet = -1;

    if(scrt_trng_init(u8Idx))
    {
        SCRT_LOGE("[%s %d] scrt_trng_init fail\n", __func__, __LINE__);
        goto done;
    }

    iRet = 0;

done:
    return iRet;
}

int scrt_param_init_impl(void)
{
    uint32_t i = 0;

    // init scrt resource
    for(i = 0; i < SCRT_MB_IDX_MAX; i++)
    {
        g_tScrtRes[i].u8Used = 0;
        g_tScrtRes[i].u8MbIdx = i;
    }

    //g_u8ScrtOtpReady = 0;
    //g_tScrtResSem = NULL;

    #ifdef SCRT_ACCESS_SEM
    //g_tScrtAccessSem = NULL;
    #endif
    
    //g_u32ScrtKeyParamAsId = 0;

    g_u32ScrtWaitResCnt = 0;
    g_u32ScrtWaitRspCnt = 0;
    return 0;
}

/*
 * nl_scrt_otp_status_get_impl - Get OTP status.
 *
 * @param [in] None
 *    N/A
 *
 * @return 1 success
 *    OTP ready
 *
 * @return 0 fail
 *    OTP not ready
 *
 */
int nl_scrt_otp_status_get_impl(void)
{
    int iRet = 0;
    volatile uint32_t *pu32OtpStatus = NULL;

    scrt_clk_enable(1, SCRT_OTP_CLK_MSK);

    pu32OtpStatus = (uint32_t *)SCRT_OTP_STATUS_ADDR;

    if(*pu32OtpStatus == 0)
    {
        SCRT_LOGI("[%s %d] OTP not ready\n", __func__, __LINE__);

        //scrt_clk_enable(0, SCRT_OTP_CLK_MSK);
        goto done;
    }

    iRet = 1;

done:
    return iRet;
}

/*
 * nl_scrt_ecdh_key_pair_gen_impl - Generate ECDH key pair.
 *
 * @param [in] pPubKey
 *    Public Key
 *
 * @param [out] u32aPrivKey
 *    Private Key
 *
 * @param [in/out] pu32PrivKeyId
 *    [in]: ID of previous private key
 *    [out]: ID of current private key
 *
 * @return 1 success
 *
 * @return 0 fail
 *
 */
int nl_scrt_ecdh_key_pair_gen_impl(void *pPubKey, uint32_t *u32aPrivKey, uint32_t *pu32PrivKeyId)
{
    int iRet = 0;
    volatile uint32_t *u32aBase = NULL;
    volatile uint32_t *u32aStatus = (uint32_t *)SCRT_STAT_CTRL_ADDR;
    uint32_t u32Output = 0;
    uint32_t u32OutputTokenId = 0;
    uint8_t *u8aPubKey = (uint8_t *)pPubKey;
    uint8_t u8ResId = SCRT_MB_IDX_MAX;
    uint32_t u32KeyParamAsId = 0;
    uint32_t u32PrivKeyAsId = 0;
    uint32_t u32PubKeyAsId = 0;
    uint16_t u16TokenId = 0;
    uint32_t u32OutputBufLen = 0;
    uint8_t *u8aOutputBuf = NULL;
    uint8_t *u8aAlignBuf = NULL;
    uint8_t u8EcdhLock = 0;

    #ifdef SCRT_ENABLE_UNLINK
    uint8_t u8Link = 0;
    #endif

    if(!nl_scrt_otp_status_get())
    {
        SCRT_LOGE("[%s %d] OTP not ready\n", __func__, __LINE__);
        goto done;
    }

    if((pPubKey == NULL) || (pu32PrivKeyId == NULL))
    {
        goto done;
    }

    if(scrt_ecdh_lock())
    {
        SCRT_LOGE("[%s %d] scrt_ecdh_lock fail\n", __func__, __LINE__);
        goto done;
    }

    u8EcdhLock = 1;

    u8ResId = scrt_res_alloc();

    if(u8ResId >= SCRT_MB_IDX_MAX)
    {
        SCRT_LOGE("[%s %d] scrt_res_alloc fail\n", __func__, __LINE__);
        goto done;
    }

    SCRT_LOGI("[%s %d] u8ResId[%d]\n", __func__, __LINE__, u8ResId);

    #ifdef SCRT_PRE_LINK
    #else
    // Link: start
    if(scrt_link(g_tScrtRes[u8ResId].u8MbIdx))
    {
        SCRT_LOGE("[%s %d] scrt_link fail\n", __func__, __LINE__);
        goto done;
    }

    #ifdef SCRT_ENABLE_UNLINK
    u8Link = 1;
    #endif
    // Link: end
    #endif

    if(scrt_eng_init(g_tScrtRes[u8ResId].u8MbIdx))
    {
        SCRT_LOGE("[%s %d] scrt_eng_init fail\n", __func__, __LINE__);
        goto done;
    }

    if(scrt_ecdh_key_param_init(g_tScrtRes[u8ResId].u8MbIdx, &u32KeyParamAsId))
    {
        SCRT_LOGE("[%s %d] scrt_ecdh_key_param_init fail\n", __func__, __LINE__);
        goto done;
    }

    if(*pu32PrivKeyId != 0)
    {
        if(scrt_asset_delete(g_tScrtRes[u8ResId].u8MbIdx, *pu32PrivKeyId))
        {
            SCRT_LOGE("[%s %d] scrt_asset_delete fail for private key\n", __func__, __LINE__);
        }
    }

    if(scrt_ecdh_priv_key_asset_create(g_tScrtRes[u8ResId].u8MbIdx, &u32PrivKeyAsId))
    {
        SCRT_LOGE("[%s %d] scrt_ecdh_priv_key_asset_create fail\n", __func__, __LINE__);
        goto done;
    }

    if(scrt_ecdh_pub_key_asset_create(g_tScrtRes[u8ResId].u8MbIdx, &u32PubKeyAsId))
    {
        SCRT_LOGE("[%s %d] scrt_ecdh_pub_key_asset_create fail\n", __func__, __LINE__);
        goto done;
    }

    u16TokenId = SCRT_TOKEN_ID_KEY_PAIR_GEN + g_tScrtRes[u8ResId].u8MbIdx;
    u32aBase = (volatile uint32_t *)SCRT_BASE_ADDR(g_tScrtRes[u8ResId].u8MbIdx);

    SCRT_LOGI("[%s %d] u32aBase at[%08X]\n", __func__, __LINE__, u32aBase);

    u32OutputBufLen = SCRT_PUB_KEY_FMT_LEN + 4 + 16;
    u8aOutputBuf = (uint8_t *)SCRT_MALLOC(u32OutputBufLen);

    if(!u8aOutputBuf)
    {
        SCRT_LOGE("[%s %d] SCRT_MALLOC(%u) fail\n", __func__, __LINE__, u32OutputBufLen);
        goto done;
    }

    u8aAlignBuf = (uint8_t *)IPC_ADDR_ALIGN(u8aOutputBuf, 16);
    
    // Public Key Token (ECDH/ECDSA/ElGamal ECC generate private and public key): start
    u32aBase[0] = 0x19040000 | u16TokenId;
    u32aBase[1] = SCRT_ID;
    u32aBase[2] = 0x08080015;
    u32aBase[3] = 0x00000000;
    u32aBase[4] = u32PrivKeyAsId;
    u32aBase[5] = u32KeyParamAsId;
    u32aBase[6] = u32PubKeyAsId;
    u32aBase[7] = (SCRT_PUB_KEY_FMT_LEN & 0x0FFF) << 16;
    u32aBase[8] = 0x00000000;
    u32aBase[9] = 0x00000000;
    u32aBase[10] = (uint32_t)u8aAlignBuf;
    u32aBase[11] = 0x00000000;
    u32aBase[12] = 0x00000000;

    SCRT_LOGI("[%s %d] param[%08X] priv[%08X] pub[%08X]\n", __func__, __LINE__, 
              u32KeyParamAsId, u32PrivKeyAsId, u32PubKeyAsId);

    *u32aStatus = SCRT_CTRL_WRITE_MSK(g_tScrtRes[u8ResId].u8MbIdx);
    // Public Key Token (ECDH/ECDSA/ElGamal ECC generate private and public key): end

    // write done and ready to read
    if(scrt_status_chk(SCRT_STATUS_WRITE_MSK(g_tScrtRes[u8ResId].u8MbIdx) | SCRT_STATUS_READ_MSK(g_tScrtRes[u8ResId].u8MbIdx), 
                       SCRT_STATUS_READ_MSK(g_tScrtRes[u8ResId].u8MbIdx)))
    {
        SCRT_LOGE("[%s %d] scrt_status_chk fail\n", __func__, __LINE__);

        #ifdef SCRT_CHECK
        SCRT_ASSERT(0);
        #else
        goto done;
        #endif
    }

    // Public Key Token output (ECDH/ECDSA/ElGamal ECC generate private and public key): start
    u32Output = u32aBase[0];

    *u32aStatus = SCRT_CTRL_READ_MSK(g_tScrtRes[u8ResId].u8MbIdx);

    if(u32Output != u16TokenId)
    {
        SCRT_LOGE("[%s %d] invalid output[%08X] for SCRT_TOKEN_ID_KEY_PAIR_GEN[%08X]\n", __func__, __LINE__, u32Output, u16TokenId);
        goto done;
    }

    memcpy(&u32OutputTokenId, &(u8aAlignBuf[SCRT_PUB_KEY_FMT_LEN]), 4);

    SCRT_LOGI("[%s %d] output token id[%08X] SCRT_TOKEN_ID_KEY_PAIR_GEN[%08X]\n", __func__, __LINE__, 
              u32OutputTokenId, u16TokenId);

    if(u32OutputTokenId != u16TokenId)
    {
        SCRT_LOGE("[%s %d] output token id[%08X] != SCRT_TOKEN_ID_KEY_PAIR_GEN[%08X]\n", __func__, __LINE__, 
                  u32OutputTokenId, u16TokenId);
        goto done;
    }
    // Public Key Token output (ECDH/ECDSA/ElGamal ECC generate private and public key): end

    memcpy(&(u8aPubKey[0]), &(u8aAlignBuf[4]), SCRT_PUB_KEY_VEC_LEN);
    memcpy(&(u8aPubKey[SCRT_PUB_KEY_VEC_LEN]), &(u8aAlignBuf[40]), SCRT_PUB_KEY_VEC_LEN);

    *pu32PrivKeyId = u32PrivKeyAsId;

    iRet = 1;

done:
    if(u8aOutputBuf)
    {
        SCRT_FREE(u8aOutputBuf);
    }

    if(u8ResId < SCRT_MB_IDX_MAX)
    {
        if(u32KeyParamAsId)
        {
            if(scrt_asset_delete(g_tScrtRes[u8ResId].u8MbIdx, u32KeyParamAsId))
            {
                SCRT_LOGE("[%s %d] scrt_asset_delete fail for key param\n", __func__, __LINE__);
            }
        }
        
        if(u32PubKeyAsId)
        {
            if(scrt_asset_delete(g_tScrtRes[u8ResId].u8MbIdx, u32PubKeyAsId))
            {
                SCRT_LOGE("[%s %d] scrt_asset_delete fail for public key\n", __func__, __LINE__);
            }
        }

        #ifdef SCRT_ENABLE_UNLINK
        // Unlink: start
        if(u8Link)
        {
            if(scrt_unlink(g_tScrtRes[u8ResId].u8MbIdx))
            {
                SCRT_LOGE("[%s %d] scrt_unlink fail\n", __func__, __LINE__);
            }
        }
        // Unlink: end
        #endif
    
        // free resource
        scrt_res_free(u8ResId);
    }

    if(u8EcdhLock)
    {
        if(scrt_ecdh_unlock())
        {
            SCRT_LOGE("[%s %d] scrt_ecdh_unlock fail\n", __func__, __LINE__);
        }
    }

    return iRet;
}

/*
 * nl_scrt_ecdh_dhkey_gen_impl - Generate ECDH shared secret (DHKey).
 *
 * @param [in] u8aPubKeyX
 *    Sub-vector X of Public Key
 *
 * @param [in] u8aPubKeyY
 *    Sub-vector Y of Public Key
 *
 * @param [in] u32aPrivKey
 *    Private Key
 *
 * @param [out] pDhKey
 *    DHKey
 *
 * @param [in] pu32PrivKeyId
 *    [in]: ID of private key
 *
 * @return 1 success
 *
 * @return 0 fail
 *
 */
int nl_scrt_ecdh_dhkey_gen_impl(uint8_t *u8aPubKeyX, uint8_t *u8aPubKeyY, uint32_t *u32aPrivKey, void *pDhKey, uint32_t u32PrivKeyId)
{
    int iRet = 0;
    volatile uint32_t *u32aBase = NULL;
    volatile uint32_t *u32aStatus = (uint32_t *)SCRT_STAT_CTRL_ADDR;
    uint8_t *u8aDhKey = (uint8_t *)pDhKey;
    uint32_t i = 0;
    uint32_t u32Output = 0;
    uint8_t u8ResId = SCRT_MB_IDX_MAX;
    uint32_t u32KeyParamAsId = 0;
    uint32_t u32PrivKeyAsId = 0;
    uint32_t u32PubKeyAsId = 0;
    uint32_t u32SharedSecretAsId = 0;
    uint16_t u16TokenId = 0;
    uint32_t u32OutputBufLen = 0;
    uint8_t *u8aOutputBuf = NULL;
    uint8_t *u8aAlignBuf = NULL;
    uint8_t u8EcdhLock = 0;

    #ifdef SCRT_ENABLE_UNLINK
    uint8_t u8Link = 0;
    #endif

    if(!nl_scrt_otp_status_get())
    {
        SCRT_LOGE("[%s %d] OTP not ready\n", __func__, __LINE__);
        goto done;
    }

    if((u8aPubKeyX == NULL) || (u8aPubKeyY == NULL) || 
       (pDhKey == NULL))
    {
        SCRT_LOGE("[%s %d] invalid param\n", __func__, __LINE__);
        goto done;
    }

    if(scrt_ecdh_lock())
    {
        SCRT_LOGE("[%s %d] scrt_ecdh_lock fail\n", __func__, __LINE__);
        goto done;
    }

    u8EcdhLock = 1;

    u8ResId = scrt_res_alloc();

    if(u8ResId >= SCRT_MB_IDX_MAX)
    {
        SCRT_LOGE("[%s %d] scrt_res_alloc fail\n", __func__, __LINE__);
        goto done;
    }

    SCRT_LOGI("[%s %d] u8ResId[%d]\n", __func__, __LINE__, u8ResId);

    #ifdef SCRT_PRE_LINK
    #else
    // Link: start
    if(scrt_link(g_tScrtRes[u8ResId].u8MbIdx))
    {
        SCRT_LOGE("[%s %d] scrt_link fail\n", __func__, __LINE__);
        goto done;
    }

    #ifdef SCRT_ENABLE_UNLINK
    u8Link = 1;
    #endif
    // Link: end
    #endif

    if(scrt_eng_init(g_tScrtRes[u8ResId].u8MbIdx))
    {
        SCRT_LOGE("[%s %d] scrt_eng_init fail\n", __func__, __LINE__);
        goto done;
    }

    if(scrt_ecdh_key_param_init(g_tScrtRes[u8ResId].u8MbIdx, &u32KeyParamAsId))
    {
        SCRT_LOGE("[%s %d] scrt_ecdh_key_param_init fail\n", __func__, __LINE__);
        goto done;
    }

    u32OutputBufLen = SCRT_PUB_KEY_FMT_LEN + 16; // >= SCRT_SHARED_SRCRET_LEN + 4 + 16;
    u8aOutputBuf = (uint8_t *)SCRT_MALLOC(u32OutputBufLen);

    if(!u8aOutputBuf)
    {
        SCRT_LOGE("[%s %d] SCRT_MALLOC(%u) fail\n", __func__, __LINE__, u32OutputBufLen);
        goto done;
    }

    u8aAlignBuf = (uint8_t *)IPC_ADDR_ALIGN(u8aOutputBuf, 16);

    if(u32PrivKeyId) // use previous private key
    {
        u32PrivKeyAsId = u32PrivKeyId;

        SCRT_LOGI("[%s %d] use previous private key[%08X]\n", __func__, __LINE__, u32PrivKeyAsId);
    }
    else // create and load private key
    {
        if(u32aPrivKey == NULL)
        {
            SCRT_LOGE("[%s %d] invalid private key\n", __func__, __LINE__);
            goto done;
        }

        if(scrt_ecdh_priv_key_asset_create(g_tScrtRes[u8ResId].u8MbIdx, &u32PrivKeyAsId))
        {
            SCRT_LOGE("[%s %d] scrt_ecdh_priv_key_asset_create fail\n", __func__, __LINE__);
            goto done;
        }

        SCRT_LOGI("[%s %d] create private key[%08X]\n", __func__, __LINE__, u32PrivKeyAsId);

        u8aAlignBuf[0] = 0x00;
        u8aAlignBuf[1] = 0x01;
        u8aAlignBuf[2] = 0x00;
        u8aAlignBuf[3] = 0x01;

        memcpy(&(u8aAlignBuf[4]), u32aPrivKey, SCRT_PRIV_KEY_LEN);

        SCRT_LOGI("[%s %d] load private key[%08X]\n", __func__, __LINE__, u32PrivKeyAsId);
    
        if(scrt_asset_load(g_tScrtRes[u8ResId].u8MbIdx, u32PrivKeyAsId, u8aAlignBuf, SCRT_PRIV_KEY_FMT_LEN))
        {
            SCRT_LOGE("[%s %d] scrt_asset_load fail for private key\n", __func__, __LINE__);
            goto done;
        }
    }

    SCRT_LOGI("[%s %d] u32PrivKeyAsId[%08X]\n", __func__, __LINE__, u32PrivKeyAsId);

    if(scrt_ecdh_pub_key_asset_create(g_tScrtRes[u8ResId].u8MbIdx, &u32PubKeyAsId))
    {
        SCRT_LOGE("[%s %d] scrt_asset_load fail for private key\n", __func__, __LINE__);
        goto done;
    }
    
    // 0x02000100
    u8aAlignBuf[0] = 0x00;
    u8aAlignBuf[1] = 0x01;
    u8aAlignBuf[2] = 0x00;
    u8aAlignBuf[3] = 0x02;
    
    memcpy(&(u8aAlignBuf[4]), u8aPubKeyX, SCRT_PUB_KEY_VEC_LEN);

    // 0x02010100
    u8aAlignBuf[36] = 0x00;
    u8aAlignBuf[37] = 0x01;
    u8aAlignBuf[38] = 0x01;
    u8aAlignBuf[39] = 0x02;
    
    memcpy(&(u8aAlignBuf[40]), u8aPubKeyY, SCRT_PUB_KEY_VEC_LEN);

    SCRT_LOGI("[%s %d] load public key\n", __func__, __LINE__);

    if(scrt_asset_load(g_tScrtRes[u8ResId].u8MbIdx, u32PubKeyAsId, u8aAlignBuf, SCRT_PUB_KEY_FMT_LEN))
    {
        SCRT_LOGE("[%s %d] scrt_asset_load fail for public key\n", __func__, __LINE__);
        goto done;
    }

    if(scrt_ecdh_shared_secret_asset_create(g_tScrtRes[u8ResId].u8MbIdx, &u32SharedSecretAsId))
    {
        SCRT_LOGE("[%s %d] scrt_asset_load fail for private key\n", __func__, __LINE__);
        goto done;
    }

    u16TokenId = SCRT_TOKEN_ID_SHARED_SECRET_GEN + g_tScrtRes[u8ResId].u8MbIdx;
    u32aBase = (volatile uint32_t *)SCRT_BASE_ADDR(g_tScrtRes[u8ResId].u8MbIdx);

    SCRT_LOGI("[%s %d] u32aBase at[%08X]\n", __func__, __LINE__, u32aBase);

    // Public Key Token (ECDH generate shared secrets (single key-pair)): start
    u32aBase[0] = 0x19000000 | u16TokenId;
    u32aBase[1] = SCRT_ID;
    u32aBase[2] = 0x00080016;
    u32aBase[3] = 0x80000100;
    u32aBase[4] = u32PrivKeyAsId;
    u32aBase[5] = u32KeyParamAsId;
    u32aBase[6] = u32PubKeyAsId;
    u32aBase[7] = 0x00000000;
    u32aBase[8] = 0x00000000;
    u32aBase[9] = 0x00000000;
    u32aBase[10] = 0x00000000;
    u32aBase[11] = 0x00000000;
    u32aBase[12] = u32SharedSecretAsId;

    SCRT_LOGI("[%s %d] param[%08X] priv[%08X] pub[%08X] ss[%08X]\n", __func__, __LINE__, 
              u32KeyParamAsId, u32PrivKeyAsId, 
              u32PubKeyAsId, u32SharedSecretAsId);

    *u32aStatus = SCRT_CTRL_WRITE_MSK(g_tScrtRes[u8ResId].u8MbIdx);
    // Public Key Token (ECDH generate shared secrets (single key-pair)): end

    // write done and ready to read
    if(scrt_status_chk(SCRT_STATUS_WRITE_MSK(g_tScrtRes[u8ResId].u8MbIdx) | SCRT_STATUS_READ_MSK(g_tScrtRes[u8ResId].u8MbIdx), 
                       SCRT_STATUS_READ_MSK(g_tScrtRes[u8ResId].u8MbIdx)))
    {
        SCRT_LOGE("[%s %d] scrt_status_chk fail\n", __func__, __LINE__);

        #ifdef SCRT_CHECK
        SCRT_ASSERT(0);
        #else
        goto done;
        #endif
    }

    // Public Key Token (ECDH generate shared secrets (single key-pair)) output: start
    u32Output = u32aBase[0];

    *u32aStatus = SCRT_CTRL_READ_MSK(g_tScrtRes[u8ResId].u8MbIdx);

    if(u32Output != u16TokenId)
    {
        SCRT_LOGE("[%s %d] invalid output[%08X] for SCRT_TOKEN_ID_SHARED_SECRET[%08X]\n", __func__, __LINE__, u32Output, u16TokenId);
        goto done;
    }
    // Public Key Token (ECDH generate shared secrets (single key-pair)) output: end
    
    if(scrt_asset_get(g_tScrtRes[u8ResId].u8MbIdx, u32SharedSecretAsId, u8aAlignBuf, SCRT_SHARED_SRCRET_LEN + 4, SCRT_SHARED_SRCRET_LEN))
    {
        SCRT_LOGE("[%s %d] scrt_asset_get fail for shared secret\n", __func__, __LINE__);
        goto done;
    }

    for(i = 0; i < SCRT_SHARED_SRCRET_LEN; i++)
    {
        u8aDhKey[i] = u8aAlignBuf[SCRT_SHARED_SRCRET_LEN - 1 - i];
    }

    iRet = 1;

done:
    if(u8aOutputBuf)
    {
        SCRT_FREE(u8aOutputBuf);
    }

    if(u8ResId < SCRT_MB_IDX_MAX)
    {
        if(u32KeyParamAsId)
        {
            if(scrt_asset_delete(g_tScrtRes[u8ResId].u8MbIdx, u32KeyParamAsId))
            {
                SCRT_LOGE("[%s %d] scrt_asset_delete fail for key param\n", __func__, __LINE__);
            }
        }

        if((u32PrivKeyAsId) && (u32PrivKeyAsId != u32PrivKeyId))
        {
            if(scrt_asset_delete(g_tScrtRes[u8ResId].u8MbIdx, u32PrivKeyAsId))
            {
                SCRT_LOGE("[%s %d] scrt_asset_delete fail for private key\n", __func__, __LINE__);
            }
        }

        if(u32PubKeyAsId)
        {
            if(scrt_asset_delete(g_tScrtRes[u8ResId].u8MbIdx, u32PubKeyAsId))
            {
                SCRT_LOGE("[%s %d] scrt_asset_delete fail for public key\n", __func__, __LINE__);
            }
        }

        if(u32SharedSecretAsId)
        {
            if(scrt_asset_delete(g_tScrtRes[u8ResId].u8MbIdx, u32SharedSecretAsId))
            {
                SCRT_LOGE("[%s %d] scrt_asset_delete fail for shared secret\n", __func__, __LINE__);
            }
        }

        #ifdef SCRT_ENABLE_UNLINK
        // Unlink: start
        if(u8Link)
        {
            if(scrt_unlink(g_tScrtRes[u8ResId].u8MbIdx))
            {
                SCRT_LOGE("[%s %d] scrt_unlink fail\n", __func__, __LINE__);
            }
        }
        // Unlink: end
        #endif
    
        // free resource
        scrt_res_free(u8ResId);
    }

    if(u8EcdhLock)
    {
        if(scrt_ecdh_unlock())
        {
            SCRT_LOGE("[%s %d] scrt_ecdh_unlock fail\n", __func__, __LINE__);
        }
    }

    return iRet;
}

/*
 * nl_scrt_key_delete_impl - Delete key.
 *
 * @param [in] u32KeyId
 *    [in]: ID of key
 *
 * @return 1 success
 *
 * @return 0 fail
 *
 */
int nl_scrt_key_delete_impl(uint32_t u32KeyId)
{
    int iRet = 0;
    uint8_t u8ResId = SCRT_MB_IDX_MAX;

    #ifdef SCRT_ENABLE_UNLINK
    uint8_t u8Link = 0;
    #endif

    if(!u32KeyId)
    {
        SCRT_LOGE("[%s %d] invalid id\n", __func__, __LINE__);
        goto done;
    }

    u8ResId = scrt_res_alloc();

    if(u8ResId >= SCRT_MB_IDX_MAX)
    {
        SCRT_LOGE("[%s %d] scrt_res_alloc fail\n", __func__, __LINE__);
        goto done;
    }

    #ifdef SCRT_PRE_LINK
    #else
    // Link: start
    if(scrt_link(g_tScrtRes[u8ResId].u8MbIdx))
    {
        SCRT_LOGE("[%s %d] scrt_link fail\n", __func__, __LINE__);
        goto done;
    }

    #ifdef SCRT_ENABLE_UNLINK
    u8Link = 1;
    #endif
    // Link: end
    #endif

    if(scrt_asset_delete(g_tScrtRes[u8ResId].u8MbIdx, u32KeyId))
    {
        SCRT_LOGE("[%s %d] scrt_asset_delete fail\n", __func__, __LINE__);
    }

    iRet = 1;

done:

    if(u8ResId < SCRT_MB_IDX_MAX)
    {
        #ifdef SCRT_ENABLE_UNLINK
        // Unlink: start
        if(u8Link)
        {
            if(scrt_unlink(g_tScrtRes[u8ResId].u8MbIdx))
            {
                SCRT_LOGE("[%s %d] scrt_unlink fail\n", __func__, __LINE__);
            }
        }
        // Unlink: end
        #endif
    
        // free resource
        scrt_res_free(u8ResId);
    }

    return iRet;
}

#if 1
/*
 * nl_scrt_init_impl - Initialize scrt module.
 *
 * @param [in] None
 *    N/A
 *
 * @return 1 success
 *
 * @return 0 fail
 *
 */
int nl_scrt_init_impl(void)
{
    int iRet = 0;

    scrt_clk_enable(1, (SCRT_OTP_CLK_MSK | SCRT_IP_CLK_MSK));

    if(scrt_param_init())
    {
        SCRT_LOGE("[%s %d] scrt_param_init fail\n", __func__, __LINE__);
        goto done;
    }

    if(scrt_sem_create())
    {
        SCRT_LOGE("[%s %d] scrt_sem_create fail\n", __func__, __LINE__);
        goto done;
    }

    if(scrt_mb_init())
    {
        SCRT_LOGE("[%s %d] scrt_mb_init fail\n", __func__, __LINE__);
        goto done;
    }

    iRet = 1;

done:
    //scrt_clk_enable(0, SCRT_IP_CLK_MSK);
    return iRet;
}

/*
 * nl_scrt_aes_ccm_impl - Perform the AES CCM Operation.
 *
 * @param [in] bEncrypt
 *    1:encrypt 0:decrypt
 *
 * @param [in] sk
 *    Key
 *
 * @param [in] sk_len
 *    Size of Key
 *
 * @param [in] nonce
 *    Nonce
 *
 * @param [in] nonce_len
 *    Size of Nonce
 *
 * @param [in] adata
 *    Additional Associated Data
 *
 * @param [in] adata_len
 *    Size of Additional Associated Data
 *
 * @param [in/out] plain_text
 *    Plain Text Data
 *
 * @param [in/out] encrypted_text
 *    Encrypted Data
 *
 * @param [in] text_len
 *    The length of the Data Transfer
 *
 * @param [out] tag
 *    Output Tag Data
 *
 * @param [in] tag_len
 *    Size of the Output Tag Data
 *
 * @return 1 success
 *
 * @return 0 fail
 *
 */
int nl_scrt_aes_ccm_impl(int bEncrypt, unsigned char *sk, int sk_len, unsigned char *nonce, int nonce_len, unsigned char *adata, int adata_len, unsigned char *plain_text, unsigned char *encrypted_text, int text_len, unsigned char *tag, int tag_len)
{
    int status = 0;
    unsigned int word_11 = 0;
    int buf_len = 0;
    volatile uint32_t *u32aBase = NULL;
    volatile uint32_t *u32aStatus = (uint32_t *)SCRT_STAT_CTRL_ADDR;
    uint32_t u32Output = 0;
    uint8_t u8ResId = SCRT_MB_IDX_MAX;
    uint16_t u16TokenId = 0;
    uint8_t *u8aInput = NULL;
    uint8_t *u8aOutput = NULL;
    uint8_t u8NeedToClr = 0;

    #ifdef SCRT_ENABLE_UNLINK
    uint8_t u8Link = 0;
    #endif
    
    if((!sk) || (!sk_len) || 
       (!nonce) || (!nonce_len) || 
       (!adata) || (!adata_len) || 
       (!plain_text) || (!encrypted_text) || (!text_len) || 
       (!tag) || (!tag_len))
    {
        SCRT_LOGE("[%s %d] invalid param\n", __func__, __LINE__);
        goto done;
    }

    u8ResId = scrt_res_alloc();

    if(u8ResId >= SCRT_MB_IDX_MAX)
    {
        SCRT_LOGE("[%s %d] scrt_res_alloc fail\n", __func__, __LINE__);
        goto done;
    }

    //SCRT_LOGI("[%s %d] u8ResId[%d]\n", __func__, __LINE__, u8ResId);

    #if 0 // Todo
    {
        uint32_t *pu32Reg = (uint32_t *)0x40001134;
        uint32_t u32Value = *pu32Reg;

        *pu32Reg = u32Value | (0x01000000);
    }
    #endif

    buf_len = text_len & (~0x0F);

    if(text_len & 0x0F)
    {
        buf_len += 16;
    }

    #ifdef SCRT_PRE_LINK
    #else
    // Link: start
    if(scrt_link(g_tScrtRes[u8ResId].u8MbIdx))
    {
        SCRT_LOGE("[%s %d] scrt_link fail\n", __func__, __LINE__);
        goto done;
    }

    #ifdef SCRT_ENABLE_UNLINK
    u8Link = 1;
    #endif
    // Link: end
    #endif

    if(bEncrypt)
    {
        u16TokenId = SCRT_TOKEN_ID_AES_CCM_ENCRYPT + g_tScrtRes[u8ResId].u8MbIdx;
        u8aInput = plain_text;
        u8aOutput = encrypted_text;
    }
    else
    {
        u16TokenId = SCRT_TOKEN_ID_AES_CCM_DECRYPT + g_tScrtRes[u8ResId].u8MbIdx;
        u8aInput = encrypted_text;
        u8aOutput = plain_text;
    }

    u32aBase = (volatile uint32_t *)SCRT_BASE_ADDR(g_tScrtRes[u8ResId].u8MbIdx);

    // Encryption/Decryption (AES-CCM): start
    u32aBase[0] = 0x01040000 | u16TokenId;
    //u32aBase[0] = 0x01000000 | u16TokenId;
    u32aBase[1] = 0x00000000;
    u32aBase[2] = text_len;                                 //The length of the data transfer
    u32aBase[3] = (uint32_t)u8aInput;                       //Input Data
    u32aBase[4] = 0x00000000;                               //Input Data
    u32aBase[5] = buf_len;                                  //Input Data Length
    u32aBase[6] = (uint32_t)u8aOutput;                      //Output Data
    u32aBase[7] = 0x00000000;                               //Output Data
    u32aBase[8] = (buf_len + 4);                            //Output Data Length
    //u32aBase[8] = buf_len;                            //Output Data Length
    u32aBase[9] = (uint32_t)adata;                          //Associated Data
    u32aBase[10] = 0x00000000;                              //Associated Data
    //u32aBase[11] = 0x10d18050;                                                //0x10d18050;           //Nonce length               => word 11[23:20]  => 13  => 0xd
                                                                                //Key length             => word 11[19:16]           => 1 (1 means 128 bits key)
                                                                                //Tag length             => word 11[28:24] 5 bits  => 0x10  => 16
                                                                                //Encrypt or Decrypt  => word11[15]                => 1:encrypt 0:decrypt

    if (bEncrypt == 1) {
        word_11 = ((tag_len & 0xff) << 24) | ((((nonce_len & 0xf) << 4) | (0x1)) << 16) | ((0x80) << 8) | ((0x50) << 0);
    } else {
        word_11 = ((tag_len & 0xff) << 24) | ((((nonce_len & 0xf) << 4) | (0x1)) << 16) | ((0x0) << 8) | ((0x50) << 0);
    }

    //printf("\r\n text_len:%d word_11:%08x \r\n", text_len, word_11);

    u32aBase[11] = word_11;
    u32aBase[12] = 0x00000000;
    u32aBase[13] = 0x00000000;
    u32aBase[14] = 0x00000000;
    u32aBase[15] = 0x00000000;
    u32aBase[16] = 0x00000000;

    memcpy((void *)&(u32aBase[17]), sk, sk_len);

    u32aBase[21] = 0x00000000;          //key
    u32aBase[22] = 0x00000000;          //key
    u32aBase[23] = 0x00000000;          //key
    u32aBase[24] = 0x00000000;          //key
    u32aBase[25] = adata_len;           //Associated Data Length
    u32aBase[26] = 0x00000000;
    u32aBase[27] = 0x00000000;
    u32aBase[28] = 0x00000000;

    memcpy((void *)&(u32aBase[29]), nonce, nonce_len);

    if (bEncrypt == 1) {
        u32aBase[33] = 0x00000000;
        u32aBase[34] = 0x00000000;
        u32aBase[35] = 0x00000000;
        u32aBase[36] = 0x00000000;
    } else {
        memcpy((void *)&(u32aBase[33]), tag, tag_len);
    }

    *u32aStatus = SCRT_CTRL_WRITE_MSK(g_tScrtRes[u8ResId].u8MbIdx);
    // Encryption/Decryption (AES-CCM): end

    // write done and ready to read
    if(scrt_status_chk(SCRT_STATUS_WRITE_MSK(g_tScrtRes[u8ResId].u8MbIdx) | SCRT_STATUS_READ_MSK(g_tScrtRes[u8ResId].u8MbIdx), 
                       SCRT_STATUS_READ_MSK(g_tScrtRes[u8ResId].u8MbIdx)))
    {
        SCRT_LOGE("[%s %d] scrt_status_chk fail\n", __func__, __LINE__);

        #ifdef SCRT_CHECK
        SCRT_ASSERT(0);
        #else
        goto done;
        #endif
    }

    u8NeedToClr = 1;

    // Encryption/Decryption (AES-CCM) output: start
    u32Output = u32aBase[0];

    if(u32Output != u16TokenId)
    {
        SCRT_LOGI("[%s %d] enc[%d] invalid output[%08X] for SCRT_TOKEN_ID_AES_CCM[%08X]\n", __func__, __LINE__, bEncrypt, u32Output, u16TokenId);

        #ifdef SCRT_CHECK
        if(bEncrypt)
        {
            SCRT_ASSERT(0);
        }
        #endif
        
        goto done;
    }

    u32Output = *((volatile unsigned int *)(u8aOutput + buf_len));

    if(u32Output != u16TokenId)
    {
        SCRT_LOGE("[%s %d] output token id[%08X] != SCRT_TOKEN_ID_AES_CCM[%08X]\n", __func__, __LINE__, 
                  u32Output, u16TokenId);

        #ifdef SCRT_CHECK
        SCRT_ASSERT(0);
        #else
        goto done;
        #endif
    }

    if (bEncrypt == 1) {
        /* Copy the output tag data to the buffer "tag" */
        memcpy((void *)tag, (void *)&(u32aBase[6]), tag_len);
    }

    *u32aStatus = SCRT_CTRL_READ_MSK(g_tScrtRes[u8ResId].u8MbIdx);
    u8NeedToClr = 0;
    // Encryption/Decryption (AES-CCM) output: end

    status = 1;

done:
    if(u8ResId < SCRT_MB_IDX_MAX)
    {
        if(u8NeedToClr)
        {
            *u32aStatus = SCRT_CTRL_READ_MSK(g_tScrtRes[u8ResId].u8MbIdx);
        }

        #ifdef SCRT_ENABLE_UNLINK
        // Unlink: start
        if(u8Link)
        {
            if(scrt_unlink(g_tScrtRes[u8ResId].u8MbIdx))
            {
                SCRT_LOGE("[%s %d] scrt_unlink fail\n", __func__, __LINE__);
            }
        }
        // Unlink: end
        #endif
    
        // free resource
        scrt_res_free(u8ResId);
    }

    return status;
}

/*
 * nl_aes_ccm_impl  - AES CCM Operation. [deprecated]
 *
 * @param [in] bEncrypt
 *    true:encrypt false:decrypt
 *
 * @param [in] sk
 *    Key
 *
 * @param [in] sk_len
 *    Size of Key
 *
 * @param [in] nonce
 *    Nonce
 *
 * @param [in] nonce_len
 *    Size of Nonce
 *
 * @param [in] adata
 *    Additional Associated Data
 *
 * @param [in] adata_len
 *    Size of Additional Associated Data
 *
 * @param [in/out] plain_text
 *    Plain Text Data
 *
 * @param [in/out] encrypted_text
 *    Encrypted Data
 *
 * @param [in] text_len
 *    The length of the Data Transfer
 *
 * @param [out] tag
 *    Output Tag Data
 *
 * @param [in] tag_len
 *    Size of the Output Tag Data
 *
 * @return 1 success
 *
 * @return 0 fail
 *
 * @deprecated It will be removed in the future
 *
 */
int nl_aes_ccm_impl(bool bEncrypt, uint8_t *sk, int sk_len, uint8_t *nonce, int nonce_len, unsigned int adata, int adata_len, unsigned int plain_text, unsigned int encrypted_text, int text_len, uint8_t *tag, int tag_len)
{
    int iRet = 0;
    
    if(!nl_scrt_aes_ccm((bEncrypt)?(1):(0), sk, sk_len, nonce, nonce_len, (unsigned char *)adata, adata_len, 
                        (unsigned char *)plain_text, (unsigned char *)encrypted_text, text_len, tag, tag_len))
    {
        goto done;
    }

    iRet = 1;

done:
    return iRet;
}

/*
 * nl_hmac_sha_1_impl - Perform the HMAC SHA1 Operation
 *
 * @param [in] sk
 *      The Key
 *
 * @param [in] sk_len
 *      The Key Length
 *
 * @param [in] in_data
 *      The Input Data which to be handled
 *
 * @param [in] in_data_len
 *      The length of Input Data
 *
 * @param [out] mac
 *      Output MAC Data
 *
 * @return 1 success
 *
 * @return 0 fail
 *
 */
int nl_hmac_sha_1_impl(uint8_t *sk, int sk_len, uint8_t *in_data, int in_data_len, uint8_t *mac)
{
    int status = 0;
    unsigned int word_6 = 0;
    volatile uint32_t *u32aBase = NULL;
    volatile uint32_t *u32aStatus = (uint32_t *)SCRT_STAT_CTRL_ADDR;
    uint32_t u32Output = 0;
    uint8_t u8ResId = SCRT_MB_IDX_MAX;
    uint16_t u16TokenId = 0;
    uint8_t u8NeedToClr = 0;

    #ifdef SCRT_ENABLE_UNLINK
    uint8_t u8Link = 0;
    #endif

    if((!sk) || (!sk_len) || 
       (!in_data) || (!in_data_len) || 
       (!mac))
    {
        SCRT_LOGE("[%s %d] invalid param\n", __func__, __LINE__);
        goto done;
    }

    u8ResId = scrt_res_alloc();

    if(u8ResId >= SCRT_MB_IDX_MAX)
    {
        SCRT_LOGE("[%s %d] scrt_res_alloc fail\n", __func__, __LINE__);
        goto done;
    }

    #ifdef SCRT_PRE_LINK
    #else
    // Link: start
    if(scrt_link(g_tScrtRes[u8ResId].u8MbIdx))
    {
        SCRT_LOGE("[%s %d] scrt_link fail\n", __func__, __LINE__);
        goto done;
    }

    #ifdef SCRT_ENABLE_UNLINK
    u8Link = 1;
    #endif
    // Link: end
    #endif

    u16TokenId = SCRT_TOKEN_ID_HMAC_SHA_1 + g_tScrtRes[u8ResId].u8MbIdx;
    u32aBase = (volatile uint32_t *)SCRT_BASE_ADDR(g_tScrtRes[u8ResId].u8MbIdx);

    // MAC: start
    u32aBase[0] = 0x03000000 | u16TokenId;
    u32aBase[1] = 0x00000000;
    u32aBase[2] = in_data_len; //The length of the input data (in bytes).
    u32aBase[3] = (uint32_t)in_data;
    u32aBase[4] = 0x00000000;
    u32aBase[5] = in_data_len;

    //*(volatile unsigned int *)0x60000018 = 0x00200001;
    word_6 = ((sk_len & 0xff) << 16) | ((0x1) << 0);
    u32aBase[6] = word_6;      //key length: 0x08  ;  [3:0] Algorithm  HMAC-SHA-1, 160-bit MAC, block size is 64 Bytes

    u32aBase[7] = 0x00000000;
    u32aBase[8] = 0x00000000;
    u32aBase[9] = 0x00000000;
    u32aBase[10] = 0x00000000;
    u32aBase[11] = 0x00000000;
    u32aBase[12] = 0x00000000;
    u32aBase[13] = 0x00000000;
    u32aBase[14] = 0x00000000;
    u32aBase[15] = 0x00000000;
    u32aBase[16] = 0x00000000;
    u32aBase[17] = 0x00000000;
    u32aBase[18] = 0x00000000;
    u32aBase[19] = 0x00000000;
    u32aBase[20] = 0x00000000;
    u32aBase[21] = 0x00000000;
    u32aBase[22] = 0x00000000;
    u32aBase[23] = 0x00000000;

    //word 24
    u32aBase[24] = in_data_len;

    u32aBase[25] = 0x00000000;
    u32aBase[26] = 0x00000000;
    u32aBase[27] = 0x00000000;

    //msg_print(LOG_HIGH_LEVEL, "[scrt] nl_hmac_sha_1, in_data_len:%d word_6:%08x \r\n", in_data_len, word_6);

    /* Key */
    memcpy((void *)&(u32aBase[28]), sk, sk_len);

    /* Write a word */
    *u32aStatus = SCRT_CTRL_WRITE_MSK(g_tScrtRes[u8ResId].u8MbIdx);
    // MAC: end

    // write done and ready to read
    if(scrt_status_chk(SCRT_STATUS_WRITE_MSK(g_tScrtRes[u8ResId].u8MbIdx) | SCRT_STATUS_READ_MSK(g_tScrtRes[u8ResId].u8MbIdx), 
                       SCRT_STATUS_READ_MSK(g_tScrtRes[u8ResId].u8MbIdx)))
    {
        SCRT_LOGE("[%s %d] scrt_status_chk fail\n", __func__, __LINE__);

        #ifdef SCRT_CHECK
        SCRT_ASSERT(0);
        #else
        goto done;
        #endif
    }

    u8NeedToClr = 1;

    // MAC output: start
    u32Output = u32aBase[0];

    if(u32Output != u16TokenId)
    {
        SCRT_LOGE("[%s %d] output token id[%08X] != SCRT_TOKEN_ID_HMAC_SHA_1[%08X]\n", __func__, __LINE__, 
                  u32Output, u16TokenId);

        #ifdef SCRT_CHECK
        SCRT_ASSERT(0);
        #else
        goto done;
        #endif
    }

    /* Copy the output MAC data */
    memcpy((void *)mac, (void *)&(u32aBase[2]), 20);

    *u32aStatus = SCRT_CTRL_READ_MSK(g_tScrtRes[u8ResId].u8MbIdx);
    u8NeedToClr = 0;
    // MAC output: end

    status = 1;

done:
    if(u8ResId < SCRT_MB_IDX_MAX)
    {
        if(u8NeedToClr)
        {
            *u32aStatus = SCRT_CTRL_READ_MSK(g_tScrtRes[u8ResId].u8MbIdx);
        }

        #ifdef SCRT_ENABLE_UNLINK
        // Unlink: start
        if(u8Link)
        {
            if(scrt_unlink(g_tScrtRes[u8ResId].u8MbIdx))
            {
                SCRT_LOGE("[%s %d] scrt_unlink fail\n", __func__, __LINE__);
            }
        }
        // Unlink: end
        #endif
    
        // free resource
        scrt_res_free(u8ResId);
    }

    return status;
}

/*
 * nl_scrt_aes_ecb_impl - Perform the AES ECB Operation.
 *
 * @param [in] bEncrypt
 *    1:Encrypt
 *    0:Decrypt
 *
 * @param [in] sk
 *    Key
 *
 * @param [in] sk_len
 *    Size of Key
 *
 * @param [in/out] data_in
 *    Input Data
 *
 * @param [out] data_out
 *    Output Data
 *
 * @param [in] data_len
 *    The length of the Data Transfer
 *
 * @return 1 success
 *
 * @return 0 fail
 *
 */
int nl_scrt_aes_ecb_impl(int bEncrypt, unsigned char *sk, int sk_len, unsigned char *data_in, unsigned char *data_out, unsigned data_len)
{
    int status = 0;
    unsigned int word_11 = 0;
    int buf_len = 0;
    int key_len = 0;
    int fieldEncrypt = 0;
    volatile uint32_t *u32aBase = NULL;
    volatile uint32_t *u32aStatus = (uint32_t *)SCRT_STAT_CTRL_ADDR;
    uint32_t u32Output = 0;
    uint8_t u8ResId = SCRT_MB_IDX_MAX;
    uint16_t u16TokenId = 0;
    uint8_t u8NeedToClr = 0;

    #ifdef SCRT_ENABLE_UNLINK
    uint8_t u8Link = 0;
    #endif

    if((!sk) || (!sk_len) || (!data_in) || (!data_out) || (!data_len))
    {
        SCRT_LOGE("[%s %d] invalid param\n", __func__, __LINE__);
        goto done;
    }

    if(bEncrypt)
    {
        fieldEncrypt = 1;
    }

    switch(sk_len)
    {
        case 16:
            key_len = 1;
            break;
        case 24:
            key_len = 2;
            break;
        case 32:
            key_len = 3;
            break;
        default:
            SCRT_LOGE("[%s %d] invalid sk_len[%d]\n", __func__, __LINE__, sk_len);
            goto done;
    }

    u8ResId = scrt_res_alloc();

    if(u8ResId >= SCRT_MB_IDX_MAX)
    {
        SCRT_LOGE("[%s %d] scrt_res_alloc fail\n", __func__, __LINE__);
        goto done;
    }

    buf_len = data_len & (~0x0F);

    if(data_len & 0x0F)
    {
        buf_len += 16;
    }

    #ifdef SCRT_PRE_LINK
    #else
    // Link: start
    if(scrt_link(g_tScrtRes[u8ResId].u8MbIdx))
    {
        SCRT_LOGE("[%s %d] scrt_link fail\n", __func__, __LINE__);
        goto done;
    }

    #ifdef SCRT_ENABLE_UNLINK
    u8Link = 1;
    #endif
    // Link: end
    #endif

    u16TokenId = SCRT_TOKEN_ID_AES_ECB + g_tScrtRes[u8ResId].u8MbIdx;
    u32aBase = (volatile uint32_t *)SCRT_BASE_ADDR(g_tScrtRes[u8ResId].u8MbIdx);

    // Encryption/Decryption (AES-ECB): start
    u32aBase[0] = 0x01040000 | u16TokenId;
    u32aBase[1] = 0x00000000;
    u32aBase[2] = buf_len;
    u32aBase[3] = (uint32_t)data_in;
    u32aBase[4] = 0x00000000;
    u32aBase[5] = buf_len;
    u32aBase[6] = (uint32_t)data_out;
    u32aBase[7] = 0x00000000;
    u32aBase[8] = buf_len + 4;
    u32aBase[9] = 0x00000000;
    u32aBase[10] = 0x00000000;

    word_11 = ((key_len << 16) | (fieldEncrypt << 15));
    u32aBase[11] = word_11;

    u32aBase[12] = 0x00000000;
    u32aBase[13] = 0x00000000;
    u32aBase[14] = 0x00000000;
    u32aBase[15] = 0x00000000;
    u32aBase[16] = 0x00000000;

    memcpy((void *)&(u32aBase[17]), (void *)sk, 16);

    u32aBase[21] = 0x00000000;
    u32aBase[22] = 0x00000000;
    u32aBase[23] = 0x00000000;
    u32aBase[24] = 0x00000000;

    *u32aStatus = SCRT_CTRL_WRITE_MSK(g_tScrtRes[u8ResId].u8MbIdx);
    // Encryption/Decryption (AES-ECB): end

    // write done and ready to read
    if(scrt_status_chk(SCRT_STATUS_WRITE_MSK(g_tScrtRes[u8ResId].u8MbIdx) | SCRT_STATUS_READ_MSK(g_tScrtRes[u8ResId].u8MbIdx), 
                       SCRT_STATUS_READ_MSK(g_tScrtRes[u8ResId].u8MbIdx)))
    {
        SCRT_LOGE("[%s %d] scrt_status_chk fail\n", __func__, __LINE__);

        #ifdef SCRT_CHECK
        SCRT_ASSERT(0);
        #else
        goto done;
        #endif
    }

    u8NeedToClr = 1;

    // Encryption/Decryption (AES-ECB) output: start
    u32Output = u32aBase[0];

    if(u32Output != u16TokenId)
    {
        SCRT_LOGI("[%s %d] output token id[%08X] != SCRT_TOKEN_ID_AES_ECB[%08X]\n", __func__, __LINE__, 
                  u32Output, u16TokenId);

        #ifdef SCRT_CHECK
        if(bEncrypt)
        {
            SCRT_ASSERT(0);
        }
        #endif
        
        goto done;
    }

    u32Output = *((volatile unsigned int *)(data_out + buf_len));

    if(u32Output != u16TokenId)
    {
        SCRT_LOGE("[%s %d] output token id[%08X] != SCRT_TOKEN_ID_AES_ECB[%08X]\n", __func__, __LINE__, 
                  u32Output, u16TokenId);

        #ifdef SCRT_CHECK
        SCRT_ASSERT(0);
        #else
        goto done;
        #endif
    }

    *u32aStatus = SCRT_CTRL_READ_MSK(g_tScrtRes[u8ResId].u8MbIdx);
    u8NeedToClr = 0;
    // Encryption/Decryption (AES-ECB) output: end

    status = 1;

done:
    if(u8ResId < SCRT_MB_IDX_MAX)
    {
        if(u8NeedToClr)
        {
            *u32aStatus = SCRT_CTRL_READ_MSK(g_tScrtRes[u8ResId].u8MbIdx);
        }

        #ifdef SCRT_ENABLE_UNLINK
        // Unlink: start
        if(u8Link)
        {
            if(scrt_unlink(g_tScrtRes[u8ResId].u8MbIdx))
            {
                SCRT_LOGE("[%s %d] scrt_unlink fail\n", __func__, __LINE__);
            }
        }
        // Unlink: end
        #endif
    
        // free resource
        scrt_res_free(u8ResId);
    }

    return status;
}

/*
 * nl_aes_cmac_patch - Perform the AES CMAC Operation
 *
 * @param [in] sk
 *      The Key
 *
 * @param [in] sk_len
 *      The Key Length
 *
 * @param [in] in_data
 *      The Input Data which to be handled
 *
 * @param [in] in_data_len
 *      The length of Input Data
 *
 * @param [out] mac
 *      Output MAC Data
 *
 * @return 1 success
 *
 * @return 0 fail
 *
 */
int nl_scrt_aes_cmac_impl(uint8_t *u8aKey, uint8_t u8KeyLen, uint8_t *u8aInputBuf, uint32_t u32BufSize, uint32_t u32InputLen, uint8_t *u8aMac)
{
    int status = 0;
    unsigned int word_6 = 0;
    volatile uint32_t *u32aBase = NULL;
    volatile uint32_t *u32aStatus = (uint32_t *)SCRT_STAT_CTRL_ADDR;
    uint32_t u32Output = 0;
    uint8_t u8ResId = SCRT_MB_IDX_MAX;
    uint16_t u16TokenId = 0;
    uint8_t u8NeedToClr = 0;
    uint32_t u32BlkSize = 16;
    uint32_t u32DataLen = 0;
    uint32_t u32PadLen = 0;
    uint8_t *u8aInput = NULL;

    #ifdef SCRT_ENABLE_UNLINK
    uint8_t u8Link = 0;
    #endif

    if((!u8aKey) || (!u8KeyLen) || 
       (!u8aMac))
    {
        SCRT_LOGE("[%s %d] invalid param\n", __func__, __LINE__);
        goto done;
    }

    if(u32InputLen)
    {
        if(!u8aInputBuf)
        {
            SCRT_LOGE("[%s %d] invalid input buffer\n", __func__, __LINE__);
            goto done;
        }
    }

    if((u8KeyLen != 16) && (u8KeyLen != 24) && (u8KeyLen != 32))
    {
        SCRT_LOGE("[%s %d] invalid key length[%d]\n", __func__, __LINE__, u8KeyLen);
        goto done;
    }

    u8ResId = scrt_res_alloc();

    if(u8ResId >= SCRT_MB_IDX_MAX)
    {
        SCRT_LOGE("[%s %d] scrt_res_alloc fail\n", __func__, __LINE__);
        goto done;
    }

    #ifdef SCRT_PRE_LINK
    #else
    // Link: start
    if(scrt_link(g_tScrtRes[u8ResId].u8MbIdx))
    {
        SCRT_LOGE("[%s %d] scrt_link fail\n", __func__, __LINE__);
        goto done;
    }

    #ifdef SCRT_ENABLE_UNLINK
    u8Link = 1;
    #endif
    // Link: end
    #endif

    if(u32InputLen)
    {
        uint32_t u32Remain = u32InputLen & (u32BlkSize - 1);

        u32DataLen = u32InputLen & (~(u32BlkSize - 1));
        
        if(u32Remain)
        {
            u32DataLen += u32BlkSize;
            u32PadLen = u32BlkSize - u32Remain;
        }
    }
    else
    {
        u32DataLen = u32BlkSize;
        u32PadLen = u32BlkSize;
    }

    // MAC: start
    if(u32PadLen)
    {
        uint8_t u8Alloc = 1;

        if(u8aInputBuf)
        {
            if(u32BufSize >= u32DataLen)
            {
                u8aInput = u8aInputBuf;
                u8Alloc = 0;
            }
        }

        if(u8Alloc)
        {
            u8aInput = SCRT_MALLOC(u32DataLen);

            if(u8aInput == NULL)
            {
                SCRT_LOGE("[%s %d] SCRT_MALLOC fail\n", __func__, __LINE__);
                goto done;
            }

            if(u8aInputBuf && u32InputLen)
            {
                memcpy(u8aInput, u8aInputBuf, u32InputLen);
            }
        }

        // padding
        u8aInput[u32InputLen] = 0x80;
        memset(&(u8aInput[u32InputLen + 1]), 0x00, u32PadLen - 1);
    }
    else // multiple of 16
    {
        u8aInput = u8aInputBuf;
    }

    u16TokenId = SCRT_TOKEN_ID_AES_CMAC + g_tScrtRes[u8ResId].u8MbIdx;
    u32aBase = (volatile uint32_t *)SCRT_BASE_ADDR(g_tScrtRes[u8ResId].u8MbIdx);

    u32aBase[0] = 0x03000000 | u16TokenId;
    u32aBase[1] = SCRT_ID;
    u32aBase[2] = u32DataLen;
    u32aBase[3] = (uint32_t)u8aInput;
    u32aBase[4] = 0x00000000;
    u32aBase[5] = u32DataLen;

    word_6 = ((u8KeyLen & 0xff) << 16) | 0x08;
    u32aBase[6] = word_6;

    u32aBase[7] = 0x00000000;
    u32aBase[8] = 0x00000000;
    u32aBase[9] = 0x00000000;
    u32aBase[10] = 0x00000000;
    u32aBase[11] = 0x00000000;
    u32aBase[12] = 0x00000000;
    u32aBase[13] = 0x00000000;
    u32aBase[14] = 0x00000000;
    u32aBase[15] = 0x00000000;
    u32aBase[16] = 0x00000000;
    u32aBase[17] = 0x00000000;
    u32aBase[18] = 0x00000000;
    u32aBase[19] = 0x00000000;
    u32aBase[20] = 0x00000000;
    u32aBase[21] = 0x00000000;
    u32aBase[22] = 0x00000000;
    u32aBase[23] = 0x00000000;

    //word 24
    if(u32PadLen < u32BlkSize)
    {
        u32aBase[24] = u32PadLen & 0x0F;
    }
    else
    {
        u32aBase[24] = 0x0F;
    }

    u32aBase[25] = 0x00000000;
    u32aBase[26] = 0x00000000;
    u32aBase[27] = 0x00000000;

    //msg_print(LOG_HIGH_LEVEL, "[scrt] nl_hmac_sha_1, in_data_len:%d word_6:%08x \r\n", in_data_len, word_6);

    /* Key */
    memcpy((void *)&(u32aBase[28]), u8aKey, u8KeyLen);

    /* Write a word */
    *u32aStatus = SCRT_CTRL_WRITE_MSK(g_tScrtRes[u8ResId].u8MbIdx);
    // MAC: end

    // write done and ready to read
    if(scrt_status_chk(SCRT_STATUS_WRITE_MSK(g_tScrtRes[u8ResId].u8MbIdx) | SCRT_STATUS_READ_MSK(g_tScrtRes[u8ResId].u8MbIdx), 
                       SCRT_STATUS_READ_MSK(g_tScrtRes[u8ResId].u8MbIdx)))
    {
        SCRT_LOGE("[%s %d] scrt_status_chk fail\n", __func__, __LINE__);

        #ifdef SCRT_CHECK
        SCRT_ASSERT(0);
        #else
        goto done;
        #endif
    }

    u8NeedToClr = 1;

    // MAC output: start
    u32Output = u32aBase[0];

    if(u32Output != u16TokenId)
    {
        SCRT_LOGE("[%s %d] output token id[%08X] != SCRT_TOKEN_ID_AES_CMAC[%08X]\n", __func__, __LINE__, 
                  u32Output, u16TokenId);

        #ifdef SCRT_CHECK
        SCRT_ASSERT(0);
        #else
        goto done;
        #endif
    }

    /* Copy the output MAC data */
    memcpy((void *)u8aMac, (void *)&(u32aBase[2]), SCRT_AES_CMAC_OUTPUT_LEN);

    *u32aStatus = SCRT_CTRL_READ_MSK(g_tScrtRes[u8ResId].u8MbIdx);
    u8NeedToClr = 0;
    // MAC output: end

    status = 1;

done:
    if((u8aInput) && (u8aInput != u8aInputBuf))
    {
        SCRT_FREE(u8aInput);
    }

    if(u8ResId < SCRT_MB_IDX_MAX)
    {
        if(u8NeedToClr)
        {
            *u32aStatus = SCRT_CTRL_READ_MSK(g_tScrtRes[u8ResId].u8MbIdx);
        }

        #ifdef SCRT_ENABLE_UNLINK
        // Unlink: start
        if(u8Link)
        {
            if(scrt_unlink(g_tScrtRes[u8ResId].u8MbIdx))
            {
                SCRT_LOGE("[%s %d] scrt_unlink fail\n", __func__, __LINE__);
            }
        }
        // Unlink: end
        #endif
    
        // free resource
        scrt_res_free(u8ResId);
    }

    return status;
}

#if 0
/*
 * nl_hmac_sha_1_step - Perform the HMAC SHA1 Operation
 *
 * @param [in] type
 *      Operation type: 0: new/1: continue/2:final
 *
 * @param [in] total length
 *      Total Length. It's necessary for final operation.
 *
 * @param [in] sk
 *      The Key
 *
 * @param [in] sk_len
 *      The Key Length
 *
 * @param [in] in_data
 *      The Input Data which to be handled
 *
 * @param [in] in_data_len
 *      The length of Input Data
 *
 * @param [out] mac
 *      Output MAC Data
 *
 * @return 1 success
 *
 * @return 0 fail
 *
 */
int nl_scrt_hmac_sha_1_step_impl(uint8_t type, uint32_t total_len, uint8_t *sk, int sk_len, uint8_t *in_data, int in_data_len, uint8_t *mac)
{
    int status = 0;
    unsigned int word_6 = 0;
    volatile uint32_t *u32aBase = NULL;
    volatile uint32_t *u32aStatus = (uint32_t *)SCRT_STAT_CTRL_ADDR;
    uint32_t u32Output = 0;
    uint8_t u8ResId = SCRT_MB_IDX_MAX;
    uint16_t u16TokenId = 0;
    uint8_t u8NeedToClr = 0;

    uint8_t u8Mode = 0;
    uint32_t u32DataLen = 0;
    uint32_t u32BlkSize = 64; // must be power of 2

    #ifdef SCRT_ENABLE_UNLINK
    uint8_t u8Link = 0;
    #endif

    if(type >= SCRT_STEP_MAX)
    {
        SCRT_LOGE("[%s %d] invalid type[%d]\n", __func__, __LINE__, type);
        goto done;
    }

    if((!sk) || (!sk_len) || 
       (!in_data) || (!in_data_len) || 
       (!mac))
    {
        SCRT_LOGE("[%s %d] invalid param\n", __func__, __LINE__);
        goto done;
    }

    if(type == SCRT_STEP_FINAL)
    {
        u32DataLen = in_data_len;
    }
    else
    {
        if(in_data_len & (u32BlkSize - 1))
        {
            SCRT_LOGE("[%s %d] invalid length[%u] for non-final block\n", __func__, __LINE__, in_data_len);
            goto done;
        }

        u32DataLen = in_data_len;
    }

    u8ResId = scrt_res_alloc();

    if(u8ResId >= SCRT_MB_IDX_MAX)
    {
        SCRT_LOGE("[%s %d] scrt_res_alloc fail\n", __func__, __LINE__);
        goto done;
    }

    #ifdef SCRT_PRE_LINK
    #else
    // Link: start
    if(scrt_link(g_tScrtRes[u8ResId].u8MbIdx))
    {
        SCRT_LOGE("[%s %d] scrt_link fail\n", __func__, __LINE__);
        goto done;
    }

    #ifdef SCRT_ENABLE_UNLINK
    u8Link = 1;
    #endif
    // Link: end
    #endif

    u16TokenId = SCRT_TOKEN_ID_HMAC_SHA_1 + g_tScrtRes[u8ResId].u8MbIdx;
    u32aBase = (volatile uint32_t *)SCRT_BASE_ADDR(g_tScrtRes[u8ResId].u8MbIdx);

    // MAC: start
    u32aBase[0] = 0x03000000 | u16TokenId;
    u32aBase[1] = SCRT_ID;
    u32aBase[2] = u32DataLen;
    u32aBase[3] = (uint32_t)in_data;
    u32aBase[4] = 0x00000000;
    u32aBase[5] = in_data_len;

    switch(type)
    {
    case SCRT_STEP_NEW:
        u8Mode = 0x02;
        break;

    case SCRT_STEP_CONTINUE:
        u8Mode = 0x03;
        break;

    case SCRT_STEP_FINAL:
        u8Mode = 0x01;
        break;

    default:
        SCRT_ASSERT(0);
    }

    word_6 = ((sk_len & 0xff) << 16) | ((u8Mode & 0x03) << 4) | ((0x1) << 0);
    
    u32aBase[6] = word_6;      //key length: 0x08  ;  [3:0] Algorithm  HMAC-SHA-1, 160-bit MAC, block size is 64 Bytes
    u32aBase[7] = 0x00000000;

    if(type == SCRT_STEP_NEW)
    {
        // clear intermediate MAC (word 8 ~ 15)
        memset((void *)&(u32aBase[8]), 0, SCRT_SHA_1_INTER_MAC_LEN);
    }
    else
    {
        // copy intermediate MAC (word 8 ~ 15)
        memcpy((void *)&(u32aBase[8]), mac, SCRT_SHA_1_INTER_MAC_LEN);
    }

    u32aBase[16] = 0x00000000;
    u32aBase[17] = 0x00000000;
    u32aBase[18] = 0x00000000;
    u32aBase[19] = 0x00000000;
    u32aBase[20] = 0x00000000;
    u32aBase[21] = 0x00000000;
    u32aBase[22] = 0x00000000;
    u32aBase[23] = 0x00000000;

    //word 24
    if(type == SCRT_STEP_FINAL)
    {
        u32aBase[24] = total_len;
    }
    else
    {
        u32aBase[24] = 0x00000000;
    }

    u32aBase[25] = 0x00000000;
    
    u32aBase[26] = 0x00000000;
    u32aBase[27] = 0x00000000;

    //msg_print(LOG_HIGH_LEVEL, "[scrt] nl_hmac_sha_1, in_data_len:%d word_6:%08x \r\n", in_data_len, word_6);

    /* Key */
    memcpy((void *)&(u32aBase[28]), sk, sk_len);

    /* Write a word */
    *u32aStatus = SCRT_CTRL_WRITE_MSK(g_tScrtRes[u8ResId].u8MbIdx);
    // MAC: end

    // write done and ready to read
    if(scrt_status_chk(SCRT_STATUS_WRITE_MSK(g_tScrtRes[u8ResId].u8MbIdx) | SCRT_STATUS_READ_MSK(g_tScrtRes[u8ResId].u8MbIdx), 
                       SCRT_STATUS_READ_MSK(g_tScrtRes[u8ResId].u8MbIdx)))
    {
        SCRT_LOGE("[%s %d] scrt_status_chk fail\n", __func__, __LINE__);

        #ifdef SCRT_CHECK
        SCRT_ASSERT(0);
        #else
        goto done;
        #endif
    }

    u8NeedToClr = 1;

    // MAC output: start
    u32Output = u32aBase[0];

    if(u32Output != u16TokenId)
    {
        SCRT_LOGE("[%s %d] output token id[%08X] != SCRT_TOKEN_ID_HMAC_SHA_1[%08X]\n", __func__, __LINE__, 
                  u32Output, u16TokenId);

        #ifdef SCRT_CHECK
        SCRT_ASSERT(0);
        #else
        goto done;
        #endif
    }

    if(type == SCRT_STEP_FINAL)
    {
        /* Copy the output MAC data */
        memcpy((void *)mac, (void *)&(u32aBase[2]), SCRT_SHA_1_OUTPUT_LEN);
    }
    else
    {
        memcpy((void *)mac, (void *)&(u32aBase[2]), SCRT_SHA_1_INTER_MAC_LEN);
    }

    *u32aStatus = SCRT_CTRL_READ_MSK(g_tScrtRes[u8ResId].u8MbIdx);
    u8NeedToClr = 0;
    // MAC output: end

    status = 1;

done:
    if(u8ResId < SCRT_MB_IDX_MAX)
    {
        if(u8NeedToClr)
        {
            *u32aStatus = SCRT_CTRL_READ_MSK(g_tScrtRes[u8ResId].u8MbIdx);
        }

        #ifdef SCRT_ENABLE_UNLINK
        // Unlink: start
        if(u8Link)
        {
            if(scrt_unlink(g_tScrtRes[u8ResId].u8MbIdx))
            {
                SCRT_LOGE("[%s %d] scrt_unlink fail\n", __func__, __LINE__);
            }
        }
        // Unlink: end
        #endif
    
        // free resource
        scrt_res_free(u8ResId);
    }

    return status;
}
#endif

int nl_scrt_sha_impl(uint8_t u8Type, uint8_t u8Step, uint32_t u32TotalLen, uint8_t *u8aData, uint32_t u32DataLen, uint8_t u8HasInterMac, uint8_t *u8aMac)
{
    int status = 0;
    volatile uint32_t *u32aBase = NULL;
    volatile uint32_t *u32aStatus = (uint32_t *)SCRT_STAT_CTRL_ADDR;
    uint32_t u32Output = 0;
    uint8_t u8ResId = SCRT_MB_IDX_MAX;
    uint16_t u16TokenId = 0;
    uint8_t u8NeedToClr = 0;
    uint8_t u8Mode = 0;
    uint8_t u8Alg = 0;
    uint32_t u32StepSize = 0;
    uint32_t u32OutputLen = 0;

    #ifdef SCRT_ENABLE_UNLINK
    uint8_t u8Link = 0;
    #endif

    if(!u8aMac)
    {
        SCRT_LOGE("[%s %d] invalid param\n", __func__, __LINE__);
        goto done;
    }

    if((u32DataLen) && (!u8aData))
    {
        SCRT_LOGE("[%s %d] invalid data buffer\n", __func__, __LINE__);
        goto done;
    }

    switch(u8Type)
    {
        case SCRT_TYPE_SHA_1:
            u8Alg = 1;
            u32StepSize = SCRT_SHA_1_STEP_SIZE;
            u32OutputLen = SCRT_SHA_1_OUTPUT_LEN;
            break;
    
        case SCRT_TYPE_SHA_224:
            u8Alg = 2;
            u32StepSize = SCRT_SHA_224_STEP_SIZE;
            u32OutputLen = SCRT_SHA_224_OUTPUT_LEN;
            break;
    
        case SCRT_TYPE_SHA_256:
            u8Alg = 3;
            u32StepSize = SCRT_SHA_256_STEP_SIZE;
            u32OutputLen = SCRT_SHA_256_OUTPUT_LEN;
            break;
    
        case SCRT_TYPE_SHA_384:
            u8Alg = 4;
            u32StepSize = SCRT_SHA_384_STEP_SIZE;
            u32OutputLen = SCRT_SHA_384_OUTPUT_LEN;
            break;
    
        case SCRT_TYPE_SHA_512:
            u8Alg = 5;
            u32StepSize = SCRT_SHA_512_STEP_SIZE;
            u32OutputLen = SCRT_SHA_512_OUTPUT_LEN;
            break;
    
        default:
            SCRT_LOGE("[%s %d] unknown type[%d]\n", __func__, __LINE__, u8Type);
            goto done;
    }

    if(u8Step)
    {
        if(u32DataLen > u32StepSize)
        {
            SCRT_LOGE("[%s %d] invalid data_len[%u] > [%u]\n", __func__, __LINE__, u32DataLen, u32StepSize);
            goto done;
        }

        if(u8HasInterMac)
        {
            if(u32DataLen < u32StepSize)
            {
                // final
                u8Mode = 0x01;
            }
            else
            {
                // continue
                u8Mode = 0x03;
            }
        }
        else
        {
            if(u32DataLen < u32StepSize)
            {
                // initial and final
                u8Mode = 0x00;
            }
            else
            {
                // initial
                u8Mode = 0x02;
            }
        }
    }
    else
    {
        // initial and final
        u8Mode = 0x00;
    }

    u8ResId = scrt_res_alloc();

    if(u8ResId >= SCRT_MB_IDX_MAX)
    {
        SCRT_LOGE("[%s %d] scrt_res_alloc fail\n", __func__, __LINE__);
        goto done;
    }

    #ifdef SCRT_PRE_LINK
    #else
    // Link: start
    if(scrt_link(g_tScrtRes[u8ResId].u8MbIdx))
    {
        SCRT_LOGE("[%s %d] scrt_link fail\n", __func__, __LINE__);
        goto done;
    }

    #ifdef SCRT_ENABLE_UNLINK
    u8Link = 1;
    #endif
    // Link: end
    #endif

    u16TokenId = SCRT_TOKEN_ID_SHA + g_tScrtRes[u8ResId].u8MbIdx;
    u32aBase = (volatile uint32_t *)SCRT_BASE_ADDR(g_tScrtRes[u8ResId].u8MbIdx);

    // Hash: start
    u32aBase[0] = 0x02000000 | u16TokenId;
    u32aBase[1] = SCRT_ID;
    u32aBase[2] = u32DataLen;
    u32aBase[3] = (uint32_t)u8aData;
    u32aBase[4] = 0x00000000;
    u32aBase[5] = u32DataLen;
    u32aBase[6] = ((u8Mode & 0x03) << 4) | (u8Alg & 0x0F);
    u32aBase[7] = 0x00000000;

    if(u8Mode & 0x01) // continue
    {
        // copy intermediate MAC
        memcpy((void *)&(u32aBase[8]), u8aMac, u32OutputLen);
    }
    
    u32aBase[24] = u32TotalLen;
    u32aBase[25] = 0x00000000;

    /* Write a word */
    *u32aStatus = SCRT_CTRL_WRITE_MSK(g_tScrtRes[u8ResId].u8MbIdx);
    // MAC: end

    // write done and ready to read
    if(scrt_status_chk(SCRT_STATUS_WRITE_MSK(g_tScrtRes[u8ResId].u8MbIdx) | SCRT_STATUS_READ_MSK(g_tScrtRes[u8ResId].u8MbIdx), 
                       SCRT_STATUS_READ_MSK(g_tScrtRes[u8ResId].u8MbIdx)))
    {
        SCRT_LOGE("[%s %d] scrt_status_chk fail\n", __func__, __LINE__);

        #ifdef SCRT_CHECK
        SCRT_ASSERT(0);
        #else
        goto done;
        #endif
    }

    u8NeedToClr = 1;

    // Hash output: start
    u32Output = u32aBase[0];

    if(u32Output != u16TokenId)
    {
        SCRT_LOGE("[%s %d] output token id[%08X] != SCRT_TOKEN_ID_SHA[%08X]\n", __func__, __LINE__, 
                  u32Output, u16TokenId);

        #ifdef SCRT_CHECK
        SCRT_ASSERT(0);
        #else
        goto done;
        #endif
    }

    // intermediate or final mac
    memcpy((void *)u8aMac, (void *)&(u32aBase[2]), u32OutputLen);

    *u32aStatus = SCRT_CTRL_READ_MSK(g_tScrtRes[u8ResId].u8MbIdx);
    u8NeedToClr = 0;
    // Hash output: end

    status = 1;

done:
    if(u8ResId < SCRT_MB_IDX_MAX)
    {
        if(u8NeedToClr)
        {
            *u32aStatus = SCRT_CTRL_READ_MSK(g_tScrtRes[u8ResId].u8MbIdx);
        }

        #ifdef SCRT_ENABLE_UNLINK
        // Unlink: start
        if(u8Link)
        {
            if(scrt_unlink(g_tScrtRes[u8ResId].u8MbIdx))
            {
                SCRT_LOGE("[%s %d] scrt_unlink fail\n", __func__, __LINE__);
            }
        }
        // Unlink: end
        #endif
    
        // free resource
        scrt_res_free(u8ResId);
    }

    return status;
}

/*
 * scrt_drv_func_init - Interface Initialization: SCRT
 *
 */
void scrt_drv_func_init(void)
{
    // internal
    scrt_param_init = scrt_param_init_impl;
    scrt_mb_init = scrt_mb_init_impl;
    scrt_trng_init = scrt_trng_init_impl;
    scrt_eng_init = scrt_eng_init_impl;
    scrt_status_chk = scrt_status_chk_impl;
    scrt_ecdh_key_param_init = scrt_ecdh_key_param_init_impl;
    scrt_ecdh_priv_key_asset_create = scrt_ecdh_priv_key_asset_create_impl;
    scrt_ecdh_pub_key_asset_create = scrt_ecdh_pub_key_asset_create_impl;
    scrt_ecdh_shared_secret_asset_create = scrt_ecdh_shared_secret_asset_create_impl;
    scrt_asset_delete = scrt_asset_delete_impl;
    scrt_asset_load = scrt_asset_load_impl;
    scrt_asset_get = scrt_asset_get_impl;
    //scrt_reset = scrt_reset_impl;
    //scrt_internal_reset = scrt_internal_reset_impl;
    scrt_link = scrt_link_impl;

    #ifdef SCRT_ENABLE_UNLINK
    scrt_unlink = scrt_unlink_impl;
    #endif

    #ifdef SCRT_ACCESS_SEM
    scrt_access_lock = scrt_access_lock_impl;
    scrt_access_unlock = scrt_access_unlock_impl;
    #endif

    scrt_res_lock = scrt_res_lock_impl;
    scrt_res_unlock = scrt_res_unlock_impl;
    scrt_ecdh_lock = scrt_ecdh_lock_impl;
    scrt_ecdh_unlock = scrt_ecdh_unlock_impl;
    scrt_sem_create = scrt_sem_create_impl;
    scrt_res_alloc = scrt_res_alloc_impl;
    scrt_res_free = scrt_res_free_impl;
    scrt_clk_enable = scrt_clk_enable_impl;

    // external
    nl_scrt_Init = nl_scrt_init_impl;
    nl_aes_ccm = nl_aes_ccm_impl;
    nl_scrt_aes_ccm = nl_scrt_aes_ccm_impl;
    nl_hmac_sha_1 = nl_hmac_sha_1_impl;
    nl_scrt_aes_ecb = nl_scrt_aes_ecb_impl;
    nl_scrt_otp_status_get = nl_scrt_otp_status_get_impl;
    nl_scrt_ecdh_key_pair_gen = nl_scrt_ecdh_key_pair_gen_impl;
    nl_scrt_ecdh_dhkey_gen = nl_scrt_ecdh_dhkey_gen_impl;
    nl_scrt_key_delete = nl_scrt_key_delete_impl;
    nl_scrt_aes_cmac = nl_scrt_aes_cmac_impl;
    //nl_scrt_hmac_sha_1_step = nl_scrt_hmac_sha_1_step_impl;
    nl_scrt_sha = nl_scrt_sha_impl;

    #ifdef SCRT_CMD
    nl_scrt_cmd_func_init();
    #endif
    
    return;
}

#else

/*
 * delay - Perform a delay.
 *
 * @param [in] times  The times to do __NOP
 *
 */
void delay(int times){
    int i;
    for(i=0; i<times; i++){
        __NOP();
    }
}


/*
 * nl_scrt_init_impl - Perform the SCRT initialization.
 *
 * @return N/A
 *
 */
int nl_scrt_init_impl(void)
{
    int rc;

	msg_print(LOG_HIGH_LEVEL, "[security] nl_scrt_Init \r\n");
    rc = vex_Init();
    return rc;
}


/*
 * nl_aes_ccm  - AES CCM Operation. [deprecated]
 *
 * @param [in] bEncrypt
 *    true:encrypt false:decrypt
 *
 * @param [in] sk
 *    Key
 *
 * @param [in] sk_len
 *    Size of Key
 *
 * @param [in] nonce
 *    Nonce
 *
 * @param [in] nonce_len
 *    Size of Nonce
 *
 * @param [in] adata
 *    Additional Associated Data
 *
 * @param [in] adata_len
 *    Size of Additional Associated Data
 *
 * @param [in/out] plain_text
 *    Plain Text Data
 *
 * @param [in/out] encrypted_text
 *    Encrypted Data
 *
 * @param [in] text_len
 *    The length of the Data Transfer
 *
 * @param [out] tag
 *    Output Tag Data
 *
 * @param [in] tag_len
 *    Size of the Output Tag Data
 *
 * @return 1 success
 *
 * @return 0 fail
 *
 * @deprecated It will be removed in the future
 *
 */
int nl_aes_ccm_impl (bool bEncrypt, uint8_t *sk, int sk_len, uint8_t *nonce, int nonce_len, unsigned int adata, int adata_len, unsigned int plain_text, unsigned int encrypted_text, int text_len, uint8_t *tag, int tag_len)
{
    unsigned int token_id_output;
    unsigned int token_id_current = 0xd82c;
    unsigned int data = 0;
    unsigned int word_11;
    bool status = true;
    int i;
    int quo, rem;
    int buf_len;

    quo = (text_len / 16);
    rem = (text_len % 16);
    if(rem == 0) {
        buf_len = (quo * 16);
    } else {
        buf_len = ((quo + 1) * 16);
    }

    //msg_print(LOG_HIGH_LEVEL, "\r\n[scrt]quo:%d rem:%d buf_len:%d \r\n", quo, rem, buf_len);

    /*
         SCRT Control- Link Mailbox
       */
    *(volatile unsigned int *)0x60003f00 = 0x4;
    //*(volatile unsigned int *)0x60003f00 = 0x40;
    //*(volatile unsigned int *)0x60003f00 = 0x400;
    //*(volatile unsigned int *)0x60003f00 = 0x4000;

    if (bEncrypt)
    {
        /*
                SCRT Control- Check Mailbox Status
             */
        while(1){
            data = *((volatile unsigned int *)0x60003f00);
            if (( data & 0x1 ) == 0x0) {
                //success
                break;
            } else {
                continue;
            }
            delay(1);
        }

        /* Burst write 37 words */
        *(volatile unsigned int *)0x60000000 = 0x0104d82c;
        *(volatile unsigned int *)0x60000004 = 0x00000000;
        *(volatile unsigned int *)0x60000008 = text_len;                            //The length of the data transfer   //0x10
        *(volatile unsigned int *)0x6000000c = (unsigned int) plain_text;           //addr_input_data;//plain_text;//(unsigned int) plain_text;           //Input Data                                 //0x00401000
        *(volatile unsigned int *)0x60000010 = 0x00000000;                          //Input Data
        *(volatile unsigned int *)0x60000014 = buf_len;                            //Input Data Length                     //0x10
        *(volatile unsigned int *)0x60000018 = (unsigned int) encrypted_text;       //Output Data                              //0x00401500
        *(volatile unsigned int *)0x6000001c = 0x00000000;                          //Output Data
        *(volatile unsigned int *)0x60000020 = (buf_len + 4);                      //Output Data Length                  //0x14
        *(volatile unsigned int *)0x60000024 = (unsigned int) adata;                //Associated Data                        //0x00402000
        *(volatile unsigned int *)0x60000028 = 0x00000000;                          //Associated Data
        //*(volatile unsigned int *)0x6000002c = 0x10d18050;                                                //0x10d18050;                           //Nonce length              => word 11[23:20]       => 13  => 0xd
                                                                                    //Key length             => word 11[19:16]    => 1 (1 means 128 bits key)
                                                                                    //Tag length             => word 11[28:24]    5 bits => 0x10  => 16
                                                                                    //Encrypt or Decrypt  => word11[15]         => 1 (1 means encrypt)

        word_11 = ((tag_len & 0xff) << 24) | ((((nonce_len & 0xf) << 4) | (0x1)) << 16) | ((0x80) << 8) | ((0x50) << 0);
        //msg_print(LOG_HIGH_LEVEL, "[scrt]text_len:%d word_11:%08x \r\n", text_len, word_11);
        *(volatile unsigned int *)0x6000002c = word_11;
        *(volatile unsigned int *)0x60000030 = 0x00000000;
        *(volatile unsigned int *)0x60000034 = 0x00000000;
        *(volatile unsigned int *)0x60000038 = 0x00000000;
        *(volatile unsigned int *)0x6000003c = 0x00000000;
        *(volatile unsigned int *)0x60000040 = 0x00000000;

        /* Key */
        for (i=0; i<sk_len; i++) {
            *(volatile unsigned int *)(0x60000044 + i) = *(sk + i);
        }

        *(volatile unsigned int *)0x60000054 = 0x00000000;          //key
        *(volatile unsigned int *)0x60000058 = 0x00000000;          //key
        *(volatile unsigned int *)0x6000005c = 0x00000000;          //key
        *(volatile unsigned int *)0x60000060 = 0x00000000;          //key
        *(volatile unsigned int *)0x60000064 = adata_len;           //Associated Data Length
        *(volatile unsigned int *)0x60000068 = 0x00000000;
        *(volatile unsigned int *)0x6000006c = 0x00000000;
        *(volatile unsigned int *)0x60000070 = 0x00000000;

        /* Nonce */
        for (i=0; i<nonce_len; i++) {
            *(volatile unsigned int *)(0x60000074 + i) = *(nonce + i);
        }

        *(volatile unsigned int *)0x60000084 = 0x00000000;
        *(volatile unsigned int *)0x60000088 = 0x00000000;
        *(volatile unsigned int *)0x6000008c = 0x00000000;
        *(volatile unsigned int *)0x60000090 = 0x00000000;

        /*
                SCRT Control- Write Token
             */
        *(volatile unsigned int *)0x60003f00 = 0x1;

        /*
                SCRT Control- Check Operation Status
             */
        while(1){
            data = *((volatile unsigned int *)0x60003f00);
            if (( data & 0x2 ) == 0x2) {
                //success
                break;
            } else {
                continue;
            }
            delay(1);
        }

        /*
                SCRT Control- Check output token status
             */
        token_id_output = *((volatile unsigned int *)0x60000000);
        if (token_id_output != token_id_current) {
            status = false;
            //msg_print(LOG_HIGH_LEVEL, "[scrt]output token status fail \r\n");
        }

        /*
                SCRT Control- Check Token ID in output data's last word
             */
        token_id_output = *((volatile unsigned int *)(encrypted_text + buf_len));
        if (token_id_output != token_id_current) {
            status = false;
            //msg_print(LOG_HIGH_LEVEL, "[scrt]output data's last word's token id fail \r\n");
        }

        /*
                Copy the output tag data to the buffer "tag"
             */
        os_memcpy((void *)tag, (void *)0x60000018, tag_len);

        /*
                SCRT Control- Clear Status
             */
        *(volatile unsigned int *)0x60003f00 = 0x2;

        //msg_print(LOG_HIGH_LEVEL, "[scrt]encryption status:%s \r\n", (status==true)?"success":"fail");
    }
    else
    {
        /*
                SCRT Control- Check Mailbox Status
             */
        while(1){
            data = *((volatile unsigned int *)0x60003f00);
            if (( data & 0x1 ) == 0x0) {
                //success
                break;
            } else {
                continue;
            }
            delay(1);
        }

        //Burst write 37 words
        *(volatile unsigned int *)0x60000000 = 0x0104d82c;
        *(volatile unsigned int *)0x60000004 = 0x00000000;
        *(volatile unsigned int *)0x60000008 = text_len;                            //The length of the data transfer           //0x10
        *(volatile unsigned int *)0x6000000c = (unsigned int) encrypted_text;       //addr_input_data;//plain_text;          //(unsigned int) plain_text;           //Input Data      //0x00401000
        *(volatile unsigned int *)0x60000010 = 0x00000000;                          //Input Data
        *(volatile unsigned int *)0x60000014 = buf_len;                            //Input Data Length                              //0x10
        *(volatile unsigned int *)0x60000018 = (unsigned int) plain_text;           //Output Data                                        //0x00401500
        *(volatile unsigned int *)0x6000001c = 0x00000000;                          //Output Data
        *(volatile unsigned int *)0x60000020 = (buf_len + 4);                      //Output Data Length                             //0x14
        *(volatile unsigned int *)0x60000024 = (unsigned int) adata;                //Associated Data                                   //0x00402000
        *(volatile unsigned int *)0x60000028 = 0x00000000;                          //Associated Data
        //*(volatile unsigned int *)0x6000002c = 0x10d10050;                                                //0x10d18050;                               //Nonce length              => word 11[23:20]       => 13  => 0xd
                                                                                    //Key length             => word 11[19:16]    => 1 (1 means 128 bits key)
                                                                                    //Tag length             => word 11[28:24]    5 bits => 0x10  => 16
                                                                                    //Encrypt or Decrypt  => word11[15]         => 0 (0 means decrypt)

        word_11 = ((tag_len & 0xff) << 24) | ((((nonce_len & 0xf) << 4) | (0x1)) << 16) | ((0x0) << 8) | ((0x50) << 0);
        //msg_print(LOG_HIGH_LEVEL, "[scrt]text_len:%d word_11:%08x \r\n", text_len, word_11);
        *(volatile unsigned int *)0x6000002c = word_11;

        *(volatile unsigned int *)0x60000030 = 0x00000000;
        *(volatile unsigned int *)0x60000034 = 0x00000000;
        *(volatile unsigned int *)0x60000038 = 0x00000000;
        *(volatile unsigned int *)0x6000003c = 0x00000000;
        *(volatile unsigned int *)0x60000040 = 0x00000000;

        //Key
        for (i=0; i<sk_len; i++) {
            *(volatile unsigned int *)(0x60000044 + i) = *(sk + i);
        }

        *(volatile unsigned int *)0x60000054 = 0x00000000;          //key
        *(volatile unsigned int *)0x60000058 = 0x00000000;          //key
        *(volatile unsigned int *)0x6000005c = 0x00000000;          //key
        *(volatile unsigned int *)0x60000060 = 0x00000000;          //key
        *(volatile unsigned int *)0x60000064 = adata_len;           //Associated Data Length
        *(volatile unsigned int *)0x60000068 = 0x00000000;
        *(volatile unsigned int *)0x6000006c = 0x00000000;
        *(volatile unsigned int *)0x60000070 = 0x00000000;

        //Nonce
        for (i=0; i<nonce_len; i++) {
            *(volatile unsigned int *)(0x60000074 + i) = *(nonce + i);
        }

        //Tag to verify
        for (i=0; i<tag_len; i++) {
            *(volatile unsigned int *)(0x60000084 + i) = *(tag + i);
        }

        /*
                SCRT Control- Write Token
            */
        *(volatile unsigned int *)0x60003f00 = 0x1;

        /*
                SCRT Control- Check Operation Status
            */
        while(1){
            data = *((volatile unsigned int *)0x60003f00);
            if (( data & 0x2 ) == 0x2) {
                //success
                break;
            } else {
                continue;
            }
            delay(1);
        }

        //Check output token status
        token_id_output = *((volatile unsigned int *)0x60000000);
        if (token_id_output != token_id_current) {
            status = false;
            //msg_print(LOG_HIGH_LEVEL, "[scrt]output token status fail \r\n");
        }

        //Check Token ID in output data's last word
        token_id_output = *((volatile unsigned int *)(plain_text + buf_len)); //0x00401500 + 0x10
        if (token_id_output != token_id_current) {
            status = false;
            //msg_print(LOG_HIGH_LEVEL, "[scrt]output data's last word's token id fail \r\n");
        }

        /*
                SCRT Control- Clear Status
            */
        *(volatile unsigned int *)0x60003f00 = 0x2;

        //msg_print(LOG_HIGH_LEVEL, "[scrt]decryption status:%s \r\n", (status==true)?"success":"fail");
    }


func_return:
    /* Unlink MailBox */
    *(volatile unsigned int *)0x60003f00 = 0x8;

    return status;
}


/*
 * nl_scrt_aes_ccm - Perform the AES CCM Operation.
 *
 * @param [in] bEncrypt
 *    1:encrypt 0:decrypt
 *
 * @param [in] sk
 *    Key
 *
 * @param [in] sk_len
 *    Size of Key
 *
 * @param [in] nonce
 *    Nonce
 *
 * @param [in] nonce_len
 *    Size of Nonce
 *
 * @param [in] adata
 *    Additional Associated Data
 *
 * @param [in] adata_len
 *    Size of Additional Associated Data
 *
 * @param [in/out] plain_text
 *    Plain Text Data
 *
 * @param [in/out] encrypted_text
 *    Encrypted Data
 *
 * @param [in] text_len
 *    The length of the Data Transfer
 *
 * @param [out] tag
 *    Output Tag Data
 *
 * @param [in] tag_len
 *    Size of the Output Tag Data
 *
 * @return 1 success
 *
 * @return 0 fail
 *
 */
int nl_scrt_aes_ccm_impl (int bEncrypt, unsigned char *sk, int sk_len, unsigned char *nonce, int nonce_len, unsigned char *adata, int adata_len, unsigned char *plain_text, unsigned char *encrypted_text, int text_len, unsigned char *tag, int tag_len)
{
    unsigned int token_id_output;
    unsigned int token_id_current = 0xd82c;
    unsigned int data = 0;
    unsigned int word_11;
    int status = 1;
    int i;
    int quo, rem;
    int buf_len;
    unsigned int adata_m;
    unsigned int plain_m;
    unsigned int cipher_m;
    int cnt = 0;

    quo = (text_len / 16);
    rem = (text_len % 16);
    if(rem == 0) {
        buf_len = (quo * 16);
    } else {
        buf_len = ((quo + 1) * 16);
    }

#if 0 //Remapping for m0 usage
    adata_m = (unsigned int)adata | 0x20000000;
    plain_m = (unsigned int)plain_text | 0x20000000;
    cipher_m = (unsigned int)encrypted_text | 0x20000000;
#else //For M3 usage
    adata_m = (unsigned int)adata;
    plain_m = (unsigned int)plain_text;
    cipher_m = (unsigned int)encrypted_text;
#endif

    //printf("\r\n sk_len:%d nonce_len:%d adata_len:%d text_len:%d tag_len:%d \r\n", sk_len, nonce_len, adata_len, text_len, tag_len);
    //printf("\r\n quo:%d rem:%d buf_len:%d \r\n", quo, rem, buf_len);
    //printf("\r\n adata_m:%lx plain_m:%lx cipher_m:%lx \r\n", adata_m, plain_m, cipher_m);

    /*
        SCRT Control- Link Mailbox
      */
    *(volatile unsigned int *)0x60003f00 = 0x4;

    /*
        SCRT Control- Check Mailbox Status
      */
    while(1){
        delay(1);
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }

    /*Burst write 37 words*/
    *(volatile unsigned int *)0x60000000 = 0x0104d82c;
    *(volatile unsigned int *)0x60000004 = 0x00000000;
    *(volatile unsigned int *)0x60000008 = text_len;                            //The length of the data transfer

    if (bEncrypt == 1) {
        *(volatile unsigned int *)0x6000000c = plain_m;                         //Input Data
    } else {
        *(volatile unsigned int *)0x6000000c = cipher_m;                        //Input Data
    }

    *(volatile unsigned int *)0x60000010 = 0x00000000;                          //Input Data
    *(volatile unsigned int *)0x60000014 = buf_len;                             //Input Data Length

    if (bEncrypt == 1) {
        *(volatile unsigned int *)0x60000018 = cipher_m;                       //Output Data
    } else {
        *(volatile unsigned int *)0x60000018 = plain_m;                        //Output Data
    }

    *(volatile unsigned int *)0x6000001c = 0x00000000;                          //Output Data
    *(volatile unsigned int *)0x60000020 = (buf_len + 4);                       //Output Data Length
    *(volatile unsigned int *)0x60000024 = adata_m;                            //Associated Data
    *(volatile unsigned int *)0x60000028 = 0x00000000;                          //Associated Data
    //*(volatile unsigned int *)0x6000002c = 0x10d18050;                                                //0x10d18050;           //Nonce length               => word 11[23:20]  => 13  => 0xd
                                                                                //Key length             => word 11[19:16]           => 1 (1 means 128 bits key)
                                                                                //Tag length             => word 11[28:24] 5 bits  => 0x10  => 16
                                                                                //Encrypt or Decrypt  => word11[15]                => 1:encrypt 0:decrypt

    if (bEncrypt == 1) {
        word_11 = ((tag_len & 0xff) << 24) | ((((nonce_len & 0xf) << 4) | (0x1)) << 16) | ((0x80) << 8) | ((0x50) << 0);
    } else {
        word_11 = ((tag_len & 0xff) << 24) | ((((nonce_len & 0xf) << 4) | (0x1)) << 16) | ((0x0) << 8) | ((0x50) << 0);
    }

    //printf("\r\n text_len:%d word_11:%08x \r\n", text_len, word_11);

    *(volatile unsigned int *)0x6000002c = word_11;
    *(volatile unsigned int *)0x60000030 = 0x00000000;
    *(volatile unsigned int *)0x60000034 = 0x00000000;
    *(volatile unsigned int *)0x60000038 = 0x00000000;
    *(volatile unsigned int *)0x6000003c = 0x00000000;
    *(volatile unsigned int *)0x60000040 = 0x00000000;

    memcpy((void *)0x60000044, sk, sk_len);

    *(volatile unsigned int *)0x60000054 = 0x00000000;          //key
    *(volatile unsigned int *)0x60000058 = 0x00000000;          //key
    *(volatile unsigned int *)0x6000005c = 0x00000000;          //key
    *(volatile unsigned int *)0x60000060 = 0x00000000;          //key
    *(volatile unsigned int *)0x60000064 = adata_len;           //Associated Data Length
    *(volatile unsigned int *)0x60000068 = 0x00000000;
    *(volatile unsigned int *)0x6000006c = 0x00000000;
    *(volatile unsigned int *)0x60000070 = 0x00000000;

    memcpy((void *)0x60000074, nonce, nonce_len);

    if (bEncrypt == 1) {
        *(volatile unsigned int *)0x60000084 = 0x00000000;
        *(volatile unsigned int *)0x60000088 = 0x00000000;
        *(volatile unsigned int *)0x6000008c = 0x00000000;
        *(volatile unsigned int *)0x60000090 = 0x00000000;
    } else {
        memcpy((void *)0x60000084, tag, tag_len);
    }

    /*
            SCRT Control- Write Token
        */
    *(volatile unsigned int *)0x60003f00 = 0x1;

    /*
            SCRT Control- Check Operation Status
        */
    while(1){
        cnt++;
        delay(INTERVAL_EACH_CHECKING);
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
        if (cnt > MAX_TIMES_CHECKING)
        {
            status = 0;
            goto func_return;
        }
    }

    /*
        SCRT Control- Check output token status
       */
    token_id_output = *((volatile unsigned int *)0x60000000);
    if (token_id_output != token_id_current) {
        status = 0;
        //printf("\r\n output token status fail \r\n");
    }

    /*
        SCRT Control- Check Token ID in output data's last word
       */
    if (bEncrypt == 1) {
        token_id_output = *((volatile unsigned int *)(encrypted_text + buf_len));
    } else {
        token_id_output = *((volatile unsigned int *)(plain_text + buf_len));
    }

    if (token_id_output != token_id_current) {
        status = 0;
        //printf("\r\n output data's last word's token id fail \r\n");
    }

    if (bEncrypt == 1) {
        /* Copy the output tag data to the buffer "tag" */
        memcpy((void *)tag, (void *)0x60000018, tag_len);
    }

    /*
            SCRT Control- Clear Status
       */
    *(volatile unsigned int *)0x60003f00 = 0x2;

    //printf("[scrt]encryption status:%s cnt:%d \r\n", (status==1)?"success":"fail", cnt);
    return status;

func_return:
    /* Unlink MailBox */
    *(volatile unsigned int *)0x60003f00 = 0x8;

    return status;
}
#endif

/*
 * aes_ccm_sample - Sample code for AES CCM output 4 bytes tag
 *
 */
void aes_ccm_sample(void)
{
    unsigned char sk[16] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10};
    unsigned char nonce[13] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d};
    unsigned char a_data[16] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10};
    unsigned char plain_text[16] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10};
    unsigned char cipher_text[16] = {0};
    unsigned char tag[4] = {0};
    unsigned char plain_output[16] = {0};

    int i;

    printf("\r\n do aes ccm encryption \r\n");

    memset(cipher_text, 0, 16);
    memset(tag, 0, 8);
    memset(plain_output, 0, 16);

    //AES CCM Encryption
    nl_scrt_aes_ccm(1, sk, sizeof(sk), nonce, sizeof(nonce), a_data, sizeof(a_data), plain_text, cipher_text, sizeof(plain_text), tag, sizeof(tag));

    //Show output tag data
    for(i=0; i < sizeof(tag) ;i++) {
        printf("output tag[%d]:%02x \r\n", i, tag[i]);
    }

    //Show output cipher data (size of cipher data is the same as size of plain text)
    for(i=0; i < sizeof(cipher_text); i++) {
        printf("output cipher[%d]:%02x \r\n", i, *((volatile char *)(cipher_text + i)));
    }

    printf("\r\n do aes ccm decryption \r\n");

    //AES CCM Decryption
    nl_scrt_aes_ccm(0, sk, sizeof(sk), nonce, sizeof(nonce), a_data, sizeof(a_data), plain_output, cipher_text, sizeof(cipher_text), tag, sizeof(tag));

    //Show output plain text data
    for(i=0; i<sizeof(plain_text); i++) {
       printf("output plain text[%d]:%02x \r\n", i, plain_output[i]);
    }
}

#if 0
/*
 * nl_hmac_sha_1_impl - Perform the HMAC SHA1 Operation
 *
 * @param [in] sk
 *      The Key
 *
 * @param [in] sk_len
 *      The Key Length
 *
 * @param [in] in_data
 *      The Input Data which to be handled
 *
 * @param [in] in_data_len
 *      The length of Input Data
 *
 * @param [out] mac
 *      Output MAC Data
 *
 * @return 1 success
 *
 * @return 0 fail
 *
 */
int nl_hmac_sha_1_impl(uint8_t *sk, int sk_len, uint8_t *in_data, int in_data_len, uint8_t *mac)
{
    unsigned int data                   = 0;
    unsigned int c_output_token_id      = 0x9d37;
    unsigned int r_output_token_data    = 0;
    bool status = true;
    unsigned int word_6;
    int i;
    int cnt = 0;

    /*
        SCRT Control- Link Mailbox
      */
    *(volatile unsigned int *)0x60003f00 = 0x4;

    /*
        SCRT Control- Check Mailbox Status
      */
    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
        delay(1);
    }

    /* Write Token to mailbox */
    *(volatile unsigned int *)0x60000000 = 0x03009d37;
    *(volatile unsigned int *)0x60000004 = 0x00000000;
    *(volatile unsigned int *)0x60000008 = in_data_len; //The length of the input data (in bytes).
    *(volatile unsigned int *)0x6000000c = (unsigned int) in_data;
    *(volatile unsigned int *)0x60000010 = 0x00000000;
    *(volatile unsigned int *)0x60000014 = in_data_len;

    //*(volatile unsigned int *)0x60000018 = 0x00200001;
    word_6 = ((sk_len & 0xff) << 16) | ((0x1) << 0);
    *(volatile unsigned int *)0x60000018 = word_6;      //key length: 0x08  ;  [3:0] Algorithm  HMAC-SHA-1, 160-bit MAC, block size is 64 Bytes

    *(volatile unsigned int *)0x6000001c = 0x00000000;
    *(volatile unsigned int *)0x60000020 = 0x00000000;
    *(volatile unsigned int *)0x60000024 = 0x00000000;
    *(volatile unsigned int *)0x60000028 = 0x00000000;
    *(volatile unsigned int *)0x6000002c = 0x00000000;
    *(volatile unsigned int *)0x60000030 = 0x00000000;
    *(volatile unsigned int *)0x60000034 = 0x00000000;
    *(volatile unsigned int *)0x60000038 = 0x00000000;
    *(volatile unsigned int *)0x6000003c = 0x00000000;
    *(volatile unsigned int *)0x60000040 = 0x00000000;
    *(volatile unsigned int *)0x60000044 = 0x00000000;
    *(volatile unsigned int *)0x60000048 = 0x00000000;
    *(volatile unsigned int *)0x6000004c = 0x00000000;
    *(volatile unsigned int *)0x60000050 = 0x00000000;
    *(volatile unsigned int *)0x60000054 = 0x00000000;
    *(volatile unsigned int *)0x60000058 = 0x00000000;
    *(volatile unsigned int *)0x6000005c = 0x00000000;

    //word 24
    *(volatile unsigned int *)0x60000060 = in_data_len;

    *(volatile unsigned int *)0x60000064 = 0x00000000;
    *(volatile unsigned int *)0x60000068 = 0x00000000;
    *(volatile unsigned int *)0x6000006c = 0x00000000;

    //msg_print(LOG_HIGH_LEVEL, "[scrt] nl_hmac_sha_1, in_data_len:%d word_6:%08x \r\n", in_data_len, word_6);

    /* Key */
     for (i=0; i<sk_len; i++) {
         *(volatile unsigned int *)(0x60000070 + i) = *(sk + i);
     }

    /* Write a word */
    *(volatile unsigned int *)0x60003f00 = 0x1;

    /*
            SCRT Control- Check Operation Status
        */
    while(1){
        cnt++;
        delay(INTERVAL_EACH_CHECKING);
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
        if (cnt > MAX_TIMES_CHECKING)
        {
            status = 0;
            goto func_return;
        }
    }

    /*
        SCRT Control- Check output token status
       */
    r_output_token_data = *((volatile unsigned int *)0x60000000);
    if (r_output_token_data != c_output_token_id) {
        status = false;
        //msg_print(LOG_HIGH_LEVEL, "[scrt] nl_hmac_sha_1, output token status fail \r\n");
    }

    /* Copy the output MAC data */
    os_memcpy((void *)mac, (void *)0x60000008, 20);

    /*
            SCRT Control- Clear Status
       */
    *(volatile unsigned int *)0x60003f00 = 0x2;

    return status;
func_return:
    /* Unlink MailBox */
    *(volatile unsigned int *)0x60003f00 = 0x8;

    return status;
}
#endif

/*
 * hmac_sha_1_sample - Sample code to do HMAC SHA1
 *
 */
void hmac_sha_1_sample(void)
{
    uint8_t passphrase[8] = {0x32, 0x32, 0x32, 0x32,
                             0x32, 0x32, 0x32, 0x32};
    uint8_t data[20] = {0xad, 0x82, 0x16, 0xc9,
                        0x63, 0x59, 0x33, 0xfd,
                        0x82, 0xa7, 0x6b, 0xad,
                        0x69, 0xe4, 0x51, 0x81,
                        0xa1, 0x18, 0x37, 0xb7
                        };
    uint8_t mac[20] = {0};
    int i;

    msg_print(LOG_HIGH_LEVEL, "\r\nhmac_sha_1_sample\r\n");
    os_memset(mac, 0, sizeof(mac));

    nl_hmac_sha_1(passphrase, sizeof(passphrase), data, sizeof(data), mac);

    /* Show output MAC */
    for(i=0; i < sizeof(mac) ;i++) {
        msg_print(LOG_HIGH_LEVEL, "hmac_sha_1_sample, output mac[%d]:%02x \r\n", i, mac[i]);
    }
}

#if 0
/*
 * nl_scrt_aes_cmac
 *
 * AES-CMAC Message Authentication Code operation.
 *
 * @param [in] sk
 *    Pointer to the key structure of the key.
 *
 * @param [in] sk_len
 *    Size of the key.
 *
 * @param [in] data_in
 *    Pointer to the input message data.
 *
 * @param [in] data_in_len
 *    Size of the input message data.
 *
 * @param [out] data_out
 *    Pointer to the output MAC data.
 *
 * @param [in] data_out_len
 *    Size of the output MAC data.
 *
 */
int
nl_scrt_aes_cmac( unsigned char *sk, int sk_len, unsigned char *data_in, int data_in_len, unsigned char *data_out, int data_out_len)
{ //not finish yet
    unsigned int data                   = 0;
    unsigned int c_output_token_id      = 0x9d37;
    unsigned int r_output_token_data    = 0;

    //unsigned int sk_m;
    //unsigned int data_in_m;

    int quo, rem;
    int buf_in_len;
    int key_len = 1;
    //int buf_out_len;

    int status = 1;
    int cnt = 0;

    unsigned int word_6;

    //printf("\r\n nl_scrt_aes_cmac, sk:%lx data_in:%lx data_out:%lx \r\n", sk, data_in, data_out);

    //sk_m = (unsigned int)sk;
    //data_in_m = (unsigned int)data_in;

    if(sk == 0||
       data_in == 0 ||
       data_out == 0){
        return 0;
    }

    //printf("\r\n nl_scrt_aes_cmac, sk_m:%lx \r\n", sk_m);
    //printf("\r\n nl_scrt_aes_cmac, data_in_m:%lx \r\n", data_in_m);
    //printf("\r\n nl_scrt_aes_cmac, data_out_m:%lx \r\n", data_out_m);

    if(sk_len != 16 &&
       sk_len != 24 &&
       sk_len != 32) {
        return 0;
    }

    if(data_in_len <= 0 || data_out_len <= 0) {
        return 0;
    }

    //It should be a multiple of block size (16 bytes)
    quo = (data_in_len / 16);
    rem = (data_in_len % 16);
    if(rem == 0) {
        buf_in_len = (quo * 16);
    } else {
        buf_in_len = ((quo + 1) * 16);
    }

    //printf("\r\n nl_scrt_aes_cmac, data_in_len:%d \r\n", data_in_len);
    //printf("\r\n nl_scrt_aes_cmac, buf_in_len:%d \r\n", buf_in_len);

    *(volatile unsigned int *)0x60003f00 = 0x4;

    while(1){
        delay(3);
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }

    *(volatile unsigned int *)0x60000000 = 0x03009d37;
    *(volatile unsigned int *)0x60000004 = 0x00000000;
    *(volatile unsigned int *)0x60000008 = buf_in_len;
    *(volatile unsigned int *)0x6000000c = (unsigned int)data_in;
    *(volatile unsigned int *)0x60000010 = 0x00000000;
    *(volatile unsigned int *)0x60000014 = buf_in_len;

    //*(volatile unsigned int *)0x60000018 = 0x00100008;
    word_6 = (sk_len << 16) | 0x0008;
    *(volatile unsigned int *)0x60000018 = word_6;
    //printf("\r\n nl_scrt_aes_cmac, word_6:%lx \r\n", word_6);

    *(volatile unsigned int *)0x6000001c = 0x00000000;
    *(volatile unsigned int *)0x60000020 = 0x00000000;
    *(volatile unsigned int *)0x60000024 = 0x00000000;
    *(volatile unsigned int *)0x60000028 = 0x00000000;
    *(volatile unsigned int *)0x6000002c = 0x00000000;
    *(volatile unsigned int *)0x60000030 = 0x00000000;
    *(volatile unsigned int *)0x60000034 = 0x00000000;
    *(volatile unsigned int *)0x60000038 = 0x00000000;
    *(volatile unsigned int *)0x6000003c = 0x00000000;
    *(volatile unsigned int *)0x60000040 = 0x00000000;
    *(volatile unsigned int *)0x60000044 = 0x00000000;
    *(volatile unsigned int *)0x60000048 = 0x00000000;
    *(volatile unsigned int *)0x6000004c = 0x00000000;
    *(volatile unsigned int *)0x60000050 = 0x00000000;
    *(volatile unsigned int *)0x60000054 = 0x00000000;
    *(volatile unsigned int *)0x60000058 = 0x00000000;
    *(volatile unsigned int *)0x6000005c = 0x00000000;
    *(volatile unsigned int *)0x60000060 = 0x00000000;
    *(volatile unsigned int *)0x60000064 = 0x00000000;
    *(volatile unsigned int *)0x60000068 = 0x00000000;
    *(volatile unsigned int *)0x6000006c = 0x00000000;

    //key, word 28~57
    memcpy((void *)0x60000070, (void *)sk, sk_len);

    if (sk_len == 16) {
        *(volatile unsigned int *)0x60000080 = 0x00000000;
        *(volatile unsigned int *)0x60000084 = 0x00000000;
        *(volatile unsigned int *)0x60000088 = 0x00000000;
        *(volatile unsigned int *)0x6000008c = 0x00000000;
    } else if (sk_len == 24) {
        *(volatile unsigned int *)0x60000088 = 0x00000000;
        *(volatile unsigned int *)0x6000008c = 0x00000000;
    }

    *(volatile unsigned int *)0x60000090 = 0x00000000;
    *(volatile unsigned int *)0x60000094 = 0x00000000;
    *(volatile unsigned int *)0x60000098 = 0x00000000;
    *(volatile unsigned int *)0x6000009c = 0x00000000;
    *(volatile unsigned int *)0x600000a0 = 0x00000000;
    *(volatile unsigned int *)0x600000a4 = 0x00000000;
    *(volatile unsigned int *)0x600000a8 = 0x00000000;
    *(volatile unsigned int *)0x600000ac = 0x00000000;
    *(volatile unsigned int *)0x600000b0 = 0x00000000;
    *(volatile unsigned int *)0x600000b4 = 0x00000000;
    *(volatile unsigned int *)0x600000b8 = 0x00000000;
    *(volatile unsigned int *)0x600000bc = 0x00000000;
    *(volatile unsigned int *)0x600000c0 = 0x00000000;
    *(volatile unsigned int *)0x600000c4 = 0x00000000;
    *(volatile unsigned int *)0x600000c8 = 0x00000000;
    *(volatile unsigned int *)0x600000cc = 0x00000000;
    *(volatile unsigned int *)0x600000d0 = 0x00000000;
    *(volatile unsigned int *)0x600000d4 = 0x00000000;
    *(volatile unsigned int *)0x600000d8 = 0x00000000;
    *(volatile unsigned int *)0x600000dc = 0x00000000;
    *(volatile unsigned int *)0x600000e0 = 0x00000000;
    *(volatile unsigned int *)0x600000e4 = 0x00000000;
    *(volatile unsigned int *)0x600000e8 = 0x00000000;
    *(volatile unsigned int *)0x600000ec = 0x00000000;

    *(volatile unsigned int *)0x60003f00 = 0x1;

    while(1){
        cnt++;
        delay(INTERVAL_EACH_CHECKING);
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
        if (cnt > MAX_TIMES_CHECKING)
        {
            status = 0;
            goto func_return;
        }
    }

    r_output_token_data = *((volatile unsigned int *)0x60000000);
    if (r_output_token_data != c_output_token_id) {
        status = 0;
    }

    /* Copy the output mac data */
    memcpy((void *)data_out, (void *)0x60000008, data_out_len);

    //Write
    *(volatile unsigned int *)0x60003f00 = 0x2;

    //printf("\r\n nl_scrt_aes_cmac, cnt:%d status:%d \r\n", cnt, status);

    return status;

func_return:
    /* Unlink MailBox */
    *(volatile unsigned int *)0x60003f00 = 0x8;

    return status;
}


/*
 * aes_cmac_sample - Sample code for AES CMAC
 *
 */
void aes_cmac_sample(void)
{
    unsigned char sk[16] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10};
    //unsigned char plain_text[16] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10}; //Expect Output: 851555ca 5a3938f9 1d55ecc8 ad8c21b0
    unsigned char plain_text[32] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10}; //Expect Output: 29bc3c81 9b9bfcf4 6f1cf8ca 4597008e

    unsigned char tag[16] = {0};
    int i;

    printf("\r\n do aes cmac \r\n");

    memset(tag, 0, 16);

    //AES CMAC
    nl_scrt_aes_cmac(sk, sizeof(sk), plain_text, sizeof(plain_text), tag, sizeof(tag));

    //Show output MAC data
    for(i=0; i < sizeof(tag) ;i++) {
        printf("output mac[%d]:%02x \r\n", i, tag[i]);
    }
}


/*
 * nl_scrt_aes_ecb - Perform the AES ECB Operation.
 *
 * @param [in] bEncrypt
 *    1:Encrypt
 *    0:Decrypt
 *
 * @param [in] sk
 *    Key
 *
 * @param [in] sk_len
 *    Size of Key
 *
 * @param [in/out] data_in
 *    Input Data
 *
 * @param [out] data_out
 *    Output Data
 *
 * @param [in] data_len
 *    The length of the Data Transfer
 *
 * @return 1 success
 *
 * @return 0 fail
 *
 */
int nl_scrt_aes_ecb_impl(int bEncrypt, unsigned char *sk, int sk_len, unsigned char *data_in, unsigned char *data_out, unsigned data_len)
{
    unsigned int data;
    unsigned int r_output_token_data;
    unsigned int c_output_token_id = 0xd82c;
    int status = 1;
    int i;
    unsigned int word_11;
    int quo, rem;
    int buf_len;
    int key_len = 1;
    int fieldEncrypt;
    unsigned int sk_m;
    unsigned int data_in_m;
    unsigned int data_out_m;
    unsigned char w_key[4] = {0};
    int cnt = 0;

    //printf("\r\n nl_scrt_aes_ecb, sk:%lx data_in:%lx data_out:%lx \r\n", sk, data_in, data_out); //sk:602b28 data_in:602b38 data_out:602b18

#if 0
    //Remapping
    sk_m = (unsigned int)sk | 0x20000000;
    data_in_m = (unsigned int)data_in | 0x20000000;
    data_out_m = (unsigned int)data_out | 0x20000000;
#else
    sk_m = (unsigned int)sk;
    data_in_m = (unsigned int)data_in;
    data_out_m = (unsigned int)data_out;
#endif

    if(sk == 0||
       data_in == 0 ||
       data_out == 0){
        return 0;
    }

    //printf("\r\n nl_scrt_aes_ecb, sk_m:%lx \r\n", sk_m);
    //printf("\r\n nl_scrt_aes_ecb, data_in_m:%lx \r\n", data_in_m);
    //printf("\r\n nl_scrt_aes_ecb, data_out_m:%lx \r\n", data_out_m);

    if(sk_len != 16 &&
       sk_len != 24 &&
       sk_len != 32) {
        return 0;
    }

    if(data_len <= 0) {
        return 0;
    }

    if(bEncrypt == 1) {
        fieldEncrypt = 1;
    } else {
        fieldEncrypt = 0;
    }

    switch (sk_len)
    {
        case 16:
            key_len = 1;
            break;
        case 24:
            key_len = 2;
            break;
        case 32:
            key_len = 3;
            break;
    }

    //It should be a multiple of block size (16 bytes)
    quo = (data_len / 16);
    rem = (data_len % 16);
    if(rem == 0) {
        buf_len = (quo * 16);
    } else {
        buf_len = ((quo + 1) * 16);
    }

    *(volatile unsigned int *)0x60003f00 = 0x4;

    //printf("\r\n nl_scrt_aes_ecb, 444, fieldEncrypt:%d key_len:%d buf_len:%d \r\n", fieldEncrypt, key_len, buf_len);

    while(1){
        delay(3);
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }

    //printf("\r\n nl_scrt_aes_ecb, 555 \r\n");

    *(volatile unsigned int *)0x60000000 = 0x0104d82c;
    *(volatile unsigned int *)0x60000004 = 0x00000000;
    *(volatile unsigned int *)0x60000008 = buf_len;
    *(volatile unsigned int *)0x6000000c = data_in_m;//(unsigned int) data_in;
    *(volatile unsigned int *)0x60000010 = 0x00000000;
    *(volatile unsigned int *)0x60000014 = buf_len;
    *(volatile unsigned int *)0x60000018 = data_out_m;//(unsigned int) data_out;
    *(volatile unsigned int *)0x6000001c = 0x00000000;
    *(volatile unsigned int *)0x60000020 = buf_len + 4;
    *(volatile unsigned int *)0x60000024 = 0x00000000;
    *(volatile unsigned int *)0x60000028 = 0x00000000;

    word_11 = ((key_len << 16) | (fieldEncrypt << 15) | (0x0));
    *(volatile unsigned int *)0x6000002c = word_11;
    //printf("\r\n word_11:0x%lx \r\n", word_11);

    *(volatile unsigned int *)0x60000030 = 0x00000000;
    *(volatile unsigned int *)0x60000034 = 0x00000000;
    *(volatile unsigned int *)0x60000038 = 0x00000000;
    *(volatile unsigned int *)0x6000003c = 0x00000000;
    *(volatile unsigned int *)0x60000040 = 0x00000000;

    memcpy((void *)0x60000044, (void *)sk_m, 16);

    *(volatile unsigned int *)0x60000054 = 0x00000000;
    *(volatile unsigned int *)0x60000058 = 0x00000000;
    *(volatile unsigned int *)0x6000005c = 0x00000000;
    *(volatile unsigned int *)0x60000060 = 0x00000000;

    *(volatile unsigned int *)0x60003f00 = 0x1;

    while(1){
        cnt++;
        if (cnt > MAX_TIMES_CHECKING)
        {
            status = 0;
            goto func_return;
        }
        delay(INTERVAL_EACH_CHECKING);
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }

    //printf("\r\n nl_scrt_aes_ecb, 777 \r\n");

    r_output_token_data = *((volatile unsigned int *)0x60000000);
    if (r_output_token_data != c_output_token_id) {
        status = 0;
        //printf("\r\n nl_scrt_aes_ecb, 888 \r\n");
    }

    r_output_token_data = *((volatile unsigned int *) (data_out + buf_len));
    if ( r_output_token_data != c_output_token_id )
    {
       status = 0;
       //printf("\r\n nl_scrt_aes_ecb, 999 \r\n");
    }

    *(volatile unsigned int *)0x60003f00 = 0x2;
    //printf("\r\n nl_scrt_aes_ecb, ret:%d \r\n", ret);

    return status;

func_return:
    /* Unlink MailBox */
    *(volatile unsigned int *)0x60003f00 = 0x8;

    return status;
}
#endif

/*
 * aes_ecb_sample - Sample code for AES ECB
 *
 */
void aes_ecb_sample(void)
{
    int i;
    unsigned char sk[16] = {0x1f, 0x1e, 0x1d, 0x1c, 0x1b, 0x1a, 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10};
    //unsigned char sk[24] = {0x1f, 0x1e, 0x1d, 0x1c, 0x1b, 0x1a, 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    //unsigned char sk[36] = {0x1f, 0x1e, 0x1d, 0x1c, 0x1b, 0x1a, 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    unsigned char data_in[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x60, 0x50};
    unsigned char data_out[16] = {0};
    unsigned char data_out2[16] = {0};

    printf("\r\n aes_ecb_sample \r\n");

    /* AEC ECB Encryption */
    memset(data_out, 0, 16);
    nl_scrt_aes_ecb(1, sk, sizeof(sk), data_in, data_out, sizeof(data_in));
    for(i=0; i < sizeof(data_in); i++) {
        printf( "output data_out[%d]:%02x \r\n", i, data_out[i]);
    }

    /* AES ECB Decryption */
    memset(data_out2, 0, 16);
    nl_scrt_aes_ecb(0, sk, sizeof(sk), data_out, data_out2, sizeof(data_out));
    for(i=0; i < sizeof(data_out); i++) {
        printf( "output data_out2[%d]:%02x \r\n", i, data_out2[i]);
    }
}

#if 0
/*
 * nl_scrt_trng
 *
 * Generate True Random Number
 *
 * @param [in] size
 *    Size of the random number
 *
 * @param [out] trng_out
 *    Pointer to the output random number.
 *
 * @return 1 success
 *
 * @return 0 fail
 */
int nl_scrt_trng(int size, unsigned char *trng_out)
{
    unsigned int c_output_token_id_1    = 0xc8a7;
    unsigned int c_output_token_id_2    = 0x4da5;
    unsigned int r_output_token_data    = 0;
    unsigned int data                   = 0;
    int status = 1;
    int cnt = 0;

    // Todo
    if(!nl_scrt_otp_status_get())
    {
        SCRT_LOGE("[%s %d] OTP not ready\n", __func__, __LINE__);
        return 0;
    }

    if(trng_out == 0) return 0;

    //It should be a multiple of 4 bytes
    if( (size <= 0) || (size > 256) || ((size % 4) != 0)) return 0;

    *(volatile unsigned int *)0x60003f00 = 0x4;

    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }

    //Write Token - Configure Random Number
    *(volatile unsigned int *)0x60000000 = 0x1402c8a7;
    *(volatile unsigned int *)0x60000004 = 0x4f5a3647;
    *(volatile unsigned int *)0x60000008 = 0x00000003;
    *(volatile unsigned int *)0x6000000c = 0x00030801;

    *(volatile unsigned int *)0x60003f00 = 0x1;

    while(1){
        cnt++;
        delay(10);
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
        if (cnt > MAX_TIMES_CHECKING)
        {
            status = 0;
            goto func_return;
        }
    }

    r_output_token_data = *((volatile unsigned int *)0x60000000);
    if (r_output_token_data != c_output_token_id_1) {
        status = 0;
    }

    *(volatile unsigned int *)0x60003f00 = 0x2;

    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }

    //Write Token - Get Random Number
    *(volatile unsigned int *)0x60000000 = 0x04044da5;
    *(volatile unsigned int *)0x60000004 = 0x00000000;
    *(volatile unsigned int *)0x60000008 = size;
    *(volatile unsigned int *)0x6000000c = (unsigned int) trng_out;
    *(volatile unsigned int *)0x60000010 = 0x00000000;

    *(volatile unsigned int *)0x60003f00 = 0x1;

    cnt = 0;
    while(1){
        cnt++;
        delay(10);
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
        if (cnt > MAX_TIMES_CHECKING)
        {
            status = 0;
            goto func_return;
        }
    }

    r_output_token_data = *((volatile unsigned int *)0x60000000);
    if (r_output_token_data != c_output_token_id_2) {
        status = 0;
    }

    //Write a word
    *(volatile unsigned int *)0x60003f00 = 0x2;

    //printf("\r\n cnt:%d status:%d \r\n", cnt, status);
    return status;
func_return:
    /* Unlink MailBox */
    *(volatile unsigned int *)0x60003f00 = 0x8;
    return status;
}


/*
 * trng_sample - Sample code for TRNG
 *
 */
void trng_sample(void)
{
    int i;
    unsigned char data_out[64] = {0};

    printf("\r\n trng_sample \r\n");
    memset(data_out, 0, sizeof(data_out));
    nl_scrt_trng(sizeof(data_out), data_out);
    for(i = 0; i < sizeof(data_out); i++) {
        printf( "output data_out[%d]:%02x \r\n", i, data_out[i]);
    }
}


void fpga_test_scrt_ecdh_phase1(void)
{
    unsigned int data;
    unsigned int asset_id_0;
    unsigned int asset_id_1;
    unsigned int asset_id_2;
    unsigned int asset_id_3;

    unsigned int data_sram_1[] = {
            0x0a802e59,
            0x5b33fc19,
            0x2bebd6bb,
            0xad0ef7ac,
            0x984057d8,
            0x68782c71,
            0x5b4f8291,
            0x038f40d6,
            0x470b55bd,
            0x495af43d,
            0x9ea2f25b,
            0xa66a63b7,
            0x3d553100,
            0x91e060a3,
            0x6bb3666a,
            0x42720ba9
    };

    unsigned int data_sram_2[] = {
            0xb73650ad,
            0x1987d200,
            0x47dfa61e,
            0x65da19fa,
            0xca734614,
            0xa68522dc,
            0x0447df58,
            0xf28eae0a,
            0xbf0ae8f0,
            0xe7b6a7b3,
            0x35895fa3,
            0x55260296,
            0x9e56b7ca,
            0x4afb290a,
            0x7a4b8c65,
            0x852d0c3d
    };

    unsigned int data_sram_3[] = {
            0x60a0ebe4,
            0x1510a3f9,
            0x64020fbf,
            0x957d7284,
            0xfd5e6cad,
            0x5b6ec642,
            0xe7b3dd10,
            0x001ead73
    };

    unsigned int data_sram_4[] = {
            0x906d1d4e,
            0x196aefec,
            0x7207bbc6,
            0xf6eb07af,
            0x5b1214b8,
            0xf9025907,
            0x35e412d1,
            0x1ff0ac34,
            0x9a065e01,
            0x581c2fdb,
            0x5cf721f5,
            0xf5ef4a79,
            0x1afe2fa2,
            0xf2e52894,
            0x00000000,
            0x00000000
    };

    unsigned int data_sram_5[] = {
            0x7115a36c,
            0x680ef25e,
            0x3452ba6a,
            0xcbf3784b,
            0xcbb08e46,
            0x6ec46672,
            0x24ef743f,
            0x00000000
    };

    unsigned int data_sram_6[] = {
            0x278ba1c4,
            0xc2ad78b4,
            0xfb47c33b,
            0x410ae21e,
            0x1a659486,
            0xe8c14840,
            0x4eca0998,
            0xeb3f8757,
            0x854a6a29,
            0x6988a092,
            0xc9c767a2,
            0x28a05f4a,
            0x239761b2,
            0x6a795aab,
            0xa651b17a,
            0x0dbe4c0f,
            0xe378069f,
            0xdd118602,
            0x01abbdaf,
            0xdfff9e0a,
            0xc03abab3,
            0x54751a49,
            0x6fe56b2b,
            0x2186b572,
            0x63ca71e9,
            0x323863ce,
            0x630c96f0,
            0x0e2f153b,
            0x9c9c2ca3,
            0xe04883b4,
            0x84a1755a,
            0xefeef4f4,
            0x37b47d94,
            0x00290b44,
            0xaec923a7,
            0x74eb65f8,
            0xcbe21a56,
            0x07537452,
            0x86355e0b,
            0xbcac3330,
            0x5841dc4f,
            0x12ef0bb6,
            0xcf0cb975,
            0x141e75fa
    };

    unsigned int data_sram_7[] = {
            0x070000c0,
            0xffffffff,
            0xffffffff,
            0xfffffffe,
            0xffffffff,
            0xffffffff,
            0xffffffff,
            0x070100c0,
            0xfffffffc,
            0xffffffff,
            0xfffffffe,
            0xffffffff,
            0xffffffff,
            0xffffffff,
            0x070200bf,
            0xc146b9b1,
            0xfeb8deec,
            0x72243049,
            0x0fa7e9ab,
            0xe59c80e7,
            0x64210519,
            0x070300c0,
            0xb4d22831,
            0x146bc9b1,
            0x99def836,
            0xffffffff,
            0xffffffff,
            0xffffffff,
            0x070400bd,
            0x82ff1012,
            0xf4ff0afd,
            0x43a18800,
            0x7cbf20eb,
            0xb03090f6,
            0x188da80e,
            0x070500bb,
            0x1e794811,
            0x73f977a1,
            0x6b24cdd5,
            0x631011ed,
            0xffc8da78,
            0x07192b95,
            0x07060001,
            0x00000001
    };

    //Token: TRNG configure
    unsigned int data_mb_1[] = {
            0x140019fb,
            0x4f5a3647,
            0x00000001,
            0x00030101
    };

    //Token: Create asset
    unsigned int data_mb_2[] = {
            0x1700428a,
            0x00000000,
            0x30000000,
            0x00000010,
            0x00000040,
            0x00000000,
            0x00000000
    };

    //Token: Create asset
    unsigned int data_mb_3[] = {
            0x1700b9bd,
            0x00000000,
            0x30000000,
            0x00000010,
            0x00000040,
            0x00000000,
            0x00000000
    };

    //Token: Load asset
    unsigned int data_mb_4[] = {
            0x27046d31,
            0x00000000,
            0x00015004,
            0x02000000,
            0x00000000,
            0x00000000,
            0x00000000,
            0x00000000,
            0x00000000,
            0x00000000
    };

    //Token: Create asset
    unsigned int data_mb_5[] = {
            0x1700c786,
            0x00000000,
            0x80000004,
            0x00001000,
            0x00000038,
            0x00000000,
            0x00000000
    };

    //Token: Create asset
    unsigned int data_mb_6[] = {
            0x17008cff,
            0x00000000,
            0x00000004,
            0x00001000,
            0x0000001c,
            0x00000000,
            0x00000000
    };

    //Token: Create asset
    unsigned int data_mb_7[] = {
            0x17009e65,
            0x00000000,
            0x00000000,
            0x00008000,
            0x000000b0,
            0x00000000,
            0x00000000
    };

    //Token: Load asset
    unsigned int data_mb_8[] = {
            0x27040a12,
            0x00000000,
            0x00015014,
            0x080000b0,
            0x0040a600,
            0x00000000,
            0x00000000,
            0x00000000,
            0x00000000,
            0x00000000
    };

    //Token: Public key token
    unsigned int data_mb_9[] = {
            0x1904d4f1,
            0x00000000,
            0x06060015,
            0x00000030,
            0x00015010,
            0x00015014,
            0x0001500c,
            0x0000002c,
            0x0040b000,
            0x00000000,
            0x00000000,
            0x00000000,
            0x00015004,
            0x7473754a,
            0x6d6f7320,
            0x73612065,
            0x69636f73,
            0x64657461,
            0x74616420,
            0x6f662061,
            0x656b2072,
            0x6f6c6279,
            0x65672062,
            0x6172656e,
            0x6e6f6974
    };


    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x4);
    //wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x40);
    //wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x400);
    //wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x4000);


    //===========================================================================  Block1
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }
    /*
     unsigned int data_mb_1[] = {
        0x140019fb,
        0x4f5a3647,
        0x00000001,
        0x00030101
    };
    */
    wrCoreBlock(EIP130_MAILBOX_IN_BASE, data_mb_1, sizeof(data_mb_1)/sizeof(data_mb_1[0]));
    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x1);
    write_sram(0x0040a000, data_sram_1, sizeof(data_sram_1)/sizeof(data_sram_1[0]));
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }
    /*
    unsigned int data_mb_2[] = {
            0x1700428a,
            0x00000000,
            0x30000000,
            0x00000010,
            0x00000040,
            0x00000000,
            0x00000000
    };
    */
    //Create Asset
    wrCoreBlock(EIP130_MAILBOX_IN_BASE, data_mb_2, sizeof(data_mb_2)/sizeof(data_mb_2[0]));
    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x1);
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }
    data = rdCore(EIP130_MAILBOX_OUT_BASE);  //it shoud be: 0x000019fb => test ok
    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x2);
    printf("\r\n ECDH Phase 1 Block 1- data:%lx \r\n", data);
    //===========================================================================



    //===========================================================================  Block2
    write_sram(0x0040a100, data_sram_2, sizeof(data_sram_2)/sizeof(data_sram_2[0]));
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }

    /*
    unsigned int data_mb_3[] = {
            0x1700b9bd,
            0x00000000,
            0x30000000,
            0x00000010,
            0x00000040,
            0x00000000,
            0x00000000
    };
    */
    //Create Asset
    wrCoreBlock(EIP130_MAILBOX_IN_BASE, data_mb_3, sizeof(data_mb_3)/sizeof(data_mb_3[0]));
    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x1);
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }
    data = rdCore(EIP130_MAILBOX_OUT_BASE);    //it shoud be: 0x0000428a   => test ok
    asset_id_0 = rdCore(EIP130_MAILBOX_OUT_BASE + 4);//it shoud be: 0x00015004   => test ok
    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x2);
    printf("\r\n ECDH Phase 1 Block 2- asset_id_0:%lx \r\n", asset_id_0);
    //===========================================================================





    //===========================================================================  Block3
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }
    data = rdCore(EIP130_MAILBOX_OUT_BASE);     //it shoud be: 0x0000b9bd  => test ok
    data = rdCore(EIP130_MAILBOX_OUT_BASE + 4); //it shoud be: 0x00015008  => test ok
    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x2);
    printf("\r\n ECDH Phase 1 Block 3- data:%lx \r\n", data);
    //===========================================================================






    //===========================================================================  Block4
    write_sram(0x0040a200, data_sram_3, sizeof(data_sram_3)/sizeof(data_sram_3[0]));
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }


/*
        unsigned int data_mb_4[] = {
            0x27046d31,
            0x00000000,
            0x00015004,
            0x02000000,
            0x00000000,
            0x00000000,
            0x00000000,
            0x00000000,
            0x00000000,
            0x00000000
    };
*/
    //Load Asset
    wrCoreBlock(EIP130_MAILBOX_IN_BASE, data_mb_4, sizeof(data_mb_4)/sizeof(data_mb_4[0]));
    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x1);
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }
    data = rdCore(EIP130_MAILBOX_OUT_BASE);    //it shoud be: 0x00006D31  => test ok
    data = rdCore(EIP130_MAILBOX_OUT_BASE + 4);//it shoud be: 0x0  => test ok
    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x2);
    printf("\r\n ECDH Phase 1 Block 4- data:%lx \r\n", data);
    //===========================================================================






    //===========================================================================  Block5
    write_sram(0x0040a300, data_sram_4, sizeof(data_sram_4)/sizeof(data_sram_4[0]));
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }


/*
    unsigned int data_mb_5[] = {
            0x1700c786,
            0x00000000,
            0x80000004,
            0x00001000,
            0x00000038,
            0x00000000,
            0x00000000
    };
*/
    //Create Asset
    wrCoreBlock(EIP130_MAILBOX_IN_BASE, data_mb_5, sizeof(data_mb_5)/sizeof(data_mb_5[0]));
    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x1);
    write_sram(0x0040a400, data_sram_5, sizeof(data_sram_5)/sizeof(data_sram_5[0]));
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }


/*
    unsigned int data_mb_6[] = {
            0x17008cff,
            0x00000000,
            0x00000004,
            0x00001000,
            0x0000001c,
            0x00000000,
            0x00000000
    };
*/
    //Create Asset
    wrCoreBlock(EIP130_MAILBOX_IN_BASE, data_mb_6, sizeof(data_mb_6)/sizeof(data_mb_6[0]));
    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x1);
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }
    data = rdCore(EIP130_MAILBOX_OUT_BASE);    //it shoud be: 0x0000c786  OK  => test ok
    asset_id_1 = rdCore(EIP130_MAILBOX_OUT_BASE + 4);//it shoud be: 0x0001500c  => test ok
    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x2);
    printf("\r\n ECDH Phase 1 Block 5- asset_id_1:%lx \r\n", asset_id_1);
    //===========================================================================






    //===========================================================================  Block6
    write_sram(0x0040a500, data_sram_6, sizeof(data_sram_6)/sizeof(data_sram_6[0]));
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }

/*
    unsigned int data_mb_7[] = {
            0x17009e65,
            0x00000000,
            0x00000000,
            0x00008000,
            0x000000b0,
            0x00000000,
            0x00000000
    };
*/
    //Create Asset
    wrCoreBlock(EIP130_MAILBOX_IN_BASE, data_mb_7, sizeof(data_mb_7)/sizeof(data_mb_7[0]));
    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x1);
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }
    data = rdCore(EIP130_MAILBOX_OUT_BASE);    //it shoud be: 0x00008cff  OK  => test ok
    asset_id_2 = rdCore(EIP130_MAILBOX_OUT_BASE + 4);//it shoud be: 0x00015010  => test ok
    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x2);
    printf("\r\n ECDH Phase 1 Block 6- asset_id_2:%lx \r\n", asset_id_2);
    //===========================================================================





    //===========================================================================  Block7
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }
    data = rdCore(EIP130_MAILBOX_OUT_BASE);    //it shoud be: 0x00009e65  OK  => test ok
    asset_id_3 = rdCore(EIP130_MAILBOX_OUT_BASE + 4);//it shoud be: 0x00015014  => test ok
    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x2);
    printf("\r\n ECDH Phase 1 Block 7- asset_id_3:%lx \r\n", asset_id_3);
    //===========================================================================






    //===========================================================================  Block8
    write_sram(0x0040a600, data_sram_7, sizeof(data_sram_7)/sizeof(data_sram_7[0]));
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }

/*
    unsigned int data_mb_8[] = {
            0x27040a12,
            0x00000000,
            0x00015014,
            0x080000b0,
            0x0040a600,
            0x00000000,
            0x00000000,
            0x00000000,
            0x00000000,
            0x00000000
    };
*/
    //Load Asset
    wrCoreBlock(EIP130_MAILBOX_IN_BASE, data_mb_8, sizeof(data_mb_8)/sizeof(data_mb_8[0]));
    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x1);
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }
    data = rdCore(EIP130_MAILBOX_OUT_BASE);    //it shoud be: 0x0x00000a12  => test ok
    data = rdCore(EIP130_MAILBOX_OUT_BASE + 4);//it shoud be: 0x0  => test ok
    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x2);
    printf("\r\n ECDH Phase 1 Block 8- data:%lx \r\n", data);
    //===========================================================================






    //===========================================================================  Block9
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }

/*
    data_mb_9[25] = {
            0x1904d4f1,
            0x00000000,
            0x06060015,
            0x00000030,
            0x00015010, //asset_id_2
            0x00015014, //asset_id_3
            0x0001500c, //asset_id_1
            0x0000002c,
            0x0040b000,
            0x00000000,
            0x00000000,
            0x00000000,
            0x00015004,
            0x7473754a,
            0x6d6f7320,
            0x73612065,
            0x69636f73,
            0x64657461,
            0x74616420,
            0x6f662061,
            0x656b2072,
            0x6f6c6279,
            0x65672062,
            0x6172656e,
            0x6e6f6974
    };*/

    data_mb_9[0] = 0x1904d4f1;
    data_mb_9[1] = 0x00000000;
    data_mb_9[2] = 0x06060015;
    data_mb_9[3] = 0x00000030;
    data_mb_9[4] = asset_id_2; //modified
    data_mb_9[5] = asset_id_3; //modified
    data_mb_9[6] = asset_id_1; //modified
    data_mb_9[7] = 0x0000002c;
    data_mb_9[8] = 0x0040b000;
    data_mb_9[9] = 0x00000000;
    data_mb_9[10] = 0x00000000;
    data_mb_9[11] = 0x00000000;
    data_mb_9[12] = asset_id_0; //modified
    data_mb_9[13] = 0x7473754a;
    data_mb_9[14] = 0x6d6f7320;
    data_mb_9[15] = 0x73612065;
    data_mb_9[16] = 0x69636f73;
    data_mb_9[17] = 0x64657461;
    data_mb_9[18] = 0x74616420;
    data_mb_9[19] = 0x6f662061;
    data_mb_9[20] = 0x656b2072;
    data_mb_9[21] = 0x6f6c6279;
    data_mb_9[22] = 0x65672062;
    data_mb_9[23] = 0x6172656e;
    data_mb_9[24] = 0x6e6f6974;

    //Create PKCP Key Pair
    wrCoreBlock(EIP130_MAILBOX_IN_BASE, data_mb_9, sizeof(data_mb_9)/sizeof(data_mb_9[0]));
    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x1);
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }
    //Polling
    while(1){
        data = rdCore(EIP130_REGISTEROFFSET_MAILBOX_STAT);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }
    data = rdCore(EIP130_MAILBOX_OUT_BASE);  //it shoud be: 0x0x0000d4f1  => test ok
    wrCore(EIP130_REGISTEROFFSET_MAILBOX_CTRL, 0x2);
    printf("\r\n ECDH Phase 1 Block 9- data:%lx \r\n", data);
    //===========================================================================




}

unsigned char plain_text_[2000] ={0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                                0x01, 0x02, 0x03, 0x04,

                                };
unsigned char cipher_text_[2000] = {0};

void aes_ccm_sample_2k(void) //It's working
{
    unsigned char sk[32] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                            0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10
                            };
    unsigned char nonce[13] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d};
    unsigned char a_data[16] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10};

    unsigned char tag[8] = {0};
    int i;

    //printf("\r\n do aes ccm encryption \r\n");

    memset(cipher_text_, 0, 16);
    memset(tag, 0, 8);

    //AES CCM Encryption
    nl_scrt_aes_ccm(1, sk, sizeof(sk), nonce, sizeof(nonce), a_data, sizeof(a_data), plain_text_, cipher_text_, sizeof(plain_text_), tag, sizeof(tag));

    //Show output tag data
    for(i=0; i < sizeof(tag) ;i++) {
        printf("output tag[%d]:%02x \r\n", i, tag[i]);
    }

    //Show output cipher data (size of cipher data is the same as size of plain text)
    for(i=0; i < sizeof(cipher_text_); i++) {
        printf("output cipher[%d]:%02x \r\n", i, *((volatile char *)(cipher_text_ + i)));
    }
}

int aes_ccm_special (int bEncrypt, unsigned char *sk, int sk_len, unsigned char *nonce, int nonce_len, unsigned char *adata, int adata_len, unsigned char *plain_text, unsigned char *encrypted_text, int text_len, unsigned char *tag, int tag_len)
{
    unsigned int token_id_output;
    unsigned int token_id_current = 0xd82c;
    unsigned int data = 0;
    unsigned int word_11;
    int status = 1;
    int i;
    int quo, rem;
    int buf_len;
    unsigned int adata_m;
    unsigned int plain_m;
    unsigned int cipher_m;
    int cnt = 0;

    quo = (text_len / 16);
    rem = (text_len % 16);
    if(rem == 0) {
        buf_len = (quo * 16);
    } else {
        buf_len = ((quo + 1) * 16);
    }

    adata_m = (unsigned int)adata;
    plain_m = (unsigned int)plain_text;
    cipher_m = (unsigned int)encrypted_text;

    /*
        SCRT Control- Link Mailbox
      */
    *(volatile unsigned int *)0x60003f00 = 0x4;

    /*
        SCRT Control- Check Mailbox Status
      */
    while(1){
        delay(1);
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }

    /*Burst write 37 words*/
    *(volatile unsigned int *)0x60000000 = 0x0104d82c;
    *(volatile unsigned int *)0x60000004 = 0x00000000;
    *(volatile unsigned int *)0x60000008 = text_len;                            //The length of the data transfer

    if (bEncrypt == 1) {
        *(volatile unsigned int *)0x6000000c = plain_m;                         //Input Data
    } else {
        *(volatile unsigned int *)0x6000000c = cipher_m;                        //Input Data
    }

    *(volatile unsigned int *)0x60000010 = 0x00000000;                          //Input Data
    *(volatile unsigned int *)0x60000014 = buf_len;                             //Input Data Length

    if (bEncrypt == 1) {
        *(volatile unsigned int *)0x60000018 = cipher_m;                       //Output Data
    } else {
        *(volatile unsigned int *)0x60000018 = plain_m;                        //Output Data
    }

    *(volatile unsigned int *)0x6000001c = 0x00000000;                          //Output Data
    *(volatile unsigned int *)0x60000020 = (buf_len + 4);                       //Output Data Length
    *(volatile unsigned int *)0x60000024 = adata_m;                            //Associated Data
    *(volatile unsigned int *)0x60000028 = 0x00000000;                          //Associated Data
    //*(volatile unsigned int *)0x6000002c = 0x10d18050;                                                //0x10d18050;           //Nonce length               => word 11[23:20]  => 13  => 0xd
                                                                                //Key length             => word 11[19:16]           => 1 (1 means 128 bits key)
                                                                                //Tag length             => word 11[28:24] 5 bits  => 0x10  => 16
                                                                                //Encrypt or Decrypt  => word11[15]                => 1:encrypt 0:decrypt

    if (bEncrypt == 1) {
        word_11 = ((tag_len & 0xff) << 24) | ((((nonce_len & 0xf) << 4) | (0x1)) << 16) | ((0x80) << 8) | ((0x50) << 0);
    } else {
        word_11 = ((tag_len & 0xff) << 24) | ((((nonce_len & 0xf) << 4) | (0x1)) << 16) | ((0x0) << 8) | ((0x50) << 0);
    }

    *(volatile unsigned int *)0x6000002c = word_11;
    *(volatile unsigned int *)0x60000030 = 0x00000000;
    *(volatile unsigned int *)0x60000034 = 0x00000000;
    *(volatile unsigned int *)0x60000038 = 0x00000000;
    *(volatile unsigned int *)0x6000003c = 0x00000000;
    *(volatile unsigned int *)0x60000040 = 0x00000000;

    memcpy((void *)0x60000044, sk, sk_len);

    *(volatile unsigned int *)0x60000054 = 0x00000000;          //key
    *(volatile unsigned int *)0x60000058 = 0x00000000;          //key
    *(volatile unsigned int *)0x6000005c = 0x00000000;          //key
    *(volatile unsigned int *)0x60000060 = 0x00000000;          //key
    *(volatile unsigned int *)0x60000064 = adata_len;           //Associated Data Length
    *(volatile unsigned int *)0x60000068 = 0x00000000;
    *(volatile unsigned int *)0x6000006c = 0x00000000;
    *(volatile unsigned int *)0x60000070 = 0x00000000;

    memcpy((void *)0x60000074, nonce, nonce_len);

    if (bEncrypt == 1) {
        *(volatile unsigned int *)0x60000084 = 0x00000000;
        *(volatile unsigned int *)0x60000088 = 0x00000000;
        *(volatile unsigned int *)0x6000008c = 0x00000000;
        *(volatile unsigned int *)0x60000090 = 0x00000000;
    } else {
        memcpy((void *)0x60000084, tag, tag_len);
    }

    /*
            SCRT Control- Write Token
        */
    *(volatile unsigned int *)0x60003f00 = 0x1;

    return status;
}

void aes_ccm_sample_2k_special(void)
{
    unsigned char sk[32] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
                            0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10
                            };
    unsigned char nonce[13] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d};
    unsigned char a_data[16] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10};

    unsigned char tag[8] = {0};
    int i;

    //printf("\r\n do aes ccm encryption \r\n");

    memset(cipher_text_, 0, 16);
    memset(tag, 0, 8);

    //AES CCM Encryption
    aes_ccm_special(1, sk, sizeof(sk), nonce, sizeof(nonce), a_data, sizeof(a_data), plain_text_, cipher_text_, sizeof(plain_text_), tag, sizeof(tag));
}
#endif

#if 0
#include "valtest_internal.h"
#include "testvectors_aes_basic.h"
#include "testvectors_des.h"
#include "testvectors_xts_aes.h"
#include "testvectors_aes_f8.h"
#include "sfzutf.h"
#include "valtest_sym_crypto.h"
#include "valtest_random.h"
#include "valtest_hash.h"
#include "valtest_cipher_mac.h"
#include "api_val_buffers.h"
#include "api_val_random.h"
#endif

#if 0
/*----------------------------------------------------------------------------
 * do_SymCrypt
 *
 * Perform a test with the 'val_SymCipher' API using the data and
 * options from the given "SymCryptTestCtx_t" test context.
 */
int
do_SymCrypt(
        SymCryptCtx_t * const TestCntxt_p,
        bool fDecrypt)
{
    static uint8_t result_text[SYM_CRYPT_MAX_BUFLEN];
    static uint8_t input_text[SYM_CRYPT_MAX_BUFLEN];

    ValSize_t result_len = sizeof(result_text);
    ValStatus_t Status;
    uint8_t * TxtIn_p;
    uint8_t * TxtOut_p;
    uint8_t * Result_p = result_text;
    uint32_t padding = 0;

    if (fDecrypt)
    {//Decrypt
        TxtIn_p = TestCntxt_p->ctx_p;
        TxtOut_p = TestCntxt_p->ptx_p;
    }
    else
    {//Encrypt
        Status = val_SymCipherInitEncrypt(TestCntxt_p->SymContext_p);
        //fail_if(Status != VAL_SUCCESS, "val_SymCipherInitEncrypt()=", Status);

        TxtIn_p = TestCntxt_p->ptx_p;
        TxtOut_p = TestCntxt_p->ctx_p;
    }

    if (TestCntxt_p->options & SYM_CRYPT_IN_PLACE)
    {
        memset(Result_p, 0xDC, result_len);
        memcpy(Result_p, TxtIn_p, TestCntxt_p->txt_len);
        TxtIn_p = Result_p;
    }

    if (TestCntxt_p->options & SYM_CRYPT_UNALIGNED)
    {
        int offset = 1 + (TestCntxt_p->misalign % 3);

        memset(input_text, 0xDC, sizeof(input_text));
        memcpy(input_text + offset, TxtIn_p, TestCntxt_p->txt_len);
        TxtIn_p = input_text + offset;
        Result_p += 1 + ((offset + 1) % 3);
        result_len -= 1 + ((offset + 1) % 3);
        TestCntxt_p->misalign += 1;
    }

    if (TestCntxt_p->txt_len & (TestCntxt_p->BlockSize - 1))
    {
        //fail_if(TestCntxt_p->fPadding == false, "Bad length for ECB/CBC ", TestCntxt_p->txt_len);
        msg_print(LOG_HIGH_LEVEL, "[security] do_SymCrypt, Bad length for ECB/CBC \r\n");

        padding = (0 - TestCntxt_p->txt_len) & (TestCntxt_p->BlockSize - 1);
        if (!(TestCntxt_p->options & SYM_CRYPT_UNALIGNED))
        {
            memset(input_text, TestCntxt_p->PaddingByte, (TestCntxt_p->txt_len + padding));
            memcpy(input_text, TxtIn_p, TestCntxt_p->txt_len);
            TxtIn_p = input_text;
        }
    }

    if (TestCntxt_p->options & SYM_CRYPT_MULTIPART)
    {
        ValSize_t src_len = TestCntxt_p->txt_len + padding;
        ValSize_t dst_space = result_len;
        uint8_t * srcpart_p = TxtIn_p;
        uint8_t * dstpart_p = Result_p;

        do
        {
            ValSize_t part_len = MIN(TestCntxt_p->BlockSize, src_len);
            ValSize_t dst_len = dst_space;

            if (src_len == part_len)
            {
                uint8_t * InCopy_p;
                uint8_t * OutCopy_p;

                InCopy_p = (uint8_t *)SFZUTF_MALLOC(part_len);
                //fail_if(InCopy_p == NULL, "Allocation ", (int)part_len);
                if (InCopy_p == NULL) {
                    msg_print(LOG_HIGH_LEVEL, "[security] do_SymCrypt, InCopy_p == NULL \r\n");
                }

                OutCopy_p = (uint8_t *)SFZUTF_MALLOC(dst_len);
                //fail_if(OutCopy_p == NULL, "Allocation ", (int)dst_len);
                if (OutCopy_p == NULL) {
                    msg_print(LOG_HIGH_LEVEL, "[security] do_SymCrypt, OutCopy_p == NULL \r\n");
                }

                memcpy(InCopy_p, srcpart_p, part_len);
                Status = val_SymCipherFinal(TestCntxt_p->SymContext_p,
                                            InCopy_p, part_len,
                                            OutCopy_p, &dst_len);
                if (Status == VAL_SUCCESS)
                {
                    memcpy(dstpart_p, OutCopy_p, dst_len);
                }

                SFZUTF_FREE(OutCopy_p);
                SFZUTF_FREE(InCopy_p);

                //fail_if(Status != VAL_SUCCESS, "val_SymCipherFinal()=", Status);
            }
            else
            {
                uint8_t * InCopy_p;
                uint8_t * OutCopy_p;

                InCopy_p = (uint8_t *)SFZUTF_MALLOC(part_len);
                //fail_if(InCopy_p == NULL, "Allocation ", (int)part_len);
                OutCopy_p = (uint8_t *)SFZUTF_MALLOC(dst_len);
                //fail_if(OutCopy_p == NULL, "Allocation ", (int)dst_len);

                memcpy(InCopy_p, srcpart_p, part_len);
                Status = val_SymCipherUpdate(TestCntxt_p->SymContext_p,
                                             InCopy_p, part_len,
                                             OutCopy_p, &dst_len);
                if (Status == VAL_SUCCESS)
                {
                    memcpy(dstpart_p, OutCopy_p, dst_len);
                }

                SFZUTF_FREE(OutCopy_p);
                SFZUTF_FREE(InCopy_p);

                //fail_if(Status != VAL_SUCCESS, "val_SymCipherUpdate()=", Status);
            }

            //fail_if(dst_len != TestCntxt_p->BlockSize,"Unexpected partial result length ",(int)dst_len);

            src_len -= part_len;
            dst_space -= dst_len;
            srcpart_p += part_len;
            dstpart_p += part_len;
        } while (src_len > 0);

        /* Let result_len = accumulated result length. */
        result_len -= dst_space;
    }
    else
    {
		{
	        uint8_t * InCopy_p;
	        uint8_t * OutCopy_p;

	        InCopy_p = (uint8_t *)SFZUTF_MALLOC(TestCntxt_p->txt_len + padding);
	        OutCopy_p = (uint8_t *)SFZUTF_MALLOC(result_len);

	        memcpy(InCopy_p, TxtIn_p, (TestCntxt_p->txt_len + padding));
	        Status = val_SymCipherFinal(TestCntxt_p->SymContext_p,
	                                    InCopy_p, (TestCntxt_p->txt_len + padding),
	                                    OutCopy_p, &result_len);
	        if (Status == VAL_SUCCESS)
	        {
	            memcpy(Result_p, OutCopy_p, result_len);
	        }

	        SFZUTF_FREE(OutCopy_p);
	        SFZUTF_FREE(InCopy_p);

		}
    }

    //fail_if(result_len != (ValSize_t)(TestCntxt_p->txt_len + padding),"Unexpected result length: ", (int)result_len);
    //fail_if(memcmp(Result_p, TxtOut_p, TestCntxt_p->txt_len) != 0,"Unexpected output result ", -1);


	/*
    if (memcmp(Result_p, TxtOut_p, TestCntxt_p->txt_len) != 0) {

	} else {

	}*/

    return END_TEST_SUCCES;
}
#endif

#if 0
/*
 * nl_se_SetupContextAndEnvironment
 *
 * @param [in] PubKey_p
 *     Pointer to the asymmetric crypto key structure of the public key for ECDH.
 *
 * @param [in] PrivKey_p
 *     Pointer to the asymmetric crypto key structure of the private key for ECDH.
 *
 */
int
nl_se_SetupContextAndEnvironment(
        SymCryptCtx_t * TestCntxt_p,
        ValSymAlgo_t Algorithm,
        TestVectors_Mode_t TestMode,
        const uint8_t * const Key_p,
        uint32_t KeySize,
        const uint8_t * const Iv_p,
        uint8_t * Ptx_p,
        uint8_t * Ctx_p,
        const uint32_t TxtLen)
{
    ValSymMode_t Mode = VAL_SYM_MODE_NONE;
    ValStatus_t Status;
    uint32_t IvSize = 0;

    memset(TestCntxt_p, 0, sizeof(SymCryptCtx_t));

    switch (Algorithm)
    {
    case VAL_SYM_ALGO_CIPHER_AES:
        switch (TestMode)
        {
	        case TESTVECTORS_MODE_ECB:
	            Mode = VAL_SYM_MODE_CIPHER_ECB;
	            break;
	        case TESTVECTORS_MODE_CBC:
	            Mode = VAL_SYM_MODE_CIPHER_CBC;
	            IvSize = VAL_SYM_ALGO_AES_IV_SIZE;
	            break;
	        default:
	            break;
        }
        TestCntxt_p->BlockSize = VAL_SYM_ALGO_AES_BLOCK_SIZE;
        break;
#if 0
    case VAL_SYM_ALGO_CIPHER_DES:
        // Note: No key Asset is allowed
        switch (TestMode)
        {
        case TESTVECTORS_MODE_ECB:
            Mode = VAL_SYM_MODE_CIPHER_ECB;
            break;
        case TESTVECTORS_MODE_CBC:
            Mode = VAL_SYM_MODE_CIPHER_CBC;
            IvSize = VAL_SYM_ALGO_DES_IV_SIZE;
            break;
        default:
            break;
        }
        TestCntxt_p->BlockSize = VAL_SYM_ALGO_DES_BLOCK_SIZE;
        break;
    case VAL_SYM_ALGO_CIPHER_TRIPLE_DES:
        switch (TestMode)
        {
        case TESTVECTORS_MODE_ECB:
            Mode = VAL_SYM_MODE_CIPHER_ECB;
            break;
        case TESTVECTORS_MODE_CBC:
            Mode = VAL_SYM_MODE_CIPHER_CBC;
            IvSize = VAL_SYM_ALGO_DES_IV_SIZE;
            break;
        default:
            break;
        }
        TestCntxt_p->BlockSize = VAL_SYM_ALGO_DES_BLOCK_SIZE;
        break;
#endif

    default:
        break;
    }

    Status = val_SymAlloc(Algorithm, Mode, &TestCntxt_p->SymContext_p);
	if(Status == VAL_INVALID_ALGORITHM) {

	}

    Status = val_SymInitKey(TestCntxt_p->SymContext_p,
                            TestCntxt_p->KeyAssetId,
                            Key_p,
                            KeySize);

    if (IvSize)
    {
        Status = val_SymInitIV(TestCntxt_p->SymContext_p,
                               Iv_p, IvSize);
    }

    //TestCntxt_p->Index = Index;
    //TestCntxt_p->ptx_p = sfzutf_discard_const(Ptx_p);
    //TestCntxt_p->ctx_p = sfzutf_discard_const(Ctx_p);
    TestCntxt_p->ptx_p = Ptx_p;
    TestCntxt_p->ctx_p = Ctx_p;
    TestCntxt_p->txt_len = TxtLen;

    return END_TEST_SUCCES;
}
#endif

#if 0
/*
 * nl_scrt_aes_cbc
 *
 * AES CBC Encryption/Decryption.
 *
 * @param [in] user
 *    User Type.
 *
 * @param [in] fDecrypt
 *    Encrypt or Decrypt.
 *
 * @param [in] key
 *    Encryption key.
 *
 * @param [in] keySize.
 *    Key size.
 *
 * @param [in] iv
 *    Encryption IV for CBC mode.
 *
 * @param [in] Ptx_p
 *   Point to the Plain Text Data.
 *
 * @param [in] TxtLen
 *    The length of the Plain Text Data.
 *
 * @param [in] Ctx_p
 *    Point to the Output Encrypted data buffer.
 *
 */
int
nl_scrt_aes_cbc(Nl_Scrt_Usr_t user, bool fDecrypt, const uint8_t *key, uint32_t keySize, const uint8_t *iv, uint8_t *Ptx_p, size_t TxtLen, uint8_t *Ctx_p)
{
    SymCryptCtx_t CryptoCntxt;
    TestVector_AES_BASIC_t tv_p;

    //Link Mailbox here

    if (nl_se_SetupContextAndEnvironment(&CryptoCntxt,
                                       	VAL_SYM_ALGO_CIPHER_AES,
                                       	TESTVECTORS_MODE_CBC,
                                       	key,
                                       	(keySize/8),
                                       	iv,
                                       	Ptx_p,
                                       	Ctx_p,
                                       	(uint32_t)TxtLen) == END_TEST_SUCCES)
    {

        CryptoCntxt.options |= SYM_CRYPT_IN_PLACE;

        if (do_SymCrypt(&CryptoCntxt, fDecrypt) == END_TEST_SUCCES)
        {
          	return END_TEST_SUCCES;
        }
    }

	return END_TEST_FAIL;
}
#endif

#if 0
/*
 * nl_scrt_aes_ecb
 *
 * AES ECB Encryption/Decryption.
 *
 * @param [in] user
 *    User Type.
 *
 * @param [in] fDecrypt
 *    Encrypt or Decrypt.
 *
 * @param [in] key
 *    Encryption key.
 *
 * @param [in] keySize
 *    Key size.
 *
 * @param [in] iv
 *    Encryption IV for ECB mode
 *
 * @param [in] Ptx_p
 *    Pointer to the Plain Text Data.
 *
 * @param [in] TxtLen
 *    The length of the Plain Text Data.
 *
 * @param [in] Ctx_p
 *    Pointer to the Output Encrypted data buffer.
 *
 */
int
nl_scrt_aes_ecb(Nl_Scrt_Usr_t user, bool fDecrypt, const uint8_t *key, uint32_t keySize, const uint8_t *iv, uint8_t *Ptx_p, size_t TxtLen, uint8_t *Ctx_p)
{
    SymCryptCtx_t CryptoCntxt;
    TestVector_AES_BASIC_t tv_p;

    //Link Mailbox here

    if (nl_se_SetupContextAndEnvironment(&CryptoCntxt,
                                       	VAL_SYM_ALGO_CIPHER_AES,
                                       	TESTVECTORS_MODE_ECB,
                                       	key,
                                       	(keySize/8),
                                       	NULL,
                                       	Ptx_p,
                                       	Ctx_p,
                                       	(uint32_t)TxtLen) == END_TEST_SUCCES)
    {

        CryptoCntxt.options |= SYM_CRYPT_IN_PLACE;

        if (do_SymCrypt(&CryptoCntxt, fDecrypt) == END_TEST_SUCCES)
        {
          	return END_TEST_SUCCES;
        }
    }

	return END_TEST_FAIL;
}
#endif

#if 0
/*
 * nl_scrt_aes_ctr
 *
 * AES CBC Encryption/Decryption.
 *
 * @param [in] user
 *    User Type.
 *
 * @param [in] fDecrypt
 *    Encrypt or Decrypt.
 *
 * @param [in] key
 *    Encryption key.
 *
 * @param [in] keySize.
 *    Key size.
 *
 * @param [in] iv
 *    Encryption IV for CBC mode.
 *
 * @param [in] Ptx_p
 *   Point to the Plain Text Data.
 *
 * @param [in] TxtLen
 *    The length of the Plain Text Data.
 *
 * @param [in] Ctx_p
 *    Point to the Output Encrypted data buffer.
 *
 */
int
nl_scrt_aes_ctr(Nl_Scrt_Usr_t user, bool fDecrypt, const uint8_t *key, uint32_t keySize, const uint8_t *iv, uint8_t *Ptx_p, size_t TxtLen, uint8_t *Ctx_p)
{
    SymCryptCtx_t CryptoCntxt;
    TestVector_AES_BASIC_t tv_p;

    //Link Mailbox here

    if (nl_se_SetupContextAndEnvironment(&CryptoCntxt,
                                       	VAL_SYM_ALGO_CIPHER_AES,
                                       	TESTVECTORS_MODE_CTR,
                                       	key,
                                       	(keySize/8),
                                       	iv,
                                       	Ptx_p,
                                       	Ctx_p,
                                       	(uint32_t)TxtLen) == END_TEST_SUCCES)
    {

        CryptoCntxt.options |= SYM_CRYPT_IN_PLACE;

        if (do_SymCrypt(&CryptoCntxt, fDecrypt) == END_TEST_SUCCES)
        {
          	return END_TEST_SUCCES;
        }
    }

	return END_TEST_FAIL;
}
#endif

#if 0
/*
 * nl_scrt_aes_cmac
 *
 * AES-CMAC Message Authentication Code operation.
 *
 * @param [in] user
 *    User Type.
 *
 * @param [in] Key_p
 *    Pointer to the key structure of the key.
 *
 * @param [in] KeySize
 *    Size of the key.
 *
 * @param [in] Msg_p
 *    Pointer to the message data.
 *
 * @param [in] MsgLen
 *    Size of the message data.
 *
 * @param [out] Mac_p
 *    Pointer to the output MAC data.
 *
 * @param [in] MacLen
 *    Size of the MAC data.
 *
 */
int
nl_scrt_aes_cmac(Nl_Scrt_Usr_t user, uint8_t * Key_p, uint32_t KeySize, uint8_t * Msg_p, uint32_t MsgLen, uint8_t * Mac_p, uint32_t MacLen)
{
    //Link Mailbox here

	return do_CMAC_Gen(Key_p, KeySize, Msg_p, MsgLen, Mac_p, MacLen);
}
#endif

#if 0
/*
 * nl_scrt_aes_cbcmac
 *
 * AES-CBC-MAC Message Authentication Code operation.
 *
 * @param [in] user
 *    User Type.
 *
 * @param [in] Key_p
 *    Pointer to the key structure of the key.
 *
 * @param [in] KeySize
 *    Size of the key.
 *
 * @param [in] Msg_p
 *    Pointer to the message data.
 *
 * @param [in] MsgLen
 *    Size of the message data.
 *
 * @param [out] Mac_p
 *    Pointer to the output MAC data.
 *
 * @param [in] MacLen
 *    Size of the MAC data.
 *
 * @param [in] Iv_p
 *    Encryption IV for CBC mode.
 *
 */
int
nl_scrt_aes_cbcmac(Nl_Scrt_Usr_t user, uint8_t * Key_p, uint32_t KeySize, uint8_t * Msg_p, uint32_t MsgLen, uint8_t * Mac_p, uint32_t MacLen, uint8_t * Iv_p)
{
    //Link Mailbox here

	return do_CBCMAC_Gen(Key_p, KeySize, Msg_p, MsgLen, Mac_p, MacLen, Iv_p);
}
#endif

#if 0
/*
 * nl_scrt_trng_gen
 *
 * True Random Number Generation.
 *
 * @param [in] user
 *    User Type.
 *
 * @param [in] DataSize
 *    Size of the Random Number.
 *
 * @param [out] Data_p
 *    Pointer to the output Random Number buffer.
 *
 */
ValStatus_t
nl_scrt_trng_gen(Nl_Scrt_Usr_t user, const ValSize_t DataSize, ValOctetsOut_t * const  Data_p)
{
    //Link Mailbox here

	return val_RandomData(DataSize, Data_p);
}
#endif

#if 0
void nl_se_sample_test(){
	//Crypto
	test_sym_crypto_common();
	test_sym_crypto_aes();

	//MAC
	test_aes_cmac();
	test_aes_cbcmac();

    //Random Number
	//Fail, status is not change, #TBC
	//test_rand();
}
#endif

#if 0
/*
 * nl_scrt_aes_ecb
 *
 * AES ECB Operation.
 *
 * @param [in] bEncrypt
 *    true:Encrypt
 *    false:Decrypt
 *
 * @param [in] sk
 *    Key
 *
 * @param [in] sk_len
 *    Size of Key
 *
 * @param [in/out] data_in
 *    Input Data
 *
 * @param [out] data_out
 *    Output Data
 *
 * @param [in] data_len
 *    The length of the Data Transfer
 *
 */
bool nl_scrt_aes_ecb(bool bEncrypt, unsigned char *sk, int sk_len, unsigned char *data_in, unsigned char *data_out, unsigned data_len)
{
    unsigned int data;
    unsigned int r_output_token_data;
    unsigned int c_output_token_id = 0xd82c;
    bool ret = true;
    int i;
    unsigned int word_11;
    int quo, rem;
    int buf_len;
    int key_len = 1;
    int fieldEncrypt;

    if(sk == NULL ||
       data_in == NULL ||
       data_out == NULL){
        return false;
    }

    if(sk_len != 16 &&
       sk_len != 24 &&
       sk_len != 32) {
        return false;
    }

    if(data_len <= 0) {
        return false;
    }

    if(bEncrypt) {
        fieldEncrypt = 1;
    } else {
        fieldEncrypt = 0;
    }

    switch (sk_len)
    {
        case 16:
            key_len = 1;
            break;
        case 24:
            key_len = 2;
            break;
        case 32:
            key_len = 3;
            break;
    }

    //It should be a multiple of block size (16 bytes)
    quo = (data_len / 16);
    rem = (data_len % 16);
    if(rem == 0) {
        buf_len = (quo * 16);
    } else {
        buf_len = ((quo + 1) * 16);
    }

    *(volatile unsigned int *)0x60003f00 = 0x4;

    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x1 ) == 0x0) {
            //success
            break;
        } else {
            continue;
        }
    }

    *(volatile unsigned int *)0x60000000 = 0x0104d82c;
    *(volatile unsigned int *)0x60000004 = 0x00000000;
    *(volatile unsigned int *)0x60000008 = buf_len;
    *(volatile unsigned int *)0x6000000c = (unsigned int) data_in;
    *(volatile unsigned int *)0x60000010 = 0x00000000;
    *(volatile unsigned int *)0x60000014 = buf_len;
    *(volatile unsigned int *)0x60000018 = (unsigned int) data_out;
    *(volatile unsigned int *)0x6000001c = 0x00000000;
    *(volatile unsigned int *)0x60000020 = buf_len + 4;
    *(volatile unsigned int *)0x60000024 = 0x00000000;
    *(volatile unsigned int *)0x60000028 = 0x00000000;

    word_11 = ((key_len << 16) | (fieldEncrypt << 15) | (0x0));
    *(volatile unsigned int *)0x6000002c = word_11;
    //printf("\r\n [nl_aes_ecb] word_11:%lx \r\n", word_11);

    *(volatile unsigned int *)0x60000030 = 0x00000000;
    *(volatile unsigned int *)0x60000034 = 0x00000000;
    *(volatile unsigned int *)0x60000038 = 0x00000000;
    *(volatile unsigned int *)0x6000003c = 0x00000000;
    *(volatile unsigned int *)0x60000040 = 0x00000000;

    for (i=0; i<sk_len; i++) {
        *(volatile unsigned int *)(0x60000044 + i) = *(sk + i);
    }

    *(volatile unsigned int *)0x60000054 = 0x00000000;
    *(volatile unsigned int *)0x60000058 = 0x00000000;
    *(volatile unsigned int *)0x6000005c = 0x00000000;
    *(volatile unsigned int *)0x60000060 = 0x00000000;

    *(volatile unsigned int *)0x60003f00 = 0x1;

    while(1){
        data = *((volatile unsigned int *)0x60003f00);
        if (( data & 0x2 ) == 0x2) {
            //success
            break;
        } else {
            continue;
        }
    }

    r_output_token_data = *((volatile unsigned int *)0x60000c00);
    if (r_output_token_data != c_output_token_id) {
        ret = false;
    }

    r_output_token_data = *((volatile unsigned int *) (data_out + buf_len));
    if ( r_output_token_data != c_output_token_id )
    {
       ret = false;
    }

    *(volatile unsigned int *)0x60003f00 = 0x2;

    return ret;
}


void aes_ecb_sample(void)
{
    int i;
    unsigned char sk[16] = {0x1f, 0x1e, 0x1d, 0x1c, 0x1b, 0x1a, 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10};
    //unsigned char sk[24] = {0x1f, 0x1e, 0x1d, 0x1c, 0x1b, 0x1a, 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    //unsigned char sk[36] = {0x1f, 0x1e, 0x1d, 0x1c, 0x1b, 0x1a, 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    unsigned char data_in[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x60, 0x50};
    unsigned char data_out[16] = {0}; //The length of data_out should be the same as data_in
    unsigned char data_out2[16] = {0};//The length of data_out2 should be the same as data_out

    printf("\r\n aes_ecb_sample \r\n");

    //AEC ECB Encryption
    memset(data_out, 0, 16);
    nl_scrt_aes_ecb(true, sk, sizeof(sk), data_in, data_out, sizeof(data_in));
    for(i=0; i < sizeof(data_in); i++) {
        printf( "output data_out[%d]:%02x \r\n", i, data_out[i]);
    }

    //AES ECB Decryption
    memset(data_out2, 0, 16);
    nl_scrt_aes_ecb(false, sk, sizeof(sk), data_out, data_out2, sizeof(data_out));
    for(i=0; i < sizeof(data_out); i++) {
        printf( "output data_out2[%d]:%02x \r\n", i, data_out2[i]);
    }
}
#endif


#if 0
/**
 * nl_se_aes_cbc_128_encrypt - AES-128 CBC encryption
 * @key: Encryption key
 * @iv: Encryption IV for CBC mode (16 bytes)
 * @data: Data to encrypt in-place
 * @data_len: Length of data in bytes (must be divisible by 16)
 * Returns: 0 on success, -1 on failure
 */
int nl_se_aes_cbc_128_encrypt(const uint8_t *key, const uint8_t *iv, uint8_t *data, size_t data_len, uint8_t *out_data)
{
    SymCryptCtx_t CryptoCntxt;
    TestVector_AES_BASIC_t tv_p;

    if (nl_se_SetupContextAndEnvironment(&CryptoCntxt,
                                       	VAL_SYM_ALGO_CIPHER_AES,
                                       	TESTVECTORS_MODE_CBC,
                                       	key,
                                       	(128/8),
                                       	iv,
                                       	data,
                                       	out_data,
                                       	(uint32_t)data_len) == END_TEST_SUCCES)
    {

        CryptoCntxt.options |= SYM_CRYPT_IN_PLACE;

        if (do_SymCrypt(&CryptoCntxt, false) == END_TEST_SUCCES)
        {
          	return END_TEST_SUCCES;
        }
    }

	return END_TEST_FAIL;
}
#endif

#if 0
/**
 * nl_se_aes_cbc_128_decrypt - AES-128 CBC decryption
 * @key: Decryption key
 * @iv: Decryption IV for CBC mode (16 bytes)
 * @data: Data to decrypt in-place
 * @data_len: Length of data in bytes (must be divisible by 16)
 * Returns: 0 on success, -1 on failure
 */
int nl_se_aes_cbc_128_decrypt(const uint8_t *key, const uint8_t *iv, uint8_t *data, size_t data_len, uint8_t *out_data)
{
    SymCryptCtx_t CryptoCntxt;
    TestVector_AES_BASIC_t tv_p;

    if (nl_se_SetupContextAndEnvironment(&CryptoCntxt,
                                       	VAL_SYM_ALGO_CIPHER_AES,
                                       	TESTVECTORS_MODE_CBC,
                                       	key,
                                       	(128/8),
                                       	iv,
                                       	data,
                                       	out_data,
                                       	(uint32_t)data_len) == END_TEST_SUCCES)
    {

        CryptoCntxt.options |= SYM_CRYPT_IN_PLACE;

        if (do_SymCrypt(&CryptoCntxt, true) == END_TEST_SUCCES)
        {
        	return END_TEST_SUCCES;
        }
    }

	return END_TEST_FAIL;

}
#endif

#if 0
/**
 * nl_se_aes_cbc_256_encrypt - AES-256 CBC encryption
 * @key: Encryption key
 * @iv: Encryption IV for CBC mode (16 bytes)
 * @data: Data to encrypt in-place
 * @data_len: Length of data in bytes (must be divisible by ?)
 * Returns: 0 on success, -1 on failure
 */
int nl_se_aes_cbc_256_encrypt(const uint8_t *key, const uint8_t *iv, uint8_t *data, size_t data_len, uint8_t *out_data)
{
    SymCryptCtx_t CryptoCntxt;
    TestVector_AES_BASIC_t tv_p;

    if (nl_se_SetupContextAndEnvironment(&CryptoCntxt,
                                       	VAL_SYM_ALGO_CIPHER_AES,
                                       	TESTVECTORS_MODE_CBC,
                                       	key,
                                       	(256/8),
                                       	iv,
                                       	data,
                                       	out_data,
                                       	(uint32_t)data_len) == END_TEST_SUCCES)
    {

        CryptoCntxt.options |= SYM_CRYPT_IN_PLACE;

        if (do_SymCrypt(&CryptoCntxt, false) == END_TEST_SUCCES)
        {
          	return END_TEST_SUCCES;
        }
    }

	return END_TEST_FAIL;
}
#endif

#if 0
/**
 * nl_se_aes_cbc_256_decrypt - AES-256 CBC decryption
 * @key: Decryption key
 * @iv: Decryption IV for CBC mode (16 bytes)
 * @data: Data to decrypt in-place
 * @data_len: Length of data in bytes (must be divisible by ?)
 * Returns: 0 on success, -1 on failure
 */
int nl_se_aes_cbc_256_decrypt(const uint8_t *key, const uint8_t *iv, uint8_t *data, size_t data_len, uint8_t *out_data)
{
    SymCryptCtx_t CryptoCntxt;
    TestVector_AES_BASIC_t tv_p;

    if (nl_se_SetupContextAndEnvironment(&CryptoCntxt,
                                       	VAL_SYM_ALGO_CIPHER_AES,
                                       	TESTVECTORS_MODE_CBC,
                                       	key,
                                       	(256/8),
                                       	iv,
                                       	data,
                                       	out_data,
                                       	(uint32_t)data_len) == END_TEST_SUCCES)
    {

        CryptoCntxt.options |= SYM_CRYPT_IN_PLACE;

        if (do_SymCrypt(&CryptoCntxt, true) == END_TEST_SUCCES)
        {
        	return END_TEST_SUCCES;
        }
    }

	return END_TEST_FAIL;

}
#endif

#if 0
int nl_se_aes_ecb_128_encrypt(const uint8_t *key, const uint8_t *iv, uint8_t *data, size_t data_len, uint8_t *out_data)
{
    SymCryptCtx_t CryptoCntxt;
    TestVector_AES_BASIC_t tv_p;

    if (nl_se_SetupContextAndEnvironment(&CryptoCntxt,
                                       	VAL_SYM_ALGO_CIPHER_AES,
                                       	TESTVECTORS_MODE_ECB,
                                       	key,
                                       	(128/8),
                                       	NULL,
                                       	data,
                                       	out_data,
                                       	(uint32_t)data_len) == END_TEST_SUCCES)
    {

        CryptoCntxt.options |= SYM_CRYPT_IN_PLACE;

        if (do_SymCrypt(&CryptoCntxt, false) == END_TEST_SUCCES)
        {
          	return END_TEST_SUCCES;
        }
    }

	return END_TEST_FAIL;
}
#endif

#if 0
int nl_se_aes_ecb_128_decrypt(const uint8_t *key, const uint8_t *iv, uint8_t *data, size_t data_len, uint8_t *out_data)
{
    SymCryptCtx_t CryptoCntxt;
    TestVector_AES_BASIC_t tv_p;

    if (nl_se_SetupContextAndEnvironment(&CryptoCntxt,
                                       	VAL_SYM_ALGO_CIPHER_AES,
                                       	TESTVECTORS_MODE_ECB,
                                       	key,
                                       	(128/8),
                                       	NULL,
                                       	data,
                                       	out_data,
                                       	(uint32_t)data_len) == END_TEST_SUCCES)
    {

        CryptoCntxt.options |= SYM_CRYPT_IN_PLACE;

        if (do_SymCrypt(&CryptoCntxt, true) == END_TEST_SUCCES)
        {
          	return END_TEST_SUCCES;
        }
    }

	return END_TEST_FAIL;
}
#endif

#if 0
int nl_se_aes_ecb_256_encrypt(const uint8_t *key, const uint8_t *iv, uint8_t *data, size_t data_len, uint8_t *out_data)
{
    SymCryptCtx_t CryptoCntxt;
    TestVector_AES_BASIC_t tv_p;

    if (nl_se_SetupContextAndEnvironment(&CryptoCntxt,
                                       	VAL_SYM_ALGO_CIPHER_AES,
                                       	TESTVECTORS_MODE_ECB,
                                       	key,
                                       	(256/8),
                                       	NULL,
                                       	data,
                                       	out_data,
                                       	(uint32_t)data_len) == END_TEST_SUCCES)
    {

        CryptoCntxt.options |= SYM_CRYPT_IN_PLACE;

        if (do_SymCrypt(&CryptoCntxt, false) == END_TEST_SUCCES)
        {
          	return END_TEST_SUCCES;
        }
    }

	return END_TEST_FAIL;
}
#endif

#if 0
int nl_se_aes_ecb_256_decrypt(const uint8_t *key, const uint8_t *iv, uint8_t *data, size_t data_len, uint8_t *out_data)
{
    SymCryptCtx_t CryptoCntxt;
    TestVector_AES_BASIC_t tv_p;

    if (nl_se_SetupContextAndEnvironment(&CryptoCntxt,
                                       	VAL_SYM_ALGO_CIPHER_AES,
                                       	TESTVECTORS_MODE_ECB,
                                       	key,
                                       	(128/8),
                                       	NULL,
                                       	data,
                                       	out_data,
                                       	(uint32_t)data_len) == END_TEST_SUCCES)
    {

        CryptoCntxt.options |= SYM_CRYPT_IN_PLACE;

        if (do_SymCrypt(&CryptoCntxt, true) == END_TEST_SUCCES)
        {
          	return END_TEST_SUCCES;
        }
    }

	return END_TEST_FAIL;
}

#endif

