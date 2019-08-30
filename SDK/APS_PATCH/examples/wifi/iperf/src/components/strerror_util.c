/******************************************************************************
*  Copyright 2019, Opulinks Technology Ltd.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2019
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "strerror_util.h"

typedef struct {
    int index;
    char err_str[24];
}util_strerror_t;

static const util_strerror_t gUtilStrErr[] = {
    {0,   "Ok"},            /* No Error. */
    {5,   "EIO"},           /* I/O error */
    {11,  "EAGAIN"},        /* Try again */ /* Operation would block */
    {12,  "ENOMEM"},        /* Out of memory */
    {22,  "EINVAL"},        /* Invalid argument */
    {98,  "EADDRINUSE"},    /* Address already in use */
    {103, "ECONNABORTED"},  /* Software caused connection abort */
    {104, "ECONNRESET"},    /* Connection reset by peer */
    {105, "ENOBUFS"},       /* No buffer space available */
    {106, "EISCONN"},       /* Transport endpoint is already connected */
    {107, "ENOTCONN"},      /* Transport endpoint is not connected */
    {113, "EHOSTUNREACH"},  /* No route to host */
    {114, "EALREADY"},      /* Operation already in progress */
    {115, "EINPROGRESS"},   /* Operation now in progress */
};

/**
 * @brief Translate the errno to error string.
 * 
 * @param err [in] The errno when something error occur
 * @return error string
 */
const char * util_strerr(int err)
{
    int i;
    int table_count;
    
    if (err < 0) {
        return "Unknown error";
    }
    
    table_count = sizeof(gUtilStrErr)/sizeof(gUtilStrErr[0]);
    
    for (i=0; i<table_count; i++) {
        if (err == gUtilStrErr[i].index) {
            return gUtilStrErr[i].err_str;
        }
    }
    
    //Not found
    return "Unknown error";
}
