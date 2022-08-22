#include <stdio.h>
#include <stdlib.h>

#include "zk/zklib.h"

int cmp_int(const void *const data, const void *const user_data)
{
	if (*(int *)data == *(int *)user_data)
		return 0;
	return -1;
}

int main()
{
	zk_slist_t *list = NULL, *list_node = NULL;

	int node1_data = 1;
	int node2_data = 2;
	int node3_data = 3;
	int node4_data = 4;

	// This is a list of integers.
	list = zk_slist_append(list, &node1_data);
	list = zk_slist_append(list, &node2_data);
	list = zk_slist_append(list, &node3_data);
	list = zk_slist_append(list, &node4_data);

	// Find without a custom function
	if ((list_node = zk_slist_find(list, &node2_data, NULL))) {
		printf("Node data: %d\n", *((int *)list_node->data));
	}

	// Make use of a custom funtion to find the node
	if ((list_node = zk_slist_find(list, &node2_data, cmp_int))) {
		printf("Node data: %d\n", *((int *)list_node->data));
	}

	// free lists after use
	zk_slist_free(&list, NULL);

	return 0;
}
