#include <stdlib.h>

#include "unity.h"
#include "zk/zklib.h"

typedef struct person {
	char *name;
	int age;
} person;

void person_destructor(void *data)
{
	person *p = data;
	free(p->name);
	free(p);
}

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
	zk_vector *vec = zk_vector_new();
	// create 100 persons and push them to the vector
	for (int i = 0; i < 100; i++) {
		person *p = malloc(sizeof(person));
		p->name = malloc(10);
		sprintf(p->name, "person%d", i);
		p->age = i;
		zk_vector_push_back(vec, p);
	}

	zk_vector_free(vec, person_destructor);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_zk_vector_free_should_free_a_vector_when_a_destructor_function_is_not_provided);
	RUN_TEST(test_zk_vector_free_should_free_a_vector_when_a_destructor_function_is_provided);
	return UNITY_END();
}
