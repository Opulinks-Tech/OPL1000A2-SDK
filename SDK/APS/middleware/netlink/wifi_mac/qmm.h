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
 * @file qmm.h
 *
 * @brief This file contains the Queue Management Module definitions.
 */

#ifndef QMM_INTERFACE_H
#define QMM_INTERFACE_H

#include <stdint.h>
#include "bmm.h"

/**
 * \ingroup group_resources
 * \defgroup group_qmm  Queue Management
 * Queue Management: provides services for creating and maintaining the queues.
 *  @{
 */

typedef enum
{
	QUEUE_SUCCESS       = 0x00,
    QUEUE_FULL          = 0x01,
    QUEUE_EMPTY         = 0x02,
} retval_t;

/**
 * @brief Structure to search for a buffer to be removed from a queue
 */
typedef struct search_tag
{
	/** Pointer to search criteria function */
	uint8_t (*criteria_func)(void *buf, void *handle);
	/** Handle to callbck parameter */
	void *handle;
} search_t;

/**
 * @brief Queue structure
 *
 * This structur defines the queue structure.
 * The application should declare the queue of type queue_t
 * and call qmm_queue_init before invoking any other functionality of qmm.
 *
 * @ingroup apiMacTypes
 */
typedef struct queue_tag
{
	/** Pointer to head of queue */
	buffer_t *head;
	/** Pointer to tail of queue */
	buffer_t *tail;

	/**
	 * Maximum number of buffers that can be accomodated in the current queue
	 * Note: This is only required if the queue capacity shall be different
	 * from 255.
	 */
	uint8_t capacity;

	/**
	 * Number of buffers present in the current queue
	 */
	uint8_t size;
} queue_t;


#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Initializes the queue.
 *
 * This function initializes the queue. Note that this function
 * should be called before invoking any other functionality of QMM.
 *
 * @param q The queue which should be initialized.
 *
 */
void qmm_queue_init(queue_t *q, uint8_t capacity);

/**
 * @brief Appends a buffer into the queue.
 *
 * This function appends a buffer into the queue.
 *
 * @param q Queue into which buffer should be appended
 *
 * @param buf Pointer to the buffer that should be appended into the queue.
 * Note that this pointer should be same as the
 * pointer returned by bmm_buffer_alloc.
 *
 */
retval_t qmm_queue_append(queue_t *q, buffer_t *buf);

/**
 * @brief Removes a buffer from queue.
 *
 * This function removes a buffer from queue
 *
 * @param q Queue from which buffer should be removed
 *
 * @param search Search criteria. If this parameter is NULL, first buffer in the
 * queue will be removed. Otherwise buffer matching the criteria will be
 * removed.
 *
 * @return Pointer to the buffer header, if the buffer is
 * successfully removed, NULL otherwise.
 *
 */
buffer_t *qmm_queue_remove(queue_t *q, search_t *search);

/**
 * @brief Reads a buffer from queue.
 *
 * This function reads either the first buffer if search is NULL or buffer
 * matching the given criteria from queue.
 *
 * @param q The queue from which buffer should be read.
 *
 * @param search If this parameter is NULL first buffer in the queue will be
 * read. Otherwise buffer matching the criteria will be read
 *
 * @return Pointer to the buffer header which is to be read, NULL if the buffer
 * is not available
 *
 */
buffer_t *qmm_queue_read(queue_t *q, search_t *search);

/**
 * @brief Internal function for flushing a specific queue
 *
 * @param q Queue to be flushed
 *
 */
void qmm_queue_flush(queue_t *q);

#ifdef __cplusplus
} /* extern "C" */
#endif

/* ! @} */
#endif /* QMM_INTERFACE_H */

/* EOF */
