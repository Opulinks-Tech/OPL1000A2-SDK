
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

#ifndef _SYS_COMMON_LOG_H_
#define _SYS_COMMON_LOG_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SYS_COMMON_LOGE(fmt,arg...)             printf(("E [SYS_COMMON]: "fmt"\r\n"), ##arg)
#define SYS_COMMON_LOGI(fmt,arg...)             printf(("I [SYS_COMMON]: "fmt"\r\n"), ##arg)
    
#ifdef __cplusplus
}
#endif

#endif /* _SYS_COMMON_LOG_H_ */
