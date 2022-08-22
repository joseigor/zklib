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

	list = zk_slist_append(list, "1");
	list = zk_slist_append(list, "3");

	// uses zk_slist_insert_before() to insert a new node in the middle of the list
	list = zk_slist_insert_before(list, list->next, "2");

	// calls zk_slist_foreach() to print each node`s data of the list
	zk_slist_foreach(list, print_list, NULL);

	// frees list
	zk_slist_free(&list, NULL);

	return 0;
}
