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

#ifndef _MEMORY_POOL_CTRL_H_
#define _MEMORY_POOL_CTRL_H_

typedef void * memory_pool_t;

// initialize memory pool with with given storage, block size and count
void   memory_pool_create(memory_pool_t *pool, void * storage, int count, int block_size);

// get free block from pool, @returns NULL or pointer to block
void * memory_pool_get(memory_pool_t *pool);

// return previously reserved block to memory pool
void   memory_pool_free(memory_pool_t *pool, void * block);


#endif /* _MEMORY_POOL_CTRL_H_ */
