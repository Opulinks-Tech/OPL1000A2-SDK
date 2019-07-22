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

/**
 * @file bmm.h
 *
 * @brief This file contains the Buffer Management Module definitions.
 */

#ifndef BMM_INTERFACE_H
#define BMM_INTERFACE_H

/* === Includes ============================================================ */

#include <stdint.h>
#include <stdbool.h>
#include "pool_config.h"

/**
 * \defgroup group_resources  Resource Management
 * The Resource Management provides access to resources  to the stack or the
 * application.
 *  @{
 */

/**
 * \defgroup group_bmm  Buffer Management
 * Buffer Management (large and small buffers): provides services for
 * dynamically
 * allocating and freeing memory buffers.
 *  @{
 */

/* === Macros ============================================================== */

/**
 * This macro provides the pointer to the corresponding body of the supplied
 * buffer header.
 */

#define BMM_BUFFER_POINTER(buf)  ((buf)->body)

/* === Types =============================================================== */

/**
 * @brief Buffer structure holding information of each buffer.
 *
 */
typedef struct buffer_tag
{
	/** Pointer to the buffer body */
	uint8_t *body;

	/** Pointer to next free buffer */
	struct buffer_tag *next;
} buffer_t;
/* === Externals =========================================================== */

/* === Prototypes ========================================================== */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the buffer module.
 *
 * This function initializes the buffer module.
 * This function should be called before using any other functionality
 * of buffer module.
 *
 * @ingroup apiResApi
 */
void bmm_buffer_init(void);

/**
 * @brief Allocates a buffer
 *
 * This function allocates a buffer and returns a pointer to the buffer.
 * The same pointer should be used while freeing the buffer.User should
 * call BMM_BUFFER_POINTER(buf) to get the pointer to buffer user area.
 *
 * @param size size of buffer to be allocated.
 *
 * @return pointer to the buffer allocated,
 *  NULL if buffer not available.
 *
 * @ingroup apiResApi
 */
#if defined(ENABLE_LARGE_BUFFER)
buffer_t *bmm_buffer_alloc(uint16_t size);

#else
buffer_t *bmm_buffer_alloc(uint8_t size);

#endif

/**
 * @brief Frees up a buffer.
 *
 * This function frees up a buffer. The pointer passed to this function
 * should be the pointer returned during buffer allocation. The result is
 * unpredictable if an incorrect pointer is passed.
 *
 * @param pbuffer Pointer to buffer that has to be freed.
 *
 * @ingroup apiResApi
 */
void bmm_buffer_free(buffer_t *pbuffer);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* BMM_INTERFACE_H */

/* EOF */
