#include <stdlib.h>
#include <string.h>

#include "unity.h"
#include "zk/zklib.h"

void setUp(void) {}

void tearDown(void) {}

void test_zk_slist_free_a_null_list_should_just_return(void)
{
	zk_slist *list = NULL;
	zk_slist_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_slist_free_list_with_no_dynamic_allocated_node_data(void)
{
	zk_slist *list = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;
	int node_4_data = 4;

	list = zk_slist_push_back(list, &node_1_data);
	list = zk_slist_push_back(list, &node_2_data);
	list = zk_slist_push_back(list, &node_3_data);
	list = zk_slist_push_back(list, &node_4_data);

	zk_slist_free(&list, NULL);

	TEST_ASSERT_NULL(list);
}

void test_zk_slist_free_for_list_of_strings(void)
{
	int number_of_nodes = 100;
	zk_slist *list = NULL;

	for (int i = 0; i < number_of_nodes; i++) {
		list = zk_slist_push_back(list, strdup("a"));
		TEST_ASSERT_NOT_NULL(list);
	}

	zk_slist_free(&list, free);
	TEST_ASSERT_NULL(list);
}

void test_zk_slist_free_for_a_null_list_should_just_return(void)
{
	zk_slist *list = NULL;
	zk_slist_free(&list, free);
	TEST_ASSERT_NULL(list);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_zk_slist_free_a_null_list_should_just_return);
	RUN_TEST(test_zk_slist_free_list_with_no_dynamic_allocated_node_data);
	RUN_TEST(test_zk_slist_free_for_list_of_strings);
	RUN_TEST(test_zk_slist_free_for_a_null_list_should_just_return);
	return UNITY_END();
}
