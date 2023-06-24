#include <stdio.h>
#include <stdlib.h>

#include "zk/zklib.h"

int main()
{
	zk_slist *list = NULL;

	// add 10 elements to the list
	for (int i = 0; i < 10; i++)
		zk_slist_push_back(&list, "1");

	printf("List length: %ld\n", zk_slist_size(list));

	zk_slist_free(&list, NULL);

	return 0;
}
