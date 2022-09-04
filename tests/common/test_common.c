#include <stdlib.h>

#include "common/test_common.h"
#include "zk/zklib.h"

void dummy_node_data_free_foreach(void *data, void *user_data)
{
	ZK_UNUSED(user_data);
	struct dummy_node_data *node_data = data;
	free(node_data->string);
	free(node_data);
}

void dummy_node_data_free(void *data)
{
	struct dummy_node_data *node_data = data;
	free(node_data->string);
	free(node_data);
}
