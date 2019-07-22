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

#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

typedef struct linked_item{
    struct linked_item *next; // <-- next element in list, or NULL
    void *user_data;          // <-- pointer to struct base
} linked_item_t;

typedef struct linked_item_dl{
    struct linked_item_dl *next; // <-- next element in list, or NULL    
    struct linked_item_dl *prev; // <-- prev element in list, or NULL
    void *user_data;          // <-- pointer to struct base
} linked_item_dl_t;

typedef linked_item_dl_t * linked_list_dl_t;
typedef linked_item_t * linked_list_t;

typedef struct {
    linked_item_dl_t* first;
    linked_item_dl_t* last;
//    u32       length;
} dl_list_t;

typedef struct {
    volatile linked_item_t* first;
    volatile linked_item_t* last;
    volatile signed int length;
} sl_list_t;

typedef struct {
	int advance_on_next;
    linked_item_t * prev;	// points to the item before the current one
    linked_item_t * curr;	// points to the current item (to detect item removal)
} linked_list_iterator_t;


void            linked_item_set_user(linked_item_t *item, void *user_data);        // <-- set user data
void *          linked_item_get_user(linked_item_t *item);                         // <-- get user data
int             linked_list_empty(linked_list_t * list);
void            linked_list_add(linked_list_t * list, linked_item_t *item);        // <-- add item to list as first element
void            linked_list_add_tail(linked_list_t * list, linked_item_t *item);   // <-- add item to list as last element
int             linked_list_remove(linked_list_t * list, linked_item_t *item);     // <-- remove item from list
linked_item_t * linked_list_get_last_item(linked_list_t * list);                   // <-- find the last item in the list

/**
 * @brief Counts number of items in list
 * @returns number of items in list
 */
int linked_list_count(linked_list_t * list);

//
// iterator for linked lists. alloes to remove current element. also robust against removal of current element by linked_list_remove
//
void            linked_list_iterator_init(linked_list_iterator_t * it, linked_list_t * list);
int             linked_list_iterator_has_next(linked_list_iterator_t * it);
linked_item_t * linked_list_iterator_next(linked_list_iterator_t * it);
void            linked_list_iterator_remove(linked_list_iterator_t * it);
int  linked_list_remove_first_dl(dl_list_t * dl_list);
void linked_list_add_tail_dl(dl_list_t * dl_list, linked_item_dl_t *item_dl);
int  linked_list_remove_first_sl(sl_list_t * sl_list);
void linked_list_add_tail_sl(sl_list_t * sl_list, linked_item_t *item);

void test_linked_list(void);

#endif //_LINKED_LIST_H_
