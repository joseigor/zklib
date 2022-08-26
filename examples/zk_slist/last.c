#include <stdio.h>
#include <stdlib.h>

#include "zk/zklib.h"

int main()
{
	zk_slist_t *list = NULL;

	list = zk_push_back(list, "1");
	list = zk_push_back(list, "2");
	list = zk_push_back(list, "3");

	printf("Last node: %s \n", (char *)zk_slist_last(list)->data);

	// frees list
	zk_slist_free(&list, NULL);

	return 0;
}
