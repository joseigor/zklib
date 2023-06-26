#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "zk/zklib.h"

void print_list(void *data, void *user_data)
{
	ZK_UNUSED(user_data);
	printf("%d-", *(int *)data);
}

int zk_int_cmp(const void *const a, const void *const b)
{
	return *(int *)a - *(int *)b;
}

int main()
{
	zk_slist *list = NULL;
	zk_slist *other = NULL;

	int list_data[] = { 6, 7, 8, 9, 10 };
	int other_data[] = { 1, 2, 3, 4, 5 };

	for (int i = 0; i < 5; i++) {
		zk_slist_push_back(&list, &list_data[i]);
		zk_slist_push_back(&other, &other_data[i]);
	}

	// merge the two lists
	list = zk_slist_merge(list, other, zk_int_cmp);

	// print the merged list
	zk_slist_for_each(zk_begin(list), zk_end(list), print_list, NULL);
	printf("\n");

	// free the list, other_list was already freed by zk_slist_merge
	zk_free(&list, NULL);

	return 0;
}
