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
zk_c_dlist *zk_c_dlist_new_node(void *const data)
{
	zk_c_dlist *list = malloc(sizeof(zk_c_dlist));
	list->data = data;
	list->prev = NULL;
	list->next = NULL;

	return list;
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

zk_c_dlist *zk_c_dlist_pop_back(zk_c_dlist *list, zk_destructor_t const func)
{
	if (list != NULL) {
		if (list == list->next) {
			// list has only one element
			_zk_c_dlist_free(&list, func);
		} else {
			zk_c_dlist *back = list->prev;
			back->prev->next = list;
			list->prev = back->prev;
			_zk_c_dlist_free(&back, func);
		}
	}
	return list;
}

zk_c_dlist *zk_c_dlist_pop_front(zk_c_dlist *list, zk_destructor_t const func)
{
	if (list != NULL) {
		if (list == list->next) {
			// list has only one element
			_zk_c_dlist_free(&list, func);
		} else {
			zk_c_dlist *front = list;
			list = list->next;
			list->prev = front->prev;
			front->prev->next = list;
			_zk_c_dlist_free(&front, func);
		}
	}
	return list;
}

zk_c_dlist *zk_c_dlist_push_back(zk_c_dlist *list, void *const data)
{
	zk_c_dlist *node = zk_c_dlist_new_node(data);

	if (list == NULL) {
		node->prev = node;
		node->next = node;
		list = node;
	} else {
		list->prev->next = node;
		node->prev = list->prev;
		node->next = list;
		list->prev = node;
	}

	return list;
}

zk_c_dlist *zk_c_dlist_push_front(zk_c_dlist *list, void *const data)
{
	zk_c_dlist *node = zk_c_dlist_new_node(data);

	if (list == NULL) {
		node->prev = node;
		node->next = node;

	} else {
		node->next = list;
		node->prev = list->prev;
		list->prev = node;
		node->prev->next = node;
	}

	return node;
}
