#ifndef ZK_CONTAINER_H
#define ZK_CONTAINER_H

#include "zk_c_dlist/zk_c_dlist.h"
#include "zk_c_slist/zk_c_slist.h"
#include "zk_dlist/zk_dlist.h"
#include "zk_slist/zk_slist.h"

// clang-format off

// Destructor
#define zk_free(CONTAINER, FUNC)                 \
	_Generic((CONTAINER),                    \
		zk_slist **   : zk_slist_free,   \
		zk_dlist **   : zk_dlist_free,   \
		zk_c_slist ** : zk_c_slist_free, \
		zk_c_dlist ** : zk_c_dlist_free) \
		(CONTAINER, FUNC)

// Element access
#define zk_get_data(CONTAINER, DATA)                \
	_Generic((CONTAINER),                       \
		zk_dlist *   : zk_dlist_get_data,   \
		zk_c_slist * : zk_c_slist_get_data, \
		zk_c_dlist * : zk_c_dlist_get_data) \
		(CONTAINER, DATA)

// Iterators
#define zk_for_each(CONTAINER, FUNC, USER_DATA)     \
	_Generic((CONTAINER),                       \
		zk_slist *   : zk_slist_for_each,   \
		zk_dlist *   : zk_dlist_for_each,   \
		zk_c_slist * : zk_c_slist_for_each, \
		zk_c_dlist * : zk_c_dlist_for_each) \
		(                                   \
			zk_begin(CONTAINER),        \
			zk_end(CONTAINER),          \
			FUNC,                       \
			USER_DATA                   \
		)

#define zk_begin(CONTAINER)                      \
	_Generic((CONTAINER),                    \
		zk_slist *   : zk_slist_begin,   \
		zk_dlist *   : zk_dlist_begin,   \
		zk_c_slist * : zk_c_slist_begin, \
		zk_c_dlist * : zk_c_dlist_begin) \
		(CONTAINER)

#define zk_end(CONTAINER)                      \
	_Generic((CONTAINER),                  \
		zk_slist *   : zk_slist_end,   \
		zk_dlist *   : zk_dlist_end,   \
		zk_c_slist * : zk_c_slist_end, \
		zk_c_dlist * : zk_c_dlist_end) \
		(CONTAINER)

#define zk_next(CONTAINER, NEXT)                \
	_Generic((CONTAINER),                   \
		zk_dlist *   : zk_dlist_next,   \
		zk_c_slist * : zk_c_slist_next, \
		zk_c_dlist * : zk_c_dlist_next) \
		(CONTAINER, NEXT)

#define zk_prev(CONTAINER, NEXT)                \
	_Generic((CONTAINER),                   \
		zk_dlist * : zk_dlist_prev,     \
		zk_c_dlist * : zk_c_dlist_prev) \
		(CONTAINER, NEXT)

// Modifiers
#define zk_pop_back(CONTAINER, FUNC)                 \
	_Generic((CONTAINER),                        \
		zk_slist *   : zk_slist_pop_back,    \
		zk_dlist **   : zk_dlist_pop_back,   \
		zk_c_slist ** : zk_c_slist_pop_back, \
		zk_c_dlist ** : zk_c_dlist_pop_back) \
		(CONTAINER, FUNC)

#define zk_pop_front(CONTAINER, FUNC)                 \
	_Generic((CONTAINER),                         \
		zk_slist *   : zk_slist_pop_front,    \
		zk_dlist **   : zk_dlist_pop_front,   \
		zk_c_slist ** : zk_c_slist_pop_front, \
		zk_c_dlist ** : zk_c_dlist_pop_front) \
		(CONTAINER, FUNC)

#define zk_push_back(CONTAINER, DATA)                 \
	_Generic((CONTAINER),                         \
		zk_slist *   : zk_slist_push_back,    \
		zk_dlist **   : zk_dlist_push_back,   \
		zk_c_slist ** : zk_c_slist_push_back, \
		zk_c_dlist ** : zk_c_dlist_push_back) \
		(CONTAINER, DATA)

#define zk_push_front(CONTAINER, DATA)                   \
	_Generic((CONTAINER),                            \
		zk_slist *    : zk_slist_push_front,     \
		zk_dlist **   : zk_dlist_push_front,     \
		zk_c_slist ** : zk_c_slist_push_front,   \
		zk_c_dlist ** : zk_c_dlist_push_front)   \
		(CONTAINER, DATA)

/**
 * @brief Reverse the order of the elements in the container.
 *
 * @param CONTAINER A pointer to the container to reverse.
 *
 * @return ZK_OK if the operation was successful, ZK_INVALID_ARGUMENT if the container is invalid.
 *
 * @note This function is only available for the following containers:
 *      - zk_slist
*/
#define zk_reverse(CONTAINER)                     \
	_Generic((CONTAINER),                     \
		zk_slist **   : zk_slist_reverse) \
		(CONTAINER)
/**
 * @brief Return the number of elements in the container.
 *
 * @param CONTAINER A pointer to the container.
 *
 * @return The number of elements in the container.
 *
 * @note This function is only available for the following containers:
 *     - zk_slist
*/
#define zk_size(CONTAINER)                    \
	_Generic((CONTAINER),                 \
		zk_slist *   : zk_slist_size) \
		(CONTAINER)


/**
 * @brief Merge two sorted containers. The result is stored in the first container. The second container is set to NULL.
 *
 * @param FIRST_CONTAINER A pointer to the first container.
 * @param SECOND_CONTAINER A pointer to the second container.
 * @param FUNC A pointer to the comparison function.
 *             The comparison function must return a negative value if a < b, 0 if a == b, and a positive value if a > b.
 *
 * @return ZK_OK if the operation was successful, ZK_INVALID_ARGUMENT if arguments are invalid.
 *
 * @note This function is only available for the following containers:
 *     - zk_slist
*/
#define zk_merge(FIRST_CONTAINER, SECOND_CONTAINER, FUNC) \
	_Generic((FIRST_CONTAINER),                       \
		zk_slist **   : zk_slist_merge)           \
		(FIRST_CONTAINER, SECOND_CONTAINER, FUNC)

/**
 * @brief Sort the elements in the container.
 *
 * @param CONTAINER A pointer to the container.
 * @param FUNC A pointer to the comparison function.
 *             The comparison function must return a negative value if a < b, 0 if a == b, and a positive value if a > b.
 *             Signature: int (*func)(const void *const a, const void *const b);
 *
 * @return ZK_OK if the operation was successful, ZK_INVALID_ARGUMENT if arguments are invalid.
 *
 * @note This function is only available for the following containers:
 *     - zk_slist
*/
#define zk_sort(CONTAINER, FUNC)            \
	_Generic((CONTAINER),               \
		zk_slist * : zk_slist_sort) \
		(CONTAINER, FUNC)

#define zk_find(CONTAINER, DATA, FUNC)        \
	_Generic((CONTAINER),                 \
		zk_slist *   : zk_slist_find) \
		(CONTAINER, DATA, FUNC)

#endif /* ZK_CONTAINER_H */
