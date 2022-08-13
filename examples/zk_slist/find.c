#include <stdio.h>
#include <stdlib.h>

#include "zk/zklib.h"

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

	if ((list_node = zk_slist_find(list, &node2_data))) {
		printf("Node data: %d\n", *((int *)list_node->data));
	} else {
		printf("Node not found.\n");
	}

	if ((list_node = zk_slist_find(list, &node4_data))) {
		printf("Node data: %d\n", *((int *)list_node->data));
	} else {
		printf("Node not found.\n");
	}

	// free lists after use
	zk_slist_free(&list);

	return 0;
}
