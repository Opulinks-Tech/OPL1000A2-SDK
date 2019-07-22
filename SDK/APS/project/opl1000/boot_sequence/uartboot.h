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

#ifndef _UARTBOOT_H_
#define _UARTBOOT_H_

uint8_t getChar(void);
bool checkPattern(uint8_t checkPattern, uint32_t retry);
bool checkUartBoot(void);
void uartBoot(void);

#endif /*_UARTBOOT_H_*/
