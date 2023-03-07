#include <stdio.h>
#include <stdlib.h>

#include "zk/zklib.h"

void print_list(void *data, void *user_data)
{
	ZK_UNUSED(user_data);
	if (data != NULL)
		printf("data: %s\n", (char *)data);
}

int main()
{
	zk_c_dlist *list = NULL, *begin = NULL, *end = NULL;
	void *node_data = NULL;

	zk_push_back(&list, "top");
	zk_push_back(&list, "middle");
	zk_push_back(&list, "end");

	begin = zk_begin(list);
	end = zk_end(list);

	// Uses begin and end to iterate the list.
	while (begin != end) {
		if (zk_get_data(begin, &node_data) != ZK_OK || node_data == NULL)
			printf("Error getting data from node.");
		else
			print_list(node_data, NULL);

		if (zk_next(begin, &begin) != ZK_OK) {
			printf("Error getting next node.");
			break;
		}
	}
	// print last element
	if (zk_get_data(end, &node_data) != ZK_OK || node_data == NULL)
		printf("Error getting data from node.");
	else
		print_list(node_data, NULL);

	// frees list
	zk_free(&list, NULL);

	return 0;
}
