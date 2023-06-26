#include <stdlib.h>

#include "unity.h"
#include "zk/zklib.h"

void setUp(void) {}

void tearDown(void) {}

static int compare_int(const void *a, const void *b)
{
	return *(int *)a - *(int *)b;
}

void test_zk_slist_sort_when_list_is_null(void)
{
	zk_slist *list = NULL;

	list = zk_slist_sort(list, compare_int);

	TEST_ASSERT_NULL(list);
}

void test_zk_slist_sort_when_function_pointer_is_null(void)
{
	zk_slist *list = NULL;

	// fill list with some sorted data
	int data[] = { 5, 4, 3, 2, 1 };
	for (int i = 0; i < 5; i++)
		list = zk_slist_push_back(list, &data[i]);

	list = zk_slist_sort(list, NULL);

	// list should be unchanged since the function pointer is NULL
	zk_slist *current = list;
	for (int i = 0; i < 5; i++) {
		TEST_ASSERT_EQUAL(data[i], *(int *)current->data);
		current = current->next;
	}

	zk_slist_free(&list, NULL);
}

void test_zk_slist_sort_when_list_is_empty(void)
{
	zk_slist *list = NULL;

	list = zk_slist_sort(list, compare_int);

	TEST_ASSERT_NULL(list);

	zk_slist_free(&list, NULL);
}

void test_zk_list_sort_when_list_has_n_elements()
{
	zk_slist *list = NULL, *current = NULL;

	// Dynamically create a list that varies in size from 1 to 101 elements and sort it
	for (size_t i = 1; i <= 101; i += 10) {
		int *data = malloc(sizeof(int) * i);
		for (size_t j = 0; j < i; j++) {
			data[j] = i - j;
			list = zk_slist_push_back(list, &data[j]);
		}

		list = zk_slist_sort(list, compare_int);

		current = list;
		for (size_t j = 0; j < i; j++) {
			TEST_ASSERT_EQUAL(j + 1, *(int *)current->data);
			current = current->next;
		}

		TEST_ASSERT_EQUAL(i, zk_slist_size(list));

		zk_slist_free(&list, NULL);
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
