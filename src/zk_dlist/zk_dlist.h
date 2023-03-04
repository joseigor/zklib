#ifndef ZK_DLIST_H
#define ZK_DLIST_H

#include "zk_common/zk_common.h"

typedef struct zk_dlist zk_dlist;

// Constructor
zk_status zk_dlist_new_node(zk_dlist **node_p, void *const data);

// Destructor
void zk_dlist_free(zk_dlist **list_p, zk_destructor_t const func);

// Element access
zk_status zk_dlist_get_data(const zk_dlist *const list, void **data);

// Iterators
zk_status zk_dlist_next(const zk_dlist *const list, zk_dlist **next);

zk_status zk_dlist_prev(const zk_dlist *const list, zk_dlist **prev);

zk_dlist *zk_dlist_begin(zk_dlist *list);

zk_dlist *zk_dlist_end(zk_dlist *list);

void zk_dlist_for_each(zk_dlist *begin, zk_dlist *const end, zk_for_each_func func, void *const user_data);

// Modifiers
zk_status zk_dlist_pop_back(zk_dlist **list_p, zk_destructor_t const func);

zk_status zk_dlist_pop_front(zk_dlist **list_p, zk_destructor_t const func);

zk_status zk_dlist_push_back(zk_dlist **list_p, void *const data);

zk_status zk_dlist_push_front(zk_dlist **list_p, void *const data);

#endif
