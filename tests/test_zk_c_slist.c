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
// tests for zk_c_slist_new_node()
void test_zk_c_slist_new_node_when_reference_is_null(void)
{
	zk_c_slist **list = NULL;
	TEST_ASSERT_EQUAL(ZK_INVALID_ARGUMENT, zk_c_slist_new_node(list, NULL));
}

void test_zk_c_slist_new_node_when_data_is_null(void)
{
	zk_c_slist *list = NULL, *next = NULL;
	void *data = NULL;
	TEST_ASSERT_EQUAL(ZK_OK, zk_c_slist_new_node(&list, NULL));
	TEST_ASSERT_NOT_NULL(list);

	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(list, &data));
	TEST_ASSERT_NULL(data);

	TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
	TEST_ASSERT_NULL(next);

	// free is called instead of zk_free because a new node is not a circular singly linked list
	free(list);
}
void test_zk_c_slist_new_node_when_data_is_not_null(void)
{
	zk_c_slist *list = NULL, *next = NULL;
	void *data = NULL;

	TEST_ASSERT_EQUAL(ZK_OK, zk_c_slist_new_node(&list, "data"));
	TEST_ASSERT_NOT_NULL(list);

	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(list, &data));
	TEST_ASSERT_NOT_NULL(data);
	TEST_ASSERT_EQUAL("data", (char *)(data));

	TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
	TEST_ASSERT_NULL(next);

	// free is called instead of zk_free because a new node is not a circular singly linked list
	free(list);
}

/*--------------- Test Destructor ---------------*/
// tests for zk_free()
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

	zk_push_back(&list, &node_1_data);
	zk_push_back(&list, &node_2_data);
	zk_push_back(&list, &node_3_data);
	zk_push_back(&list, &node_4_data);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_free_for_list_of_strings(void)
{
	int number_of_nodes = 100;
	zk_c_slist *list = NULL;

	for (int i = 0; i < number_of_nodes; i++) {
		zk_push_back(&list, strdup("a"));
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

// tests for for zk_begin()
void test_zk_begin_returns_null_when_list_is_empty(void)
{
	zk_c_slist *list = NULL;

	TEST_ASSERT_NULL(zk_begin(list));
}

void test_zk_begin_when_list_has_one_element_begin_equal_end(void)
{
	zk_c_slist *list = NULL, *begin = NULL;
	void *list_data = NULL, *begin_data = NULL;
	int node_1_data = 1;

	zk_push_back(&list, &node_1_data);

	begin = zk_begin(list);
	TEST_ASSERT_EQUAL_PTR(list, begin);

	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(list, &list_data));
	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(begin, &begin_data));

	TEST_ASSERT_EQUAL_PTR(list_data, begin_data);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_begin_returns_1st_element_when_list_has_more_than_one_element(void)
{
	zk_c_slist *list = NULL, *begin = NULL, *next = NULL;
	void *node_data = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;

	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, &node_1_data));
	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, &node_2_data));
	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, &node_3_data));

	begin = zk_begin(list);

	TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
	TEST_ASSERT_EQUAL_PTR(next, begin);
	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(begin, &node_data));
	TEST_ASSERT_EQUAL_PTR(&node_1_data, node_data);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

// tests for for zk_end()
void test_zk_end_returns_null_when_list_is_empty(void)
{
	zk_c_slist *list = NULL;

	TEST_ASSERT_NULL(zk_end(list));
}

void test_zk_end_when_list_has_one_element_end_equal_begin(void)
{
	zk_c_slist *list = NULL, *begin = NULL, *end = NULL;
	int node_1_data = 1;

	zk_push_back(&list, &node_1_data);

	begin = zk_begin(list);
	end = zk_end(list);

	TEST_ASSERT_EQUAL_PTR(begin, end);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_end_returns_last_when_list_has_more_than_one_element(void)
{
	zk_c_slist *list = NULL, *end = NULL;
	void *node_data = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;

	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, &node_1_data));
	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, &node_2_data));
	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, &node_3_data));

	end = zk_end(list);
	TEST_ASSERT_EQUAL_PTR(list, end);

	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(end, &node_data));
	TEST_ASSERT_EQUAL_PTR(&node_3_data, node_data);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

// tests for zk_for_each()
void test_zk_for_each_when_func_is_null(void)
{
	struct dummy_node_data *node_1_data = malloc(sizeof(struct dummy_node_data));
	node_1_data->value = 1;
	node_1_data->string = strdup("node_1");
	zk_c_slist *list = NULL;

	zk_push_back(&list, node_1_data);
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
	zk_push_back(&list, node_1_data);
	zk_push_back(&list, node_2_data);
	zk_push_back(&list, node_3_data);

	// Use foreach to clean each node data
	zk_for_each(list, dummy_node_data_free_foreach, NULL);

	// As nodes data were freed, we just need to call zk_free.
	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

/*--------------- Test Modifiers ---------------*/
// tests for zk_pop_back()
void test_zk_pop_back_when_reference_is_null(void)
{
	zk_c_slist **list = NULL;
	TEST_ASSERT_EQUAL(ZK_INVALID_ARGUMENT, zk_pop_back(list, NULL));
	TEST_ASSERT_NULL(list);
}

void test_zk_pop_back_when_list_is_empty(void)
{
	zk_c_slist *list = NULL;
	TEST_ASSERT_EQUAL(ZK_OK, zk_pop_back(&list, NULL));
	TEST_ASSERT_NULL(list);
}

void test_zk_pop_back_when_list_has_1_element(void)
{
	zk_c_slist *list = NULL;
	char *data[] = { "0" };
	zk_push_back(&list, data[0]);

	zk_pop_back(&list, NULL);
	TEST_ASSERT_NULL(list);

	// no need to call zk_free() as list is freed by now
}

void test_zk_pop_back_when_list_has_2_elements(void)
{
	zk_c_slist *list = NULL, *next = NULL;
	void *node_data = NULL;
	char *data[] = { "0", "1" };

	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, data[0]));
	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, data[1]));

	TEST_ASSERT_EQUAL(ZK_OK, zk_pop_back(&list, NULL));

	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
	TEST_ASSERT_NOT_NULL(next);
	TEST_ASSERT_EQUAL_PTR(list, next);
	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(list, &node_data));
	TEST_ASSERT_EQUAL_STRING(data[0], (char *)node_data);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_pop_back_when_list_has_3_elements(void)
{
	zk_c_slist *list = NULL, *first_node = NULL, *last_node = NULL, *next = NULL;
	void *node_data = NULL;
	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, "0"));
	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, "1"));
	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, "2"));

	TEST_ASSERT_EQUAL(ZK_OK, zk_pop_back(&list, NULL));

	first_node = zk_begin(list);
	last_node = zk_end(list);

	// checks first->next points to last element
	TEST_ASSERT_EQUAL(ZK_OK, zk_next(first_node, &next));
	TEST_ASSERT_EQUAL_PTR(last_node, next);
	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(first_node, &node_data));
	TEST_ASSERT_EQUAL_STRING("0", (char *)node_data);

	// checks last element
	TEST_ASSERT_NOT_NULL(last_node);
	TEST_ASSERT_EQUAL(ZK_OK, zk_next(last_node, &next));
	TEST_ASSERT_NOT_NULL(next);
	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(last_node, &node_data));
	TEST_ASSERT_EQUAL_STRING("1", (char *)node_data);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_pop_back_when_list_has_n_elements(void)
{
	char *data[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
	zk_c_slist *list = NULL;
	for (size_t i = 0; i < 10; i++) {
		TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, data[i]));
	}

	// pop all elements
	size_t i = 9;
	while (list != NULL) {
		zk_c_slist *next = NULL;
		void *node_data = NULL;

		// checks first element of the list
		TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
		TEST_ASSERT_NOT_NULL(next);
		TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(next, &node_data));
		TEST_ASSERT_EQUAL_STRING(data[0], (char *)node_data);

		// checks last element of the list
		TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(list, &node_data));
		TEST_ASSERT_EQUAL_STRING(data[i--], (char *)node_data);

		TEST_ASSERT_EQUAL(ZK_OK, zk_pop_back(&list, NULL));
	}
	// no need to call zk_free() as list is freed by now
}

void test_zk_pop_back_with_free_func_when_list_has_n_elements(void)
{
	zk_c_slist *list = NULL;
	for (size_t i = 0; i < 10; i++) {
		zk_push_back(&list, strdup("data"));
	}

	while (list != NULL) {
		zk_c_slist *next = NULL;
		void *node_data = NULL;
		TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
		TEST_ASSERT_NOT_NULL(next);

		TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(list, &node_data));
		TEST_ASSERT_EQUAL_STRING("data", (char *)node_data);

		TEST_ASSERT_EQUAL(ZK_OK, zk_pop_back(&list, free));
	}

	// no need to call zk_free() as list is freed by now
}

// tests for zk_pop_front()
void test_zk_pop_front_when_reference_is_null(void)
{
	zk_c_slist **list = NULL;
	TEST_ASSERT_EQUAL(ZK_INVALID_ARGUMENT, zk_pop_front(list, NULL));
	TEST_ASSERT_NULL(list);
}
void test_zk_pop_front_when_list_is_empty(void)
{
	zk_c_slist *list = NULL;
	TEST_ASSERT_EQUAL(ZK_OK, zk_pop_front(&list, NULL));
	TEST_ASSERT_NULL(list);
}

void test_zk_pop_front_when_list_has_1_element(void)
{
	zk_c_slist *list = NULL;
	char *data[] = { "0" };
	zk_push_back(&list, data[0]);

	zk_pop_front(&list, NULL);
	TEST_ASSERT_NULL(list);

	// no need to call zk_free() as list is freed by now
}

void test_zk_pop_front_when_list_has_2_elements(void)
{
	zk_c_slist *list = NULL, *next = NULL;
	void *node_data = NULL;
	char *data[] = { "0", "1" };
	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, data[0]));
	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, data[1]));

	TEST_ASSERT_EQUAL(ZK_OK, zk_pop_front(&list, NULL));

	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
	TEST_ASSERT_EQUAL_PTR(list, next);
	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(list, &node_data));
	TEST_ASSERT_EQUAL_STRING(data[1], (char *)node_data);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_pop_front_when_list_has_3_elements(void)
{
	zk_c_slist *list = NULL, *first_node = NULL, *last_node = NULL, *next = NULL;
	void *node_data = NULL;
	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, "0"));
	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, "1"));
	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, "2"));

	TEST_ASSERT_EQUAL(ZK_OK, zk_pop_front(&list, NULL));
	TEST_ASSERT_NOT_NULL(list);

	first_node = zk_begin(list);
	last_node = zk_end(list);

	// checks 1st element
	TEST_ASSERT_NOT_NULL(first_node);
	TEST_ASSERT_EQUAL(ZK_OK, zk_next(first_node, &next));
	TEST_ASSERT_EQUAL_PTR(last_node, next);
	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(first_node, &node_data));
	TEST_ASSERT_EQUAL_STRING("1", (char *)node_data);

	// checks 2nd element
	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(last_node, &node_data));
	TEST_ASSERT_EQUAL_STRING("2", (char *)node_data);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_pop_front_when_list_has_n_elements(void)
{
	char *data[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
	zk_c_slist *list = NULL;
	for (size_t i = 0; i < 10; i++) {
		TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, data[i]));
	}

	// pops all elements
	while (list != NULL) {
		zk_c_slist *next = NULL;
		void *node_data = NULL;

		TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
		TEST_ASSERT_NOT_NULL(next);

		TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(list, &node_data));
		TEST_ASSERT_EQUAL_STRING("9", (char *)node_data);

		zk_pop_front(&list, NULL);
	}
	// no need to call zk_free() as list is freed by now
}

void test_zk_pop_front_with_free_func_when_list_has_n_elements(void)
{
	zk_c_slist *list = NULL;
	for (size_t i = 0; i < 10; i++) {
		TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, strdup("data")));
	}

	// pops all elements
	while (list != NULL) {
		zk_c_slist *next = NULL;
		void *node_data = NULL;

		TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
		TEST_ASSERT_NOT_NULL(next);
		TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(list, &node_data));
		TEST_ASSERT_EQUAL_STRING("data", (char *)node_data);

		zk_pop_front(&list, free);
	}

	// no need to call zk_free() as list is freed by now
}

// tests for zk_push_back()
void test_zk_push_back_when_reference_is_null(void)
{
	zk_c_slist **list_p = NULL;
	TEST_ASSERT_EQUAL(ZK_INVALID_ARGUMENT, zk_push_back(list_p, NULL));
}
void test_zk_push_back_to_empty_list(void)
{
	zk_c_slist *list = NULL, *next = NULL;
	void *node_data = NULL;
	int data = 5;

	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, &data));

	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(list, &node_data));

	TEST_ASSERT_EQUAL_PTR(node_data, &data);
	TEST_ASSERT_EQUAL(*((int *)node_data), data);
	TEST_ASSERT_EQUAL_PTR(list, next);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_push_back_2_items_to_list(void)
{
	zk_c_slist *list = NULL, *node1 = NULL, *next = NULL;
	void *node_data = NULL;

	int node1_data = 0;
	int node2_data = 1;

	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, &node1_data));
	node1 = list;

	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(list, &node_data));
	TEST_ASSERT_EQUAL_PTR(node_data, &node1_data);
	TEST_ASSERT_EQUAL_PTR(*((int *)node_data), node1_data);

	TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
	TEST_ASSERT_EQUAL_PTR(list, next);

	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, &node2_data));

	// checks that list points to the last element of the list
	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(list, &node_data));
	TEST_ASSERT_EQUAL_PTR(node_data, &node2_data);
	TEST_ASSERT_EQUAL_PTR(*((int *)node_data), node2_data);

	TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
	TEST_ASSERT_NOT_NULL(next);
	TEST_ASSERT_EQUAL_PTR(node1, next);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_push_back_3_items_to_list(void)
{
	zk_c_slist *list = NULL, *node1 = NULL, *next = NULL;
	void *node_data = NULL;

	int node1_data = 0;
	int node2_data = 1;
	int node3_data = 3;

	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, &node1_data));
	node1 = list;

	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(list, &node_data));
	TEST_ASSERT_EQUAL_PTR(node_data, &node1_data);
	TEST_ASSERT_EQUAL(*((int *)node_data), node1_data);

	TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
	TEST_ASSERT_EQUAL_PTR(list, next);

	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, &node2_data));

	// checks that list points to the last element of the list
	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(list, &node_data));
	TEST_ASSERT_EQUAL_PTR(node_data, &node2_data);
	TEST_ASSERT_EQUAL_PTR(*((int *)node_data), node2_data);

	TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
	TEST_ASSERT_NOT_NULL(next);
	TEST_ASSERT_EQUAL_PTR(node1, next);

	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, &node3_data));

	// checks that list points to the last element of the list
	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(list, &node_data));
	TEST_ASSERT_EQUAL_PTR(node_data, &node3_data);
	TEST_ASSERT_EQUAL(*((int *)node_data), node3_data);

	TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
	TEST_ASSERT_EQUAL_PTR(node1, next);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_push_back_n_items_to_list(void)
{
	int number_of_nodes = 100;
	int nodes_data[number_of_nodes];
	zk_c_slist *list = NULL, *next = NULL;
	void *node_data = NULL;

	for (int i = 0; i < number_of_nodes; i++) {
		nodes_data[i] = i;
		TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, &nodes_data[i]));
		TEST_ASSERT_NOT_NULL(list);
		TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
		TEST_ASSERT_NOT_NULL(next);
	}

	int node_idx = 0;
	zk_c_slist *current = zk_begin(list); // begin of the list
	zk_c_slist *last = zk_end(list);
	while (current != last) {
		TEST_ASSERT_NOT_NULL(current);
		TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(current, &node_data));
		TEST_ASSERT_EQUAL_PTR(node_data, &nodes_data[node_idx]);
		TEST_ASSERT_EQUAL(*((int *)node_data), nodes_data[node_idx]);
		TEST_ASSERT_EQUAL(ZK_OK, zk_next(current, &current));
		node_idx++;
	}

	// checks end node
	TEST_ASSERT_NOT_NULL(current);
	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(current, &node_data));
	TEST_ASSERT_EQUAL_PTR(node_data, &nodes_data[node_idx]);
	TEST_ASSERT_EQUAL(*((int *)node_data), nodes_data[node_idx]);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_push_back_null_data_to_list(void)
{
	int number_of_nodes = 100;
	zk_c_slist *list = NULL, *next = NULL;
	void *node_data = NULL;

	for (int i = 0; i < number_of_nodes; i++) {
		TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, NULL));
		TEST_ASSERT_NOT_NULL(list);
		TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
		TEST_ASSERT_NOT_NULL(next);
	}

	zk_c_slist *current = zk_begin(list); // begin of the list
	zk_c_slist *last = zk_end(list);
	while (current != last) {
		TEST_ASSERT_NOT_NULL(current);
		TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(current, &node_data));
		TEST_ASSERT_EQUAL_PTR(node_data, NULL);

		TEST_ASSERT_EQUAL(ZK_OK, zk_next(current, &current));
	}
	// checks end node
	TEST_ASSERT_NOT_NULL(current);
	TEST_ASSERT_EQUAL_PTR(node_data, NULL);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

// tests for for zk_push_front()
void test_zk_push_front_when_reference_is_null(void)
{
	zk_c_slist **list_p = NULL;
	TEST_ASSERT_EQUAL(ZK_INVALID_ARGUMENT, zk_push_front(list_p, NULL));
}

void test_zk_push_front_n_items_to_list(void)
{
	int number_of_nodes = 100;
	int nodes_data[number_of_nodes];
	zk_c_slist *list = NULL, *next = NULL;
	void *node_data = NULL;

	for (int i = 0; i < number_of_nodes; i++) {
		nodes_data[i] = i;
		TEST_ASSERT_EQUAL(ZK_OK, zk_push_front(&list, &nodes_data[i]));
		TEST_ASSERT_NOT_NULL(list);
		TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
		TEST_ASSERT_NOT_NULL(next);
	}

	int node_idx = 99;
	zk_c_slist *current = zk_begin(list); // begin of the list
	zk_c_slist *last = zk_end(list);
	while (current != last) {
		TEST_ASSERT_NOT_NULL(current);
		TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(current, &node_data));
		TEST_ASSERT_EQUAL_PTR(node_data, &nodes_data[node_idx]);
		TEST_ASSERT_EQUAL(*((int *)node_data), nodes_data[node_idx]);

		TEST_ASSERT_EQUAL(ZK_OK, zk_next(current, &next));
		TEST_ASSERT_NOT_NULL(next);
		TEST_ASSERT_EQUAL(ZK_OK, zk_next(current, &current));
		node_idx--;
	}
	// checks end node
	TEST_ASSERT_NOT_NULL(last);

	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(last, &node_data));
	TEST_ASSERT_EQUAL_PTR(node_data, &nodes_data[node_idx]);
	TEST_ASSERT_EQUAL(*((int *)node_data), nodes_data[node_idx]);

	TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
	TEST_ASSERT_NOT_NULL(next);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_push_front_null_data_to_list(void)
{
	int number_of_nodes = 100;
	zk_c_slist *list = NULL, *next = NULL;
	void *node_data = NULL;

	for (int i = 0; i < number_of_nodes; i++) {
		TEST_ASSERT_EQUAL(ZK_OK, zk_push_front(&list, NULL));
		TEST_ASSERT_NOT_NULL(list);
		TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
		TEST_ASSERT_NOT_NULL(next);
	}

	int node_idx = 99;
	zk_c_slist *current = zk_begin(list); // begin of the list
	zk_c_slist *last = zk_end(list);
	while (current != last) {
		TEST_ASSERT_NOT_NULL(current);

		TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(current, &node_data));
		TEST_ASSERT_EQUAL_PTR(node_data, NULL);

		TEST_ASSERT_EQUAL(ZK_OK, zk_next(current, &next));
		TEST_ASSERT_NOT_NULL(next);
		TEST_ASSERT_EQUAL(ZK_OK, zk_next(current, &current));
		node_idx--;
	}
	// checks end node
	TEST_ASSERT_NOT_NULL(last);

	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(last, &node_data));
	TEST_ASSERT_EQUAL_PTR(node_data, NULL);

	TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
	TEST_ASSERT_NOT_NULL(next);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

int main(void)
{
	UNITY_BEGIN();

	/*--------------- Test Constructor ---------------*/
	{ // tests for zk_c_slist_new_node()
		RUN_TEST(test_zk_c_slist_new_node_when_reference_is_null);
		RUN_TEST(test_zk_c_slist_new_node_when_data_is_null);
		RUN_TEST(test_zk_c_slist_new_node_when_data_is_not_null);
	}

	/*--------------- Test Destructor ---------------*/

	{ // tests for zk_free()
		RUN_TEST(test_zk_free_a_null_list_should_just_return);
		RUN_TEST(test_zk_free_list_with_no_dynamic_allocated_node_data);
		RUN_TEST(test_zk_free_for_list_of_strings);
		RUN_TEST(test_zk_free_for_a_null_list_should_just_return);
	}

	/*--------------- Test Iterators ---------------*/

	{ // tests for zk_begin()
		RUN_TEST(test_zk_begin_returns_null_when_list_is_empty);
		RUN_TEST(test_zk_begin_when_list_has_one_element_begin_equal_end);
		RUN_TEST(test_zk_begin_returns_1st_element_when_list_has_more_than_one_element);
	}

	{ // tests for zk_end()
		RUN_TEST(test_zk_end_returns_null_when_list_is_empty);
		RUN_TEST(test_zk_end_when_list_has_one_element_end_equal_begin);
		RUN_TEST(test_zk_end_returns_last_when_list_has_more_than_one_element);
	}

	{ // tests for zk_for_each()
		RUN_TEST(test_zk_for_each_when_func_is_null);
		RUN_TEST(test_zk_for_each_when_list_is_null);
		RUN_TEST(test_zk_for_each_when_func_is_not_null);
	}

	/*--------------- Test Modifiers ---------------*/

	{ // tests for zk_pop_back()
		RUN_TEST(test_zk_pop_back_when_reference_is_null);
		RUN_TEST(test_zk_pop_back_when_list_is_empty);
		RUN_TEST(test_zk_pop_back_when_list_has_1_element);
		RUN_TEST(test_zk_pop_back_when_list_has_2_elements);
		RUN_TEST(test_zk_pop_back_when_list_has_3_elements);
		RUN_TEST(test_zk_pop_back_when_list_has_n_elements);
		RUN_TEST(test_zk_pop_back_with_free_func_when_list_has_n_elements);
	}

	{ // tests for zk_pop_front()
		RUN_TEST(test_zk_pop_front_when_reference_is_null);
		RUN_TEST(test_zk_pop_front_when_list_is_empty);
		RUN_TEST(test_zk_pop_front_when_list_has_1_element);
		RUN_TEST(test_zk_pop_front_when_list_has_2_elements);
		RUN_TEST(test_zk_pop_front_when_list_has_3_elements);
		RUN_TEST(test_zk_pop_front_when_list_has_n_elements);
		RUN_TEST(test_zk_pop_front_with_free_func_when_list_has_n_elements);
	}

	{ // tests for zk_push_back function()
		RUN_TEST(test_zk_push_back_when_reference_is_null);
		RUN_TEST(test_zk_push_back_to_empty_list);
		RUN_TEST(test_zk_push_back_2_items_to_list);
		RUN_TEST(test_zk_push_back_3_items_to_list);
		RUN_TEST(test_zk_push_back_n_items_to_list);
		RUN_TEST(test_zk_push_back_null_data_to_list);
	}

	{ // tests for zk_push_front()

		RUN_TEST(test_zk_push_front_when_reference_is_null);
		RUN_TEST(test_zk_push_front_n_items_to_list);
		RUN_TEST(test_zk_push_front_null_data_to_list);
	}

	return UNITY_END();
}
