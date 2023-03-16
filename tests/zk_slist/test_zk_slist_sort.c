#include <stdlib.h>

#include "unity.h"
#include "zk/zklib.h"

void setUp(void)
{
	// set stuff up here
}

void tearDown(void)
{
	// clean stuff up here
}

static int compare_int(const void *a, const void *b)
{
	return *(int *)a - *(int *)b;
}

void test_zk_slist_sort_when_list_is_null(void)
{
	zk_slist **list_p = NULL;

	TEST_ASSERT_EQUAL(ZK_INVALID_ARGUMENT, zk_sort(list_p, compare_int));
}

void test_zk_slist_sort_when_function_pointer_is_null(void)
{
	zk_slist *list = NULL;

	// fill list with some sorted data
	int data[] = { 1, 2, 3, 4, 5 };
	for (int i = 0; i < 5; i++)
		zk_push_back(&list, &data[i]);

	TEST_ASSERT_EQUAL(ZK_INVALID_ARGUMENT, zk_sort(&list, NULL));

	zk_free(&list, NULL);
}

void test_zk_slist_sort_when_list_is_empty(void)
{
	zk_slist *list = NULL;

	TEST_ASSERT_EQUAL(ZK_INVALID_ARGUMENT, zk_sort(&list, compare_int));
	TEST_ASSERT_NULL(list);

	zk_free(&list, NULL);
}

void test_zk_list_sort_when_list_has_n_elements()
{
	zk_slist *list = NULL, *current = NULL;
	void *node_data = NULL;

	// Dynamically create a list that varies in size from 1 t0 101 and sort them
	for (size_t i = 1; i <= 101; i += 10) {
		int *data = malloc(sizeof(int) * i);
		for (size_t j = 0; j < i; j++)
			data[j] = i - j;

		for (size_t j = 0; j < i; j++)
			zk_push_back(&list, &data[j]);

		TEST_ASSERT_EQUAL(ZK_OK, zk_sort(&list, compare_int));

		current = list;
		for (size_t j = 0; j < i; j++) {
			zk_get_data(current, &node_data);
			TEST_ASSERT_EQUAL(j + 1, *(int *)node_data);
			zk_next(current, &current);
		}

		TEST_ASSERT_EQUAL(i, zk_size(list));

		zk_free(&list, NULL);
		free(data);
	}
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_zk_slist_sort_when_list_is_null);
	RUN_TEST(test_zk_slist_sort_when_function_pointer_is_null);
	RUN_TEST(test_zk_slist_sort_when_list_is_empty);
	RUN_TEST(test_zk_list_sort_when_list_has_n_elements);
	return UNITY_END();
}
