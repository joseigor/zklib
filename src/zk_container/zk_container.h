#ifndef ZK_CONTAINER_H
#define ZK_CONTAINER_H

#include <assert.h>

#include "zk_slist/zk_slist.h"

/**
 * @brief
 * Adds a new element to the end of the \p CONTAINER.
 *
 * @param CONTAINER
 * - Could be any of the types below:
 *	- zk_slist_t *
 * - The data is owned by the caller of the function.
 *
 * @param DATA
 * - Type: void*
 * - A pointer to the data to be added.
 * - This argument can be \c NULL.
 * - The data is owned by the caller of the function.
 *
 * @return
 * - If \p CONTAINER is:
 * 	- zk_slist_t *: The start of the \p CONTAINER.
 * - The data is owned by the caller of the function.
 *
 * **Time Complexity / Space Complexity:**
 * - If \p CONTAINER is:
 * 	- zk_slist_t *: **O(N) / O(1)**.
 *
 * **Example**
 * \include zk_slist/push_back.c
 */
#define zk_push_back(CONTAINER, DATA) _Generic((CONTAINER), zk_slist_t * : zk_slist_push_back)(CONTAINER, DATA)

#endif
