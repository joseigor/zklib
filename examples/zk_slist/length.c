#include <stdio.h>
#include <stdlib.h>

#include "zk/zklib.h"

int main()
{
	zk_slist *list = NULL;

	// add 10 elements to the list
	for (int i = 0; i < 10; i++)
		zk_push_back(&list, "1");

	printf("List length: %ld\n", zk_length(list));

	// frees list
	zk_free(&list, NULL);

	return 0;
}
