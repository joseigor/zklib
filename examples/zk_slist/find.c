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

	char *emails_to_find[] = { "simon@email.com", "andrew@email.com", "jose@email.com" };
	for (int i = 0; i < 3; i++) {
		zk_slist *person = zk_slist_find(list, emails_to_find[i], zk_person_find_by_email);
		if (person) {
			printf("Found %s with email %s\n",
			       ((struct person *)person->data)->name,
			       ((struct person *)person->data)->email);
		} else {
			printf("Not found person with email %s\n", emails_to_find[i]);
		}
	}

	zk_free(&list, NULL);
	return 0;
}
