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
	zk_c_slist *list = NULL;

	list = zk_push_back(list, "top");
	list = zk_push_back(list, "middle");
	list = zk_push_back(list, "end");

	zk_c_slist *begin = zk_begin(list);
	zk_c_slist *end = zk_end(list);

	// Uses begin and end to iterate the list.
	for (; begin != end; begin = begin->next) {
		print_list(begin->data, NULL);
	}
	// print last element
	print_list(end->data, NULL);

	// frees list
	zk_free(&list, NULL);

	return 0;
}
