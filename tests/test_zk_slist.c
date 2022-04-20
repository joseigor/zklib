#include <stdlib.h>
#include <string.h>

#include "unity.h"
#include "zk/zklib.h"
#include "zk_common/zk_utils.h"

struct dummy_node_data {
	int value;
	char *string;
};

static void dummy_node_data_free(void *data)
{
	struct dummy_node_data *node_data = data;
	free(node_data->string);
	free(node_data);
}

static void *copy_node_data(ZK_GNUC_UNUSED const void *const data, ZK_GNUC_UNUSED void *user_data)
{
	const struct dummy_node_data *const node_data = data;
	struct dummy_node_data *node_data_copy = malloc(sizeof(struct dummy_node_data));
	node_data_copy->value = node_data->value;
	node_data_copy->string = strdup(node_data->string);

	return node_data_copy;
}

void setUp(void)
{
	// set stuff up here
}

void tearDown(void)
{
	// clean stuff up here
}

void test_zk_slist_append_to_empty_list(void)
{
	zk_slist_t *slist = NULL;
	int data = 5;

	slist = zk_slist_append(slist, &data);

	TEST_ASSERT_NOT_NULL(slist);
	TEST_ASSERT_EQUAL_PTR(slist->data, &data);
	TEST_ASSERT_EQUAL_PTR(*((int *)slist->data), data);
	TEST_ASSERT_NULL(slist->next);

	zk_slist_free(&slist);
	TEST_ASSERT_NULL(slist);
}

void test_zk_slist_append_2_items_to_slist(void)
{
	zk_slist_t *slist = NULL;

	int node1_data = 0;
	int node2_data = 1;

	slist = zk_slist_append(slist, &node1_data);

	TEST_ASSERT_NOT_NULL(slist);
	TEST_ASSERT_EQUAL_PTR(slist->data, &node1_data);
	TEST_ASSERT_EQUAL_PTR(*((int *)slist->data), node1_data);
	TEST_ASSERT_NULL(slist->next);

	slist = zk_slist_append(slist, &node2_data);

	// first we check that slist still points to the head of the list
	TEST_ASSERT_NOT_NULL(slist);
	TEST_ASSERT_EQUAL_PTR(slist->data, &node1_data);
	TEST_ASSERT_EQUAL_PTR(*((int *)slist->data), node1_data);
	TEST_ASSERT_NOT_NULL(slist->next);

	// now we check the content of the second node of the list
	zk_slist_t *node_1 = slist->next;
	TEST_ASSERT_NOT_NULL(node_1);
	TEST_ASSERT_EQUAL_PTR(node_1->data, &node2_data);
	TEST_ASSERT_EQUAL_PTR(*((int *)node_1->data), node2_data);
	TEST_ASSERT_NULL(node_1->next);

	zk_slist_free(&slist);
	TEST_ASSERT_NULL(slist);
}

void test_zk_slist_append_3_items_to_slist(void)
{
	zk_slist_t *slist = NULL;

	int node1_data = 0;
	int node2_data = 1;
	int node3_data = 3;

	zk_slist_t *node_2 = NULL;
	zk_slist_t *node_3 = NULL;

	slist = zk_slist_append(slist, &node1_data);

	TEST_ASSERT_NOT_NULL(slist);
	TEST_ASSERT_EQUAL_PTR(slist->data, &node1_data);
	TEST_ASSERT_EQUAL_PTR(*((int *)slist->data), node1_data);
	TEST_ASSERT_NULL(slist->next);

	slist = zk_slist_append(slist, &node2_data);

	// first we check that slist still points to the head of the list
	TEST_ASSERT_NOT_NULL(slist);
	TEST_ASSERT_EQUAL_PTR(slist->data, &node1_data);
	TEST_ASSERT_EQUAL_PTR(*((int *)slist->data), node1_data);
	TEST_ASSERT_NOT_NULL(slist->next);

	// now we check the content of the second node of the list
	node_2 = slist->next;
	TEST_ASSERT_NOT_NULL(node_2);
	TEST_ASSERT_EQUAL_PTR(node_2->data, &node2_data);
	TEST_ASSERT_EQUAL_PTR(*((int *)node_2->data), node2_data);
	TEST_ASSERT_NULL(node_2->next);

	slist = zk_slist_append(slist, &node3_data);

	// first we check that slist still points to the head of the list
	TEST_ASSERT_NOT_NULL(slist);
	TEST_ASSERT_EQUAL_PTR(slist->data, &node1_data);
	TEST_ASSERT_EQUAL_PTR(*((int *)slist->data), node1_data);
	TEST_ASSERT_NOT_NULL(slist->next);

	// now we check the content of the second node of the list
	node_2 = slist->next;
	TEST_ASSERT_NOT_NULL(node_2);
	TEST_ASSERT_EQUAL_PTR(node_2->data, &node2_data);
	TEST_ASSERT_EQUAL_PTR(*((int *)node_2->data), node2_data);
	TEST_ASSERT_NOT_NULL(node_2->next);

	// now we check the content of the third node of the list

	node_3 = node_2->next;
	TEST_ASSERT_NOT_NULL(node_3);
	TEST_ASSERT_EQUAL_PTR(node_3->data, &node3_data);
	TEST_ASSERT_EQUAL_PTR(*((int *)node_3->data), node3_data);
	TEST_ASSERT_NULL(node_3->next);

	zk_slist_free(&slist);
	TEST_ASSERT_NULL(slist);
}

void test_zk_slist_append_n_items_to_slist(void)
{
	int number_of_nodes = 100;
	int nodes_data[number_of_nodes];
	zk_slist_t *slist = NULL;

	for (int i = 0; i < number_of_nodes; i++) {
		nodes_data[i] = i;
		slist = zk_slist_append(slist, &nodes_data[i]);
		TEST_ASSERT_NOT_NULL(slist);
	}

	int node_idx = 0;
	for (zk_slist_t *node_i = slist;; node_i = node_i->next) {
		TEST_ASSERT_NOT_NULL(node_i);
		TEST_ASSERT_EQUAL_PTR(node_i->data, &nodes_data[node_idx]);
		TEST_ASSERT_EQUAL_PTR(*((int *)node_i->data), nodes_data[node_idx]);
		printf("%d", *((int *)node_i->data));
		if (node_i->next == NULL) {
			printf("\n");
			break;
		}
		printf("->");
		node_idx++;
	}

	zk_slist_free(&slist);
	TEST_ASSERT_NULL(slist);
}

void test_zk_slist_append_null_data_to_slist(void)
{
	int number_of_nodes = 100;
	zk_slist_t *slist = NULL;

	for (int i = 0; i < number_of_nodes; i++) {
		slist = zk_slist_append(slist, NULL);
		TEST_ASSERT_NOT_NULL(slist);
	}

	for (zk_slist_t *node_i = slist;; node_i = node_i->next) {
		TEST_ASSERT_NOT_NULL(node_i);
		TEST_ASSERT_EQUAL_PTR(node_i->data, NULL);
		printf("%s", node_i->data ? "NOT_NULL" : "NULL");
		if (node_i->next == NULL) {
			printf("\n");
			break;
		}
		printf("->");
	}

	zk_slist_free(&slist);
	TEST_ASSERT_NULL(slist);
}

void test_zk_slist_prepend_n_items_to_slist(void)
{
	int number_of_nodes = 100;
	int nodes_data[number_of_nodes];
	zk_slist_t *slist = NULL;

	for (int i = 0; i < number_of_nodes; i++) {
		nodes_data[i] = i;
		slist = zk_slist_prepend(slist, &nodes_data[i]);
		TEST_ASSERT_NOT_NULL(slist);
	}

	int node_idx = 1;
	for (zk_slist_t *node_i = slist;; node_i = node_i->next) {
		TEST_ASSERT_NOT_NULL(node_i);
		TEST_ASSERT_EQUAL_PTR(node_i->data, &nodes_data[number_of_nodes - node_idx]);
		TEST_ASSERT_EQUAL_PTR(*((int *)node_i->data), nodes_data[number_of_nodes - node_idx]);
		printf("%d", *((int *)node_i->data));
		if (node_i->next == NULL) {
			printf("\n");
			break;
		}
		printf("->");
		node_idx++;
	}

	zk_slist_free(&slist);
	TEST_ASSERT_NULL(slist);
}

void test_zk_slist_prepend_null_data_to_slist(void)
{
	int number_of_nodes = 100;
	zk_slist_t *slist = NULL;

	for (int i = 0; i < number_of_nodes; i++) {
		slist = zk_slist_prepend(slist, NULL);
		TEST_ASSERT_NOT_NULL(slist);
	}

	for (zk_slist_t *node_i = slist;; node_i = node_i->next) {
		TEST_ASSERT_NOT_NULL(node_i);
		TEST_ASSERT_EQUAL_PTR(node_i->data, NULL);
		printf("%s", node_i->data ? "NOT_NULL" : "NULL");
		if (node_i->next == NULL) {
			printf("\n");
			break;
		}
		printf("->");
	}

	zk_slist_free(&slist);
	TEST_ASSERT_NULL(slist);
}

void test_zk_slist_free_a_null_list_should_just_return(void)
{
	zk_slist_t *slist = NULL;
	zk_slist_free(&slist);
	TEST_ASSERT_NULL(slist);
}

void test_zk_slist_free_full_for_list_of_strings(void)
{
	int number_of_nodes = 100;
	zk_slist_t *slist = NULL;

	for (int i = 0; i < number_of_nodes; i++) {
		slist = zk_slist_append(slist, strdup("a"));
		TEST_ASSERT_NOT_NULL(slist);
	}

	zk_slist_free_full(&slist, free);
	TEST_ASSERT_NULL(slist);
}

void test_zk_slist_free_full_for_a_null_list_should_just_return(void)
{
	zk_slist_t *slist = NULL;
	zk_slist_free_full(&slist, free);
	TEST_ASSERT_NULL(slist);
}

void test_zk_slist_free_full_for_a_null_destructor_should_just_return(void)
{
	zk_slist_t *slist = NULL;
	slist = zk_slist_append(slist, strdup("a"));
	// As destructor argument is NULL, list should not be freed and function should just return
	zk_slist_free_full(&slist, NULL);
	TEST_ASSERT_NOT_NULL(slist);
	TEST_ASSERT_NOT_NULL(slist->data);
	TEST_ASSERT_EQUAL_STRING("a", (char *)slist->data);

	// Now list is freed
	zk_slist_free_full(&slist, free);
	TEST_ASSERT_NULL(slist);
}

void test_zk_slist_concat_lists_of_strings(void)
{
	zk_slist_t *slist_dest = NULL;
	zk_slist_t *slist_src = NULL;

	slist_dest = zk_slist_append(slist_dest, strdup("list_dest_node_1"));
	slist_dest = zk_slist_append(slist_dest, strdup("list_dest_node_2"));
	slist_dest = zk_slist_append(slist_dest, strdup("list_dest_node_3"));

	slist_src = zk_slist_append(slist_src, strdup("list_src_node_1"));
	slist_src = zk_slist_append(slist_src, strdup("list_src_node_2"));
	slist_src = zk_slist_append(slist_src, strdup("list_src_node_3"));

	// concatenates slist_src to the end of slist_dest
	slist_dest = zk_slist_concat(slist_dest, slist_src);

	zk_slist_t *node = slist_dest;
	TEST_ASSERT_EQUAL_STRING("list_dest_node_1", (char *)node->data);
	TEST_ASSERT_NOT_NULL(node->next);
	node = node->next;
	TEST_ASSERT_EQUAL_STRING("list_dest_node_2", (char *)node->data);
	TEST_ASSERT_NOT_NULL(node->next);
	node = node->next;
	TEST_ASSERT_EQUAL_STRING("list_dest_node_3", (char *)node->data);
	TEST_ASSERT_NOT_NULL(node->next);
	node = node->next;
	TEST_ASSERT_EQUAL_STRING("list_src_node_1", (char *)node->data);
	TEST_ASSERT_NOT_NULL(node->next);
	node = node->next;
	TEST_ASSERT_EQUAL_STRING("list_src_node_2", (char *)node->data);
	TEST_ASSERT_NOT_NULL(node->next);
	node = node->next;
	TEST_ASSERT_EQUAL_STRING("list_src_node_3", (char *)node->data);
	TEST_ASSERT_NULL(node->next);

	// In here we verify that the source list is still the same
	node = slist_src;
	TEST_ASSERT_EQUAL_STRING("list_src_node_1", (char *)node->data);
	TEST_ASSERT_NOT_NULL(node->next);
	node = node->next;
	TEST_ASSERT_EQUAL_STRING("list_src_node_2", (char *)node->data);
	TEST_ASSERT_NOT_NULL(node->next);
	node = node->next;
	TEST_ASSERT_EQUAL_STRING("list_src_node_3", (char *)node->data);
	TEST_ASSERT_NULL(node->next);

	// as slist_src was concatenated to the end of slist_dest by freeing slist_dest the slist_src is also freed.
	zk_slist_free_full(&slist_dest, free);
	TEST_ASSERT_NULL(slist_dest);
	// Now slist_src is a dangling pointer
	TEST_ASSERT_NOT_NULL(slist_src);
}

void test_zk_slist_concat_when_destination_list_is_null_should_return_null(void)
{
	int data = 1;
	zk_slist_t *slist_dest = NULL;
	zk_slist_t *slist_src = NULL;

	slist_src = zk_slist_append(slist_src, &data);

	TEST_ASSERT_NULL(zk_slist_concat(slist_dest, slist_src));

	zk_slist_free(&slist_dest);
	zk_slist_free(&slist_src);
}

void test_zk_slist_concat_when_source_list_is_null_should_return_null(void)
{
	int data = 1;
	zk_slist_t *slist_dest = NULL;
	zk_slist_t *slist_src = NULL;

	slist_dest = zk_slist_append(slist_dest, &data);

	TEST_ASSERT_NULL(zk_slist_concat(slist_dest, slist_src));

	zk_slist_free(&slist_dest);
	zk_slist_free(&slist_src);
}

void test_zk_slist_copy_when_source_list_is_null(void)
{
	zk_slist_t *slist_src = NULL;
	TEST_ASSERT_NULL(zk_slist_copy(slist_src));
}

void test_zk_slist_copy_when_source_list_node_data_is_a_pointer_to_data_only_the_pointer_is_copied(void)
{
	zk_slist_t *slist_src = NULL;
	zk_slist_t *slist_copy = NULL;

	struct dummy_node_data *node_1_data = malloc(sizeof(struct dummy_node_data));
	node_1_data->value = 1;
	node_1_data->string = strdup("node_1");

	struct dummy_node_data *node_2_data = malloc(sizeof(struct dummy_node_data));
	node_2_data->value = 2;
	node_2_data->string = strdup("node_2");

	struct dummy_node_data *node_3_data = malloc(sizeof(struct dummy_node_data));
	node_3_data->value = 3;
	node_3_data->string = strdup("node_3");

	slist_src = zk_slist_append(slist_src, node_1_data);
	slist_src = zk_slist_append(slist_src, node_2_data);
	slist_src = zk_slist_append(slist_src, node_3_data);

	// Print slist
	for (zk_slist_t *node_i = slist_src;; node_i = node_i->next) {
		struct dummy_node_data *node_data = node_i->data;
		printf("### Node ###\n");
		printf("Value: %d\n", node_data->value);
		printf("String: %s\n", node_data->string);
		if (node_i->next == NULL) {
			break;
		}
	}

	slist_copy = zk_slist_copy(slist_src);

	zk_slist_t *node_i_src = slist_src;
	zk_slist_t *node_i_cp = slist_copy;
	while (1) {
		struct dummy_node_data *node_data_src = node_i_src->data;
		struct dummy_node_data *node_data_cp = node_i_cp->data;

		// as zk_slist_copy is a shallow copy, data pointer  of both source and data list should pointer to the
		// same memory addresses
		TEST_ASSERT_EQUAL(node_i_src->data, node_i_cp->data);

		// check that node data holds the same data
		TEST_ASSERT_EQUAL(node_data_src->value, node_data_cp->value);
		TEST_ASSERT_EQUAL_STRING(node_data_src->string, node_data_cp->string);

		node_i_cp = node_i_cp->next;
		node_i_src = node_i_src->next;
		if (node_i_cp->next == NULL) {
			break;
		}
	}

	zk_slist_free_full(&slist_src, dummy_node_data_free);
	// data of the nodes were already freed with the above call to zk_slist_free_full
	zk_slist_free(&slist_copy);
}

void test_zk_slist_copy_full_should_return_null_when_source_list_is_null(void)
{
	zk_slist_t *slist_src = NULL;
	TEST_ASSERT_NULL(zk_slist_copy_deep(slist_src, copy_node_data, NULL));
}

void test_zk_slist_copy_full_should_return_null_when_copy_function_is_null(void)
{
	zk_slist_t *slist_src = NULL;
	int data = 1;
	slist_src = zk_slist_prepend(slist_src, &data);
	TEST_ASSERT_NULL(zk_slist_copy_deep(slist_src, NULL, NULL));
	zk_slist_free(&slist_src);
}

void test_zk_slist_copy_deep_should_perform_a_deep_copy_of_source_list_nodes_data(void)
{
	zk_slist_t *slist_src = NULL;
	zk_slist_t *slist_dest = NULL;

	struct dummy_node_data *node_1_data = malloc(sizeof(struct dummy_node_data));
	node_1_data->value = 1;
	node_1_data->string = strdup("node_1");

	struct dummy_node_data *node_2_data = malloc(sizeof(struct dummy_node_data));
	node_2_data->value = 2;
	node_2_data->string = strdup("node_2");

	struct dummy_node_data *node_3_data = malloc(sizeof(struct dummy_node_data));
	node_3_data->value = 3;
	node_3_data->string = strdup("node_3");

	slist_src = zk_slist_append(slist_src, node_1_data);
	slist_src = zk_slist_append(slist_src, node_2_data);
	slist_src = zk_slist_append(slist_src, node_3_data);

	slist_dest = zk_slist_copy_deep(slist_src, copy_node_data, NULL);

	zk_slist_t *node_i_src = slist_src;
	zk_slist_t *node_i_dest = slist_dest;
	while (node_i_src != NULL || node_i_dest != NULL) {
		struct dummy_node_data *node_data_src = node_i_src->data;
		struct dummy_node_data *node_data_dest = node_i_dest->data;

		// as zk_slist_dest_deep performs a deep copy the data pointer of the destination slit should pointer to
		// a different memory address
		TEST_ASSERT_NOT_EQUAL(node_i_src->data, node_i_dest->data);

		// check that node data holds the same data
		TEST_ASSERT_EQUAL(node_data_src->value, node_data_dest->value);
		TEST_ASSERT_EQUAL_STRING(node_data_src->string, node_data_dest->string);

		node_i_dest = node_i_dest->next;
		node_i_src = node_i_src->next;
	}

	zk_slist_free_full(&slist_src, dummy_node_data_free);

	// create a temporary array to check against the values of slit_dest
	struct dummy_node_data *node_data_arr[] = {
		&(struct dummy_node_data){ .value = 1, .string = "node_1" },
		&(struct dummy_node_data){ .value = 2, .string = "node_2" },
		&(struct dummy_node_data){ .value = 3, .string = "node_3" },
	};

	node_i_dest = slist_dest;
	int i = 0;
	while (node_i_dest != NULL) {
		struct dummy_node_data *node_data_dest = node_i_dest->data;

		TEST_ASSERT_NOT_NULL(node_i_dest->data);

		// check that node data holds the same data
		TEST_ASSERT_EQUAL(node_data_arr[i]->value, node_data_dest->value);
		TEST_ASSERT_EQUAL_STRING(node_data_arr[i]->string, node_data_dest->string);

		node_i_dest = node_i_dest->next;
		i++;
	}

	zk_slist_free_full(&slist_dest, dummy_node_data_free);
}

int main(void)
{
	UNITY_BEGIN();

	// tests for zk_slist_append function
	RUN_TEST(test_zk_slist_append_to_empty_list);
	RUN_TEST(test_zk_slist_append_2_items_to_slist);
	RUN_TEST(test_zk_slist_append_3_items_to_slist);
	RUN_TEST(test_zk_slist_append_n_items_to_slist);
	RUN_TEST(test_zk_slist_append_null_data_to_slist);

	// test for zk_slist_concat
	RUN_TEST(test_zk_slist_concat_lists_of_strings);
	RUN_TEST(test_zk_slist_concat_when_destination_list_is_null_should_return_null);
	RUN_TEST(test_zk_slist_concat_when_source_list_is_null_should_return_null);

	// test for zk_slist_copy
	RUN_TEST(test_zk_slist_copy_when_source_list_is_null);
	RUN_TEST(test_zk_slist_copy_when_source_list_node_data_is_a_pointer_to_data_only_the_pointer_is_copied);

	// test for zk_slist_copy_deep
	RUN_TEST(test_zk_slist_copy_full_should_return_null_when_source_list_is_null);
	RUN_TEST(test_zk_slist_copy_full_should_return_null_when_copy_function_is_null);
	RUN_TEST(test_zk_slist_copy_deep_should_perform_a_deep_copy_of_source_list_nodes_data);

	// test for zk_slist_free
	RUN_TEST(test_zk_slist_free_a_null_list_should_just_return);

	// tests for zk_slist_free_full function
	RUN_TEST(test_zk_slist_free_full_for_list_of_strings);
	RUN_TEST(test_zk_slist_free_full_for_a_null_list_should_just_return);
	RUN_TEST(test_zk_slist_free_full_for_a_null_destructor_should_just_return);

	// tests for zk_slist_prepend function
	RUN_TEST(test_zk_slist_prepend_n_items_to_slist);
	RUN_TEST(test_zk_slist_prepend_null_data_to_slist);

	return UNITY_END();
}