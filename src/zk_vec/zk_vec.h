#pragma once

#include <stddef.h>

#include "zk_common/zk_common.h"

// FIXME: add macro documentation
#define zk_vec(type)      type *
#define ZK_VEC_CAST(data) ((void **)&data)

#define ZK_VEC_DESTRUCTOR(func) (zk_destructor_t)(func)
#define ZK_VEC_COPY(func)       (zk_copy_t)(func)
#define ZK_VEC_MOVE(func)       (zk_move_t)(func)

#define zk_vec_new(type, destructor, copy, move) \
	_zk_vec_new(sizeof(type), ZK_VEC_DESTRUCTOR(destructor), ZK_VEC_COPY(copy), ZK_VEC_MOVE(move))

#define zk_vec_push_back(vec, element, mode) _zk_vec_push_back(ZK_VEC_CAST(vec), element, mode)
#define zk_vec_push_back_copy(vec, element)  _zk_vec_push_back(ZK_VEC_CAST(vec), element, ZK_COPY_CONSTRUCTOR)
#define zk_vec_push_back_move(vec, element)  _zk_vec_push_back(ZK_VEC_CAST(vec), element, ZK_MOVE_CONSTRUCTOR)

size_t zk_vector_capacity(void *data);

void zk_vector_copy_constructor(const void *const src, void *dst);

void zk_vector_move_constructor(void *src, void *dst);

void zk_vector_destructor(void *data);

void zk_vector_free(void *data);

void *_zk_vec_new(size_t sizeof_element, zk_destructor_t destructor, zk_copy_t copy, zk_move_t move);

// void zk_vector_pop_back(zk_vector *vec, zk_destructor_t func);

// void zk_vector_pop_front(zk_vector *vec, zk_destructor_t func);

zk_status _zk_vec_push_back(void **data, void *element, zk_constructor_mode_t mode);

// void zk_vector_push_front(zk_vector *vec, void *data);

size_t zk_vector_size(void *data);

/* #### Iterator functions #### */

void *zk_vec_begin(void *data);
void *zk_vec_end(void *data);
