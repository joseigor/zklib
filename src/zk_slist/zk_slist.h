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
 * - The data is owned by the caller of the function.
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
 * - The data is owned by the caller of the function.
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
 * - The data is owned by the caller of the function.
 *
 * **Example**
 * \include zk_slist/copy.c
 */
zk_slist_t *zk_slist_copy(const zk_slist_t *list);

/**
 * @brief
 * Makes a full (deep) copy of a \c zk_slist_t.
 *
 * In contrast with `zk_slist_copy()`, this function uses \p func to make a copy of each list element, in addition to
 * copying the list container itself. \p func as a `zk_copy_data_t` takes two arguments, the `data` to be copied and a
 * `user_data` pointer which can be \c NULL.
 *
 * **Time Complexity:** O(n)
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
 * - The data is owned by the caller of the function.
 *
 * **Example**
 * \include zk_slist/delete_node.c
 */
zk_slist_t *zk_slist_delete_node(zk_slist_t *list, zk_slist_t *node, zk_destructor_t const func);

/**
 * @brief
 * Finds the node from the \p list that contains \p data. If \p list contains more than one node with \p data the first
 * match is returned. User can provide a custom function \p func to performe the comparison which will be called for
 * each node of the list. This function should return 0 when the desired node is found. The function takes two
 * arguments, the zk_slist_t node’s data as the first argument and the given user data as second argument.
 *
 * **Time Complexity:** O(n)
 *
 * @param list
 * - Type: A list of \c zk_list_t
 * - The data is owned by the caller of the function.
 *
 * @param data
 * - Type: Pointer to \c void
 * - The node to be find.
 * - This argument can be \c NULL .
 * - The data is owned by the caller of the function.
 *
 * @param func
 * - Type: zk_compare_t
 * - Function to be used to compare \p list nodes with the \p data provided.
 * - This argument can be \c NULL in which case simple comparison is performed.
 * - The data is owned by the caller of the function.
 *
 * @return zk_slist_t*
 * - Type: A list of \c zk_list_t
 * - The found \c zk_list_t element, or \c NULL if it is not found.
 * - The data is owned by the caller of the function.
 *
 * **Example**
 * \include zk_slist/find.c
 */
zk_slist_t *zk_slist_find(zk_slist_t *list, const void *const data, zk_compare_t func);

/**
 * @brief
 * Iterates over \p list and calls the function \p func for each node of the \p list. This function takes two
 * arguments, the zk_slist_t node’s data as the first argument and the given \p user_data as second argument.
 *
 * **Time Complexity:** O(n) if \p func is O(1)
 *
 * @param list
 * - Type: A list of \c zk_list_t
 * - The data is owned by the caller of the function.
 *
 * @param func
 * - Type: zk_foreach_t
 * - Function called for each node of the list.
 * - The data is owned by the caller of the function.
 *
 * @param user_data
 * - Type: Pointer to void
 * - User data to be passed as second argument of \p func.
 * - It can be NULL.
 * - The data is owned by the caller of the function.
 *
 * **Example**
 * \include zk_slist/foreach.c
 */
void zk_slist_foreach(zk_slist_t *list, zk_foreach_t const func, void *user_data);

/**
 * @brief
 * Frees a zk_slist_t and set it to NULL. If \p func is provided it will be called for every node of \p list. \p func
 * receives as argument the node`s data. So if you need to free the node but also the data the node is pointing
 * to you can do it by providing your own implementation for \p func, see example below. If you just want to free the
 * \p list but not the data each node is pointing to then \p func can be NULL.
 *
 * **Time Complexity:** O(n) if \p func is O(1)
 *
 * @param list_p
 * - Type: A pointer to a list of \c zk_list_t
 * - The list to be freed.
 * - The data is owned by the caller of the function.
 *
 * @param func
 * - Type: zk_destructor_t
 * - Function called for each node of the list.
 * - This is your custom implementation to free each node`s data.
 * - It can be NULL.
 * - The data is owned by the caller of the function.
 *
 * **Example**
 * \include zk_slist/free.c
 */
void zk_slist_free(zk_slist_t **list_p, zk_destructor_t const func);

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
 * - The data is owned by the caller of the function.
 */
zk_slist_t *zk_slist_reverse(zk_slist_t *list);

// TODO: to implement
zk_slist_t *zk_slist_sort(zk_slist_t *list, zk_compare_t func);

// TODO: to implement and create zk_compare_data_t
zk_slist_t *zk_slist_sort_with_data(zk_slist_t *list, zk_compare_t func, void *user_data);
