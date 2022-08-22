#include <stdlib.h>

#include "zk_slist/zk_slist.h"

// SECTION: Private functions

static zk_slist_t *_zk_slist_new_node(void)
{
	zk_slist_t *list = malloc(sizeof(zk_slist_t));
	list->data = NULL;
	list->next = NULL;

	return list;
}

static void _zk_slist_free(zk_slist_t **node, zk_destructor_t func)
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

static int _zk_slist_compare(const void *const node_data, const void *const data, zk_compare_t const func)
{
	return (func != NULL ? func(node_data, data) : !(node_data == data));
}

void _zk_slist_front_back_split(zk_slist_t *list, zk_slist_t **front, zk_slist_t **back)
{
	if (list == NULL) {
		*front = NULL;
		*back = NULL;
		return;
	}
	zk_slist_t *slow = list;
	zk_slist_t *fast = list->next;

	while (fast != NULL && fast->next != NULL) {
		slow = slow->next;
		fast = fast->next->next;
	}

	*front = list;
	*back = slow->next;
	slow->next = NULL;
}

// SECTION END: Private functions

zk_slist_t *zk_slist_append(zk_slist_t *list, void *const data)
{
	zk_slist_t *node = _zk_slist_new_node();
	node->data = data;

	if (list == NULL) {
		list = node;
	} else {
		zk_slist_t *last_node = zk_slist_last(list);
		last_node->next = node;
	}

	return list;
}

zk_slist_t *zk_slist_concat(zk_slist_t *const list_dest, zk_slist_t *const list_src)
{
	if (list_dest == NULL) {
		return NULL;
	}

	zk_slist_t *last_node = zk_slist_last(list_dest);
	last_node->next = list_src;

	return list_dest;
}

zk_slist_t *zk_slist_copy(const zk_slist_t *list)
{
	if (list == NULL) {
		return NULL;
	}

	zk_slist_t *cp = NULL;
	while (list != NULL) {
		cp = zk_slist_prepend(cp, list->data);
		list = list->next;
	}
	cp = zk_slist_reverse(cp);
	return cp;
}

zk_slist_t *zk_slist_copy_deep(const zk_slist_t *list, zk_copy_data_t func, void *user_data)
{
	if (list == NULL || func == NULL) {
		return NULL;
	}

	zk_slist_t *cp = NULL;
	while (list != NULL) {
		void *data = func(list->data, user_data);
		cp = zk_slist_prepend(cp, data);
		list = list->next;
	}
	cp = zk_slist_reverse(cp);
	return cp;
}

zk_slist_t *zk_slist_delete_node(zk_slist_t *list, zk_slist_t *node, zk_destructor_t const func)
{
	if (list == NULL) {
		return NULL;
	}

	if (node == NULL) {
		return list;
	}

	// node is head of the list
	if (list == node) {
		list = list->next;
		_zk_slist_free(&node, func);
		return list;
	}

	zk_slist_t *temp = list;
	while (temp->next != NULL) {
		if (temp->next == node) {
			temp->next = node->next;
			_zk_slist_free(&node, func);
			break;
		}
		temp = temp->next;
	}

	return list;
}

zk_slist_t *zk_slist_find(zk_slist_t *list, const void *const data, zk_compare_t const func)
{
	while (list != NULL) {
		if (_zk_slist_compare(list->data, data, func) == 0) {
			break;
		}
		list = list->next;
	}

	return list;
}

void zk_slist_foreach(zk_slist_t *list, zk_foreach_t func, void *user_data)
{
	if (func == NULL) {
		return;
	}

	while (list != NULL) {
		func(list->data, user_data);
		list = list->next;
	}
}

void zk_slist_free(zk_slist_t **list_p, zk_destructor_t const func)
{
	if (list_p == NULL) {
		return;
	}

	while ((*list_p) != NULL) {
		zk_slist_t *node = *list_p;
		*list_p = (*list_p)->next;
		_zk_slist_free(&node, func);
	}
}

size_t zk_slist_index(zk_slist_t *list, const void *const data, zk_compare_t const func)
{
	size_t index = 1;
	while (list != NULL) {
		if (_zk_slist_compare(list->data, data, func) == 0) {
			return index;
		}
		index++;
		list = list->next;
	}
	return 0;
}

zk_slist_t *zk_slist_insert(zk_slist_t *list, void *data, size_t position)
{
	if (position < 1) {
		list = zk_slist_append(list, data);
	} else if (position == 1) {
		list = zk_slist_prepend(list, data);
	} else {
		zk_slist_t *temp = list;
		size_t index = 1;
		while (temp->next != NULL && index < position - 1) {
			temp = temp->next;
			index++;
		}
		temp->next = zk_slist_prepend(temp->next, data);
	}

	return list;
}

zk_slist_t *zk_slist_insert_before(zk_slist_t *list, zk_slist_t *sibling, void *data)
{
	if (list == NULL) {
		return NULL;
	}

	// sibling is the head of the list
	if (list == sibling) {
		list = zk_slist_prepend(list, data);
		return list;
	}

	// if sibling == NULL or is not part of the list, a new link with data is appended to list
	zk_slist_t *temp = list;
	while (temp->next != sibling && temp->next != NULL) {
		temp = temp->next;
	}

	temp->next = zk_slist_prepend(temp->next, data);

	return list;
}

zk_slist_t *zk_slist_last(zk_slist_t *list)
{
	while (list != NULL && list->next != NULL) {
		list = list->next;
	}

	return list;
}

size_t zk_slist_length(zk_slist_t *list)
{
	size_t count = 0;

	while (list != NULL) {
		count++;
		list = list->next;
	}

	return count;
}

zk_slist_t *zk_slist_nth(zk_slist_t *list, size_t n)
{
	if (n == 0) {
		list = zk_slist_last(list);
	} else {
		size_t index = 1;
		while (list != NULL && list->next != NULL && index != n) {
			index++;
			list = list->next;
		}
	}
	return list;
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

zk_slist_t *zk_slist_reverse(zk_slist_t *list)
{
	zk_slist_t *prev = NULL;
	while (list != NULL) {
		zk_slist_t *next = list->next;
		list->next = prev;
		prev = list;
		// end of the list reached
		if (next == NULL) {
			break;
		}
		list = next;
	}
	return list;
}
