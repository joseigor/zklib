#ifndef ZK_COMMON_H
#define ZK_COMMON_H

#include <stdbool.h>

#define ZK_UNUSED(x) (void)(x)
#define ZK_COPY(x)   (x), (ZK_COPY_CONSTRUCTOR)
#define ZK_MOVE(x)   (x), (ZK_MOVE_CONSTRUCTOR)

typedef int (*zk_compare_func)(const void *const a, const void *const b);

typedef bool (*zk_predicate_func)(const void *const data, void *user_data);

typedef void *(*zk_copy_data_t)(const void *const data, void *user_data);

typedef void (*zk_destructor_t)(void *data);

typedef void (*zk_for_each_func)(void *data, void *user_data);

typedef void (*zk_copy_t)(const void *const src, void *dst);

typedef void (*zk_move_t)(void *src, void *dst);

#define ZK_DESTRUCTOR(func) ((zk_destructor_t)(func))

typedef enum zk_status {
	ZK_OK = 0,
	ZK_ERROR_ALLOC = 1,
	ZK_INVALID_ARGUMENT = 2,
} zk_status;

typedef enum zk_constructor_mode {
	ZK_COPY_CONSTRUCTOR = 0,
	ZK_MOVE_CONSTRUCTOR = 1,
} zk_constructor_mode_t;

#endif
