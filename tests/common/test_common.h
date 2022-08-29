#ifndef TEST_COMMON_H
#define TEST_COMMON_H

struct dummy_node_data {
	int value;
	char *string;
};

void dummy_node_data_free_foreach(void *data, void *user_data);

void dummy_node_data_free(void *data);

#endif
