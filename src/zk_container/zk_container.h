#ifndef ZK_CONTAINER_H
#define ZK_CONTAINER_H

#include "zk_c_slist/zk_c_slist.h"
#include "zk_dlist/zk_dlist.h"
#include "zk_slist/zk_slist.h"

// clang-format off

// Iterators
#define zk_for_each(CONTAINER, FUNC, USER_DATA)     \
	_Generic((CONTAINER),                       \
		zk_slist * : zk_slist_for_each,     \
		zk_dlist * : zk_dlist_for_each,     \
		zk_c_slist * : zk_c_slist_for_each) \
		(zk_begin(CONTAINER), zk_end(CONTAINER), FUNC, USER_DATA)

#define zk_begin(CONTAINER)                      \
	_Generic((CONTAINER),                    \
		zk_slist * : zk_slist_begin,     \
		zk_dlist * : zk_dlist_begin,     \
		zk_c_slist * : zk_c_slist_begin) \
		(CONTAINER)

#define zk_end(CONTAINER)                      \
	_Generic((CONTAINER),                  \
		zk_slist * : zk_slist_end,     \
		zk_dlist * : zk_dlist_end,     \
		zk_c_slist * : zk_c_slist_end) \
		(CONTAINER)

// Modifiers
#define zk_push_back(CONTAINER, DATA)                \
	_Generic((CONTAINER),                        \
		zk_slist *   : zk_slist_push_back,   \
		zk_dlist *   : zk_dlist_push_back,   \
		zk_c_slist * : zk_c_slist_push_back) \
		(CONTAINER, DATA)

#define zk_free(CONTAINER, FUNC)                  \
	_Generic((CONTAINER),                     \
		zk_slist * * : zk_slist_free,     \
		zk_dlist * * : zk_dlist_free,     \
		zk_c_slist * * : zk_c_slist_free) \
		(CONTAINER, FUNC)

// clang-format on
#endif
