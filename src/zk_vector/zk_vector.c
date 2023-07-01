#include <stdlib.h>

#include "zk_vector/zk_vector.h"

#define ZK_VECTOR_DEFAULT_CAPACITY 8

/**
 * @brief vector struct
 */
struct zk_vector {
	size_t size;
	size_t capacity;
	void **data;
};
typedef struct zk_vector zk_vector;

/**
 * @brief Return the capacity of the vector.
 *
 * @param vector The vector
 *
 * @return The capacity of the vector
 *
 * @note Time complexity: O(1)
 * @note Space complexity: O(1)
 */
size_t zk_vector_capacity(const zk_vector *const vector)
{
	if (!vector)
		return 0;

	return vector->capacity;
}

/**
 * @brief Free a vector and all of its elements if a destructor function is provided.
 *
 * @param vector The vector to free
 * @param func The destructor function to call on each element of the vector. It can be NULL.
 *
 * @note Time complexity: O(1)
 * @note Space complexity: O(1)
 */
void zk_vector_free(zk_vector *vector, zk_destructor_t func)
{
	if (!vector)
		return;

	if (func)
		for (size_t i = 0; i < vector->size; i++)
			func(vector->data[i]);

	free(vector->data);
	free(vector);
}

/**
 * @brief Create a new vector, with a default capacity of 8. All elements are initialized to NULL.
 *
 * @return zk_vector* on success, NULL on failure
 *
 * @note The caller is responsible for freeing the returned vector
 *
 * @note Time complexity: O(1)
 * @note Space complexity: O(1)
 */
zk_vector *zk_vector_new(void)
{
	zk_vector *vec = malloc(sizeof(zk_vector));
	if (!vec)
		return NULL;

	vec->size = 0;
	vec->capacity = ZK_VECTOR_DEFAULT_CAPACITY;
	vec->data = malloc(sizeof(void *) * vec->capacity);
	if (!vec->data) {
		free(vec);
		return NULL;
	}
	for (size_t i = 0; i < ZK_VECTOR_DEFAULT_CAPACITY; i++)
		vec->data[i] = NULL;

	return vec;
}

/**
 * @brief Return the number of elements in the vector.
 *
 * @param vector The vector
 *
 * @return The number of elements in the vector
 *
 * @note Time complexity: O(1)
 * @note Space complexity: O(1)
 */
size_t zk_vector_size(const zk_vector *const vector)
{
	if (!vector)
		return 0;

	return vector->size;
}
