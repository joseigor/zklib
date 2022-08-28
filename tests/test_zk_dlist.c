#include <stdlib.h>
#include <string.h>

#include "unity.h"
#include "zk/zklib.h"
#include "zk_common/zk_common.h"

struct dummy_node_data {
	int value;
	char *string;
};

static void dummy_node_data_free_foreach(void *data, void *user_data)
{
	ZK_UNUSED(user_data);
	struct dummy_node_data *node_data = data;
	free(node_data->string);
	free(node_data);
}

static void dummy_node_data_free(void *data)
{
	struct dummy_node_data *node_data = data;
	free(node_data->string);
	free(node_data);
}

void setUp(void)
{
	// set stuff up here
}

void tearDown(void)
{
	// clean stuff up here
}

/*--------------- Test Iterators ---------------*/

// test for zk_begin
void test_zk_begin_returns_null_when_list_is_empty(void)
{
	zk_dlist *list = NULL;

	TEST_ASSERT_NULL(zk_begin(list));
}

void test_zk_begin_returns_1st_element_when_list_has_one_element(void)
{
	zk_dlist *list = NULL, *begin = NULL;
	int node_1_data = 1;

	list = zk_push_back(list, &node_1_data);

	begin = zk_begin(list);

	TEST_ASSERT_EQUAL_PTR(list, begin);
	TEST_ASSERT_EQUAL(list->data, begin->data);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_begin_returns_1st_element_when_list_has_more_than_one_element(void)
{
	zk_dlist *list = NULL, *begin = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;

	list = zk_push_back(list, &node_1_data);
	list = zk_push_back(list, &node_2_data);
	list = zk_push_back(list, &node_3_data);

	begin = zk_begin(list);

	TEST_ASSERT_EQUAL_PTR(list, begin);
	TEST_ASSERT_EQUAL(list->data, begin->data);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

// test for zk_end
void test_zk_end_returns_null_when_list_is_empty(void)
{
	zk_dlist *list = NULL;

	TEST_ASSERT_NULL(zk_end(list));
}

void test_zk_end_returns_null_when_list_has_one_element(void)
{
	zk_dlist *list = NULL;
	int node_1_data = 1;

	list = zk_push_back(list, &node_1_data);

	TEST_ASSERT_NULL(zk_end(list));

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_end_returns_null_when_list_has_more_than_one_element(void)
{
	zk_dlist *list = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;

	list = zk_push_back(list, &node_1_data);
	list = zk_push_back(list, &node_2_data);
	list = zk_push_back(list, &node_3_data);

	TEST_ASSERT_NULL(zk_end(list));

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

// test zk_for_each

void test_zk_for_each_when_func_is_null(void)
{
	struct dummy_node_data *node_1_data = malloc(sizeof(struct dummy_node_data));
	node_1_data->value = 1;
	node_1_data->string = strdup("node_1");
	zk_dlist *list = NULL;

	list = zk_push_back(list, node_1_data);
	TEST_ASSERT_NOT_NULL(list);

	// Func is null, it will just return
	zk_for_each(list, NULL, NULL);

	zk_free(&list, dummy_node_data_free);
	TEST_ASSERT_NULL(list);
}

void test_zk_for_each_when_list_is_null(void)
{
	zk_dlist *list = NULL;

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

	zk_dlist *list = NULL;
	list = zk_push_back(list, node_1_data);
	list = zk_push_back(list, node_2_data);
	list = zk_push_back(list, node_3_data);

	// Use foreach to clean each node data
	zk_for_each(list, dummy_node_data_free_foreach, NULL);

	// As nodes data were freed, we just need to call zk_free.
	zk_free(&list, NULL);
}

/*--------------- Test Modifiers ---------------*/

// Test zk_push_back
void test_zk_push_back_to_empty_list(void)
{
	zk_dlist *list = NULL;
	int data = 5;

	list = zk_push_back(list, &data);

	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_EQUAL_PTR(list->data, &data);
	TEST_ASSERT_EQUAL(*((int *)list->data), data);
	TEST_ASSERT_NULL(list->prev);
	TEST_ASSERT_NULL(list->next);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_push_back_2_items_to_list(void)
{
	zk_dlist *list = NULL;

	int node1_data = 0;
	int node2_data = 1;

	list = zk_push_back(list, &node1_data);

	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_EQUAL_PTR(list->data, &node1_data);
	TEST_ASSERT_EQUAL_PTR(*((int *)list->data), node1_data);
	TEST_ASSERT_NULL(list->prev);
	TEST_ASSERT_NULL(list->next);

	list = zk_push_back(list, &node2_data);

	// first we check that list still points to the head of the list
	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_EQUAL_PTR(list->data, &node1_data);
	TEST_ASSERT_EQUAL_PTR(*((int *)list->data), node1_data);
	TEST_ASSERT_NULL(list->prev);
	TEST_ASSERT_NOT_NULL(list->next);

	// now we check the content of the second node of the list
	zk_dlist *node_2 = list->next;
	TEST_ASSERT_NOT_NULL(node_2);
	TEST_ASSERT_EQUAL_PTR(node_2->data, &node2_data);
	TEST_ASSERT_EQUAL_PTR(*((int *)node_2->data), node2_data);
	TEST_ASSERT_EQUAL_PTR(list, node_2->prev);
	TEST_ASSERT_NULL(node_2->next);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_push_back_3_items_to_list(void)
{
	zk_dlist *list = NULL;

	int node1_data = 0;
	int node2_data = 1;
	int node3_data = 3;

	zk_dlist *node_2 = NULL;
	zk_dlist *node_3 = NULL;

	list = zk_push_back(list, &node1_data);

	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_EQUAL_PTR(list->data, &node1_data);
	TEST_ASSERT_EQUAL(*((int *)list->data), node1_data);
	TEST_ASSERT_NULL(list->prev);
	TEST_ASSERT_NULL(list->next);

	list = zk_push_back(list, &node2_data);

	// first we check that list still points to the head of the list
	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_EQUAL_PTR(list->data, &node1_data);
	TEST_ASSERT_EQUAL(*((int *)list->data), node1_data);
	TEST_ASSERT_NULL(list->prev);
	TEST_ASSERT_NOT_NULL(list->next);

	// now we check the content of the second node of the list
	node_2 = list->next;
	TEST_ASSERT_NOT_NULL(node_2);
	TEST_ASSERT_EQUAL_PTR(node_2->data, &node2_data);
	TEST_ASSERT_EQUAL(*((int *)node_2->data), node2_data);
	TEST_ASSERT_EQUAL_PTR(list, node_2->prev);
	TEST_ASSERT_NULL(node_2->next);

	list = zk_push_back(list, &node3_data);

	// first we check that list still points to the head of the list
	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_EQUAL_PTR(list->data, &node1_data);
	TEST_ASSERT_EQUAL(*((int *)list->data), node1_data);
	TEST_ASSERT_NULL(list->prev);
	TEST_ASSERT_NOT_NULL(list->next);

	// now we check the content of the second node of the list
	node_2 = list->next;
	TEST_ASSERT_NOT_NULL(node_2);
	TEST_ASSERT_EQUAL_PTR(node_2->data, &node2_data);
	TEST_ASSERT_EQUAL(*((int *)node_2->data), node2_data);
	TEST_ASSERT_EQUAL_PTR(list, node_2->prev);
	TEST_ASSERT_NOT_NULL(node_2->next);

	// now we check the content of the third node of the list

	node_3 = node_2->next;
	TEST_ASSERT_NOT_NULL(node_3);
	TEST_ASSERT_EQUAL_PTR(node_3->data, &node3_data);
	TEST_ASSERT_EQUAL(*((int *)node_3->data), node3_data);
	TEST_ASSERT_EQUAL_PTR(node_2, node_3->prev);
	TEST_ASSERT_NULL(node_3->next);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_push_back_n_items_to_list(void)
{
	int number_of_nodes = 100;
	int nodes_data[number_of_nodes];
	zk_dlist *list = NULL;

	for (int i = 0; i < number_of_nodes; i++) {
		nodes_data[i] = i;
		list = zk_push_back(list, &nodes_data[i]);
		TEST_ASSERT_NOT_NULL(list);
	}

	int node_idx = 0;
	for (zk_dlist *node_i = list;; node_i = node_i->next) {
		TEST_ASSERT_NOT_NULL(node_i);
		TEST_ASSERT_EQUAL_PTR(node_i->data, &nodes_data[node_idx]);
		if (node_idx == 0) {
			TEST_ASSERT_NULL(node_i->prev);
		} else {
			TEST_ASSERT_NOT_NULL(node_i->prev);
		}
		TEST_ASSERT_EQUAL(*((int *)node_i->data), nodes_data[node_idx]);
		if (node_i->next == NULL) {
			break;
		}
		node_idx++;
	}

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_push_back_null_data_to_list(void)
{
	int number_of_nodes = 100;
	zk_dlist *list = NULL;

	for (int i = 0; i < number_of_nodes; i++) {
		list = zk_push_back(list, NULL);
		TEST_ASSERT_NOT_NULL(list);
	}

	int i = 0;
	for (zk_dlist *node_i = list;; node_i = node_i->next) {
		TEST_ASSERT_NOT_NULL(node_i);
		TEST_ASSERT_EQUAL_PTR(node_i->data, NULL);
		if (i == 0) {
			TEST_ASSERT_NULL(node_i->prev);
		} else {
			TEST_ASSERT_NOT_NULL(node_i->prev);
		}
		if (node_i->next == NULL) {
			break;
		}
		i++;
	}

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

// test zk_free
void test_zk_free_a_null_list_should_just_return(void)
{
	zk_dlist *list = NULL;
	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_free_list_with_no_dynamic_allocated_node_data(void)
{
	zk_dlist *list = NULL;
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
	zk_dlist *list = NULL;

	for (int i = 0; i < number_of_nodes; i++) {
		list = zk_push_back(list, strdup("a"));
		TEST_ASSERT_NOT_NULL(list);
	}

	zk_free(&list, free);
	TEST_ASSERT_NULL(list);
}

void test_zk_free_for_a_null_list_should_just_return(void)
{
	zk_dlist *list = NULL;
	zk_free(&list, free);
	TEST_ASSERT_NULL(list);
}

int main(void)
{
	UNITY_BEGIN();

	/*--------------- Test Iterators ---------------*/

	{ // test zk_for_each
		RUN_TEST(test_zk_for_each_when_func_is_null);
		RUN_TEST(test_zk_for_each_when_list_is_null);
		RUN_TEST(test_zk_for_each_when_func_is_not_null);
	}

	{ // test zk_begin
		RUN_TEST(test_zk_begin_returns_null_when_list_is_empty);
		RUN_TEST(test_zk_begin_returns_1st_element_when_list_has_one_element);
		RUN_TEST(test_zk_begin_returns_1st_element_when_list_has_more_than_one_element);
	}

	{ // test zk_end
		RUN_TEST(test_zk_end_returns_null_when_list_is_empty);
		RUN_TEST(test_zk_end_returns_null_when_list_has_one_element);
		RUN_TEST(test_zk_end_returns_null_when_list_has_more_than_one_element);
	}

	/*--------------- Test Modifiers ---------------*/

	{ // tests for zk_push_back function
		RUN_TEST(test_zk_push_back_to_empty_list);
		RUN_TEST(test_zk_push_back_2_items_to_list);
		RUN_TEST(test_zk_push_back_3_items_to_list);
		RUN_TEST(test_zk_push_back_n_items_to_list);
		RUN_TEST(test_zk_push_back_null_data_to_list);
	}

	{ // test zk_free
		RUN_TEST(test_zk_free_a_null_list_should_just_return);
		RUN_TEST(test_zk_free_list_with_no_dynamic_allocated_node_data);
		RUN_TEST(test_zk_free_for_list_of_strings);
		RUN_TEST(test_zk_free_for_a_null_list_should_just_return);
	}

	return UNITY_END();
}
