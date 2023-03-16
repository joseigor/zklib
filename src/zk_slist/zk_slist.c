#include <stdlib.h>

#include "zk_slist/zk_slist.h"

/**
 * @brief Singly linked list struct
 *
 */
struct zk_slist {
	void *data;
	struct zk_slist *next;
};

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

// Element access
zk_status zk_slist_get_data(const zk_slist *const list, void **data)
{
	if (list == NULL)
		return ZK_INVALID_ARGUMENT;

	*data = list->data;

	return ZK_OK;
}

// Iterators
zk_status zk_slist_next(const zk_slist *const list, zk_slist **next)
{
	if (list == NULL)
		return ZK_INVALID_ARGUMENT;

	*next = list->next;

	return ZK_OK;
}

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

zk_status zk_slist_reverse(zk_slist **list_p)
{
	if (list_p == NULL || *list_p == NULL)
		return ZK_INVALID_ARGUMENT;

	zk_slist *prev = NULL;
	zk_slist *curr = *list_p;
	zk_slist *next = NULL;

	while (curr != NULL) {
		next = curr->next;
		curr->next = prev;
		prev = curr;
		curr = next;
	}

	*list_p = prev;

	return ZK_OK;
}

size_t zk_slist_size(const zk_slist *const list)
{
	size_t length = 0;
	for (const zk_slist *node = list; node != NULL; node = node->next) {
		length++;
	}
	return length;
}

static void zk_slist_split_left_right_tail(zk_slist **list_p, zk_slist **left_p, zk_slist **right_p, size_t l_r_size)
{
	zk_slist *end_node = NULL;
	*left_p = *list_p;
	*right_p = NULL;
	// loop through the list until l_r_size is reached to get the end of the left list
	for (size_t i = 0; i < l_r_size && *list_p != NULL; i++) {
		end_node = *list_p;
		*list_p = (*list_p)->next;
	}

	if (end_node != NULL)
		end_node->next = NULL;

	// loop through the list until l_r_size is reached to get the end of the right list
	*right_p = *list_p;
	for (size_t j = 0; j < l_r_size && *list_p != NULL; j++) {
		end_node = *list_p;
		*list_p = (*list_p)->next;
	}

	if (end_node != NULL)
		end_node->next = NULL;
}

static zk_status zk_slist_merge_and_set_tail(zk_slist **list_p, zk_slist **other_p, zk_compare_func const func)
{
	if (list_p == NULL || other_p == NULL || func == NULL)
		return ZK_INVALID_ARGUMENT;

	if (*list_p == NULL) {
		*list_p = *other_p;
		*other_p = NULL;
		return ZK_OK;
	}

	if (*other_p == NULL)
		return ZK_OK;

	zk_slist *head = NULL;
	zk_slist *tail = NULL;

	while (*list_p != NULL && *other_p != NULL) {
		if (func((*list_p)->data, (*other_p)->data) <= 0) {
			if (head == NULL) {
				head = *list_p;
				tail = *list_p;
			} else {
				tail->next = *list_p;
				tail = tail->next;
			}
			*list_p = (*list_p)->next;
		} else {
			if (head == NULL) {
				head = *other_p;
				tail = *other_p;
			} else {
				tail->next = *other_p;
				tail = tail->next;
			}
			*other_p = (*other_p)->next;
		}
	}

	if (*list_p != NULL) {
		tail->next = *list_p;
	} else if (*other_p != NULL) {
		tail->next = *other_p;
	}

	*list_p = head;
	// get last element from the list and set it to other_p, we start from tail->next to avoid a loop through the
	// whole list
	*other_p = zk_slist_last(tail->next);

	return ZK_OK;
}

zk_status zk_slist_sort(zk_slist **list_p, zk_compare_func const func)
{
	ZK_UNUSED(list_p);
	ZK_UNUSED(func);
	if (list_p == NULL || *list_p == NULL || func == NULL)
		return ZK_INVALID_ARGUMENT;

	const size_t length = zk_slist_size(*list_p);
	zk_slist *head = *list_p;

	// number of passes is log2(length)
	size_t p = 1;
	while ((p = p * 2) <= length) {
		zk_slist *next_head = head;
		zk_slist *head_tail = NULL;
		head = NULL; // reset head to NULL for the next pass
		while (next_head != NULL) {
			zk_slist *left = NULL;
			zk_slist *right = NULL;
			zk_slist_split_left_right_tail(&next_head, &left, &right, p / 2);
			// merge left and right, after this left will be the merged list and right will be the tail of
			// the merged list
			zk_slist_merge_and_set_tail(&left, &right, func);
			// set head to left if it is the first pass
			if (head == NULL) {
				head = left;
				head_tail = right;
			} else {
				// append left to the end of the previous merged list
				head_tail->next = left;
				head_tail = right;
			}
		}
	}
	// if the number of passes is odd, we need to merge the last two lists
	if (length != p) {
		zk_slist *left = NULL;
		zk_slist *right = NULL;
		zk_slist_split_left_right_tail(&head, &left, &right, p / 2);
		zk_slist_merge(&left, &right, func);
		head = left;
	}
	*list_p = head;
	return ZK_OK;
}

zk_status zk_slist_merge(zk_slist **list_p, zk_slist **other_p, zk_compare_func const func)
{
	if (list_p == NULL || other_p == NULL || func == NULL)
		return ZK_INVALID_ARGUMENT;

	if (*list_p == NULL) {
		*list_p = *other_p;
		*other_p = NULL;
		return ZK_OK;
	}

	if (*other_p == NULL)
		return ZK_OK;

	zk_slist *head = NULL;
	zk_slist *tail = NULL;

	while (*list_p != NULL && *other_p != NULL) {
		if (func((*list_p)->data, (*other_p)->data) <= 0) {
			if (head == NULL) {
				head = *list_p;
				tail = *list_p;
			} else {
				tail->next = *list_p;
				tail = tail->next;
			}
			*list_p = (*list_p)->next;
		} else {
			if (head == NULL) {
				head = *other_p;
				tail = *other_p;
			} else {
				tail->next = *other_p;
				tail = tail->next;
			}
			*other_p = (*other_p)->next;
		}
	}

	if (*list_p != NULL) {
		tail->next = *list_p;
	} else if (*other_p != NULL) {
		tail->next = *other_p;
	}

	*list_p = head;
	*other_p = NULL;

	return ZK_OK;
}
