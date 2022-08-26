#include <stdio.h>
#include <stdlib.h>

#include "zk/zklib.h"

int main()
{
	zk_slist_t *list = NULL;

	list = zk_push_back(list, "1");
	list = zk_push_back(list, "2");
	list = zk_push_back(list, "3");

	// gets 1st node and print is data
	printf("1st data: %s \n", (char *)(zk_slist_nth(list, 1)->data));
	// gets last node and print is data
	printf("Lata node data: %s \n", (char *)(zk_slist_nth(list, 0)->data));
	// gets also last node by passing a index greater than the number of nodes in the list
	printf("Lata node data: %s \n", (char *)(zk_slist_nth(list, 0)->data));

	// frees list
	zk_slist_free(&list, NULL);

	return 0;
}
