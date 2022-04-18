#include <stdlib.h>
#include <string.h>

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

int main(void)
{
	UNITY_BEGIN();

	// tests for zk_slist_append function
	RUN_TEST(test_zk_slist_append_to_empty_list);
	RUN_TEST(test_zk_slist_append_2_items_to_slist);
	RUN_TEST(test_zk_slist_append_3_items_to_slist);
	RUN_TEST(test_zk_slist_append_n_items_to_slist);
	RUN_TEST(test_zk_slist_append_null_data_to_slist);

	// tests for zk_slist_prepend function
	RUN_TEST(test_zk_slist_prepend_n_items_to_slist);
	RUN_TEST(test_zk_slist_prepend_null_data_to_slist);

	RUN_TEST(test_zk_slist_free_a_null_list_should_just_return);

	// tests for zk_slist_free_full function
	RUN_TEST(test_zk_slist_free_full_for_list_of_strings);
	RUN_TEST(test_zk_slist_free_full_for_a_null_list_should_just_return);
	RUN_TEST(test_zk_slist_free_full_for_a_null_destructor_should_just_return);

	// test for zk_slist_concat
	RUN_TEST(test_zk_slist_concat_lists_of_strings);
	RUN_TEST(test_zk_slist_concat_when_destination_list_is_null_should_return_null);
	RUN_TEST(test_zk_slist_concat_when_source_list_is_null_should_return_null);

	return UNITY_END();
}
