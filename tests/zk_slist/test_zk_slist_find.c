#include <stdlib.h>

#include "unity.h"
#include "zk/zklib.h"

void setUp(void) {}

void tearDown(void) {}

static int compare_int(const void *a, const void *b)
{
	return *(int *)a - *(int *)b;
}

void test_zk_slist_find_when_list_is_null(void)
{
	zk_slist *list = NULL;
	int data = 1;

	TEST_ASSERT_NULL(zk_slist_find(list, &data, compare_int));
}

void test_zk_slist_find_when_function_pointer_is_null(void)
{
	zk_slist *list = NULL;
	int data_list[] = { 1, 2, 3, 4, 5 };

	int data = 1;
	for (int i = 0; i < 5; i++)
		list = zk_slist_push_back(list, &data_list[i]);

	TEST_ASSERT_NULL(zk_slist_find(list, &data, NULL));

	zk_slist_free(&list, NULL);
}

void test_zk_slist_find_when_list_is_empty(void)
{
	zk_slist *list = NULL;
	int data = 1;

	TEST_ASSERT_NULL(zk_slist_find(list, &data, compare_int));
	TEST_ASSERT_NULL(list);

	zk_slist_free(&list, NULL);
}

void test_zk_slist_find_when_list_has_n_elements(void)
{
	zk_slist *list = NULL;
	int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	for (int i = 0; i < 10; i++) {
		list = zk_slist_push_back(list, &data[i]);
	}

	// find all elements
	for (int i = 0; i < 10; i++) {
		zk_slist *found = zk_slist_find(list, &i, compare_int);
		TEST_ASSERT_NOT_NULL(found);
		TEST_ASSERT_EQUAL_INT(data[i], *(int *)found->data);
	}

	// free list
	zk_slist_free(&list, NULL);
}

void test_zk_slist_find_when_list_has_n_elements_and_element_is_not_found(void)
{
	zk_slist *list = NULL;
	int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	for (int i = 0; i < 10; i++) {
		list = zk_slist_push_back(list, &data[i]);
	}

	int not_found = 10;
	TEST_ASSERT_NULL(zk_slist_find(list, &not_found, compare_int));

	not_found = -1;
	TEST_ASSERT_NULL(zk_slist_find(list, &not_found, compare_int));

	// free list
	zk_slist_free(&list, NULL);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_zk_slist_find_when_list_is_null);
	RUN_TEST(test_zk_slist_find_when_function_pointer_is_null);
	RUN_TEST(test_zk_slist_find_when_list_is_empty);
	RUN_TEST(test_zk_slist_find_when_list_has_n_elements);
	RUN_TEST(test_zk_slist_find_when_list_has_n_elements_and_element_is_not_found);
	return UNITY_END();
}
