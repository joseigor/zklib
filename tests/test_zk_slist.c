#include <stdlib.h>
#include <string.h>

#include "common/test_common.h"
#include "unity.h"
#include "zk/zklib.h"
#include "zk_common/zk_common.h"

void setUp(void) {}

void tearDown(void) {}

/*--------------- Test Destructor ---------------*/
// test zk_free
void test_zk_free_a_null_list_should_just_return(void)
{
	zk_slist *list = NULL;
	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_free_list_with_no_dynamic_allocated_node_data(void)
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

	zk_free(&list, NULL);

	TEST_ASSERT_NULL(list);
}

void test_zk_free_for_list_of_strings(void)
{
	int number_of_nodes = 100;
	zk_slist *list = NULL;

	for (int i = 0; i < number_of_nodes; i++) {
		list = zk_slist_push_back(list, strdup("a"));
		TEST_ASSERT_NOT_NULL(list);
	}

	zk_free(&list, free);
	TEST_ASSERT_NULL(list);
}

void test_zk_free_for_a_null_list_should_just_return(void)
{
	zk_slist *list = NULL;
	zk_free(&list, free);
	TEST_ASSERT_NULL(list);
}

/*--------------- Test Iterators ---------------*/

// test for zk_begin
void test_zk_begin_returns_null_when_slist_is_empty(void)
{
	zk_slist *list = NULL;

	TEST_ASSERT_NULL(zk_begin(list));
}

void test_zk_begin_returns_1st_element_when_slist_has_one_element(void)
{
	zk_slist *list = NULL, *begin = NULL;
	int node_1_data = 1;

	list = zk_slist_push_back(list, &node_1_data);

	begin = zk_begin(list);

	TEST_ASSERT_EQUAL_PTR(list, begin);

	void *data = NULL;
	zk_get_data(list, &data);
	TEST_ASSERT_NOT_NULL(data);
	TEST_ASSERT_EQUAL(node_1_data, *((int *)data));

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_begin_returns_1st_element_when_slist_has_more_than_one_element(void)
{
	zk_slist *list = NULL, *begin = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;

	list = zk_slist_push_back(list, &node_1_data);
	list = zk_slist_push_back(list, &node_2_data);
	list = zk_slist_push_back(list, &node_3_data);

	begin = zk_begin(list);

	TEST_ASSERT_EQUAL_PTR(list, begin);

	void *begin_data, *list_data = NULL;
	zk_get_data(begin, &begin_data);
	zk_get_data(list, &list_data);
	TEST_ASSERT_EQUAL(list_data, begin_data);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

// test for zk_end
void test_zk_end_returns_null_when_slist_is_empty(void)
{
	zk_slist *list = NULL;

	TEST_ASSERT_NULL(zk_end(list));
}

void test_zk_end_returns_null_when_slist_has_one_element(void)
{
	zk_slist *list = NULL;
	int node_1_data = 1;

	list = zk_slist_push_back(list, &node_1_data);

	TEST_ASSERT_NULL(zk_end(list));

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_end_returns_null_when_slist_has_more_than_one_element(void)
{
	zk_slist *list = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;

	list = zk_slist_push_back(list, &node_1_data);
	list = zk_slist_push_back(list, &node_2_data);
	list = zk_slist_push_back(list, &node_3_data);

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
	zk_slist *list = NULL;

	list = zk_slist_push_back(list, node_1_data);
	TEST_ASSERT_NOT_NULL(list);

	// Func is null, it will just return
	zk_for_each(list, NULL, NULL);

	zk_free(&list, dummy_node_data_free);
	TEST_ASSERT_NULL(list);
}

void test_zk_for_each_when_list_is_null(void)
{
	zk_slist *list = NULL;

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

	zk_slist *list = NULL;
	list = zk_slist_push_back(list, node_1_data);
	list = zk_slist_push_back(list, node_2_data);
	list = zk_slist_push_back(list, node_3_data);

	// Use foreach to clean each node data
	zk_for_each(list, dummy_node_data_free_foreach, NULL);

	// As nodes data were freed, we just need to call zk_free.
	zk_free(&list, NULL);
}

/*--------------- Test Modifiers ---------------*/
// tests for zk_pop_back()
void test_zk_pop_back_when_reference_is_null(void)
{
	zk_slist **list = NULL;
	TEST_ASSERT_EQUAL(ZK_INVALID_ARGUMENT, zk_pop_back(list, NULL));
	TEST_ASSERT_NULL(list);
}

void test_zk_pop_back_when_list_is_empty(void)
{
	zk_slist *list = NULL;
	TEST_ASSERT_EQUAL(ZK_OK, zk_pop_back(&list, NULL));
	TEST_ASSERT_NULL(list);
}

void test_zk_pop_back_when_list_has_1_element(void)
{
	zk_slist *list = NULL;
	char *data[] = { "0" };
	list = zk_slist_push_back(list, data[0]);

	zk_pop_back(&list, NULL);
	TEST_ASSERT_NULL(list);

	// no need to call zk_free() as list is freed by now
}

void test_zk_pop_back_when_list_has_2_elements(void)
{
	zk_slist *list = NULL;
	char *data[] = { "0", "1" };
	list = zk_slist_push_back(list, data[0]);
	list = zk_slist_push_back(list, data[1]);

	zk_pop_back(&list, NULL);

	TEST_ASSERT_NOT_NULL(list);

	zk_slist *next = NULL;
	zk_next(list, &next);
	TEST_ASSERT_NULL(next);

	void *element_data = NULL;
	zk_get_data(list, &element_data);
	TEST_ASSERT_EQUAL_STRING(data[0], (char *)element_data);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_pop_back_when_list_has_3_elements(void)
{
	zk_slist *list = NULL, *next = NULL;

	void *element_data = NULL;

	list = zk_slist_push_back(list, "0");
	list = zk_slist_push_back(list, "1");
	list = zk_slist_push_back(list, "2");

	zk_pop_back(&list, NULL);

	TEST_ASSERT_NOT_NULL(list);

	zk_get_data(list, &element_data);
	TEST_ASSERT_EQUAL_STRING("0", (char *)element_data);

	zk_next(list, &next);
	TEST_ASSERT_NOT_NULL(next);

	zk_get_data(next, &element_data);
	TEST_ASSERT_EQUAL_STRING("1", (char *)element_data);

	zk_next(next, &next);
	TEST_ASSERT_NULL(next);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_pop_back_when_list_has_n_elements(void)
{
	char *data[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
	zk_slist *list = NULL;
	for (size_t i = 0; i < 10; i++) {
		list = zk_slist_push_back(list, data[i]);
	}

	// pop all elements
	while (list != NULL) {
		TEST_ASSERT_NOT_NULL(list);
		void *element_data = NULL;
		zk_get_data(list, &element_data);
		TEST_ASSERT_EQUAL_STRING("0", (char *)element_data);
		zk_pop_back(&list, NULL);
	}
	// no need to call zk_free() as list is freed by now
}
void test_zk_pop_back_with_free_func_when_list_has_n_elements(void)
{
	zk_slist *list = NULL;
	for (size_t i = 0; i < 10; i++) {
		list = zk_slist_push_back(list, strdup("data"));
	}

	while (list != NULL) {
		TEST_ASSERT_NOT_NULL(list);
		void *element_data = NULL;
		zk_get_data(list, &element_data);
		TEST_ASSERT_EQUAL_STRING("data", (char *)element_data);
		zk_pop_back(&list, free);
	}

	// no need to call zk_free() as list is freed by now
}

// tests for zk_pop_front()
void test_zk_pop_front_when_reference_is_null(void)
{
	zk_slist **list = NULL;
	TEST_ASSERT_EQUAL(ZK_INVALID_ARGUMENT, zk_pop_front(list, NULL));
	TEST_ASSERT_NULL(list);
}

void test_zk_pop_front_when_list_is_empty(void)
{
	zk_slist *list = NULL;
	TEST_ASSERT_EQUAL(ZK_OK, zk_pop_front(&list, NULL));
	TEST_ASSERT_NULL(list);
}

void test_zk_pop_front_when_list_has_1_element(void)
{
	zk_slist *list = NULL;
	char *data[] = { "0" };
	list = zk_slist_push_back(list, data[0]);

	zk_pop_front(&list, NULL);
	TEST_ASSERT_NULL(list);

	// no need to call zk_free() as list is freed by now
}

void test_zk_pop_front_when_list_has_2_elements(void)
{
	zk_slist *list = NULL, *next = NULL;
	char *data[] = { "0", "1" };
	void *element_data = NULL;

	list = zk_slist_push_back(list, data[0]);
	list = zk_slist_push_back(list, data[1]);

	zk_pop_front(&list, NULL);

	TEST_ASSERT_NOT_NULL(list);

	zk_next(list, &next);
	TEST_ASSERT_NULL(next);

	zk_get_data(list, &element_data);
	TEST_ASSERT_EQUAL_STRING(data[1], (char *)element_data);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_pop_front_when_list_has_3_elements(void)
{
	zk_slist *list = NULL, *next = NULL;
	void *element_data = NULL;

	list = zk_slist_push_back(list, "0");
	list = zk_slist_push_back(list, "1");
	list = zk_slist_push_back(list, "2");

	zk_pop_front(&list, NULL);

	TEST_ASSERT_NOT_NULL(list);
	zk_get_data(list, &element_data);
	TEST_ASSERT_EQUAL_STRING("1", (char *)element_data);

	zk_next(list, &next);
	TEST_ASSERT_NOT_NULL(next);
	zk_get_data(next, &element_data);
	TEST_ASSERT_EQUAL_STRING("2", (char *)element_data);

	zk_next(next, &next);
	TEST_ASSERT_NULL(next);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_pop_front_when_list_has_n_elements(void)
{
	char *data[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
	zk_slist *list = NULL;
	for (size_t i = 0; i < 10; i++) {
		list = zk_slist_push_back(list, data[i]);
	}

	// pop all elements
	size_t i = 0;
	while (list != NULL) {
		TEST_ASSERT_NOT_NULL(list);
		void *element_data = NULL;
		zk_get_data(list, &element_data);
		TEST_ASSERT_EQUAL_STRING(data[i++], (char *)element_data);
		zk_pop_front(&list, NULL);
	}
	// no need to call zk_free() as list is freed by now
}

void test_zk_pop_front_with_free_func_when_list_has_n_elements(void)
{
	zk_slist *list = NULL;
	for (size_t i = 0; i < 10; i++) {
		list = zk_slist_push_back(list, strdup("data"));
	}

	while (list != NULL) {
		TEST_ASSERT_NOT_NULL(list);
		void *element_data = NULL;
		zk_get_data(list, &element_data);
		TEST_ASSERT_EQUAL_STRING("data", (char *)element_data);
		zk_pop_front(&list, free);
	}

	// no need to call zk_free() as list is freed by now
}

int main(void)
{
	UNITY_BEGIN();

	// /*--------------- Test Destructor ---------------*/

	{ // tests for zk_free()
		RUN_TEST(test_zk_free_a_null_list_should_just_return);
		RUN_TEST(test_zk_free_list_with_no_dynamic_allocated_node_data);
		RUN_TEST(test_zk_free_for_list_of_strings);
		RUN_TEST(test_zk_free_for_a_null_list_should_just_return);
	}

	// /*--------------- Test Iterators ---------------*/

	{ // tests for zk_for_each()
		RUN_TEST(test_zk_for_each_when_func_is_null);
		RUN_TEST(test_zk_for_each_when_list_is_null);
		RUN_TEST(test_zk_for_each_when_func_is_not_null);
	}

	{ // tests for zk_begin()
		RUN_TEST(test_zk_begin_returns_null_when_slist_is_empty);
		RUN_TEST(test_zk_begin_returns_1st_element_when_slist_has_one_element);
		RUN_TEST(test_zk_begin_returns_1st_element_when_slist_has_more_than_one_element);
	}

	{ // tests for zk_end()
		RUN_TEST(test_zk_end_returns_null_when_slist_is_empty);
		RUN_TEST(test_zk_end_returns_null_when_slist_has_one_element);
		RUN_TEST(test_zk_end_returns_null_when_slist_has_more_than_one_element);
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

	return UNITY_END();
}
