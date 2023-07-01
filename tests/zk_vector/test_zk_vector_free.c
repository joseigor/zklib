#include "unity.h"
#include "zk/zklib.h"

void setUp(void) {}

void tearDown(void) {}

void test_zk_vector_free_should_free_a_vector_when_a_destructor_function_is_not_provided(void)
{
	zk_vector *vec = zk_vector_new();
	TEST_ASSERT_NOT_NULL(vec);
	zk_vector_free(vec, NULL);
}
void test_zk_vector_free_should_free_a_vector_when_a_destructor_function_is_provided(void)
{
	TEST_IGNORE_MESSAGE("Implement me!");
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_zk_vector_free_should_free_a_vector_when_a_destructor_function_is_not_provided);
	RUN_TEST(test_zk_vector_free_should_free_a_vector_when_a_destructor_function_is_provided);
	return UNITY_END();
}
