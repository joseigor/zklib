#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "zk/zklib.h"

struct dummy_node_data {
	int value;
	char *string;
};

void print_list(void *data, __attribute__((__unused__)) void *user_data)
{
	struct dummy_node_data *node_data = data;
	printf("data->value: %d\n", node_data->value);
	printf("data->string: %s\n", node_data->string);
}
void dummy_node_data_free(void *data)
{
	struct dummy_node_data *node_data = data;
	free(node_data->string);
	free(node_data);
}

int main()
{
	zk_slist_t *list = NULL;
	zk_slist_t *list_node = NULL;

	// create some custom data
	struct dummy_node_data *node_1_data = malloc(sizeof(struct dummy_node_data));
	node_1_data->value = 1;
	node_1_data->string = strdup("node_1");

	struct dummy_node_data *node_2_data = malloc(sizeof(struct dummy_node_data));
	node_2_data->value = 2;
	node_2_data->string = strdup("node_2");

	struct dummy_node_data *node_3_data = malloc(sizeof(struct dummy_node_data));
	node_3_data->value = 3;
	node_3_data->string = strdup("node_3");

	struct dummy_node_data *node_to_delete = malloc(sizeof(struct dummy_node_data));
	node_to_delete->value = 3;
	node_to_delete->string = strdup("node_to_delete");

	// create list_node
	list_node = zk_push_back(list_node, node_to_delete);

	// creates list
	list = zk_push_back(list, node_1_data);
	list = zk_push_back(list, node_2_data);
	// insert list_node into the list
	list = zk_slist_concat(list, list_node);
	// add more data to list
	list = zk_push_back(list, node_3_data);

	// print list
	zk_slist_foreach(list, print_list, NULL);

	// remove list_node from list
	list = zk_slist_delete_node(list, list_node, dummy_node_data_free);

	printf("\nlist after list_node was delete\n");

	// print list after list_node was delete
	zk_slist_foreach(list, print_list, NULL);

	// free list
	zk_free(&list, dummy_node_data_free);

	// There is no need to free list_node as zk_slist_delete_node() already did it.

	return 0;
}
