#pragma once

#include "zk_common/zk_types.h"

typedef struct _zk_slist_t {
	void *data;
	struct _zk_slist_t *next;
} zk_slist_t;

// TODO: add comments and description for all methods
// TODO: generate documentation from the code comments

zk_slist_t *zk_slist_append(zk_slist_t *list, void *data);

zk_slist_t *zk_slist_concat(zk_slist_t *list_dest, zk_slist_t *list_src);

zk_slist_t *zk_slist_copy(const zk_slist_t *list);

zk_slist_t *zk_slist_copy_deep(const zk_slist_t *list, zk_copy_data_t func, void *user_data);

void zk_slist_free(zk_slist_t **list_p);

void zk_slist_free_full(zk_slist_t **list_p, zk_destructor_t destructor);

zk_slist_t *zk_slist_prepend(zk_slist_t *list, void *data);
