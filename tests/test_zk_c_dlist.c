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
// tests for zk_c_dlist_new_node()
void test_zk_c_dlist_new_node_when_reference_is_null(void)
{
	zk_c_dlist **list = NULL;
	TEST_ASSERT_EQUAL(ZK_INVALID_ARGUMENT, zk_c_dlist_new_node(list, NULL));
}

void test_zk_c_dlist_new_node_when_data_is_null(void)
{
	zk_c_dlist *list = NULL, *next = NULL, *prev = NULL;
	void *node_data = NULL;

	TEST_ASSERT_EQUAL(ZK_OK, zk_c_dlist_new_node(&list, NULL));
	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
	TEST_ASSERT_EQUAL(ZK_OK, zk_prev(list, &prev));
	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(list, &node_data));
	TEST_ASSERT_NULL(node_data);
	TEST_ASSERT_NULL(prev);
	TEST_ASSERT_NULL(next);

	free(list);
}
void test_zk_c_dlist_new_node_when_data_is_not_null(void)
{
	zk_c_dlist *list = NULL, *next = NULL, *prev = NULL;
	void *node_data = NULL;

	TEST_ASSERT_EQUAL(ZK_OK, zk_c_dlist_new_node(&list, "data"));
	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
	TEST_ASSERT_EQUAL(ZK_OK, zk_prev(list, &prev));
	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(list, &node_data));

	TEST_ASSERT_NOT_NULL(node_data);
	TEST_ASSERT_EQUAL("data", (char *)node_data);
	TEST_ASSERT_NULL(prev);
	TEST_ASSERT_NULL(next);

	free(list);
}

/*--------------- Test Destructor ---------------*/
// tests for zk_free()
void test_zk_free_a_null_list_should_just_return(void)
{
	zk_c_dlist *list = NULL;
	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_free_list_with_no_dynamic_allocated_node_data(void)
{
	zk_c_dlist *list = NULL;
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
	zk_c_dlist *list = NULL;

	for (int i = 0; i < number_of_nodes; i++) {
		zk_push_back(&list, strdup("a"));
		TEST_ASSERT_NOT_NULL(list);
	}

	zk_free(&list, free);
	TEST_ASSERT_NULL(list);
}

void test_zk_free_for_a_null_list_should_just_return(void)
{
	zk_c_dlist *list = NULL;
	zk_free(&list, free);
	TEST_ASSERT_NULL(list);
}

/*--------------- Test Iterators ---------------*/

// tests for for zk_begin()
void test_zk_begin_returns_null_when_list_is_empty(void)
{
	zk_c_dlist *list = NULL;

	TEST_ASSERT_NULL(zk_begin(list));
}

void test_zk_begin_when_list_has_one_element_begin_equal_end(void)
{
	zk_c_dlist *list = NULL, *begin = NULL;
	void *node_data = NULL, *begin_data = NULL;
	int node_1_data = 1;

	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, &node_1_data));

	begin = zk_begin(list);
	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(begin, &begin_data));

	TEST_ASSERT_EQUAL_PTR(list, begin);
	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(begin, &node_data));

	TEST_ASSERT_EQUAL_PTR(begin_data, node_data);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_begin_returns_1st_element_when_list_has_more_than_one_element(void)
{
	zk_c_dlist *list = NULL, *begin = NULL;
	void *begin_data = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;

	zk_push_back(&list, &node_1_data);
	zk_push_back(&list, &node_2_data);
	zk_push_back(&list, &node_3_data);

	begin = zk_begin(list);
	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(begin, &begin_data));

	TEST_ASSERT_EQUAL_PTR(list, begin);
	TEST_ASSERT_EQUAL_PTR(&node_1_data, begin_data);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

// tests for for zk_end()
void test_zk_end_returns_null_when_list_is_empty(void)
{
	zk_c_dlist *list = NULL;

	TEST_ASSERT_NULL(zk_end(list));
}

void test_zk_end_when_list_has_one_element_end_equal_begin(void)
{
	zk_c_dlist *list = NULL, *begin = NULL, *end = NULL;
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
	zk_c_dlist *list = NULL, *end = NULL, *prev = NULL;
	void *end_data = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;

	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, &node_1_data));
	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, &node_2_data));
	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, &node_3_data));

	end = zk_end(list);
	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(end, &end_data));
	TEST_ASSERT_EQUAL_PTR(&node_3_data, end_data);

	TEST_ASSERT_EQUAL(ZK_OK, zk_prev(list, &prev));
	TEST_ASSERT_EQUAL_PTR(prev, end);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

// tests for zk_for_each()
void test_zk_for_each_when_func_is_null(void)
{
	struct dummy_node_data *node_1_data = malloc(sizeof(struct dummy_node_data));
	node_1_data->value = 1;
	node_1_data->string = strdup("node_1");
	zk_c_dlist *list = NULL;

	zk_push_back(&list, node_1_data);
	TEST_ASSERT_NOT_NULL(list);

	// Func is null, it will just return
	zk_for_each(list, NULL, NULL);

	zk_free(&list, dummy_node_data_free);
	TEST_ASSERT_NULL(list);
}

void test_zk_for_each_when_list_is_null(void)
{
	zk_c_dlist *list = NULL;

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

	zk_c_dlist *list = NULL;
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
	zk_c_dlist **list = NULL;
	TEST_ASSERT_EQUAL(ZK_INVALID_ARGUMENT, zk_pop_back(list, NULL));
	TEST_ASSERT_NULL(list);
}

void test_zk_pop_back_when_list_is_empty(void)
{
	zk_c_dlist *list = NULL;
	TEST_ASSERT_EQUAL(ZK_OK, zk_pop_back(&list, NULL));
	TEST_ASSERT_NULL(list);
}

void test_zk_pop_back_when_list_has_1_element(void)
{
	zk_c_dlist *list = NULL;
	char *data[] = { "0" };
	zk_push_back(&list, data[0]);

	zk_pop_back(&list, NULL);
	TEST_ASSERT_NULL(list);

	// no need to call zk_free() as list is freed by now
}

void test_zk_pop_back_when_list_has_2_elements(void)
{
	zk_c_dlist *list = NULL, *prev = NULL, *next = NULL;
	void *node_data = NULL;
	char *data[] = { "0", "1" };

	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, data[0]));
	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, data[1]));

	zk_pop_back(&list, NULL);

	TEST_ASSERT_NOT_NULL(list);

	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(list, &node_data));
	TEST_ASSERT_EQUAL_STRING(data[0], (char *)node_data);

	TEST_ASSERT_EQUAL(ZK_OK, zk_prev(list, &prev));
	TEST_ASSERT_EQUAL_PTR(list, prev);

	TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
	TEST_ASSERT_EQUAL_PTR(list, next);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_pop_back_when_list_has_3_elements(void)
{
	zk_c_dlist *list = NULL, *prev = NULL, *next = NULL;
	void *node_data = NULL;
	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, "0"));
	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, "1"));
	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, "2"));

	TEST_ASSERT_EQUAL(ZK_OK, zk_pop_back(&list, NULL));
	TEST_ASSERT_NOT_NULL(list);

	// checks 1st element

	TEST_ASSERT_EQUAL(ZK_OK, zk_prev(list, &prev));
	TEST_ASSERT_NOT_NULL(prev);

	TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
	TEST_ASSERT_NOT_NULL(next);

	TEST_ASSERT_EQUAL(ZK_OK, zk_prev(prev, &prev));
	TEST_ASSERT_EQUAL_PTR(list, prev);

	TEST_ASSERT_EQUAL(ZK_OK, zk_next(next, &next));
	TEST_ASSERT_EQUAL_PTR(list, next);

	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(list, &node_data));
	TEST_ASSERT_EQUAL_STRING("0", (char *)node_data);

	// checks 2nd element
	TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(next, &node_data));
	TEST_ASSERT_EQUAL_STRING("1", (char *)node_data);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_pop_back_when_list_has_n_elements(void)
{
	char *data[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
	zk_c_dlist *list = NULL;
	for (size_t i = 0; i < 10; i++) {
		TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, data[i]));
	}

	// pops all elements
	while (list != NULL) {
		zk_c_dlist *prev = NULL, *next = NULL;
		void *node_data = NULL;
		TEST_ASSERT_EQUAL(ZK_OK, zk_prev(list, &prev));
		TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
		TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(list, &node_data));

		TEST_ASSERT_NOT_NULL(prev);
		TEST_ASSERT_NOT_NULL(next);

		// list always point to the first element
		TEST_ASSERT_EQUAL_STRING(data[0], (char *)node_data);
		TEST_ASSERT_EQUAL(ZK_OK, zk_pop_back(&list, NULL));
	}
	// no need to call zk_free() as list is freed by now
}

void test_zk_pop_back_with_free_func_when_list_has_n_elements(void)
{
	zk_c_dlist *list = NULL;
	for (size_t i = 0; i < 10; i++) {
		TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, strdup("data")));
	}

	// pops all elements
	while (list != NULL) {
		zk_c_dlist *prev = NULL, *next = NULL;
		void *node_data = NULL;
		TEST_ASSERT_EQUAL(ZK_OK, zk_prev(list, &prev));
		TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
		TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(list, &node_data));

		TEST_ASSERT_NOT_NULL(prev);
		TEST_ASSERT_NOT_NULL(next);
		TEST_ASSERT_EQUAL_STRING("data", (char *)node_data);

		TEST_ASSERT_EQUAL(ZK_OK, zk_pop_back(&list, free));
	}

	// no need to call zk_free() as list is freed by now
}

// tests for zk_pop_front()
void test_zk_pop_front_when_reference_is_null(void)
{
	zk_c_dlist **list = NULL;
	TEST_ASSERT_EQUAL(ZK_INVALID_ARGUMENT, zk_pop_front(list, NULL));
	TEST_ASSERT_NULL(list);
}

void test_zk_pop_front_when_list_is_empty(void)
{
	zk_c_dlist *list = NULL;
	TEST_ASSERT_EQUAL(ZK_OK, zk_pop_front(&list, NULL));
	TEST_ASSERT_NULL(list);
}

void test_zk_pop_front_when_list_has_1_element(void)
{
	zk_c_dlist *list = NULL;
	char *data[] = { "0" };
	zk_push_back(&list, data[0]);

	zk_pop_front(&list, NULL);
	TEST_ASSERT_NULL(list);

	// no need to call zk_free() as list is freed by now
}

void test_zk_pop_front_when_list_has_2_elements(void)
{
	zk_c_dlist *list = NULL, *next = NULL, *prev = NULL;
	void *node_data = NULL;
	char *data[] = { "0", "1" };
	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, data[0]));
	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, data[1]));

	TEST_ASSERT_EQUAL(ZK_OK, zk_pop_front(&list, NULL));

	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_EQUAL(ZK_OK, zk_prev(list, &prev));
	TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
	TEST_ASSERT_EQUAL_PTR(list, prev);
	TEST_ASSERT_EQUAL_PTR(list, next);

	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(list, &node_data));
	TEST_ASSERT_EQUAL_STRING(data[1], (char *)node_data);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_pop_front_when_list_has_3_elements(void)
{
	zk_c_dlist *list = NULL, *next = NULL, *prev = NULL;
	void *node_data = NULL;
	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, "0"));
	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, "1"));
	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, "2"));

	TEST_ASSERT_EQUAL(ZK_OK, zk_pop_front(&list, NULL));
	TEST_ASSERT_NOT_NULL(list);

	// checks 1st element
	TEST_ASSERT_EQUAL(ZK_OK, zk_prev(list, &prev));
	TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
	TEST_ASSERT_NOT_NULL(prev);
	TEST_ASSERT_NOT_NULL(next);

	TEST_ASSERT_EQUAL(ZK_OK, zk_prev(prev, &prev));
	TEST_ASSERT_EQUAL(ZK_OK, zk_next(next, &next));
	TEST_ASSERT_EQUAL_PTR(list, prev);
	TEST_ASSERT_EQUAL_PTR(list, next);

	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(list, &node_data));
	TEST_ASSERT_EQUAL_STRING("1", (char *)node_data);
	// checks 2nd element
	TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(next, &node_data));
	TEST_ASSERT_EQUAL_STRING("2", (char *)node_data);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_pop_front_when_list_has_n_elements(void)
{
	char *data[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
	zk_c_dlist *list = NULL;
	for (size_t i = 0; i < 10; i++) {
		TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, data[i]));
	}

	// pops all elements
	size_t i = 0;
	while (list != NULL) {
		zk_c_dlist *prev = NULL, *next = NULL;
		void *node_data = NULL;
		TEST_ASSERT_EQUAL(ZK_OK, zk_prev(list, &prev));
		TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
		TEST_ASSERT_NOT_NULL(prev);
		TEST_ASSERT_NOT_NULL(next);
		TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(list, &node_data));
		TEST_ASSERT_EQUAL_STRING(data[i++], (char *)node_data);

		TEST_ASSERT_EQUAL(ZK_OK, zk_pop_front(&list, NULL));
	}
	// no need to call zk_free() as list is freed by now
}

void test_zk_pop_front_with_free_func_when_list_has_n_elements(void)
{
	zk_c_dlist *list = NULL;
	for (size_t i = 0; i < 10; i++) {
		TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, strdup("data")));
	}

	// pops all elements
	while (list != NULL) {
		zk_c_dlist *prev = NULL, *next = NULL;
		void *node_data = NULL;
		TEST_ASSERT_EQUAL(ZK_OK, zk_prev(list, &prev));
		TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
		TEST_ASSERT_NOT_NULL(prev);
		TEST_ASSERT_NOT_NULL(next);
		TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(list, &node_data));
		TEST_ASSERT_EQUAL_STRING("data", (char *)node_data);

		TEST_ASSERT_EQUAL(ZK_OK, zk_pop_front(&list, free));
	}

	// no need to call zk_free() as list is freed by now
}

// tests for zk_push_back()
void test_zk_push_back_when_reference_is_null(void)
{
	zk_c_dlist **list_p = NULL;
	TEST_ASSERT_EQUAL(ZK_INVALID_ARGUMENT, zk_push_back(list_p, NULL));
}

void test_zk_push_back_to_empty_list(void)
{
	zk_c_dlist *list = NULL, *next = NULL, *prev = NULL;
	void *node_data = NULL;
	int data = 5;

	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, &data));

	TEST_ASSERT_NOT_NULL(list);

	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(list, &node_data));
	TEST_ASSERT_EQUAL_PTR(&data, node_data);
	TEST_ASSERT_EQUAL(data, *((int *)node_data));

	TEST_ASSERT_EQUAL(ZK_OK, zk_prev(list, &prev));
	TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
	TEST_ASSERT_EQUAL_PTR(list, prev);
	TEST_ASSERT_EQUAL_PTR(list, next);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_push_back_2_items_to_list(void)
{
	zk_c_dlist *list = NULL, *next = NULL, *prev = NULL;
	void *node_data = NULL;

	int node1_data = 0;
	int node2_data = 1;

	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, &node1_data));

	TEST_ASSERT_NOT_NULL(list);

	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(list, &node_data));
	TEST_ASSERT_EQUAL_PTR(node_data, &node1_data);
	TEST_ASSERT_EQUAL(*((int *)node_data), node1_data);

	TEST_ASSERT_EQUAL(ZK_OK, zk_prev(list, &prev));
	TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
	TEST_ASSERT_EQUAL_PTR(list, prev);
	TEST_ASSERT_EQUAL_PTR(list, next);

	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, &node2_data));

	// checks that list still points to the first element of the list
	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(list, &node_data));
	TEST_ASSERT_EQUAL_PTR(node_data, &node1_data);
	TEST_ASSERT_EQUAL(*((int *)node_data), node1_data);
	TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
	TEST_ASSERT_NOT_NULL(next);

	// checks 2nd element
	TEST_ASSERT_EQUAL(ZK_OK, zk_prev(list, &prev));
	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(prev, &node_data));
	TEST_ASSERT_EQUAL_PTR(&node2_data, node_data);

	TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(next, &node_data));
	TEST_ASSERT_EQUAL(node2_data, *((int *)node_data));
	TEST_ASSERT_EQUAL_PTR(&node2_data, node_data);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_push_back_3_items_to_list(void)
{
	zk_c_dlist *list = NULL, *next = NULL, *prev = NULL;
	void *node_data = NULL;

	int node1_data = 0;
	int node2_data = 1;
	int node3_data = 3;

	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, &node1_data));

	TEST_ASSERT_NOT_NULL(list);

	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(list, &node_data));
	TEST_ASSERT_EQUAL_PTR(node_data, &node1_data);
	TEST_ASSERT_EQUAL(*((int *)node_data), node1_data);

	TEST_ASSERT_EQUAL(ZK_OK, zk_prev(list, &prev));
	TEST_ASSERT_EQUAL_PTR(list, prev);

	TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
	TEST_ASSERT_EQUAL_PTR(list, next);

	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, &node2_data));

	// checks that list points to the first element of the list
	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(list, &node_data));
	TEST_ASSERT_EQUAL_PTR(node_data, &node1_data);
	TEST_ASSERT_EQUAL(*((int *)node_data), node1_data);

	// checks 2nd element
	TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(next, &node_data));
	TEST_ASSERT_EQUAL_PTR(&node2_data, node_data);
	TEST_ASSERT_EQUAL(node2_data, *((int *)node_data));

	TEST_ASSERT_EQUAL(ZK_OK, zk_prev(list, &prev));
	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(prev, &node_data));
	TEST_ASSERT_EQUAL_PTR(&node2_data, node_data);

	TEST_ASSERT_EQUAL_PTR(prev, next);
	TEST_ASSERT_EQUAL(ZK_OK, zk_prev(next, &prev));
	TEST_ASSERT_EQUAL_PTR(list, prev);

	TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, &node3_data)); // add 3th element to the list

	// checks that list points to the first element of the list
	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(list, &node_data));
	TEST_ASSERT_EQUAL_PTR(node_data, &node1_data);
	TEST_ASSERT_EQUAL(*((int *)node_data), node1_data);

	// checks 3nd element
	TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next)); // next points to 2th element
	TEST_ASSERT_EQUAL(ZK_OK, zk_next(next, &next)); // next points to 3th element
	TEST_ASSERT_EQUAL(ZK_OK, zk_prev(list, &prev)); // prev points to 3th element

	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(prev, &node_data)); // node_data points to data of 3th element
	TEST_ASSERT_EQUAL_PTR(&node3_data, node_data);
	TEST_ASSERT_EQUAL(node3_data, *((int *)node_data));

	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(next, &node_data)); // node_data points to data of 3th element
	TEST_ASSERT_EQUAL_PTR(&node3_data, node_data);

	TEST_ASSERT_EQUAL(ZK_OK, zk_next(next, &next)); // next points to  1st element
	TEST_ASSERT_EQUAL_PTR(list, next);

	TEST_ASSERT_EQUAL(ZK_OK, zk_prev(prev, &prev)); // prev points to 2th element
	TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next)); // next points to 2th element
	TEST_ASSERT_EQUAL_PTR(next, prev);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_push_back_n_items_to_list(void)
{
	int number_of_nodes = 100;
	int nodes_data[number_of_nodes];
	zk_c_dlist *list = NULL, *next = NULL, *prev = NULL;
	void *node_data = NULL;

	for (int i = 0; i < number_of_nodes; i++) {
		nodes_data[i] = i;
		TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, &nodes_data[i]));
		TEST_ASSERT_NOT_NULL(list);
	}

	int node_idx = 0;
	zk_c_dlist *current = list; // begin of the list
	zk_c_dlist *last = zk_end(list); // end of the list
	while (current != last) {
		TEST_ASSERT_NOT_NULL(current);

		TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(current, &node_data));
		TEST_ASSERT_EQUAL_PTR(node_data, &nodes_data[node_idx]);
		TEST_ASSERT_EQUAL(*((int *)node_data), nodes_data[node_idx]);

		TEST_ASSERT_EQUAL(ZK_OK, zk_next(current, &next));
		TEST_ASSERT_NOT_NULL(next);

		TEST_ASSERT_EQUAL(ZK_OK, zk_prev(current, &prev));
		TEST_ASSERT_NOT_NULL(prev);

		TEST_ASSERT_EQUAL(ZK_OK, zk_next(current, &current));
		node_idx++;
	}

	// checks end node
	TEST_ASSERT_NOT_NULL(current);

	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(current, &node_data));
	TEST_ASSERT_EQUAL_PTR(node_data, &nodes_data[node_idx]);
	TEST_ASSERT_EQUAL(*((int *)node_data), nodes_data[node_idx]);

	TEST_ASSERT_EQUAL(ZK_OK, zk_next(current, &next));
	TEST_ASSERT_NOT_NULL(prev);
	TEST_ASSERT_EQUAL(ZK_OK, zk_prev(current, &prev));
	TEST_ASSERT_NOT_NULL(next);
	TEST_ASSERT_EQUAL_PTR(list, next);

	TEST_ASSERT_EQUAL(ZK_OK, zk_prev(list, &prev));
	TEST_ASSERT_EQUAL_PTR(prev, current);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_push_back_null_data_to_list(void)
{
	int number_of_nodes = 100;
	zk_c_dlist *list = NULL, *next = NULL, *prev = NULL;
	void *node_data = NULL;

	for (int i = 0; i < number_of_nodes; i++) {
		TEST_ASSERT_EQUAL(ZK_OK, zk_push_back(&list, NULL));
		TEST_ASSERT_NOT_NULL(list);
	}

	zk_c_dlist *current = list; // begin of the list
	zk_c_dlist *last = zk_end(list); // end of the list
	while (current != last) {
		TEST_ASSERT_NOT_NULL(current);
		TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(current, &node_data));
		TEST_ASSERT_EQUAL_PTR(node_data, NULL);

		TEST_ASSERT_EQUAL(ZK_OK, zk_next(current, &next));
		TEST_ASSERT_NOT_NULL(next);

		TEST_ASSERT_EQUAL(ZK_OK, zk_prev(current, &prev));
		TEST_ASSERT_NOT_NULL(prev);

		TEST_ASSERT_EQUAL(ZK_OK, zk_next(current, &current));
	}

	// checks end node
	TEST_ASSERT_NOT_NULL(current);
	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(current, &node_data));
	TEST_ASSERT_EQUAL_PTR(node_data, NULL);

	TEST_ASSERT_EQUAL(ZK_OK, zk_next(current, &next));
	TEST_ASSERT_NOT_NULL(next);
	TEST_ASSERT_EQUAL_PTR(list, next);

	TEST_ASSERT_EQUAL(ZK_OK, zk_prev(current, &prev));
	TEST_ASSERT_NOT_NULL(prev);

	TEST_ASSERT_EQUAL(ZK_OK, zk_prev(list, &prev));
	TEST_ASSERT_EQUAL_PTR(prev, current);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

// tests for for zk_push_front()
void test_zk_push_front_when_reference_is_null(void)
{
	zk_c_dlist **list_p = NULL;
	TEST_ASSERT_EQUAL(ZK_INVALID_ARGUMENT, zk_push_front(list_p, NULL));
}

void test_zk_push_front_n_items_to_list(void)
{
	int number_of_nodes = 100;
	int nodes_data[number_of_nodes];
	zk_c_dlist *list = NULL, *next = NULL, *prev = NULL;
	void *node_data = NULL;

	for (int i = 0; i < number_of_nodes; i++) {
		nodes_data[i] = i;
		TEST_ASSERT_EQUAL(ZK_OK, zk_push_front(&list, &nodes_data[i]));
		TEST_ASSERT_NOT_NULL(list);

		TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
		TEST_ASSERT_NOT_NULL(next);

		TEST_ASSERT_EQUAL(ZK_OK, zk_prev(list, &prev));
		TEST_ASSERT_NOT_NULL(prev);
	}

	int node_idx = 99;
	zk_c_dlist *current = list; // begin of the list
	zk_c_dlist *last = zk_end(list); // end of the list
	while (current != last) {
		TEST_ASSERT_NOT_NULL(current);

		TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(current, &node_data));
		TEST_ASSERT_EQUAL_PTR(node_data, &nodes_data[node_idx]);
		TEST_ASSERT_EQUAL(*((int *)node_data), nodes_data[node_idx]);

		TEST_ASSERT_EQUAL(ZK_OK, zk_next(current, &next));
		TEST_ASSERT_NOT_NULL(next);

		TEST_ASSERT_EQUAL(ZK_OK, zk_prev(current, &prev));
		TEST_ASSERT_NOT_NULL(prev);

		TEST_ASSERT_EQUAL(ZK_OK, zk_next(current, &current));

		node_idx--;
	}
	// checks end node
	TEST_ASSERT_NOT_NULL(last);

	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(last, &node_data));
	TEST_ASSERT_EQUAL_PTR(node_data, &nodes_data[node_idx]);
	TEST_ASSERT_EQUAL(*((int *)node_data), nodes_data[node_idx]);

	TEST_ASSERT_EQUAL(ZK_OK, zk_next(last, &next));
	TEST_ASSERT_NOT_NULL(next);
	TEST_ASSERT_EQUAL_PTR(list, next);

	TEST_ASSERT_EQUAL(ZK_OK, zk_prev(last, &prev));
	TEST_ASSERT_NOT_NULL(prev);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_push_front_null_data_to_list(void)
{
	int number_of_nodes = 100;
	zk_c_dlist *list = NULL, *next = NULL, *prev = NULL;
	void *node_data = NULL;

	for (int i = 0; i < number_of_nodes; i++) {
		TEST_ASSERT_EQUAL(ZK_OK, zk_push_front(&list, NULL));

		TEST_ASSERT_NOT_NULL(list);
		TEST_ASSERT_EQUAL(ZK_OK, zk_next(list, &next));
		TEST_ASSERT_NOT_NULL(next);

		TEST_ASSERT_EQUAL(ZK_OK, zk_prev(list, &prev));
		TEST_ASSERT_NOT_NULL(prev);
	}

	int node_idx = 99;
	zk_c_dlist *current = list; // begin of the list
	zk_c_dlist *last = zk_end(list); // end of the list
	while (current != last) {
		TEST_ASSERT_NOT_NULL(current);
		TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(current, &node_data));
		TEST_ASSERT_EQUAL_PTR(node_data, NULL);

		TEST_ASSERT_EQUAL(ZK_OK, zk_next(current, &next));
		TEST_ASSERT_NOT_NULL(next);

		TEST_ASSERT_EQUAL(ZK_OK, zk_prev(current, &prev));
		TEST_ASSERT_NOT_NULL(prev);

		TEST_ASSERT_EQUAL(ZK_OK, zk_next(current, &current));

		node_idx--;
	}

	// checks end node
	TEST_ASSERT_NOT_NULL(last);
	TEST_ASSERT_EQUAL(ZK_OK, zk_get_data(last, &node_data));
	TEST_ASSERT_EQUAL_PTR(node_data, NULL);

	TEST_ASSERT_EQUAL(ZK_OK, zk_next(last, &next));
	TEST_ASSERT_NOT_NULL(next);
	TEST_ASSERT_EQUAL_PTR(list, next);

	TEST_ASSERT_EQUAL(ZK_OK, zk_prev(last, &prev));
	TEST_ASSERT_NOT_NULL(prev);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

int main(void)
{
	UNITY_BEGIN();

	/*--------------- Test Constructor ---------------*/
	{ // tests for zk_c_dlist_new_node()
		RUN_TEST(test_zk_c_dlist_new_node_when_reference_is_null);
		RUN_TEST(test_zk_c_dlist_new_node_when_data_is_null);
		RUN_TEST(test_zk_c_dlist_new_node_when_data_is_not_null);
	}

	/*--------------- Test Destructor ---------------*/

	{ // tests for zk_free()
		RUN_TEST(test_zk_free_a_null_list_should_just_return);
		RUN_TEST(test_zk_free_list_with_no_dynamic_allocated_node_data);
		RUN_TEST(test_zk_free_for_list_of_strings);
		RUN_TEST(test_zk_free_for_a_null_list_should_just_return);
	}

	// /*--------------- Test Iterators ---------------*/

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

	// /*--------------- Test Modifiers ---------------*/

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
