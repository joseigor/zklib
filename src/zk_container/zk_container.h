#ifndef ZK_CONTAINER_H
#define ZK_CONTAINER_H

#include <assert.h>

#include "zk_slist/zk_slist.h"

#define zk_push_back(CONTAINER, DATA) _Generic((CONTAINER), zk_slist_t * : zk_slist_push_back)(CONTAINER, DATA)

#endif
