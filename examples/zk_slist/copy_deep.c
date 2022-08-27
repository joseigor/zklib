#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "zk/zklib.h"

void print_list(void *data, void *user_data)
{
	ZK_UNUSED(user_data);
	printf("data: %s\n", (char *)data);
}

void *copy_list_data(const void *data, void *user_data)
{
	printf("User data: %s \n", (char *)user_data);
	return strdup((char *)data);
}

int main()
{
	zk_slist *list = NULL, *list_cp = NULL;

	list = zk_push_back(list, strdup("This"));
	list = zk_push_back(list, strdup("list"));
	list = zk_push_back(list, strdup("will"));
	list = zk_push_back(list, strdup("be"));
	list = zk_push_back(list, strdup("deep"));
	list = zk_push_back(list, strdup("copied."));

	// creates a deep copy of list
	list_cp = zk_slist_copy_deep(list, copy_list_data, "My own data");

	// print list_cp
	zk_for_each(list_cp, print_list, NULL);

	zk_free(&list, free);
	zk_free(&list_cp, free);

	return 0;
}
