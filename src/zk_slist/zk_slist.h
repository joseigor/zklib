#pragma once

#include <stddef.h>

#include "zk_common/zk_common.h"

typedef struct zk_slist zk_slist;
// Constructor
zk_status zk_slist_new_node(zk_slist **node_p, void *const data);

// Destructor
void zk_slist_free(zk_slist **list_p, zk_destructor_t const func);

// Element access
zk_status zk_slist_get_data(const zk_slist *const list, void **data);

// Iterators
zk_status zk_slist_next(const zk_slist *const list, zk_slist **next);

zk_slist *zk_slist_begin(zk_slist *list);

zk_slist *zk_slist_end(zk_slist *list);

void zk_slist_for_each(zk_slist *begin, zk_slist *const end, zk_for_each_func const func, void *const user_data);

// Modifiers
zk_status zk_slist_pop_back(zk_slist **list_p, zk_destructor_t const func);

zk_status zk_slist_pop_front(zk_slist **list_p, zk_destructor_t const func);

zk_status zk_slist_push_back(zk_slist **list_p, void *const data);

zk_status zk_slist_push_front(zk_slist **list_p, void *const data);

/**
 * @brief Reverses the order of the elements in the list.
 *
 * @param list_p Pointer to the list to reverse.
 *
 * @return ZK_OK if the list was reversed successfully. ZK_INVALID_ARGUMENT if list_p is NULL or *list_p is NULL.
 *
 * @note This function is O(n).
 */
zk_status zk_slist_reverse(zk_slist **list_p);

/**
 * @brief Returns the number of elements in the list.
 *
 * @param list Pointer to the list.
 *
 * @return The number of elements in the list.
 * @return 0 if list is NULL.
 *
 * @note This function is O(n).
 */
size_t zk_slist_size(const zk_slist *const list);

/**
 * @brief Sorts the list.
 *
 * @param list_p Pointer to the list to sort.
 * @param func Pointer to the comparison function.
 *
 * @return ZK_OK if the list was sorted successfully. ZK_INVALID_ARGUMENT if list_p is NULL or *list_p is NULL or func
 * is NULL.
 *
 * @note This function is O(n log n)
 * @note The comparison function must return a negative value if a < b, 0 if a == b, and a positive value if a > b
 * @note This sort algorithm is stable.
 * @note This sort algorithm is in-place.
 */
zk_status zk_slist_sort(zk_slist **list_p, zk_compare_func const func);

/**
 * @brief Merges two sorted lists. If function succeeds list will contain the merged list and other will be set to NULL.
 *
 * @param list Pointer to the first list.
 * @param other Pointer to the second list.
 * @param func Pointer to the comparison function.
 *
 * @return ZK_OK if the lists were merged successfully. ZK_INVALID_ARGUMENT if list is NULL or other is NULL or func is
 * NULL.
 *
 * @note This function is O(n), where n is the number of elements of the bigest list.
 * @note The comparison function must return a negative value if a < b, 0 if a == b, and a positive value if a > b
 * @note This merge algorithm is stable.
 * @note This merge algorithm is in-place.
 */
zk_status zk_slist_merge(zk_slist **list_p, zk_slist **other, zk_compare_func const func);
