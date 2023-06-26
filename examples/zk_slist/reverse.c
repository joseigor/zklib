#include <stdio.h>
#include <stdlib.h>

#include "zk/zklib.h"

void print_list(void *data, void *user_data)
{
	ZK_UNUSED(user_data);

	if (data == NULL)
		return;

	printf("data: %s\n", (char *)data);
}

int main()
{
	zk_slist *list = NULL;

	list = zk_slist_push_back(list, "1");
	list = zk_slist_push_back(list, "2");
	list = zk_slist_push_back(list, "3");

	printf("Original list:\n");
	zk_slist_for_each(zk_slist_begin(list), zk_slist_end(list), print_list, NULL);

	list = zk_slist_reverse(list);

	printf("Reversed list:\n");
	zk_slist_for_each(zk_slist_begin(list), zk_slist_end(list), print_list, NULL);

	zk_slist_free(&list, NULL);

	return 0;
}
