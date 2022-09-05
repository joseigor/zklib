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

	zk_push_back(&list, "top");
	zk_push_back(&list, "middle");
	zk_push_back(&list, "end");

	zk_slist *begin = zk_begin(list);
	zk_slist *end = zk_end(list);

	// Uses begin and end to iterate the list.
	for (; begin != end; begin = begin->next) {
		print_list(begin->data, NULL);
	}

	// frees list
	zk_free(&list, NULL);

	return 0;
}
