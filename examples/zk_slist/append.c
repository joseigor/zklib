#include <stdio.h>
#include <stdlib.h>

#include "zk/zklib.h"

int main()
{
	zk_slist_t *number_slist = NULL, *string_list = NULL, *custom_slist = NULL;

	int node1_data = 1;
	int node2_data = 2;

	// This is a list of integers.
	number_slist = zk_slist_append(number_slist, &node1_data);
	number_slist = zk_slist_append(number_slist, &node2_data);

	// This is a list of strings.
	string_list = zk_slist_append(string_list, "first");
	string_list = zk_slist_append(string_list, "second");

	struct custom_data {
		int value;
		char *string;
	};

	struct custom_data custom_node_1 = { .value = 1, .string = "node1" };
	struct custom_data custom_node_2 = { .value = 2, .string = "node2" };
	struct custom_data custom_node_3 = { .value = 3, .string = "node3" };

	// This is a list of custom data.
	custom_slist = zk_slist_append(custom_slist, &custom_node_1);
	custom_slist = zk_slist_append(custom_slist, &custom_node_2);
	custom_slist = zk_slist_append(custom_slist, &custom_node_3);

	// print all lists
	printf("number_slist->data: %d\n", *(int *)number_slist->data);
	printf("number_slist->next->data: %d\n", *(int *)number_slist->next->data);
	printf("\n");
	printf("string_list->data: %s\n", (char *)string_list->data);
	printf("string_list->next->data: %s\n", (char *)string_list->next->data);
	printf("\n");
	struct custom_data *custom_node = custom_slist->data;
	printf("custom_slist->data: %d %s\n", custom_node->value, custom_node->string);
	custom_node = custom_slist->next->data;
	printf("custom_slist->next->data: %d %s\n", custom_node->value, custom_node->string);

	// free lists after use
	zk_slist_free(&number_slist, NULL);
	zk_slist_free(&string_list, NULL);
	zk_slist_free(&custom_slist, NULL);

	return 0;
}
