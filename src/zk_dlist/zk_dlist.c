#include <stdlib.h>

#include "zk_dlist/zk_dlist.h"

// SECTION: Private functions
static zk_dlist *zk_dlist_back(zk_dlist *list)
{
	if (list != NULL) {
		while (list->next != NULL) {
			list = list->next;
		}
	}

	return list;
}

static void _zk_dlist_free(zk_dlist **node, zk_destructor_t func)
{
	if (node == NULL || *node == NULL) {
		return;
	}

	if (func != NULL) {
		func((*node)->data);
	}

	free(*node);
	*node = NULL;
}

// SECTION END: Private functions

// Constructor
zk_dlist *zk_dlist_new_node(void *const data)
{
	zk_dlist *node = malloc(sizeof(zk_dlist));
	node->data = data;
	node->prev = NULL;
	node->next = NULL;
	return node;
}

// Destructor
void zk_dlist_free(zk_dlist **list_p, zk_destructor_t const func)
{
	if (list_p != NULL) {
		while ((*list_p) != NULL) {
			zk_dlist *node = *list_p;
			*list_p = node->next;
			_zk_dlist_free(&node, func);
		}
	}
}

// Iterators
zk_dlist *zk_dlist_begin(zk_dlist *list)
{
	return list;
}

zk_dlist *zk_dlist_end(zk_dlist *list)
{
	ZK_UNUSED(list);
	return NULL;
}

void zk_dlist_for_each(zk_dlist *begin, zk_dlist *const end, zk_foreach_t const func, void *const user_data)
{
	if (func != NULL) {
		for (; begin != end; begin = begin->next) {
			func(begin->data, user_data);
		}
	}
}

// Modifiers
zk_dlist *zk_dlist_push_back(zk_dlist *list, void *const data)
{
	zk_dlist *node = zk_dlist_new_node(data);
	if (list == NULL) {
		list = node;
	} else {
		zk_dlist *tail = zk_dlist_back(list);
		tail->next = node;
		node->prev = tail;
	}
	return list;
}
