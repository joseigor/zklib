#include <stdlib.h>

#include "zk_slist/zk_slist.h"

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
	while (list != NULL && list->next != NULL)
		list = list->next;

	return list;
}

/**
 * @brief Creates a new node with data.
 *
 * @param data Pointer to the data to be stored in the node. Caller is responsible for the memory management of the
 * data.
 *
 * @return Pointer to the new node or NULL if function fails. This function can only fail in case of memory allocation
 *
 * @note Time complexity: O(1)
 * @note Space complexity: O(1)
 */
zk_slist *zk_slist_new_node(void *const data)
{
	zk_slist *node = malloc(sizeof(zk_slist));
	if (!node)
		return NULL;

	node->data = data;
	node->next = NULL;
	return node;
}

/**
 * @brief Frees the list and its nodes if `func` is provided.
 *
 * @param list_p Pointer to the list. It is set to NULL after the list is freed.
 * @param func Pointer to the destructor function. If NULL, the data is not freed.
 *
 * @note Time complexity: O(n)
 * @note Space complexity: O(1)
 */
void zk_slist_free(zk_slist **list_p, zk_destructor_t const func)
{
	if (list_p) {
		while (*list_p) {
			zk_slist *node = *list_p;
			*list_p = node->next;
			_zk_slist_free(&node, func);
		}
	}
}

zk_status zk_slist_next(const zk_slist *const list, zk_slist **next)
{
	if (list == NULL)
		return ZK_INVALID_ARGUMENT;

	*next = list->next;

	return ZK_OK;
}

/**
 * @brief Returns an iterator to the first element of the list.
 *
 * @param list Pointer to the list.
 * @return Iterator to the first element of the list.
 *
 * @note Time complexity: O(1)
 * @note Space complexity: O(1)
 */
zk_slist *zk_slist_begin(zk_slist *list)
{
	return list;
}

/**
 * @brief Returns an iterator to the element following the last element of the list.
 *
 * @param list Pointer to the list.
 * @return Iterator to the element following the last element of the list. For single linked list, this is always NULL.
 *
 * @note Time complexity: O(1)
 * @note Space complexity: O(1)
 */
zk_slist *zk_slist_end(zk_slist *list)
{
	ZK_UNUSED(list);
	return NULL;
}

/**
 * @brief Applies the given function to each element in the list.
 *
 * @param begin Iterator to the first element of the list.
 * @param end Iterator to the element following the last element of the list.
 * @param func Pointer to the function to be applied to each element.
 * @param user_data Pointer to user data to be passed to the function. Can be NULL.
 *
 * @note Time complexity: O(n)
 * @note Space complexity: O(1)
 */
void zk_slist_for_each(zk_slist *begin, zk_slist *const end, zk_for_each_func const func, void *const user_data)
{
	if (func) {
		while (begin != end) {
			func(begin->data, user_data);
			begin = begin->next;
		}
	}
}

/**
 * @brief Removes the last element from the list.
 *
 * @param list Pointer to the list.
 * @param func Pointer to the destructor function. If NULL, the data is not freed.
 *
 * @return Pointer to the new head of the list.
 *
 * @note Time complexity: O(n)
 * @note Space complexity: O(1)
 */
zk_slist *zk_slist_pop_back(zk_slist *list, zk_destructor_t const func)
{
	if (list) {
		// list has only one element
		if (list->next == NULL)
			_zk_slist_free(&list, func);
		else {
			zk_slist *node = list;
			// moves node to element before last element
			while (node->next->next != NULL)
				node = node->next;

			// remove last element from the list
			_zk_slist_free(&node->next, func);
			node->next = NULL;
		}
	}
	return list;
}

/**
 * @brief Removes the first element from the list.
 *
 * @param list Pointer to the list.
 * @param func Pointer to the destructor function. If NULL, the data is not freed.
 *
 * @return Pointer to the new head of the list.
 *
 * @note Time complexity: O(1)
 * @note Space complexity: O(1)
 */
zk_slist *zk_slist_pop_front(zk_slist *list, zk_destructor_t const func)
{
	if (list) {
		zk_slist *old_head = list;
		list = list->next;
		_zk_slist_free(&old_head, func);
	}
	return list;
}

/**
 * @brief Appends new node with data to the list.
 *
 * @param list Pointer to the list.
 * @param data Pointer to the data to be appended. Caller is responsible for the memory management of the data.
 *
 * @return Pointer to the new head of the list or NULL if function fails. This function can only fail in case of memory
 * allocation failure.
 *
 * @note Time complexity: O(n)
 * @note Space complexity: O(1)
 */
zk_slist *zk_slist_push_back(zk_slist *list, void *const data)
{
	zk_slist *tail = zk_slist_new_node(data);
	if (tail == NULL)
		return NULL;

	if (list)
		zk_slist_last(list)->next = tail;
	else
		list = tail;

	return list;
}

/**
 * @brief Prepends new node with data to the list.
 *
 * @param list Pointer to the list.
 * @param data Pointer to the data to be prepended. Caller is responsible for the memory management of the data.
 *
 * @return Pointer to the new head of the list or NULL if function fails. This function can only fail in case of memory
 * allocation failure.
 *
 * @note Time complexity: O(1)
 * @note Space complexity: O(1)
 */
zk_slist *zk_slist_push_front(zk_slist *list, void *const data)
{
	zk_slist *head = zk_slist_new_node(data);
	if (head == NULL)
		return NULL;

	head->next = list;
	return head;
}

/**
 * @brief Reverses the order of the elements in the list.
 *
 * @param list Pointer to the list to reverse.
 *
 * @return Pointer to the reversed list.
 *
 * @note Time complexity: O(n)
 * @note Space complexity: O(1)
 */
zk_slist *zk_slist_reverse(zk_slist *list)
{
	zk_slist *prev = NULL;
	zk_slist *next = NULL;
	while (list != NULL) {
		next = list->next;
		list->next = prev;
		prev = list;
		list = next;
	}
	return prev;
}

/**
 * @brief Returns the number of elements in the list.
 *
 * @param list Pointer to the list.
 *
 * @return The number of elements in the list.
 * @return 0 if list is NULL.
 *
 * @note Time complexity: O(n)
 * @note Space complexity: O(1)
 */
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
		// TODO: check if we can start merging from here instead of going through the list again
		// maybe we can we can iterate the left list and the right list at the same time
		end_node = *list_p;
		*list_p = (*list_p)->next;
	}

	if (end_node != NULL)
		end_node->next = NULL;
}

/**
 * @brief Sorts list in ascending order if func(a, b) <= 0 and in descending order if func(a, b) > 0.
 *        The original list is invalid after the sort as it is sorted in place.
 *
 * @param list Pointer to the list to sort.
 * @param func Pointer to the comparison function. The comparison function must return a negative value if a < b, 0 if
 *             a == b, and a positive value if a > b. If `NULL`, the list is returned unsorted.
 *
 * @return Pointer to the sorted list.
 *
 * @note Time complexity: O(n log n), it uses iterative merge sort algorithm.
 * @note Space complexity: O(1)
 * @note This sort algorithm is stable.
 * @note This sort algorithm is in-place.
 */
zk_slist *zk_slist_sort(zk_slist *list, zk_compare_func const func)
{
	if (func == NULL)
		return list;

	const size_t length = zk_slist_size(list);
	zk_slist *head = list;

	// number of passes is log2(length)
	size_t p = 1;
	while ((p = p * 2) <= length) {
		zk_slist *next_head = head;
		zk_slist *head_tail = NULL;
		head = NULL; // reset head to NULL for the next pass
		while (next_head != NULL) {
			zk_slist *left = next_head;
			zk_slist *left_end = left;

			// loop p/2 to get left list and set its last node to null
			for (size_t i = 0; i < p / 2 && next_head != NULL; i++) {
				left_end = next_head;
				next_head = next_head->next;
			}
			// turn left in a NULL terminated list.
			left_end->next = NULL;

			// loop again p/2 but this time also do merge and sort.
			zk_slist *merged = NULL;
			zk_slist *merged_tail = NULL;
			size_t i = 0;
			while (i < p / 2 && next_head != NULL && left != NULL) {
				if (func(left->data, next_head->data) <= 0) {
					if (merged == NULL) {
						merged = left;
						merged_tail = left;
					} else {
						merged_tail->next = left;
						merged_tail = left;
					}
					left = left->next;
				} else {
					if (merged == NULL) {
						merged = next_head;
						merged_tail = next_head;
					} else {
						merged_tail->next = next_head;
						merged_tail = next_head;
					}
					next_head = (next_head)->next;
					i++;
				}
			}

			// check if left node remains else right nodes
			if (left != NULL) {
				// list only have left side then initialize merged to left
				if (merged == NULL) {
					merged = left;
					merged_tail = left;
					left = left->next; // as merge tail point to left, increment left
				}
				while (left != NULL) {
					merged_tail->next = left;
					left = left->next;
					merged_tail = merged_tail->next;
				}
			} else {
				while (i < p / 2 && next_head != NULL) {
					merged_tail->next = next_head;
					next_head = next_head->next;
					merged_tail = merged_tail->next;
					i++;
				}
			}

			// set head to left if it is the first pass
			if (head == NULL) {
				head = merged;
				head_tail = merged_tail;
			} else {
				// append left to the end of the previous merged list
				head_tail->next = merged;
				head_tail = merged_tail;
			}
		}
	}
	// if the number of passes is odd, we need to merge the last two lists
	if (length != p) {
		zk_slist *left = NULL;
		zk_slist *right = NULL;
		zk_slist_split_left_right_tail(&head, &left, &right, p / 2);
		left = zk_slist_merge(left, right, func);
		head = left;
	}

	return head;
}

/**
 * @brief Merges two sorted lists. Merges in ascending order if func(a, b) <= 0 and  in descending order if func(a, b) >
 *        0. As the merge happens in place, first and second lists are invalid after the merge as they are merged into
 *        a the list that is returned by the function.
 *
 * @param list Pointer to the first list.
 * @param other Pointer to the second list.
 * @param func Pointer to the comparison function. Must not be NULL.
 *             The comparison function must return a negative value if a < b, 0 if a == b, and a positive value if a >
 *             b.
 *
 * @return Pointer to the merged list or `NULL` if function fails.
 *
 * @note Time complexity: O(n), where n is the number of elements in the bigger list.
 * @note Space complexity: O(1)
 * @note This merge algorithm is stable.
 * @note This merge algorithm is in-place.
 */
zk_slist *zk_slist_merge(zk_slist *list, zk_slist *other, zk_compare_func const func)
{
	if (func == NULL)
		return NULL;

	if (list == NULL) {
		list = other;
		other = NULL;
		return list;
	}

	if (other == NULL)
		return list;

	zk_slist *head = NULL;
	zk_slist *tail = NULL;

	while (list != NULL && other != NULL) {
		if (func((list)->data, (other)->data) <= 0) {
			if (head == NULL) {
				head = list;
				tail = list;
			} else {
				tail->next = list;
				tail = tail->next;
			}
			list = (list)->next;
		} else {
			if (head == NULL) {
				head = other;
				tail = other;
			} else {
				tail->next = other;
				tail = tail->next;
			}
			other = (other)->next;
		}
	}

	if (list != NULL)
		tail->next = list;
	else if (other != NULL)
		tail->next = other;

	list = head;
	other = NULL;

	return list;
}

/**
 * @brief Finds the first element in the list that matches the given data.
 *
 * @param list Pointer to the list.
 * @param data Pointer to the data to find.
 * @param func A pointer to a comparison function. On match, the function should return `0`.
 *
 * @return Pointer to the first element in the list that matches the given data or NULL if no match is found.
 *
 * @note Time complexity: O(n)
 * @note Space complexity: O(1)
 */
zk_slist *zk_slist_find(zk_slist *list, const void *const data, zk_compare_func const func)
{
	if (list == NULL || func == NULL)
		return NULL;

	while (list != NULL) {
		if (func(list->data, data) == 0)
			break;
		list = list->next;
	}
	return list;
}

/**
 * @brief Find the element at the given index.
 *
 * @param list Pointer to the list.
 * @param index Index of the element to be found.
 *
 * @return - Pointer to the element at the given index or NULL if no match is found.
 *
 * @note Time complexity: O(n)
 * @note Space complexity: O(1)
 */
zk_slist *zk_slist_find_index(zk_slist *list, size_t const index)
{
	if (list == NULL)
		return NULL;

	size_t i = 0;
	while (list != NULL) {
		if (i == index)
			break;
		list = list->next;
		i++;
	}
	return list;
}
