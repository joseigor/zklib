#include <stdlib.h>

#include "zk_slist/zk_slist.h"

// Private functions
static void _zk_slist_free(zk_slist **node, zk_destructor_t func)
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

static zk_slist *zk_slist_last(zk_slist *list)
{
	while (list != NULL && list->next != NULL) {
		list = list->next;
	}

	return list;
}

// Constructor
zk_status zk_slist_new_node(zk_slist **node_p, void *const data)
{
	if (node_p == NULL)
		return ZK_INVALID_ARGUMENT;

	*node_p = malloc(sizeof(zk_slist));
	if (*node_p == NULL)
		return ZK_ERROR_ALLOC;

	(*node_p)->data = data;
	(*node_p)->next = NULL;

	return ZK_OK;
}

// Destructor
void zk_slist_free(zk_slist **list_p, zk_destructor_t const func)
{
	if (list_p != NULL) {
		while ((*list_p) != NULL) {
			zk_slist *node = *list_p;
			*list_p = node->next;
			_zk_slist_free(&node, func);
		}
	}
}

// Iterators
zk_slist *zk_slist_begin(zk_slist *list)
{
	return list;
}

zk_slist *zk_slist_end(zk_slist *list)
{
	ZK_UNUSED(list);
	return NULL;
}

void zk_slist_for_each(zk_slist *begin, zk_slist *const end, zk_for_each_func const func, void *const user_data)
{
	if (func != NULL) {
		for (; begin != end; begin = begin->next) {
			func(begin->data, user_data);
		}
	}
}

// Modifiers
zk_status zk_slist_pop_back(zk_slist **list_p, zk_destructor_t const func)
{
	if (list_p == NULL)
		return ZK_INVALID_ARGUMENT;

	if ((*list_p) != NULL) {
		// (*list_p) has only one element
		if ((*list_p)->next == NULL) {
			_zk_slist_free(&(*list_p), func);
		} else {
			zk_slist *node = (*list_p);
			// moves node to element before last element
			while (node->next->next != NULL) {
				node = node->next;
			}
			// remove last element form the (*list_p)
			_zk_slist_free(&node->next, func);
			node->next = NULL;
		}
	}
	return ZK_OK;
}

zk_status zk_slist_pop_front(zk_slist **list_p, zk_destructor_t const func)
{
	if (list_p == NULL)
		return ZK_INVALID_ARGUMENT;

	if ((*list_p) != NULL) {
		zk_slist *front_node = (*list_p);
		(*list_p) = (*list_p)->next;
		_zk_slist_free(&front_node, func);
	}
	return ZK_OK;
}

zk_status zk_slist_push_back(zk_slist **list_p, void *const data)
{
	if (list_p == NULL)
		return ZK_INVALID_ARGUMENT;

	zk_slist *node = NULL;
	if (zk_slist_new_node(&node, data) != ZK_OK)
		return ZK_ERROR_ALLOC;

	if ((*list_p) == NULL) {
		(*list_p) = node;
	} else {
		zk_slist *last_node = zk_slist_last((*list_p));
		last_node->next = node;
	}

	return ZK_OK;
}

zk_status zk_slist_push_front(zk_slist **list_p, void *const data)
{
	if (list_p == NULL)
		return ZK_INVALID_ARGUMENT;

	zk_slist *node = NULL;
	if (zk_slist_new_node(&node, data) != ZK_OK)
		return ZK_ERROR_ALLOC;

	if ((*list_p) != NULL)
		node->next = (*list_p);

	(*list_p) = node;
	return ZK_OK;
}
