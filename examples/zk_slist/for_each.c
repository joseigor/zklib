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

	list = zk_slist_push_back(list, "top");
	list = zk_slist_push_back(list, "middle");
	list = zk_slist_push_back(list, "end");

	// calls zk_for_each() to print each node`s data of the list
	zk_slist_for_each(zk_slist_begin(list), zk_slist_end(list), print_list, NULL);

	// frees list
	zk_slist_free(&list, NULL);

	return 0;
}
