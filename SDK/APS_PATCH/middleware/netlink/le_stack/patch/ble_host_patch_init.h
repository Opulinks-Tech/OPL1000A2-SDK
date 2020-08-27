/******************************************************************************
*  Copyright 2017, Netlink Communication Corp.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Netlink Communication Corp. (C) 2017
******************************************************************************/

#ifndef _BLE_HOST_PATCH_INIT_H_
#define _BLE_HOST_PATCH_INIT_H_

#ifdef  SDK_LITE
void LeGapIfPatch_Init(void);
#define LeHostPatchAssign()         do {                            \
                                        LeGapIfPatch_Init();        \
                                    } while(0)
#else   /* SDK_LITE */
void LeHostPatchAssign(void);
#endif  /* SDK_LITE */

#endif
