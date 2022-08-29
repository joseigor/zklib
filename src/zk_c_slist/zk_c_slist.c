#include <stdlib.h>

#include "zk_c_slist/zk_c_slist.h"

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
zk_c_slist *zk_c_slist_new_node(void *const data)
{
	zk_c_slist *list = malloc(sizeof(zk_c_slist));
	list->data = data;
	list->next = NULL;

	return list;
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

// Iterators
zk_c_slist *zk_c_slist_begin(zk_c_slist *list)
{
	return list != NULL ? list->next : list;
}

zk_c_slist *zk_c_slist_end(zk_c_slist *list)
{
	return list;
}

void zk_c_slist_for_each(zk_c_slist *begin, zk_c_slist *const end, zk_foreach_t const func, void *const user_data)
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
zk_c_slist *zk_c_slist_push_back(zk_c_slist *list, void *const data)
{
	zk_c_slist *node = zk_c_slist_new_node(data);

	if (list == NULL) {
		node->next = node;
	} else {
		node->next = list->next;
		list->next = node;
	}

	return node;
}