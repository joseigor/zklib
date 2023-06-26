#include "unity.h"
#include "zk/zklib.h"

void setUp(void) {}

void tearDown(void) {}

static int compare_int(const void *a, const void *b)
{
	return *(int *)a - *(int *)b;
}

void test_zk_slist_merge_when_first_list_is_null(void)
{
	zk_slist *first = NULL;
	zk_slist *second = NULL;
	zk_slist *merged = NULL;

	// fill second list with some sorted data
	int data[] = { 1, 2, 3, 4, 5 };
	for (int i = 0; i < 5; i++)
		second = zk_slist_push_back(second, &data[i]);

	merged = zk_slist_merge(first, second, compare_int);

	// check all data is in the merged list
	zk_slist *current = merged;
	for (int i = 0; i < 5; i++) {
		TEST_ASSERT_EQUAL(data[i], *(int *)current->data);
		current = current->next;
	}

	zk_slist_free(&merged, NULL); // second list is empty no need to free
}

void test_zk_slist_merge_when_second_list_is_null(void)
{
	zk_slist *first = NULL;
	zk_slist *second = NULL;
	zk_slist *merged = NULL;

	// fill first list with some sorted data
	int data[] = { 1, 2, 3, 4, 5 };
	for (int i = 0; i < 5; i++)
		first = zk_slist_push_back(first, &data[i]);

	merged = zk_slist_merge(first, second, compare_int);

	// check all data is in the merged list
	zk_slist *current = merged;
	for (int i = 0; i < 5; i++) {
		TEST_ASSERT_EQUAL(data[i], *(int *)current->data);
		current = current->next;
	}

	zk_slist_free(&merged, NULL);
}

void test_zk_slist_merge_when_function_pointer_is_null(void)
{
	zk_slist *first = NULL;
	zk_slist *second = NULL;

	// fill first and second list with some sorted data
	int data[] = { 1, 2, 3, 4, 5 };
	for (int i = 0; i < 5; i++) {
		first = zk_slist_push_back(first, &data[i]);
		second = zk_slist_push_back(second, &data[i]);
	}

	TEST_ASSERT_NULL(zk_slist_merge(first, second, NULL));

	zk_slist_free(&second, NULL);
	zk_slist_free(&first, NULL);
}

void test_zk_slist_merge_when_both_lists_are_empty(void)
{
	zk_slist *first = NULL;
	zk_slist *second = NULL;

	TEST_ASSERT_NULL(zk_slist_merge(first, second, compare_int));
}

void test_zk_slist_merge_when_first_list_is_empty(void)
{
	zk_slist *first = NULL;
	zk_slist *second = NULL;
	zk_slist *merged = NULL;

	// fill second list with some sorted data
	int data[] = { 1, 2, 3, 4, 5 };
	for (int i = 0; i < 5; i++)
		second = zk_slist_push_back(second, &data[i]);

	merged = zk_slist_merge(first, second, compare_int);

	// check all data is in the merged list
	zk_slist *current = merged;
	for (int i = 0; i < 5; i++) {
		TEST_ASSERT_EQUAL(data[i], *(int *)current->data);
		current = current->next;
	}

	zk_slist_free(&merged, NULL);
}

void test_zk_slist_merge_when_second_list_is_empty(void)
{
	zk_slist *first = NULL;
	zk_slist *second = NULL;
	zk_slist *merged = NULL;

	// fill first list with some sorted data
	int data[] = { 1, 2, 3, 4, 5 };
	for (int i = 0; i < 5; i++)
		first = zk_slist_push_back(first, &data[i]);

	merged = zk_slist_merge(first, second, compare_int);

	// check all data is in the merged list
	zk_slist *current = merged;
	for (int i = 0; i < 5; i++) {
		TEST_ASSERT_EQUAL(data[i], *(int *)current->data);
		current = current->next;
	}

	zk_slist_free(&merged, NULL);
}

void test_zk_slist_merge_when_both_lists_are_not_empty(void)
{
	zk_slist *first = NULL;
	zk_slist *second = NULL;
	zk_slist *merged = NULL;

	// fill first list with some sorted data
	int data1[] = { 1, 3, 5, 7, 9 };
	for (int i = 0; i < 5; i++)
		first = zk_slist_push_back(first, &data1[i]);

	// fill second list with some sorted data
	int data2[] = { 2, 4, 6, 8, 10 };
	for (int i = 0; i < 5; i++)
		second = zk_slist_push_back(second, &data2[i]);

	merged = zk_slist_merge(first, second, compare_int);

	// check all data is in the merged list
	zk_slist *current = merged;
	for (int i = 0; i < 10; i++) {
		TEST_ASSERT_EQUAL(i + 1, *(int *)current->data);
		current = current->next;
	}

	zk_slist_free(&merged, NULL);
}

void test_zk_slist_merge_is_stable(void)
{
	zk_slist *first = NULL;
	zk_slist *second = NULL;
	zk_slist *merged = NULL;

	// fill first list with some sorted data
	int data1[] = { 1, 1, 1, 1, 1 };
	for (int i = 0; i < 5; i++)
		first = zk_slist_push_back(first, &data1[i]);

	// fill second list with some sorted data
	int data2[] = { 1, 1, 1, 1, 1 };
	for (int i = 0; i < 5; i++)
		second = zk_slist_push_back(second, &data2[i]);

	merged = zk_slist_merge(first, second, compare_int);

	// check all data is in the merged list and is stable
	zk_slist *current = merged;
	for (int i = 0; i < 10; i++) {
		TEST_ASSERT_EQUAL(1, *(int *)current->data);
		// check that is stable by comparing pointers
		if (i < 5)
			TEST_ASSERT_EQUAL_PTR(&data1[i], current->data);
		else
			TEST_ASSERT_EQUAL_PTR(&data2[i - 5], current->data);
		current = current->next;
	}

	zk_slist_free(&merged, NULL);
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
