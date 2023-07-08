#include <stdbool.h>
#include <stdlib.h>

#include "zk_common/zk_common.h"
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
 * @brief Return a pointer to the element at the specified index in the vector.
 *
 * @param vec The vector
 * @param index The index of the element to return
 * @param out The pointer that will be set to the element at the specified index
 *
 * @return ZK_OK on success, ZK_INVALID_ARGUMENT if the vector is NULL or the index is out of bounds
 *
 * @note Time complexity: O(1)
 * @note Space complexity: O(1)
 *
 */
zk_status zk_vector_at(const zk_vector *const vec, size_t index, void **out)
{
	if (!vec || index >= vec->size)
		return ZK_INVALID_ARGUMENT;

	*out = vec->data[index];
	return ZK_OK;
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
 * @brief Insert an element at the specified index in the vector. If the index is out of bounds, the vector is resized.
 *
 * @param vec The vector
 * @param index The index at which to insert the element
 * @param data The data to insert
 *
 * @return  An iterator that points to the inserted element, or NULL on failure.
 *
 * @note Time complexity: O(1) amortized
 * @note Space complexity: O(1)
 */
void **zk_vector_insert(zk_vector *vec, size_t index, void *data)
{
	if (!vec)
		return NULL;

	if (index >= vec->size) {
		vec->capacity = index + 1;
		vec->size = index + 1;
		if (!_zk_vector_resize(vec))
			return NULL;
	}

	vec->data[index] = data;
	return &vec->data[index];
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
 * @brief Remove the last element of the vector.
 *
 * @param vec The vector
 * @param func The destructor function to call on the element. It can be NULL.
 *
 * @return None
 *
 * @note Time complexity: O(1)
 * @note Space complexity: O(1)
 */
void zk_vector_pop_back(zk_vector *vec, zk_destructor_t func)
{
	if (!vec)
		return;

	if (vec->size) {
		vec->size--;
		if (func)
			func(vec->data[vec->size]);

		vec->data[vec->size] = NULL;
	}
}

/**
 * @brief Remove the first element of the vector.
 *
 * @param vec The vector
 * @param func The destructor function to call on the element. It can be NULL.
 *
 * @return None
 *
 * @note Time complexity: O(n)
 * @note Space complexity: O(1)
 */
void zk_vector_pop_front(zk_vector *vec, zk_destructor_t func)
{
	if (!vec)
		return;

	if (vec->size) {
		vec->size--;
		if (func)
			func(vec->data[0]);

		for (size_t i = 0; i < vec->size; i++)
			vec->data[i] = vec->data[i + 1];

		vec->data[vec->size] = NULL;
	}
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

	size_t i = vec->size;
	while (i) {
		vec->data[i] = vec->data[i - 1];
		i--;
	}

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
