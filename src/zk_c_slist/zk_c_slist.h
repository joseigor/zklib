#ifndef ZK_C_SLIST_H
#define ZK_C_SLIST_H

#include "zk_common/zk_common.h"

typedef struct zk_c_slist {
	void *data;
	struct zk_c_slist *next;
} zk_c_slist;

// Constructor
zk_c_slist *zk_c_slist_new_node(void *const data);

// Destructor
void zk_c_slist_free(zk_c_slist **list_p, zk_destructor_t const func);

// Iterators
zk_c_slist *zk_c_slist_begin(zk_c_slist *list);

zk_c_slist *zk_c_slist_end(zk_c_slist *list);

void zk_c_slist_for_each(zk_c_slist *begin, zk_c_slist *const end, zk_for_each_func const func, void *const user_data);

// Modifiers
zk_c_slist *zk_c_slist_push_back(zk_c_slist *list, void *const data);

zk_c_slist *zk_c_slist_push_front(zk_c_slist *list, void *const data);

#endif
