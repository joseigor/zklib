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

	list = zk_slist_append(list, "top");
	list = zk_slist_append(list, "middle");
	list = zk_slist_append(list, "end");

	// calls zk_slist_foreach() to print each node`s data of the list before it is reversed
	zk_slist_foreach(list, print_list, NULL);

	// reverses the list
	list = zk_slist_reverse(list);

	printf("----- list reversed -----\n");

	// prints the reversed list
	zk_slist_foreach(list, print_list, NULL);

	// frees list
	zk_slist_free(&list, NULL);

	return 0;
}
