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

	list = zk_push_back(list, "top");
	list = zk_push_back(list, "middle");
	list = zk_push_back(list, "end");

	// calls zk_for_each() to print each node`s data of the list
	zk_for_each(list, print_list, NULL);

	// frees list
	zk_free(&list, NULL);

	return 0;
}
