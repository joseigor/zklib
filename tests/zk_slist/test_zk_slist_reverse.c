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

void test_reverse_when_list_is_null(void)
{
	zk_slist *list = NULL;
	TEST_ASSERT_EQUAL(ZK_INVALID_ARGUMENT, zk_reverse(&list));
}

void test_reverse_when_list_has_one_element(void)
{
	zk_slist *list = NULL, *next = NULL;
	void *node_data = NULL;
	int data = 1;

	zk_push_front(&list, &data);

	TEST_ASSERT_EQUAL(ZK_OK, zk_reverse(&list));

	// check reversed list still has one element and it is the same as the original one
	zk_get_data(list, &node_data);
	TEST_ASSERT_EQUAL_PTR(&data, node_data);
	TEST_ASSERT_EQUAL(data, *(int *)node_data);
	// next element is always NULL for singly linked list with one element
	zk_next(list, &next);
	TEST_ASSERT_NULL(next);

	zk_free(&list, NULL);
}

void test_reverse_when_list_has_multiple_elements(void)
{
	zk_slist *list = NULL;
	int data[] = { 1, 2, 3, 4, 5 };

	for (int i = 0; i < 5; i++)
		zk_push_front(&list, &data[i]);

	// reverse the list
	TEST_ASSERT_EQUAL(ZK_OK, zk_reverse(&list));

	zk_slist *tmp = list;
	int i = 0;
	// check reversed list still has 5 elements and they are the same as the original ones
	while (tmp != NULL) {
		void *node_data = NULL;
		zk_get_data(tmp, &node_data);
		TEST_ASSERT_EQUAL_PTR(&data[i], node_data);
		TEST_ASSERT_EQUAL(data[i], *(int *)node_data);
		zk_next(tmp, &tmp);
		i++;
	}

	zk_free(&list, NULL);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_reverse_when_list_is_null);
	RUN_TEST(test_reverse_when_list_has_one_element);
	RUN_TEST(test_reverse_when_list_has_multiple_elements);
	return UNITY_END();
}
