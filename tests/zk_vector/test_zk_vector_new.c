#include "common/person.h"
#include "unity.h"
#include "zk/zklib.h"

void setUp(void) {}

void tearDown(void) {}

void test_zk_vector_new_should_return_a_vector_with_size_0_and_capacity_8(void)
{
	zk_vector *vec = zk_vector_new(sizeof(person_t),
	                               (zk_destructor_t)person_free,
	                               (zk_copy_t)person_copy,
	                               (zk_move_t)person_move);
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
