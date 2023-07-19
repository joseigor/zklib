#include <stdio.h>

#include "unity.h"
#include "zk/zklib.h"

void setUp(void) {}

void tearDown(void) {}

void test_zk_vec_begin_end_should_return_the_same_pointer_when_the_vector_is_empty(void)
{
	zk_vec(int) vec = zk_vec_new(int, NULL, NULL, NULL);
	TEST_ASSERT_EQUAL_PTR(zk_vec_begin(vec), zk_vec_end(vec));
	zk_vector_free(vec);
}

void test_zk_vec_begin_end_should_allocate_extra_memory_for_the_end_iterator(void)
{
	zk_vec(int) vec = zk_vec_new(int, NULL, NULL, NULL);
	// We need to make sure that when vector is resized, there will be enough space for the end iterator
	// to be allocated. Initially, the vector has capacity of 8 elements, so we need to add 9 elements
	// to hold a vector of size 8 and an end iterator. When the vector is resized, the capacity will be
	// doubled, so we need to add 17 elements to hold a vector of size 16 and an end iterator and so on.

	// Add 8 elements to the vector and check that end iterator is properly allocated
	for (int i = 0; i < 8; i++)
		zk_vec_push_back_copy(vec, &i);

	// iterate over the vector using the begin and end iterators
	int i = 0;
	for (zk_vec(int) it = zk_vec_begin(vec); it != zk_vec_end(vec); ++it, ++i) {
		TEST_ASSERT_EQUAL(*it, i);
	}

	// Force a resize of the vector and add 8 more elements. This should allocate enough memory for the
	// end iterator.
	for (int i = 8; i < 16; i++)
		zk_vec_push_back_copy(vec, &i);

	// iterate over the vector using the begin and end iterators
	i = 0;
	for (zk_vec(int) it = zk_vec_begin(vec); it != zk_vec_end(vec); ++it, ++i)
		TEST_ASSERT_EQUAL(*it, i);

	zk_vector_free(vec);
}

void test_zk_vec_begin_end_use_iterator_to_modify_vector(void)
{
	zk_vec(int) *vec = zk_vec_new(int, NULL, NULL, NULL);
	for (int i = 0; i < 8; i++)
		zk_vec_push_back_copy(vec, &i);

	// add 10 to each element of the vector
	zk_vec(int) it = zk_vec_begin(vec);
	for (; it != zk_vec_end(vec); ++it)
		*it += 10;

	// iterate over the vector using the begin and end iterators and check that each element has been modified
	// correctly
	int i = 10;
	for (zk_vec(int) it = zk_vec_begin(vec); it != zk_vec_end(vec); ++it, ++i)
		TEST_ASSERT_EQUAL(*it, i);

	zk_vector_free(vec);
}

void test_zk_vec_begin_end_use_iterator_in_reverse_order(void)
{
	zk_vec(int) *vec = zk_vec_new(int, NULL, NULL, NULL);
	for (int i = 0; i < 8; i++)
		zk_vec_push_back_copy(vec, &i);

	// add 10 to each element of the vector
	zk_vec(int) it = zk_vec_end(vec);

	while (it != zk_vec_begin(vec)) {
		--it;
		*it += 10;
	}

	// iterate over the vector using the begin and end iterators and check that each element has been modified
	// correctly
	int i = 10;
	for (zk_vec(int) it = zk_vec_begin(vec); it != zk_vec_end(vec); ++it, ++i) {
		TEST_ASSERT_EQUAL(*it, i);
	}

	zk_vector_free(vec);
}

void test_zk_vec_begin_end_iterate_over_big_vector(void)
{
	zk_vec(int) vec = zk_vec_new(int, NULL, NULL, NULL);

	// add 1000 elements to the vector
	for (int i = 0; i < 1000; i++)
		zk_vec_push_back_copy(vec, &i);

	// iterate over the vector using the begin and end iterators
	int i = 0;
	for (zk_vec(int) it = zk_vec_begin(vec); it != zk_vec_end(vec); ++it, ++i)
		TEST_ASSERT_EQUAL(*it, i);

	zk_vector_free(vec);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_zk_vec_begin_end_should_return_the_same_pointer_when_the_vector_is_empty);
	RUN_TEST(test_zk_vec_begin_end_should_allocate_extra_memory_for_the_end_iterator);
	RUN_TEST(test_zk_vec_begin_end_use_iterator_to_modify_vector);
	RUN_TEST(test_zk_vec_begin_end_use_iterator_in_reverse_order);
	RUN_TEST(test_zk_vec_begin_end_iterate_over_big_vector);
	return UNITY_END();
}
