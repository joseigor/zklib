#ifndef ZK_C_DLIST_H
#define ZK_C_DLIST_H

#include "zk_common/zk_common.h"

typedef struct zk_c_dlist {
	void *data;
	struct zk_c_dlist *prev;
	struct zk_c_dlist *next;
} zk_c_dlist;

// Constructor
zk_c_dlist *zk_c_dlist_new_node(void *const data);

// Destructor
void zk_c_dlist_free(zk_c_dlist **list_p, zk_destructor_t const func);

// Iterators
zk_c_dlist *zk_c_dlist_begin(zk_c_dlist *list);

zk_c_dlist *zk_c_dlist_end(zk_c_dlist *list);

void zk_c_dlist_for_each(zk_c_dlist *begin, zk_c_dlist *const end, zk_for_each_func const func, void *const user_data);

// Modifiers
zk_c_dlist *zk_c_dlist_push_back(zk_c_dlist *list, void *const data);

zk_c_dlist *zk_c_dlist_push_front(zk_c_dlist *list, void *const data);

#endif
