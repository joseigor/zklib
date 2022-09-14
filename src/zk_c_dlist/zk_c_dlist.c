#include <stdlib.h>

#include "zk_c_dlist/zk_c_dlist.h"

// Private functions
static void _zk_c_dlist_free(zk_c_dlist **node, zk_destructor_t const func)
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

// Constructor
zk_status zk_c_dlist_new_node(zk_c_dlist **node_p, void *const data)
{
	if (node_p == NULL)
		return ZK_INVALID_ARGUMENT;

	*node_p = malloc(sizeof(zk_c_dlist));
	if (*node_p == NULL)
		return ZK_ERROR_ALLOC;

	(*node_p)->data = data;
	(*node_p)->prev = NULL;
	(*node_p)->next = NULL;

	return ZK_OK;
}

// Destructor
void zk_c_dlist_free(zk_c_dlist **list_p, zk_destructor_t const func)
{
	if (list_p != NULL && *list_p != NULL) {
		zk_c_dlist *current = zk_c_dlist_begin(*list_p);
		zk_c_dlist *end = zk_c_dlist_end(*list_p);
		while (current != end) {
			zk_c_dlist *node = current;
			current = current->next;
			_zk_c_dlist_free(&node, func);
		}
		// frees end node
		_zk_c_dlist_free(&end, func);
		*list_p = NULL;
	}
}

// Iterators
zk_c_dlist *zk_c_dlist_begin(zk_c_dlist *list)
{
	return list;
}

zk_c_dlist *zk_c_dlist_end(zk_c_dlist *list)
{
	return list != NULL ? list->prev : list;
}

void zk_c_dlist_for_each(zk_c_dlist *begin, zk_c_dlist *const end, zk_for_each_func const func, void *const user_data)
{
	if (func != NULL && begin != NULL) {
		for (; begin != end; begin = begin->next) {
			func(begin->data, user_data);
		}
		// calls func on last element
		func(end->data, user_data);
	}
}

// Modifiers

zk_status zk_c_dlist_pop_back(zk_c_dlist **list_p, zk_destructor_t const func)
{
	if (list_p == NULL)
		return ZK_INVALID_ARGUMENT;

	if ((*list_p) != NULL) {
		if ((*list_p) == (*list_p)->next) {
			// (*list_p) has only one element
			_zk_c_dlist_free(&(*list_p), func);
		} else {
			zk_c_dlist *back = (*list_p)->prev;
			back->prev->next = (*list_p);
			(*list_p)->prev = back->prev;
			_zk_c_dlist_free(&back, func);
		}
	}
	return ZK_OK;
}

zk_status zk_c_dlist_pop_front(zk_c_dlist **list_p, zk_destructor_t const func)
{
	if (list_p == NULL)
		return ZK_INVALID_ARGUMENT;

	if ((*list_p) != NULL) {
		if ((*list_p) == (*list_p)->next) {
			// (*list_p) has only one element
			_zk_c_dlist_free(&(*list_p), func);
		} else {
			zk_c_dlist *front = (*list_p);
			(*list_p) = (*list_p)->next;
			(*list_p)->prev = front->prev;
			front->prev->next = (*list_p);
			_zk_c_dlist_free(&front, func);
		}
	}
	return ZK_OK;
}

zk_status zk_c_dlist_push_back(zk_c_dlist **list_p, void *const data)
{
	if (list_p == NULL)
		return ZK_INVALID_ARGUMENT;

	zk_c_dlist *node = NULL;
	if (zk_c_dlist_new_node(&node, data) != ZK_OK)
		return ZK_ERROR_ALLOC;

	if ((*list_p) == NULL) {
		node->prev = node;
		node->next = node;
		(*list_p) = node;
	} else {
		(*list_p)->prev->next = node;
		node->prev = (*list_p)->prev;
		node->next = (*list_p);
		(*list_p)->prev = node;
	}

	return ZK_OK;
}

zk_status zk_c_dlist_push_front(zk_c_dlist **list_p, void *const data)
{
	if (list_p == NULL)
		return ZK_INVALID_ARGUMENT;

	zk_c_dlist *node = NULL;
	if (zk_c_dlist_new_node(&node, data) != ZK_OK)
		return ZK_ERROR_ALLOC;

	if ((*list_p) == NULL) {
		node->prev = node;
		node->next = node;
	} else {
		node->next = (*list_p);
		node->prev = (*list_p)->prev;
		(*list_p)->prev = node;
		node->prev->next = node;
	}

	(*list_p) = node;
	return ZK_OK;
}
