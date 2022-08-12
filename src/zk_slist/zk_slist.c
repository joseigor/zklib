#include <stddef.h>
#include <stdlib.h>

#include "zk_common/zk_utils.h"
#include "zk_slist/zk_slist.h"

// FIXME: check function where const apply to arguments, like append ...

// SECTION: Private functions

static zk_slist_t *_zk_slist_new_node(void)
{
	zk_slist_t *list = malloc(sizeof(zk_slist_t));
	list->data = NULL;
	list->next = NULL;

	return list;
}

static void _zk_slist_free_1(zk_slist_t **node)
{
	if (node == NULL || *node == NULL) {
		return;
	}
	free(*node);
	*node = NULL;
}

PRIVATE void _zk_slist_free_1_full(zk_slist_t **node, zk_destructor_t func)
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

zk_slist_t *zk_slist_delete_node(zk_slist_t *list, zk_slist_t *node, zk_destructor_t func)
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
		_zk_slist_free_1_full(&node, func);
		return list;
	}

	zk_slist_t *temp = list;
	while (temp != NULL) {
		if (temp->next == node) {
			temp->next = node->next;
			_zk_slist_free_1_full(&node, func);
			break;
		}
		temp = temp->next;
	}

	return list;
}

zk_slist_t *zk_slist_find_by_data(zk_slist_t *list, const void *const data)
{
	while (list != NULL) {
		if (list->data == data) {
			break;
		}
		list = list->next;
	}
	return list;
}

zk_slist_t *zk_slist_find_by_data_custom(zk_slist_t *list, const void *const data, zk_compare_t func)
{
	if (func == NULL) {
		return NULL;
	}

	while (list != NULL) {
		if (func(list->data, data) == 0) {
			break;
		}
		list = list->next;
	}
	return list;
}

void zk_slist_foreach(zk_slist_t *list, zk_foreach_t func, void *user_data)
{
	if (list == NULL || func == NULL) {
		return;
	}

	while (list != NULL) {
		func(list->data, user_data);
		list = list->next;
	}
}

void zk_slist_free(zk_slist_t **list_p)
{
	if (list_p == NULL || *list_p == NULL) {
		return;
	}

	while ((*list_p) != NULL) {
		zk_slist_t *node = *list_p;
		*list_p = (*list_p)->next;
		_zk_slist_free_1(&node);
	}
}

void zk_slist_free_full(zk_slist_t **list_p, zk_destructor_t func)
{
	if (list_p == NULL || *list_p == NULL || func == NULL) {
		return;
	}

	while ((*list_p) != NULL) {
		zk_slist_t *node = *list_p;
		*list_p = (*list_p)->next;
		_zk_slist_free_1_full(&node, func);
	}
}

int zk_slist_get_index(zk_slist_t *list, const void *const data)
{
	int index = 0;
	while (list != NULL) {
		if (list->data == data) {
			return index;
		}
		index++;
		list = list->next;
	}

	return -1;
}

zk_slist_t *zk_slist_insert(zk_slist_t *list, void *data, int position)
{
	if (position <= 0 || list == NULL) {
		list = zk_slist_prepend(list, data);
		return list;
	}

	zk_slist_t *temp = list;
	int index = 0;
	while (temp->next != NULL && index < position - 1) {
		temp = temp->next;
		index++;
	}
	temp->next = zk_slist_prepend(temp->next, data);

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

unsigned int zk_slist_length(zk_slist_t *list)
{
	unsigned int count = 0;

	while (list != NULL) {
		count++;
		list = list->next;
	}

	return count;
}

zk_slist_t *zk_slist_nth(zk_slist_t *list, unsigned int n)
{
	unsigned index = 0;
	while (list != NULL && index != n) {
		index++;
		list = list->next;
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
	if (list == NULL) {
		return NULL;
	}
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

zk_slist_t *zk_slist_sort(zk_slist_t *list, zk_compare_t func)
{
	if (list == NULL || func == NULL) {
		return NULL;
	}

	return list;
}
