#ifndef ZK_C_SLIST_H
#define ZK_C_SLIST_H

#include "zk_common/zk_common.h"

typedef struct zk_c_slist zk_c_slist;

// Constructor
zk_status zk_c_slist_new_node(zk_c_slist **node_p, void *const data);

// Destructor
void zk_c_slist_free(zk_c_slist **list_p, zk_destructor_t const func);

// Element access
zk_status zk_c_slist_get_data(const zk_c_slist *const list, void **data);

// Iterators
zk_status zk_c_slist_next(const zk_c_slist *const list, zk_c_slist **next);

zk_c_slist *zk_c_slist_begin(zk_c_slist *list);

zk_c_slist *zk_c_slist_end(zk_c_slist *list);

void zk_c_slist_for_each(zk_c_slist *begin, zk_c_slist *const end, zk_for_each_func const func, void *const user_data);

// Modifiers

zk_status zk_c_slist_pop_back(zk_c_slist **list_p, zk_destructor_t const func);

zk_status zk_c_slist_pop_front(zk_c_slist **list_p, zk_destructor_t const func);

zk_status zk_c_slist_push_back(zk_c_slist **list_p, void *const data);

zk_status zk_c_slist_push_front(zk_c_slist **list_p, void *const data);

#endif
