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
zk_slist *zk_slist_new_node(void *const data)
{
	zk_slist *list = malloc(sizeof(zk_slist));
	list->data = data;
	list->next = NULL;

	return list;
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
zk_slist *zk_slist_pop_back(zk_slist *list, zk_destructor_t const func)
{
	if (list != NULL) {
		// list has only one element
		if (list->next == NULL) {
			_zk_slist_free(&list, func);
		} else {
			zk_slist *node = list;
			// moves node to element before last element
			while (node->next->next != NULL) {
				node = node->next;
			}
			// remove last element form the list
			_zk_slist_free(&node->next, func);
			node->next = NULL;
		}
	}
	return list;
}

zk_slist *zk_slist_pop_front(zk_slist *list, zk_destructor_t const func)
{
	if (list != NULL) {
		zk_slist *front_node = list;
		list = list->next;
		_zk_slist_free(&front_node, func);
	}
	return list;
}

zk_slist *zk_slist_push_back(zk_slist *list, void *const data)
{
	zk_slist *node = zk_slist_new_node(data);

	if (list == NULL) {
		list = node;
	} else {
		zk_slist *last_node = zk_slist_last(list);
		last_node->next = node;
	}

	return list;
}

zk_slist *zk_slist_push_front(zk_slist *list, void *data)
{
	zk_slist *node = zk_slist_new_node(data);

	if (list == NULL) {
		list = node;
	} else {
		node->next = list;
		list = node;
	}

	return list;
}
