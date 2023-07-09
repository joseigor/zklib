#include "common/person.h"
#include "unity.h"
#include "zk/zklib.h"

person_t *persons = NULL;

void setUp(void)
{
	persons = zk_vector_new(sizeof(struct person),
	                    (zk_destructor_t)person_free_internal,
	                    (zk_copy_t)person_copy,
	                    (zk_move_t)person_move);
	if (persons == NULL) {
		TEST_FAIL_MESSAGE("Failed to allocate memory for zk_vector");
	}
}

void tearDown(void)
{
	zk_vector_free(persons);
}

void test_zk_vector_push_back_when_no_resize_when_size_is_less_than_capacity(void)
{
	person_t *person_1 = person_new("Kelly", 25);
	person_set_address(person_1, "123 Main St", "10", "1030", "Amsterdam", "Provincie Noord-Holland", "Nederland");
	person_t *person_2 = person_new("Jose", 30);
	person_set_address(person_2, "456 Main St", "20", "1030", "Amsterdam", "Provincie Noord-Holland", "Nederland");

	zk_vector_push_back((void**)&persons, ZK_COPY(person_1));
	TEST_ASSERT_EQUAL(1, zk_vector_size(persons));
	TEST_ASSERT_EQUAL(8, zk_vector_capacity(persons));
	zk_vector_push_back((void**)&persons, ZK_COPY(person_2));
	TEST_ASSERT_EQUAL(2, zk_vector_size(persons));
	TEST_ASSERT_EQUAL(8, zk_vector_capacity(persons));

	// we need to free person_1 and person_2 because we used ZK_COPY to copy them into the vector
	person_free(person_1);
	person_free(person_2);
}

// void test_zk_vector_push_back_capacity_is_expanded_when_size_is_equal_to_capacity(void)
// {
// 	person_t *person_1 = person_new("Kelly", 25);
// 	person_set_address(person_1, "123 Main St", "10", "1030", "Amsterdam", "Provincie Noord-Holland", "Nederland");
// 	person_t *person_2 = person_new("Jose", 30);
// 	person_set_address(person_2, "456 Main St", "20", "1030", "Amsterdam", "Provincie Noord-Holland", "Nederland");

// 	for (int i = 0; i < 8; i++) {
// 		zk_vector_push_back(vec, ZK_COPY(person_1));
// 	}
// 	TEST_ASSERT_EQUAL(8, zk_vector_size(vec));
// 	TEST_ASSERT_EQUAL(8, zk_vector_capacity(vec));

// 	zk_vector_push_back(vec, ZK_COPY(person_2));
// 	TEST_ASSERT_EQUAL(9, zk_vector_size(vec));
// 	TEST_ASSERT_EQUAL(16, zk_vector_capacity(vec));

// 	// we need to free person_1 and person_2 because we used ZK_COPY to copy them into the vector
// 	person_free(person_1);
// 	person_free(person_2);
// }

// void test_zk_vector_push_back_move_elements_into_vector(void)
// {
// 	person_t *person_1 = person_new("Kelly", 25);
// 	person_set_address(person_1, "123 Main St", "10", "1030", "Amsterdam", "Provincie Noord-Holland", "Nederland");
// 	person_t *person_2 = person_new("Jose", 30);
// 	person_set_address(person_2, "456 Main St", "20", "1030", "Amsterdam", "Provincie Noord-Holland", "Nederland");

// 	zk_vector_push_back(vec, ZK_MOVE(person_1));
// 	TEST_ASSERT_EQUAL(1, zk_vector_size(vec));
// 	TEST_ASSERT_EQUAL(8, zk_vector_capacity(vec));

// 	zk_vector_push_back(vec, ZK_MOVE(person_2));
// 	TEST_ASSERT_EQUAL(2, zk_vector_size(vec));
// 	TEST_ASSERT_EQUAL(8, zk_vector_capacity(vec));

// 	// create another person and move it into the vector
// 	zk_vector_push_back(vec, ZK_MOVE(person_new("John", 35)));
// 	TEST_ASSERT_EQUAL(3, zk_vector_size(vec));
// 	TEST_ASSERT_EQUAL(8, zk_vector_capacity(vec));

// 	// we don't need to free person_1 and person_2 because we used ZK_MOVE to move them into the vector
// }

// void test_zk_vector_push_back_elements_are_kept_in_correct_order(void)
// {
// 	zk_vector *int_vec = zk_vector_new(sizeof(int), NULL, NULL, NULL);
// 	for (int i = 0; i < 100; i++)
// 		zk_vector_push_back(int_vec, ZK_COPY(&i));

// 	TEST_ASSERT_EQUAL(100, zk_vector_size(int_vec));
// 	TEST_ASSERT_EQUAL(128, zk_vector_capacity(int_vec));

// 	int *data = zk_vector_data(int_vec);

// 	// checks that the elements are in the correct order and that they are all there
// 	for (int i = 0; i < 100; i++)
// 		TEST_ASSERT_EQUAL(i, data[i]);

// 	zk_vector_free(int_vec);
// }

// void test_zk_vector_push_back_when_invalid_constructor_mode_is_provided_vector_is_not_modified(void)
// {
// 	person_t *person_1 = person_new("Kelly", 25);
// 	person_set_address(person_1, "123 Main St", "10", "1030", "Amsterdam", "Provincie Noord-Holland", "Nederland");

// 	zk_vector_push_back(vec, person_1, 3);
// 	TEST_ASSERT_EQUAL(0, zk_vector_size(vec));
// 	TEST_ASSERT_EQUAL(8, zk_vector_capacity(vec));

// 	// we need to free person_1 as we didn't use ZK_COPY or ZK_MOVE to copy or move it into the vector
// 	person_free(person_1);
// }

// void test_vec_of_vecs(void)
// {
// 	// create a vector of 100 ints
// 	zk_vector *int_vec = zk_vector_new(sizeof(int), NULL, NULL, NULL);
// 	for (int i = 0; i < 100; i++)
// 		zk_vector_push_back(int_vec, ZK_COPY(&i));

// 	// create another vector of 100 ints from 100 to 199
// 	zk_vector *int_vec_2 = zk_vector_new(sizeof(int), NULL, NULL, NULL);
// 	for (int i = 100; i < 200; i++)
// 		zk_vector_push_back(int_vec_2, ZK_COPY(&i));

// 	// create a vector of int vectors
// 	zk_vector *vec_of_vecs = zk_vector_new(sizeof(zk_vector *), (zk_destructor_t)zk_vector_free, NULL, NULL);
// 	zk_vector_push_back(vec_of_vecs, ZK_MOVE(int_vec));
// 	zk_vector_push_back(vec_of_vecs, ZK_MOVE(int_vec_2));

// 	// get data
// 	zk_vector **data = zk_vector_data(vec_of_vecs);
// 	// get the first vector

// 	// print the first vector
// 	int *int_data = zk_vector_data(data[0]);
// 	size_t size = zk_vector_size(data[0]);
// 	for (size_t i = 0; i < size; i++)
// 		printf("%d\n", int_data[i]);

// 	// print the second vector
// 	int_data = zk_vector_data(data[1]);
// 	size = zk_vector_size(data[1]);
// 	for (size_t i = 0; i < size; i++)
// 		printf("%d\n", int_data[i]);

// 	zk_vector_free(vec_of_vecs);
// 	// zk_vector_free(int_vec);
// 	// zk_vector_free(int_vec_2);
// }


// void test_vec_of_vecs_of_vecs(void)
// {
// 	// create a vector of 100 ints
// 	zk_vector *int_vec = zk_vector_new(sizeof(int), NULL, NULL, NULL);
// 	for (int i = 0; i < 100; i++)
// 		zk_vector_push_back(int_vec, ZK_COPY(&i));

// 	// create another vector of 50 ints from 100 to 149
// 	zk_vector *int_vec_2 = zk_vector_new(sizeof(int), NULL, NULL, NULL);
// 	for (int i = 100; i < 150; i++)
// 		zk_vector_push_back(int_vec_2, ZK_COPY(&i));

// 	// create a vector of int vectors
// 	zk_vector *vec_of_vecs = zk_vector_new(sizeof(zk_vector **), (zk_destructor_t)zk_vector_free, NULL, NULL);
// 	zk_vector_push_back(vec_of_vecs, ZK_MOVE(int_vec));
// 	zk_vector_push_back(vec_of_vecs, ZK_MOVE(int_vec_2));

// 	// create a vector of vec_of_vecs
// 	zk_vector *vec_of_vecs_of_vecs =
// 	        zk_vector_new(sizeof(zk_vector ***), (zk_destructor_t)zk_vector_free, NULL, NULL);
// 	zk_vector_push_back(vec_of_vecs_of_vecs, ZK_MOVE(vec_of_vecs));

// 	// get data of vec_vec_vec
// 	zk_vector **vvv_data = zk_vector_data(vec_of_vecs_of_vecs);

// 	// get the vec of vecs
// 	zk_vector **vv_data = zk_vector_data(vvv_data[0]);

// 	// get the first vector of ints
// 	// zk_vector *v_data = zk_vector_data(vv_data[0]); // this is the first vector of ints

// 	// print the first vector
// 	int *int_data = zk_vector_data(vv_data[0]); // this is the first vector of ints
// 	size_t size = zk_vector_size(vv_data[0]);
// 	for (size_t i = 0; i < size; i++)
// 		printf("%d\n", int_data[i]);

// 	// print the second vector
// 	int_data = zk_vector_data(vv_data[1]); // this is the second vector of ints
// 	size = zk_vector_size(vv_data[1]);
// 	for (size_t i = 0; i < size; i++)
// 		printf("%d\n", int_data[i]);

// 	// add more elements to the second vector of ints
// 	for (int i = 150; i < 200; i++)
// 		zk_vector_push_back(vv_data[1], ZK_COPY(&i));

// 	// print the second vector again
// 	int_data = zk_vector_data(vv_data[1]); // this is the second vector of ints
// 	size = zk_vector_size(vv_data[1]);
// 	printf("Printing second vector again\n");
// 	for (size_t i = 0; i < size; i++)
// 		printf("%d\n", int_data[i]);

// 	zk_vector_free(vec_of_vecs_of_vecs);
// 	// zk_vector_free(int_vec);
// 	// zk_vector_free(int_vec_2);
// }

int main(void)
{
	UNITY_BEGIN();
	// RUN_TEST(test_zk_vector_push_back_when_no_resize_when_size_is_less_than_capacity);
	// RUN_TEST(test_zk_vector_push_back_capacity_is_expanded_when_size_is_equal_to_capacity);
	// RUN_TEST(test_zk_vector_push_back_move_elements_into_vector);
	// RUN_TEST(test_zk_vector_push_back_elements_are_kept_in_correct_order);
	// RUN_TEST(test_zk_vector_push_back_when_invalid_constructor_mode_is_provided_vector_is_not_modified);
	// RUN_TEST(test_vec_of_vecs);
	// RUN_TEST(test_vec_of_vecs_of_vecs);
	return UNITY_END();
}
