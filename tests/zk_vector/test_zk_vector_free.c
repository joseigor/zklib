#include <stdlib.h>

#include "common/person.h"
#include "unity.h"
#include "zk/zklib.h"

void setUp(void) {}

void tearDown(void) {}

void test_zk_vector_free_should_free_a_vector_when_vector_is_null(void)
{
	zk_vector_free(NULL);
}

void test_zk_vector_free_should_free_a_vector_when_vector_contains_elements(void)
{
	zk_vector *vec = zk_vector_new(sizeof(person_t),
	                               (zk_destructor_t)person_free_internal,
	                               (zk_copy_t)person_copy,
	                               (zk_move_t)person_move);
	person_t *person = person_new("Kelly", 25);

	zk_vector_push_back(vec, ZK_COPY(person));
	TEST_ASSERT_EQUAL(1, zk_vector_size(vec));
	TEST_ASSERT_EQUAL(8, zk_vector_capacity(vec));

	// create and move 10 elements
	for (int i = 0; i < 10; i++)
		zk_vector_push_back(vec, ZK_MOVE(person_new("Jose", i)));

	TEST_ASSERT_EQUAL(11, zk_vector_size(vec));
	TEST_ASSERT_EQUAL(16, zk_vector_capacity(vec));

	zk_vector_free(vec);
	person_free(person);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_zk_vector_free_should_free_a_vector_when_vector_is_null);
	RUN_TEST(test_zk_vector_free_should_free_a_vector_when_vector_contains_elements);
	return UNITY_END();
}
