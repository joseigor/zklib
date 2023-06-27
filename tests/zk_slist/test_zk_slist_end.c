#include <stdlib.h>

#include "unity.h"
#include "zk/zklib.h"

void setUp(void) {}

void tearDown(void) {}

void test_zk_slist_end_returns_null_when_slist_is_empty(void)
{
	zk_slist *list = NULL;

	TEST_ASSERT_NULL(zk_slist_end(list));
}

void test_zk_slist_end_returns_null_when_slist_has_one_element(void)
{
	zk_slist *list = NULL;
	int node_1_data = 1;

	list = zk_slist_push_back(list, &node_1_data);

	TEST_ASSERT_NULL(zk_slist_end(list));

	zk_slist_free(&list, NULL);
}

void test_zk_slist_end_returns_null_when_slist_has_more_than_one_element(void)
{
	zk_slist *list = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;

	list = zk_slist_push_back(list, &node_1_data);
	list = zk_slist_push_back(list, &node_2_data);
	list = zk_slist_push_back(list, &node_3_data);

	TEST_ASSERT_NULL(zk_slist_end(list));

	zk_slist_free(&list, NULL);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_zk_slist_end_returns_null_when_slist_is_empty);
	RUN_TEST(test_zk_slist_end_returns_null_when_slist_has_one_element);
	RUN_TEST(test_zk_slist_end_returns_null_when_slist_has_more_than_one_element);
	return UNITY_END();
}
