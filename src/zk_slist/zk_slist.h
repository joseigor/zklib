#pragma once

// TODO: move to private?

typedef struct _zklib_slist_node {
	void *data;
	struct _zklib_slist_node *next;
} zklib_slist_node_t;

typedef struct _zk_slist {
	zklib_slist_node_t *head;
	zklib_slist_node_t *tail;
} zk_slist_t;

// TODO: add comments and description for all methods
// TODO: generate documentation from the code comments

zk_slist_t *zk_slist_append(zk_slist_t *list, void *data);

zk_slist_t *zk_slist_free(zk_slist_t *list);

zk_slist_t *zk_slist_free_full(zk_slist_t *list);

zk_slist_t *zk_slist_insert(zk_slist_t *list, int position);

unsigned int zk_slist_lenght(zk_slist_t *list);

zk_slist_t *zk_slist_prepend(zk_slist_t *list, void *data);

zk_slist_t *zk_slist_new(void); // TODO: do we need this methodin the api?
