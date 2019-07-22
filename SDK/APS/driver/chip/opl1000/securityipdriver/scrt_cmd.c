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

#include "scrt_cmd.h"


#ifdef SCRT_CMD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scrt.h"
#include "le_smp_util.h"
#include "diag_task.h"


#define SCRT_CMD_ECDH
#define SCRT_CMD_AES_CCM
#define SCRT_CMD_HMAC_SHA_1
#define SCRT_CMD_AES_ECB
#define SCRT_CMD_SUT_TASK
//#define SCRT_CMD_HMAC_SHA_1_STEP
#define SCRT_CMD_AES_CMAC
#define SCRT_CMD_SHA

//#define SCRT_KEY_PAIR_1
#define SCRT_DHKEY_1
//#define SCRT_AES_CCM_ENC_1
//#define SCRT_AES_CCM_DEC_1
//#define SCRT_HMAC_SHA1_1
//#define SCRT_AES_ECB_ENC_1
//#define SCRT_AES_ECB_DEC_1

//#define SCRT_KEY_PAIR_2
//#define SCRT_DHKEY_2
#define SCRT_AES_CCM_ENC_2
#define SCRT_AES_CCM_DEC_2
#define SCRT_HMAC_SHA1_2
#define SCRT_AES_ECB_ENC_2
#define SCRT_AES_ECB_DEC_2

//#define SCRT_KEY_PAIR_3
//#define SCRT_DHKEY_3
#define SCRT_AES_CCM_ENC_3
#define SCRT_AES_CCM_DEC_3
#define SCRT_HMAC_SHA1_3
#define SCRT_AES_ECB_ENC_3
#define SCRT_AES_ECB_DEC_3

//#define SCRT_KEY_PAIR_4
//#define SCRT_DHKEY_4
#define SCRT_AES_CCM_ENC_4
#define SCRT_AES_CCM_DEC_4
#define SCRT_HMAC_SHA1_4
#define SCRT_AES_ECB_ENC_4
#define SCRT_AES_ECB_DEC_4

#define SCRT_SUT_TASK_NUM           4
#define SCRT_SUT_TASK_DELAY         200 // ms
#define SCRT_SUT_TASK_STACK_SIZE    512

#define SCRT_CURR_TIME              (*(volatile uint32_t *)(0x40003044))


// internal
#ifdef SCRT_CMD_SUT_TASK
RET_DATA os_pthread scrt_sut_task_main_1;
RET_DATA os_pthread scrt_sut_task_main_2;
RET_DATA os_pthread scrt_sut_task_main_3;
RET_DATA os_pthread scrt_sut_task_main_4;
RET_DATA scrt_sut_task_delete_fp_t scrt_sut_task_delete;
RET_DATA scrt_sut_task_create_fp_t scrt_sut_task_create;
#endif //#ifdef SCRT_CMD_SUT_TASK

// external
RET_DATA nl_scrt_cmd_fp_t nl_scrt_cmd;


#ifdef SCRT_CMD_ECDH

uint32_t g_u32aHwPubKey[16] = {0};
uint32_t g_u32aSwPrivKey[8] = {0};
uint32_t g_u32aSwPubKey[16] = {0};

// 7.1.2.1 P-256 Data Set 1
// Private key of A
const uint32_t g_u32aAPrivKey1[] = 
{
    0xcd3c1abd, 0x5899b8a6, 0xeb40b799, 0x4aff607b, 
    0xd2103f50, 0x74c9b3e3, 0xa3c55f38, 0x3f49f6d4 
};

// Public key of B
const uint8_t g_u8aBPubKeyX1[] = 
{
    0x90, 0xa1, 0xaa, 0x2f, 0xb2, 0x77, 0x90, 0x55, 0x9f, 0xa6, 0x15, 0x86, 0xfd, 0x8a, 0xb5, 0x47, 
    0x00, 0x4c, 0x9e, 0xf1, 0x84, 0x22, 0x59, 0x09, 0x96, 0x1d, 0xaf, 0x1f, 0xf0, 0xf0, 0xa1, 0x1e
};

const uint8_t g_u8aBPubKeyY1[] = 
{
    0x4a, 0x21, 0xb1, 0x15, 0xf9, 0xaf, 0x89, 0x5f, 0x76, 0x36, 0x8e, 0xe2, 0x30, 0x11, 0x2d, 0x47, 
    0x60, 0x51, 0xb8, 0x9a, 0x3a, 0x70, 0x56, 0x73, 0x37, 0xad, 0x9d, 0x42, 0x3e, 0xf3, 0x55, 0x4c
};
// --------------------------------------------------------------------
// Private key of B
const uint32_t g_u32aBPrivKey1[] = 
{
    // LSB to MSB
    0xf47fc5fd, 0x6b4fdd49, 0xf19d7cfb, 0x59cb9ac2, 
    0xeed4e72a, 0x900afcfb, 0x32f6bb9a, 0x55188b3d 
};

// Public key of A
const uint8_t g_u8aAPubKeyX1[] = 
{
    0xe6, 0x9d, 0x35, 0x0e, 0x48, 0x01, 0x03, 0xcc, 0xdb, 0xfd, 0xf4, 0xac, 0x11, 0x91, 0xf4, 0xef,
    0xb9, 0xa5, 0xf9, 0xe9, 0xa7, 0x83, 0x2c, 0x5e, 0x2c, 0xbe, 0x97, 0xf2, 0xd2, 0x03, 0xb0, 0x20
};

const uint8_t g_u8aAPubKeyY1[] = 
{
    0x8b, 0xd2, 0x89, 0x15, 0xd0, 0x8e, 0x1c, 0x74, 0x24, 0x30, 0xed, 0x8f, 0xc2, 0x45, 0x63, 0x76,
    0x5c, 0x15, 0x52, 0x5a, 0xbf, 0x9a, 0x32, 0x63, 0x6d, 0xeb, 0x2a, 0x65, 0x49, 0x9c, 0x80, 0xdc
};

// 7.1.2.1 P-256 Data Set 2
// Private key of A
const uint32_t g_u32aAPrivKey2[] = 
{
    0xd0457663, 0xb7ac73f7, 0x7203ddff, 0xb48572b9, 
    0x0c5db641, 0x6084545d, 0x3c9aa31a, 0x06a51669 
};

// Public key of B
const uint8_t g_u8aBPubKeyX2[] = 
{
    0xcc, 0x00, 0x65, 0xe1, 0xf5, 0x6c, 0x0d, 0xcf, 
    0xec, 0x96, 0x47, 0x20, 0x66, 0xc9, 0xdb, 0x84, 
    0x81, 0x75, 0xa8, 0x4d, 0xc0, 0xdf, 0xc7, 0x9d,
    0x1b, 0x3f, 0x3d, 0xf2, 0x3f, 0xe4, 0x65, 0xf4
};

const uint8_t g_u8aBPubKeyY2[] = 
{
    0x79, 0xb2, 0xec, 0xd8, 0xca, 0x55, 0xa1, 0xa8, 
    0x43, 0x4d, 0x6b, 0xca, 0x10, 0xb0, 0xc2, 0x01,
    0xc2, 0x33, 0x4e, 0x16, 0x24, 0xc4, 0xef, 0xee, 
    0x99, 0xd8, 0xbb, 0xbc, 0x48, 0xd0, 0x01, 0x02
};

// --------------------------------------------------------------------
// Private key of B
const uint32_t g_u32aBPrivKey2[] = 
{
    // LSB to MSB
    0x505530ba, 0xa3caa219, 0xc60829a5, 0x7e8803b5, 
    0x73502b03, 0x97502ed4, 0x0d72cd64, 0x529aa067 
};

// Public key of A
const uint8_t g_u8aAPubKeyX2[] = 
{
    0xdd, 0x78, 0x5c, 0x74, 0x03, 0x9b, 0x7e, 0x98,
    0xcb, 0x94, 0x87, 0x4a, 0xad, 0xfa, 0xf8, 0xd5,
    0x43, 0x3e, 0x5c, 0xaf, 0xea, 0xb5, 0x4c, 0xf4, 
    0x9e, 0x80, 0x79, 0x57, 0x7b, 0xa4, 0x31, 0x2c
};

const uint8_t g_u8aAPubKeyY2[] = 
{
    0x4f, 0x5d, 0x71, 0x43, 0x77, 0x43, 0xf8, 0xea, 
    0xd4, 0x3e, 0xbd, 0x17, 0x91, 0x10, 0x21, 0xd0,
    0x1f, 0x87, 0x43, 0x8e, 0x40, 0xe2, 0x52, 0xcd,
    0xbe, 0xdf, 0x98, 0x38, 0x18, 0x12, 0x95, 0x91
};
#endif //#ifdef SCRT_CMD_ECDH

#ifdef SCRT_CMD_AES_CCM
const uint8_t g_u8aScrtAesCcmSk[16] = 
{
    0x5D, 0xA7, 0x86, 0xFF, 0xD9, 0x89, 0xC0, 0x91, 0x03, 0xB4, 0xEC, 0xDC, 0x24, 0xAE, 0x49, 0x36, 
};

const uint8_t g_u8aScrtAesCcmNonce[13] = 
{
    0x00, 0x10, 0x62, 0xEB, 0xF8, 0x03, 0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10,
};

const uint8_t g_u8aScrtAesCcmAdata[22] = 
{
    0x08, 0x42, 0xAA, 0xD0, 0x59, 0x12, 0x34, 0x56, 0x10, 0x62, 0xEB, 0xF8, 0x03, 0xAA, 0x74, 0xDA, 
    0x38, 0xB0, 0x57, 0x46, 0x00, 0x00, 
};

const uint8_t g_u8aScrtAesCcmTag[8] = 
{
    0x9C, 0x8B, 0xAD, 0x10, 0x96, 0x84, 0x9C, 0xE6, 
};

const uint8_t g_u8ScrtAesCcmPlainText[286 + 6] = 
{
    0xAA, 0xAA, 0x03, 0x00, 0x00, 0x00, 0x08, 0x00, 0x45, 0x00, 0x01, 0x16, 0x37, 0xBB, 0x00, 0x00, 
    0x80, 0x01, 0x80, 0x12, 0xC0, 0xA8, 0x00, 0x64, 0xC0, 0xA8, 0x00, 0x65, 0x08, 0x00, 0x1B, 0x0D, 
    0x00, 0x01, 0x00, 0x0B, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 
    0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x61, 0x62, 0x63, 0x64, 0x65, 
    0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 
    0x76, 0x77, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 
    0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 
    0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 
    0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 
    0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 
    0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x61, 0x62, 
    0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 
    0x73, 0x74, 0x75, 0x76, 0x77, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 
    0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x61, 0x62, 0x63, 0x64, 
    0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 
    0x75, 0x76, 0x77, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 
    0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 
    0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

const uint8_t g_u8aScrtAesCcmCipher[286 + 6] = 
{
    0x38, 0xD7, 0xCB, 0xF0, 0xE7, 0x88, 0x2A, 0x76, 0xE1, 0x97, 0x6B, 0x6F, 0xFA, 0x68, 0xF7, 0xB3, 
    0x0F, 0x12, 0x73, 0xFD, 0xAF, 0xC5, 0x88, 0x2F, 0xE8, 0x4A, 0x40, 0xA1, 0xED, 0x6C, 0x34, 0xD7, 
    0xDF, 0x31, 0xE9, 0x75, 0x3C, 0x64, 0xA4, 0x78, 0x17, 0x1E, 0xF1, 0x0A, 0x1C, 0xFE, 0xA0, 0xF6, 
    0x5E, 0x58, 0x47, 0xBF, 0x66, 0x75, 0x31, 0x4B, 0x3C, 0x90, 0xD6, 0x6D, 0x37, 0x13, 0x66, 0xF1, 
    0x4C, 0x93, 0xF3, 0xEC, 0xD0, 0xE6, 0x57, 0x55, 0x3A, 0xB4, 0x46, 0x52, 0x37, 0xBC, 0x5A, 0xDE, 
    0x43, 0x27, 0xE5, 0x46, 0xE1, 0xF1, 0xA9, 0x8E, 0x35, 0x9F, 0x30, 0x9E, 0xA5, 0x84, 0x43, 0x64, 
    0x65, 0x38, 0x16, 0x66, 0xF5, 0xEF, 0x67, 0x68, 0xAD, 0x7F, 0x16, 0xAD, 0x37, 0x04, 0xF0, 0x45, 
    0x24, 0xE4, 0xFC, 0x15, 0x06, 0x6C, 0x7E, 0x37, 0x6D, 0x40, 0xE0, 0xC0, 0x3F, 0x38, 0xF0, 0x2A, 
    0x91, 0x4A, 0xC0, 0x8E, 0x10, 0x66, 0x97, 0xF3, 0xB3, 0x2E, 0xC9, 0xAF, 0x66, 0x04, 0xDE, 0x37, 
    0xE5, 0xF3, 0xF6, 0x21, 0xC1, 0x7D, 0x54, 0x18, 0x5D, 0xFB, 0x5F, 0x39, 0xA6, 0x7D, 0xD1, 0xAA, 
    0xE5, 0x20, 0x87, 0xE6, 0xD0, 0x2D, 0xDB, 0x63, 0xAE, 0xDA, 0xC9, 0x33, 0xE9, 0x28, 0x36, 0x4D, 
    0xBF, 0xE4, 0x2B, 0x94, 0x52, 0x92, 0x05, 0xA2, 0x17, 0xF6, 0x85, 0x35, 0x19, 0x46, 0x4D, 0x6D, 
    0x88, 0xA3, 0xA5, 0x2F, 0xE7, 0xC5, 0xA2, 0xAB, 0xC3, 0x67, 0xB1, 0x34, 0x9E, 0x1D, 0x49, 0x70, 
    0xB8, 0x72, 0x48, 0x0F, 0x3A, 0x58, 0xDE, 0xD0, 0x8D, 0x22, 0x08, 0x77, 0x6A, 0x6E, 0x7D, 0xE5, 
    0x75, 0x92, 0xFE, 0x45, 0xCD, 0xB0, 0x8D, 0x1B, 0xA4, 0xC4, 0x58, 0xA3, 0x76, 0x52, 0x22, 0x9E, 
    0xF4, 0x72, 0x85, 0xCD, 0xF6, 0xC4, 0x94, 0x47, 0x96, 0xCB, 0x52, 0xF7, 0x6A, 0xAE, 0x12, 0x5D, 
    0xC2, 0x60, 0xBE, 0x8B, 0x77, 0xA4, 0xDE, 0x4F, 0x1F, 0x3A, 0x3E, 0xDD, 0x51, 0xAB, 0x70, 0x7A, 
    0x44, 0x55, 0x80, 0x48, 0xAF, 0x52, 0x4E, 0xD3, 0xB3, 0xE7, 0x90, 0x59, 0x7D, 0x55, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};
#endif //#ifdef SCRT_CMD_AES_CCM

#ifdef SCRT_CMD_HMAC_SHA_1
const uint8_t g_u8aHmacSha1Sk[] = 
{
    0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38
};

const uint8_t g_u8aHmacSha1Data[] = 
{
    0x0E, 0x56, 0xE1, 0xB8, 0xFE, 0x65, 0x07, 0x35, 0x37, 0x60, 0x4A, 0x90, 0x57, 0x92, 0xF0, 0xE7, 
    0x61, 0x1F, 0xFB, 0xB7
};

const uint8_t g_u8aHmacSha1Mac[] = 
{
    0xF4, 0x40, 0xBF, 0x29, 0x17, 0xB5, 0x31, 0x62, 0x0F, 0xB2, 0x6B, 0xF2, 0x74, 0x73, 0x59, 0x28, 
    0xE4, 0x60, 0x6A, 0x64
};
#endif //#ifdef SCRT_CMD_HMAC_SHA_1

#ifdef SCRT_CMD_AES_ECB
const uint8_t g_u8aAesEcbSk[16] = 
{
    0x4E, 0x58, 0xED, 0x48, 0xFC, 0xB5, 0x44, 0x66, 0x24, 0xBD, 0xF8, 0x40, 0x48, 0xE1, 0x9A, 0x27,
};

const uint8_t g_u8aAesEcbInput[16] = 
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
};

const uint8_t g_u8aAesEcbOutput[16] = 
{
    0xF1, 0x4D, 0xF1, 0x0A, 0x2E, 0x9A, 0xF7, 0x4E, 0x5A, 0x4F, 0xB0, 0x8B, 0xBF, 0x5D, 0x19, 0x98,
};
#endif //#ifdef SCRT_CMD_AES_ECB

#ifdef SCRT_CMD_AES_CMAC
const uint8_t g_u8aScrtAesCmacSk[] = 
{
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
    0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F 
};

const uint8_t g_u8aScrtAesCmacData[128] = 
{
    0x38, 0xD7, 0xCB, 0xF0, 0xE7, 0x88, 0x2A, 0x76, 0xE1, 0x97, 0x6B, 0x6F, 0xFA, 0x68, 0x80, 0xB3, 
    0x0F, 0x12, 0x73, 0x00, 0xAF, 0xC5, 0x88, 0x2F, 0xE8, 0x4A, 0x40, 0xA1, 0xED, 0x6C, 0x34, 0xD7, 
    0xDF, 0x31, 0xE9, 0x75, 0x3C, 0x64, 0xA4, 0x78, 0x17, 0x1E, 0x80, 0x0A, 0x1C, 0xFE, 0xA0, 0xF6, 
    0x5E, 0x58, 0x47, 0xBF, 0x66, 0x75, 0x00, 0x4B, 0x3C, 0x90, 0xD6, 0x6D, 0x37, 0x13, 0x66, 0xF1, 
    0x4C, 0x93, 0xF3, 0xEC, 0xD0, 0x00, 0x57, 0x55, 0x3A, 0xB4, 0x46, 0x52, 0x37, 0xBC, 0x5A, 0xDE, 
};
#endif //#ifdef SCRT_CMD_AES_CMAC

#ifdef SCRT_CMD_HMAC_SHA_1_STEP
const uint8_t g_u8aScrtHmacSha1SkStep[] = 
{
    0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38
};
#endif //#ifdef SCRT_CMD_HMAC_SHA_1_STEP

#ifdef SCRT_CMD_SHA
const uint8_t g_u8aScrtShaData[] = 
{
    0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 
    0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 
    0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x61, 0x62, 
    0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 
    
    0x73, 0x74, 0x75, 0x76, 0x77, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B,
    0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 
    0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 
    0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x61, 0x62, 

    0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 
    0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 
    0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x61, 0x62, 
    0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 
    
    0x73, 0x74, 0x75, 0x76, 0x77, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B,
    0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 
    0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 
    0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x61, 0x62, 
    
};
#endif //#ifdef SCRT_CMD_SHA

#ifdef SCRT_CMD_SUT_TASK

volatile uint8_t g_bScrtSutTaskRun = 0;
osThreadId g_taScrtSutThreadId[SCRT_SUT_TASK_NUM] = {0};

const char *g_saScrtSutTaskName[SCRT_SUT_TASK_NUM] = 
{
    "scrt_sut_1", 
    "scrt_sut_2", 
    "scrt_sut_3",
    "scrt_sut_4"
};

uint32_t g_baScrtError[SCRT_SUT_TASK_NUM] = {0};
uint32_t g_baScrtCnt[SCRT_SUT_TASK_NUM] = {0};


void scrt_sut_task_main_1_impl(void *pParam)
{
    #ifdef SCRT_KEY_PAIR_1
    uint32_t u32PrivKeyId = 0;
    #endif

    #ifdef SCRT_DHKEY_1
    uint32_t u32aSelfPrivKey[8] = {0};
    uint8_t u8aPeerPubKeyX[32] = {0};
    uint8_t u8aPeerPubKeyY[32] = {0};

    memcpy(u32aSelfPrivKey, g_u32aBPrivKey1, sizeof(u32aSelfPrivKey));
    memcpy(u8aPeerPubKeyX, g_u8aAPubKeyX1, sizeof(u8aPeerPubKeyX));
    memcpy(u8aPeerPubKeyY, g_u8aAPubKeyY1, sizeof(u8aPeerPubKeyY));
    #endif

    #if defined(SCRT_AES_CCM_ENC_1) || defined(SCRT_AES_CCM_DEC_1)
    uint8_t u8aScrtAesCcmSk[16] = {0};
    uint8_t u8aScrtAesCcmNonce[13] = {0};
    uint8_t u8aScrtAesCcmAdata[22] = {0};
    uint8_t u8ScrtAesCcmPlainText[286 + 6] = {0};
    uint8_t u8aScrtAesCcmCipher[286 + 6] = {0};
    uint8_t u8aScrtAesCcmTag[8] = {0};

    memcpy(u8aScrtAesCcmSk, g_u8aScrtAesCcmSk, sizeof(u8aScrtAesCcmSk));
    memcpy(u8aScrtAesCcmNonce, g_u8aScrtAesCcmNonce, sizeof(u8aScrtAesCcmNonce));
    memcpy(u8aScrtAesCcmAdata, g_u8aScrtAesCcmAdata, sizeof(u8aScrtAesCcmAdata));
    memcpy(u8ScrtAesCcmPlainText, g_u8ScrtAesCcmPlainText, sizeof(u8ScrtAesCcmPlainText));
    memcpy(u8aScrtAesCcmCipher, g_u8aScrtAesCcmCipher, sizeof(u8aScrtAesCcmCipher));
    memcpy(u8aScrtAesCcmTag, g_u8aScrtAesCcmTag, sizeof(g_u8aScrtAesCcmTag));
    #endif

    #ifdef SCRT_HMAC_SHA1_1
    uint8_t u8aHmacSha1Sk[16] = {0};
    uint8_t u8aHmacSha1Data[20] = {0}; 
    uint8_t u8aHmacSha1Mac[20] = {0};
    uint32_t u32HmacSha1SkLen = 8;
    uint32_t u32HmacSha1DataLen = 20;

    memcpy(u8aHmacSha1Sk, g_u8aHmacSha1Sk, sizeof(u8aHmacSha1Sk));
    memcpy(u8aHmacSha1Data, g_u8aHmacSha1Data, sizeof(u8aHmacSha1Data));
    memcpy(u8aHmacSha1Mac, g_u8aHmacSha1Mac, sizeof(u8aHmacSha1Mac));
    #endif

    #if defined(SCRT_AES_ECB_ENC_1) || defined(SCRT_AES_ECB_DEC_1)
    uint8_t u8aAesEcbSk[16] = {0};
    uint8_t u8aAesEcbInput[16] = {0};
    uint8_t u8aAesEcbOutput[16] = {0};

    memcpy(u8aAesEcbSk, g_u8aAesEcbSk, sizeof(u8aAesEcbSk));
    memcpy(u8aAesEcbInput, g_u8aAesEcbInput, sizeof(u8aAesEcbInput));
    memcpy(u8aAesEcbOutput, g_u8aAesEcbOutput, sizeof(u8aAesEcbOutput));
    #endif

    while(g_bScrtSutTaskRun)
    {
        #ifdef SCRT_KEY_PAIR_1
        uint32_t u32aPubKey[16] = {0};
        uint32_t u32aPrivKey[8] = {0};
        #endif

        #ifdef SCRT_DHKEY_1
        uint8_t u8aDhKeyHw[32] = {0};
        //uint8_t u8aDhKeySw[32] = {0};
        //uint32_t *u32aSelfPrivKey = g_u32aBPrivKey1;
        //uint32_t *u32aPeerPrivKey = g_u32aAPrivKey1;
        //uint8_t *u8aPeerPubKeyX = g_u8aAPubKeyX1;
        //uint8_t *u8aPeerPubKeyY = g_u8aAPubKeyY1;
        #endif

        #if defined(SCRT_AES_CCM_ENC_1) || defined(SCRT_AES_CCM_DEC_1)
        uint32_t u32SkLen = 16;
        uint32_t u32NonceLen = 13;
        uint32_t u32AdataLen = 22;
        uint32_t u32TagLen = 8;
        uint32_t u32DataLen = 286;
        uint8_t u8aBuf[286 + 6] = {0};
        uint8_t u8aTag[8] = {0};
        #endif

        #ifdef SCRT_HMAC_SHA1_1
        uint8_t u8aHmacSha1Output[20] = {0};
        #endif

        #if defined(SCRT_AES_ECB_ENC_1) || defined(SCRT_AES_ECB_DEC_1)
        uint32_t u32AesEcbSkLen = 16;
        uint32_t u32AesEcbDataLen = 16;
        uint8_t u8aAesEcbBuf[64] = {0};
        #endif

    #if 1
        #ifdef SCRT_KEY_PAIR_1
        if(!nl_scrt_ecdh_key_pair_gen(u32aPubKey, u32aPrivKey, &u32PrivKeyId))
        {
            tracer_log(LOG_HIGH_LEVEL, "[%s %d] 1 key pair FAIL\n", __func__, __LINE__);
            g_baScrtError[0] = 1;
            break;
        }
        else
        {
            //tracer_log(LOG_HIGH_LEVEL, "[%s %d] 1 key pair SUCCESS\n", __func__, __LINE__);
        }
        #endif

        #ifdef SCRT_DHKEY_1
        if(!nl_scrt_ecdh_dhkey_gen(u8aPeerPubKeyX, u8aPeerPubKeyY, u32aSelfPrivKey, u8aDhKeyHw, 0))
        {
            tracer_log(LOG_HIGH_LEVEL, "[%s %d] 1 dhkey FAIL\n", __func__, __LINE__);
            g_baScrtError[0] = 2;
            break;
        }
        else
        {
            //tracer_log(LOG_HIGH_LEVEL, "[%s %d] 1 dhkey SUCCESS\n", __func__, __LINE__);
        }
        #endif

        #ifdef SCRT_AES_CCM_ENC_1
        if(!nl_scrt_aes_ccm(1, 
                            u8aScrtAesCcmSk, u32SkLen, 
                            u8aScrtAesCcmNonce, u32NonceLen, 
                            u8aScrtAesCcmAdata, u32AdataLen, 
                            u8ScrtAesCcmPlainText, u8aBuf, u32DataLen, 
                            u8aTag, u32TagLen))
        {
            tracer_log(LOG_HIGH_LEVEL, "[%s %d] 1 aes-ccm enc FAIL\n", __func__, __LINE__);
            g_baScrtError[0] = 3;
            break;
        }
        else
        {
            //tracer_log(LOG_HIGH_LEVEL, "[%s %d] 1 aes-ccm enc SUCCESS\n", __func__, __LINE__);
        }
        #endif

        #ifdef SCRT_AES_CCM_DEC_1
        if(!nl_scrt_aes_ccm(0, 
                            u8aScrtAesCcmSk, u32SkLen, 
                            u8aScrtAesCcmNonce, u32NonceLen, 
                            u8aScrtAesCcmAdata, u32AdataLen, 
                            u8aBuf, u8aScrtAesCcmCipher, u32DataLen, 
                            u8aScrtAesCcmTag, u32TagLen))
        {
            tracer_log(LOG_HIGH_LEVEL, "[%s %d] 1 aes-ccm dec FAIL\n", __func__, __LINE__);
            g_baScrtError[0] = 4;
            break;
        }
        else
        {
            //tracer_log(LOG_HIGH_LEVEL, "[%s %d] 1 aes-ccm dec SUCCESS\n", __func__, __LINE__);
        }
        #endif

        #ifdef SCRT_HMAC_SHA1_1
        if(!nl_hmac_sha_1(u8aHmacSha1Sk, u32HmacSha1SkLen, u8aHmacSha1Data, u32HmacSha1DataLen, u8aHmacSha1Output))
        {
            tracer_log(LOG_HIGH_LEVEL, "[%s %d] 1 hmac-sh1-1 FAIL\n", __func__, __LINE__);
            g_baScrtError[0] = 5;
            break;
        }
        #endif

        #ifdef SCRT_AES_ECB_ENC_1
        if(!nl_scrt_aes_ecb(1, 
                            u8aAesEcbSk, u32AesEcbSkLen, 
                            u8aAesEcbInput, u8aAesEcbBuf, u32AesEcbDataLen))
        {
            tracer_log(LOG_HIGH_LEVEL, "[%s %d] 1 aes-ecb enc FAIL\n", __func__, __LINE__);
            g_baScrtError[0] = 6;
            break;
        }
        #endif

        #ifdef SCRT_AES_ECB_DEC_1
        if(!nl_scrt_aes_ecb(0, 
                            u8aAesEcbSk, u32AesEcbSkLen, 
                            u8aAesEcbOutput, u8aAesEcbBuf, u32AesEcbDataLen))
        {
            tracer_log(LOG_HIGH_LEVEL, "[%s %d] 1 aes-ecb dec FAIL\n", __func__, __LINE__);
            g_baScrtError[0] = 7;
            break;
        }
        #endif

        #ifdef SCRT_DHKEY_1
        //osDelay(1);
        #endif

        g_baScrtCnt[0] += 1;
    #else
        if(!nl_scrt_ecdh_key_pair_gen(u32aPubKey, u32aPrivKey, &u32PrivKeyId))
        {
            tracer_log(LOG_HIGH_LEVEL, "[%s %d] 1 key pair FAIL\n", __func__, __LINE__);
            //goto done;
        }
        else
        {
            tracer_log(LOG_HIGH_LEVEL, "[%s %d] 1 key pair SUCCESS\n", __func__, __LINE__);

            osDelay(SCRT_SUT_TASK_DELAY);

            if(!nl_scrt_ecdh_dhkey_gen(u8aPeerPubKeyX, u8aPeerPubKeyY, NULL, u8aDhKeyHw, u32PrivKeyId))
            {
                tracer_log(LOG_HIGH_LEVEL, "[%s %d] 1 dhkey FAIL\n", __func__, __LINE__);
            }
            else
            {
                tracer_log(LOG_HIGH_LEVEL, "[%s %d] 1 dhkey SUCCESS\n", __func__, __LINE__);

                if(LeSmpUtilGenDhkey((uint8_t *)&u32aPubKey[0], (uint8_t *)&u32aPubKey[8], u32aPeerPrivKey, u8aDhKeySw))
                {
                    tracer_log(LOG_HIGH_LEVEL, "[%s %d] 1 LeSmpUtilGenDhkey fail\n", __func__, __LINE__);
                }
                else
                {
                    if(memcmp(u8aDhKeyHw, u8aDhKeySw, sizeof(u8aDhKeyHw)))
                    {
                        tracer_log(LOG_HIGH_LEVEL, "[%s %d] 1 dhkey compare FAIL\n", __func__, __LINE__);
                    }
                    else
                    {
                        tracer_log(LOG_HIGH_LEVEL, "[%s %d] 1 dhkey compare SUCCESS\n", __func__, __LINE__);
                    }
                }
            }
        }

        osDelay(SCRT_SUT_TASK_DELAY);
    #endif
    }

    vTaskDelete(NULL);
    return;
}

void scrt_sut_task_main_2_impl(void *pParam)
{
    #ifdef SCRT_KEY_PAIR_2
    uint32_t u32PrivKeyId = 0;
    #endif

    #ifdef SCRT_DHKEY_2
    uint32_t u32aSelfPrivKey[8] = {0};
    uint8_t u8aPeerPubKeyX[32] = {0};
    uint8_t u8aPeerPubKeyY[32] = {0};

    memcpy(u32aSelfPrivKey, g_u32aBPrivKey2, sizeof(u32aSelfPrivKey));
    memcpy(u8aPeerPubKeyX, g_u8aAPubKeyX2, sizeof(u8aPeerPubKeyX));
    memcpy(u8aPeerPubKeyY, g_u8aAPubKeyY2, sizeof(u8aPeerPubKeyY));
    #endif

    #if defined(SCRT_AES_CCM_ENC_2) || defined(SCRT_AES_CCM_DEC_2)
    uint8_t u8aScrtAesCcmSk[16] = {0};
    uint8_t u8aScrtAesCcmNonce[13] = {0};
    uint8_t u8aScrtAesCcmAdata[22] = {0};
    uint8_t u8ScrtAesCcmPlainText[286 + 6] = {0};
    uint8_t u8aScrtAesCcmCipher[286 + 6] = {0};
    uint8_t u8aScrtAesCcmTag[8] = {0};

    memcpy(u8aScrtAesCcmSk, g_u8aScrtAesCcmSk, sizeof(u8aScrtAesCcmSk));
    memcpy(u8aScrtAesCcmNonce, g_u8aScrtAesCcmNonce, sizeof(u8aScrtAesCcmNonce));
    memcpy(u8aScrtAesCcmAdata, g_u8aScrtAesCcmAdata, sizeof(u8aScrtAesCcmAdata));
    memcpy(u8ScrtAesCcmPlainText, g_u8ScrtAesCcmPlainText, sizeof(u8ScrtAesCcmPlainText));
    memcpy(u8aScrtAesCcmCipher, g_u8aScrtAesCcmCipher, sizeof(u8aScrtAesCcmCipher));
    memcpy(u8aScrtAesCcmTag, g_u8aScrtAesCcmTag, sizeof(g_u8aScrtAesCcmTag));
    #endif

    #ifdef SCRT_HMAC_SHA1_2
    uint8_t u8aHmacSha1Sk[16] = {0};
    uint8_t u8aHmacSha1Data[20] = {0}; 
    uint8_t u8aHmacSha1Mac[20] = {0};
    uint32_t u32HmacSha1SkLen = 8;
    uint32_t u32HmacSha1DataLen = 20;

    memcpy(u8aHmacSha1Sk, g_u8aHmacSha1Sk, sizeof(u8aHmacSha1Sk));
    memcpy(u8aHmacSha1Data, g_u8aHmacSha1Data, sizeof(u8aHmacSha1Data));
    memcpy(u8aHmacSha1Mac, g_u8aHmacSha1Mac, sizeof(u8aHmacSha1Mac));
    #endif

    #if defined(SCRT_AES_ECB_ENC_2) || defined(SCRT_AES_ECB_DEC_2)
    uint8_t u8aAesEcbSk[16] = {0};
    uint8_t u8aAesEcbInput[16] = {0};
    uint8_t u8aAesEcbOutput[16] = {0};

    memcpy(u8aAesEcbSk, g_u8aAesEcbSk, sizeof(u8aAesEcbSk));
    memcpy(u8aAesEcbInput, g_u8aAesEcbInput, sizeof(u8aAesEcbInput));
    memcpy(u8aAesEcbOutput, g_u8aAesEcbOutput, sizeof(u8aAesEcbOutput));
    #endif

    while(g_bScrtSutTaskRun)
    {
        #ifdef SCRT_KEY_PAIR_2
        uint32_t u32aPubKey[16] = {0};
        uint32_t u32aPrivKey[8] = {0};
        #endif

        #ifdef SCRT_DHKEY_2
        uint8_t u8aDhKeyHw[32] = {0};
        //uint8_t u8aDhKeySw[32] = {0};
        //uint32_t *u32aSelfPrivKey = g_u32aBPrivKey2;
        //uint32_t *u32aPeerPrivKey = g_u32aAPrivKey2;
        //uint8_t *u8aPeerPubKeyX = g_u8aAPubKeyX2;
        //uint8_t *u8aPeerPubKeyY = g_u8aAPubKeyY2;
        #endif

        #if defined(SCRT_AES_CCM_ENC_2) || defined(SCRT_AES_CCM_DEC_2)
        uint32_t u32SkLen = 16;
        uint32_t u32NonceLen = 13;
        uint32_t u32AdataLen = 22;
        uint32_t u32TagLen = 8;
        uint32_t u32DataLen = 286;
        uint8_t u8aBuf[286 + 6] = {0};
        uint8_t u8aTag[8] = {0};
        #endif

        #ifdef SCRT_HMAC_SHA1_2
        uint8_t u8aHmacSha1Output[20] = {0};
        #endif

        #if defined(SCRT_AES_ECB_ENC_2) || defined(SCRT_AES_ECB_DEC_2)
        uint32_t u32AesEcbSkLen = 16;
        uint32_t u32AesEcbDataLen = 16;
        uint8_t u8aAesEcbBuf[64] = {0};
        #endif

    #if 1
        #ifdef SCRT_KEY_PAIR_2
        if(!nl_scrt_ecdh_key_pair_gen(u32aPubKey, u32aPrivKey, &u32PrivKeyId))
        {
            tracer_log(LOG_HIGH_LEVEL, "[%s %d] 2 key pair FAIL\n", __func__, __LINE__);
            g_baScrtError[1] = 1;
            break;
        }
        else
        {
            //tracer_log(LOG_HIGH_LEVEL, "[%s %d] 2 key pair SUCCESS\n", __func__, __LINE__);
        }
        #endif

        #ifdef SCRT_DHKEY_2
        if(!nl_scrt_ecdh_dhkey_gen(u8aPeerPubKeyX, u8aPeerPubKeyY, u32aSelfPrivKey, u8aDhKeyHw, 0))
        {
            tracer_log(LOG_HIGH_LEVEL, "[%s %d] 2 dhkey FAIL\n", __func__, __LINE__);
            g_baScrtError[1] = 2;
            break;
        }
        else
        {
            //tracer_log(LOG_HIGH_LEVEL, "[%s %d] 2 dhkey SUCCESS\n", __func__, __LINE__);
        }
        #endif

        #ifdef SCRT_AES_CCM_ENC_2
        if(!nl_scrt_aes_ccm(1, 
                            u8aScrtAesCcmSk, u32SkLen, 
                            u8aScrtAesCcmNonce, u32NonceLen, 
                            u8aScrtAesCcmAdata, u32AdataLen, 
                            u8ScrtAesCcmPlainText, u8aBuf, u32DataLen, 
                            u8aTag, u32TagLen))
        {
            tracer_log(LOG_HIGH_LEVEL, "[%s %d] 2 aes-ccm enc FAIL\n", __func__, __LINE__);
            g_baScrtError[1] = 3;
            break;
        }
        else
        {
            //tracer_log(LOG_HIGH_LEVEL, "[%s %d] 2 aes-ccm enc SUCCESS\n", __func__, __LINE__);
        }
        #endif

        #ifdef SCRT_AES_CCM_DEC_2
        if(!nl_scrt_aes_ccm(0, 
                            u8aScrtAesCcmSk, u32SkLen, 
                            u8aScrtAesCcmNonce, u32NonceLen, 
                            u8aScrtAesCcmAdata, u32AdataLen, 
                            u8aBuf, u8aScrtAesCcmCipher, u32DataLen, 
                            u8aScrtAesCcmTag, u32TagLen))
        {
            tracer_log(LOG_HIGH_LEVEL, "[%s %d] 2 aes-ccm dec FAIL\n", __func__, __LINE__);
            g_baScrtError[1] = 4;
            break;
        }
        else
        {
            //tracer_log(LOG_HIGH_LEVEL, "[%s %d] 2 aes-ccm dec SUCCESS\n", __func__, __LINE__);
        }
        #endif

        #ifdef SCRT_HMAC_SHA1_2
        if(!nl_hmac_sha_1(u8aHmacSha1Sk, u32HmacSha1SkLen, u8aHmacSha1Data, u32HmacSha1DataLen, u8aHmacSha1Output))
        {
            tracer_log(LOG_HIGH_LEVEL, "[%s %d] 2 hmac-sh1-1 FAIL\n", __func__, __LINE__);
            g_baScrtError[1] = 5;
            break;
        }
        #endif

        #ifdef SCRT_AES_ECB_ENC_2
        if(!nl_scrt_aes_ecb(1, 
                            u8aAesEcbSk, u32AesEcbSkLen, 
                            u8aAesEcbInput, u8aAesEcbBuf, u32AesEcbDataLen))
        {
            tracer_log(LOG_HIGH_LEVEL, "[%s %d] 2 aes-ecb enc FAIL\n", __func__, __LINE__);
            g_baScrtError[1] = 6;
            break;
        }
        #endif

        #ifdef SCRT_AES_ECB_DEC_2
        if(!nl_scrt_aes_ecb(0, 
                            u8aAesEcbSk, u32AesEcbSkLen, 
                            u8aAesEcbOutput, u8aAesEcbBuf, u32AesEcbDataLen))
        {
            tracer_log(LOG_HIGH_LEVEL, "[%s %d] 2 aes-ecb dec FAIL\n", __func__, __LINE__);
            g_baScrtError[1] = 7;
            break;
        }
        #endif

        #ifdef SCRT_DHKEY_2
        //osDelay(1);
        #endif

        g_baScrtCnt[1] += 1;
    #else
        if(!nl_scrt_ecdh_key_pair_gen(u32aPubKey, u32aPrivKey, &u32PrivKeyId))
        {
            tracer_log(LOG_HIGH_LEVEL, "[%s %d] 2 key pair FAIL\n", __func__, __LINE__);
            //goto done;
        }
        else
        {
            tracer_log(LOG_HIGH_LEVEL, "[%s %d] 2 key pair SUCCESS\n", __func__, __LINE__);

            osDelay(SCRT_SUT_TASK_DELAY);

            if(!nl_scrt_ecdh_dhkey_gen(u8aPeerPubKeyX, u8aPeerPubKeyY, NULL, u8aDhKeyHw, u32PrivKeyId))
            {
                tracer_log(LOG_HIGH_LEVEL, "[%s %d] 2 dhkey FAIL\n", __func__, __LINE__);
            }
            else
            {
                tracer_log(LOG_HIGH_LEVEL, "[%s %d] 2 dhkey SUCCESS\n", __func__, __LINE__);

                if(LeSmpUtilGenDhkey((uint8_t *)&u32aPubKey[0], (uint8_t *)&u32aPubKey[8], u32aPeerPrivKey, u8aDhKeySw))
                {
                    tracer_log(LOG_HIGH_LEVEL, "[%s %d] 2 LeSmpUtilGenDhkey fail\n", __func__, __LINE__);
                }
                else
                {
                    if(memcmp(u8aDhKeyHw, u8aDhKeySw, sizeof(u8aDhKeyHw)))
                    {
                        tracer_log(LOG_HIGH_LEVEL, "[%s %d] 2 dhkey compare FAIL\n", __func__, __LINE__);
                    }
                    else
                    {
                        tracer_log(LOG_HIGH_LEVEL, "[%s %d] 2 dhkey compare SUCCESS\n", __func__, __LINE__);
                    }
                }
            }
        }

        osDelay(SCRT_SUT_TASK_DELAY * 8);
    #endif
    }

    vTaskDelete(NULL);
    return;
}

void scrt_sut_task_main_3_impl(void *pParam)
{
    #ifdef SCRT_KEY_PAIR_3
    uint32_t u32PrivKeyId = 0;
    #endif

    #ifdef SCRT_DHKEY_3
    uint32_t u32aSelfPrivKey[8] = {0};
    uint8_t u8aPeerPubKeyX[32] = {0};
    uint8_t u8aPeerPubKeyY[32] = {0};

    memcpy(u32aSelfPrivKey, g_u32aAPrivKey1, sizeof(u32aSelfPrivKey));
    memcpy(u8aPeerPubKeyX, g_u8aBPubKeyX1, sizeof(u8aPeerPubKeyX));
    memcpy(u8aPeerPubKeyY, g_u8aBPubKeyY1, sizeof(u8aPeerPubKeyY));
    #endif

    #if defined(SCRT_AES_CCM_ENC_3) || defined(SCRT_AES_CCM_DEC_3)
    uint8_t u8aScrtAesCcmSk[16] = {0};
    uint8_t u8aScrtAesCcmNonce[13] = {0};
    uint8_t u8aScrtAesCcmAdata[22] = {0};
    uint8_t u8ScrtAesCcmPlainText[286 + 6] = {0};
    uint8_t u8aScrtAesCcmCipher[286 + 6] = {0};
    uint8_t u8aScrtAesCcmTag[8] = {0};

    memcpy(u8aScrtAesCcmSk, g_u8aScrtAesCcmSk, sizeof(u8aScrtAesCcmSk));
    memcpy(u8aScrtAesCcmNonce, g_u8aScrtAesCcmNonce, sizeof(u8aScrtAesCcmNonce));
    memcpy(u8aScrtAesCcmAdata, g_u8aScrtAesCcmAdata, sizeof(u8aScrtAesCcmAdata));
    memcpy(u8ScrtAesCcmPlainText, g_u8ScrtAesCcmPlainText, sizeof(u8ScrtAesCcmPlainText));
    memcpy(u8aScrtAesCcmCipher, g_u8aScrtAesCcmCipher, sizeof(u8aScrtAesCcmCipher));
    memcpy(u8aScrtAesCcmTag, g_u8aScrtAesCcmTag, sizeof(g_u8aScrtAesCcmTag));
    #endif

    #ifdef SCRT_HMAC_SHA1_3
    uint8_t u8aHmacSha1Sk[16] = {0};
    uint8_t u8aHmacSha1Data[20] = {0}; 
    uint8_t u8aHmacSha1Mac[20] = {0};
    uint32_t u32HmacSha1SkLen = 8;
    uint32_t u32HmacSha1DataLen = 20;

    memcpy(u8aHmacSha1Sk, g_u8aHmacSha1Sk, sizeof(u8aHmacSha1Sk));
    memcpy(u8aHmacSha1Data, g_u8aHmacSha1Data, sizeof(u8aHmacSha1Data));
    memcpy(u8aHmacSha1Mac, g_u8aHmacSha1Mac, sizeof(u8aHmacSha1Mac));
    #endif

    #if defined(SCRT_AES_ECB_ENC_3) || defined(SCRT_AES_ECB_DEC_3)
    uint8_t u8aAesEcbSk[16] = {0};
    uint8_t u8aAesEcbInput[16] = {0};
    uint8_t u8aAesEcbOutput[16] = {0};

    memcpy(u8aAesEcbSk, g_u8aAesEcbSk, sizeof(u8aAesEcbSk));
    memcpy(u8aAesEcbInput, g_u8aAesEcbInput, sizeof(u8aAesEcbInput));
    memcpy(u8aAesEcbOutput, g_u8aAesEcbOutput, sizeof(u8aAesEcbOutput));
    #endif

    while(g_bScrtSutTaskRun)
    {
        #ifdef SCRT_KEY_PAIR_3
        uint32_t u32aPubKey[16] = {0};
        uint32_t u32aPrivKey[8] = {0};
        #endif

        #ifdef SCRT_DHKEY_3
        uint8_t u8aDhKeyHw[32] = {0};
        //uint8_t u8aDhKeySw[32] = {0};
        //uint32_t *u32aSelfPrivKey = g_u32aAPrivKey1;
        //uint32_t *u32aPeerPrivKey = g_u32aBPrivKey1;
        //uint8_t *u8aPeerPubKeyX = g_u8aBPubKeyX1;
        //uint8_t *u8aPeerPubKeyY = g_u8aBPubKeyY1;
        #endif

        #if defined(SCRT_AES_CCM_ENC_3) || defined(SCRT_AES_CCM_DEC_3)
        uint32_t u32SkLen = 16;
        uint32_t u32NonceLen = 13;
        uint32_t u32AdataLen = 22;
        uint32_t u32TagLen = 8;
        uint32_t u32DataLen = 286;
        uint8_t u8aBuf[286 + 6] = {0};
        uint8_t u8aTag[8] = {0};
        #endif

        #ifdef SCRT_HMAC_SHA1_3
        uint8_t u8aHmacSha1Output[20] = {0};
        #endif

        #if defined(SCRT_AES_ECB_ENC_3) || defined(SCRT_AES_ECB_DEC_3)
        uint32_t u32AesEcbSkLen = 16;
        uint32_t u32AesEcbDataLen = 16;
        uint8_t u8aAesEcbBuf[64] = {0};
        #endif

    #if 1
        #ifdef SCRT_KEY_PAIR_3
        if(!nl_scrt_ecdh_key_pair_gen(u32aPubKey, u32aPrivKey, &u32PrivKeyId))
        {
            tracer_log(LOG_HIGH_LEVEL, "[%s %d] 3 key pair FAIL\n", __func__, __LINE__);
            g_baScrtError[2] = 1;
            break;
        }
        else
        {
            //tracer_log(LOG_HIGH_LEVEL, "[%s %d] 3 key pair SUCCESS\n", __func__, __LINE__);
        }
        #endif

        #ifdef SCRT_DHKEY_3
        if(!nl_scrt_ecdh_dhkey_gen(u8aPeerPubKeyX, u8aPeerPubKeyY, u32aSelfPrivKey, u8aDhKeyHw, 0))
        {
            tracer_log(LOG_HIGH_LEVEL, "[%s %d] 3 dhkey FAIL\n", __func__, __LINE__);
            g_baScrtError[2] = 2;
            break;
        }
        else
        {
            //tracer_log(LOG_HIGH_LEVEL, "[%s %d] 3 dhkey SUCCESS\n", __func__, __LINE__);
        }
        #endif

        #ifdef SCRT_AES_CCM_ENC_3
        if(!nl_scrt_aes_ccm(1, 
                            u8aScrtAesCcmSk, u32SkLen, 
                            u8aScrtAesCcmNonce, u32NonceLen, 
                            u8aScrtAesCcmAdata, u32AdataLen, 
                            u8ScrtAesCcmPlainText, u8aBuf, u32DataLen, 
                            u8aTag, u32TagLen))
        {
            tracer_log(LOG_HIGH_LEVEL, "[%s %d] 3 aes-ccm enc FAIL\n", __func__, __LINE__);
            g_baScrtError[2] = 3;
            break;
        }
        else
        {
            //tracer_log(LOG_HIGH_LEVEL, "[%s %d] 3 aes-ccm enc SUCCESS\n", __func__, __LINE__);
        }
        #endif

        #ifdef SCRT_AES_CCM_DEC_3
        if(!nl_scrt_aes_ccm(0, 
                            u8aScrtAesCcmSk, u32SkLen, 
                            u8aScrtAesCcmNonce, u32NonceLen, 
                            u8aScrtAesCcmAdata, u32AdataLen, 
                            u8aBuf, u8aScrtAesCcmCipher, u32DataLen, 
                            u8aScrtAesCcmTag, u32TagLen))
        {
            tracer_log(LOG_HIGH_LEVEL, "[%s %d] 3 aes-ccm dec FAIL\n", __func__, __LINE__);
            g_baScrtError[2] = 4;
            break;
        }
        else
        {
            //tracer_log(LOG_HIGH_LEVEL, "[%s %d] 3 aes-ccm dec SUCCESS\n", __func__, __LINE__);
        }
        #endif

        #ifdef SCRT_HMAC_SHA1_3
        if(!nl_hmac_sha_1(u8aHmacSha1Sk, u32HmacSha1SkLen, u8aHmacSha1Data, u32HmacSha1DataLen, u8aHmacSha1Output))
        {
            tracer_log(LOG_HIGH_LEVEL, "[%s %d] 3 hmac-sh1-1 FAIL\n", __func__, __LINE__);
            g_baScrtError[2] = 5;
            break;
        }
        #endif

        #ifdef SCRT_AES_ECB_ENC_3
        if(!nl_scrt_aes_ecb(1, 
                            u8aAesEcbSk, u32AesEcbSkLen, 
                            u8aAesEcbInput, u8aAesEcbBuf, u32AesEcbDataLen))
        {
            tracer_log(LOG_HIGH_LEVEL, "[%s %d] 3 aes-ecb enc FAIL\n", __func__, __LINE__);
            g_baScrtError[2] = 6;
            break;
        }
        #endif

        #ifdef SCRT_AES_ECB_DEC_3
        if(!nl_scrt_aes_ecb(0, 
                            u8aAesEcbSk, u32AesEcbSkLen, 
                            u8aAesEcbOutput, u8aAesEcbBuf, u32AesEcbDataLen))
        {
            tracer_log(LOG_HIGH_LEVEL, "[%s %d] 3 aes-ecb dec FAIL\n", __func__, __LINE__);
            g_baScrtError[2] = 7;
            break;
        }
        #endif

        #ifdef SCRT_DHKEY_3
        //osDelay(1);
        #endif

        g_baScrtCnt[2] += 1;
    #else
        if(!nl_scrt_ecdh_key_pair_gen(u32aPubKey, u32aPrivKey, &u32PrivKeyId))
        {
            tracer_log(LOG_HIGH_LEVEL, "[%s %d] 3 key pair FAIL\n", __func__, __LINE__);
            //goto done;
        }
        else
        {
            tracer_log(LOG_HIGH_LEVEL, "[%s %d] 3 key pair SUCCESS\n", __func__, __LINE__);

            osDelay(SCRT_SUT_TASK_DELAY);

            if(!nl_scrt_ecdh_dhkey_gen(u8aPeerPubKeyX, u8aPeerPubKeyY, NULL, u8aDhKeyHw, u32PrivKeyId))
            {
                tracer_log(LOG_HIGH_LEVEL, "[%s %d] 3 dhkey FAIL\n", __func__, __LINE__);
            }
            else
            {
                tracer_log(LOG_HIGH_LEVEL, "[%s %d] 3 dhkey SUCCESS\n", __func__, __LINE__);

                if(LeSmpUtilGenDhkey((uint8_t *)&u32aPubKey[0], (uint8_t *)&u32aPubKey[8], u32aPeerPrivKey, u8aDhKeySw))
                {
                    tracer_log(LOG_HIGH_LEVEL, "[%s %d] 3 LeSmpUtilGenDhkey fail\n", __func__, __LINE__);
                }
                else
                {
                    if(memcmp(u8aDhKeyHw, u8aDhKeySw, sizeof(u8aDhKeyHw)))
                    {
                        tracer_log(LOG_HIGH_LEVEL, "[%s %d] 3 dhkey compare FAIL\n", __func__, __LINE__);
                    }
                    else
                    {
                        tracer_log(LOG_HIGH_LEVEL, "[%s %d] 3 dhkey compare SUCCESS\n", __func__, __LINE__);
                    }
                }
            }
        }

        osDelay(SCRT_SUT_TASK_DELAY * 6);
    #endif
    }

    vTaskDelete(NULL);
    return;
}

void scrt_sut_task_main_4_impl(void *pParam)
{
    #ifdef SCRT_KEY_PAIR_4
    uint32_t u32PrivKeyId = 0;
    #endif

    #ifdef SCRT_DHKEY_4
    uint32_t u32aSelfPrivKey[8] = {0};
    uint8_t u8aPeerPubKeyX[32] = {0};
    uint8_t u8aPeerPubKeyY[32] = {0};

    memcpy(u32aSelfPrivKey, g_u32aAPrivKey2, sizeof(u32aSelfPrivKey));
    memcpy(u8aPeerPubKeyX, g_u8aBPubKeyX2, sizeof(u8aPeerPubKeyX));
    memcpy(u8aPeerPubKeyY, g_u8aBPubKeyY2, sizeof(u8aPeerPubKeyY));
    #endif

    #if defined(SCRT_AES_CCM_ENC_4) || defined(SCRT_AES_CCM_DEC_4)
    uint8_t u8aScrtAesCcmSk[16] = {0};
    uint8_t u8aScrtAesCcmNonce[13] = {0};
    uint8_t u8aScrtAesCcmAdata[22] = {0};
    uint8_t u8ScrtAesCcmPlainText[286 + 6] = {0};
    uint8_t u8aScrtAesCcmCipher[286 + 6] = {0};
    uint8_t u8aScrtAesCcmTag[8] = {0};

    memcpy(u8aScrtAesCcmSk, g_u8aScrtAesCcmSk, sizeof(u8aScrtAesCcmSk));
    memcpy(u8aScrtAesCcmNonce, g_u8aScrtAesCcmNonce, sizeof(u8aScrtAesCcmNonce));
    memcpy(u8aScrtAesCcmAdata, g_u8aScrtAesCcmAdata, sizeof(u8aScrtAesCcmAdata));
    memcpy(u8ScrtAesCcmPlainText, g_u8ScrtAesCcmPlainText, sizeof(u8ScrtAesCcmPlainText));
    memcpy(u8aScrtAesCcmCipher, g_u8aScrtAesCcmCipher, sizeof(u8aScrtAesCcmCipher));
    memcpy(u8aScrtAesCcmTag, g_u8aScrtAesCcmTag, sizeof(g_u8aScrtAesCcmTag));
    #endif

    #ifdef SCRT_HMAC_SHA1_4
    uint8_t u8aHmacSha1Sk[16] = {0};
    uint8_t u8aHmacSha1Data[20] = {0}; 
    uint8_t u8aHmacSha1Mac[20] = {0};
    uint32_t u32HmacSha1SkLen = 8;
    uint32_t u32HmacSha1DataLen = 20;

    memcpy(u8aHmacSha1Sk, g_u8aHmacSha1Sk, sizeof(u8aHmacSha1Sk));
    memcpy(u8aHmacSha1Data, g_u8aHmacSha1Data, sizeof(u8aHmacSha1Data));
    memcpy(u8aHmacSha1Mac, g_u8aHmacSha1Mac, sizeof(u8aHmacSha1Mac));
    #endif

    #if defined(SCRT_AES_ECB_ENC_4) || defined(SCRT_AES_ECB_DEC_4)
    uint8_t u8aAesEcbSk[16] = {0};
    uint8_t u8aAesEcbInput[16] = {0};
    uint8_t u8aAesEcbOutput[16] = {0};

    memcpy(u8aAesEcbSk, g_u8aAesEcbSk, sizeof(u8aAesEcbSk));
    memcpy(u8aAesEcbInput, g_u8aAesEcbInput, sizeof(u8aAesEcbInput));
    memcpy(u8aAesEcbOutput, g_u8aAesEcbOutput, sizeof(u8aAesEcbOutput));
    #endif

    while(g_bScrtSutTaskRun)
    {
        #ifdef SCRT_KEY_PAIR_4
        uint32_t u32aPubKey[16] = {0};
        uint32_t u32aPrivKey[8] = {0};
        #endif

        #ifdef SCRT_DHKEY_4
        uint8_t u8aDhKeyHw[32] = {0};
        //uint8_t u8aDhKeySw[32] = {0};
        //uint32_t *u32aSelfPrivKey = g_u32aAPrivKey2;
        //uint32_t *u32aPeerPrivKey = g_u32aBPrivKey2;
        //uint8_t *u8aPeerPubKeyX = g_u8aBPubKeyX2;
        //uint8_t *u8aPeerPubKeyY = g_u8aBPubKeyY2;
        #endif

        #if defined(SCRT_AES_CCM_ENC_4) || defined(SCRT_AES_CCM_DEC_4)
        uint32_t u32SkLen = 16;
        uint32_t u32NonceLen = 13;
        uint32_t u32AdataLen = 22;
        uint32_t u32TagLen = 8;
        uint32_t u32DataLen = 286;
        uint8_t u8aBuf[286 + 6] = {0};
        uint8_t u8aTag[8] = {0};
        #endif

        #ifdef SCRT_HMAC_SHA1_4
        uint8_t u8aHmacSha1Output[20] = {0};
        #endif

        #if defined(SCRT_AES_ECB_ENC_4) || defined(SCRT_AES_ECB_DEC_4)
        uint32_t u32AesEcbSkLen = 16;
        uint32_t u32AesEcbDataLen = 16;
        uint8_t u8aAesEcbBuf[64] = {0};
        #endif

    #if 1
        #ifdef SCRT_KEY_PAIR_4
        if(!nl_scrt_ecdh_key_pair_gen(u32aPubKey, u32aPrivKey, &u32PrivKeyId))
        {
            tracer_log(LOG_HIGH_LEVEL, "[%s %d] 4 key pair FAIL\n", __func__, __LINE__);
            g_baScrtError[3] = 1;
            break;
        }
        else
        {
            //tracer_log(LOG_HIGH_LEVEL, "[%s %d] 4 key pair SUCCESS\n", __func__, __LINE__);
        }
        #endif

        #ifdef SCRT_DHKEY_4
        if(!nl_scrt_ecdh_dhkey_gen(u8aPeerPubKeyX, u8aPeerPubKeyY, u32aSelfPrivKey, u8aDhKeyHw, 0))
        {
            tracer_log(LOG_HIGH_LEVEL, "[%s %d] 4 dhkey FAIL\n", __func__, __LINE__);
            g_baScrtError[3] = 2;
            break;
        }
        else
        {
            //tracer_log(LOG_HIGH_LEVEL, "[%s %d] 4 dhkey SUCCESS\n", __func__, __LINE__);
        }
        #endif

        #ifdef SCRT_AES_CCM_ENC_4
        if(!nl_scrt_aes_ccm(1, 
                            u8aScrtAesCcmSk, u32SkLen, 
                            u8aScrtAesCcmNonce, u32NonceLen, 
                            u8aScrtAesCcmAdata, u32AdataLen, 
                            u8ScrtAesCcmPlainText, u8aBuf, u32DataLen, 
                            u8aTag, u32TagLen))
        {
            tracer_log(LOG_HIGH_LEVEL, "[%s %d] 4 aes-ccm enc FAIL\n", __func__, __LINE__);
            g_baScrtError[3] = 3;
            break;
        }
        else
        {
            //tracer_log(LOG_HIGH_LEVEL, "[%s %d] 4 aes-ccm enc SUCCESS\n", __func__, __LINE__);
        }
        #endif

        #ifdef SCRT_AES_CCM_DEC_4
        if(!nl_scrt_aes_ccm(0, 
                            u8aScrtAesCcmSk, u32SkLen, 
                            u8aScrtAesCcmNonce, u32NonceLen, 
                            u8aScrtAesCcmAdata, u32AdataLen, 
                            u8aBuf, u8aScrtAesCcmCipher, u32DataLen, 
                            u8aScrtAesCcmTag, u32TagLen))
        {
            tracer_log(LOG_HIGH_LEVEL, "[%s %d] 4 aes-ccm dec FAIL\n", __func__, __LINE__);
            g_baScrtError[3] = 4;
            break;
        }
        else
        {
            //tracer_log(LOG_HIGH_LEVEL, "[%s %d] 4 aes-ccm dec SUCCESS\n", __func__, __LINE__);
        }
        #endif

        #ifdef SCRT_HMAC_SHA1_4
        if(!nl_hmac_sha_1(u8aHmacSha1Sk, u32HmacSha1SkLen, u8aHmacSha1Data, u32HmacSha1DataLen, u8aHmacSha1Output))
        {
            tracer_log(LOG_HIGH_LEVEL, "[%s %d] 4 hmac-sh1-1 FAIL\n", __func__, __LINE__);
            g_baScrtError[3] = 5;
            break;
        }
        #endif

        #ifdef SCRT_AES_ECB_ENC_4
        if(!nl_scrt_aes_ecb(1, 
                            u8aAesEcbSk, u32AesEcbSkLen, 
                            u8aAesEcbInput, u8aAesEcbBuf, u32AesEcbDataLen))
        {
            tracer_log(LOG_HIGH_LEVEL, "[%s %d] 4 aes-ecb enc FAIL\n", __func__, __LINE__);
            g_baScrtError[3] = 6;
            break;
        }
        #endif

        #ifdef SCRT_AES_ECB_DEC_4
        if(!nl_scrt_aes_ecb(0, 
                            u8aAesEcbSk, u32AesEcbSkLen, 
                            u8aAesEcbOutput, u8aAesEcbBuf, u32AesEcbDataLen))
        {
            tracer_log(LOG_HIGH_LEVEL, "[%s %d] 4 aes-ecb dec FAIL\n", __func__, __LINE__);
            g_baScrtError[3] = 7;
            break;
        }
        #endif

        #ifdef SCRT_DHKEY_4
        //osDelay(1);
        #endif

        g_baScrtCnt[3] += 1;
    #else
        if(!nl_scrt_ecdh_key_pair_gen(u32aPubKey, u32aPrivKey, &u32PrivKeyId))
        {
            tracer_log(LOG_HIGH_LEVEL, "[%s %d] 4 key pair FAIL\n", __func__, __LINE__);
            //goto done;
        }
        else
        {
            tracer_log(LOG_HIGH_LEVEL, "[%s %d] 4 key pair SUCCESS\n", __func__, __LINE__);

            osDelay(SCRT_SUT_TASK_DELAY);

            if(!nl_scrt_ecdh_dhkey_gen(u8aPeerPubKeyX, u8aPeerPubKeyY, NULL, u8aDhKeyHw, u32PrivKeyId))
            {
                tracer_log(LOG_HIGH_LEVEL, "[%s %d] 4 dhkey FAIL\n", __func__, __LINE__);
            }
            else
            {
                tracer_log(LOG_HIGH_LEVEL, "[%s %d] 4 dhkey SUCCESS\n", __func__, __LINE__);

                if(LeSmpUtilGenDhkey((uint8_t *)&u32aPubKey[0], (uint8_t *)&u32aPubKey[8], u32aPeerPrivKey, u8aDhKeySw))
                {
                    tracer_log(LOG_HIGH_LEVEL, "[%s %d] 4 LeSmpUtilGenDhkey fail\n", __func__, __LINE__);
                }
                else
                {
                    if(memcmp(u8aDhKeyHw, u8aDhKeySw, sizeof(u8aDhKeyHw)))
                    {
                        tracer_log(LOG_HIGH_LEVEL, "[%s %d] 4 dhkey compare FAIL\n", __func__, __LINE__);
                    }
                    else
                    {
                        tracer_log(LOG_HIGH_LEVEL, "[%s %d] 4 dhkey compare SUCCESS\n", __func__, __LINE__);
                    }
                }
            }
        }

        osDelay(SCRT_SUT_TASK_DELAY);
    #endif
    }

    vTaskDelete(NULL);
    return;
}

os_pthread g_fpaScrtSutTaskMain[SCRT_SUT_TASK_NUM] = {0};

osPriority g_tScrtSutTaskPri[SCRT_SUT_TASK_NUM] = 
{
    #if 0
    osPriorityLow,
    osPriorityNormal,
    osPriorityBelowNormal,
    osPriorityLow
    #else

    osPriorityLow,
    osPriorityLow,
    osPriorityLow,
    osPriorityLow,

    /*
    osPriorityNormal,
    osPriorityNormal,
    osPriorityNormal,
    osPriorityNormal
    */
    #endif
};

void scrt_sut_task_delete_impl(void)
{
    uint8_t i = 0;

    for(i = 0; i < SCRT_SUT_TASK_NUM; i++)
    {
        if(g_taScrtSutThreadId[i])
        {
            osThreadTerminate(g_taScrtSutThreadId[i]);
            g_taScrtSutThreadId[i] = 0;
        }
    }
}

int scrt_sut_task_create_impl(uint8_t bValue)
{
    int iRet = -1;
    uint8_t i = 0;
    osThreadDef_t tThreadDef = {0};
    uint32_t dwValue = bValue;

    if(g_bScrtSutTaskRun)
    {
        tracer_cli(LOG_LOW_LEVEL, "[%s %d] terminate sut tasks\n", __func__, __LINE__);
        goto done;
    }

    memset(g_baScrtCnt, 0, sizeof(g_baScrtCnt));
    memset(g_baScrtError, 0, sizeof(g_baScrtError));

    g_fpaScrtSutTaskMain[0] = scrt_sut_task_main_1;
    g_fpaScrtSutTaskMain[1] = scrt_sut_task_main_2;
    g_fpaScrtSutTaskMain[2] = scrt_sut_task_main_3;
    g_fpaScrtSutTaskMain[3] = scrt_sut_task_main_4;

    g_bScrtSutTaskRun = 1;

    for(i = 0; i < SCRT_SUT_TASK_NUM; i++)
    {
        //create task
        tThreadDef.name = (char *)g_saScrtSutTaskName[i];
        tThreadDef.stacksize = SCRT_SUT_TASK_STACK_SIZE;
        tThreadDef.tpriority = g_tScrtSutTaskPri[i];
        tThreadDef.pthread = g_fpaScrtSutTaskMain[i];
    
        g_taScrtSutThreadId[i] = osThreadCreate(&tThreadDef, (void *)dwValue);
    
        if(g_taScrtSutThreadId[i] == NULL)
        {
            tracer_cli(LOG_LOW_LEVEL, "[%s %d] osThreadCreate fail\n", __func__, __LINE__);
            goto done;
        }
    }

    iRet = 0;
    
done:
    if(iRet)
    {
        //scrt_sut_task_delete();
        g_bScrtSutTaskRun = 0;
    }

    return iRet;
}
#endif //#ifdef SCRT_CMD_SUT_TASK

void nl_scrt_cmd_impl(char *sCmd)
{
    char *baParam[8] = {0};
    uint32_t dwNum = 8;
    uint32_t dwParamNum = 0;
    uint32_t dwCase = 0;
    uint32_t i = 0;
    uint32_t u32Start = 0;
    uint32_t u32End = 0;

    #ifdef SCRT_CMD_ECDH
    int32_t i32Overflow = 0;
    uint32_t u32HwTime = 0;
    uint32_t u32SwTime = 0;
    uint32_t u32PeerIdx = 0;
    #endif

    extern uint32_t g_u32ScrtWaitResCnt;
    extern uint32_t g_u32ScrtWaitRspCnt;

    (void)i;

    dwParamNum = ParseParam(sCmd, baParam, dwNum);
    (void)dwParamNum;

    tracer_cli(LOG_HIGH_LEVEL, "\n\n");

    if(!nl_scrt_otp_status_get())
    {
        tracer_cli(LOG_HIGH_LEVEL, "OTP not ready\n");
        //goto done;
    }
    else
    {
        tracer_cli(LOG_HIGH_LEVEL, "OTP ready\n");
    }

    if(dwParamNum > 1)
    {
        dwCase = strtoul((char *)baParam[1], NULL, 10);
    }

    if(dwCase == 0)
    {
        #ifdef SCRT_CMD_SUT_TASK
        tracer_cli(LOG_HIGH_LEVEL, "count [%u] [%u] [%u] [%u]\n", 
                   g_baScrtCnt[0], g_baScrtCnt[1], g_baScrtCnt[2], g_baScrtCnt[3]);
        tracer_cli(LOG_HIGH_LEVEL, "error [%u] [%u] [%u] [%u]\n", 
                   g_baScrtError[0], g_baScrtError[1], g_baScrtError[2], g_baScrtError[3]);
        #endif //#ifdef SCRT_CMD_SUT_TASK
    }
    #ifdef SCRT_CMD_ECDH
    else if(dwCase == 1) // load private key
    {
        uint8_t u8aDhKey1[32] = {0};
        uint8_t u8aDhKey2[32] = {0};
    
        uint32_t u32StartUs = 0;
        uint32_t u32EndUs = 0;
        uint32_t u32HwTimeUs = 0;
        uint32_t u32SwTimeUs = 0;
    
        uint8_t u8aAPubKeyX1[32] = {0};
        uint8_t u8aAPubKeyY1[32] = {0};
        uint32_t u32aBPrivKey1[32] = {0};

        memcpy(u8aAPubKeyX1, g_u8aAPubKeyX1, sizeof(u8aAPubKeyX1));
        memcpy(u8aAPubKeyY1, g_u8aAPubKeyY1, sizeof(u8aAPubKeyY1));
        memcpy(u32aBPrivKey1, g_u32aBPrivKey1, sizeof(u32aBPrivKey1));
    
        osKernelSysTickEx(&u32Start, &i32Overflow);
        u32StartUs = SCRT_CURR_TIME;
    
        if(!nl_scrt_ecdh_dhkey_gen(u8aAPubKeyX1, u8aAPubKeyY1, u32aBPrivKey1, u8aDhKey1, 0))
        {
            tracer_cli(LOG_HIGH_LEVEL, "[%s %d] nl_scrt_dhkey_gen fail\n", __func__, __LINE__);
            goto done;
        }
    
        u32EndUs = SCRT_CURR_TIME;
        osKernelSysTickEx(&u32End, &i32Overflow);

        u32HwTime = u32End - u32Start;
        u32Start = u32End;
    
        u32HwTimeUs = u32EndUs - u32StartUs;

        u32StartUs = SCRT_CURR_TIME;
    
        if(LeSmpUtilGenDhkey(u8aAPubKeyX1, u8aAPubKeyY1, u32aBPrivKey1, u8aDhKey2))
        {
            tracer_cli(LOG_HIGH_LEVEL, "[%s %d] LeSmpUtilGenDhkey fail\n", __func__, __LINE__);
            goto done;
        }
    
        u32EndUs = SCRT_CURR_TIME;
        osKernelSysTickEx(&u32End, &i32Overflow);

        u32SwTime = u32End - u32Start;
        u32SwTimeUs = u32EndUs - u32StartUs;
    
        tracer_cli(LOG_HIGH_LEVEL, "HW proc_time: %u ms, %u us\n", u32HwTime, u32HwTimeUs);
    
        for(i = 0; i < 32; i++)
        {
            if((i % 8) == 0)
            {
                tracer_cli(LOG_HIGH_LEVEL, "\n");
            }
    
            tracer_cli(LOG_HIGH_LEVEL, " [%02X]", u8aDhKey1[i]);
        }
    
        tracer_cli(LOG_HIGH_LEVEL, "\n");
    
        tracer_cli(LOG_HIGH_LEVEL, "\nSW proc_time: %u ms, %u us\n", u32SwTime, u32SwTimeUs);
    
        for(i = 0; i < 32; i++)
        {
            if((i % 8) == 0)
            {
                tracer_cli(LOG_HIGH_LEVEL, "\n");
            }
    
            tracer_cli(LOG_HIGH_LEVEL, " [%02X]", u8aDhKey2[i]);
        }
    
        tracer_cli(LOG_HIGH_LEVEL, "\n\n");
    
        if(memcmp(u8aDhKey1, u8aDhKey2, 32))
        {
            tracer_cli(LOG_HIGH_LEVEL, "[%s %d] FAIL: DHKey not matched\n\n", __func__, __LINE__);
        }
        else
        {
            tracer_cli(LOG_HIGH_LEVEL, "[%s %d] SUCCESS\n\n", __func__, __LINE__);
        }
    }
    else if(dwCase == 2) // generate key pair
    {
        uint32_t u32aHwPrivKey[8] = {0};
        uint32_t u32PrivKeyId = 0;

        if(dwParamNum > 2)
        {
            u32PrivKeyId = strtoul((char *)baParam[2], NULL, 16);
        }

        osKernelSysTickEx(&u32Start, &i32Overflow);

        if(!nl_scrt_ecdh_key_pair_gen(g_u32aHwPubKey, u32aHwPrivKey, &u32PrivKeyId))
        {
            tracer_cli(LOG_HIGH_LEVEL, "[%s %d] nl_scrt_ecdh_key_pair_gen fail\n", __func__, __LINE__);
            //goto done;
        }

        osKernelSysTickEx(&u32End, &i32Overflow);
        u32HwTime = u32End - u32Start;

        osKernelSysTickEx(&u32Start, &i32Overflow);

        if(LeSmpUtilGenKeyPair(g_u32aSwPubKey, g_u32aSwPrivKey))
        {
            tracer_cli(LOG_HIGH_LEVEL, "[%s %d] LeSmpUtilGenKeyPair fail\n", __func__, __LINE__);
            //goto done;
        }

        osKernelSysTickEx(&u32End, &i32Overflow);
        u32SwTime = u32End - u32Start;

        tracer_cli(LOG_HIGH_LEVEL, "HW proc_time: %u ms    u32PrivKeyId[%08X]\n", u32HwTime, u32PrivKeyId);

        for(i = 0; i < 8; i++)
        {
            if((i % 4) == 0)
            {
                tracer_cli(LOG_HIGH_LEVEL, "\n");
            }
    
            tracer_cli(LOG_HIGH_LEVEL, " [%08X]", u32aHwPrivKey[i]);
        }

        tracer_cli(LOG_HIGH_LEVEL, "\n\nPublic Key: ");

        for(i = 0; i < 16; i++)
        {
            if((i % 4) == 0)
            {
                tracer_cli(LOG_HIGH_LEVEL, "\n");
            }
    
            tracer_cli(LOG_HIGH_LEVEL, " [%08X]", g_u32aHwPubKey[i]);
        }

        tracer_cli(LOG_HIGH_LEVEL, "\n\n");

        tracer_cli(LOG_HIGH_LEVEL, "SW proc_time: %u ms\n", u32SwTime);

        for(i = 0; i < 8; i++)
        {
            if((i % 4) == 0)
            {
                tracer_cli(LOG_HIGH_LEVEL, "\n");
            }
    
            tracer_cli(LOG_HIGH_LEVEL, " [%08X]", g_u32aSwPrivKey[i]);
        }

        tracer_cli(LOG_HIGH_LEVEL, "\n\nPublic Key: ");

        for(i = 0; i < 16; i++)
        {
            if((i % 4) == 0)
            {
                tracer_cli(LOG_HIGH_LEVEL, "\n");
            }
    
            tracer_cli(LOG_HIGH_LEVEL, " [%08X]", g_u32aSwPubKey[i]);
        }

        tracer_cli(LOG_HIGH_LEVEL, "\n\n");
    }
    else if(dwCase == 3) // use prevous private key (by AS_ID)
    {
        uint8_t u8aDhKey1[32] = {0};
        uint8_t u8aDhKey2[32] = {0};

        uint8_t *pu8PubKeyX = (uint8_t *)&(g_u32aHwPubKey[0]);
        uint8_t *pu8PubKeyY = (uint8_t *)&(g_u32aHwPubKey[8]);

        uint8_t u8aPeerPubKeyX[32] = {0};
        uint8_t u8aPeerPubKeyY[32] = {0};
        uint32_t u32aPeerPrivKey[8] = {0};

        uint32_t u32PrivKeyId = 0;

        if(dwParamNum > 2)
        {
            u32PrivKeyId = strtoul((char *)baParam[2], NULL, 16);
        }

        if(dwParamNum > 3)
        {
            u32PeerIdx = strtoul((char *)baParam[3], NULL, 10);
    
            tracer_cli(LOG_HIGH_LEVEL, "u32PeerIdx[%u]\n", u32PeerIdx);
        }

        if(u32PeerIdx == 0)
        {
            tracer_cli(LOG_HIGH_LEVEL, "Data1: A\n");

            memcpy(u8aPeerPubKeyX, g_u8aAPubKeyX1, sizeof(u8aPeerPubKeyX));
            memcpy(u8aPeerPubKeyY, g_u8aAPubKeyY1, sizeof(u8aPeerPubKeyY));
            memcpy(u32aPeerPrivKey, g_u32aAPrivKey1, sizeof(u32aPeerPrivKey));
            /*
            u8aPeerPubKeyX = g_u8aAPubKeyX1;
            u8aPeerPubKeyY = g_u8aAPubKeyY1;
            u32aPeerPrivKey = g_u32aAPrivKey1;
            */
        }
        else if(u32PeerIdx == 1)
        {
            tracer_cli(LOG_HIGH_LEVEL, "Data1: B\n");

            memcpy(u8aPeerPubKeyX, g_u8aBPubKeyX1, sizeof(u8aPeerPubKeyX));
            memcpy(u8aPeerPubKeyY, g_u8aBPubKeyY1, sizeof(u8aPeerPubKeyY));
            memcpy(u32aPeerPrivKey, g_u32aBPrivKey1, sizeof(u32aPeerPrivKey));
            /*
            u8aPeerPubKeyX = g_u8aBPubKeyX1;
            u8aPeerPubKeyY = g_u8aBPubKeyY1;
            u32aPeerPrivKey = g_u32aBPrivKey1;
            */
        }
        else if(u32PeerIdx == 2)
        {
            tracer_cli(LOG_HIGH_LEVEL, "Data2: A\n");

            memcpy(u8aPeerPubKeyX, g_u8aAPubKeyX2, sizeof(u8aPeerPubKeyX));
            memcpy(u8aPeerPubKeyY, g_u8aAPubKeyY2, sizeof(u8aPeerPubKeyY));
            memcpy(u32aPeerPrivKey, g_u32aAPrivKey2, sizeof(u32aPeerPrivKey));

            /*
            u8aPeerPubKeyX = g_u8aAPubKeyX2;
            u8aPeerPubKeyY = g_u8aAPubKeyY2;
            u32aPeerPrivKey = g_u32aAPrivKey2;
            */
        }
        else if(u32PeerIdx == 3)
        {
            tracer_cli(LOG_HIGH_LEVEL, "Data2: B\n");

            memcpy(u8aPeerPubKeyX, g_u8aBPubKeyX2, sizeof(u8aPeerPubKeyX));
            memcpy(u8aPeerPubKeyY, g_u8aBPubKeyY2, sizeof(u8aPeerPubKeyY));
            memcpy(u32aPeerPrivKey, g_u32aBPrivKey2, sizeof(u32aPeerPrivKey));

            /*
            u8aPeerPubKeyX = g_u8aBPubKeyX2;
            u8aPeerPubKeyY = g_u8aBPubKeyY2;
            u32aPeerPrivKey = g_u32aBPrivKey2;
            */
        }
        else if(u32PeerIdx == 4)
        {
            tracer_cli(LOG_HIGH_LEVEL, "SW\n");

            memcpy(u8aPeerPubKeyX, &g_u32aSwPubKey[0], sizeof(u8aPeerPubKeyX));
            memcpy(u8aPeerPubKeyY, &g_u32aSwPubKey[8], sizeof(u8aPeerPubKeyY));
            memcpy(u32aPeerPrivKey, g_u32aSwPrivKey, sizeof(u32aPeerPrivKey));

            /*
            u8aPeerPubKeyX = (uint8_t *)&g_u32aSwPubKey[0];
            u8aPeerPubKeyY = (uint8_t *)&g_u32aSwPubKey[8];
            u32aPeerPrivKey = g_u32aSwPrivKey;
            */
        }
        else
        {
            tracer_cli(LOG_HIGH_LEVEL, "unknown\n");
            goto done;
        }
    
        osKernelSysTickEx(&u32Start, &i32Overflow);
    
        if(!nl_scrt_ecdh_dhkey_gen(u8aPeerPubKeyX, u8aPeerPubKeyY, NULL, u8aDhKey1, u32PrivKeyId))
        {
            tracer_cli(LOG_HIGH_LEVEL, "[%s %d] nl_scrt_dhkey_gen fail\n", __func__, __LINE__);
            goto done;
        }
    
        osKernelSysTickEx(&u32End, &i32Overflow);
        u32HwTime = u32End - u32Start;
        u32Start = u32End;
    
        if(LeSmpUtilGenDhkey(pu8PubKeyX, pu8PubKeyY, u32aPeerPrivKey, u8aDhKey2))
        {
            tracer_cli(LOG_HIGH_LEVEL, "[%s %d] LeSmpUtilGenDhkey fail\n", __func__, __LINE__);
            goto done;
        }
    
        osKernelSysTickEx(&u32End, &i32Overflow);
        u32SwTime = u32End - u32Start;
    
        tracer_cli(LOG_HIGH_LEVEL, "HW proc_time: %u ms\n", u32HwTime);
    
        for(i = 0; i < 32; i++)
        {
            if((i % 8) == 0)
            {
                tracer_cli(LOG_HIGH_LEVEL, "\n");
            }
    
            tracer_cli(LOG_HIGH_LEVEL, " [%02X]", u8aDhKey1[i]);
        }
    
        tracer_cli(LOG_HIGH_LEVEL, "\n");
    
        tracer_cli(LOG_HIGH_LEVEL, "\nSW proc_time: %u ms\n", u32SwTime);
    
        for(i = 0; i < 32; i++)
        {
            if((i % 8) == 0)
            {
                tracer_cli(LOG_HIGH_LEVEL, "\n");
            }
    
            tracer_cli(LOG_HIGH_LEVEL, " [%02X]", u8aDhKey2[i]);
        }
    
        tracer_cli(LOG_HIGH_LEVEL, "\n\n");
    
        if(memcmp(u8aDhKey1, u8aDhKey2, 32))
        {
            tracer_cli(LOG_HIGH_LEVEL, "[%s %d] FAIL: DHKey not matched\n\n", __func__, __LINE__);
        }
        else
        {
            tracer_cli(LOG_HIGH_LEVEL, "[%s %d] SUCCESS\n\n", __func__, __LINE__);
        }
    }
    #endif //#ifdef SCRT_CMD_ECDH
    #ifdef SCRT_CMD_SUT_TASK
    else if(dwCase == 4)
    {
        tracer_cli(LOG_HIGH_LEVEL, "Toggle scrt sut task\n");
        scrt_sut_task_create(0);
    }
    #endif //#ifdef SCRT_CMD_SUT_TASK
    #ifdef SCRT_CMD_AES_CCM
    else if(dwCase == 5)
    {
        uint32_t u32SkLen = 16;
        uint32_t u32NonceLen = 13;
        uint32_t u32AdataLen = 22;
        uint32_t u32TagLen = 8;
        uint32_t u32DataLen = 286;
        uint8_t u8aInput[286 + 6] = {0};
        uint8_t u8aBuf[286 + 6] = {0};
        uint8_t u8aTag[8] = {0};
        //uint32_t u32Old = 0;
        uint32_t u32New = 0;

        uint8_t u8aScrtAesCcmSk[16] = {0};
        uint8_t u8aScrtAesCcmNonce[13] = {0};
        uint8_t u8aScrtAesCcmAdata[22] = {0};
        uint8_t u8aScrtAesCcmTag[8] = {0};
    
        memcpy(u8aScrtAesCcmSk, g_u8aScrtAesCcmSk, sizeof(u8aScrtAesCcmSk));
        memcpy(u8aScrtAesCcmNonce, g_u8aScrtAesCcmNonce, sizeof(u8aScrtAesCcmNonce));
        memcpy(u8aScrtAesCcmAdata, g_u8aScrtAesCcmAdata, sizeof(u8aScrtAesCcmAdata));
        memcpy(u8aScrtAesCcmTag, g_u8aScrtAesCcmTag, sizeof(g_u8aScrtAesCcmTag));

        #if 0 // Todo: disable scrt clock
        {
            uint32_t *pu32Reg = (uint32_t *)0x40001134;
            uint32_t u32Value = *pu32Reg;
    
            tracer_cli(LOG_HIGH_LEVEL, "before [%08X]\n", *pu32Reg);
            *pu32Reg = u32Value & (~(0x01000000));
            tracer_cli(LOG_HIGH_LEVEL, "after  [%08X]\n", *pu32Reg);
        }
        #endif

        memset(u8aTag, 0, sizeof(u8aTag));
        memset(u8aBuf, 0xEE, sizeof(u8aBuf));

        memcpy(u8aInput, g_u8ScrtAesCcmPlainText, sizeof(u8aInput));

        u32Start = SCRT_CURR_TIME;

        if(!nl_scrt_aes_ccm(1, 
                            u8aScrtAesCcmSk, u32SkLen, 
                            u8aScrtAesCcmNonce, u32NonceLen, 
                            u8aScrtAesCcmAdata, u32AdataLen, 
                            u8aInput, u8aBuf, u32DataLen, 
                            u8aTag, u32TagLen))
        {
            tracer_cli(LOG_HIGH_LEVEL, "nl_scrt_aes_ccm fail\n");
            goto done;
        }

        u32End = SCRT_CURR_TIME;
        u32New = u32End - u32Start;
        tracer_cli(LOG_HIGH_LEVEL, "nl_scrt_aes_ccm enc proc_time: %u us\n", u32New);

        if(memcmp(u8aBuf, g_u8aScrtAesCcmCipher, u32DataLen))
        {
            tracer_cli(LOG_HIGH_LEVEL, "data not matched for nl_scrt_aes_ccm enc1\n");
            //goto done;
        }

        if(memcmp(u8aTag, g_u8aScrtAesCcmTag, u32TagLen))
        {
            tracer_cli(LOG_HIGH_LEVEL, "tag not matched for nl_scrt_aes_ccm enc\n");
        }
        
        memset(u8aTag, 0, sizeof(u8aTag));
        memset(u8aBuf, 0xEE, sizeof(u8aBuf));

        memcpy(u8aInput, g_u8aScrtAesCcmCipher, sizeof(u8aInput));

        u32Start = SCRT_CURR_TIME;

        if(!nl_scrt_aes_ccm(0, 
                            u8aScrtAesCcmSk, u32SkLen, 
                            u8aScrtAesCcmNonce, u32NonceLen, 
                            u8aScrtAesCcmAdata, u32AdataLen, 
                            u8aBuf, u8aInput, u32DataLen, 
                            u8aScrtAesCcmTag, u32TagLen))
        {
            tracer_cli(LOG_HIGH_LEVEL, "nl_scrt_aes_ccm fail\n");
            goto done;
        }

        u32End = SCRT_CURR_TIME;
        u32New = u32End - u32Start;
        tracer_cli(LOG_HIGH_LEVEL, "nl_scrt_aes_ccm dec proc_time: %u us\n", u32New);

        if(memcmp(u8aBuf, g_u8ScrtAesCcmPlainText, u32DataLen))
        {
            tracer_cli(LOG_HIGH_LEVEL, "data not matched for nl_scrt_aes_ccm dec\n");
            //goto done;
        }
        
        memset(u8aTag, 0, sizeof(u8aTag));
        memset(u8aBuf, 0xEE, sizeof(u8aBuf));

        memcpy(u8aInput, g_u8ScrtAesCcmPlainText, sizeof(u8aInput));

        #if 0
        u32Start = SCRT_CURR_TIME;
        
        if(!nl_scrt_aes_ccm(1, 
                            u8aScrtAesCcmSk, u32SkLen, 
                            u8aScrtAesCcmNonce, u32NonceLen, 
                            u8aScrtAesCcmAdata, u32AdataLen, 
                            u8aInput, u8aBuf, u32DataLen, 
                            u8aTag, u32TagLen))
        {
            tracer_cli(LOG_HIGH_LEVEL, "nl_scrt_aes_ccm_impl fail\n");
            goto done;
        }

        u32End = SCRT_CURR_TIME;
        u32Old = u32End - u32Start;
        tracer_cli(LOG_HIGH_LEVEL, "nl_scrt_aes_ccm_impl enc proc_time: %u us\n", u32Old);

        if(memcmp(u8aBuf, g_u8aScrtAesCcmCipher, u32DataLen))
        {
            tracer_cli(LOG_HIGH_LEVEL, "data not matched for nl_scrt_aes_ccm_impl enc\n");
            //goto done;
        }

        if(memcmp(u8aTag, g_u8aScrtAesCcmTag, u32TagLen))
        {
            tracer_cli(LOG_HIGH_LEVEL, "tag not matched for nl_scrt_aes_ccm enc\n");
        }
        
        memset(u8aTag, 0, sizeof(u8aTag));
        memset(u8aBuf, 0xEE, sizeof(u8aBuf));

        memcpy(u8aInput, g_u8aScrtAesCcmCipher, sizeof(u8aInput));

        u32Start = SCRT_CURR_TIME;

        if(!nl_scrt_aes_ccm(0, 
                            u8aScrtAesCcmSk, u32SkLen, 
                            u8aScrtAesCcmNonce, u32NonceLen, 
                            u8aScrtAesCcmAdata, u32AdataLen, 
                            u8aBuf, u8aInput, u32DataLen, 
                            u8aScrtAesCcmTag, u32TagLen))
        {
            tracer_cli(LOG_HIGH_LEVEL, "nl_scrt_aes_ccm_impl fail\n");
            goto done;
        }

        u32End = SCRT_CURR_TIME;
        u32Old = u32End - u32Start;
        tracer_cli(LOG_HIGH_LEVEL, "nl_scrt_aes_ccm_impl dec proc_time: %u us\n", u32Old);

        if(memcmp(u8aBuf, g_u8ScrtAesCcmPlainText, u32DataLen))
        {
            tracer_cli(LOG_HIGH_LEVEL, "data not matched for nl_scrt_aes_ccm_impl dec\n");
            //goto done;
        }
        #endif
    }
    #endif //#ifdef SCRT_CMD_AES_CCM
    #ifdef SCRT_CMD_HMAC_SHA_1
    else if(dwCase == 6)
    {
        uint8_t u8aHmacSha1Sk[8] = {0};
        uint8_t u8aHmacSha1Data[20] = {0};
        uint8_t u8aOutput[20] = {0};

        uint32_t u32SkLen = 8;
        uint32_t u32DataLen = 20;
        uint32_t u32Time = 0;

        memcpy(u8aHmacSha1Sk, g_u8aHmacSha1Sk, sizeof(u8aHmacSha1Sk));
        memcpy(u8aHmacSha1Data, g_u8aHmacSha1Data, sizeof(u8aHmacSha1Data));

        memset(u8aOutput, 0, u32DataLen);

        u32Start = SCRT_CURR_TIME;

        if(!nl_hmac_sha_1(u8aHmacSha1Sk, u32SkLen, u8aHmacSha1Data, u32DataLen, u8aOutput))
        {
            tracer_cli(LOG_HIGH_LEVEL, "nl_hmac_sha_1 fail\n");
            goto done;
        }

        u32End = SCRT_CURR_TIME;
        u32Time = u32End - u32Start;
        tracer_cli(LOG_HIGH_LEVEL, "nl_hmac_sha_1 proc_time: %u us\n", u32Time);

        if(memcmp(u8aOutput, g_u8aHmacSha1Mac, u32DataLen))
        {
            tracer_cli(LOG_HIGH_LEVEL, "mac not matched for nl_hmac_sha_1\n");
            //goto done;
        }
    }
    #endif
    #ifdef SCRT_CMD_AES_ECB
    else if(dwCase == 7)
    {
        uint8_t u8aAesEcbSk[16] = {0};
        uint8_t u8aAesEcbInput[16] = {0};
        uint8_t u8aAesEcbOutput[16] = {0};
        uint32_t u32SkLen = 16;
        uint32_t u32DataLen = 16;
        uint8_t u8aBuf[64] = {0};
        uint32_t u32Time = 0;

        memcpy(u8aAesEcbSk, g_u8aAesEcbSk, sizeof(u8aAesEcbSk));
        memcpy(u8aAesEcbInput, g_u8aAesEcbInput, sizeof(u8aAesEcbInput));
        memcpy(u8aAesEcbOutput, g_u8aAesEcbOutput, sizeof(u8aAesEcbOutput));

        memset(u8aBuf, 0xEE, sizeof(u8aBuf));

        u32Start = SCRT_CURR_TIME;
        
        if(!nl_scrt_aes_ecb(1, 
                            u8aAesEcbSk, u32SkLen, 
                            u8aAesEcbInput, u8aBuf, u32DataLen))
        {
            tracer_cli(LOG_HIGH_LEVEL, "nl_scrt_aes_ecb fail\n");
            goto done;
        }

        u32End = SCRT_CURR_TIME;
        u32Time = u32End - u32Start;
        tracer_cli(LOG_HIGH_LEVEL, "nl_scrt_aes_ecb enc proc_time: %u us\n", u32Time);

        if(memcmp(u8aBuf, g_u8aAesEcbOutput, u32DataLen))
        {
            tracer_cli(LOG_HIGH_LEVEL, "output not matched\n");
            goto done;
        }

        memset(u8aBuf, 0xEE, sizeof(u8aBuf));

        u32Start = SCRT_CURR_TIME;
        
        if(!nl_scrt_aes_ecb(0, 
                            u8aAesEcbSk, u32SkLen, 
                            u8aAesEcbOutput, u8aBuf, u32DataLen))
        {
            tracer_cli(LOG_HIGH_LEVEL, "nl_scrt_aes_ecb fail\n");
            goto done;
        }

        u32End = SCRT_CURR_TIME;
        u32Time = u32End - u32Start;
        tracer_cli(LOG_HIGH_LEVEL, "nl_scrt_aes_ecb dec proc_time: %u us\n", u32Time);

        if(memcmp(u8aBuf, g_u8aAesEcbInput, u32DataLen))
        {
            tracer_cli(LOG_HIGH_LEVEL, "output not matched\n");
            goto done;
        }
    }
    #endif
    #ifdef SCRT_CMD_SHA
    else if(dwCase == 8) // SHA-1
    {
        uint8_t u8aOutput[SCRT_SHA_1_OUTPUT_LEN] = {0};
        uint32_t u32BlkSize = SCRT_SHA_1_STEP_SIZE;
        uint32_t u32Total = 64;
        uint32_t u32Offset = 0;
        uint32_t u32Input = 0;
        uint32_t u32DataLen = SCRT_SHA_1_OUTPUT_LEN;
        uint32_t u32Time = 0;
        uint8_t u8IsInterMac = 0;
        //uint8_t u8aInterMac[32] = {0};
        uint8_t u8aScrtShaData[256] = {0};
        uint8_t u8Step = 1;

        if(dwParamNum > 2)
        {
            u32Total = strtoul((char *)baParam[2], NULL, 10);
        }

        if(dwParamNum > 3)
        {
            u8Step = (uint8_t)strtoul((char *)baParam[3], NULL, 10);
        }

        tracer_cli(LOG_HIGH_LEVEL, "u32Total[%u]\n", u32Total);

        memcpy(u8aScrtShaData, g_u8aScrtShaData, u32Total);

        while(1)
        {
            if(u8Step)
            {
                if(u32Offset + u32BlkSize > u32Total)
                {
                    u32Input = u32Total - u32Offset;
                }
                else
                {
                    u32Input = u32BlkSize;
                }
            }
            else
            {
                u32Input = u32Total;
            }
            

            tracer_cli(LOG_HIGH_LEVEL, "step[%u] interM[%u] total[%u] offset[%u] input[%u]\n", u8Step, u8IsInterMac, u32Total, u32Offset, u32Input);

            u32Start = SCRT_CURR_TIME;

            if(!nl_scrt_sha(SCRT_TYPE_SHA_1, u8Step, u32Total, &(u8aScrtShaData[u32Offset]), u32Input, u8IsInterMac, u8aOutput))
            {
                tracer_cli(LOG_HIGH_LEVEL, "nl_scrt_sha_1_step fail\n");
                goto done;
            }

            u32End = SCRT_CURR_TIME;
            u32Time = u32End - u32Start;

            if((!u8Step) || (u32Input < u32BlkSize))
            {
                tracer_cli(LOG_HIGH_LEVEL, "Output:");
            }
            else
            {
                tracer_cli(LOG_HIGH_LEVEL, "InterM:");
            }

            for(i = 0; i < u32DataLen; i++)
            {
                tracer_cli(LOG_HIGH_LEVEL, " %02x", u8aOutput[i]);
            }

            tracer_cli(LOG_HIGH_LEVEL, "\n");

            tracer_cli(LOG_HIGH_LEVEL, "nl_scrt_sha_1_step proc_time: %u us\n", u32Time);

            if(u8Step)
            {
                u8IsInterMac = 1;
            }
            else
            {
                break;
            }

            if(u32Input < u32BlkSize)
            {
                break;
            }

            u32Offset += u32Input;
        }
    }
    /*
    else if(dwCase == 9)
    {
        extern void mbedtls_sha1( const unsigned char *input, size_t ilen, unsigned char output[20] );

        uint8_t u8aOutput[SCRT_SHA_1_OUTPUT_LEN] = {0};
        uint32_t u32DataLen = SCRT_SHA_1_OUTPUT_LEN;
        uint8_t u8aScrtShaData[256] = {0};
        uint32_t u32Total = 64;
        uint32_t u32Time = 0;

        if(dwParamNum > 2)
        {
            u32Total = strtoul((char *)baParam[2], NULL, 10);
        }

        tracer_cli(LOG_HIGH_LEVEL, "u32Total[%u]\n", u32Total);

        memcpy(u8aScrtShaData, g_u8aScrtShaData, u32Total);

        u32Start = SCRT_CURR_TIME;

        mbedtls_sha1(u8aScrtShaData, u32Total, u8aOutput);

        u32End = SCRT_CURR_TIME;
        u32Time = u32End - u32Start;

        tracer_cli(LOG_HIGH_LEVEL, "mbedtls_sha1\nOutput:", u32Total);

        for(i = 0; i < u32DataLen; i++)
        {
            tracer_cli(LOG_HIGH_LEVEL, " %02x", u8aOutput[i]);
        }

        tracer_cli(LOG_HIGH_LEVEL, "\n");

        tracer_cli(LOG_HIGH_LEVEL, "mbedtls_sha1 proc_time: %u us\n", u32Time);
    }
    */
    #endif //#ifdef SCRT_CMD_SHA
    #ifdef SCRT_CMD_AES_CMAC
    else if(dwCase == 20) // AES-CMAC
    {
        uint8_t u8aSk[16] = {0};
        uint8_t u8aSwOutput[16] = {0};
        uint8_t u8aHwOutput[16] = {0};
        uint8_t u8aScrtAesCmacData[128] = {0};
        uint32_t u32BufSize = sizeof(g_u8aScrtAesCmacData);
        uint32_t u32DataLen = u32BufSize;
        uint32_t u32SkLen = sizeof(u8aSk);
        uint32_t u32MacLen = 16;
        uint32_t u32Time = 0;
        //T_LeAesCmacFp fpSwAesCmac = (T_LeAesCmacFp)(0x00023401); //LeSmpUtilAesCmac_Impl

        if(dwParamNum > 2)
        {
            u32BufSize = strtoul((char *)baParam[2], NULL, 10);
            tracer_cli(LOG_HIGH_LEVEL, "BufSize[%u]\n", u32BufSize);
        }

        if(dwParamNum > 3)
        {
            u32DataLen = strtoul((char *)baParam[3], NULL, 10);
            tracer_cli(LOG_HIGH_LEVEL, "DataLen[%u]\n", u32DataLen);
        }

        memcpy(u8aSk, g_u8aScrtAesCmacSk, u32SkLen);
        memcpy(u8aScrtAesCmacData, g_u8aScrtAesCmacData, u32DataLen);

        u32Start = SCRT_CURR_TIME;

        if(LeSmpUtilAesCmac(u8aSk, &(u8aScrtAesCmacData[0]), u32DataLen, u8aSwOutput))
        {
            tracer_cli(LOG_HIGH_LEVEL, "LeSmpUtilAesCmac fail\n");
            goto done;
        }

        u32End = SCRT_CURR_TIME;
        u32Time = u32End - u32Start;
        tracer_cli(LOG_HIGH_LEVEL, "\nLeSmpUtilAesCmac_Impl proc_time: %u us\n\n", u32Time);

        for(i = 0; i < u32MacLen; i++)
        {
            tracer_cli(LOG_HIGH_LEVEL, "%02x", u8aSwOutput[i]);
        }

        tracer_cli(LOG_HIGH_LEVEL, "\n");


        u32Start = SCRT_CURR_TIME;

        if(!nl_scrt_aes_cmac(u8aSk, u32SkLen, &(u8aScrtAesCmacData[0]), u32BufSize, u32DataLen, u8aHwOutput))
        {
            tracer_cli(LOG_HIGH_LEVEL, "nl_scrt_aes_cmac fail\n");
            goto done;
        }

        u32End = SCRT_CURR_TIME;
        u32Time = u32End - u32Start;
        tracer_cli(LOG_HIGH_LEVEL, "\nnl_aes_cmac_patch proc_time: %u us\n\n", u32Time);

        for(i = 0; i < u32MacLen; i++)
        {
            tracer_cli(LOG_HIGH_LEVEL, "%02x", u8aHwOutput[i]);
        }

        tracer_cli(LOG_HIGH_LEVEL, "\n");

        if(memcmp(u8aHwOutput, u8aSwOutput, u32MacLen))
        {
            tracer_cli(LOG_HIGH_LEVEL, "\nFAIL: NOT Match\n\n");
        }
        else
        {
            tracer_cli(LOG_HIGH_LEVEL, "\nSUCCESS\n\n");
        }
    }
    else if(dwCase == 21) // AES-CMAC
    {
        uint8_t u8aSk[16] = {0};
        uint8_t u8aSwOutput[16] = {0};
        uint8_t u8aHwOutput[16] = {0};
        uint8_t u8aScrtAesCmacData[128] = {0};
        uint32_t u32BufSize = sizeof(g_u8aScrtAesCmacData);
        uint32_t u32DataLen = u32BufSize;
        uint32_t u32SkLen = sizeof(u8aSk);
        uint32_t u32MacLen = 16;
        uint32_t u32Time = 0;
        //T_LeAesCmacFp fpSwAesCmac = (T_LeAesCmacFp)(0x00023401); //LeSmpUtilAesCmac_Impl

        if(dwParamNum > 2)
        {
            u32BufSize = strtoul((char *)baParam[2], NULL, 10);
            tracer_cli(LOG_HIGH_LEVEL, "BufSize[%u]\n", u32BufSize);
        }

        if(dwParamNum > 3)
        {
            u32DataLen = strtoul((char *)baParam[3], NULL, 10);
            tracer_cli(LOG_HIGH_LEVEL, "DataLen[%u]\n", u32DataLen);
        }

        memcpy(u8aSk, g_u8aScrtAesCmacSk, u32SkLen);
        memcpy(u8aScrtAesCmacData, g_u8aScrtAesCmacData, u32DataLen);

        for(i = 0; i <= u32DataLen; i++)
        {
            u32Start = SCRT_CURR_TIME;

            if(LeSmpUtilAesCmac(u8aSk, &(u8aScrtAesCmacData[0]), i, u8aSwOutput))
            {
                tracer_cli(LOG_HIGH_LEVEL, "LeSmpUtilAesCmac fail\n");
                goto done;
            }
    
            u32End = SCRT_CURR_TIME;
            u32Time = u32End - u32Start;
            tracer_cli(LOG_HIGH_LEVEL, "Input[%u] SW[%u] ", i, u32Time);
    
            u32Start = SCRT_CURR_TIME;
    
            if(!nl_scrt_aes_cmac(u8aSk, u32SkLen, &(u8aScrtAesCmacData[0]), u32BufSize, i, u8aHwOutput))
            {
                tracer_cli(LOG_HIGH_LEVEL, "nl_scrt_aes_cmac fail\n");
                goto done;
            }
    
            u32End = SCRT_CURR_TIME;
            u32Time = u32End - u32Start;
            tracer_cli(LOG_HIGH_LEVEL, "HW[%u]\n", u32Time);

            if(memcmp(u8aHwOutput, u8aSwOutput, u32MacLen))
            {
                tracer_cli(LOG_HIGH_LEVEL, "\nFAIL: NOT Match\n\n");
                goto done;
            }
        }

        tracer_cli(LOG_HIGH_LEVEL, "\nSUCCESS\n\n");
    }
    #endif //#ifdef SCRT_CMD_AES_CMAC
    #ifdef SCRT_CMD_HMAC_SHA_1_STEP
    else if(dwCase == 22) // HMAC-SHA-1
    {
        uint8_t u8aHmacSha1Sk[8] = {0};
        uint8_t u8aOutput[32] = {0};
        uint8_t u8ScrtHmacSha1Data[256] = {0};
        uint32_t u32Total = sizeof(u8ScrtHmacSha1Data);
        uint32_t u32SkLen = 8;
        uint32_t u32DataLen = 20;
        uint32_t u32Time = 0;

        memcpy(u8aHmacSha1Sk, g_u8aScrtHmacSha1SkStep, sizeof(u8aHmacSha1Sk));
        memcpy(u8ScrtHmacSha1Data, g_u8aScrtSha1Data, u32Total);

        u32Start = SCRT_CURR_TIME;

        if(!nl_hmac_sha_1(u8aHmacSha1Sk, u32SkLen, &(u8ScrtHmacSha1Data[0]), u32Total, u8aOutput))
        {
            tracer_cli(LOG_HIGH_LEVEL, "nl_hmac_sha_1 fail\n");
            goto done;
        }

        u32End = SCRT_CURR_TIME;
        u32Time = u32End - u32Start;
        tracer_cli(LOG_HIGH_LEVEL, "nl_hmac_sha_1 proc_time: %u us\n", u32Time);

        for(i = 0; i < u32DataLen; i++)
        {
            tracer_cli(LOG_HIGH_LEVEL, "%02x", u8aOutput[i]);
        }

        tracer_cli(LOG_HIGH_LEVEL, "\n");
    }
    else if(dwCase == 23) // HMAC-SHA-1
    {
        uint8_t u8aHmacSha1Sk[8] = {0};
        uint8_t u8aOutput[32] = {0};
        uint8_t u8ScrtHmacSha1Data[256] = {0};
        uint32_t u32Total = sizeof(u8ScrtHmacSha1Data);
        uint32_t u32Offset = 0;
        uint32_t u32Input = 64;

        uint32_t u32SkLen = 8;
        uint32_t u32DataLen = 20;
        uint32_t u32Time = 0;

        memcpy(u8aHmacSha1Sk, g_u8aScrtHmacSha1SkStep, sizeof(u8aHmacSha1Sk));
        memcpy(u8ScrtHmacSha1Data, g_u8aScrtSha1Data, u32Total);

        for(u32Offset = 0; u32Offset < u32Total; )
        {
            uint8_t u8Type = 0;

            /*
            if(u32Input == 64)
            {
                u32Input = 128;
            }
            else
            {
                u32Input = 64;
            }
            */

            if(u32Offset == 0)
            {
                u8Type = 0;
            }
            else if(u32Offset + u32Input >= u32Total)
            {
                u8Type = 2;
                u32Input = u32Total - u32Offset;
            }
            else
            {
                u8Type = 1;
            }

            tracer_cli(LOG_HIGH_LEVEL, "type[%d]\n", u8Type);

            u32Start = SCRT_CURR_TIME;

            if(!nl_scrt_hmac_sha_1_step(u8Type, u32Total, u8aHmacSha1Sk, u32SkLen, &(u8ScrtHmacSha1Data[u32Offset]), u32Input, u8aOutput))
            {
                tracer_cli(LOG_HIGH_LEVEL, "nl_hmac_sha_1 fail\n");
                goto done;
            }

            u32End = SCRT_CURR_TIME;
            u32Time = u32End - u32Start;
            tracer_cli(LOG_HIGH_LEVEL, "nl_hmac_sha_1_step proc_time: %u us\n", u32Time);

            u32Offset += u32Input;
        }
        
        for(i = 0; i < u32DataLen; i++)
        {
            tracer_cli(LOG_HIGH_LEVEL, "%02x", u8aOutput[i]);
        }

        tracer_cli(LOG_HIGH_LEVEL, "\n");
        
    }
    #endif //#ifdef SCRT_CMD_HMAC_SHA_1_STEP
    else
    {
        #ifdef SCRT_CMD_SUT_TASK
        tracer_cli(LOG_HIGH_LEVEL, "count [%u] [%u] [%u] [%u]\n", 
                   g_baScrtCnt[0], g_baScrtCnt[1], g_baScrtCnt[2], g_baScrtCnt[3]);
        tracer_cli(LOG_HIGH_LEVEL, "error [%u] [%u] [%u] [%u]\n", 
                   g_baScrtError[0], g_baScrtError[1], g_baScrtError[2], g_baScrtError[3]);
        #endif //#ifdef SCRT_CMD_SUT_TASK
    }

    tracer_cli(LOG_HIGH_LEVEL, "WaitResCnt[%u] WaitRspCnt[%u]\n\n", g_u32ScrtWaitResCnt, g_u32ScrtWaitRspCnt);

done:
    return;
}

void nl_scrt_cmd_func_init(void)
{
    // internal
    #ifdef SCRT_CMD_SUT_TASK
    scrt_sut_task_main_1 = scrt_sut_task_main_1_impl;
    scrt_sut_task_main_2 = scrt_sut_task_main_2_impl;
    scrt_sut_task_main_3 = scrt_sut_task_main_3_impl;
    scrt_sut_task_main_4 = scrt_sut_task_main_4_impl;
    scrt_sut_task_delete = scrt_sut_task_delete_impl;
    scrt_sut_task_create = scrt_sut_task_create_impl;
    #endif //#ifdef SCRT_CMD_SUT_TASK

    // external
    nl_scrt_cmd = nl_scrt_cmd_impl;
    return;
}

#endif //#ifdef SCRT_CMD

