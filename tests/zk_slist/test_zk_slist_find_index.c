#include <stdlib.h>

#include "unity.h"
#include "zk/zklib.h"

void setUp(void) {}

void tearDown(void) {}

void test_zk_slist_find_index_when_list_is_null(void)
{
	zk_slist *list = NULL;
	size_t index = 0;

	TEST_ASSERT_NULL(zk_slist_find_index(list, index));
}

void test_zk_slist_find_index_when_index_is_out_of_bounds(void)
{
	zk_slist *list = NULL;
	size_t index = 6;

	// fill list with some sorted data
	int data_list[] = { 1, 2, 3, 4, 5 };
	for (int i = 0; i < 5; i++)
		zk_slist_push_back(&list, &data_list[i]);

	TEST_ASSERT_NULL(zk_slist_find_index(list, index));

	zk_slist_free(&list, NULL);
}

void test_zk_slist_find_index_when_index_is_valid(void)
{
	zk_slist *list = NULL;
	size_t index = 3;

	// fill list with some sorted data
	int data_list[] = { 1, 2, 3, 4, 5 };
	for (int i = 0; i < 5; i++)
		zk_slist_push_back(&list, &data_list[i]);

	zk_slist *node = zk_slist_find_index(list, index);
	TEST_ASSERT_EQUAL_INT(4, *(int *)node->data);

	zk_slist_free(&list, NULL);
}


int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_zk_slist_find_index_when_list_is_null);
	RUN_TEST(test_zk_slist_find_index_when_index_is_out_of_bounds);
	RUN_TEST(test_zk_slist_find_index_when_index_is_valid);
	return UNITY_END();
}
