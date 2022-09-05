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
zk_status zk_dlist_new_node(zk_dlist **node_p, void *const data)
{
	if (node_p == NULL)
		return ZK_INVALID_ARGUMENT;

	*node_p = malloc(sizeof(zk_dlist));
	if (*node_p == NULL)
		return ZK_ERROR_ALLOC;

	(*node_p)->data = data;
	(*node_p)->prev = NULL;
	(*node_p)->next = NULL;

	return ZK_OK;
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

void zk_dlist_for_each(zk_dlist *begin, zk_dlist *const end, zk_for_each_func const func, void *const user_data)
{
	if (func != NULL) {
		for (; begin != end; begin = begin->next) {
			func(begin->data, user_data);
		}
	}
}

// Modifiers
zk_status zk_dlist_pop_back(zk_dlist **list_p, zk_destructor_t const func)
{
	if (list_p == NULL)
		return ZK_INVALID_ARGUMENT;

	if (*list_p != NULL) {
		if ((*list_p)->next == NULL) {
			// list has only one element
			_zk_dlist_free(&(*list_p), func);
		} else {
			zk_dlist *node = (*list_p);
			// moves node to element before last element
			while (node->next->next != NULL) {
				node = node->next;
			}
			// remove last element form the list
			_zk_dlist_free(&node->next, func);
			node->next = NULL;
		}
	}
	return ZK_OK;
}

zk_status zk_dlist_pop_front(zk_dlist **list_p, zk_destructor_t const func)
{
	if (list_p == NULL)
		return ZK_INVALID_ARGUMENT;

	if (*list_p != NULL) {
		if ((*list_p)->next == NULL) {
			// list has only one element
			_zk_dlist_free(&(*list_p), func);
		} else {
			zk_dlist *front_node = (*list_p);
			(*list_p) = (*list_p)->next;
			(*list_p)->prev = NULL;
			_zk_dlist_free(&front_node, func);
		}
	}

	return ZK_OK;
}

zk_status zk_dlist_push_back(zk_dlist **list_p, void *const data)
{
	if (list_p == NULL)
		return ZK_INVALID_ARGUMENT;

	zk_dlist *node = NULL;
	if (zk_dlist_new_node(&node, data) != ZK_OK)
		return ZK_ERROR_ALLOC;

	if (*list_p == NULL) {
		*list_p = node;
	} else {
		zk_dlist *tail = zk_dlist_back(*list_p);
		tail->next = node;
		node->prev = tail;
	}

	return ZK_OK;
}

zk_status zk_dlist_push_front(zk_dlist **list_p, void *const data)
{
	if (list_p == NULL)
		return ZK_INVALID_ARGUMENT;

	zk_dlist *node = NULL;
	if (zk_dlist_new_node(&node, data) != ZK_OK)
		return ZK_ERROR_ALLOC;

	if (*list_p != NULL) {
		node->next = (*list_p);
		(*list_p)->prev = node;
	}

	(*list_p) = node;
	return ZK_OK;
}
