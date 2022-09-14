#pragma once

#include <stddef.h>

#include "zk_common/zk_common.h"

/**
 * @brief Single linked list struct
 *
 */
typedef struct zk_slist {
	void *data;
	struct zk_slist *next;
} zk_slist;

// Constructor
zk_status zk_slist_new_node(zk_slist **node_p, void *const data);

// Destructor
void zk_slist_free(zk_slist **list_p, zk_destructor_t const func);

// Iterators
zk_slist *zk_slist_begin(zk_slist *list);

zk_slist *zk_slist_end(zk_slist *list);

void zk_slist_for_each(zk_slist *begin, zk_slist *const end, zk_for_each_func const func, void *const user_data);

// Modifiers
zk_status zk_slist_pop_back(zk_slist **list_p, zk_destructor_t const func);

zk_status zk_slist_pop_front(zk_slist **list_p, zk_destructor_t const func);

zk_status zk_slist_push_back(zk_slist **list_p, void *const data);

zk_status zk_slist_push_front(zk_slist **list_p, void *const data);
