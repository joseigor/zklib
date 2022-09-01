#pragma once

#include <stddef.h>

#include "zk_common/zk_common.h"

/**
 * @brief Single linked list struct
 *
 */
typedef struct zk_slist {
	void *data;
	struct zk_slist *next;
} zk_slist;

#if UNIT_TEST
void _zk_slist_front_back_split(zk_slist *list, zk_slist **front, zk_slist **back);
#endif

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
 * - The \c zk_slist to add to the end of the first zk_slist, this must point to the top of the list.
 * - This argument can be \c NULL
 * - The data is owned by the caller of the function.
 *
 * @return zk_slist*
 * - Type: A list of \c zk_slist
 * - The start of the new \c zk_slist or if \p list_dest is \c NULL returns \c NULL.
 * - The data is owned by the caller of the function.
 *
 * **Example**
 * \include zk_slist/concat.c
 */
zk_slist *zk_slist_concat(zk_slist *const list_dest, zk_slist *const list_src);

/**
 * @brief
 * Copies a \c zk_slist. Note that this is a "shallow" copy. If the \p list elements consist of pointers to data,
 * the pointers are copied but the actual data is not. See `zk_slist_copy_deep()` if you need to copy the data as well.
 *
 * **Time Complexity:** O(n)
 *
 * @param list
 * - Type: A list of \c zk_list_t
 * - The data is owned by the caller of the function.
 *
 * @return zk_slist*
 * - Type: A list of \c zk_slist
 * - The start of the new list that holds the same data as list.
 * - The data is owned by the caller of the function.
 *
 * **Example**
 * \include zk_slist/copy.c
 */
zk_slist *zk_slist_copy(const zk_slist *list);

/**
 * @brief
 * Makes a full (deep) copy of a \c zk_slist.
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
 * @return zk_slist*
 * - Type: A list of \c zk_slist
 * - The start of the new list that holds the same data as \p list or \c NULL if \p list or \p func is \c NULL
 * - Use `zk_slist_free_full()` to free it.
 * - The data is owned by the called function
 *
 * **Example**
 * \include zk_slist/copy_deep.c
 */
zk_slist *zk_slist_copy_deep(const zk_slist *list, zk_copy_data_t func, void *user_data);

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
 * @return zk_slist*
 * - Type: A list of \c zk_list_t
 * - The new head of the \p list .
 * - The data is owned by the caller of the function.
 *
 * **Example**
 * \include zk_slist/delete_node.c
 */
zk_slist *zk_slist_delete_node(zk_slist *list, zk_slist *node, zk_destructor_t const func);

/**
 * @brief
 * Finds the node from the \p list that contains \p data. If \p list contains more than one node with \p data the first
 * match is returned. User can provide a custom function \p func to performe the comparison which will be called for
 * each node of the list. The provided user function function should return 0 when the desired node is found. The
 * function takes two arguments, the zk_slist node’s data as the first argument and the given user data as second
 * argument.
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
 * @return zk_slist*
 * - Type: A list of \c zk_list_t
 * - The found \c zk_list_t element, or \c NULL if it is not found.
 * - The data is owned by the caller of the function.
 *
 * - \note \p func should handle NULL arguments.
 *
 * **Example**
 * \include zk_slist/find.c
 */
zk_slist *zk_slist_find(zk_slist *list, const void *const data, zk_compare_t func);

/**
 * @brief
 * Frees a zk_slist and set it to NULL. If \p func is provided it will be called for every node of \p list. \p func
 * receives as argument the node`s data. So if you need to free the node but also the data the node is pointing
 * to you can do it by providing your own implementation for \p func, see example below. If you just want to free the
 * \p list but not the data each node is pointing to then \p func can be NULL.
 *
 * **Time Complexity:** O(n)
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
void zk_slist_free(zk_slist **list_p, zk_destructor_t const func);

/**
 * @brief
 * Finds first node that contains \p data and returns its index. If \p list contains more than one node with \p data the
 * index of the first match is returned. User can provide a custom function \p func to performe the comparison which
 * will be called for each node of the list. The provided user function should return 0 when the desired data is found.
 * The function takes two arguments, the zk_slist node’s data as the first argument and the given user data as second
 * argument.
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
 * @return size_t
 * - Type size_t
 * - The index of the node or 0 if data is not found.
 * - Index starts at 1.
 *
 * \note \p func should handle NULL arguments.
 *
 * **Example**
 * \include zk_slist/index.c
 */
size_t zk_slist_index(zk_slist *list, const void *const data, zk_compare_t const func);

/**
 * @brief Inserts a new  node with \p data at the given \p position.
 *
 * **Time Complexity:** O(n)
 *
 * @param list
 * - Type: A list of \c zk_list_t
 * - The data is owned by the caller of the function.
 *
 * @param data
 * - Type: Pointer to \c void
 * - Data to be inserted in the list as part of a new node.
 * - This argument can be \c NULL .
 * - The data is owned by the caller of the function.
 *
 * @param position
 * - Type size_t
 * - The position where to insert the new node with \p data.
 * - First node of the list is 1.
 * - If \p postion is 0 or greater than the number of nodes that list contains the new node is inserted at the
 * end of the list.
 *
 * @return zk_slist*
 * - Type: A list of \c zk_list_t
 * - The head of the new \p list which contains the new node with \p data.
 * - The data is owned by the caller of the function.
 *
 * **Example**
 * \include zk_slist/insert.c
 */
zk_slist *zk_slist_insert(zk_slist *list, void *data, size_t position);

/**
 * @brief Inserts a node before \p sibling containing data.
 *
 * **Time Complexity:** O(n)
 *
 * @param list
 * - Type: A list of \c zk_list_t
 * - The data is owned by the caller of the function.
 *
 * @param sibling
 * - Type: A list of \c zk_list_t
 * - Node to insert data before.
 *
 * @param data
 * - Type: Pointer to \c void
 * - Data to put in the newly-inserted node.
 * - This argument can be \c NULL .
 * - The data is owned by the caller of the function.
 *
 * @return zk_slist*
 * - Type: A list of \c zk_list_t
 * - Thenew head of the \p list.
 * - The data is owned by the caller of the function.
 *
 * **Example**
 * \include zk_slist/insert_before.c
 */
zk_slist *zk_slist_insert_before(zk_slist *list, zk_slist *sibling, void *data);

/**
 * @brief Gets last node of \p list.
 *
 * @param list
 * - Type: A list of \c zk_list_t
 * - The data is owned by the caller of the function.
 *
 * @return zk_slist*
 * - Type: A list of \c zk_list_t
 * - Last node of the \p list of NULL if \p list is NULL.
 * - The data is owned by the caller of the function.
 *
 * **Example**
 * \include zk_slist/last.c
 */

zk_slist *zk_slist_last(zk_slist *list);

/**
 * @brief Get the length of the list.
 *
 * **Time Complexity:** O(n)
 *
 * @param list
 * - Type: A list of \c zk_list_t
 * - The data is owned by the caller of the function.
 *
 * @return size_t
 * - Type size_t
 * - The length of the list or 0 if list has no nodes.
 *
 * **Example**
 * \include zk_slist/length.c
 */
size_t zk_slist_length(zk_slist *list);

/**
 * @brief Gets the \p list node at a given position \p n.
 *
 * **Time Complexity:** O(n)
 *
 * @param list
 * - Type: A list of \c zk_list_t
 * - The data is owned by the caller of the function.
 *
 * @param n
 * - Type size_t
 * - The node position.
 * - First node of the list is at position 1.
 *
 * @return zk_slist*
 * - Type: A list of \c zk_list_t
 * - The node at position \p n.
 * - If \p n is 0 or greater than the number of nodes that \p list contains the the last node is returned.
 * - NULL is \p list is NULL.
 * - The data is owned by the caller of the function.
 *
 * **Example**
 * \include zk_slist/nth.c
 */
zk_slist *zk_slist_nth(zk_slist *list, size_t n);

/**
 * @brief Reverses a \c zk_slist. It simply switches the next and prev pointers of each element.
 *
 * **Time Complexity:** O(n)
 *
 * @param list
 * - Type: A list of \c zk_list_t
 * - The data is owned by the caller of the function.
 *
 * @return zk_slist*
 * - Type: A list of \c zk_list_t
 * - The start of the reversed list.
 * - The data is owned by the caller of the function.
 *
 * **Example**
 * \include zk_slist/reverse.c
 */
zk_slist *zk_slist_reverse(zk_slist *list);

// Constructor
zk_slist *zk_slist_new_node(void *const data);

// Destructor
void zk_slist_free(zk_slist **list_p, zk_destructor_t const func);

// Iterators
zk_slist *zk_slist_begin(zk_slist *list);

zk_slist *zk_slist_end(zk_slist *list);

void zk_slist_for_each(zk_slist *begin, zk_slist *const end, zk_for_each_func const func, void *const user_data);

// Modifiers
zk_slist *zk_slist_push_back(zk_slist *list, void *const data);

zk_slist *zk_slist_push_front(zk_slist *list, void *const data);
