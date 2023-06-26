#include <stdlib.h>
#include <string.h>

#include "unity.h"
#include "zk/zklib.h"

void setUp(void) {}

void tearDown(void) {}

void test_zk_slist_pop_front_when_list_is_empty(void)
{
	zk_slist *list = NULL;
	list = zk_slist_pop_front(list, NULL);
	TEST_ASSERT_NULL(list);
}

void test_zk_slist_pop_front_when_list_has_1_element(void)
{
	zk_slist *list = NULL;
	char *data[] = { "0" };
	list = zk_slist_push_back(list, data[0]);

	list = zk_slist_pop_front(list, NULL);
	TEST_ASSERT_NULL(list);

	// no need to call zk_slist_free() as list is freed by now
}

void test_zk_slist_pop_front_when_list_has_2_elements(void)
{
	zk_slist *list = NULL;
	char *data[] = { "0", "1" };

	list = zk_slist_push_back(list, data[0]);
	list = zk_slist_push_back(list, data[1]);

	list = zk_slist_pop_front(list, NULL);

	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_NULL(list->next);

	TEST_ASSERT_EQUAL_STRING(data[1], (char *)list->data);

	zk_slist_free(&list, NULL);
}

void test_zk_slist_pop_front_when_list_has_3_elements(void)
{
	zk_slist *list = NULL, *next = NULL;

	list = zk_slist_push_back(list, "0");
	list = zk_slist_push_back(list, "1");
	list = zk_slist_push_back(list, "2");

	list = zk_slist_pop_front(list, NULL);

	TEST_ASSERT_NOT_NULL(list);
	TEST_ASSERT_EQUAL_STRING("1", (char *)list->data);

	next = list->next;
	TEST_ASSERT_NOT_NULL(next);
	TEST_ASSERT_EQUAL_STRING("2", (char *)next->data);

	next = next->next;
	TEST_ASSERT_NULL(next);

	zk_slist_free(&list, NULL);
}

void test_zk_slist_pop_front_when_list_has_n_elements(void)
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
		TEST_ASSERT_EQUAL_STRING(data[i++], (char *)list->data);
		list = zk_slist_pop_front(list, NULL);
	}
	// no need to call zk_slist_free() as list is freed by now
}

void test_zk_slist_pop_front_with_free_func_when_list_has_n_elements(void)
{
	zk_slist *list = NULL;
	for (size_t i = 0; i < 10; i++) {
		list = zk_slist_push_back(list, strdup("data"));
	}

	while (list != NULL) {
		TEST_ASSERT_NOT_NULL(list);
		TEST_ASSERT_EQUAL_STRING("data", (char *)list->data);
		list = zk_slist_pop_front(list, free);
	}

	// no need to call zk_slist_free() as list is freed by now
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_zk_slist_pop_front_when_list_is_empty);
	RUN_TEST(test_zk_slist_pop_front_when_list_has_1_element);
	RUN_TEST(test_zk_slist_pop_front_when_list_has_2_elements);
	RUN_TEST(test_zk_slist_pop_front_when_list_has_3_elements);
	RUN_TEST(test_zk_slist_pop_front_when_list_has_n_elements);
	RUN_TEST(test_zk_slist_pop_front_with_free_func_when_list_has_n_elements);
	return UNITY_END();
}
