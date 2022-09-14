#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "zk/zklib.h"

struct custom_data {
	int value;
	char *string;
};

void custom_data_free(void *data)
{
	struct custom_data *node_data = data;
	free(node_data->string);
	free(node_data);
}

int main()
{
	zk_c_dlist *list = NULL;

	// create some instances of struct custom_data in put them in the main stack frame.
	struct custom_data custom_node_1 = { .value = 1, .string = "node1" };
	struct custom_data custom_node_2 = { .value = 2, .string = "node2" };
	struct custom_data custom_node_3 = { .value = 3, .string = "node3" };

	// this is a list of custom data.
	zk_push_back(&list, &custom_node_1);
	zk_push_back(&list, &custom_node_2);
	zk_push_back(&list, &custom_node_3);

	// use your list ...

	// in this case there is no need to free the memory the list node are pointing to because they are pointing to
	// static allocated memory (no-heap object) so func argument of zk_free() can be NULL
	zk_free(&list, NULL);

	// now lets dynamically allocate some data of type struct custom_data and insert them in a list. Let`s just
	// reuse the previous list that by now is clean due to previous call to zk_free().

	struct custom_data *node_1_data = malloc(sizeof(struct custom_data));
	node_1_data->value = 1;
	node_1_data->string = strdup("node_1");

	struct custom_data *node_2_data = malloc(sizeof(struct custom_data));
	node_2_data->value = 2;
	node_2_data->string = strdup("node_2");

	struct custom_data *node_3_data = malloc(sizeof(struct custom_data));
	node_3_data->value = 3;
	node_3_data->string = strdup("node_3");

	zk_push_back(&list, node_1_data);
	zk_push_back(&list, node_2_data);
	zk_push_back(&list, node_3_data);

	// use your list ..

	// In this case if you want to free the list but also the data each node is pointing to you can provide your
	// own function to free the data as bellow
	zk_free(&list, custom_data_free);

	return 0;
}
