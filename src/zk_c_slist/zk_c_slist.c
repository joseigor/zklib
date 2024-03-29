#include <stdlib.h>

#include "zk_c_slist/zk_c_slist.h"

/**
 * @brief Circular singly linked list struct
 * Internally this list is managed in a way that it always points to the last node for efficient operations.
 */

struct zk_c_slist {
	void *data;
	struct zk_c_slist *next;
};

// Private functions
static void _zk_c_slist_free(zk_c_slist **node, zk_destructor_t const func)
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
zk_status zk_c_slist_new_node(zk_c_slist **node_p, void *const data)
{
	if (node_p == NULL)
		return ZK_INVALID_ARGUMENT;

	*node_p = malloc(sizeof(zk_c_slist));
	if (*node_p == NULL)
		return ZK_ERROR_ALLOC;

	(*node_p)->data = data;
	(*node_p)->next = NULL;

	return ZK_OK;
}

// Destructor
void zk_c_slist_free(zk_c_slist **list_p, zk_destructor_t const func)
{
	if (list_p != NULL && *list_p != NULL) {
		zk_c_slist *current = zk_c_slist_begin(*list_p);
		zk_c_slist *end = zk_c_slist_end(*list_p);
		while (current != end) {
			zk_c_slist *node = current;
			current = current->next;
			_zk_c_slist_free(&node, func);
		}
		// frees end node
		_zk_c_slist_free(&end, func);
		*list_p = NULL;
	}
}

// Element access
zk_status zk_c_slist_get_data(const zk_c_slist *const list, void **data)
{
	if (list == NULL)
		return ZK_INVALID_ARGUMENT;

	*data = list->data;

	return ZK_OK;
}

// Iterators
zk_status zk_c_slist_next(const zk_c_slist *const list, zk_c_slist **next)
{
	if (list == NULL)
		return ZK_INVALID_ARGUMENT;

	*next = list->next;

	return ZK_OK;
}

zk_c_slist *zk_c_slist_begin(zk_c_slist *list)
{
	return list != NULL ? list->next : list;
}

zk_c_slist *zk_c_slist_end(zk_c_slist *list)
{
	return list;
}

void zk_c_slist_for_each(zk_c_slist *begin, zk_c_slist *const end, zk_for_each_func const func, void *const user_data)
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

zk_status zk_c_slist_pop_back(zk_c_slist **list_p, zk_destructor_t const func)
{
	if (list_p == NULL)
		return ZK_INVALID_ARGUMENT;

	if (*list_p != NULL) {
		if (*list_p == (*list_p)->next) {
			// list has only one element
			_zk_c_slist_free(&(*list_p), func);
		} else {
			zk_c_slist *node = (*list_p)->next; // 1st element for the list
			// loop until node is the element previous to the last element
			while (node->next != (*list_p)) {
				node = node->next;
			}
			// removes last element and set list as the new last element.
			node->next = (*list_p)->next;
			_zk_c_slist_free(&(*list_p), func);
			(*list_p) = node;
		}
	}
	return ZK_OK;
}

zk_status zk_c_slist_pop_front(zk_c_slist **list_p, zk_destructor_t const func)
{
	if (list_p == NULL)
		return ZK_INVALID_ARGUMENT;

	if ((*list_p) != NULL) {
		if ((*list_p) == (*list_p)->next) {
			// list has only one element
			_zk_c_slist_free(&(*list_p), func);
		} else {
			zk_c_slist *front_node = (*list_p)->next;
			(*list_p)->next = (*list_p)->next->next;
			_zk_c_slist_free(&front_node, func);
		}
	}
	return ZK_OK;
}

zk_status zk_c_slist_push_back(zk_c_slist **list_p, void *const data)
{
	if (list_p == NULL)
		return ZK_INVALID_ARGUMENT;

	zk_c_slist *node = NULL;
	if (zk_c_slist_new_node(&node, data) != ZK_OK)
		return ZK_ERROR_ALLOC;

	if ((*list_p) == NULL) {
		node->next = node;
	} else {
		node->next = (*list_p)->next;
		(*list_p)->next = node;
	}
	(*list_p) = node;

	return ZK_OK;
}

zk_status zk_c_slist_push_front(zk_c_slist **list_p, void *const data)
{
	if (list_p == NULL)
		return ZK_INVALID_ARGUMENT;

	zk_c_slist *node = NULL;
	if (zk_c_slist_new_node(&node, data) != ZK_OK)
		return ZK_ERROR_ALLOC;

	if ((*list_p) == NULL) {
		node->next = node;
		(*list_p) = node;
	} else {
		node->next = (*list_p)->next;
		(*list_p)->next = node;
	}
	return ZK_OK;
}
