#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "zk_common/zk_common.h"
#include "zk_vector/zk_vector.h"

#define ZK_VECTOR_DEFAULT_CAPACITY       8
#define ZK_VECTOR_GROWTH_FACTOR          2
#define ZK_VECTOR_GET_FROM_DATA(data)    (zk_vector *)((char *)data - sizeof(zk_vector))
#define ZK_VECTOR_GET_DATA_FROM_VEC(vec) (void *)((char *)vec + sizeof(zk_vector))
#define ZK_VECTOR_TYPE_SIZE              sizeof(zk_vector)

/**
 * @brief vector struct
 */
struct zk_vector {
	size_t size; // number of elements in the vector
	size_t capacity; // total capacity of the vector
	size_t element_size; // size of each element of the vector
	zk_destructor_t destructor; // destructor function for each element of the vector
	zk_copy_t copy; // copy function for each element of the vector
	zk_move_t move; // move function for each element of the vector
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

	char *new_array = realloc(vec, ZK_VECTOR_TYPE_SIZE * vec->element_size * new_capacity);
	if (!new_array)
		return false;


	vec->capacity = new_capacity;
	return true;
}

size_t zk_vector_size(void *data)
{
	if (!data)
		return 0;

	zk_vector *vector = ZK_VECTOR_GET_FROM_DATA(data);
	return vector ? vector->size : 0;
}

size_t zk_vector_capacity(void *data)
{
	if (!data)
		return 0;
	zk_vector *vector = ZK_VECTOR_GET_FROM_DATA(data);
	return vector ? vector->capacity : 0;
}

void *zk_vector_new(size_t sizeof_element, zk_destructor_t destructor, zk_copy_t copy, zk_move_t move)
{
	// FIXME: no need to allocate a vector is can be in the stack
	zk_vector *vec = malloc(sizeof(zk_vector));
	if (!vec)
		return NULL;

	vec->size = 0;
	vec->capacity = ZK_VECTOR_DEFAULT_CAPACITY;
	vec->element_size = sizeof_element;
	vec->destructor = destructor;
	vec->copy = copy;
	vec->move = move;

	// create block to hold the vector and the data
	char *data = malloc(sizeof(zk_vector) + sizeof_element * ZK_VECTOR_DEFAULT_CAPACITY);
	if (!data) {
		free(vec);
		return NULL;
	}

	// copy the vector into the block
	memcpy(data, vec, sizeof(zk_vector));

	free(vec);

	return data + sizeof(zk_vector);
}

void zk_vector_free(void *data)
{
	if (!data)
		return;

	zk_vector *vec = ZK_VECTOR_GET_FROM_DATA(data);

	if (!vec)
		return;

	if (vec->destructor)
		for (size_t i = 0; i < vec->size; i++)
			vec->destructor((char *)data + i * vec->element_size);

	free(vec);
}

zk_status zk_vector_push_back(void **data, void *element, zk_constructor_mode_t mode)
{
	if (!data || !*data)
		return ZK_INVALID_ARGUMENT;

	zk_vector *vec = ZK_VECTOR_GET_FROM_DATA(*data);

	if (_zk_vector_needs_resize(vec) && !_zk_vector_resize(vec))
		return ZK_ERROR_ALLOC;

	// set the new head
	*data = ZK_VECTOR_GET_DATA_FROM_VEC(vec);

	if (mode == ZK_COPY_CONSTRUCTOR) {
		if (vec->copy)
			vec->copy(element, (char *)data + vec->size * vec->element_size);
		else
			memcpy((char *)data + vec->size * vec->element_size, element, vec->element_size);
	} else if (mode == ZK_MOVE_CONSTRUCTOR) {
		if (vec->move)
			vec->move(element, (char *)data + vec->size * vec->element_size);
		else
			memcpy((char *)data + vec->size * vec->element_size, element, vec->element_size);
	} else
		return ZK_INVALID_ARGUMENT;

	vec->size++;
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
// zk_status zk_vector_at(const zk_vector *const vec, size_t index, void **out)
// {
// 	if (!vec || index >= vec->size)
// 		return ZK_INVALID_ARGUMENT;

// 	*out = vec->data[index];
// 	return ZK_OK;
// }

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
// void *zk_vector_data(void *data)
// {
// 	if (!data)
// 		return NULL;
// 	return vec ? (void *)vec->self->data : NULL;
// }

/**
 * @brief Free a vector and all of its elements if a destructor function is provided.
 *
 * @param vec The vector to free
 * @param func The destructor function to call on each element of the vector. It can be NULL.
 *
 * @note Time complexity: O(1)
 * @note Space complexity: O(1)
 */

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
// void **zk_vector_insert(zk_vector *vec, size_t index, void *data)
// {
// 	if (!vec)
// 		return NULL;

// 	if (index >= vec->size) {
// 		vec->capacity = index + 1;
// 		vec->size = index + 1;
// 		if (!_zk_vector_resize(vec))
// 			return NULL;
// 	}

// 	vec->data[index] = data;
// 	return &vec->data[index];
// }

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
// void zk_vector_pop_back(zk_vector *vec, zk_destructor_t func)
// {
// 	if (!vec)
// 		return;

// 	if (vec->size) {
// 		vec->size--;
// 		if (func)
// 			func(vec->data[vec->size]);

// 		vec->data[vec->size] = NULL;
// 	}
// }

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
// void zk_vector_pop_front(zk_vector *vec, zk_destructor_t func)
// {
// 	if (!vec)
// 		return;

// 	if (vec->size) {
// 		vec->size--;
// 		if (func)
// 			func(vec->data[0]);

// 		for (size_t i = 0; i < vec->size; i++)
// 			vec->data[i] = vec->data[i + 1];

// 		vec->data[vec->size] = NULL;
// 	}
// }

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
// void zk_vector_push_front(zk_vector *vec, void *data)
// {
// 	if (!vec)
// 		return;

// 	if (_zk_vector_needs_resize(vec) && !_zk_vector_resize(vec))
// 		return;

// 	size_t i = vec->size;
// 	while (i) {
// 		vec->data[i] = vec->data[i - 1];
// 		i--;
// 	}

// 	vec->data[0] = data;
// 	vec->size++;
// }

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
