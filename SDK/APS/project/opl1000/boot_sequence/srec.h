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

#ifndef SREC_H_
#define SREC_H_

#include <stdint.h>

int32_t serial_gets_s(int8_t *buf, uint32_t len);
uint32_t srec_process(const int8_t *srec_line, uint32_t len, uint32_t *addr, uint32_t *byte_num);
int32_t srec_load(uint32_t *entry_point);

#endif /*SREC_H_*/
