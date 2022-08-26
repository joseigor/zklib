#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "zk/zklib.h"

void print_list(void *data, __attribute__((__unused__)) void *user_data)
{
	printf("data: %s\n", (char *)data);
}

int main()
{
	zk_slist *list = NULL, *list_cp = NULL;

	list = zk_push_back(list, strdup("This"));
	list = zk_push_back(list, strdup("list"));
	list = zk_push_back(list, strdup("will"));
	list = zk_push_back(list, strdup("be"));
	list = zk_push_back(list, strdup("shallow"));
	list = zk_push_back(list, strdup("copied."));

	// creates a shallow copy of list
	list_cp = zk_slist_copy(list);

	// print list_cp
	zk_for_each(list_cp, print_list, NULL);

	// As this is a shallow copy if one of the lists is freed the other list loses its reference to data.
	zk_free(&list, free);
	// Note that from now on all list_cp nodes have dangling pointers to data
	zk_free(&list_cp, NULL);

	return 0;
}
