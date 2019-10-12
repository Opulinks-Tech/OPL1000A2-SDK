/*
 * Amazon FreeRTOS PKCS #11 PAL V1.0.0
 * Copyright (C) 2018 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */

/**
 * @file aws_pkcs11_pal.c
 * @brief Device specific helpers for PKCS11 Interface.
 */

/* Amazon FreeRTOS Includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "aws_crypto.h"
#include "aws_pkcs11.h"
#include "aws_pkcs11_config.h"
#include "cmsis_os.h"


#include "mw_fim_default_group08.h"
#include "mw_fim_default_group08_project.h"
#include "mw_fim_default_group11_project.h"

/* C runtime includes. */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* flash driver includes. */

#include "aws_pkcs11_config.h"

/*-----------------------------------------------------------*/

#define pkcs11palFILE_NAME_CLIENT_CERTIFICATE    "FreeRTOS_P11_Certificate.dat"
#define pkcs11palFILE_NAME_KEY                   "FreeRTOS_P11_Key.dat"
#define pkcs11palFILE_CODE_SIGN_PUBLIC_KEY       "FreeRTOS_P11_CodeSignKey.dat"

/*-----------------------------------------------------------*/

//extern T_MwFim_GP08_AWS_PKCS11_KEYS tAWSPKCSKeys;
extern T_MwFim_GP08_AWS_PKCS11_KEYS tGP08_AWSPKCSKeys;
extern T_MwFim_GP11_AWS_PKCS11_KEYS tGP11_AWSPKCSKeys;
extern T_AWS_PKCS11_KEYS tAWSPKCSKeys;

struct pkcs11_Key_size
{
    uint32_t nCertificateSize;
    uint32_t nKeySize;
    uint32_t nCodeSignKey;
};



enum eObjectHandles
{
    eInvalidHandle = 0, /* According to PKCS #11 spec, 0 is never a valid object handle. */
    eAwsDevicePrivateKey = 1,
    eAwsDevicePublicKey,
    eAwsDeviceCertificate,
    eAwsCodeSigningKey
};

/*-----------------------------------------------------------*/

struct dict_entry
{
    char                    *label;
    char                    *filename;
    CK_OBJECT_HANDLE        xHandle;
    CK_BBOOL                isPrivate;
} dict[] = {
    {
        pkcs11configLABEL_DEVICE_CERTIFICATE_FOR_TLS,
        pkcs11palFILE_NAME_CLIENT_CERTIFICATE,
        eAwsDeviceCertificate,
        CK_FALSE
    },
    {
        pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS,
        pkcs11palFILE_NAME_KEY, // why same as pkcs11configLABEL_DEVICE_PUBLIC_KEY_FOR_TLS?
        eAwsDevicePrivateKey,
        CK_TRUE
    },
    {
        pkcs11configLABEL_DEVICE_PUBLIC_KEY_FOR_TLS,
        pkcs11palFILE_NAME_KEY, // why same as pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS?
        eAwsDevicePublicKey,
        CK_FALSE
    },
    {
        pkcs11configLABEL_CODE_VERIFICATION_KEY,
        pkcs11palFILE_CODE_SIGN_PUBLIC_KEY,
        eAwsCodeSigningKey,
        CK_FALSE
    }
};

#define DICT_SIZE   ( sizeof( dict ) / sizeof ( struct dict_entry ) )
 
struct pkcs11_Key_size opl_pkcs11_key_size;

/*-----------------------------------------------------------*/

/* Converts a label to its respective filename and handle. */
void prvLabelToFilenameHandle( uint8_t              *pcLabel,
                               char                 **pcFileName,
                               CK_OBJECT_HANDLE_PTR pHandle )
{
    int i;

    if( pcLabel != NULL )
    {
        /* Translate from the PKCS#11 label to local storage file name. */

        for( i = 0; i < DICT_SIZE; i++ )
        {
            if( 0 == strcmp( dict[ i ].label, (const char *)pcLabel ) )
            {
                *pcFileName = &dict[ i ].filename[0];
                *pHandle    = dict[ i ].xHandle;
                return;
            }
        }
    }

    *pcFileName = NULL;
    *pHandle    = eInvalidHandle;
}

/*-----------------------------------------------------------*/

/* Converts a label to its respective filename and handle. */
void prvHandleToFilenamePrivate( CK_OBJECT_HANDLE   xHandle,
                                 char               **pcFileName,
                                 CK_BBOOL           *pIsPrivate )
{
    int i;

    /*
     * Translate from the PKCS#11 handle to local storage file name and
     * private requirement.
     */

    for( i = 0; i < DICT_SIZE; i++ )
    {
        if( dict[ i ].xHandle == xHandle )
        {
            if( pcFileName )
            {
                *pcFileName = &dict[ i ].filename[ 0 ];
            }

            if( pIsPrivate )
            {
                *pIsPrivate = dict[ i ].isPrivate;
            }

            return;
        }
    }

    *pcFileName = NULL;
    *pIsPrivate = CK_FALSE;
}

/*-----------------------------------------------------------*/
/**
* @brief Writes a file to local storage.
*
* Port-specific file write for crytographic information.
*
* @param[in] pxLabel       Label of the object to be saved.
* @param[in] pucData       Data buffer to be written to file
* @param[in] ulDataSize    Size (in bytes) of data to be saved.
*
* @return The file handle of the object that was stored.
*/
CK_OBJECT_HANDLE PKCS11_PAL_SaveObject( CK_ATTRIBUTE_PTR pxLabel,
    uint8_t * pucData,
    uint32_t ulDataSize )
{
    char                *pcFileName = NULL;
    CK_OBJECT_HANDLE    xHandle     = eInvalidHandle;
    bool needUpdateFIM = false;

    /* Converts a label to its respective filename and handle. */

    prvLabelToFilenameHandle( pxLabel->pValue, &pcFileName, &xHandle );

    /* If your project requires additional PKCS#11 objects, add them here. */

    if( pcFileName != NULL )
    {
        //if( hal_file_write( pcFileName, pucData, ulDataSize ) == pdFALSE ) //Goter
        if(0 == strcmp( pcFileName, pkcs11palFILE_NAME_CLIENT_CERTIFICATE ))
        {
            
            printf("===========save file %s , size is %d  =========\n",pcFileName,ulDataSize);
            printf("=========== pucData is %s LINE is %d =========\n",pucData,__LINE__);
            memset(tAWSPKCSKeys.P11_Certificate,0x00,sizeof(tAWSPKCSKeys.P11_Certificate));
            memcpy(tAWSPKCSKeys.P11_Certificate,pucData,ulDataSize);
            opl_pkcs11_key_size.nCertificateSize = ulDataSize;
            needUpdateFIM = true;
        }
        else if (0 == strcmp( pcFileName, pkcs11palFILE_NAME_KEY ))
        {
            
            printf("===========save file %s , size is %d  =========\n",pcFileName,ulDataSize);
            printf("=========== pucData is %s LINE is %d =========\n",pucData,__LINE__);
            memset(tAWSPKCSKeys.P11_Key,0x00,sizeof(tAWSPKCSKeys.P11_Key));
            memcpy(tAWSPKCSKeys.P11_Key,pucData,ulDataSize);
            opl_pkcs11_key_size.nKeySize = ulDataSize;
            needUpdateFIM = true;
        }
        else if (0 == strcmp( pcFileName, pkcs11palFILE_CODE_SIGN_PUBLIC_KEY ))
        {
            printf("===========save file %s , size is %d  =========\n",pcFileName,ulDataSize);
            printf("=========== pucData is %s LINE is %d =========\n",pucData,__LINE__);
            memset(tAWSPKCSKeys.P11_CodeSignKey,0x00,sizeof(tAWSPKCSKeys.P11_CodeSignKey));
            memcpy(tAWSPKCSKeys.P11_CodeSignKey,pucData,ulDataSize);
            opl_pkcs11_key_size.nCodeSignKey = ulDataSize;
            needUpdateFIM = true;
        }
        else
        {
            xHandle = eInvalidHandle;
            needUpdateFIM = false;
        }
    }
    else
    {
        printf("===========   prvLabelToFilenameHandle fail   =========\n");
    }

    if(needUpdateFIM == true)
    {
			  // write AWS_PKCS11_KEY into FIM group8 and FIM group8
        if (MW_FIM_OK != MwFim_FileWrite(MW_FIM_IDX_GP08_PROJECT_AWS_PKCS11_KEY, 0, MW_FIM_GP08_AWS_PKCS11_KEY_SIZE, (uint8_t*)&tGP08_AWSPKCSKeys))
        {
            printf("===========   PKCS Key update FIM fail   =========\n");
        }
        if (MW_FIM_OK != MwFim_FileWrite(MW_FIM_IDX_GP11_PROJECT_AWS_PKCS11_KEYS, 0, MW_FIM_GP11_AWS_PKCS11_KEYS_SIZE, (uint8_t*)&tGP11_AWSPKCSKeys))
        {
            printf("===========   PKCS Certificate & CodeSignKey update FIM fail   =========\n");
        }				
    }

    
    return xHandle;
}

/**
* @brief Translates a PKCS #11 label into an object handle.
*
* Port-specific object handle retrieval.
*
*
* @param[in] pLabel         Pointer to the label of the object
*                           who's handle should be found.
* @param[in] usLength       The length of the label, in bytes.
*
* @return The object handle if operation was successful.
* Returns eInvalidHandle if unsuccessful.
*/
CK_OBJECT_HANDLE PKCS11_PAL_FindObject( uint8_t * pLabel,
    uint8_t usLength )
{
        /* Avoid compiler warnings about unused variables. */
    ( void ) usLength;

    char                *pcFileName = NULL;
    CK_OBJECT_HANDLE    xHandle     = eInvalidHandle;

    /* Converts a label to its respective filename and handle. */

    prvLabelToFilenameHandle( pLabel, &pcFileName, &xHandle );

    if( pcFileName != NULL )
    {
        /* Check if object exists/has been created before returning. */

       // if( pdTRUE != hal_file_find( pcFileName, NULL ) )  //Goter
        if(0 == strcmp( pcFileName, pkcs11palFILE_NAME_CLIENT_CERTIFICATE ))
        {
           
        }
        else if (0 == strcmp( pcFileName, pkcs11palFILE_NAME_KEY ))
        {
            
        }
        else if (0 == strcmp( pcFileName, pkcs11palFILE_CODE_SIGN_PUBLIC_KEY ))
        {
            
        }
        else
        {
            xHandle = eInvalidHandle;
        }

    }

    return xHandle;
}

/**
* @brief Gets the value of an object in storage, by handle.
*
* Port-specific file access for cryptographic information.
*
* This call dynamically allocates the buffer which object value
* data is copied into.  PKCS11_PAL_GetObjectValueCleanup()
* should be called after each use to free the dynamically allocated
* buffer.
*
* @sa PKCS11_PAL_GetObjectValueCleanup
*
* @param[in] pcFileName    The name of the file to be read.
* @param[out] ppucData     Pointer to buffer for file data.
* @param[out] pulDataSize  Size (in bytes) of data located in file.
* @param[out] pIsPrivate   Boolean indicating if value is private (CK_TRUE)
*                          or exportable (CK_FALSE)
*
* @return CKR_OK if operation was successful.  CKR_KEY_HANDLE_INVALID if
* no such object handle was found, CKR_DEVICE_MEMORY if memory for
* buffer could not be allocated, CKR_FUNCTION_FAILED for device driver
* error.
*/
CK_RV PKCS11_PAL_GetObjectValue( CK_OBJECT_HANDLE xHandle,
    uint8_t ** ppucData,
    uint32_t * pulDataSize,
    CK_BBOOL * pIsPrivate )
{
    CK_RV       ulReturn        = CKR_OK;
    char        *pcFileName     = NULL;
    uint8_t     *pucData; 

    prvHandleToFilenamePrivate( xHandle, &pcFileName, pIsPrivate );
    if( NULL == pcFileName )
    {
        ulReturn = CKR_KEY_HANDLE_INVALID;
    }
    else
    {
        #define MAX_FILE_SIZE         (2048)
        pucData = pvPortMalloc( MAX_FILE_SIZE );
        if( NULL == pucData )
        {
            ulReturn = CKR_DEVICE_MEMORY;
        }
        else
        {
            uint32_t    status = pdTRUE;
            

            //status = hal_file_read( pcFileName, pucData, pulDataSize );  //Goter

            if(0 == strcmp( pcFileName, pkcs11palFILE_NAME_CLIENT_CERTIFICATE ))
            {
                memcpy(pucData, tAWSPKCSKeys.P11_Certificate, opl_pkcs11_key_size.nCertificateSize);
                *pulDataSize = opl_pkcs11_key_size.nCertificateSize;
            }
            else if (0 == strcmp( pcFileName, pkcs11palFILE_NAME_KEY ))
            {
                memcpy(pucData, tAWSPKCSKeys.P11_Key, opl_pkcs11_key_size.nKeySize);
                *pulDataSize = opl_pkcs11_key_size.nKeySize;
            }
            else if (0 == strcmp( pcFileName, pkcs11palFILE_CODE_SIGN_PUBLIC_KEY ))
            {
                memcpy(pucData, tAWSPKCSKeys.P11_CodeSignKey, opl_pkcs11_key_size.nCodeSignKey);;
                *pulDataSize = opl_pkcs11_key_size.nCodeSignKey;
            }
            else
            {
                status = pdFAIL;
            }


            printf("===========Get file %s , size is %d  =========\n",pcFileName,*pulDataSize);
            
            if ( status != pdTRUE )
            {
                vPortFree( pucData );
                ulReturn = CKR_FUNCTION_FAILED;
            }
            else
            {
                *ppucData = pucData;
            }
        }
    }

    return ulReturn;
}


/**
* @brief Cleanup after PKCS11_GetObjectValue().
*
* @param[in] pucData       The buffer to free.
*                          (*ppucData from PKCS11_PAL_GetObjectValue())
* @param[in] ulDataSize    The length of the buffer to free.
*                          (*pulDataSize from PKCS11_PAL_GetObjectValue())
*/
void PKCS11_PAL_GetObjectValueCleanup( uint8_t * pucData,
    uint32_t ulDataSize )
{
        /* Unused parameters. */
    ( void ) ulDataSize;

    if( NULL != pucData )
    {
        vPortFree( pucData );
    }
    else
    {

    }
}

/*-----------------------------------------------------------*/
#if 0
int mbedtls_hardware_poll( void * data,
                           unsigned char * output,
                           size_t len,
                           size_t * olen )
{
    /* FIX ME. */
    return 0;
}
#endif
