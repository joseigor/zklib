#include <stdlib.h>

#include "unity.h"
#include "zk/zklib.h"

void setUp(void) {}

void tearDown(void) {}

void test_zk_slist_new_node_when_list_is_null_and_data_is_null(void)
{
	zk_slist *list = NULL;
	list = zk_slist_new_node(NULL);
	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_NULL(list->data);
	TEST_ASSERT_NULL(list->next);
	zk_slist_free(&list, NULL);
}

void test_zk_slist_new_node_when_data_is_not_null(void)
{
	zk_slist *list = NULL, *next = NULL;
	void *data = NULL;

	list = zk_slist_new_node("data");
	TEST_ASSERT_NOT_NULL(list);

	TEST_ASSERT_NOT_NULL(list->data);
	TEST_ASSERT_EQUAL("data", (char *)list->data);

	TEST_ASSERT_NULL(list->next);

	zk_free(&list, NULL);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_zk_slist_new_node_when_list_is_null_and_data_is_null);
	RUN_TEST(test_zk_slist_new_node_when_data_is_not_null);
	return UNITY_END();
}
