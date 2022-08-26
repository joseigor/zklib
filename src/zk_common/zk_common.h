#ifndef ZK_COMMON_H
#define ZK_COMMON_H

#define ZK_UNUSED(x) (void)(x)

typedef int (*zk_compare_t)(const void *const data, const void *const user_data);

typedef void *(*zk_copy_data_t)(const void *const data, void *user_data);

typedef void (*zk_destructor_t)(void *data);

typedef void (*zk_foreach_t)(void *data, void *user_data);

#endif
