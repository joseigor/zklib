#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "zk/zklib.h"

int custom_compare_func(const void *const node_data, const void *const user_data)
{
	// let`s first handle cases where node_data or user_date could be null
	if (node_data == NULL && user_data != NULL)
		return -1;
	if (node_data != NULL && user_data == NULL)
		return -1;
	if (node_data == NULL && user_data == NULL)
		return 0;

	// neither node_data nor user_data  are NULL so we can cast ithem to integers
	const int a = *((int *)node_data);
	const int b = *((int *)user_data);

	return a == b ? 0 : -1;
}

int main()
{
	zk_slist_t *list = NULL;
	int node_1 = 1, node_2 = 2, node_3 = 3, node_4 = 4;
	size_t index = 0;

	// creates a list of integers with some NULL nodes
	list = zk_push_back(list, &node_1);
	list = zk_push_back(list, &node_2);
	list = zk_push_back(list, NULL);
	list = zk_push_back(list, &node_3);
	list = zk_push_back(list, NULL);
	list = zk_push_back(list, &node_4);

	// find indexes without custom function
	index = zk_slist_index(list, &node_2, NULL);
	assert(index == 2);
	// find index of NULL in this case first match will be returned
	index = zk_slist_index(list, NULL, NULL);
	assert(index == 3);

	// use a custom function to find indexes
	index = zk_slist_index(list, &node_2, custom_compare_func);
	assert(index == 2);
	index = zk_slist_index(list, NULL, custom_compare_func);
	assert(index == 3);

	zk_slist_free(&list, NULL);

	return 0;
}
