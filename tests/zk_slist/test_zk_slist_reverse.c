#include "unity.h"
#include "zk/zklib.h"

void setUp(void) {}

void tearDown(void) {}

void test_reverse_when_list_is_null(void)
{
	zk_slist *list = NULL;
	TEST_ASSERT_NULL(zk_slist_reverse(list));
}

void test_reverse_when_list_has_one_element(void)
{
	zk_slist *list = NULL, *next = NULL;
	int data = 1;

	list = zk_slist_push_front(list, &data);
	list = zk_slist_reverse(list);

	TEST_ASSERT_NOT_NULL(list);

	// check reversed list still has one element and it is the same as the original one
	TEST_ASSERT_EQUAL_PTR(&data, list->data);
	TEST_ASSERT_EQUAL(data, *(int *)list->data);
	// next element is always NULL for singly linked list with one element
	zk_next(list, &next);
	TEST_ASSERT_NULL(next);

	zk_slist_free(&list, NULL);
}

void test_reverse_when_list_has_multiple_elements(void)
{
	zk_slist *list = NULL;
	int data[] = { 1, 2, 3, 4, 5 };

	for (int i = 0; i < 5; i++)
		list = zk_slist_push_front(list, &data[i]);

	// reverse the list
	list = zk_slist_reverse(list);
	TEST_ASSERT_NOT_NULL(list);

	zk_slist *tmp = list;
	int i = 0;
	// check reversed list still has 5 elements and they are the same as the original ones
	while (tmp != NULL) {
		TEST_ASSERT_EQUAL_PTR(&data[i], tmp->data);
		TEST_ASSERT_EQUAL(data[i], *(int *)tmp->data);
		tmp = tmp->next;
		i++;
	}

	zk_slist_free(&list, NULL);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_reverse_when_list_is_null);
	RUN_TEST(test_reverse_when_list_has_one_element);
	RUN_TEST(test_reverse_when_list_has_multiple_elements);
	return UNITY_END();
}
