#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "zk/zklib.h"

void print_list(void *data, void *user_data)
{
	ZK_UNUSED(user_data);
	printf("data: %s\n", (char *)data);
}

int main()
{
	zk_slist *list = NULL;

	list = zk_slist_push_back(list, strdup("0"));
	list = zk_slist_push_back(list, strdup("1"));
	list = zk_slist_push_back(list, strdup("2"));
	list = zk_slist_push_back(list, strdup("3"));

	// remove last element and use free function to free the element`s data
	list = zk_slist_pop_back(list, free);

	// calls zk_for_each() to print each node`s data of the list
	zk_slist_for_each(zk_slist_begin(list), zk_slist_end(list), print_list, NULL);

	// frees list
	zk_slist_free(&list, free);

	return 0;
}
