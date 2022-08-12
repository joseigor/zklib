#include <stdio.h>
#include <stdlib.h>

#include "zk_slist/zk_slist.h"

void print_list(void *data, __attribute__((__unused__)) void *user_data)
{
	printf("data: %s\n", (char *)data);
}

int main()
{
	zk_slist_t *list = NULL, *list_top = NULL, *list_end = NULL;

	list = zk_slist_append(list, "middle");
	list_top = zk_slist_append(list_top, "top");
	list_end = zk_slist_append(list_end, "end");

	// add list_top to top of list
	list = zk_slist_concat(list_top, list);
	// add list_end to end of list
	list = zk_slist_concat(list, list_end);

	// print list
	zk_slist_foreach(list, print_list, NULL);

	// As list_top and list_list are linked to list only list needs to be freed
	zk_slist_free(&list);

	return 0;
}