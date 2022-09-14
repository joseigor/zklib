#include <stdio.h>
#include <stdlib.h>

#include "zk/zklib.h"

void print_list(void *data, void *user_data)
{
	ZK_UNUSED(user_data);
	printf("data: %s\n", (char *)data);
}

int main()
{
	zk_dlist *list = NULL;

	zk_push_back(&list, "1");
	zk_push_back(&list, "2");
	zk_push_back(&list, "3");

	// prepends new node with data to the list.
	zk_push_front(&list, "0");

	// calls zk_for_each() to print each node`s data of the list
	zk_for_each(list, print_list, NULL);

	// frees list
	zk_free(&list, NULL);

	return 0;
}
