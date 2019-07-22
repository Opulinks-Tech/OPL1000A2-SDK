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

#ifndef _BLE_QUEUE_H_
#define _BLE_QUEUE_H_

#include "ble_type_define.h"

typedef void *LE_QLIST_T[5];


void LeQueuePushHead(void *ls, void *m);

void LeQueuePush(void *ls, void *m);

void* LeQueuePeekAny(void *ls, void *s);

void* LeQueuePeek(void *ls);

void* LeQueuePop(void *ls);

void LeQueueDelete(void *ls, void *m);

void LeQueueMove(void *d, void *s, void *m);

void LeQueueBroken(void *ls);

UINT16 LeQueueCount(void *ls);

#endif
