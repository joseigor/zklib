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

void test_zk_slist_merge_when_first_list_is_null(void)
{
	zk_slist **first_p = NULL;
	zk_slist *second = NULL;

	// fill second list with some sorted data
	int data[] = { 1, 2, 3, 4, 5 };
	for (int i = 0; i < 5; i++)
		zk_push_back(&second, &data[i]);

	TEST_ASSERT_EQUAL(ZK_INVALID_ARGUMENT, zk_merge(first_p, &second, compare_int));

	zk_free(&second, NULL);
}

void test_zk_slist_merge_when_second_list_is_null(void)
{
	zk_slist *first = NULL;
	zk_slist **second_p = NULL;

	// fill first list with some sorted data
	int data[] = { 1, 2, 3, 4, 5 };
	for (int i = 0; i < 5; i++)
		zk_push_back(&first, &data[i]);

	TEST_ASSERT_EQUAL(ZK_INVALID_ARGUMENT, zk_merge(&first, second_p, compare_int));

	zk_free(&first, NULL);
}

void test_zk_slist_merge_when_function_pointer_is_null(void)
{
	zk_slist *first = NULL;
	zk_slist *second = NULL;

	// fill first and second list with some sorted data
	int data[] = { 1, 2, 3, 4, 5 };
	for (int i = 0; i < 5; i++) {
		zk_push_back(&first, &data[i]);
		zk_push_back(&second, &data[i]);
	}

	TEST_ASSERT_EQUAL(ZK_INVALID_ARGUMENT, zk_merge(&first, &second, NULL));

	zk_free(&second, NULL);
	zk_free(&first, NULL);
}

void test_zk_slist_merge_when_both_lists_are_empty(void)
{
	zk_slist *first = NULL;
	zk_slist *second = NULL;

	TEST_ASSERT_EQUAL(ZK_OK, zk_merge(&first, &second, compare_int));

	TEST_ASSERT_NULL(first);
	TEST_ASSERT_NULL(second);

	// no need to free as both lists are empty
}

void test_zk_slist_merge_when_first_list_is_empty(void)
{
	zk_slist *first = NULL;
	zk_slist *second = NULL;

	// fill second list with some sorted data
	int data[] = { 1, 2, 3, 4, 5 };
	for (int i = 0; i < 5; i++)
		zk_push_back(&second, &data[i]);

	TEST_ASSERT_EQUAL(ZK_OK, zk_merge(&first, &second, compare_int));

	// check second list is empty
	TEST_ASSERT_NULL(second);
	// check all data is in first list
	zk_slist *current = first;
	for (int i = 0; i < 5; i++) {
		void *node_nada = NULL;
		zk_get_data(current, &node_nada);
		TEST_ASSERT_EQUAL(data[i], *(int *)node_nada);
		zk_next(current, &current);
	}

	zk_free(&first, NULL);
	// second list is empty no need to free
}

void test_zk_slist_merge_when_second_list_is_empty(void)
{
	zk_slist *first = NULL;
	zk_slist *second = NULL;

	// fill first list with some sorted data
	int data[] = { 1, 2, 3, 4, 5 };
	for (int i = 0; i < 5; i++)
		zk_push_back(&first, &data[i]);

	TEST_ASSERT_EQUAL(ZK_OK, zk_merge(&first, &second, compare_int));

	// check second list is empty
	TEST_ASSERT_NULL(second);
	// check all data is in first
	zk_slist *current = first;
	for (int i = 0; i < 5; i++) {
		void *node_nada = NULL;
		zk_get_data(current, &node_nada);
		TEST_ASSERT_EQUAL(data[i], *(int *)node_nada);
		zk_next(current, &current);
	}

	zk_free(&first, NULL);
	// second list is empty no need to free
}

void test_zk_slist_merge_when_both_lists_are_not_empty(void)
{
	zk_slist *first = NULL;
	zk_slist *second = NULL;

	// fill first list with some sorted data
	int data1[] = { 1, 3, 5, 7, 9 };
	for (int i = 0; i < 5; i++)
		zk_push_back(&first, &data1[i]);

	// fill second list with some sorted data
	int data2[] = { 2, 4, 6, 8, 10 };
	for (int i = 0; i < 5; i++)
		zk_push_back(&second, &data2[i]);

	TEST_ASSERT_EQUAL(ZK_OK, zk_merge(&first, &second, compare_int));

	// check second list is empty
	TEST_ASSERT_NULL(second);
	// check all data is in first list and is sorted
	zk_slist *current = first;
	for (int i = 0; i < 10; i++) {
		void *node_nada = NULL;
		zk_get_data(current, &node_nada);
		TEST_ASSERT_EQUAL(i + 1, *(int *)node_nada);
		zk_next(current, &current);
	}

	zk_free(&first, NULL);
	// second list is empty no need to free
}

void test_zk_slist_merge_is_stable(void)
{
	zk_slist *first = NULL;
	zk_slist *second = NULL;

	// fill first list with some sorted data
	int data1[] = { 1, 1, 1, 1, 1 };
	for (int i = 0; i < 5; i++)
		zk_push_back(&first, &data1[i]);

	// fill second list with some sorted data
	int data2[] = { 1, 1, 1, 1, 1 };
	for (int i = 0; i < 5; i++)
		zk_push_back(&second, &data2[i]);

	TEST_ASSERT_EQUAL(ZK_OK, zk_merge(&first, &second, compare_int));

	// check second list is empty
	TEST_ASSERT_NULL(second);
	// check all data is in first list and is sorted
	zk_slist *current = first;
	for (int i = 0; i < 10; i++) {
		void *node_nada = NULL;
		zk_get_data(current, &node_nada);
		TEST_ASSERT_EQUAL(1, *(int *)node_nada);
		// check that is stable by comparing pointers
		if (i < 5)
			TEST_ASSERT_EQUAL_PTR(&data1[i], node_nada);
		else
			TEST_ASSERT_EQUAL_PTR(&data2[i - 5], node_nada);
		zk_next(current, &current);
	}

	zk_free(&first, NULL);
	// second list is empty no need to free
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_zk_slist_merge_when_first_list_is_null);
	RUN_TEST(test_zk_slist_merge_when_second_list_is_null);
	RUN_TEST(test_zk_slist_merge_when_function_pointer_is_null);
	RUN_TEST(test_zk_slist_merge_when_both_lists_are_empty);
	RUN_TEST(test_zk_slist_merge_when_first_list_is_empty);
	RUN_TEST(test_zk_slist_merge_when_second_list_is_empty);
	RUN_TEST(test_zk_slist_merge_when_both_lists_are_not_empty);
	RUN_TEST(test_zk_slist_merge_is_stable);
	return UNITY_END();
}
