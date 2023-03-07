#ifndef ZK_C_DLIST_H
#define ZK_C_DLIST_H

#include "zk_common/zk_common.h"

typedef struct zk_c_dlist zk_c_dlist;

// Constructor
zk_status zk_c_dlist_new_node(zk_c_dlist **node_p, void *const data);

// Destructor
void zk_c_dlist_free(zk_c_dlist **list_p, zk_destructor_t const func);

// Element access
zk_status zk_c_dlist_get_data(const zk_c_dlist *const list, void **data);

// Iterators
zk_status zk_c_dlist_next(const zk_c_dlist *const list, zk_c_dlist **next);

zk_status zk_c_dlist_prev(const zk_c_dlist *const list, zk_c_dlist **prev);

zk_c_dlist *zk_c_dlist_begin(zk_c_dlist *list);

zk_c_dlist *zk_c_dlist_end(zk_c_dlist *list);

void zk_c_dlist_for_each(zk_c_dlist *begin, zk_c_dlist *const end, zk_for_each_func const func, void *const user_data);

// Modifiers
zk_status zk_c_dlist_pop_back(zk_c_dlist **list_p, zk_destructor_t const func);

zk_status zk_c_dlist_pop_front(zk_c_dlist **list_p, zk_destructor_t const func);

zk_status zk_c_dlist_push_back(zk_c_dlist **list_p, void *const data);

zk_status zk_c_dlist_push_front(zk_c_dlist **list_p, void *const data);

#endif
