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

/***********************
Head Block of The File
***********************/
#ifndef _MW_FIM_DEFAULT_GROUP08_PROJECT_H_
#define _MW_FIM_DEFAULT_GROUP08_PROJECT_H_

#ifdef __cplusplus
extern "C" {
#endif

// Sec 0: Comment block of the file


// Sec 1: Include File
#include "mw_fim.h"
#include "mw_fim_default_group08.h"
#include "blewifi_common.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
// the file ID
// xxxx_xxxx_xxxx_xxxx_xxxx_xxxx_xxxx_xxxx
// ^^^^ ^^^^ Zone (0~3)
//           ^^^^ ^^^^ Group (0~8), 0 is reserved for swap
//                     ^^^^ ^^^^ ^^^^ ^^^^ File ID, start from 0
typedef enum
{
    MW_FIM_IDX_GP08_PROJECT_START = 0x00080000,             // the start IDX of group 08
    
    MW_FIM_IDX_GP08_PROJECT_AWS_PKCS11_KEY,
    
    MW_FIM_IDX_GP08_PROJECT_MAX
} E_MwFimIdxGroup08_Project;


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, uniou, enum, linked list
// AWS PKCS11 Keys


//#define P11_CERTIFICATE_SIZE 1024
//#define P11_KEY_SIZE         2048
//#define P11_CODESIGNKEY      768

/*
typedef struct
{
    char P11_Certificate[P11_CERTIFICATE_SIZE];
    char P11_Key[P11_KEY_SIZE];
    char P11_CodeSignKey[P11_CODESIGNKEY];
} T_MwFim_GP08_AWS_PKCS11_KEYS;
*/

typedef struct
{
    char P11_Key[P11_KEY_SIZE];
} T_MwFim_GP08_AWS_PKCS11_KEYS;

#define MW_FIM_GP08_AWS_PKCS11_KEY_SIZE  sizeof(T_MwFim_GP08_AWS_PKCS11_KEYS)
#define MW_FIM_GP08_AWS_PKCS11_KEY_NUM   1

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern const T_MwFimFileInfo g_taMwFimGroupTable08_project[];

extern const T_MwFim_GP08_AWS_PKCS11_KEYS g_tMwFimDefaultGp08AWSPKCS11Keys;


// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


#ifdef __cplusplus
}
#endif

#endif // _MW_FIM_DEFAULT_GROUP06_PROJECT_H_
