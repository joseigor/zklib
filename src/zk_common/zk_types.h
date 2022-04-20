#pragma once

typedef void (*zk_destructor_t)(void *data);

typedef void *(*zk_copy_data_t)(const void *const data, void *user_data);
