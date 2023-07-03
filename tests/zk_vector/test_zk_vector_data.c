#include "unity.h"
#include "zk/zklib.h"

void setUp(void) {}

void tearDown(void) {}

void test_zk_vector_data_should_return_to_the_underlying_array(void)
{
	zk_vector *vec = zk_vector_new();
	TEST_ASSERT_NOT_NULL(vec);
	int data = 1;
	zk_vector_push_back(vec, &data);
	int *ptr = zk_vector_data(vec);
	TEST_ASSERT_EQUAL(1, *ptr);
	zk_vector_free(vec, NULL);
}

void test_zk_vector_data_should_return_NULL_when_vector_is_empty(void)
{
	zk_vector *vec = zk_vector_new();
	TEST_ASSERT_NOT_NULL(vec);
	TEST_ASSERT_NULL(zk_vector_data(vec));
	zk_vector_free(vec, NULL);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_zk_vector_data_should_return_to_the_underlying_array);
	RUN_TEST(test_zk_vector_data_should_return_NULL_when_vector_is_empty);
	return UNITY_END();
}
