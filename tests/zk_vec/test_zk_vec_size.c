#include "unity.h"
#include "zk/zklib.h"

void setUp(void) {}

void tearDown(void) {}

void test_zk_vector_size_should_return_the_size_of_a_vector(void)
{
	zk_vector *vec = zk_vector_new();
	TEST_ASSERT_NOT_NULL(vec);
	TEST_ASSERT_EQUAL(0, zk_vector_size(vec));
	zk_vector_free(vec, NULL);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_zk_vector_size_should_return_the_size_of_a_vector);
	return UNITY_END();
}
