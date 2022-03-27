#ifndef STACK
#define STACK

#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include "error.h"

typedef struct _stackelem {
    bool is_string;
    int value;
    struct _stackelem *next;
    struct _stackelem *prev;
} stackelem;

void free_stack(stackelem *a);
void print_stack(stackelem *a);
void stack_push(stackelem *a, int value);
void stack_push_string(stackelem *stack, stackelem *string);
void concatenate(stackelem *a, stackelem *b);
// stackelem *stack_push(stackelem *a, int value);
stackelem *create_stackelem(int value);
stackelem *stack_pop(stackelem *a);
stackelem *stack_get(stackelem *a, int index);
stackelem *string_to_stack(char *str);
stackelem *stack_peek(stackelem *first);

int stack_length(stackelem *a);

#endif
