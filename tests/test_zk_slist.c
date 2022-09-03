#include <stdlib.h>
#include <string.h>

#include "common/test_common.h"
#include "unity.h"
#include "zk/zklib.h"
#include "zk_common/zk_common.h"

static int slist_compare_data_custom(const void *const node_data, const void *const user_data)
{
	if (node_data == user_data)
		return 0;
	if (node_data == NULL && user_data != NULL)
		return -1;
	if (node_data != NULL && user_data == NULL)
		return 1;

	// neither node_data nor user_data are NULL
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

static void *copy_node_data(const void *const data, void *user_data)
{
	ZK_UNUSED(user_data);
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

void test_zk_slist_concat_lists_of_strings(void)
{
	zk_slist *slist_dest = NULL;
	zk_slist *slist_src = NULL;

	slist_dest = zk_push_back(slist_dest, strdup("list_dest_node_1"));
	slist_dest = zk_push_back(slist_dest, strdup("list_dest_node_2"));
	slist_dest = zk_push_back(slist_dest, strdup("list_dest_node_3"));

	slist_src = zk_push_back(slist_src, strdup("list_src_node_1"));
	slist_src = zk_push_back(slist_src, strdup("list_src_node_2"));
	slist_src = zk_push_back(slist_src, strdup("list_src_node_3"));

	// concatenates slist_src to the end of slist_dest
	slist_dest = zk_slist_concat(slist_dest, slist_src);

	zk_slist *node = slist_dest;
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
	zk_free(&slist_dest, free);
	TEST_ASSERT_NULL(slist_dest);
	// Now slist_src is a dangling pointer
	TEST_ASSERT_NOT_NULL(slist_src);
}

void test_zk_slist_concat_when_destination_list_is_null_should_return_null(void)
{
	int data = 1;
	zk_slist *slist_dest = NULL;
	zk_slist *slist_src = NULL;

	slist_src = zk_push_back(slist_src, &data);

	TEST_ASSERT_NULL(zk_slist_concat(slist_dest, slist_src));

	zk_free(&slist_dest, NULL);
	zk_free(&slist_src, NULL);
}

void test_zk_slist_concat_when_source_list_is_null_should_return_destination_list(void)
{
	int data = 1;
	zk_slist *slist_dest = NULL;

	slist_dest = zk_push_back(slist_dest, &data);

	slist_dest = zk_slist_concat(slist_dest, NULL);

	TEST_ASSERT_NOT_NULL(slist_dest);
	TEST_ASSERT_EQUAL(&data, slist_dest->data);
	TEST_ASSERT_EQUAL(data, *((int *)(slist_dest->data)));

	zk_free(&slist_dest, NULL);
}

void test_zk_slist_copy_when_source_list_is_null(void)
{
	zk_slist *slist_src = NULL;
	TEST_ASSERT_NULL(zk_slist_copy(slist_src));
}

void test_zk_slist_copy_when_source_list_node_data_is_a_pointer_to_data_only_the_pointer_is_copied(void)
{
	zk_slist *slist_src = NULL;
	zk_slist *slist_copy = NULL;

	struct dummy_node_data *node_1_data = malloc(sizeof(struct dummy_node_data));
	node_1_data->value = 1;
	node_1_data->string = strdup("node_1");

	struct dummy_node_data *node_2_data = malloc(sizeof(struct dummy_node_data));
	node_2_data->value = 2;
	node_2_data->string = strdup("node_2");

	struct dummy_node_data *node_3_data = malloc(sizeof(struct dummy_node_data));
	node_3_data->value = 3;
	node_3_data->string = strdup("node_3");

	slist_src = zk_push_back(slist_src, node_1_data);
	slist_src = zk_push_back(slist_src, node_2_data);
	slist_src = zk_push_back(slist_src, node_3_data);

	slist_copy = zk_slist_copy(slist_src);

	zk_slist *node_i_src = slist_src;
	zk_slist *node_i_cp = slist_copy;
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

	zk_free(&slist_src, dummy_node_data_free);
	// data of the nodes were already freed with the above call to zk_free
	zk_free(&slist_copy, NULL);
}

void test_zk_slist_copy_full_should_return_null_when_source_list_is_null(void)
{
	zk_slist *slist_src = NULL;
	TEST_ASSERT_NULL(zk_slist_copy_deep(slist_src, copy_node_data, NULL));
}

void test_zk_slist_copy_full_should_return_null_when_copy_function_is_null(void)
{
	zk_slist *slist_src = NULL;
	int data = 1;
	slist_src = zk_push_front(slist_src, &data);
	TEST_ASSERT_NULL(zk_slist_copy_deep(slist_src, NULL, NULL));
	zk_free(&slist_src, NULL);
}

void test_zk_slist_copy_deep_should_perform_a_deep_copy_of_source_list_nodes_data(void)
{
	zk_slist *slist_src = NULL;
	zk_slist *slist_dest = NULL;

	struct dummy_node_data *node_1_data = malloc(sizeof(struct dummy_node_data));
	node_1_data->value = 1;
	node_1_data->string = strdup("node_1");

	struct dummy_node_data *node_2_data = malloc(sizeof(struct dummy_node_data));
	node_2_data->value = 2;
	node_2_data->string = strdup("node_2");

	struct dummy_node_data *node_3_data = malloc(sizeof(struct dummy_node_data));
	node_3_data->value = 3;
	node_3_data->string = strdup("node_3");

	slist_src = zk_push_back(slist_src, node_1_data);
	slist_src = zk_push_back(slist_src, node_2_data);
	slist_src = zk_push_back(slist_src, node_3_data);

	slist_dest = zk_slist_copy_deep(slist_src, copy_node_data, NULL);

	zk_slist *node_i_src = slist_src;
	zk_slist *node_i_dest = slist_dest;
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

	zk_free(&slist_src, dummy_node_data_free);

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

	zk_free(&slist_dest, dummy_node_data_free);
}

void test_zk_slist_delete_node_when_list_is_null(void)
{
	zk_slist *slist = NULL;
	zk_slist *slist_node = NULL;
	slist_node = zk_push_back(slist_node, NULL);

	TEST_ASSERT_NOT_NULL(slist_node);
	TEST_ASSERT_NULL(zk_slist_delete_node(slist, slist_node, NULL));

	zk_free(&slist_node, NULL);
}

void test_zk_slist_delete_node_when_node_is_null(void)
{
	zk_slist *slist = NULL;
	zk_slist *slist_node = NULL;
	slist = zk_push_back(slist, NULL);

	TEST_ASSERT_NOT_NULL(slist);
	TEST_ASSERT_EQUAL(slist, zk_slist_delete_node(slist, slist_node, NULL));

	zk_free(&slist, NULL);
}

void test_zk_slist_delete_node_when_list_has_only_one_node(void)
{
	zk_slist *slist = NULL;
	zk_slist *slist_node = NULL;
	slist = zk_push_back(slist, NULL);

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
	zk_slist *slist = NULL;
	zk_slist *slist_node = NULL;

	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;

	slist = zk_push_back(slist, &node_1_data);
	slist = zk_push_back(slist, &node_2_data);
	slist = zk_push_back(slist, &node_3_data);

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

	zk_free(&slist, NULL);
}

void test_zk_slist_delete_last_node(void)
{
	zk_slist *slist = NULL;
	zk_slist *slist_node = NULL;

	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;

	slist = zk_push_back(slist, &node_1_data);
	slist = zk_push_back(slist, &node_2_data);
	slist = zk_push_back(slist, &node_3_data);

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

	zk_free(&slist, NULL);
}

void test_zk_slist_delete_node_when_list_has_multiple_nodes(void)
{
	zk_slist *slist = NULL;
	zk_slist *slist_node = NULL;

	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;

	slist = zk_push_back(slist, &node_1_data);
	slist = zk_push_back(slist, &node_2_data);
	slist = zk_push_back(slist, &node_3_data);

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

	zk_free(&slist, NULL);
}

void test_zk_slist_delete_node_and_use_external_function_to_free_node_data(void)
{
	zk_slist *slist = NULL;
	zk_slist *slist_node = NULL;

	struct dummy_node_data *node_1_data = malloc(sizeof(struct dummy_node_data));
	node_1_data->value = 1;
	node_1_data->string = strdup("node_1");

	struct dummy_node_data *node_2_data = malloc(sizeof(struct dummy_node_data));
	node_2_data->value = 2;
	node_2_data->string = strdup("node_2");

	struct dummy_node_data *node_3_data = malloc(sizeof(struct dummy_node_data));
	node_3_data->value = 3;
	node_3_data->string = strdup("node_3");

	slist = zk_push_back(slist, node_1_data);
	slist = zk_push_back(slist, node_2_data);
	slist = zk_push_back(slist, node_3_data);

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

	zk_free(&slist, dummy_node_data_free);
}

void test_zk_slist_delete_node_when_node_is_not_in_the_list(void)
{
	zk_slist *slist = NULL;
	zk_slist *slist_node = NULL;

	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;

	slist = zk_push_back(slist, &node_1_data);
	slist = zk_push_back(slist, &node_2_data);
	slist = zk_push_back(slist, &node_3_data);

	// node has the same data of the third node of slist, but still this node is not part of slist
	slist_node = zk_push_back(slist_node, &node_3_data);

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

	zk_free(&slist, NULL);
	zk_free(&slist_node, NULL);
}

void test_zk_slist_find_by_data(void)
{
	zk_slist *slist = NULL;
	zk_slist *slist_found = NULL;

	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;
	int node_4_data = 4;
	int node_5_data = 5;
	int node_6_data = 6;
	int node_7_data = 7;

	slist = zk_push_back(slist, &node_1_data);
	slist = zk_push_back(slist, &node_2_data);
	slist = zk_push_back(slist, &node_3_data);
	slist = zk_push_back(slist, &node_4_data);
	slist = zk_push_back(slist, &node_5_data);
	slist = zk_push_back(slist, &node_6_data);
	slist = zk_push_back(slist, &node_7_data);

	slist_found = zk_slist_find(slist, &node_5_data, NULL);

	TEST_ASSERT_NOT_NULL(slist_found);
	TEST_ASSERT_EQUAL(node_5_data, *((int *)(slist_found->data)));
	TEST_ASSERT_EQUAL(node_6_data, *((int *)(slist_found->next->data)));
	TEST_ASSERT_EQUAL(node_7_data, *((int *)(slist_found->next->next->data)));
	TEST_ASSERT_NULL(slist_found->next->next->next);

	// There is no nedd to free slist_found as it is part slist
	zk_free(&slist, NULL);
}

void test_zk_slist_find_by_data_custom_when_node_is_not_in_the_list(void)
{
	zk_slist *slist = NULL;
	zk_slist *slist_found = NULL;

	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;
	int node_4_data = 4;
	int node_5_data = 5;
	int node_6_data = 6;
	int node_7_data = 7;
	int node_8_data = 8;

	slist = zk_push_back(slist, &node_1_data);
	slist = zk_push_back(slist, &node_2_data);
	slist = zk_push_back(slist, &node_3_data);
	slist = zk_push_back(slist, &node_4_data);
	slist = zk_push_back(slist, &node_5_data);
	slist = zk_push_back(slist, &node_6_data);
	slist = zk_push_back(slist, &node_7_data);

	slist_found = zk_slist_find(slist, &node_8_data, NULL);

	TEST_ASSERT_NULL(slist_found);

	// There is no nedd to free slist_found as it is part slist
	zk_free(&slist, NULL);
}

void test_zk_slist_find_by_data_custom(void)
{
	zk_slist *slist = NULL;
	zk_slist *slist_found = NULL;

	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;
	int node_4_data = 4;
	int node_5_data = 5;
	int node_6_data = 6;
	int node_7_data = 7;

	slist = zk_push_back(slist, &node_1_data);
	slist = zk_push_back(slist, &node_2_data);
	slist = zk_push_back(slist, &node_3_data);
	slist = zk_push_back(slist, &node_4_data);
	slist = zk_push_back(slist, &node_5_data);
	slist = zk_push_back(slist, &node_6_data);
	slist = zk_push_back(slist, &node_7_data);

	slist_found = zk_slist_find(slist, &node_5_data, slist_compare_data_custom);

	TEST_ASSERT_NOT_NULL(slist_found);
	TEST_ASSERT_EQUAL(node_5_data, *((int *)(slist_found->data)));
	TEST_ASSERT_EQUAL(node_6_data, *((int *)(slist_found->next->data)));
	TEST_ASSERT_EQUAL(node_7_data, *((int *)(slist_found->next->next->data)));
	TEST_ASSERT_NULL(slist_found->next->next->next);

	// There is no nedd to free slist_found as it is part slist
	zk_free(&slist, NULL);
}

void test_zk_slist_get_index_when_list_is_null(void)
{
	zk_slist *slist = NULL;
	int node_1_data = 1;

	TEST_ASSERT_EQUAL(0, zk_slist_index(slist, &node_1_data, NULL));
}

void test_zk_slist_get_index_func_when_list_is_null(void)
{
	zk_slist *slist = NULL;
	int node_1_data = 1;

	TEST_ASSERT_EQUAL(0, zk_slist_index(slist, &node_1_data, slist_compare_data_custom));
}

void test_zk_slist_get_index_when_data_is_null(void)
{
	zk_slist *slist = NULL;
	int node_1_data = 1;
	int node_3_data = 3;

	slist = zk_push_back(slist, &node_1_data);
	slist = zk_push_back(slist, NULL);
	slist = zk_push_back(slist, &node_3_data);

	TEST_ASSERT_EQUAL(2, zk_slist_index(slist, NULL, NULL));

	zk_free(&slist, NULL);
}

void test_zk_slist_get_index_func_when_data_is_null(void)
{
	zk_slist *slist = NULL;
	int node_1_data = 1;
	int node_3_data = 3;

	slist = zk_push_back(slist, &node_1_data);
	slist = zk_push_back(slist, NULL);
	slist = zk_push_back(slist, &node_3_data);

	TEST_ASSERT_EQUAL(2, zk_slist_index(slist, NULL, slist_compare_data_custom));

	zk_free(&slist, NULL);
}

void test_zk_slist_get_index_when_data_is_on_list(void)
{
	zk_slist *slist = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;

	slist = zk_push_back(slist, &node_1_data);
	slist = zk_push_back(slist, &node_2_data);
	slist = zk_push_back(slist, &node_3_data);

	TEST_ASSERT_EQUAL(3, zk_slist_index(slist, &node_3_data, NULL));

	zk_free(&slist, NULL);
}

void test_zk_slist_get_index_func_when_data_is_on_list(void)
{
	zk_slist *slist = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;

	slist = zk_push_back(slist, &node_1_data);
	slist = zk_push_back(slist, &node_2_data);
	slist = zk_push_back(slist, &node_3_data);

	TEST_ASSERT_EQUAL(3, zk_slist_index(slist, &node_3_data, slist_compare_data_custom));

	zk_free(&slist, NULL);
}

void test_zk_slist_get_index_when_data_is_not_on_list(void)
{
	zk_slist *slist = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;
	int node_4_data = 4;

	slist = zk_push_back(slist, &node_1_data);
	slist = zk_push_back(slist, &node_2_data);
	slist = zk_push_back(slist, &node_3_data);

	TEST_ASSERT_EQUAL(0, zk_slist_index(slist, &node_4_data, NULL));

	zk_free(&slist, NULL);
}

void test_zk_slist_get_index_func_when_data_is_not_on_list(void)
{
	zk_slist *slist = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;
	int node_4_data = 4;

	slist = zk_push_back(slist, &node_1_data);
	slist = zk_push_back(slist, &node_2_data);
	slist = zk_push_back(slist, &node_3_data);

	TEST_ASSERT_EQUAL(0, zk_slist_index(slist, &node_4_data, slist_compare_data_custom));

	zk_free(&slist, NULL);
}

void test_zk_slist_insert_when_list_is_null(void)
{
	zk_slist *slist = NULL;
	int node_1_data = 1;

	slist = zk_slist_insert(slist, &node_1_data, 1);

	TEST_ASSERT_NOT_NULL(slist);
	TEST_ASSERT_EQUAL(&node_1_data, slist->data);
	TEST_ASSERT_EQUAL(node_1_data, *((int *)(slist->data)));

	zk_free(&slist, NULL);
}

void test_zk_slist_insert_when_data_is_null(void)
{
	zk_slist *slist = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;

	slist = zk_push_back(slist, &node_1_data);
	slist = zk_push_back(slist, &node_2_data);
	slist = zk_push_back(slist, &node_3_data);

	// insert a node in postion 1 of the list
	slist = zk_slist_insert(slist, NULL, 1);

	zk_slist *node = zk_slist_find(slist, NULL, NULL);

	TEST_ASSERT_EQUAL(1, zk_slist_index(slist, NULL, NULL));
	TEST_ASSERT_NOT_NULL(node);
	TEST_ASSERT_EQUAL(NULL, node->data);

	zk_free(&slist, NULL);
}

void test_zk_slist_insert_when_position_is_zero(void)
{
	zk_slist *slist = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;
	int node_4_data = 4;

	slist = zk_push_back(slist, &node_1_data);
	slist = zk_push_back(slist, &node_2_data);
	slist = zk_push_back(slist, &node_3_data);

	// as position is zero, new node is appended to the list
	slist = zk_slist_insert(slist, &node_4_data, 0);

	TEST_ASSERT_NOT_NULL(slist);
	TEST_ASSERT_EQUAL(4, zk_slist_index(slist, &node_4_data, NULL));
	TEST_ASSERT_EQUAL(&node_4_data, slist->next->next->next->data);
	TEST_ASSERT_EQUAL(node_4_data, *((int *)(slist->next->next->next->data)));

	zk_free(&slist, NULL);
}

void test_zk_slist_insert_when_position_is_greater_than_list_size(void)
{
	zk_slist *slist = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;
	int node_4_data = 4;

	slist = zk_push_back(slist, &node_1_data);
	slist = zk_push_back(slist, &node_2_data);
	slist = zk_push_back(slist, &node_3_data);

	// as position is greater than list size, new node is appended to the list
	slist = zk_slist_insert(slist, &node_4_data, 10);

	zk_slist *node = zk_slist_last(slist);

	TEST_ASSERT_EQUAL(4, zk_slist_index(slist, &node_4_data, NULL));
	TEST_ASSERT_NOT_NULL(node);
	TEST_ASSERT_EQUAL(&node_4_data, node->data);
	TEST_ASSERT_EQUAL(node_4_data, *((int *)(node->data)));

	zk_free(&slist, NULL);
}

void test_zk_slist_insert_when_position_is_inside_list_range(void)
{
	zk_slist *slist = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;
	int node_4_data = 4;

	slist = zk_push_back(slist, &node_1_data);
	slist = zk_push_back(slist, &node_2_data);
	slist = zk_push_back(slist, &node_4_data);

	// insert node in position 3
	slist = zk_slist_insert(slist, &node_3_data, 3);

	zk_slist *node = zk_slist_find(slist, &node_3_data, NULL);
	TEST_ASSERT_EQUAL(3, zk_slist_index(slist, &node_3_data, NULL));
	TEST_ASSERT_NOT_NULL(node);
	TEST_ASSERT_EQUAL(&node_3_data, node->data);
	TEST_ASSERT_EQUAL(node_3_data, *((int *)(node->data)));

	zk_free(&slist, NULL);
}

void test_zk_slist_insert_before_when_list_is_null(void)
{
	zk_slist *sibling = NULL;
	int node_1_data = 1;

	sibling = zk_push_back(sibling, &node_1_data);

	// if list is null then there is no way to sibling link be part of the list
	TEST_ASSERT_NULL(zk_slist_insert_before(NULL, sibling, &node_1_data));

	zk_free(&sibling, NULL);
}

void test_zk_slist_insert_before_when_sibling_is_null(void)
{
	zk_slist *slist = NULL;
	zk_slist *slist_last = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;
	int node_4_data = 4;

	slist = zk_push_back(slist, &node_1_data);
	slist = zk_push_back(slist, &node_2_data);
	slist = zk_push_back(slist, &node_3_data);

	// if sibling is NULL the new data is added to the end of the list
	slist = zk_slist_insert_before(slist, NULL, &node_4_data);
	TEST_ASSERT_EQUAL(4, zk_slist_length(slist));

	slist_last = zk_slist_last(slist);

	TEST_ASSERT_EQUAL(1, zk_slist_length(slist_last));
	TEST_ASSERT_EQUAL(&node_4_data, slist_last->data);
	TEST_ASSERT_EQUAL(node_4_data, *((int *)(slist_last->data)));
	TEST_ASSERT_NULL(slist_last->next);

	zk_free(&slist, NULL);
}

void test_zk_slist_insert_before_when_sibling_is_first_link_of_the_list(void)
{
	zk_slist *slist = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;
	int node_4_data = 4;

	slist = zk_push_back(slist, &node_2_data);
	slist = zk_push_back(slist, &node_3_data);
	slist = zk_push_back(slist, &node_4_data);

	// sibling is the head of the list (first link of the list)
	slist = zk_slist_insert_before(slist, slist, &node_1_data);
	TEST_ASSERT_EQUAL(4, zk_slist_length(slist));

	TEST_ASSERT_EQUAL(&node_1_data, slist->data);
	TEST_ASSERT_EQUAL(node_1_data, *((int *)(slist->data)));
	TEST_ASSERT_NOT_NULL(slist->next);

	zk_free(&slist, NULL);
}

void test_zk_slist_insert_before_when_sibling_is_in_the_middle_of_the_list(void)
{
	zk_slist *slist = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;
	int node_4_data = 4;

	slist = zk_push_back(slist, &node_1_data);
	slist = zk_push_back(slist, &node_3_data);
	slist = zk_push_back(slist, &node_4_data);

	// sibling is the middle of the list (second link of the list)
	slist = zk_slist_insert_before(slist, slist->next, &node_2_data);
	TEST_ASSERT_EQUAL(4, zk_slist_length(slist));

	// Check link 1
	TEST_ASSERT_EQUAL(&node_1_data, slist->data);
	TEST_ASSERT_EQUAL(node_1_data, *((int *)(slist->data)));
	TEST_ASSERT_NOT_NULL(slist->next);

	// Check link 2 , link that was inserted
	TEST_ASSERT_EQUAL(&node_2_data, slist->next->data);
	TEST_ASSERT_EQUAL(node_2_data, *((int *)(slist->next->data)));
	TEST_ASSERT_NOT_NULL(slist->next->next);

	// Check link 3
	TEST_ASSERT_EQUAL(&node_3_data, slist->next->next->data);
	TEST_ASSERT_EQUAL(node_3_data, *((int *)(slist->next->next->data)));
	TEST_ASSERT_NOT_NULL(slist->next->next->next);

	// Check link 4 , last one
	TEST_ASSERT_EQUAL(&node_4_data, slist->next->next->next->data);
	TEST_ASSERT_EQUAL(node_4_data, *((int *)(slist->next->next->next->data)));
	TEST_ASSERT_NULL(slist->next->next->next->next);

	zk_free(&slist, NULL);
}

void test_zk_slist_insert_before_when_sibling_is_the_last_element_of_the_list(void)
{
	zk_slist *slist = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;
	int node_4_data = 4;

	slist = zk_push_back(slist, &node_1_data);
	slist = zk_push_back(slist, &node_2_data);
	slist = zk_push_back(slist, &node_4_data);

	// sibling is last element of the list
	slist = zk_slist_insert_before(slist, slist->next->next, &node_3_data);
	TEST_ASSERT_EQUAL(4, zk_slist_length(slist));

	// Check link 1
	TEST_ASSERT_EQUAL(&node_1_data, slist->data);
	TEST_ASSERT_EQUAL(node_1_data, *((int *)(slist->data)));
	TEST_ASSERT_NOT_NULL(slist->next);

	// Check link 2
	TEST_ASSERT_EQUAL(&node_2_data, slist->next->data);
	TEST_ASSERT_EQUAL(node_2_data, *((int *)(slist->next->data)));
	TEST_ASSERT_NOT_NULL(slist->next->next);

	// Check link 3, link that was inserted before the last one
	TEST_ASSERT_EQUAL(&node_3_data, slist->next->next->data);
	TEST_ASSERT_EQUAL(node_3_data, *((int *)(slist->next->next->data)));
	TEST_ASSERT_NOT_NULL(slist->next->next->next);

	// Check link 4
	TEST_ASSERT_EQUAL(&node_4_data, slist->next->next->next->data);
	TEST_ASSERT_EQUAL(node_4_data, *((int *)(slist->next->next->next->data)));
	TEST_ASSERT_NULL(slist->next->next->next->next);

	zk_free(&slist, NULL);
}

void test_zk_slist_insert_before_when_sibling_is_not_in_the_list(void)
{
	zk_slist *slist = NULL;
	zk_slist *slist_sibling = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;
	int node_4_data = 4;

	slist = zk_push_back(slist, &node_1_data);
	slist = zk_push_back(slist, &node_2_data);
	slist = zk_push_back(slist, &node_3_data);

	slist_sibling = zk_push_back(slist_sibling, &node_4_data);

	// sibling is not in the list, a new link with data is appended to list
	slist = zk_slist_insert_before(slist, slist_sibling, &node_4_data);
	TEST_ASSERT_EQUAL(4, zk_slist_length(slist));

	// Check link 1
	TEST_ASSERT_EQUAL(&node_1_data, slist->data);
	TEST_ASSERT_EQUAL(node_1_data, *((int *)(slist->data)));
	TEST_ASSERT_NOT_NULL(slist->next);

	// Check link 2
	TEST_ASSERT_EQUAL(&node_2_data, slist->next->data);
	TEST_ASSERT_EQUAL(node_2_data, *((int *)(slist->next->data)));
	TEST_ASSERT_NOT_NULL(slist->next->next);

	// Check link 3
	TEST_ASSERT_EQUAL(&node_3_data, slist->next->next->data);
	TEST_ASSERT_EQUAL(node_3_data, *((int *)(slist->next->next->data)));
	TEST_ASSERT_NOT_NULL(slist->next->next->next);

	// Check link 4, link was appended to the slist
	TEST_ASSERT_EQUAL(&node_4_data, slist->next->next->next->data);
	TEST_ASSERT_EQUAL(node_4_data, *((int *)(slist->next->next->next->data)));
	TEST_ASSERT_NULL(slist->next->next->next->next);

	zk_free(&slist, NULL);
	zk_free(&slist_sibling, NULL);
}

void test_zk_slist_last_when_list_is_null(void)
{
	TEST_ASSERT_NULL(zk_slist_last(NULL));
}

void test_zk_slist_last_when_list_has_only_one_node(void)
{
	zk_slist *slist = NULL;
	zk_slist *slist_last = NULL;
	int node_1_data = 1;

	slist = zk_push_back(slist, &node_1_data);
	slist_last = zk_slist_last(slist);

	TEST_ASSERT_NOT_NULL(slist_last);
	TEST_ASSERT_EQUAL(&node_1_data, slist->data);
	TEST_ASSERT_EQUAL(node_1_data, *((int *)(slist->data)));
	TEST_ASSERT_NULL(slist->next);

	zk_free(&slist, NULL);
}

void test_zk_slist_last_when_list_has_more_than_one_node(void)
{
	zk_slist *slist = NULL;
	zk_slist *slist_last = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;

	slist = zk_push_back(slist, &node_1_data);
	slist = zk_push_back(slist, &node_2_data);
	slist = zk_push_back(slist, &node_3_data);

	slist_last = zk_slist_last(slist);

	TEST_ASSERT_NOT_NULL(slist_last);
	TEST_ASSERT_EQUAL_PTR(&node_3_data, slist_last->data);
	TEST_ASSERT_EQUAL(node_3_data, *((int *)(slist_last->data)));
	TEST_ASSERT_NULL(slist_last->next);

	zk_free(&slist, NULL);
}

void test_zk_slist_length_when_list_is_null(void)
{
	TEST_ASSERT_EQUAL(0, zk_slist_length(NULL));
}

void test_zk_slist_length_when_list_has_one_element(void)
{
	zk_slist *slist = NULL;
	int node_1_data = 1;

	slist = zk_push_back(slist, &node_1_data);

	TEST_ASSERT_EQUAL(1, zk_slist_length(slist));

	zk_free(&slist, NULL);
}

void test_zk_slist_length_when_list_has_3_elements(void)
{
	zk_slist *slist = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;

	slist = zk_push_back(slist, &node_1_data);
	slist = zk_push_back(slist, &node_2_data);
	slist = zk_push_back(slist, &node_3_data);

	TEST_ASSERT_EQUAL(3, zk_slist_length(slist));

	zk_free(&slist, NULL);
}

void test_zk_slist_nth_when_list_is_null(void)
{
	TEST_ASSERT_NULL(zk_slist_nth(NULL, 0));
}

void test_zk_slist_nth_when_index_is_zero(void)
{
	zk_slist *slist = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;

	slist = zk_push_back(slist, &node_1_data);
	slist = zk_push_back(slist, &node_2_data);
	slist = zk_push_back(slist, &node_3_data);

	TEST_ASSERT_EQUAL(3, zk_slist_length(slist));
	TEST_ASSERT_EQUAL(slist->next->next, zk_slist_nth(slist, 0));

	zk_free(&slist, NULL);
}

void test_zk_slist_nth_when_index_is_greater_than_list_length(void)
{
	zk_slist *slist = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;

	slist = zk_push_back(slist, &node_1_data);
	slist = zk_push_back(slist, &node_2_data);
	slist = zk_push_back(slist, &node_3_data);

	TEST_ASSERT_EQUAL(3, zk_slist_length(slist));
	TEST_ASSERT_EQUAL(slist->next->next, zk_slist_nth(slist, 4));

	zk_free(&slist, NULL);
}

void test_zk_slist_nth_when_index_is_less_than_or_equal_to_list_length(void)
{
	zk_slist *slist = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;

	slist = zk_push_back(slist, &node_1_data);
	slist = zk_push_back(slist, &node_2_data);
	slist = zk_push_back(slist, &node_3_data);

	TEST_ASSERT_EQUAL(3, zk_slist_length(slist));
	TEST_ASSERT_EQUAL(slist->next, zk_slist_nth(slist, 2));
	zk_free(&slist, NULL);
}

void test_zk_slist_nth_when_index_equal_to_list_length(void)
{
	zk_slist *slist = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;

	slist = zk_push_back(slist, &node_1_data);
	slist = zk_push_back(slist, &node_2_data);
	slist = zk_push_back(slist, &node_3_data);

	TEST_ASSERT_EQUAL(3, zk_slist_length(slist));
	TEST_ASSERT_EQUAL(slist->next->next, zk_slist_nth(slist, 3));
	zk_free(&slist, NULL);
}

void test_zk_slist_nth_when_index_is_1(void)
{
	zk_slist *slist = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;

	slist = zk_push_back(slist, &node_1_data);
	slist = zk_push_back(slist, &node_2_data);
	slist = zk_push_back(slist, &node_3_data);

	TEST_ASSERT_EQUAL(3, zk_slist_length(slist));
	TEST_ASSERT_EQUAL(slist, zk_slist_nth(slist, 1));
	zk_free(&slist, NULL);
}

void test_zk_slit_reverse_when_list_is_null(void)
{
	zk_slist *slist = NULL;
	TEST_ASSERT_NULL(zk_slist_reverse(slist));
}

void test_zk_slit_reverse_when_list_has_one_link(void)
{
	zk_slist *slist = NULL;
	int node1_data = 1;

	slist = zk_push_back(slist, &node1_data);
	slist = zk_slist_reverse(slist);

	TEST_ASSERT_NOT_NULL(slist);
	TEST_ASSERT_EQUAL_PTR(slist->data, &node1_data);
	TEST_ASSERT_EQUAL(node1_data, *((int *)slist->data));
	TEST_ASSERT_NULL(slist->next);

	zk_free(&slist, NULL);
	TEST_ASSERT_NULL(slist);
}

void test_zk_slit_reverse_when_list_has_mutiple_nodes(void)
{
	zk_slist *slist = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;

	slist = zk_push_back(slist, &node_1_data);
	slist = zk_push_back(slist, &node_2_data);
	slist = zk_push_back(slist, &node_3_data);

	slist = zk_slist_reverse(slist);
	/* after reverse operation we should expect nodes of slist to be:
	 * node_1.data = node_3_data
	 * node_2.data = node_2_data
	 * node_3.data = node_1_data
	 */
	zk_slist *node_1, *node_2, *node_3 = NULL;
	node_1 = slist;
	TEST_ASSERT_NOT_NULL(node_1);
	TEST_ASSERT_EQUAL_PTR(node_1->data, &node_3_data);
	TEST_ASSERT_EQUAL(*((int *)node_1->data), node_3_data);
	TEST_ASSERT_NOT_NULL(node_1->next);

	node_2 = node_1->next;
	TEST_ASSERT_NOT_NULL(node_2);
	TEST_ASSERT_EQUAL_PTR(node_2->data, &node_2_data);
	TEST_ASSERT_EQUAL(*((int *)node_2->data), node_2_data);
	TEST_ASSERT_NOT_NULL(node_2->next);

	node_3 = node_2->next;
	TEST_ASSERT_NOT_NULL(node_3);
	TEST_ASSERT_EQUAL_PTR(node_3->data, &node_1_data);
	TEST_ASSERT_EQUAL(*((int *)node_3->data), node_1_data);
	TEST_ASSERT_NULL(node_3->next);

	zk_free(&slist, NULL);
	TEST_ASSERT_NULL(slist);
}

/*--------------- Test Constructor ---------------*/
// test zk_slist_new_node
void test_zk_slist_new_node_when_data_is_null(void)
{
	zk_slist *list = zk_slist_new_node(NULL);
	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_NULL(list->data);
	TEST_ASSERT_NULL(list->next);

	zk_free(&list, NULL);
}
void test_zk_slist_new_node_when_data_is_not_null(void)
{
	zk_slist *list = zk_slist_new_node("data");
	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_NOT_NULL(list->data);
	TEST_ASSERT_EQUAL("data", (char *)(list->data));
	TEST_ASSERT_NULL(list->next);

	zk_free(&list, NULL);
}

/*--------------- Test Destructor ---------------*/
// test zk_free
void test_zk_free_a_null_list_should_just_return(void)
{
	zk_slist *slist = NULL;
	zk_free(&slist, NULL);
	TEST_ASSERT_NULL(slist);
}

void test_zk_free_list_with_no_dynamic_allocated_node_data(void)
{
	zk_slist *slist = NULL;
	int node_1_data = 1;
	int node_2_data = 2;
	int node_3_data = 3;
	int node_4_data = 4;

	slist = zk_push_back(slist, &node_1_data);
	slist = zk_push_back(slist, &node_2_data);
	slist = zk_push_back(slist, &node_3_data);
	slist = zk_push_back(slist, &node_4_data);

	zk_free(&slist, NULL);

	TEST_ASSERT_NULL(slist);
}

void test_zk_free_for_list_of_strings(void)
{
	int number_of_nodes = 100;
	zk_slist *slist = NULL;

	for (int i = 0; i < number_of_nodes; i++) {
		slist = zk_push_back(slist, strdup("a"));
		TEST_ASSERT_NOT_NULL(slist);
	}

	zk_free(&slist, free);
	TEST_ASSERT_NULL(slist);
}

void test_zk_free_for_a_null_list_should_just_return(void)
{
	zk_slist *slist = NULL;
	zk_free(&slist, free);
	TEST_ASSERT_NULL(slist);
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

	list = zk_push_back(list, &node_1_data);

	begin = zk_begin(list);

	TEST_ASSERT_EQUAL_PTR(list, begin);
	TEST_ASSERT_EQUAL(list->data, begin->data);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_begin_returns_1st_element_when_slist_has_more_than_one_element(void)
{
	zk_slist *list = NULL, *begin = NULL;
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
void test_zk_end_returns_null_when_slist_is_empty(void)
{
	zk_slist *list = NULL;

	TEST_ASSERT_NULL(zk_end(list));
}

void test_zk_end_returns_null_when_slist_has_one_element(void)
{
	zk_slist *list = NULL;
	int node_1_data = 1;

	list = zk_push_back(list, &node_1_data);

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
	zk_slist *slist = NULL;

	slist = zk_push_back(slist, node_1_data);
	TEST_ASSERT_NOT_NULL(slist);

	// Func is null, it will just return
	zk_for_each(slist, NULL, NULL);

	zk_free(&slist, dummy_node_data_free);
	TEST_ASSERT_NULL(slist);
}

void test_zk_for_each_when_list_is_null(void)
{
	zk_slist *slist = NULL;

	// list is null, it will just return
	zk_for_each(slist, dummy_node_data_free_foreach, NULL);

	TEST_ASSERT_NULL(slist);
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

	zk_slist *slist = NULL;
	slist = zk_push_back(slist, node_1_data);
	slist = zk_push_back(slist, node_2_data);
	slist = zk_push_back(slist, node_3_data);

	// Use foreach to clean each node data
	zk_for_each(slist, dummy_node_data_free_foreach, NULL);

	// As nodes data were freed, we just need to call zk_free.
	zk_free(&slist, NULL);
}

/*--------------- Test Modifiers ---------------*/
// tests for zk_pop_back()
void test_zk_pop_back_when_list_is_empty(void)
{
	zk_slist *list = NULL;
	TEST_ASSERT_NULL(zk_pop_back(list, NULL));
	TEST_ASSERT_NULL(list);
}

void test_zk_pop_back_when_list_has_1_element(void)
{
	zk_slist *list = NULL;
	char *data[] = { "0" };
	list = zk_push_back(list, data[0]);

	list = zk_pop_back(list, NULL);
	TEST_ASSERT_NULL(list);

	// no need to call zk_free() as list is freed by now
}

void test_zk_pop_back_when_list_has_2_elements(void)
{
	zk_slist *list = NULL;
	char *data[] = { "0", "1" };
	list = zk_push_back(list, data[0]);
	list = zk_push_back(list, data[1]);

	list = zk_pop_back(list, NULL);

	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_NULL(list->next);
	TEST_ASSERT_EQUAL_STRING(data[0], (char *)list->data);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_pop_back_when_list_has_3_elements(void)
{
	zk_slist *list = NULL;
	list = zk_push_back(list, "0");
	list = zk_push_back(list, "1");
	list = zk_push_back(list, "2");

	list = zk_pop_back(list, NULL);

	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_EQUAL_STRING("0", (char *)list->data);
	TEST_ASSERT_NOT_NULL(list->next);
	TEST_ASSERT_EQUAL_STRING("1", (char *)list->next->data);
	TEST_ASSERT_NULL(list->next->next);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_pop_back_when_list_has_n_elements(void)
{
	char *data[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
	zk_slist *list = NULL;
	for (size_t i = 0; i < 10; i++) {
		list = zk_push_back(list, data[i]);
	}

	// pop all elements
	while (list != NULL) {
		TEST_ASSERT_NOT_NULL(list);
		TEST_ASSERT_EQUAL_STRING("0", (char *)list->data);
		list = zk_pop_back(list, NULL);
	}
	// no need to call zk_free() as list is freed by now
}
void test_zk_pop_back_with_free_func_when_list_has_n_elements(void)
{
	zk_slist *list = NULL;
	for (size_t i = 0; i < 10; i++) {
		list = zk_push_back(list, strdup("data"));
	}

	while (list != NULL) {
		TEST_ASSERT_NOT_NULL(list);
		TEST_ASSERT_EQUAL_STRING("data", (char *)list->data);
		list = zk_pop_back(list, free);
	}

	// no need to call zk_free() as list is freed by now
}

// tests for zk_pop_front()
void test_zk_pop_front_when_list_is_empty(void)
{
	zk_slist *list = NULL;
	TEST_ASSERT_NULL(zk_pop_front(list, NULL));
	TEST_ASSERT_NULL(list);
}

void test_zk_pop_front_when_list_has_1_element(void)
{
	zk_slist *list = NULL;
	char *data[] = { "0" };
	list = zk_push_back(list, data[0]);

	list = zk_pop_front(list, NULL);
	TEST_ASSERT_NULL(list);

	// no need to call zk_free() as list is freed by now
}

void test_zk_pop_front_when_list_has_2_elements(void)
{
	zk_slist *list = NULL;
	char *data[] = { "0", "1" };
	list = zk_push_back(list, data[0]);
	list = zk_push_back(list, data[1]);

	list = zk_pop_front(list, NULL);

	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_NULL(list->next);
	TEST_ASSERT_EQUAL_STRING(data[1], (char *)list->data);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_pop_front_when_list_has_3_elements(void)
{
	zk_slist *list = NULL;
	list = zk_push_back(list, "0");
	list = zk_push_back(list, "1");
	list = zk_push_back(list, "2");

	list = zk_pop_front(list, NULL);

	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_EQUAL_STRING("1", (char *)list->data);
	TEST_ASSERT_NOT_NULL(list->next);
	TEST_ASSERT_EQUAL_STRING("2", (char *)list->next->data);
	TEST_ASSERT_NULL(list->next->next);

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_pop_front_when_list_has_n_elements(void)
{
	char *data[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
	zk_slist *list = NULL;
	for (size_t i = 0; i < 10; i++) {
		list = zk_push_back(list, data[i]);
	}

	// pop all elements
	size_t i = 0;
	while (list != NULL) {
		TEST_ASSERT_NOT_NULL(list);
		TEST_ASSERT_EQUAL_STRING(data[i++], (char *)list->data);
		list = zk_pop_front(list, NULL);
	}
	// no need to call zk_free() as list is freed by now
}

void test_zk_pop_front_with_free_func_when_list_has_n_elements(void)
{
	zk_slist *list = NULL;
	for (size_t i = 0; i < 10; i++) {
		list = zk_push_back(list, strdup("data"));
	}

	while (list != NULL) {
		TEST_ASSERT_NOT_NULL(list);
		TEST_ASSERT_EQUAL_STRING("data", (char *)list->data);
		list = zk_pop_front(list, free);
	}

	// no need to call zk_free() as list is freed by now
}

// tests for zk_push_back()
void test_zk_push_back_to_empty_list(void)
{
	zk_slist *slist = NULL;
	int data = 5;

	slist = zk_push_back(slist, &data);

	TEST_ASSERT_NOT_NULL(slist);
	TEST_ASSERT_EQUAL_PTR(slist->data, &data);
	TEST_ASSERT_EQUAL(*((int *)slist->data), data);
	TEST_ASSERT_NULL(slist->next);

	zk_free(&slist, NULL);
	TEST_ASSERT_NULL(slist);
}

void test_zk_push_back_2_items_to_slist(void)
{
	zk_slist *slist = NULL;

	int node1_data = 0;
	int node2_data = 1;

	slist = zk_push_back(slist, &node1_data);

	TEST_ASSERT_NOT_NULL(slist);
	TEST_ASSERT_EQUAL_PTR(slist->data, &node1_data);
	TEST_ASSERT_EQUAL_PTR(*((int *)slist->data), node1_data);
	TEST_ASSERT_NULL(slist->next);

	slist = zk_push_back(slist, &node2_data);

	// first we check that slist still points to the head of the list
	TEST_ASSERT_NOT_NULL(slist);
	TEST_ASSERT_EQUAL_PTR(slist->data, &node1_data);
	TEST_ASSERT_EQUAL_PTR(*((int *)slist->data), node1_data);
	TEST_ASSERT_NOT_NULL(slist->next);

	// now we check the content of the second node of the list
	zk_slist *node_1 = slist->next;
	TEST_ASSERT_NOT_NULL(node_1);
	TEST_ASSERT_EQUAL_PTR(node_1->data, &node2_data);
	TEST_ASSERT_EQUAL_PTR(*((int *)node_1->data), node2_data);
	TEST_ASSERT_NULL(node_1->next);

	zk_free(&slist, NULL);
	TEST_ASSERT_NULL(slist);
}

void test_zk_push_back_3_items_to_slist(void)
{
	zk_slist *slist = NULL;

	int node1_data = 0;
	int node2_data = 1;
	int node3_data = 3;

	zk_slist *node_2 = NULL;
	zk_slist *node_3 = NULL;

	slist = zk_push_back(slist, &node1_data);

	TEST_ASSERT_NOT_NULL(slist);
	TEST_ASSERT_EQUAL_PTR(slist->data, &node1_data);
	TEST_ASSERT_EQUAL(*((int *)slist->data), node1_data);
	TEST_ASSERT_NULL(slist->next);

	slist = zk_push_back(slist, &node2_data);

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

	slist = zk_push_back(slist, &node3_data);

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

	zk_free(&slist, NULL);
	TEST_ASSERT_NULL(slist);
}

void test_zk_push_back_n_items_to_slist(void)
{
	int number_of_nodes = 100;
	int nodes_data[number_of_nodes];
	zk_slist *slist = NULL;

	for (int i = 0; i < number_of_nodes; i++) {
		nodes_data[i] = i;
		slist = zk_push_back(slist, &nodes_data[i]);
		TEST_ASSERT_NOT_NULL(slist);
	}

	int node_idx = 0;
	for (zk_slist *node_i = slist;; node_i = node_i->next) {
		TEST_ASSERT_NOT_NULL(node_i);
		TEST_ASSERT_EQUAL_PTR(node_i->data, &nodes_data[node_idx]);
		TEST_ASSERT_EQUAL(*((int *)node_i->data), nodes_data[node_idx]);
		if (node_i->next == NULL) {
			break;
		}
		node_idx++;
	}

	zk_free(&slist, NULL);
	TEST_ASSERT_NULL(slist);
}

void test_zk_push_back_null_data_to_slist(void)
{
	int number_of_nodes = 100;
	zk_slist *slist = NULL;

	for (int i = 0; i < number_of_nodes; i++) {
		slist = zk_push_back(slist, NULL);
		TEST_ASSERT_NOT_NULL(slist);
	}

	for (zk_slist *node_i = slist;; node_i = node_i->next) {
		TEST_ASSERT_NOT_NULL(node_i);
		TEST_ASSERT_EQUAL_PTR(node_i->data, NULL);
		if (node_i->next == NULL) {
			break;
		}
	}

	zk_free(&slist, NULL);
	TEST_ASSERT_NULL(slist);
}

// Tests for zk_push_front()
void test_zk_push_front_n_items_to_slist(void)
{
	int number_of_nodes = 100;
	int nodes_data[number_of_nodes];
	zk_slist *list = NULL;

	for (int i = 0; i < number_of_nodes; i++) {
		nodes_data[i] = i;
		list = zk_push_front(list, &nodes_data[i]);
		TEST_ASSERT_NOT_NULL(list);
	}

	int node_idx = 1;
	for (zk_slist *node_i = list;; node_i = node_i->next) {
		TEST_ASSERT_NOT_NULL(node_i);
		TEST_ASSERT_EQUAL_PTR(node_i->data, &nodes_data[number_of_nodes - node_idx]);
		TEST_ASSERT_EQUAL_PTR(*((int *)node_i->data), nodes_data[number_of_nodes - node_idx]);
		if (node_i->next == NULL) {
			break;
		}
		node_idx++;
	}

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_push_front_null_data_to_slist(void)
{
	int number_of_nodes = 100;
	zk_slist *list = NULL;

	for (int i = 0; i < number_of_nodes; i++) {
		list = zk_push_front(list, NULL);
		TEST_ASSERT_NOT_NULL(list);
	}

	for (zk_slist *node_i = list;; node_i = node_i->next) {
		TEST_ASSERT_NOT_NULL(node_i);
		TEST_ASSERT_EQUAL_PTR(node_i->data, NULL);
		if (node_i->next == NULL) {
			break;
		}
	}

	zk_free(&list, NULL);
	TEST_ASSERT_NULL(list);
}

/*--------------- Test Private Functions ---------------*/

void test_zk_slist_front_back_split_when_list_is_null(void)
{
	zk_slist *front = NULL;
	zk_slist *back = NULL;

	_zk_slist_front_back_split(NULL, &front, &back);

	TEST_ASSERT_NULL(front);
	TEST_ASSERT_NULL(back);
}

void test_zk_slist_front_back_split_when_list_has_one_link(void)
{
	int node_data = 1;
	zk_slist *slist = NULL;
	zk_slist *front = NULL;
	zk_slist *back = NULL;

	slist = zk_push_back(slist, &node_data);
	TEST_ASSERT_NOT_NULL(slist);

	_zk_slist_front_back_split(slist, &front, &back);

	TEST_ASSERT_NOT_NULL(front);
	TEST_ASSERT_EQUAL_PTR(&node_data, front->data);
	TEST_ASSERT_EQUAL(node_data, *((int *)front->data));

	TEST_ASSERT_NULL(back);

	zk_free(&front, NULL);
}

void test_zk_slist_front_back_split_when_list_has_two_links(void)
{
	int node_data_0 = 0;
	int node_data_1 = 1;
	zk_slist *slist = NULL;
	zk_slist *front = NULL;
	zk_slist *back = NULL;

	slist = zk_push_back(slist, &node_data_0);
	slist = zk_push_back(slist, &node_data_1);
	TEST_ASSERT_NOT_NULL(slist);

	_zk_slist_front_back_split(slist, &front, &back);

	TEST_ASSERT_NOT_NULL(front);
	TEST_ASSERT_EQUAL_PTR(&node_data_0, front->data);
	TEST_ASSERT_EQUAL(node_data_0, *((int *)front->data));

	TEST_ASSERT_NOT_NULL(back);
	TEST_ASSERT_EQUAL_PTR(&node_data_1, back->data);
	TEST_ASSERT_EQUAL(node_data_1, *((int *)back->data));

	zk_free(&front, NULL);
	zk_free(&back, NULL);
}

void test_zk_slist_front_back_split_when_list_has_three_links(void)
{
	int node_data_0 = 0;
	int node_data_1 = 1;
	int node_data_2 = 3;
	zk_slist *slist = NULL;
	zk_slist *front = NULL;
	zk_slist *back = NULL;

	slist = zk_push_back(slist, &node_data_0);
	slist = zk_push_back(slist, &node_data_1);
	slist = zk_push_back(slist, &node_data_2);
	TEST_ASSERT_NOT_NULL(slist);

	_zk_slist_front_back_split(slist, &front, &back);

	TEST_ASSERT_NOT_NULL(front);
	TEST_ASSERT_EQUAL_PTR(&node_data_0, front->data);
	TEST_ASSERT_EQUAL(node_data_0, *((int *)front->data));
	TEST_ASSERT_EQUAL_PTR(&node_data_1, front->next->data);
	TEST_ASSERT_EQUAL(node_data_1, *((int *)front->next->data));

	TEST_ASSERT_NOT_NULL(back);
	TEST_ASSERT_EQUAL_PTR(&node_data_2, back->data);
	TEST_ASSERT_EQUAL(node_data_2, *((int *)back->data));

	zk_free(&front, NULL);
	zk_free(&back, NULL);
}

void test_zk_slist_front_back_split_when_number_of_links_is_even(void)
{
	const int number_of_nodes = 10;
	int nodes_data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	zk_slist *slist = NULL;
	zk_slist *front = NULL;
	zk_slist *back = NULL;

	for (int i = 0; i < number_of_nodes; i++) {
		slist = zk_push_back(slist, &nodes_data[i]);
		TEST_ASSERT_NOT_NULL(slist);
	}

	_zk_slist_front_back_split(slist, &front, &back);

	const zk_slist *node_i = front;
	int node_idx = 0;

	while (node_i != NULL) {
		TEST_ASSERT_NOT_NULL(node_i);
		TEST_ASSERT_EQUAL(nodes_data[node_idx], *((int *)node_i->data));
		TEST_ASSERT_EQUAL_PTR(&nodes_data[node_idx], node_i->data);
		node_i = node_i->next;
		node_idx++;
	}

	node_i = back;
	while (node_i != NULL) {
		TEST_ASSERT_NOT_NULL(node_i);
		TEST_ASSERT_EQUAL(nodes_data[node_idx], *((int *)node_i->data));
		TEST_ASSERT_EQUAL_PTR(&nodes_data[node_idx], node_i->data);
		node_i = node_i->next;
		node_idx++;
	}

	zk_free(&front, NULL);
	zk_free(&back, NULL);
}

void test_zk_slist_front_back_split_when_number_of_links_is_odd(void)
{
	const int number_of_nodes = 9;
	int nodes_data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
	zk_slist *slist = NULL;
	zk_slist *front = NULL;
	zk_slist *back = NULL;

	for (int i = 0; i < number_of_nodes; i++) {
		slist = zk_push_back(slist, &nodes_data[i]);
		TEST_ASSERT_NOT_NULL(slist);
	}

	_zk_slist_front_back_split(slist, &front, &back);

	const zk_slist *node_i = front;
	int node_idx = 0;

	while (node_i != NULL) {
		TEST_ASSERT_NOT_NULL(node_i);
		TEST_ASSERT_EQUAL(nodes_data[node_idx], *((int *)node_i->data));
		TEST_ASSERT_EQUAL_PTR(&nodes_data[node_idx], node_i->data);

		node_i = node_i->next;
		node_idx++;
	}

	node_i = back;
	while (node_i != NULL) {
		TEST_ASSERT_NOT_NULL(node_i);
		TEST_ASSERT_EQUAL(nodes_data[node_idx], *((int *)node_i->data));
		TEST_ASSERT_EQUAL_PTR(&nodes_data[node_idx], node_i->data);

		node_i = node_i->next;
		node_idx++;
	}

	zk_free(&front, NULL);
	zk_free(&back, NULL);
}

int main(void)
{
	UNITY_BEGIN();

	{ // tests for for zk_slist_concat
		RUN_TEST(test_zk_slist_concat_lists_of_strings);
		RUN_TEST(test_zk_slist_concat_when_destination_list_is_null_should_return_null);
		RUN_TEST(test_zk_slist_concat_when_source_list_is_null_should_return_destination_list);
	}

	{ // tests for for zk_slist_copy
		RUN_TEST(test_zk_slist_copy_when_source_list_is_null);
		RUN_TEST(test_zk_slist_copy_when_source_list_node_data_is_a_pointer_to_data_only_the_pointer_is_copied);
	}

	{ // tests for for zk_slist_copy_deep
		RUN_TEST(test_zk_slist_copy_full_should_return_null_when_source_list_is_null);
		RUN_TEST(test_zk_slist_copy_full_should_return_null_when_copy_function_is_null);
		RUN_TEST(test_zk_slist_copy_deep_should_perform_a_deep_copy_of_source_list_nodes_data);
	}

	{ // tests for zk_slist_delete_node
		RUN_TEST(test_zk_slist_delete_node_when_list_is_null);
		RUN_TEST(test_zk_slist_delete_node_when_node_is_null);
		RUN_TEST(test_zk_slist_delete_node_when_list_has_only_one_node);
		RUN_TEST(test_zk_slist_delete_first_node_when_list_has_mutiple_nodes);
		RUN_TEST(test_zk_slist_delete_last_node);
		RUN_TEST(test_zk_slist_delete_node_when_list_has_multiple_nodes);
		RUN_TEST(test_zk_slist_delete_node_and_use_external_function_to_free_node_data);
		RUN_TEST(test_zk_slist_delete_node_when_node_is_not_in_the_list);
	}

	{ // tests for zk_slist_find
		RUN_TEST(test_zk_slist_find_by_data);
		RUN_TEST(test_zk_slist_find_by_data_custom_when_node_is_not_in_the_list);
		RUN_TEST(test_zk_slist_find_by_data_custom);
	}

	{ // tests for zk_slist_index
		RUN_TEST(test_zk_slist_get_index_when_list_is_null);
		RUN_TEST(test_zk_slist_get_index_when_data_is_null);
		RUN_TEST(test_zk_slist_get_index_when_data_is_on_list);
		RUN_TEST(test_zk_slist_get_index_when_data_is_not_on_list);
		RUN_TEST(test_zk_slist_get_index_func_when_list_is_null);
		RUN_TEST(test_zk_slist_get_index_func_when_data_is_null);
		RUN_TEST(test_zk_slist_get_index_func_when_data_is_on_list);
		RUN_TEST(test_zk_slist_get_index_func_when_data_is_not_on_list);
	}

	{ // tests for zk_slist_insert

		RUN_TEST(test_zk_slist_insert_when_list_is_null);
		RUN_TEST(test_zk_slist_insert_when_data_is_null);
		RUN_TEST(test_zk_slist_insert_when_position_is_zero);
		RUN_TEST(test_zk_slist_insert_when_position_is_greater_than_list_size);
		RUN_TEST(test_zk_slist_insert_when_position_is_inside_list_range);
	}

	{ // tests for zk_slist_insert_before

		RUN_TEST(test_zk_slist_insert_before_when_list_is_null);
		RUN_TEST(test_zk_slist_insert_before_when_sibling_is_null);
		RUN_TEST(test_zk_slist_insert_before_when_sibling_is_first_link_of_the_list);
		RUN_TEST(test_zk_slist_insert_before_when_sibling_is_in_the_middle_of_the_list);
		RUN_TEST(test_zk_slist_insert_before_when_sibling_is_the_last_element_of_the_list);
		RUN_TEST(test_zk_slist_insert_before_when_sibling_is_not_in_the_list);
	}

	{ // tests for zk_slist_last
		RUN_TEST(test_zk_slist_last_when_list_is_null);
		RUN_TEST(test_zk_slist_last_when_list_has_only_one_node);
		RUN_TEST(test_zk_slist_last_when_list_has_more_than_one_node);
	}

	{ // tests for zk_slist_length

		RUN_TEST(test_zk_slist_length_when_list_is_null);
		RUN_TEST(test_zk_slist_length_when_list_has_one_element);
		RUN_TEST(test_zk_slist_length_when_list_has_3_elements);
	}

	{ // tests for zk_slist_nth

		RUN_TEST(test_zk_slist_nth_when_list_is_null);
		RUN_TEST(test_zk_slist_nth_when_index_is_zero);
		RUN_TEST(test_zk_slist_nth_when_index_is_greater_than_list_length);
		RUN_TEST(test_zk_slist_nth_when_index_is_less_than_or_equal_to_list_length);
		RUN_TEST(test_zk_slist_nth_when_index_equal_to_list_length);
		RUN_TEST(test_zk_slist_nth_when_index_is_1);
	}

	{ // tests for zk_slit_reverse
		RUN_TEST(test_zk_slit_reverse_when_list_is_null);
		RUN_TEST(test_zk_slit_reverse_when_list_has_one_link);
		RUN_TEST(test_zk_slit_reverse_when_list_has_mutiple_nodes);
	}

	/*--------------- Test Constructor ---------------*/
	{ // tests for zk_slist_new_node()
		RUN_TEST(test_zk_slist_new_node_when_data_is_null);
		RUN_TEST(test_zk_slist_new_node_when_data_is_not_null);
	}

	/*--------------- Test Destructor ---------------*/

	{ // tests for zk_free()
		RUN_TEST(test_zk_free_a_null_list_should_just_return);
		RUN_TEST(test_zk_free_list_with_no_dynamic_allocated_node_data);
		RUN_TEST(test_zk_free_for_list_of_strings);
		RUN_TEST(test_zk_free_for_a_null_list_should_just_return);
	}

	/*--------------- Test Iterators ---------------*/

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

	/*--------------- Test Modifiers ---------------*/
	{ // tests for zk_pop_back()
		RUN_TEST(test_zk_pop_back_when_list_is_empty);
		RUN_TEST(test_zk_pop_back_when_list_has_1_element);
		RUN_TEST(test_zk_pop_back_when_list_has_2_elements);
		RUN_TEST(test_zk_pop_back_when_list_has_3_elements);
		RUN_TEST(test_zk_pop_back_when_list_has_n_elements);
		RUN_TEST(test_zk_pop_back_with_free_func_when_list_has_n_elements);
	}

	{ // tests for zk_pop_front()
		RUN_TEST(test_zk_pop_front_when_list_is_empty);
		RUN_TEST(test_zk_pop_front_when_list_has_1_element);
		RUN_TEST(test_zk_pop_front_when_list_has_2_elements);
		RUN_TEST(test_zk_pop_front_when_list_has_3_elements);
		RUN_TEST(test_zk_pop_front_when_list_has_n_elements);
		RUN_TEST(test_zk_pop_front_with_free_func_when_list_has_n_elements);
	}

	{ // tests for zk_push_back()
		RUN_TEST(test_zk_push_back_to_empty_list);
		RUN_TEST(test_zk_push_back_2_items_to_slist);
		RUN_TEST(test_zk_push_back_3_items_to_slist);
		RUN_TEST(test_zk_push_back_n_items_to_slist);
		RUN_TEST(test_zk_push_back_null_data_to_slist);
	}

	{ // tests for zk_push_front()

		RUN_TEST(test_zk_push_front_n_items_to_slist);
		RUN_TEST(test_zk_push_front_null_data_to_slist);
	}

	/*--------------- Test Private Functions ---------------*/
	{ // tests for _zk_slist_front_back_split
		RUN_TEST(test_zk_slist_front_back_split_when_list_is_null);
		RUN_TEST(test_zk_slist_front_back_split_when_list_has_one_link);
		RUN_TEST(test_zk_slist_front_back_split_when_list_has_two_links);
		RUN_TEST(test_zk_slist_front_back_split_when_list_has_three_links);
		RUN_TEST(test_zk_slist_front_back_split_when_number_of_links_is_even);
		RUN_TEST(test_zk_slist_front_back_split_when_number_of_links_is_odd);
	}

	return UNITY_END();
}
