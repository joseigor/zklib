#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "unity.h"
#include "zk/zklib.h"

typedef struct person {
	char *name;
	int age;
} person;

person *person_new(const char *name, int age)
{
	person *p = malloc(sizeof(person));
	p->name = strdup(name);
	p->age = age;
	return p;
}

void person_destructor_internal(void *data)
{
	person *p = data;
	free(p->name);
}

void person_destructor(person *p)
{
	person_destructor_internal(p);
	free(p);
}

void person_copy(person *src, person *dst)
{
	dst->name = strdup(src->name);
	dst->age = src->age;
}

void person_move(person *src, person *dst)
{
	dst->name = src->name;
	dst->age = src->age;
	free(src);
}

bool person_compare(person *p1, person *p2)
{
	return strcmp(p1->name, p2->name) == 0 && p1->age == p2->age;
}

void person_debug(const person p)
{
	printf("name: %s, age: %d\n", p.name, p.age);
}

void setUp(void) {}

void tearDown(void) {}

void test_zk_vector_push_back_no_resize_when_size_is_less_than_capacity_move(void)
{
	zk_vec(person) vec = zk_vec_new(person, person_destructor_internal, person_copy, person_move);

	// add 7 person to the vector (size = 7, capacity = 8) with move constructor
	for (int i = 0; i < 7; i++) {
		zk_vec_push_back_move(vec, person_new("person", i));
	}

	TEST_ASSERT_EQUAL(7, zk_vector_size(vec));
	TEST_ASSERT_EQUAL(8, zk_vector_capacity(vec));

	// compare the person in the vector with the expected person
	for (int i = 0; i < 7; i++) {
		person *expected = person_new("person", i);
		TEST_ASSERT(person_compare(&vec[i], expected));
		person_destructor(expected);
	}

	zk_vector_free(vec);
}

void test_zk_vector_push_back_resize_when_size_is_equal_to_capacity_move(void)
{
	zk_vec(person) vec = zk_vec_new(person, person_destructor_internal, person_copy, person_move);

	// add 8 person to the vector (size = 8, capacity = 8) with move constructor
	for (int i = 0; i < 8; i++) {
		zk_vec_push_back_move(vec, person_new("person", i));
	}

	TEST_ASSERT_EQUAL(8, zk_vector_size(vec));
	TEST_ASSERT_EQUAL(8, zk_vector_capacity(vec));

	// compare the person in the vector with the expected person
	for (int i = 0; i < 8; i++) {
		person *expected = person_new("person", i);
		TEST_ASSERT(person_compare(&vec[i], expected));
		person_destructor(expected);
	}

	// add 1 person to the vector (size = 9, capacity = 16) with move constructor
	zk_vec_push_back_move(vec, person_new("person", 8));

	TEST_ASSERT_EQUAL(9, zk_vector_size(vec));
	TEST_ASSERT_EQUAL(16, zk_vector_capacity(vec));

	// compare the person in the vector with the expected person
	for (int i = 0; i < 9; i++) {
		person *expected = person_new("person", i);
		TEST_ASSERT(person_compare(&vec[i], expected));
		person_destructor(expected);
	}

	zk_vector_free(vec);
}

void test_zk_vector_push_back_no_resize_when_size_is_less_than_capacity_copy(void)
{
	zk_vec(person) vec = zk_vec_new(person, person_destructor_internal, person_copy, person_move);

	// add 7 person to the vector (size = 7, capacity = 8) with move constructor
	for (int i = 0; i < 7; i++) {
		zk_vec(person) p = person_new("person", i);
		zk_vec_push_back_copy(vec, p);
		person_destructor(p); // free the person as only the copy is stored in the vector
	}

	TEST_ASSERT_EQUAL(7, zk_vector_size(vec));
	TEST_ASSERT_EQUAL(8, zk_vector_capacity(vec));

	// compare the person in the vector with the expected person
	for (int i = 0; i < 7; i++) {
		person *expected = person_new("person", i);
		TEST_ASSERT(person_compare(&vec[i], expected));
		person_destructor(expected);
	}

	zk_vector_free(vec);
}

void test_zk_vector_push_back_resize_when_size_is_equal_to_capacity_copy(void)
{
	zk_vec(person) vec = zk_vec_new(person, person_destructor_internal, person_copy, person_move);

	// add 8 person to the vector (size = 8, capacity = 8) with move constructor
	for (int i = 0; i < 8; i++) {
		zk_vec(person) p = person_new("person", i);
		zk_vec_push_back_copy(vec, p);
		person_destructor(p); // free the person as only the copy is stored in the vector
	}

	TEST_ASSERT_EQUAL(8, zk_vector_size(vec));
	TEST_ASSERT_EQUAL(8, zk_vector_capacity(vec));

	// compare the person in the vector with the expected person
	for (int i = 0; i < 8; i++) {
		person *expected = person_new("person", i);
		TEST_ASSERT(person_compare(&vec[i], expected));
		person_destructor(expected);
	}

	// add 1 person to the vector (size = 9, capacity = 16) with move constructor
	zk_vec(person) p = person_new("person", 8);
	zk_vec_push_back_copy(vec, p);
	person_destructor(p); // free the person as only the copy is stored in the vector

	TEST_ASSERT_EQUAL(9, zk_vector_size(vec));
	TEST_ASSERT_EQUAL(16, zk_vector_capacity(vec));

	// compare the person in the vector with the expected person
	for (int i = 0; i < 9; i++) {
		person *expected = person_new("person", i);
		TEST_ASSERT(person_compare(&vec[i], expected));
		person_destructor(expected);
	}

	zk_vector_free(vec);
}

void test_zk_vector_push_back_resize_when_size_is_equal_to_capacity_simple_type(void)
{
	zk_vec(int) vec = zk_vec_new(int, NULL, NULL, NULL);
	// add 100 int to the vector (size = 100, capacity = 128). As int is a simple type, no
	// destructor, copy or move function is needed and ZK_COPY and ZK_MOVE could be used
	for (int i = 0; i < 100; i++) {
		if (i % 2 == 0)
			zk_vec_push_back_copy(vec, &i);
		else
			zk_vec_push_back_move(vec, &i);
	}

	TEST_ASSERT_EQUAL(100, zk_vector_size(vec));
	TEST_ASSERT_EQUAL(128, zk_vector_capacity(vec));

	// compare the int in the vector with the expected int
	for (int i = 0; i < 100; i++) {
		int expected = i;
		TEST_ASSERT_EQUAL(expected, vec[i]);
	}

	zk_vector_free(vec);
}

void test_zk_vector_push_back_2_nested_vectors_move(void)
{
	zk_vec(zk_vec(int)) vec =
	        zk_vec_new(zk_vec(int), zk_vector_destructor, zk_vector_copy_constructor, zk_vector_move_constructor);

	zk_vec_push_back_move(vec, zk_vec_new(int, NULL, NULL, NULL));
	zk_vec_push_back_move(vec, zk_vec_new(int, NULL, NULL, NULL));
	zk_vec_push_back_move(vec, zk_vec_new(int, NULL, NULL, NULL));

	TEST_ASSERT_EQUAL(3, zk_vector_size(vec));
	TEST_ASSERT_EQUAL(8, zk_vector_capacity(vec));

	// check that nested vectors are initialized correctly
	for (int i = 0; i < 3; i++) {
		TEST_ASSERT_EQUAL(0, zk_vector_size(vec[i]));
		TEST_ASSERT_EQUAL(8, zk_vector_capacity(vec[i]));
	}

	// Operations on vec[0]
	// add 50 element to vec[0], expected size = 50, capacity = 64
	for (int i = 0; i < 50; i++)
		zk_vec_push_back_copy(vec[0], &i);
	// check that nested_vec[0] is resized
	TEST_ASSERT_EQUAL(50, zk_vector_size(vec[0]));
	TEST_ASSERT_EQUAL(64, zk_vector_capacity(vec[0]));
	// check that vec[0] is contains the expected elements
	for (int i = 0; i < 50; i++)
		TEST_ASSERT_EQUAL(i, vec[0][i]);

	// Operations on vec[1]
	// add 10 element to vec[1], expected size = 10, capacity = 16
	for (int i = 0; i < 10; i++)
		zk_vec_push_back_copy(vec[1], &i);
	// check that nested_vec[1] is resized
	TEST_ASSERT_EQUAL(10, zk_vector_size(vec[1]));
	TEST_ASSERT_EQUAL(16, zk_vector_capacity(vec[1]));
	// check that vec[1] is contains the expected elements
	for (int i = 0; i < 10; i++)
		TEST_ASSERT_EQUAL(i, vec[1][i]);

	// Operations on vec[2]
	// add 100 element to vec[2], expected size = 100, capacity = 128
	for (int i = 0; i < 100; i++)
		zk_vec_push_back_copy(vec[2], &i);
	// check that nested_vec[2] is resized
	TEST_ASSERT_EQUAL(100, zk_vector_size(vec[2]));
	TEST_ASSERT_EQUAL(128, zk_vector_capacity(vec[2]));
	// check that vec[2] is contains the expected elements
	for (int i = 0; i < 100; i++)
		TEST_ASSERT_EQUAL(i, vec[2][i]);

	// Add an empty vector to vec[3]
	zk_vec_push_back_move(vec, zk_vec_new(int, NULL, NULL, NULL));
	TEST_ASSERT_EQUAL(4, zk_vector_size(vec));
	TEST_ASSERT_EQUAL(8, zk_vector_capacity(vec));
	TEST_ASSERT_EQUAL(0, zk_vector_size(vec[3]));
	TEST_ASSERT_EQUAL(8, zk_vector_capacity(vec[3]));

	// print all vectors nicely
	// v[i]
	//  v[i][j]
	// uncomment the following code to print all vectors nicely
	// for (size_t i = 0; i < zk_vector_size(vec); i++) {
	// 	printf("vec[%ld]\n", i);
	// 	for (size_t j = 0; j < zk_vector_size(vec[i]); j++) {
	// 		printf(" vec[%ld][%ld] = %d\n", i, j, vec[i][j]);
	// 	}
	// }

	zk_vector_free(vec);
}

void test_zk_vector_push_back_nested_3_vectors_move(void)
{
	zk_vec(zk_vec(zk_vec(int))) vec = zk_vec_new(zk_vec(zk_vec(int)),
	                                             zk_vector_destructor,
	                                             zk_vector_copy_constructor,
	                                             zk_vector_move_constructor);

	//  move 3 to vec 3 nested zk_vec(zk_vec(int))
	zk_vec_push_back_move(
	        vec,
	        zk_vec_new(zk_vec(int), zk_vector_destructor, zk_vector_copy_constructor, zk_vector_move_constructor));
	zk_vec_push_back_move(
	        vec,
	        zk_vec_new(zk_vec(int), zk_vector_destructor, zk_vector_copy_constructor, zk_vector_move_constructor));
	zk_vec_push_back_move(
	        vec,
	        zk_vec_new(zk_vec(int), zk_vector_destructor, zk_vector_copy_constructor, zk_vector_move_constructor));

	TEST_ASSERT_EQUAL(3, zk_vector_size(vec));
	TEST_ASSERT_EQUAL(8, zk_vector_capacity(vec));

	// check that nested vectors are initialized correctly
	for (size_t i = 0; i < 3; i++) {
		TEST_ASSERT_EQUAL(0, zk_vector_size(vec[i]));
		TEST_ASSERT_EQUAL(8, zk_vector_capacity(vec[i]));
	}

	// Operations on vec[0]
	// add 1 element to vec[0], expected size = 1, capacity = 8
	zk_vec_push_back_move(vec[0], zk_vec_new(int, NULL, NULL, NULL));
	// check that vec[0] has the expected size and capacity
	TEST_ASSERT_EQUAL(1, zk_vector_size(vec[0]));
	TEST_ASSERT_EQUAL(8, zk_vector_capacity(vec[0]));

	// Operations on vec[1]
	// add 10 element to vec[1], expected size = 10, capacity = 16
	for (size_t i = 0; i < 10; i++)
		zk_vec_push_back_move(vec[1], zk_vec_new(int, NULL, NULL, NULL));
	// check that vec[1] has the expected size and capacity
	TEST_ASSERT_EQUAL(10, zk_vector_size(vec[1]));
	TEST_ASSERT_EQUAL(16, zk_vector_capacity(vec[1]));

	// Operations on vec[2]
	// add 100 element to vec[2], expected size = 100, capacity = 128
	for (size_t i = 0; i < 100; i++)
		zk_vec_push_back_move(vec[2], zk_vec_new(int, NULL, NULL, NULL));
	// check that vec[2] has the expected size and capacity
	TEST_ASSERT_EQUAL(100, zk_vector_size(vec[2]));
	TEST_ASSERT_EQUAL(128, zk_vector_capacity(vec[2]));

	// Operations on vec[i][x]
	// add 10 element to vec[i][x], expected size = 10, capacity = 16
	for (size_t i = 0; i < zk_vector_size(vec); i++)
		for (size_t j = 0; j < zk_vector_size(vec[i]); j++)
			for (size_t k = 0; k < 10; k++)
				zk_vec_push_back_copy(vec[i][j], &k);

	// check that vec[i][x] has the expected size and capacity
	for (size_t i = 0; i < zk_vector_size(vec); i++) {
		for (size_t j = 0; j < zk_vector_size(vec[i]); j++) {
			TEST_ASSERT_EQUAL(10, zk_vector_size(vec[i][j]));
			TEST_ASSERT_EQUAL(16, zk_vector_capacity(vec[i][j]));
		}
	}
	// print all vectors nicely
	// v[i]
	//  v[i][j]
	//   v[i][j][k]
	// uncomment the following code to print all vectors nicely
	// for (size_t i = 0; i < zk_vector_size(vec); i++) {
	// 	printf("vec[%ld]\n", i);
	// 	for (size_t j = 0; j < zk_vector_size(vec[i]); j++) {
	// 		printf(" vec[%ld][%ld]\n", i, j);
	// 		for (size_t k = 0; k < zk_vector_size(vec[i][j]); k++) {
	// 			printf("  vec[%ld][%ld][%ld] = %d\n", i, j, k, vec[i][j][k]);
	// 		}
	// 	}
	// }

	zk_vector_free(vec);
}
int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_zk_vector_push_back_no_resize_when_size_is_less_than_capacity_move);
	RUN_TEST(test_zk_vector_push_back_resize_when_size_is_equal_to_capacity_move);
	RUN_TEST(test_zk_vector_push_back_no_resize_when_size_is_less_than_capacity_copy);
	RUN_TEST(test_zk_vector_push_back_resize_when_size_is_equal_to_capacity_copy);
	RUN_TEST(test_zk_vector_push_back_resize_when_size_is_equal_to_capacity_simple_type);
	RUN_TEST(test_zk_vector_push_back_2_nested_vectors_move);
	RUN_TEST(test_zk_vector_push_back_nested_3_vectors_move);
	return UNITY_END();
}
