#include <stdio.h>
#include <stdlib.h>

#include "zk/zklib.h"

int main()
{
	zk_slist_t *list = NULL, *empty_list = NULL;

	list = zk_push_back(list, "1");
	list = zk_push_back(list, "2");
	list = zk_push_back(list, "3");

	printf("list length: %ld \n", zk_slist_length(list));

	printf("empty_list length: %ld \n", zk_slist_length(empty_list));

	// frees list, no need to free empty_list as it is empty
	zk_slist_free(&list, NULL);

	return 0;
}
