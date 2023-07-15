#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "zk_vec/zk_vec.h"

#define ZK_VEC_DEFAULT_CAPACITY       8
#define ZK_VEC_GROWTH_FACTOR          2
#define ZK_VEC_TYPE_SIZE              sizeof(zk_vec)
#define ZK_VEC_GET_FROM_DATA(data)    (zk_vec *)((char *)data - ZK_VEC_TYPE_SIZE)
#define ZK_VEC_GET_DATA_FROM_VEC(vec) (void *)((char *)vec + ZK_VEC_TYPE_SIZE)
#define ZK_VEC_POINTER_SIZE           sizeof(void *)

// BEGIN macros
#define ZK_VEC_BEGIN_SIZE(vec)       (vec).element_size
#define ZK_VEC_BEGIN_FROM_VEC(vec)   (char *)vec + ZK_VEC_TYPE_SIZE
#define ZK_VEC_BEGIN_FROM_DATA(data) (char *)data
// END macros
#define ZK_VEC_END_SIZE(vec)     (vec).element_size
#define ZK_VEC_END_FROM_VEC(vec) (char *)vec + ZK_VEC_TYPE_SIZE + vec->size * vec->element_size
#define ZK_VEC_END_FROM_DATA(data) \
	ZK_VEC_BEGIN_FROM_DATA(data) + (ZK_VEC_GET_FROM_DATA(data))->size *(ZK_VEC_GET_FROM_DATA(data))->element_size

/**
 * @brief vector struct
 */
struct zk_vec {
	size_t size; // number of elements in the vector
	size_t capacity; // total capacity of the vector
	size_t element_size; // size of each element of the vector
	zk_destructor_t destructor; // destructor function for each element of the vector
	zk_copy_t copy; // copy function for each element of the vector
	zk_move_t move; // move function for each element of the vector
};
typedef struct zk_vec zk_vec;

static bool _zk_vector_needs_resize(void *data)
{
	if (!data)
		return false;

	zk_vec *vec = ZK_VEC_GET_FROM_DATA(data);
	return vec ? vec->size == vec->capacity : false;
}

static bool _zk_vector_resize(void **data)
{
	if (!data)
		return false;

	zk_vec *vec = ZK_VEC_GET_FROM_DATA(*data);
	if (!vec)
		return false;

	size_t new_capacity = vec->capacity * ZK_VEC_GROWTH_FACTOR;
	char *new_data = realloc(vec, ZK_VEC_TYPE_SIZE + new_capacity * vec->element_size + ZK_VEC_END_SIZE(*vec));
	if (!new_data)
		return false;

	vec = (zk_vec *)new_data;
	vec->capacity = new_capacity;
	*data = ZK_VEC_GET_DATA_FROM_VEC(vec);

	return true;
}

static void _zk_vec_copy_element(zk_vec *vec, void *element, void *data_pos)
{
	if (!vec || !element || !data_pos)
		return;

	if (vec->copy)
		vec->copy(element, data_pos);
	else
		memcpy(data_pos, element, vec->element_size);
}

static void _zk_vec_move_element(zk_vec *vec, void *element, void *data_pos)
{
	if (!vec || !element || !data_pos)
		return;

	if (vec->move)
		vec->move(element, data_pos);
	else
		memcpy(data_pos, element, vec->element_size);
}

static zk_status _zk_vec_set_element(void **data, void *element, zk_constructor_mode_t mode)
{
	if (!element || !data || !*data)
		return ZK_INVALID_ARGUMENT;

	zk_vec *vec = ZK_VEC_GET_FROM_DATA(*data);
	if (!vec)
		return ZK_INVALID_ARGUMENT;

	size_t index = vec->size * vec->element_size;
	char *data_pos = (char *)(*data) + index;

	if (mode == ZK_COPY_CONSTRUCTOR)
		_zk_vec_copy_element(vec, element, data_pos);
	else if (mode == ZK_MOVE_CONSTRUCTOR)
		_zk_vec_move_element(vec, element, data_pos);
	else
		return ZK_INVALID_ARGUMENT;

	vec->size++;
	return ZK_OK;
}

/**
 * @brief Return the capacity of the vector.
 *
 * @param data The vector
 *
 * @return The capacity of the vector
 *
 * @note Time complexity: O(1)
 * @note Space complexity: O(1)
 */
size_t zk_vector_capacity(void *data)
{
	zk_vec *vec = ZK_VEC_GET_FROM_DATA(data);
	return vec ? vec->capacity : 0;
}

/**
 * @brief Free a vector and its data.
 *
 * @param data The vector to free. If a destructor was provided when the vector was created, it will be called on each
 * element of the vector.
 *
 * @note Time complexity: O(1)
 * @note Space complexity: O(1)
 */
void zk_vector_free(void *data)
{
	if (!data)
		return;

	zk_vec *vec = ZK_VEC_GET_FROM_DATA(data);
	if (!vec)
		return;

	if (vec->destructor) {
		for (size_t i = 0; i < vec->size; i++)
			vec->destructor((char *)data + i * vec->element_size);
	}

	free((char *)vec);
}

// used to destroy a vector of vectors
void zk_vector_destructor(void *data)
{
	if (!data)
		return;

	// deference the vector of vectors
	// data is a pointer to a data segment that contains a vector
	// the vector is a pointer to a vector

	// zk_vec *vec = ZK_VEC_GET_FROM_DATA(*((char **)data));

	zk_vector_free(*((char **)data));
}

/**
 * @brief Create a new vector, with a default capacity of 8. All elements are initialized to NULL.
 *
 * @param sizeof_element The size of each element of the vector
 * @param destructor The destructor function to call on each element of the vector. It will be called when an element in
 * the array data segment is removed and when the array is freed and data segment is deallocated as well. It will be
 * passed a pointer to the element to clear, rather than the element itself. This function is expected to clear the
 * contents of the array element it is given, but not free the element itself.
 * @param copy The copy function to be called when adding elements to the vector when the intent is to copy the element.
 * This function shall perform a deep copy of the element. It can be NULL if the element is not to be copied.
 * @param move The move function to be called when adding elements to the vector when the intent is to move the element
 * into the array instead of copying it. TI shall be implemented as a C++ move constructor. It can be NULL if the
 * element is not to be moved.
 *
 * @return A pointer to the vector or NULL if an error occurred
 *
 * @note Time complexity: O(1)
 * @note Space complexity: O(1)
 */
void *_zk_vec_new(size_t sizeof_element, zk_destructor_t destructor, zk_copy_t copy, zk_move_t move)
{
	zk_vec vec = {
		.size = 0,
		.capacity = ZK_VEC_DEFAULT_CAPACITY,
		.element_size = sizeof_element,
		.destructor = destructor,
		.copy = copy,
		.move = move,
	};

	// create block to hold the vector and the data
	char *data = malloc(ZK_VEC_TYPE_SIZE + sizeof_element * ZK_VEC_DEFAULT_CAPACITY + ZK_VEC_END_SIZE(vec));
	if (!data) {
		return NULL;
	}

	// copy the vector into the block
	memcpy(data, &vec, ZK_VEC_TYPE_SIZE);

	return data + ZK_VEC_TYPE_SIZE;
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
// void zk_vector_pop_back(zk_vec *vec, zk_destructor_t func)
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
// void zk_vector_pop_front(zk_vec *vec, zk_destructor_t func)
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
 * @brief Add an element to the back of the vector.
 *
 * @param data The vector
 * @param element The element to add
 * @param mode The constructor mode, either ZK_COPY_CONSTRUCTOR or ZK_MOVE_CONSTRUCTOR.
 *
 * @return ZK_OK if the element was added, ZK_ERROR_ALLOC if the vector needed to be resized but the resize failed, or
 * ZK_INVALID_ARGUMENT if the arguments are invalid.
 *
 * @note Time complexity: O(1) amortized
 * @note Space complexity: O(1) amortized
 */
zk_status _zk_vec_push_back(void **data, void *element, zk_constructor_mode_t mode)
{
	if (!data || !*data)
		return ZK_INVALID_ARGUMENT;

	if (_zk_vector_needs_resize(*data) && !_zk_vector_resize(data))
		return ZK_ERROR_ALLOC;

	if (_zk_vec_set_element(data, element, mode) != ZK_OK)
		return ZK_INVALID_ARGUMENT;

	return ZK_OK;
}

void zk_vec_move(void *const src, void *const begin, void *const end, void **dst)
{
	if (!src || !begin || !end || !dst || !*dst)
		return;

	zk_vec *src_vec = ZK_VEC_GET_FROM_DATA(src);
	zk_vec *dst_vec = ZK_VEC_GET_FROM_DATA(dst);
	if (!src_vec || !dst_vec)
		return;

	size_t size = (char *)end - (char *)begin;

	// check if resize is needed in the destination vector
	if (dst_vec->size + size > dst_vec->capacity) {
		// TODO: change _zk_vector_resize to data data and new size so this logic can be reused
		size_t new_capacity = dst_vec->size + size;
		char *new_data = realloc(dst_vec, ZK_VEC_TYPE_SIZE + new_capacity * dst_vec->element_size);
		if (!new_data)
			return;

		dst_vec = (zk_vec *)new_data;
		dst_vec->capacity = new_capacity;
		*dst = ZK_VEC_GET_DATA_FROM_VEC(dst_vec);
	}

	// move the data, which is the same as copying it since vectors are containers
	memcpy((char *)*dst + dst_vec->size * dst_vec->element_size, begin, size);
	dst_vec->size += size;
}

void zk_vector_move_constructor(void *src, void *dst)
{
	zk_vector_copy_constructor(src, dst);
}

// FIXME: this is not a copy constructor, is does not copy the data pointed to by src
void zk_vector_copy_constructor(const void *const src, void *dst)
{
	if (!src || !dst)
		return;

	// vector of vectors are always vectors of pointers
	memcpy(dst, &src, ZK_VEC_POINTER_SIZE);
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
// void zk_vector_push_front(zk_vec *vec, void *data)
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
 * @param data The vector
 *
 * @return The number of elements in the vector
 *
 * @note Time complexity: O(1)
 * @note Space complexity: O(1)
 */
size_t zk_vector_size(void *data)
{
	zk_vec *vec = ZK_VEC_GET_FROM_DATA(data);
	return vec ? vec->size : 0;
}

/* #### Iterator functions #### */

/*!
   Iterator invalidation rules:
  -  All read only operations: Never
  - clear: always
  - insert:  if the vector changed capacity, all iterators are invalidated. If not, only the iterators after the
   insertion point.
  - erase: erased element and all iterators after the erased element, including the end iterator.
  - push_back:  if the vector changed capacity, all iterators are invalidated. If not, only the end iterator.
  - pop_back:  the element erased and end iterator.
  - resize: if the vector changed capacity, all of them. If not, only end() and any elements erased.
  - reserve/shrink_to_fit: if the vector changed capacity, all of them. If not, none.

*/

/**
 * @brief Return an iterator to the first element of the vector. If the vector is empty, the returned iterator will be
 * equal to the end iterator.
 *
 * @param data The vector
 *
 * @return An iterator to the first element of the vector
 *
 * @note Time complexity: O(1)
 * @note Space complexity: O(1)
 */
void *zk_vec_begin(void *data)
{
	return ZK_VEC_BEGIN_FROM_DATA(data);
}

/**
 * @brief Return an iterator to the element following the last element of the vector. This element acts as a
 * placeholder, attempting to access it results in undefined behavior.
 *
 * @param data The vector
 *
 * @return An iterator to the element following the last element of the vector
 *
 * @note Time complexity: O(1)
 * @note Space complexity: O(1)
 */
void *zk_vec_end(void *data)
{
	return ZK_VEC_END_FROM_DATA(data);
}
