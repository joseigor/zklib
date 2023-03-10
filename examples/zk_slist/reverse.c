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

	zk_push_back(&list, "1");
	zk_push_back(&list, "2");
	zk_push_back(&list, "3");

	printf("Original list:\n");
	zk_for_each(list, print_list, NULL);

	zk_reverse(&list);

	printf("Reversed list:\n");
	zk_for_each(list, print_list, NULL);

	// frees list
	zk_free(&list, NULL);

	return 0;
}
