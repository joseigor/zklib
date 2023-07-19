#include "unity.h"
#include "zk/zklib.h"

void setUp(void) {}

void tearDown(void) {}

void test_zk_vector_new_should_return_a_vector_with_size_0_and_capacity_8(void)
{
	zk_vec(int) vec = zk_vec_new(int, NULL, NULL, NULL);
	TEST_ASSERT_NOT_NULL(vec);
	TEST_ASSERT_EQUAL(0, zk_vector_size(vec));
	TEST_ASSERT_EQUAL(8, zk_vector_capacity(vec));
	zk_vector_free(vec);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_zk_vector_new_should_return_a_vector_with_size_0_and_capacity_8);
	return UNITY_END();
}
