#ifndef STACK
#define STACK

#include<stdlib.h>
#include<stdio.h>

typedef struct _stackelem {
	char value;
	struct _stackelem *next;
} stackelem;

void free_stack(stackelem *head){
	stackelem *iter = head;
	stackelem *tmp;
	while (iter != NULL){
		tmp = iter;
		iter = iter->next;
		free(tmp);
	}
}
stackelem* find_tail(stackelem *head){
	stackelem *current = head;
	while (current->next != NULL)
		current = current->next;
	return current;
}
void print_stack(stackelem *head){
	stackelem *tail = find_tail(head);
	stackelem *iter = head->next;
	while (iter != NULL){
		printf("%d ", iter->value);
		iter = iter->next;
	}
	printf("\n");
}

stackelem* new_stack(){
	stackelem *head = (stackelem*) malloc(sizeof(stackelem));
	stackelem *tail = (stackelem*) malloc(sizeof(stackelem));
	tail->next = NULL;
	tail->value = 0;
	head->next = tail;
	return head;
}

void push(stackelem *head, char value){
	stackelem *new = (stackelem*) malloc(sizeof(stackelem));
	stackelem *first = head->next;
	new->next = first;
	new->value = value;
	head->next = new;
}

char pop(stackelem *head){
	stackelem *first = head->next;
	stackelem *second = first->next;
	head->next = second;
	return first->value;
}

#endif
