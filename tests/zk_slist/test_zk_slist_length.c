#include "unity.h"
#include "zk/zklib.h"

void setUp(void) {}

void tearDown(void) {}

void test_length_when_list_is_null(void)
{
	zk_slist *list = NULL;
	TEST_ASSERT_EQUAL(0, zk_length(list));
}

void test_length_when_list_has_one_element(void)
{
	zk_slist *list = NULL;
	int data = 1;

	zk_push_front(&list, &data);

	TEST_ASSERT_EQUAL(1, zk_length(list));

	zk_free(&list, NULL);
}

void test_length_when_list_has_multiple_elements(void)
{
	zk_slist *list = NULL;
	int data[] = { 1, 2, 3, 4, 5 };

	for (int i = 0; i < 5; i++)
		zk_push_front(&list, &data[i]);

	TEST_ASSERT_EQUAL(5, zk_length(list));

	zk_free(&list, NULL);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_length_when_list_is_null);
	RUN_TEST(test_length_when_list_has_one_element);
	RUN_TEST(test_length_when_list_has_multiple_elements);
	return UNITY_END();
}
