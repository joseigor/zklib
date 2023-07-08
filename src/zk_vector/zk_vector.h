#pragma once

#include <stddef.h>

#include "zk_common/zk_common.h"

typedef struct zk_vector zk_vector;

zk_status zk_vector_at(const zk_vector *const vec, size_t index, void **out);

size_t zk_vector_capacity(const zk_vector *const vector);

void **zk_vector_data(const zk_vector *const vector);

void zk_vector_free(zk_vector *vector, zk_destructor_t func);

void **zk_vector_insert(zk_vector *vec, size_t index, void *data);

zk_vector *zk_vector_new(void);

void zk_vector_pop_back(zk_vector *vec, zk_destructor_t func);

void zk_vector_pop_front(zk_vector *vec, zk_destructor_t func);

void zk_vector_push_back(zk_vector *vec, void *data);

void zk_vector_push_front(zk_vector *vec, void *data);

size_t zk_vector_size(const zk_vector *const vector);
