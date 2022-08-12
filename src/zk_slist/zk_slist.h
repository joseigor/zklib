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
 * - Type: A pointer to the top of a `zk_slist_t`
 * - The data is owned by the caller of the function.
 *
 * @param data
 * - Type: A pointer to the data to be addded.
 * - This argument can be `NULL`.
 * - The data is owned by the caller of the function.
 *
 * @return zk_slist_t*
 * - Type: A pointer to a `zk_list_t`.
 * - Either \p list or the new start of the `zk_list_t` if \p list was `NULL`.
 * - The data is owned by the called function.
 *
 * **Example**
 * \code{.c}
 *
 * zk_slist_t *number_slist = NULL, *string_list = NULL, *custom_slist = NULL;
 *
 * int node1_data = 1;
 * int node2_data = 2;
 *
 * // This is a list of integers.
 * number_slist = zk_slist_append(number_slist, &node1_data);
 * number_slist = zk_slist_append(number_slist, &node2_data);
 *

 * // This is a list of strings.
 * string_list = zk_slist_append (string_list, "first");
 * string_list = zk_slist_append (string_list, "second");
 *
 *
 * struct custom_data {
 *    int value;
 *    char *string;
 * };
 *
 * struct custom_data custom_node_1 = { .value = 1, .string = "node1"};
 * struct custom_data custom_node_2 = { .value = 2, .string = "node2"};
 * struct custom_data custom_node_3 = { .value = 3, .string = "node3"};
 *
 * // This is a list of custom data.
 * custom_slist = zk_slist_append (custom_slist, &custom_node_1);
 * custom_slist = zk_slist_append (custom_slist, &custom_node_2);
 * custom_slist = zk_slist_append (custom_slist, &custom_node_3);
 *
 *  \endcode
 *
 */
zk_slist_t *zk_slist_append(zk_slist_t *list, void *const data);

zk_slist_t *zk_slist_concat(zk_slist_t *list_dest, zk_slist_t *list_src);

/**
 * @brief
 * Copies a `zk_slist_t`
 *
 * Note that this is a "shallow" copy. If the list elements consist of pointers to data,the pointers are copied but the
 * actual data is not. See `zk_slist_copy_deep()` if you need to copy the data as well.
 *
 * @param list
 * Type: A pointer to the top of a `zk_slist_t`
 * The data is owned by the caller of the function.
 *
 * @return zk_slist_t*
 * Type: A pointer to the start of the new list that holds the same data as list.
 * The data is owned by the called function.
 */
zk_slist_t *zk_slist_copy(const zk_slist_t *list);

/**
 * @brief
 * Makes a full (deep) copy of a `zk_slist_t`.
 *
 * In contrast with `zk_slist_copy()`, this function uses `func` to make a copy of each list element, in addition to
 * copying the list container itself. func, as a `zk_copy_data_t`, takes two arguments, the `data` to be copied and a
 * `user_data` pointer.
 *
 * @param list
 * Type: A pointer to the top of a zk_slist_t
 * The data is owned by the caller of the function.
 *
 * @param func
 * TYPE: A copy function of type `zk_copy_data_t` used to copy every element in the list.
 *
 * @param user_data
 * Type: `void*`
 * User data passed to the copy function `func`, or NULL.
 * The argument can be NULL.
 * The data is owned by the caller of the function.

 *
 * @return zk_slist_t*
 * Type: A pointer to the start of the new list that holds the same data as `list`.
 * Use `zk_slist_free_full()` to free it.
 * The data is owned by the called function
 */
zk_slist_t *zk_slist_copy_deep(const zk_slist_t *list, zk_copy_data_t func, void *user_data);

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
 * @brief Reverses a `zk_slist_t`. It simply switches the next and prev pointers of each element.
 *
 * @param list
 * Type: A pointer to the start of a `zk_slist_t`.
 * The data is owned by the caller of the function.
 *
 * @return zk_slist_t*
 * Type: A pointer to the start of the reversed list.
 * The data is owned by the called function.
 */
zk_slist_t *zk_slist_reverse(zk_slist_t *list);

// TODO: to implement
zk_slist_t *zk_slist_sort(zk_slist_t *list, zk_compare_t func);

// TODO: to implement and create zk_compare_data_t
zk_slist_t *zk_slist_sort_with_data(zk_slist_t *list, zk_compare_t func, void *user_data);
