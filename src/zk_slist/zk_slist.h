#pragma once

#include "zk_common/zk_types.h"

typedef struct zk_slist_t {
	void *data;
	struct zk_slist_t *next;
} zk_slist_t;

#if UNIT_TEST
void _zk_slist_front_back_split(zk_slist_t *list, zk_slist_t **front, zk_slist_t **back);
#endif

/**
 * @brief
 * Adds a new element to the end of the \p list.
 * If \p list is empty the return value is the start of a new \p list.
 *
 * **Time Complexity:** O(n)
 *
 * In order to append an element to a \p list `zk_slist_append()` needs to traverse all the `list` to find its end which
 * is a \b O(n) operation. This is not a problem when few elements need to be added but if you are working with a big
 * set of elements the best approach is to first use `zk_slist_prepend()` which has time complexity \b O(1) and then
 * reverse the list calling `zk_slist_reverse()` that is a \b O(n) operation.
 *
 * @param list
 * - Type: A list of \c zk_list_t
 * - The data is owned by the caller of the function.
 *
 * @param data
 * - Type: A pointer to the data to be addded.
 * - This argument can be \c NULL.
 * - The data is owned by the caller of the function.
 *
 * @return zk_slist_t*
 * - Type: A list of \c zk_list_t.
 * - Either \p list or the new start of the \c zk_list_t if \p list was \c NULL.
 * - The data is owned by the called function.
 *
 * **Example**
 * \include zk_slist/append.c
 */
zk_slist_t *zk_slist_append(zk_slist_t *list, void *const data);

/**
 * @brief
 * Adds the second list \p list_src to the end of the first list \p list_dest. Note that the elements \p list_src are
 * not copied they are used directly. This function just appends the \p list_src to the end of \p list_dest . If your
 * intent is to concat a copy of \p list_src to \p list_dest check `zk_slist_copy()` and `zk_slist_copy_deep()`.
 *
 * **Time Complexity:** O(n)
 *
 * @param list_dest
 * - Type: A list of \c zk_list_t
 * - This argument can be \c NULL
 * - The data is owned by the caller of the function.
 *
 * @param list_src
 * - Type: A list of \c zk_list_t
 * - The \c zk_slist_t to add to the end of the first zk_slist_t, this must point to the top of the list.
 * - This argument can be \c NULL
 * - The data is owned by the caller of the function.
 *
 * @return zk_slist_t*
 * - Type: A list of \c zk_slist_t
 * - The start of the new \c zk_slist_t or if \p list_dest is \c NULL returns \c NULL.
 * - The data is owned by the called function.
 *
 * **Example**
 * \include zk_slist/concat.c
 */
zk_slist_t *zk_slist_concat(zk_slist_t *const list_dest, zk_slist_t *const list_src);

/**
 * @brief
 * Copies a \c zk_slist_t. Note that this is a "shallow" copy. If the \p list elements consist of pointers to data,
 * the pointers are copied but the actual data is not. See `zk_slist_copy_deep()` if you need to copy the data as well.
 *
 * **Time Complexity:** O(n)
 *
 * @param list
 * - Type: A list of \c zk_list_t
 * - The data is owned by the caller of the function.
 *
 * @return zk_slist_t*
 * - Type: A list of \c zk_slist_t
 * - The start of the new list that holds the same data as list.
 * - The data is owned by the called function.
 *
 * **Example**
 * \include zk_slist/copy.c
 */
zk_slist_t *zk_slist_copy(const zk_slist_t *list);

/**
 * @brief
 * Makes a full (deep) copy of a \c zk_slist_t.
 *
 * **Time Complexity:** O(n)
 *
 * In contrast with `zk_slist_copy()`, this function uses \p func to make a copy of each list element, in addition to
 * copying the list container itself. \p func as a `zk_copy_data_t` takes two arguments, the `data` to be copied and a
 * `user_data` pointer which can be \c NULL.
 *
 * @param list
 * - Type: A list of \c zk_list_t
 * - The data is owned by the caller of the function.
 *
 * @param func
 *  - Type: A function pointer of type `zk_copy_data_t()` used to copy every element in the list.
 *
 * @param user_data
 * - Type: `void*`
 * - User data passed to the copy function \p func, or \c NULL.
 * - The data is owned by the caller of the function.
 *
 * @return zk_slist_t*
 * - Type: A list of \c zk_slist_t
 * - The start of the new list that holds the same data as \p list or \c NULL if \p list or \p func is \c NULL
 * - Use `zk_slist_free_full()` to free it.
 * - The data is owned by the called function
 *
 * **Example**
 * \include zk_slist/copy_deep.c
 */
zk_slist_t *zk_slist_copy_deep(const zk_slist_t *list, zk_copy_data_t func, void *user_data);

/**
 * @brief
 * Removes \c node from the \p list and frees it. If you need to free the data that \p node is pointing to you can pass
 * a custom function pointer \p func that implements a custom behavior to proper free the node`s data.
 *
 * **Time Complexity:** O(n)
 *
 * @param list
 * - Type: A list of \c zk_list_t
 * - The data is owned by the caller of the function.
 *
 * @param node
 * - Type: A list of \c zk_list_t
 * - Node to be delete.
 * - The data is owned by the caller of the function.
 *
 * @param func
 * - Type: A function pointer of type `zk_destructor_t()` used to free the \p node data.
 * - It can be \c NULL.
 *
 * @return zk_slist_t*
 * - Type: A list of \c zk_list_t
 * - The new head of the \p list .
 * - The data is owned by the called function.
 *
 * **Example**
 * \include zk_slist/delete_node.c
 */
zk_slist_t *zk_slist_delete_node(zk_slist_t *list, zk_slist_t *node, zk_destructor_t func);

// FIXME: rename to zk_slist_find
zk_slist_t *zk_slist_find_by_data(zk_slist_t *list, const void *const data);

// FIXME: rename to zk_slist_find_custom
zk_slist_t *zk_slist_find_by_data_custom(zk_slist_t *list, const void *const data, zk_compare_t func);

void zk_slist_foreach(zk_slist_t *list, zk_foreach_t func, void *user_data);

void zk_slist_free(zk_slist_t **list_p);

void zk_slist_free_full(zk_slist_t **list_p, zk_destructor_t func);

// FIXME: rename to zk_slist_index
int zk_slist_get_index(zk_slist_t *list, const void *const data);

zk_slist_t *zk_slist_insert(zk_slist_t *list, void *data, int position);

zk_slist_t *zk_slist_insert_before(zk_slist_t *list, zk_slist_t *sibling, void *data);

// TODO: to implement
zk_slist_t *zk_slist_insert_before_link(zk_slist_t *list, zk_slist_t *sibling, zk_slist_t *link);

// TODO: to implement
zk_slist_t *zk_slist_insert_sorted(zk_slist_t *list, void *data, zk_compare_t func);

// TODO: to implement and create zk_compare_data_t
zk_slist_t *zk_slist_insert_sorted_with_data(zk_slist_t *list, void *data, zk_compare_t func, void *user_data);

zk_slist_t *zk_slist_last(zk_slist_t *list);

unsigned int zk_slist_length(zk_slist_t *list);

zk_slist_t *zk_slist_nth(zk_slist_t *list, unsigned int n);

// TODO: to implement
void *zk_slist_nth_data(zk_slist_t *list, unsigned int n);

// TODO: to implement
zk_slist_t *zk_slist_nth_prev(zk_slist_t *list, unsigned int n);

// TODO: to implement
unsigned int zk_slist_position(zk_slist_t *list, zk_slist_t *link);

zk_slist_t *zk_slist_prepend(zk_slist_t *list, void *data);

// TODO: to implement
zk_slist_t *zk_slist_remove(zk_slist_t *list, const void *const data);

// TODO: to implement
zk_slist_t *zk_slist_remove_all(zk_slist_t *list, const void *const data);

/**
 * @brief Reverses a \c zk_slist_t. It simply switches the next and prev pointers of each element.
 *
 * @param list
 * - Type: A pointer to the start of a \c zk_slist_t.
 * - The data is owned by the caller of the function.
 *
 * @return zk_slist_t*
 * - Type: A list of \c zk_slist_t
 * - The start of the reversed list.
 * - The data is owned by the called function.
 */
zk_slist_t *zk_slist_reverse(zk_slist_t *list);

// TODO: to implement
zk_slist_t *zk_slist_sort(zk_slist_t *list, zk_compare_t func);

// TODO: to implement and create zk_compare_data_t
zk_slist_t *zk_slist_sort_with_data(zk_slist_t *list, zk_compare_t func, void *user_data);
