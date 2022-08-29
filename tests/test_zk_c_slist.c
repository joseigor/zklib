#include <stdlib.h>
#include <string.h>

#include "common/test_common.h"
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

/*--------------- Test Constructor ---------------*/
// test zk_c_slist_new_node
void test_zk_c_slist_new_node_when_data_is_null(void)
{
	zk_c_slist *list = zk_c_slist_new_node(NULL);
	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_NULL(list->data);
	TEST_ASSERT_NULL(list->next);

	free(list);
}
void test_zk_c_slist_new_node_when_data_is_not_null(void)
{
	zk_c_slist *list = zk_c_slist_new_node("data");
	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_NOT_NULL(list->data);
	TEST_ASSERT_EQUAL("data", (char *)(list->data));
	TEST_ASSERT_NULL(list->next);

	free(list);
}

/*--------------- Test Destructor ---------------*/
// test zk_free
void test_zk_free_a_null_list_should_just_return(void)
{
	zk_c_slist *list = NULL;
	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_free_list_with_no_dynamic_allocated_node_data(void)
{
	zk_c_slist *list = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;
	int node_4_data = 4;

	list = zk_push_back(list, &node_1_data);
	list = zk_push_back(list, &node_2_data);
	list = zk_push_back(list, &node_3_data);
	list = zk_push_back(list, &node_4_data);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_free_for_list_of_strings(void)
{
	int number_of_nodes = 100;
	zk_c_slist *list = NULL;

	for (int i = 0; i < number_of_nodes; i++) {
		list = zk_push_back(list, strdup("a"));
		TEST_ASSERT_NOT_NULL(list);
	}

	zk_free(&list, free);
	TEST_ASSERT_NULL(list);
}

void test_zk_free_for_a_null_list_should_just_return(void)
{
	zk_c_slist *list = NULL;
	zk_free(&list, free);
	TEST_ASSERT_NULL(list);
}

/*--------------- Test Iterators ---------------*/

// test for zk_begin
void test_zk_begin_returns_null_when_list_is_empty(void)
{
	zk_c_slist *list = NULL;

	TEST_ASSERT_NULL(zk_begin(list));
}

void test_zk_begin_when_list_has_one_element_begin_equal_end(void)
{
	zk_c_slist *list = NULL, *begin = NULL;
	int node_1_data = 1;

	list = zk_push_back(list, &node_1_data);

	begin = zk_begin(list);

	TEST_ASSERT_EQUAL_PTR(list, begin);
	TEST_ASSERT_EQUAL_PTR(list->data, begin->data);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_begin_returns_1st_element_when_list_has_more_than_one_element(void)
{
	zk_c_slist *list = NULL, *begin = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;

	list = zk_push_back(list, &node_1_data);
	list = zk_push_back(list, &node_2_data);
	list = zk_push_back(list, &node_3_data);

	begin = zk_begin(list);

	TEST_ASSERT_EQUAL_PTR(list->next, begin);
	TEST_ASSERT_EQUAL_PTR(&node_1_data, begin->data);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

// test for zk_end
void test_zk_end_returns_null_when_list_is_empty(void)
{
	zk_c_slist *list = NULL;

	TEST_ASSERT_NULL(zk_end(list));
}

void test_zk_end_when_list_has_one_element_end_equal_begin(void)
{
	zk_c_slist *list = NULL, *begin = NULL, *end = NULL;
	int node_1_data = 1;

	list = zk_push_back(list, &node_1_data);

	begin = zk_begin(list);
	end = zk_end(list);

	TEST_ASSERT_EQUAL_PTR(begin, end);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_end_returns_last_when_list_has_more_than_one_element(void)
{
	zk_c_slist *list = NULL, *end = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;

	list = zk_push_back(list, &node_1_data);
	list = zk_push_back(list, &node_2_data);
	list = zk_push_back(list, &node_3_data);

	end = zk_end(list);

	TEST_ASSERT_EQUAL_PTR(list, end);
	TEST_ASSERT_EQUAL_PTR(&node_3_data, end->data);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

// test zk_for_each
void test_zk_for_each_when_func_is_null(void)
{
	struct dummy_node_data *node_1_data = malloc(sizeof(struct dummy_node_data));
	node_1_data->value = 1;
	node_1_data->string = strdup("node_1");
	zk_c_slist *list = NULL;

	list = zk_push_back(list, node_1_data);
	TEST_ASSERT_NOT_NULL(list);

	// Func is null, it will just return
	zk_for_each(list, NULL, NULL);

	zk_free(&list, dummy_node_data_free);
	TEST_ASSERT_NULL(list);
}

void test_zk_for_each_when_list_is_null(void)
{
	zk_c_slist *list = NULL;

	// list is null, it will just return
	zk_for_each(list, dummy_node_data_free_foreach, NULL);

	TEST_ASSERT_NULL(list);
}

void test_zk_for_each_when_func_is_not_null(void)
{
	struct dummy_node_data *node_1_data = malloc(sizeof(struct dummy_node_data));
	node_1_data->value = 1;
	node_1_data->string = strdup("node_1");

	struct dummy_node_data *node_2_data = malloc(sizeof(struct dummy_node_data));
	node_2_data->value = 2;
	node_2_data->string = strdup("node_2");

	struct dummy_node_data *node_3_data = malloc(sizeof(struct dummy_node_data));
	node_3_data->value = 3;
	node_3_data->string = strdup("node_3");

	zk_c_slist *list = NULL;
	list = zk_push_back(list, node_1_data);
	list = zk_push_back(list, node_2_data);
	list = zk_push_back(list, node_3_data);

	// Use foreach to clean each node data
	zk_for_each(list, dummy_node_data_free_foreach, NULL);

	// As nodes data were freed, we just need to call zk_free.
	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

/*--------------- Test Modifiers ---------------*/
// Test zk_push_back
void test_zk_push_back_to_empty_list(void)
{
	zk_c_slist *list = NULL;
	int data = 5;

	list = zk_push_back(list, &data);

	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_EQUAL_PTR(list->data, &data);
	TEST_ASSERT_EQUAL(*((int *)list->data), data);
	TEST_ASSERT_EQUAL_PTR(list, list->next);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_push_back_2_items_to_list(void)
{
	zk_c_slist *list = NULL, *node1 = NULL;

	int node1_data = 0;
	int node2_data = 1;

	list = zk_push_back(list, &node1_data);
	node1 = list;

	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_EQUAL_PTR(list->data, &node1_data);
	TEST_ASSERT_EQUAL_PTR(*((int *)list->data), node1_data);
	TEST_ASSERT_EQUAL_PTR(list, list->next);

	list = zk_push_back(list, &node2_data);

	// checks that list points to the last element of the list
	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_EQUAL_PTR(list->data, &node2_data);
	TEST_ASSERT_EQUAL_PTR(*((int *)list->data), node2_data);
	TEST_ASSERT_NOT_NULL(list->next);
	TEST_ASSERT_EQUAL_PTR(node1, list->next);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_push_back_3_items_to_list(void)
{
	zk_c_slist *list = NULL, *node1 = NULL;

	int node1_data = 0;
	int node2_data = 1;
	int node3_data = 3;

	list = zk_push_back(list, &node1_data);
	node1 = list;

	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_EQUAL_PTR(list->data, &node1_data);
	TEST_ASSERT_EQUAL(*((int *)list->data), node1_data);
	TEST_ASSERT_EQUAL_PTR(list, list->next);

	list = zk_push_back(list, &node2_data);

	// checks that list points to the last element of the list
	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_EQUAL_PTR(list->data, &node2_data);
	TEST_ASSERT_EQUAL_PTR(*((int *)list->data), node2_data);
	TEST_ASSERT_NOT_NULL(list->next);
	TEST_ASSERT_EQUAL_PTR(node1, list->next);

	list = zk_push_back(list, &node3_data);

	// checks that list points to the last element of the list
	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_EQUAL_PTR(list->data, &node3_data);
	TEST_ASSERT_EQUAL(*((int *)list->data), node3_data);
	TEST_ASSERT_EQUAL_PTR(node1, list->next);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_push_back_n_items_to_list(void)
{
	int number_of_nodes = 100;
	int nodes_data[number_of_nodes];
	zk_c_slist *list = NULL;

	for (int i = 0; i < number_of_nodes; i++) {
		nodes_data[i] = i;
		list = zk_push_back(list, &nodes_data[i]);
		TEST_ASSERT_NOT_NULL(list);
	}

	int node_idx = 0;
	zk_c_slist *current = list->next; // begin of the list
	zk_c_slist *last = list;
	while (current != last) {
		TEST_ASSERT_NOT_NULL(current);
		TEST_ASSERT_EQUAL_PTR(current->data, &nodes_data[node_idx]);
		TEST_ASSERT_EQUAL(*((int *)current->data), nodes_data[node_idx]);
		node_idx++;
		current = current->next;
	}

	// checks end node
	TEST_ASSERT_NOT_NULL(current);
	TEST_ASSERT_EQUAL_PTR(current->data, &nodes_data[node_idx]);
	TEST_ASSERT_EQUAL(*((int *)current->data), nodes_data[node_idx]);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_push_back_null_data_to_list(void)
{
	int number_of_nodes = 100;
	zk_c_slist *list = NULL;

	for (int i = 0; i < number_of_nodes; i++) {
		list = zk_push_back(list, NULL);
		TEST_ASSERT_NOT_NULL(list);
	}

	zk_c_slist *current = list->next; // begin of the list
	zk_c_slist *last = list;
	while (current != last) {
		TEST_ASSERT_NOT_NULL(current);
		TEST_ASSERT_EQUAL_PTR(current->data, NULL);

		current = current->next;
	}
	// checks end node
	TEST_ASSERT_NOT_NULL(current);
	TEST_ASSERT_EQUAL_PTR(current->data, NULL);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

int main(void)
{
	UNITY_BEGIN();

	/*--------------- Test Constructor ---------------*/
	{ // test zk_c_slist_new_node
		RUN_TEST(test_zk_c_slist_new_node_when_data_is_null);
		RUN_TEST(test_zk_c_slist_new_node_when_data_is_not_null);
	}

	/*--------------- Test Destructor ---------------*/

	{ // test zk_free
		RUN_TEST(test_zk_free_a_null_list_should_just_return);
		RUN_TEST(test_zk_free_list_with_no_dynamic_allocated_node_data);
		RUN_TEST(test_zk_free_for_list_of_strings);
		RUN_TEST(test_zk_free_for_a_null_list_should_just_return);
	}

	/*--------------- Test Iterators ---------------*/

	{ // test zk_begin
		RUN_TEST(test_zk_begin_returns_null_when_list_is_empty);
		RUN_TEST(test_zk_begin_when_list_has_one_element_begin_equal_end);
		RUN_TEST(test_zk_begin_returns_1st_element_when_list_has_more_than_one_element);
	}

	{ // test zk_end
		RUN_TEST(test_zk_end_returns_null_when_list_is_empty);
		RUN_TEST(test_zk_end_when_list_has_one_element_end_equal_begin);
		RUN_TEST(test_zk_end_returns_last_when_list_has_more_than_one_element);
	}

	{ // test zk_for_each
		RUN_TEST(test_zk_for_each_when_func_is_null);
		RUN_TEST(test_zk_for_each_when_list_is_null);
		RUN_TEST(test_zk_for_each_when_func_is_not_null);
	}

	/*--------------- Test Modifiers ---------------*/

	{ // tests for zk_push_back function
		RUN_TEST(test_zk_push_back_to_empty_list);
		RUN_TEST(test_zk_push_back_2_items_to_list);
		RUN_TEST(test_zk_push_back_3_items_to_list);
		RUN_TEST(test_zk_push_back_n_items_to_list);
		RUN_TEST(test_zk_push_back_null_data_to_list);
	}

	return UNITY_END();
}
