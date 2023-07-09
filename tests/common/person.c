#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/person.h"

person_t *person_new(const char *const name, unsigned int age)
{
	person_t *person = malloc(sizeof(person_t));
	if (person == NULL)
		return NULL;
	person->name = strdup(name);
	person->age = age;
	person->address.street = NULL;
	person->address.house_number = NULL;
	person->address.zip_code = NULL;
	person->address.city = NULL;
	person->address.state = NULL;
	person->address.country = NULL;
	return person;
}

static void person_address_free(person_address_t *address)
{
	free(address->street);
	free(address->house_number);
	free(address->zip_code);
	free(address->city);
	free(address->state);
	free(address->country);
}

void person_free(person_t *person)
{
	if (person == NULL)
		return;
	person_free_internal(person);
	free(person);
}

void person_free_internal(person_t *person)
{
	free(person->name);
	person_address_free(&person->address);
}

void person_set_address(person_t *person,
                        const char *const street,
                        const char *const house_number,
                        const char *const zip_code,
                        const char *const city,
                        const char *const state,
                        const char *const country)
{
	person->address.street = strdup(street);
	person->address.house_number = strdup(house_number);
	person->address.zip_code = strdup(zip_code);
	person->address.city = strdup(city);
	person->address.state = strdup(state);
	person->address.country = strdup(country);
}

void person_print(person_t *person)
{
	printf("Name: %s\n", person->name);
	printf("Age: %d\n", person->age);
	printf("Address:\n");
	printf("\tStreet: %s\n", person->address.street);
	printf("\tHouse number: %s\n", person->address.house_number);
	printf("\tZip code: %s\n", person->address.zip_code);
	printf("\tCity: %s\n", person->address.city);
	printf("\tState: %s\n", person->address.state);
	printf("\tCountry: %s\n", person->address.country);
}

static void person_address_copy(const person_address_t *const src, person_address_t *dst)
{
	dst->street = src->street ? strdup(src->street) : NULL;
	dst->house_number = src->house_number ? strdup(src->house_number) : NULL;
	dst->zip_code = src->zip_code ? strdup(src->zip_code) : NULL;
	dst->city = src->city ? strdup(src->city) : NULL;
	dst->state = src->state ? strdup(src->state) : NULL;
	dst->country = src->country ? strdup(src->country) : NULL;
}

void person_copy(const person_t *const src, person_t *dst)
{
	// deep copy
	dst->name = strdup(src->name);
	dst->age = src->age;
	person_address_copy(&src->address, &dst->address);
}

void person_move(person_t *src, person_t *dst)
{
	memcpy(dst, src, sizeof(person_t));
	free(src);
}
