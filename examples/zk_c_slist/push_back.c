#include <stdio.h>
#include <stdlib.h>

#include "zk/zklib.h"

struct custom_data {
	int value;
	char *string;
};

void print_string_list(void *data, void *user_data)
{
	ZK_UNUSED(user_data);
	printf("data: %s\n", (char *)data);
}

void print_number_list(void *data, void *user_data)
{
	ZK_UNUSED(user_data);
	printf("data: %d\n", *((int *)data));
}

void print_custom_list(void *data, void *user_data)
{
	ZK_UNUSED(user_data);
	struct custom_data *c_data = data;

	printf("custom_data: %d %s\n", c_data->value, c_data->string);
}

int main()
{
	zk_c_slist *number_list = NULL, *string_list = NULL, *custom_list = NULL;

	int node1_data = 1;
	int node2_data = 2;

	// This is a list of integers.
	zk_push_back(&number_list, &node1_data);
	zk_push_back(&number_list, &node2_data);

	// This is a list of strings.
	zk_push_back(&string_list, "first");
	zk_push_back(&string_list, "second");

	struct custom_data custom_node_1 = { .value = 1, .string = "node1" };
	struct custom_data custom_node_2 = { .value = 2, .string = "node2" };
	struct custom_data custom_node_3 = { .value = 3, .string = "node3" };

	// This is a list of custom data.
	zk_push_back(&custom_list, &custom_node_1);
	zk_push_back(&custom_list, &custom_node_2);
	zk_push_back(&custom_list, &custom_node_3);

	// print all lists
	printf("number_list\n");
	zk_for_each(number_list, print_number_list, NULL);
	printf("\nstring_list\n");
	zk_for_each(string_list, print_string_list, NULL);
	printf("\ncustom_list\n");
	zk_for_each(custom_list, print_custom_list, NULL);

	// free lists after use
	zk_free(&number_list, NULL);
	zk_free(&string_list, NULL);
	zk_free(&custom_list, NULL);

	return 0;
}
