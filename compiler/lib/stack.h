#ifndef STACK
#define STACK

#include<stdlib.h>
#include<string.h>
#include "error.h"

// TODO: create union to hold different types epically

typedef struct _stringelem {
    char value;
    struct _stringelem *next;
} stringelem;

typedef struct _stackelem {
    int value;
    struct _stackelem *next;
    stringelem *string;
} stackelem;


void free_stack(stackelem *a);
void print_stack(stackelem *a);
void stack_push(stackelem *a, int value);
void stack_push_string(stackelem *stack, char *str);
void concatenate(stackelem *a, stackelem *b);
void print_chicken_string(stringelem *s);

stackelem *create_stackelem(int value);
stackelem *stack_pop(stackelem *a);
stackelem *stack_get(stackelem *a, int index);
stackelem *string_to_stack(char *str);
stackelem *stack_peek(stackelem *first);

int stack_length(stackelem *a);

#endif
