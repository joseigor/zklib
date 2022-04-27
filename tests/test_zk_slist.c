#include <stdlib.h>
#include <string.h>

#include "unity.h"
#include "zk/zklib.h"
#include "zk_common/zk_utils.h"

struct dummy_node_data {
	int value;
	char *string;
};

static int slist_compare_data_custom(const void *const node_data, const void *const user_data)
{
	const int a = *((int *)node_data);
	const int b = *((int *)user_data);

	if (a < b) {
		return -1;
	}

	if (a > b) {
		return 1;
	}

	return 0;
}

static void dummy_node_data_free(void *data)
{
	struct dummy_node_data *node_data = data;
	free(node_data->string);
	free(node_data);
}

static void dummy_node_data_free_foreach(void *data, ZK_GNUC_UNUSED void *user_data)
{
	struct dummy_node_data *node_data = data;
	free(node_data->string);
	free(node_data);
}

static void *copy_node_data(const void *const data, ZK_GNUC_UNUSED void *user_data)
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
	TEST_ASSERT_EQUAL(*((int *)slist->data), data);
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
	TEST_ASSERT_EQUAL(*((int *)slist->data), node1_data);
	TEST_ASSERT_NULL(slist->next);

	slist = zk_slist_append(slist, &node2_data);

	// first we check that slist still points to the head of the list
	TEST_ASSERT_NOT_NULL(slist);
	TEST_ASSERT_EQUAL_PTR(slist->data, &node1_data);
	TEST_ASSERT_EQUAL(*((int *)slist->data), node1_data);
	TEST_ASSERT_NOT_NULL(slist->next);

	// now we check the content of the second node of the list
	node_2 = slist->next;
	TEST_ASSERT_NOT_NULL(node_2);
	TEST_ASSERT_EQUAL_PTR(node_2->data, &node2_data);
	TEST_ASSERT_EQUAL(*((int *)node_2->data), node2_data);
	TEST_ASSERT_NULL(node_2->next);

	slist = zk_slist_append(slist, &node3_data);

	// first we check that slist still points to the head of the list
	TEST_ASSERT_NOT_NULL(slist);
	TEST_ASSERT_EQUAL_PTR(slist->data, &node1_data);
	TEST_ASSERT_EQUAL(*((int *)slist->data), node1_data);
	TEST_ASSERT_NOT_NULL(slist->next);

	// now we check the content of the second node of the list
	node_2 = slist->next;
	TEST_ASSERT_NOT_NULL(node_2);
	TEST_ASSERT_EQUAL_PTR(node_2->data, &node2_data);
	TEST_ASSERT_EQUAL(*((int *)node_2->data), node2_data);
	TEST_ASSERT_NOT_NULL(node_2->next);

	// now we check the content of the third node of the list

	node_3 = node_2->next;
	TEST_ASSERT_NOT_NULL(node_3);
	TEST_ASSERT_EQUAL_PTR(node_3->data, &node3_data);
	TEST_ASSERT_EQUAL(*((int *)node_3->data), node3_data);
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
		TEST_ASSERT_EQUAL(*((int *)node_i->data), nodes_data[node_idx]);
		if (node_i->next == NULL) {
			break;
		}
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
		if (node_i->next == NULL) {
			break;
		}
	}

	zk_slist_free(&slist);
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

	// as slist_src was concatenated to the end of slist_dest by freeing slist_dest the slist_src is also
	// freed.
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

void test_zk_slist_concat_when_source_list_is_null_should_return_destination_list(void)
{
	int data = 1;
	zk_slist_t *slist_dest = NULL;

	slist_dest = zk_slist_append(slist_dest, &data);

	slist_dest = zk_slist_concat(slist_dest, NULL);

	TEST_ASSERT_NOT_NULL(slist_dest);
	TEST_ASSERT_EQUAL(&data, slist_dest->data);
	TEST_ASSERT_EQUAL(data, *((int *)(slist_dest->data)));

	zk_slist_free(&slist_dest);
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

	slist_copy = zk_slist_copy(slist_src);

	zk_slist_t *node_i_src = slist_src;
	zk_slist_t *node_i_cp = slist_copy;
	while (1) {
		struct dummy_node_data *node_data_src = node_i_src->data;
		struct dummy_node_data *node_data_cp = node_i_cp->data;

		// as zk_slist_copy is a shallow copy, data pointer  of both source and data list should pointer
		// to the same memory addresses
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

		// as zk_slist_dest_deep performs a deep copy the data pointer of the destination slit should
		// pointer to a different memory address
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

void test_zk_slist_delete_node_when_list_is_null(void)
{
	zk_slist_t *slist = NULL;
	zk_slist_t *slist_node = NULL;
	slist_node = zk_slist_append(slist_node, NULL);

	TEST_ASSERT_NOT_NULL(slist_node);
	TEST_ASSERT_NULL(zk_slist_delete_node(slist, slist_node, NULL));

	zk_slist_free(&slist_node);
}

void test_zk_slist_delete_node_when_node_is_null(void)
{
	zk_slist_t *slist = NULL;
	zk_slist_t *slist_node = NULL;
	slist = zk_slist_append(slist, NULL);

	TEST_ASSERT_NOT_NULL(slist);
	TEST_ASSERT_EQUAL(slist, zk_slist_delete_node(slist, slist_node, NULL));

	zk_slist_free(&slist);
}

void test_zk_slist_delete_node_when_list_has_only_one_node(void)
{
	zk_slist_t *slist = NULL;
	zk_slist_t *slist_node = NULL;
	slist = zk_slist_append(slist, NULL);

	slist_node = slist;

	// slist has only one link that once deletec slist should be null
	TEST_ASSERT_NOT_NULL(slist);
	TEST_ASSERT_NOT_NULL(slist_node);
	TEST_ASSERT_NULL(zk_slist_delete_node(slist, slist_node, NULL));

	// In this point slist and slist_node are dangling pointers
	TEST_ASSERT_NOT_NULL(slist);
	TEST_ASSERT_NOT_NULL(slist_node);

	// There is no need to free slist nor slist_node the only lint slist had was deleted
}

void test_zk_slist_delete_first_node_when_list_has_mutiple_nodes(void)
{
	zk_slist_t *slist = NULL;
	zk_slist_t *slist_node = NULL;

	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;

	slist = zk_slist_append(slist, &node_1_data);
	slist = zk_slist_append(slist, &node_2_data);
	slist = zk_slist_append(slist, &node_3_data);

	// slist_node points to first node
	slist_node = slist;

	TEST_ASSERT_NOT_NULL(slist);
	TEST_ASSERT_NOT_NULL(slist_node);
	TEST_ASSERT_EQUAL(node_1_data, *(int *)(slist_node->data));

	slist = zk_slist_delete_node(slist, slist_node, NULL);

	TEST_ASSERT_NOT_NULL(slist);
	TEST_ASSERT_EQUAL(node_2_data, *(int *)(slist->data));
	TEST_ASSERT_EQUAL(node_3_data, *(int *)(slist->next->data));
	TEST_ASSERT_NULL(slist->next->next);

	// slist_node is dangling pointers no need to free it
	TEST_ASSERT_NOT_NULL(slist_node);

	zk_slist_free(&slist);
}

void test_zk_slist_delete_last_node(void)
{
	zk_slist_t *slist = NULL;
	zk_slist_t *slist_node = NULL;

	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;

	slist = zk_slist_append(slist, &node_1_data);
	slist = zk_slist_append(slist, &node_2_data);
	slist = zk_slist_append(slist, &node_3_data);

	// slist_node points to third/last node
	slist_node = slist->next->next;

	// slist has only one link that once deletec slist should be null
	TEST_ASSERT_NOT_NULL(slist);
	TEST_ASSERT_NOT_NULL(slist_node);
	TEST_ASSERT_EQUAL(node_3_data, *(int *)(slist_node->data));

	slist = zk_slist_delete_node(slist, slist_node, NULL);

	TEST_ASSERT_NOT_NULL(slist);
	TEST_ASSERT_EQUAL(node_1_data, *(int *)(slist->data));
	TEST_ASSERT_EQUAL(node_2_data, *(int *)(slist->next->data));
	TEST_ASSERT_NULL(slist->next->next);

	// slist_node is dangling pointers no need to free it
	TEST_ASSERT_NOT_NULL(slist_node);

	zk_slist_free(&slist);
}

void test_zk_slist_delete_node_when_list_has_multiple_nodes(void)
{
	zk_slist_t *slist = NULL;
	zk_slist_t *slist_node = NULL;

	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;

	slist = zk_slist_append(slist, &node_1_data);
	slist = zk_slist_append(slist, &node_2_data);
	slist = zk_slist_append(slist, &node_3_data);

	// slist_node points to second node
	slist_node = slist->next;

	// slist has only one link that once deletec slist should be null
	TEST_ASSERT_NOT_NULL(slist);
	TEST_ASSERT_NOT_NULL(slist_node);
	TEST_ASSERT_EQUAL(node_2_data, *(int *)(slist_node->data));

	slist = zk_slist_delete_node(slist, slist_node, NULL);

	TEST_ASSERT_NOT_NULL(slist);
	TEST_ASSERT_EQUAL(node_1_data, *(int *)(slist->data));
	TEST_ASSERT_EQUAL(node_3_data, *(int *)(slist->next->data));
	TEST_ASSERT_NULL(slist->next->next);

	// slist_node is dangling pointers no need to free it
	TEST_ASSERT_NOT_NULL(slist_node);

	zk_slist_free(&slist);
}

void test_zk_slist_delete_node_and_use_external_function_to_free_node_data(void)
{
	zk_slist_t *slist = NULL;
	zk_slist_t *slist_node = NULL;

	struct dummy_node_data *node_1_data = malloc(sizeof(struct dummy_node_data));
	node_1_data->value = 1;
	node_1_data->string = strdup("node_1");

	struct dummy_node_data *node_2_data = malloc(sizeof(struct dummy_node_data));
	node_2_data->value = 2;
	node_2_data->string = strdup("node_2");

	struct dummy_node_data *node_3_data = malloc(sizeof(struct dummy_node_data));
	node_3_data->value = 3;
	node_3_data->string = strdup("node_3");

	slist = zk_slist_append(slist, node_1_data);
	slist = zk_slist_append(slist, node_2_data);
	slist = zk_slist_append(slist, node_3_data);

	// slist_node points to second node
	slist_node = slist->next;

	// slist has only one link that once deletec slist should be null
	TEST_ASSERT_NOT_NULL(slist);
	TEST_ASSERT_NOT_NULL(slist_node);

	// checks that node to be deleted points to correct node and has coreect data
	TEST_ASSERT_EQUAL(node_2_data, (struct dummy_node_data *)(slist_node->data));
	TEST_ASSERT_EQUAL(node_2_data->value, ((struct dummy_node_data *)(slist_node->data))->value);
	TEST_ASSERT_EQUAL(node_2_data->string, ((struct dummy_node_data *)(slist_node->data))->string);
	TEST_ASSERT_EQUAL(*(node_2_data->string), *(((struct dummy_node_data *)(slist_node->data))->string));

	slist = zk_slist_delete_node(slist, slist_node, dummy_node_data_free);

	// checks that list has correct number of node and data after the node was removedS
	TEST_ASSERT_NOT_NULL(slist);

	TEST_ASSERT_EQUAL(node_1_data, (struct dummy_node_data *)(slist->data));
	TEST_ASSERT_EQUAL(node_1_data->value, ((struct dummy_node_data *)(slist->data))->value);
	TEST_ASSERT_EQUAL(node_1_data->string, ((struct dummy_node_data *)(slist->data))->string);
	TEST_ASSERT_EQUAL(*(node_1_data->string), *(((struct dummy_node_data *)(slist->data))->string));

	TEST_ASSERT_EQUAL(node_3_data, (struct dummy_node_data *)(slist->next->data));
	TEST_ASSERT_EQUAL(node_3_data->value, ((struct dummy_node_data *)(slist->next->data))->value);
	TEST_ASSERT_EQUAL(node_3_data->string, ((struct dummy_node_data *)(slist->next->data))->string);
	TEST_ASSERT_EQUAL(*(node_3_data->string), *(((struct dummy_node_data *)(slist->next->data))->string));

	TEST_ASSERT_NULL(slist->next->next);

	// slist_node is dangling pointers no need to free it
	TEST_ASSERT_NOT_NULL(slist_node);

	zk_slist_free_full(&slist, dummy_node_data_free);
}

void test_zk_slist_delete_node_when_node_is_not_in_the_list(void)
{
	zk_slist_t *slist = NULL;
	zk_slist_t *slist_node = NULL;

	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;

	slist = zk_slist_append(slist, &node_1_data);
	slist = zk_slist_append(slist, &node_2_data);
	slist = zk_slist_append(slist, &node_3_data);

	// node has the same data of the third node of slist, but still this node is not part of slist
	slist_node = zk_slist_append(slist_node, &node_3_data);

	// slist has only one link that once deletec slist should be null
	TEST_ASSERT_NOT_NULL(slist);
	TEST_ASSERT_NOT_NULL(slist_node);
	TEST_ASSERT_EQUAL(node_3_data, *(int *)(slist_node->data));

	slist = zk_slist_delete_node(slist, slist_node, NULL);

	TEST_ASSERT_NOT_NULL(slist);
	TEST_ASSERT_EQUAL(node_1_data, *(int *)(slist->data));
	TEST_ASSERT_EQUAL(node_2_data, *(int *)(slist->next->data));
	TEST_ASSERT_EQUAL(node_3_data, *(int *)(slist->next->next->data));
	TEST_ASSERT_NULL(slist->next->next->next);

	TEST_ASSERT_NOT_NULL(slist_node);
	TEST_ASSERT_EQUAL(node_3_data, *(int *)(slist_node->data));
	TEST_ASSERT_NULL(slist_node->next);

	zk_slist_free(&slist);
	zk_slist_free(&slist_node);
}

void test_zk_slist_find_by_data(void)
{
	zk_slist_t *slist = NULL;
	zk_slist_t *slist_found = NULL;

	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;
	int node_4_data = 4;
	int node_5_data = 5;
	int node_6_data = 6;
	int node_7_data = 7;

	slist = zk_slist_append(slist, &node_1_data);
	slist = zk_slist_append(slist, &node_2_data);
	slist = zk_slist_append(slist, &node_3_data);
	slist = zk_slist_append(slist, &node_4_data);
	slist = zk_slist_append(slist, &node_5_data);
	slist = zk_slist_append(slist, &node_6_data);
	slist = zk_slist_append(slist, &node_7_data);

	slist_found = zk_slist_find_by_data(slist, &node_5_data);

	TEST_ASSERT_NOT_NULL(slist_found);
	TEST_ASSERT_EQUAL(node_5_data, *((int *)(slist_found->data)));
	TEST_ASSERT_EQUAL(node_6_data, *((int *)(slist_found->next->data)));
	TEST_ASSERT_EQUAL(node_7_data, *((int *)(slist_found->next->next->data)));
	TEST_ASSERT_NULL(slist_found->next->next->next);

	// There is no nedd to free slist_found as it is part slist
	zk_slist_free(&slist);
}

void test_zk_slist_find_by_data_custom_when_comparison_func_is_null(void)
{
	zk_slist_t *slist = NULL;
	zk_slist_t *slist_found = NULL;

	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;
	int node_4_data = 4;
	int node_5_data = 5;
	int node_6_data = 6;
	int node_7_data = 7;

	slist = zk_slist_append(slist, &node_1_data);
	slist = zk_slist_append(slist, &node_2_data);
	slist = zk_slist_append(slist, &node_3_data);
	slist = zk_slist_append(slist, &node_4_data);
	slist = zk_slist_append(slist, &node_5_data);
	slist = zk_slist_append(slist, &node_6_data);
	slist = zk_slist_append(slist, &node_7_data);

	slist_found = zk_slist_find_by_data_custom(slist, &node_5_data, NULL);

	TEST_ASSERT_NULL(slist_found);

	// There is no nedd to free slist_found as it is part slist
	zk_slist_free(&slist);
}

void test_zk_slist_find_by_data_custom(void)
{
	zk_slist_t *slist = NULL;
	zk_slist_t *slist_found = NULL;

	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;
	int node_4_data = 4;
	int node_5_data = 5;
	int node_6_data = 6;
	int node_7_data = 7;

	slist = zk_slist_append(slist, &node_1_data);
	slist = zk_slist_append(slist, &node_2_data);
	slist = zk_slist_append(slist, &node_3_data);
	slist = zk_slist_append(slist, &node_4_data);
	slist = zk_slist_append(slist, &node_5_data);
	slist = zk_slist_append(slist, &node_6_data);
	slist = zk_slist_append(slist, &node_7_data);

	slist_found = zk_slist_find_by_data_custom(slist, &node_5_data, slist_compare_data_custom);

	TEST_ASSERT_NOT_NULL(slist_found);
	TEST_ASSERT_EQUAL(node_5_data, *((int *)(slist_found->data)));
	TEST_ASSERT_EQUAL(node_6_data, *((int *)(slist_found->next->data)));
	TEST_ASSERT_EQUAL(node_7_data, *((int *)(slist_found->next->next->data)));
	TEST_ASSERT_NULL(slist_found->next->next->next);

	// There is no nedd to free slist_found as it is part slist
	zk_slist_free(&slist);
}

void test_zk_slist_foreach_when_func_is_null(void)
{
	struct dummy_node_data *node_1_data = malloc(sizeof(struct dummy_node_data));
	node_1_data->value = 1;
	node_1_data->string = strdup("node_1");
	zk_slist_t *slist = NULL;

	slist = zk_slist_append(slist, node_1_data);
	TEST_ASSERT_NOT_NULL(slist);

	// Func is null, it will just return
	zk_slist_foreach(slist, NULL, NULL);

	zk_slist_free_full(&slist, dummy_node_data_free);
	TEST_ASSERT_NULL(slist);
}

void test_zk_slist_foreach_when_list_is_null(void)
{
	zk_slist_t *slist = NULL;

	// list is null, it will just return
	zk_slist_foreach(slist, dummy_node_data_free_foreach, NULL);

	TEST_ASSERT_NULL(slist);
}

void test_zk_slist_foreach_when_func_is_not_null(void)
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

	zk_slist_t *slist = NULL;
	slist = zk_slist_append(slist, node_1_data);
	slist = zk_slist_append(slist, node_2_data);
	slist = zk_slist_append(slist, node_3_data);

	// Use foreach to clean each node data
	zk_slist_foreach(slist, dummy_node_data_free_foreach, NULL);

	// As nodes data were freed, we just need to call zk_slist_free.
	zk_slist_free(&slist);
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

void test_zk_slist_get_index_when_list_is_null(void)
{
	zk_slist_t *slist = NULL;
	int node_1_data = 1;

	TEST_ASSERT_EQUAL(-1, zk_slist_get_index(slist, &node_1_data));
}

void test_zk_slist_get_index_when_data_is_null(void)
{
	zk_slist_t *slist = NULL;
	int node_1_data = 1;
	// int node_2_data = 2;
	int node_3_data = 3;

	slist = zk_slist_append(slist, &node_1_data);
	slist = zk_slist_append(slist, NULL);
	slist = zk_slist_append(slist, &node_3_data);

	TEST_ASSERT_EQUAL(1, zk_slist_get_index(slist, NULL));

	zk_slist_free(&slist);
}

void test_zk_slist_get_index_when_data_is_on_list(void)
{
	zk_slist_t *slist = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;

	slist = zk_slist_append(slist, &node_1_data);
	slist = zk_slist_append(slist, &node_2_data);
	slist = zk_slist_append(slist, &node_3_data);

	TEST_ASSERT_EQUAL(2, zk_slist_get_index(slist, &node_3_data));

	zk_slist_free(&slist);
}

void test_zk_slist_get_index_when_data_is_not_on_list(void)
{
	zk_slist_t *slist = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;
	int node_4_data = 3;

	slist = zk_slist_append(slist, &node_1_data);
	slist = zk_slist_append(slist, &node_2_data);
	slist = zk_slist_append(slist, &node_3_data);

	TEST_ASSERT_EQUAL(-1, zk_slist_get_index(slist, &node_4_data));

	zk_slist_free(&slist);
}

void test_zk_slist_insert_when_list_is_null(void)
{
	zk_slist_t *slist = NULL;
	int node_1_data = 1;

	slist = zk_slist_insert(slist, &node_1_data, 0);

	TEST_ASSERT_NOT_NULL(slist);
	TEST_ASSERT_EQUAL(&node_1_data, slist->data);
	TEST_ASSERT_EQUAL(node_1_data, *((int *)(slist->data)));

	zk_slist_free(&slist);
}

void test_zk_slist_insert_when_data_is_null(void)
{
	zk_slist_t *slist = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;

	slist = zk_slist_append(slist, &node_1_data);
	slist = zk_slist_append(slist, &node_2_data);
	slist = zk_slist_append(slist, &node_3_data);

	// insert a node in postion 1 of the list
	slist = zk_slist_insert(slist, NULL, 1);

	zk_slist_t *node = zk_slist_find_by_data(slist, NULL);

	TEST_ASSERT_EQUAL(1, zk_slist_get_index(slist, NULL));
	TEST_ASSERT_NOT_NULL(node);
	TEST_ASSERT_EQUAL(NULL, node->data);

	zk_slist_free(&slist);
}

void test_zk_slist_insert_when_position_is_negative(void)
{
	zk_slist_t *slist = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;
	int node_4_data = 4;

	slist = zk_slist_append(slist, &node_1_data);
	slist = zk_slist_append(slist, &node_2_data);
	slist = zk_slist_append(slist, &node_3_data);

	// as position is negative, new node is preprended to the list
	slist = zk_slist_insert(slist, &node_4_data, -1);

	TEST_ASSERT_EQUAL(0, zk_slist_get_index(slist, &node_4_data));
	TEST_ASSERT_NOT_NULL(slist);
	TEST_ASSERT_EQUAL(&node_4_data, slist->data);
	TEST_ASSERT_EQUAL(node_4_data, *((int *)(slist->data)));

	zk_slist_free(&slist);
}

void test_zk_slist_insert_when_position_is_greater_than_list_size(void)
{
	zk_slist_t *slist = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;
	int node_4_data = 4;

	slist = zk_slist_append(slist, &node_1_data);
	slist = zk_slist_append(slist, &node_2_data);
	slist = zk_slist_append(slist, &node_3_data);

	// as position is greater than list size, new node is appended to the list
	slist = zk_slist_insert(slist, &node_4_data, 10);

	zk_slist_t *node = zk_slist_last(slist);

	TEST_ASSERT_EQUAL(3, zk_slist_get_index(slist, &node_4_data));
	TEST_ASSERT_NOT_NULL(node);
	TEST_ASSERT_EQUAL(&node_4_data, node->data);
	TEST_ASSERT_EQUAL(node_4_data, *((int *)(node->data)));

	zk_slist_free(&slist);
}

void test_zk_slist_insert_when_position_is_inside_list_range(void)
{
	zk_slist_t *slist = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;
	int node_4_data = 4;

	slist = zk_slist_append(slist, &node_1_data);
	slist = zk_slist_append(slist, &node_2_data);
	slist = zk_slist_append(slist, &node_4_data);

	// insert node in position 2
	slist = zk_slist_insert(slist, &node_3_data, 2);

	zk_slist_t *node = zk_slist_find_by_data(slist, &node_3_data);
	TEST_ASSERT_EQUAL(2, zk_slist_get_index(slist, &node_3_data));
	TEST_ASSERT_NOT_NULL(node);
	TEST_ASSERT_EQUAL(&node_3_data, node->data);
	TEST_ASSERT_EQUAL(node_3_data, *((int *)(node->data)));

	zk_slist_free(&slist);
}

void test_zk_slist_last_when_list_is_null(void)
{
	TEST_ASSERT_NULL(zk_slist_last(NULL));
}

void test_zk_slist_last_when_list_has_only_one_node(void)
{
	zk_slist_t *slist = NULL;
	zk_slist_t *slist_last = NULL;
	int node_1_data = 1;

	slist = zk_slist_append(slist, &node_1_data);
	slist_last = zk_slist_last(slist);

	TEST_ASSERT_NOT_NULL(slist_last);
	TEST_ASSERT_EQUAL(&node_1_data, slist->data);
	TEST_ASSERT_EQUAL(node_1_data, *((int *)(slist->data)));
	TEST_ASSERT_NULL(slist->next);

	zk_slist_free(&slist);
}

void test_zk_slist_last_when_list_has_more_than_one_node(void)
{
	zk_slist_t *slist = NULL;
	zk_slist_t *slist_last = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;

	slist = zk_slist_append(slist, &node_1_data);
	slist = zk_slist_append(slist, &node_2_data);
	slist = zk_slist_append(slist, &node_3_data);

	slist_last = zk_slist_last(slist);

	TEST_ASSERT_NOT_NULL(slist_last);
	TEST_ASSERT_EQUAL_PTR(&node_3_data, slist_last->data);
	TEST_ASSERT_EQUAL(node_3_data, *((int *)(slist_last->data)));
	TEST_ASSERT_NULL(slist_last->next);

	zk_slist_free(&slist);
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
		if (node_i->next == NULL) {
			break;
		}
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
		if (node_i->next == NULL) {
			break;
		}
	}

	zk_slist_free(&slist);
	TEST_ASSERT_NULL(slist);
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
	RUN_TEST(test_zk_slist_concat_when_source_list_is_null_should_return_destination_list);

	// test for zk_slist_copy
	RUN_TEST(test_zk_slist_copy_when_source_list_is_null);
	RUN_TEST(test_zk_slist_copy_when_source_list_node_data_is_a_pointer_to_data_only_the_pointer_is_copied);

	// test for zk_slist_copy_deep
	RUN_TEST(test_zk_slist_copy_full_should_return_null_when_source_list_is_null);
	RUN_TEST(test_zk_slist_copy_full_should_return_null_when_copy_function_is_null);
	RUN_TEST(test_zk_slist_copy_deep_should_perform_a_deep_copy_of_source_list_nodes_data);

	// test zk_slist_delete_node
	RUN_TEST(test_zk_slist_delete_node_when_list_is_null);
	RUN_TEST(test_zk_slist_delete_node_when_node_is_null);
	RUN_TEST(test_zk_slist_delete_node_when_list_has_only_one_node);
	RUN_TEST(test_zk_slist_delete_first_node_when_list_has_mutiple_nodes);
	RUN_TEST(test_zk_slist_delete_last_node);
	RUN_TEST(test_zk_slist_delete_node_when_list_has_multiple_nodes);
	RUN_TEST(test_zk_slist_delete_node_and_use_external_function_to_free_node_data);
	RUN_TEST(test_zk_slist_delete_node_when_node_is_not_in_the_list);

	// test zk_slist_find_by_data
	RUN_TEST(test_zk_slist_find_by_data);

	// test zk_slist_find_by_data_custom
	RUN_TEST(test_zk_slist_find_by_data_custom_when_comparison_func_is_null);
	RUN_TEST(test_zk_slist_find_by_data_custom);

	// test zk_slist_foreach
	RUN_TEST(test_zk_slist_foreach_when_func_is_null);
	RUN_TEST(test_zk_slist_foreach_when_list_is_null);
	RUN_TEST(test_zk_slist_foreach_when_func_is_not_null);

	// tes zk_slist_free
	RUN_TEST(test_zk_slist_free_a_null_list_should_just_return);

	// test zk_slist_free_full function
	RUN_TEST(test_zk_slist_free_full_for_list_of_strings);
	RUN_TEST(test_zk_slist_free_full_for_a_null_list_should_just_return);
	RUN_TEST(test_zk_slist_free_full_for_a_null_destructor_should_just_return);

	// test zk_slist_get_index
	RUN_TEST(test_zk_slist_get_index_when_list_is_null);
	RUN_TEST(test_zk_slist_get_index_when_data_is_null);
	RUN_TEST(test_zk_slist_get_index_when_data_is_on_list);
	RUN_TEST(test_zk_slist_get_index_when_data_is_not_on_list);

	// test zk_slist_insert
	RUN_TEST(test_zk_slist_insert_when_list_is_null);
	RUN_TEST(test_zk_slist_insert_when_data_is_null);
	RUN_TEST(test_zk_slist_insert_when_position_is_negative);
	RUN_TEST(test_zk_slist_insert_when_position_is_greater_than_list_size);
	RUN_TEST(test_zk_slist_insert_when_position_is_inside_list_range);

	// test zk_slist_last
	RUN_TEST(test_zk_slist_last_when_list_is_null);
	RUN_TEST(test_zk_slist_last_when_list_has_only_one_node);
	RUN_TEST(test_zk_slist_last_when_list_has_more_than_one_node);

	// tests for zk_slist_prepend function
	RUN_TEST(test_zk_slist_prepend_n_items_to_slist);
	RUN_TEST(test_zk_slist_prepend_null_data_to_slist);

	return UNITY_END();
}
