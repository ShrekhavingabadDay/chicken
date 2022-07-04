#ifndef STACK
#define STACK

#include <stdlib.h>
#include <stdio.h>
#include "error.h"
#include "str.h"

struct _stackelem;

typedef union {
	int integer;
	string str;
	struct _stackelem* pointer;
} STACK_VALUE;

typedef enum {
	INTEGER,
	STRING,
	POINTER
} VALUE_TYPE;

typedef struct _stackelem {
    VALUE_TYPE value_type; 
    STACK_VALUE value;
    struct _stackelem *next;
    struct _stackelem *prev;
} stackelem;

void free_stack(stackelem *a);

stackelem* stack_push_int(stackelem *a, int value);
void stack_push_string(stackelem *stack, const char *value);
void stack_push_char(stackelem *first, const char c);

stackelem *stack_push_back_int(stackelem *s, int value);
stackelem *stack_push_back_string(stackelem *stack, const char *value);
stackelem *stack_push_back_pointer(stackelem *stack, stackelem *value);

void stack_add_elem(stackelem *stack, stackelem *element);

ERROR_TYPE stack_store_elem(stackelem *stack, stackelem *element, int store_to);

stackelem *create_int_stackelem(int value);
stackelem *create_string_stackelem(const char *value);
stackelem *create_pointer_stackelem(stackelem *value);

stackelem *stack_pop(stackelem *a);
stackelem *stack_get(stackelem *a, int index);
stackelem *stack_peek(stackelem *first);

int stack_length(stackelem *a);

void print_stackelem(stackelem *e, char sep);

void print_stack(stackelem *a);

#endif
