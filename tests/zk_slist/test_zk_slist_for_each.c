#include <stdlib.h>
#include <string.h>

#include "unity.h"
#include "zk/zklib.h"

struct dummy_node_data {
	int value;
	char *string;
};
void dummy_node_data_free_foreach(void *data, void *user_data)
{
	ZK_UNUSED(user_data);
	struct dummy_node_data *node_data = data;
	free(node_data->string);
	free(node_data);
}

void dummy_node_data_free(void *data)
{
	struct dummy_node_data *node_data = data;
	free(node_data->string);
	free(node_data);
}

void setUp(void) {}

void tearDown(void) {}

// test zk_for_each
void test_zk_slist_for_each_when_func_is_null(void)
{
	struct dummy_node_data *node_1_data = malloc(sizeof(struct dummy_node_data));
	node_1_data->value = 1;
	node_1_data->string = strdup("node_1");
	zk_slist *list = NULL;

	list = zk_slist_push_back(list, node_1_data);
	TEST_ASSERT_NOT_NULL(list);

	// Func is null, it will just return
	zk_slist_for_each(list, NULL, NULL, NULL);

	zk_slist_free(&list, dummy_node_data_free);
}

void test_zk_slist_for_each_when_list_is_null(void)
{
	zk_slist *list = NULL;

	// list is null, it will just return
	zk_slist_for_each(list, NULL, dummy_node_data_free_foreach, NULL);

	TEST_ASSERT_NULL(list);
}

void test_zk_slist_for_each_when_func_is_not_null(void)
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
	zk_slist_for_each(list, NULL, dummy_node_data_free_foreach, NULL);

	zk_slist_free(&list, NULL);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_zk_slist_for_each_when_func_is_null);
	RUN_TEST(test_zk_slist_for_each_when_list_is_null);
	RUN_TEST(test_zk_slist_for_each_when_func_is_not_null);
	return UNITY_END();
}
