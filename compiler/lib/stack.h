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
} stackelem;

void free_stack(stackelem *head);
void print_stack(stackelem *head);
stackelem *new_stack();
void stack_push(stackelem *head, int value);
void stack_push_back(stackelem *head, int value);
stackelem *stack_pop(stackelem *head, error *e);
stackelem *stack_get(stackelem *head, int index, error *e);
stackelem *string_to_stack(char *str);

#endif
