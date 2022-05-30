#ifndef STACK
#define STACK

#include <stdlib.h>
#include <stdio.h>
#include "error.h"
#include "str.h"

typedef union {
	int integer;
	string str;
} STACK_VALUE;

typedef enum {
	INTEGER,
	STRING
} VALUE_TYPE;

// TODO: create union to hold different types epically

typedef struct _stackelem {
    VALUE_TYPE value_type; 
    STACK_VALUE value;
    struct _stackelem *next;
} stackelem;

void free_stack(stackelem *a);

void stack_push_int(stackelem *a, int value);
void stack_push_string(stackelem *stack, const char *value);

stackelem *stack_push_back_int(stackelem *s, int value);
stackelem *stack_push_back_string(stackelem *stack, const char *value);

void stack_add_elem(stackelem *stack, stackelem *element);

int stack_store_elem(stackelem *stack, stackelem *element, int store_to);

stackelem *create_int_stackelem(int value);
stackelem *create_string_stackelem(const char *value);

stackelem *stack_pop(stackelem *a);
stackelem *stack_get(stackelem *a, int index);
stackelem *stack_peek(stackelem *first);

int stack_length(stackelem *a);

void print_stack(stackelem *a);

#endif
