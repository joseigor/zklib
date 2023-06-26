#include <stdio.h>
#include <string.h>

#include "zk/zklib.h"

struct person {
	char *name;
	unsigned int age;
	char *email;
};

int zk_person_find_by_email(const void *const person, const void *const email)
{
	return strcmp(((struct person *)person)->email, (char *)email);
}

int main()
{
	zk_slist *list = NULL;

	struct person persons[] = {
		{ "Andrew", 25, "andrew@email.com" },
		{ "Thiago", 23, "thiago@email.com" },
		{ "Simon", 27, "simon@email.com" },
		{ "John", 30, "jonh@email.com" },
	};

	for (int i = 0; i < 4; i++) {
		list = zk_slist_push_back(list, &persons[i]);
	}

	// find person at index 2
	zk_slist *person = zk_slist_find_index(list, 2);
	if (person) {
		printf("Found %s with email %s at index 2\n",
		       ((struct person *)person->data)->name,
		       ((struct person *)person->data)->email);
	} else {
		printf("Not found person at index 2\n");
	}

	zk_slist *not_found_person = zk_slist_find_index(list, 10);
	if (not_found_person) {
		printf("Found %s with email %s at index 10\n",
		       ((struct person *)not_found_person->data)->name,
		       ((struct person *)not_found_person->data)->email);
	} else {
		printf("Not found person at index 10\n");
	}

	zk_free(&list, NULL);
	return 0;
}
