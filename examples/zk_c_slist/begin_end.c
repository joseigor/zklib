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
	void *data = NULL;

	zk_push_back(&list, "top");
	zk_push_back(&list, "middle");
	zk_push_back(&list, "end");

	zk_c_slist *begin = zk_begin(list);
	zk_c_slist *end = zk_end(list);

	// Uses begin and end to iterate the list.
	while (begin != end) {
		zk_get_data(begin, &data);
		print_list(data, NULL);
		zk_next(begin, &begin);
	}
	// print last element
	zk_get_data(begin, &data);
	print_list(data, NULL);

	// frees list
	zk_free(&list, NULL);

	return 0;
}
