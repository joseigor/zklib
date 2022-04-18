#include <stddef.h>
#include <stdlib.h>

#include "zk_slist/zk_slist.h"

// SECTION: Private functions

static void _zk_slist_free(zk_slist_t **list_p, zk_destructor_t destructor)
{
	if (list_p == NULL || *list_p == NULL) {
		return;
	}

	// free all nodes and its data if destructor is passed by the user
	while (1) {
		zk_slist_t *node = *list_p;
		*list_p = (*list_p)->next;
		if (destructor != NULL) {
			destructor(node->data);
		}
		free(node);
		if ((*list_p) == NULL) {
			return;
		}
	}
}

static zk_slist_t *_zk_slist_new_node(void)
{
	zk_slist_t *list = malloc(sizeof(zk_slist_t));
	list->data = NULL;
	list->next = NULL;

	return list;
}

// SECTION END: Private functions

zk_slist_t *zk_slist_append(zk_slist_t *list, void *data)
{
	zk_slist_t *node = _zk_slist_new_node();
	node->data = data;
	node->next = NULL;

	if (list == NULL) {
		list = node;
		return list;
	}

	// add new node to end of the list
	for (zk_slist_t *node_i = list;; node_i = node_i->next) {
		// checks end of the list
		if (node_i->next == NULL) {
			node_i->next = node;
			break;
		}
	}

	return list;
}

zk_slist_t *zk_slist_concat(zk_slist_t *list_dest, zk_slist_t *list_src)
{
	if (list_dest == NULL || list_src == NULL) {
		return NULL;
	}

	for (zk_slist_t *node_i = list_dest;; node_i = node_i->next) {
		if (node_i->next == NULL) {
			node_i->next = list_src;
			break;
		}
	}

	return list_dest;
}

void zk_slist_free(zk_slist_t **list_p)
{
	_zk_slist_free(list_p, NULL);
}

void zk_slist_free_full(zk_slist_t **list_p, zk_destructor_t destructor)
{
	if (list_p == NULL || *list_p == NULL || destructor == NULL) {
		return;
	}

	_zk_slist_free(list_p, destructor);
}

zk_slist_t *zk_slist_prepend(zk_slist_t *list, void *data)
{
	zk_slist_t *node = _zk_slist_new_node();
	node->data = data;
	node->next = NULL;

	if (list == NULL) {
		list = node;
	} else {
		node->next = list;
		list = node;
	}

	return list;
}
