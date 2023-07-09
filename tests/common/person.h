#pragma once

typedef struct person_address_t {
	char *street;
	char *house_number;
	char *zip_code;
	char *city;
	char *state;
	char *country;
} person_address_t;

typedef struct person {
	char *name;
	unsigned int age;
	person_address_t address;
} person_t;

person_t *person_new(const char *const name, unsigned int age);

void person_free(person_t *person);

void person_free_internal(person_t *person);

void person_set_address(person_t *person,
                        const char *const street,
                        const char *const house_number,
                        const char *const zip_code,
                        const char *const city,
                        const char *const state,
                        const char *const country);

void person_print(person_t *person);

void person_copy(const person_t *const src, person_t *dst);

void person_move(person_t *src, person_t *dst);
