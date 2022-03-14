#ifndef STACK
#define STACK

#include<stdlib.h>
#include<stdio.h>

typedef struct _stackelem {
	char value;
	struct _stackelem *next;
} stackelem;

void free_stack(stackelem *head);

stackelem* find_tail(stackelem *head);

void print_stack(stackelem *head);

stackelem* new_stack();

void push(stackelem *head, int value);

void push_back(stackelem *head, int value);

char pop(stackelem *head);

stackelem* stack_get(stackelem *head, int index);

#endif
