#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "zk/zklib.h"

void print_list(void *data, void *user_data)
{
	ZK_UNUSED(user_data);
	printf("%d-", *(int *)data);
}

int zk_int_cmp_asc(const void *const a, const void *const b)
{
	return *(int *)a - *(int *)b;
}

int zk_int_cmp_desc(const void *const a, const void *const b)
{
	return *(int *)b - *(int *)a;
}

int main()
{
	zk_slist *list = NULL;

	// initialize list with random data
	srand((unsigned int)time(NULL));

	for (int i = 0; i < 10; i++) {
		int *p = malloc(sizeof(int));
		*p = rand() % 100;
		zk_slist_push_front(&list, p);
	}

	printf("Before sorting:\n");
	zk_slist_for_each(list, NULL, print_list, NULL);
	printf("\n");

	// sorts the list
	list = zk_slist_sort(list, zk_int_cmp_asc);

	printf("After sorting ascending:\n");
	zk_slist_for_each(list, NULL, print_list, NULL);
	printf("\n");

	// sort the list descending
	list = zk_slist_sort(list, zk_int_cmp_desc);
	printf("After sorting descending:\n");
	zk_slist_for_each(list, NULL, print_list, NULL);
	printf("\n");

	// frees list
	zk_slist_free(&list, free);

	return 0;
}
