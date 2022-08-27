#include <stdio.h>
#include <stdlib.h>

#include "zk/zklib.h"

void print_list(void *data, __attribute__((__unused__)) void *user_data)
{
	printf("data: %s\n", (char *)data);
}

int main()
{
	zk_dlist *list = NULL;

	list = zk_push_back(list, "top");
	list = zk_push_back(list, "middle");
	list = zk_push_back(list, "end");

	zk_dlist *begin = zk_begin(list);
	zk_dlist *end = zk_end(list);

	// Uses begin and end to iterate the list.
	for (; begin != end; begin = begin->next) {
		print_list(begin->data, NULL);
	}

	// frees list
	zk_free(&list, NULL);

	return 0;
}
