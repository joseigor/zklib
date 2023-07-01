#pragma once

#include <stddef.h>

#include "zk_common/zk_common.h"

typedef struct zk_vector zk_vector;

size_t zk_vector_capacity(const zk_vector *const vector);

void zk_vector_free(zk_vector *vector, zk_destructor_t func);

zk_vector *zk_vector_new(void);

size_t zk_vector_size(const zk_vector *const vector);
