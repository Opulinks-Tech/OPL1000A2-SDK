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
#ifndef _MW_FIM_DEFAULT_GROUP12_PROJECT_H_
#define _MW_FIM_DEFAULT_GROUP12_PROJECT_H_

#ifdef __cplusplus
extern "C" {
#endif

// Sec 0: Comment block of the file


// Sec 1: Include File
#include "mw_fim.h"
//#include "mw_fim_default_group12.h"
#include "blewifi_common.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
// the file ID
// xxxx_xxxx_xxxx_xxxx_xxxx_xxxx_xxxx_xxxx
// ^^^^ ^^^^ Zone (0~3)
//           ^^^^ ^^^^ Group (0~8), 0 is reserved for swap
//                     ^^^^ ^^^^ ^^^^ ^^^^ File ID, start from 0
typedef enum
{
    MW_FIM_IDX_GP12_PROJECT_START = 0x01020000,             // the start IDX of group 12
    
    MW_FIM_IDX_GP12_PROJECT_AWS_PKCS11_KEY,
    
    MW_FIM_IDX_GP12_PROJECT_MAX
} E_MwFimIdxGroup12_Project;


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, uniou, enum, linked list
// AWS PKCS11 Keys

typedef struct
{
    char P11_Key[P11_KEY_SIZE];
} T_MwFim_GP12_AWS_PKCS11_KEYS;

#define MW_FIM_GP12_AWS_PKCS11_KEY_SIZE  sizeof(T_MwFim_GP12_AWS_PKCS11_KEYS)
#define MW_FIM_GP12_AWS_PKCS11_KEY_NUM   1

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern const T_MwFimFileInfo g_taMwFimGroupTable12_project[];

extern const T_MwFim_GP12_AWS_PKCS11_KEYS g_tMwFimDefaultGp12AWSPKCS11Keys;


// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


#ifdef __cplusplus
}
#endif

#endif // _MW_FIM_DEFAULT_GROUP12_PROJECT_H_
