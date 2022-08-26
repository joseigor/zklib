#include <stdio.h>
#include <stdlib.h>

#include "zk/zklib.h"

void print_list(void *data, __attribute__((__unused__)) void *user_data)
{
	printf("data: %s\n", (char *)data);
}

int main()
{
	zk_slist_t *list = NULL;

	list = zk_push_back(list, "1");
	list = zk_push_back(list, "2");
	list = zk_push_back(list, "3");

	// prepends new node with data to the list.
	list = zk_slist_prepend(list, "0");

	// calls zk_slist_foreach() to print each node`s data of the list
	zk_slist_foreach(list, print_list, NULL);

	// frees list
	zk_free(&list, NULL);

	return 0;
}
