/*
 *  FIPS-180-2 compliant SHA-256 implementation
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
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
/*
 *  The SHA-256 Secure Hash Standard was published by NIST in 2002.
 *
 *  http://csrc.nist.gov/publications/fips/fips180-2/fips180-2.pdf
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_SHA256_C) && defined(MBEDTLS_SHA256_ALT)

#include "mbedtls/sha256.h"

#include <string.h>

#if defined(MBEDTLS_SELF_TEST)
#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#include <stdlib.h>
#define mbedtls_printf printf
#endif /* MBEDTLS_PLATFORM_C */
#endif /* MBEDTLS_SELF_TEST */

#include "scrt.h"


/* Implementation that should never be optimized out by the compiler */
static void mbedtls_zeroize( void *v, size_t n ) {
    volatile unsigned char *p = v; while( n-- ) *p++ = 0;
}

/*
 * 32-bit integer manipulation macros (big endian)
 */
#ifndef GET_UINT32_BE
#define GET_UINT32_BE(n,b,i)                            \
do {                                                    \
    (n) = ( (uint32_t) (b)[(i)    ] << 24 )             \
        | ( (uint32_t) (b)[(i) + 1] << 16 )             \
        | ( (uint32_t) (b)[(i) + 2] <<  8 )             \
        | ( (uint32_t) (b)[(i) + 3]       );            \
} while( 0 )
#endif

#ifndef PUT_UINT32_BE
#define PUT_UINT32_BE(n,b,i)                            \
do {                                                    \
    (b)[(i)    ] = (unsigned char) ( (n) >> 24 );       \
    (b)[(i) + 1] = (unsigned char) ( (n) >> 16 );       \
    (b)[(i) + 2] = (unsigned char) ( (n) >>  8 );       \
    (b)[(i) + 3] = (unsigned char) ( (n)       );       \
} while( 0 )
#endif

void mbedtls_sha256_init( mbedtls_sha256_context *ctx )
{
    memset( ctx, 0, sizeof( mbedtls_sha256_context ) );
}

void mbedtls_sha256_free( mbedtls_sha256_context *ctx )
{
    if( ctx == NULL )
        return;

    mbedtls_zeroize( ctx, sizeof( mbedtls_sha256_context ) );
}

void mbedtls_sha256_clone( mbedtls_sha256_context *dst,
                           const mbedtls_sha256_context *src )
{
    *dst = *src;
}

/*
 * SHA-256 context setup
 */
int mbedtls_sha256_starts_ret( mbedtls_sha256_context *ctx, int is224 )
{
    memset( ctx, 0, sizeof( mbedtls_sha256_context ) );
    ctx->is224 = is224;
    return 0;
}

#if !defined(MBEDTLS_DEPRECATED_REMOVED)
void mbedtls_sha256_starts( mbedtls_sha256_context *ctx, int is224 )
{
    mbedtls_sha256_starts_ret( ctx, is224 );
}
#endif

int mbedtls_internal_sha256_process( mbedtls_sha256_context *ctx, const unsigned char data[64] )
{
    int ret = -1;
    uint8_t type = 0;

    if(ctx->is224)
    {
        type = SCRT_TYPE_SHA_224;
    }
    else
    {
        type = SCRT_TYPE_SHA_256;
    }

    if(!nl_scrt_sha(type, 1, 0, (uint8_t *)data, SHA256_BLOCK_SIZE, ctx->u8HasInterMac, (uint8_t *)ctx->state))
    {
        goto done;
    }

    if(ctx->u8HasInterMac == 0)
    {
        ctx->u8HasInterMac = 1;
    }
    ret = 0;

done:
    return ret;
}

#if !defined(MBEDTLS_DEPRECATED_REMOVED)
void mbedtls_sha256_process( mbedtls_sha256_context *ctx,
                           const unsigned char data[64] )
{
    mbedtls_internal_sha256_process( ctx, data );
}
#endif

/*
 * SHA-256 process buffer
 */
int mbedtls_sha256_update_ret( mbedtls_sha256_context *ctx, const unsigned char *input,
                    size_t ilen )
{
    int ret = 0;
    size_t fill = 0;
    uint32_t left = 0;
    uint32_t step = SHA256_BLOCK_SIZE;

    if( ilen == 0 )
        return 0;

    left = ctx->total[0] & (step - 1);
    fill = step - left;

    ctx->total[0] += (uint32_t) ilen;
    ctx->total[0] &= 0xFFFFFFFF;

    if( ctx->total[0] < (uint32_t) ilen )
        ctx->total[1]++;

    if( left && ilen >= fill )
    {
        memcpy( (void *) (ctx->buffer + left), input, fill );

        if ( ( ret = mbedtls_internal_sha256_process( ctx, ctx->buffer ) ) != 0 ) {
            return ret;
        }

        input += fill;
        ilen  -= fill;
        left = 0;
    }

    while( ilen >= step )
    {
        if ( ( ret = mbedtls_internal_sha256_process( ctx, input ) ) != 0 ) {
            return ret;
        }

        input += step;
        ilen  -= step;
    }

    if( ilen > 0 )
        memcpy( (void *) (ctx->buffer + left), input, ilen );

    return ret;
}

#if !defined(MBEDTLS_DEPRECATED_REMOVED)
void mbedtls_sha256_update( mbedtls_sha256_context *ctx,
                          const unsigned char *input,
                          size_t ilen )
{
    mbedtls_sha256_update_ret( ctx, input, ilen );
}
#endif

/*
 * SHA-256 final digest
 */
int mbedtls_sha256_finish_ret( mbedtls_sha256_context *ctx, unsigned char output[32] )
{
    int ret = -1;
    uint8_t type = 0;

    if(ctx->is224)
    {
        type = SCRT_TYPE_SHA_224;
    }
    else
    {
        type = SCRT_TYPE_SHA_256;
    }

    if(!nl_scrt_sha(type, 1, ctx->total[0], ctx->buffer, (ctx->total[0] & (SHA256_BLOCK_SIZE - 1)), ctx->u8HasInterMac, (uint8_t *)ctx->state))
    {
        goto done;
    }

    if(ctx->is224)
    {
        memcpy(output, ctx->state, 28);
    }
    else
    {
        memcpy(output, ctx->state, 32);
    }
    ret = 0;
done:
    return ret;
}

#if !defined(MBEDTLS_DEPRECATED_REMOVED)
void mbedtls_sha256_finish( mbedtls_sha256_context *ctx,
                          unsigned char output[20] )
{
    mbedtls_sha256_finish_ret( ctx, output );
}
#endif

#endif /* MBEDTLS_SHA256_C && MBEDTLS_SHA256_ALT */
