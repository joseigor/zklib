#include <stdlib.h>

#include "unity.h"
#include "zk/zklib.h"

void setUp(void) {}

void tearDown(void) {}

void test_zk_slist_push_back_when_reference_is_null(void)
{
	zk_slist *list = NULL;
	list = zk_slist_push_back(list, NULL);
	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_NULL(list->data);
	TEST_ASSERT_NULL(list->next);
	zk_slist_free(&list, NULL);
}

void test_zk_slist_push_back_to_empty_list(void)
{
	zk_slist *list = NULL, *next = NULL;
	int data = 5;

	list = zk_slist_push_back(list, &data);

	TEST_ASSERT_NOT_NULL(list);

	TEST_ASSERT_EQUAL_PTR(list->data, &data);
	TEST_ASSERT_EQUAL(*((int *)list->data), data);

	next = list->next;
	TEST_ASSERT_NULL(next);

	zk_slist_free(&list, NULL);
}

void test_zk_slist_push_back_n_items_to_slist(void)
{
	int number_of_nodes = 100;
	int nodes_data[number_of_nodes];
	zk_slist *list = NULL;

	for (int i = 0; i < number_of_nodes; i++) {
		nodes_data[i] = i;
		list = zk_slist_push_back(list, &nodes_data[i]);
		TEST_ASSERT_NOT_NULL(list);
	}

	int node_idx = 0;
	zk_slist *node_i = list;
	while (node_i != NULL) {
		TEST_ASSERT_NOT_NULL(node_i);
		TEST_ASSERT_EQUAL_PTR(node_i->data, &nodes_data[node_idx]);
		TEST_ASSERT_EQUAL(*((int *)node_i->data), nodes_data[node_idx]);
		node_i = node_i->next;
		node_idx++;
	}

	zk_slist_free(&list, NULL);
}

void test_zk_slist_push_back_null_data_to_slist(void)
{
	int number_of_nodes = 100;
	zk_slist *list = NULL;

	for (int i = 0; i < number_of_nodes; i++) {
		list = zk_slist_push_back(list, NULL);
		TEST_ASSERT_NOT_NULL(list);
	}

	zk_slist *node_i = list;
	while (node_i != NULL) {
		TEST_ASSERT_NOT_NULL(node_i);
		TEST_ASSERT_EQUAL_PTR(node_i->data, NULL);
		node_i = node_i->next;
	}

	zk_slist_free(&list, NULL);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_zk_slist_push_back_when_reference_is_null);
	RUN_TEST(test_zk_slist_push_back_to_empty_list);
	RUN_TEST(test_zk_slist_push_back_n_items_to_slist);
	RUN_TEST(test_zk_slist_push_back_null_data_to_slist);
	return UNITY_END();
}
