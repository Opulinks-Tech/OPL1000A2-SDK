/******************************************************************************
*  Copyright 2018, Netlink Communication Corp.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Netlnik Communication Corp. (C) 2018
******************************************************************************/
/**
 *  file         : ftoa.c
 *  description  : convert double to string
 *
 */

#ifndef __FTOA_H__
#define __FTOA_H__

#ifdef __cplusplus
extern "C" {
#endif

char * ftoa(double f, char * buf, int precision);

#ifdef __cplusplus
}
#endif

#endif // __FTOA_H__
