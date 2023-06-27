#include <stdlib.h>

#include "unity.h"
#include "zk/zklib.h"

void setUp(void) {}

void tearDown(void) {}

void test_zk_slist_begin_returns_null_when_slist_is_empty(void)
{
	zk_slist *list = NULL;

	TEST_ASSERT_NULL(zk_slist_begin(list));
}

void test_zk_slist_begin_returns_1st_element_when_slist_has_one_element(void)
{
	zk_slist *list = NULL, *begin = NULL;
	int node_1_data = 1;

	list = zk_slist_push_back(list, &node_1_data);

	begin = zk_slist_begin(list);

	TEST_ASSERT_EQUAL_PTR(list, begin);

	TEST_ASSERT_NOT_NULL(list->data);
	TEST_ASSERT_EQUAL(node_1_data, *((int *)list->data));

	zk_slist_free(&list, NULL);
}

void test_zk_slist_begin_returns_1st_element_when_slist_has_more_than_one_element(void)
{
	zk_slist *list = NULL, *begin = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;

	list = zk_slist_push_back(list, &node_1_data);
	list = zk_slist_push_back(list, &node_2_data);
	list = zk_slist_push_back(list, &node_3_data);

	begin = zk_slist_begin(list);

	TEST_ASSERT_EQUAL_PTR(list, begin);

	TEST_ASSERT_EQUAL(list->data, begin->data);

	zk_slist_free(&list, NULL);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_zk_slist_begin_returns_null_when_slist_is_empty);
	RUN_TEST(test_zk_slist_begin_returns_1st_element_when_slist_has_one_element);
	RUN_TEST(test_zk_slist_begin_returns_1st_element_when_slist_has_more_than_one_element);
	return UNITY_END();
}
