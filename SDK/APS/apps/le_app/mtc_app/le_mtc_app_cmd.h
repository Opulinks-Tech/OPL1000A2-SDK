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

#ifndef _LE_MTC_APP_CMD_H_
#define _LE_MTC_APP_CMD_H_

#include "le.h"
//#include "le_mtc_app.h"
#include "le_misc_patch.h"

#ifdef INCLUDE_MTC_APP

#define LeMtcAppNumToDecStr(x, y) LeHtcUtilNumToDecStr(x, y)
#define LeMtcAppNumToStr(x, y, z) LeHtcUtilOctetNumToHexStr(x, y, z)

#define LE_TEST_APP_SHOW_MSG	LeHtcUtilShowMsg
#define LeHostSendCmdRsp(x)		LeHtcUtilSendCmdRsp(x)
#define NumToStr(x, y, z)		LeHtcUtilOctetNumToHexStr(x, y, z)

//void ParseLeMtcTestCommand(char* pszData, int dataLen);

//void LeMtcAppProcessCliCmd(void);

#endif

#endif
