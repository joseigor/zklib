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

	zk_slist *begin = zk_slist_begin(list);
	zk_slist *end = zk_slist_end(list);

	// Uses begin and end to iterate the list.
	while (begin != end) {
		print_list(begin->data, NULL);
		begin = begin->next;
	}

	// frees list
	zk_slist_free(&list, NULL);

	return 0;
}
