#ifndef STACK
#define STACK

#include<stdlib.h>
#include<string.h>
#include <stdio.h>
#include "error.h"

// TODO: create union to hold different types epically

typedef struct _stackelem {
    int value;
    struct _stackelem *next;
    char *string;
} stackelem;

void free_stack(stackelem *a);
void print_stack(stackelem *a);
void stack_push(stackelem *a, int value);
stackelem *stack_push_back(stackelem *s, int value);
void stack_push_string(stackelem *stack, char *str);
stackelem *stack_push_back_string(stackelem *stack, char *str);
void stack_add_elem(stackelem *stack, stackelem *element);

stackelem *create_stackelem(int value, char *string);
stackelem *stack_pop(stackelem *a);
stackelem *stack_get(stackelem *a, int index);
stackelem *stack_peek(stackelem *first);

int stack_length(stackelem *a);

#endif
