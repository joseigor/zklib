#include <stdbool.h>
#include <stdlib.h>

#include "zk_vector/zk_vector.h"

#define ZK_VECTOR_DEFAULT_CAPACITY 8
#define ZK_VECTOR_GROWTH_FACTOR    2
#define ZK_VECTOR_ELEMENT_SIZE     sizeof(void *)

/**
 * @brief vector struct
 */
struct zk_vector {
	size_t size;
	size_t capacity;
	void **data;
};
typedef struct zk_vector zk_vector;

static bool _zk_vector_needs_resize(zk_vector *vec)
{
	return vec ? vec->size == vec->capacity : false;
}

static bool _zk_vector_resize(zk_vector *vec)
{
	if (!vec)
		return false;

	size_t new_capacity = vec->capacity * ZK_VECTOR_GROWTH_FACTOR;
	void **new_array = realloc(vec->data, ZK_VECTOR_ELEMENT_SIZE * new_capacity);
	if (!new_array)
		return false;

	vec->data = new_array;
	vec->capacity = new_capacity;
	return true;
}

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
	return vector ? vector->capacity : 0;
}

/**
 * @brief Return a pointer to the underlying array serving as element storage.The pointer is such that range [data();
 * data() + size()) is always a valid range, even if the container is empty (data() is not dereferenceable in that
 * case).
 *
 * @param vec The vector
 *
 * @return A pointer to the underlying element storage
 *
 * @note Time complexity: O(1)
 * @note Space complexity: O(1)
 */
void **zk_vector_data(const zk_vector *const vec)
{
	return vec ? vec->data : NULL;
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
	vec->data = malloc(ZK_VECTOR_ELEMENT_SIZE * vec->capacity);
	if (!vec->data) {
		free(vec);
		return NULL;
	}
	for (size_t i = 0; i < ZK_VECTOR_DEFAULT_CAPACITY; i++)
		vec->data[i] = NULL;

	return vec;
}

/**
 * @brief Add an element to the end of the vector.
 *
 * @param vec The vector
 * @param data The data to add
 *
 * @return None
 *
 * @note Time complexity: O(1) amortized
 * @note Space complexity: O(1) amortized
 */
void zk_vector_push_back(zk_vector *vec, void *data)
{
	if (!vec)
		return;

	if (_zk_vector_needs_resize(vec) && !_zk_vector_resize(vec))
		return;

	vec->data[vec->size++] = data;
}

/**
 * @brief Add an element to the front of the vector.
 *
 * @param vec The vector
 * @param data The data to add
 *
 * @return None
 *
 * @note Time complexity: O(n)
 * @note Space complexity: O(1)
 */
void zk_vector_push_front(zk_vector *vec, void *data)
{
	if (!vec)
		return;

	if (_zk_vector_needs_resize(vec) && !_zk_vector_resize(vec))
		return;

	if (vec->size)
		for (size_t i = vec->size; i > 0; i--)
			vec->data[i] = vec->data[i - 1];

	vec->data[0] = data;
	vec->size++;
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
	return vector ? vector->size : 0;
}
