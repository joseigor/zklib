#pragma once

#include "zk_common/zk_types.h"

typedef struct _zk_slist_t {
	void *data;
	struct _zk_slist_t *next;
} zk_slist_t;

// TODO: add comments and description for all methods
// TODO: generate documentation from the code comments

#if UNIT_TEST
void _zk_slist_front_back_split(zk_slist_t *list, zk_slist_t **front, zk_slist_t **back);
#endif

zk_slist_t *zk_slist_append(zk_slist_t *list, void *data);

zk_slist_t *zk_slist_concat(zk_slist_t *list_dest, zk_slist_t *list_src);

zk_slist_t *zk_slist_copy(const zk_slist_t *list);

zk_slist_t *zk_slist_copy_deep(const zk_slist_t *list, zk_copy_data_t func, void *user_data);

zk_slist_t *zk_slist_delete_node(zk_slist_t *list, zk_slist_t *node, zk_destructor_t func);

// FIXME: rename to zk_slist_find
zk_slist_t *zk_slist_find_by_data(zk_slist_t *list, const void *const data);

// FIXME: rename to zk_slist_find_custom
zk_slist_t *zk_slist_find_by_data_custom(zk_slist_t *list, const void *const data, zk_compare_t func);

void zk_slist_foreach(zk_slist_t *list, zk_foreach_t func, void *user_data);

void zk_slist_free(zk_slist_t **list_p);

void zk_slist_free_full(zk_slist_t **list_p, zk_destructor_t func);

// FIXME: rename to zk_slist_index
int zk_slist_get_index(zk_slist_t *list, const void *const data);

zk_slist_t *zk_slist_insert(zk_slist_t *list, void *data, int position);

zk_slist_t *zk_slist_insert_before(zk_slist_t *list, zk_slist_t *sibling, void *data);

// TODO: to implement
zk_slist_t *zk_slist_insert_before_link(zk_slist_t *list, zk_slist_t *sibling, zk_slist_t *link);

// TODO: to implement
zk_slist_t *zk_slist_insert_sorted(zk_slist_t *list, void *data, zk_compare_t func);

// TODO: to implement and create zk_compare_data_t
zk_slist_t *zk_slist_insert_sorted_with_data(zk_slist_t *list, void *data, zk_compare_t func, void *user_data);

zk_slist_t *zk_slist_last(zk_slist_t *list);

unsigned int zk_slist_length(zk_slist_t *list);

zk_slist_t *zk_slist_nth(zk_slist_t *list, unsigned int n);

// TODO: to implement
void *zk_slist_nth_data(zk_slist_t *list, unsigned int n);

// TODO: to implement
zk_slist_t *zk_slist_nth_prev(zk_slist_t *list, unsigned int n);

// TODO: to implement
unsigned int zk_slist_position(zk_slist_t *list, zk_slist_t *link);

zk_slist_t *zk_slist_prepend(zk_slist_t *list, void *data);

// TODO: to implement
zk_slist_t *zk_slist_remove(zk_slist_t *list, const void *const data);

// TODO: to implement
zk_slist_t *zk_slist_remove_all(zk_slist_t *list, const void *const data);

// TODO: to implement
zk_slist_t *zk_slist_reverse(zk_slist_t *list);

// TODO: to implement
zk_slist_t *zk_slist_sort(zk_slist_t *list, zk_compare_t func);

// TODO: to implement and create zk_compare_data_t
zk_slist_t *zk_slist_sort_with_data(zk_slist_t *list, zk_compare_t func, void *user_data);
