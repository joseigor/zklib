#include "unity.h"
#include "zk/zklib.h"

zk_vector *vec = NULL;

void setUp(void)
{
	vec = zk_vector_new();
	if (vec == NULL) {
		TEST_FAIL_MESSAGE("Failed to allocate memory for zk_vector");
	}
}

void tearDown(void)
{
	zk_vector_free(vec, NULL);
}

void test_zk_vector_push_front_when_no_resize_when_size_is_less_than_capacity(void)
{
	int data = 1;
	zk_vector_push_front(vec, &data);
	TEST_ASSERT_EQUAL(1, zk_vector_size(vec));
	TEST_ASSERT_EQUAL(8, zk_vector_capacity(vec));
	int **ptr = (int **)zk_vector_data(vec);
	TEST_ASSERT_EQUAL(data, *ptr[0]);
}

void test_zk_vector_push_front_shall_increment_size()
{
	int data = 1;
	zk_vector_push_front(vec, &data);
	TEST_ASSERT_EQUAL(1, zk_vector_size(vec));
	TEST_ASSERT_EQUAL(8, zk_vector_capacity(vec));
	int **ptr = (int **)zk_vector_data(vec);
	TEST_ASSERT_EQUAL(data, *ptr[0]);
}

void test_zk_vector_push_front_when_resize_when_size_is_equal_to_capacity(void)
{
	int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
	for (int i = 0; i < 8; i++)
		zk_vector_push_front(vec, &data[i]);
	TEST_ASSERT_EQUAL(8, zk_vector_size(vec));
	TEST_ASSERT_EQUAL(8, zk_vector_capacity(vec));
	zk_vector_push_front(vec, &data[8]);
	TEST_ASSERT_EQUAL(9, zk_vector_size(vec));
	TEST_ASSERT_EQUAL(16, zk_vector_capacity(vec));
	int **ptr = (int **)zk_vector_data(vec);
	for (int i = 0; i < 9; i++)
		TEST_ASSERT_EQUAL(data[8 - i], *ptr[i]);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_zk_vector_push_front_when_no_resize_when_size_is_less_than_capacity);
	RUN_TEST(test_zk_vector_push_front_shall_increment_size);
	RUN_TEST(test_zk_vector_push_front_when_resize_when_size_is_equal_to_capacity);
	return UNITY_END();
}
