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
	zk_slist *list = NULL;

	list = zk_push_back(list, "1");
	list = zk_push_back(list, "3");

	// uses zk_slist_insert_before() to insert a new node in the middle of the list
	list = zk_slist_insert_before(list, list->next, "2");

	// calls zk_for_each() to print each node`s data of the list
	zk_for_each(list, print_list, NULL);

	// frees list
	zk_free(&list, NULL);

	return 0;
}
