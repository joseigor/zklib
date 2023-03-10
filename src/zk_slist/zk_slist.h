#pragma once

#include <stddef.h>

#include "zk_common/zk_common.h"

typedef struct zk_slist zk_slist;
// Constructor
zk_status zk_slist_new_node(zk_slist **node_p, void *const data);

// Destructor
void zk_slist_free(zk_slist **list_p, zk_destructor_t const func);

// Element access
zk_status zk_slist_get_data(const zk_slist *const list, void **data);

// Iterators
zk_status zk_slist_next(const zk_slist *const list, zk_slist **next);

zk_slist *zk_slist_begin(zk_slist *list);

zk_slist *zk_slist_end(zk_slist *list);

void zk_slist_for_each(zk_slist *begin, zk_slist *const end, zk_for_each_func const func, void *const user_data);

// Modifiers
zk_status zk_slist_pop_back(zk_slist **list_p, zk_destructor_t const func);

zk_status zk_slist_pop_front(zk_slist **list_p, zk_destructor_t const func);

zk_status zk_slist_push_back(zk_slist **list_p, void *const data);

zk_status zk_slist_push_front(zk_slist **list_p, void *const data);

/**
 * @brief Reverses the order of the elements in the list.
 *
 * @param list_p Pointer to the list to reverse.
 *
 * @return ZK_OK if the list was reversed successfully. ZK_INVALID_ARGUMENT if list_p is NULL or *list_p is NULL.
*/
zk_status zk_slist_reverse(zk_slist **list_p);
