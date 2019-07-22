/*
 *  Platform-specific and custom entropy polling functions
 *
 *  Copyright (C) 2006-2016, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_ENTROPY_C)

#include "mbedtls/entropy_poll.h"

#if defined(MBEDTLS_ENTROPY_HARDWARE_ALT)
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "hal_tmr.h"
#include "cmsis_os.h"

static uint32_t hal_get_1m_timer(void)
{
#ifndef __SIMULATOR__
	return reg_read(0x40003044);
#else
	return osKernelSysTick();
#endif
}

int get_random(unsigned char *buf, size_t len)
{
    int i, j;
    uint32_t tmp;

    for (i = 0; i < ((len + 3) & ~3) / 4; i++) {
        tmp = hal_get_1m_timer();

        for (j = 0; j < 4; j++) {
            if ((i * 4 + j) < len) {
                buf[i * 4 + j] = (uint8_t)(tmp >> (j * 8));
            } else {
                break;
            }
        }
    }

    return 0;
}

static void generate_random(void *buf, unsigned short len)
{
    uint32_t counter = hal_get_1m_timer();
    uint8_t *p = buf;
    uint32_t s;
    srand(counter);
    s = rand();

    while (len--)
    {
       srand(s++);
       *p++ = rand() & 0xFF;
    }
}

int mbedtls_hardware_poll( void *data,
        unsigned char *output, size_t len, size_t *olen )
{
   generate_random(output, len);
    *olen = len;
    return 0;
}

int mbedtls_hardware_poll_v1( void *data,
        unsigned char *output, size_t len, size_t *olen )
{
    uint32_t tick = hal_get_1m_timer();
    ((void) data);
    *olen = 0;

    if( len < sizeof(uint32_t) )
        return( 0 );
    memcpy( output, &tick, sizeof(uint32_t) );
    *olen = sizeof(uint32_t);

    return( 0 );
}
#endif
#endif /* MBEDTLS_ENTROPY_C */
