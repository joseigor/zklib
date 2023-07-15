#include <stdlib.h>

#include "unity.h"
#include "zk/zklib.h"

void setUp(void) {}
void tearDown(void) {}

void test_zk_vector_pop_back_when_size_is_zero(void)
{
	zk_vector *vec = zk_vector_new();
	zk_vector_pop_back(vec, NULL);
	TEST_ASSERT_EQUAL(0, zk_vector_size(vec));
	TEST_ASSERT_EQUAL(8, zk_vector_capacity(vec));
	zk_vector_free(vec, NULL);
}

void test_zk_vector_pop_back_when_size_is_one(void)
{
	zk_vector *vec = zk_vector_new();
	int *data = malloc(sizeof(int));
	*data = 1;
	zk_vector_push_back(vec, data);
	TEST_ASSERT_EQUAL(1, zk_vector_size(vec));
	zk_vector_pop_back(vec, free);
	TEST_ASSERT_EQUAL(0, zk_vector_size(vec));
	TEST_ASSERT_EQUAL(8, zk_vector_capacity(vec));
	zk_vector_free(vec, NULL);
}

void test_zk_vector_pop_back_when_size_big(void)
{
	zk_vector *vec = zk_vector_new();
	size_t elements = 100;
	for (size_t i = 0; i < elements; i++) {
		int *data = malloc(sizeof(int));
		*data = i;
		zk_vector_push_back(vec, data);
	}
	TEST_ASSERT_EQUAL(elements, zk_vector_size(vec));
	TEST_ASSERT_EQUAL(128, zk_vector_capacity(vec));

	for (size_t i = 0; i < elements; i++) {
		zk_vector_pop_back(vec, free);
		// check just last element is removed
		int **ptr = (int **)zk_vector_data(vec);
		for (size_t j = 0; j < zk_vector_size(vec); j++) {
			TEST_ASSERT_EQUAL(j, *ptr[j]);
		}
	}

	TEST_ASSERT_EQUAL(0, zk_vector_size(vec));
	TEST_ASSERT_EQUAL(128, zk_vector_capacity(vec));
	zk_vector_free(vec, NULL);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_zk_vector_pop_back_when_size_is_zero);
	RUN_TEST(test_zk_vector_pop_back_when_size_is_one);
	RUN_TEST(test_zk_vector_pop_back_when_size_big);
	return UNITY_END();
}
