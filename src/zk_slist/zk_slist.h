#pragma once

#include <stddef.h>

#include "zk_common/zk_common.h"

/**
 * @brief Singly linked list node.
 */
struct zk_slist {
	void *data;
	struct zk_slist *next;
};
typedef struct zk_slist zk_slist;

zk_slist *zk_slist_begin(zk_slist *list);

zk_slist *zk_slist_end(zk_slist *list);

zk_slist *zk_slist_find(zk_slist *list, const void *const data, zk_compare_func const func);

zk_slist *zk_slist_find_index(zk_slist *list, size_t const index);

void zk_slist_for_each(zk_slist *begin, zk_slist *const end, zk_for_each_func const func, void *const user_data);

void zk_slist_free(zk_slist **list_p, zk_destructor_t const func);

zk_slist *zk_slist_merge(zk_slist *list, zk_slist *other, zk_compare_func const func);

zk_slist *zk_slist_new_node(void *const data);

zk_slist *zk_slist_pop_back(zk_slist *list, zk_destructor_t const func);

zk_slist *zk_slist_pop_front(zk_slist *list, zk_destructor_t const func);

zk_slist *zk_slist_push_back(zk_slist *list, void *const data);

zk_slist *zk_slist_push_front(zk_slist *list, void *const data);

zk_slist *zk_slist_reverse(zk_slist *list);

size_t zk_slist_size(const zk_slist *const list);

zk_slist *zk_slist_sort(zk_slist *list, zk_compare_func const func);
