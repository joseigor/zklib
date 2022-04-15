#include <stdlib.h>

#include "unity.h"
#include "zk_slist.h"

void setUp(void)
{
	// set stuff up here
}

void tearDown(void)
{
	// clean stuff up here
}

void test_zk_slist_append_data_to_empty_list(void)
{
	zk_slist_append(NULL, NULL);
	TEST_PASS();
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_zk_slist_append_data_to_empty_list);
	return UNITY_END();
}
