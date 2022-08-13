#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "zk/zklib.h"

void print_list(void *data, __attribute__((__unused__)) void *user_data)
{
	printf("data: %s\n", (char *)data);
}

void *copy_list_data(const void *data, void *user_data)
{
	printf("User data: %s \n", (char *)user_data);
	return strdup((char *)data);
}

int main()
{
	zk_slist_t *list = NULL, *list_cp = NULL;

	list = zk_slist_append(list, strdup("This"));
	list = zk_slist_append(list, strdup("list"));
	list = zk_slist_append(list, strdup("will"));
	list = zk_slist_append(list, strdup("be"));
	list = zk_slist_append(list, strdup("deep"));
	list = zk_slist_append(list, strdup("copied."));

	// creates a deep copy of list
	list_cp = zk_slist_copy_deep(list, copy_list_data, "My own data");

	// print list_cp
	zk_slist_foreach(list_cp, print_list, NULL);

	zk_slist_free_full(&list, free);
	// use zk_slist_free_full() to also free list_cp
	zk_slist_free_full(&list_cp, free);

	return 0;
}
