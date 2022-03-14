#include "stack.h"
#include<stdlib.h>
#include<stdio.h>

/* size of stack is stored in the head element */

void free_stack(stackelem *head){
	stackelem *tmp;
	while (head != NULL){
		tmp = head;
		head = head->next;
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
	stackelem *iter = head->next;
	while (iter->next != NULL){
        printf("%d ", iter->value);
		iter = iter->next;
	}
}

stackelem *create_stackelem(int value){
    stackelem *new = (stackelem*) malloc(sizeof(stackelem));
    new->value = value;
    return new;
}

stackelem* new_stack(){
	stackelem *head = create_stackelem(0);
	stackelem *tail = create_stackelem(0);
	tail->next = NULL;
	head->next = tail;
	return head;
}

void push(stackelem *head, int value){
    head->value++;
	stackelem *new = create_stackelem(value);
	stackelem *first = head->next;
	new->next = first;
	head->next = new;
}

void push_back(stackelem *head, int value){

    // printf("%d ", value);

    // if we do not have any elements between head and tail:
    if (head->value == 0){
        push(head, value); 
        return;
    }

    // finding last element and tail element
    stackelem *last = stack_get(head, head->value);
    stackelem *tail = last->next;
    stackelem *new = create_stackelem(value);
    last->next = new;
    new->next = tail;
    head->value++;
}

char pop(stackelem *head){
    head->value--;
	stackelem *first = head->next;
	stackelem *second = first->next;
	head->next = second;
	int value = first->value;
    free(first);
    return value;
}

stackelem* stack_get(stackelem* head, int index){

    index--;

    if (index <= -1 || index >= head->value){
        printf("\n[STACK] Index %d out of bounds\n", index);
        return NULL;
    }

    stackelem* elem = head->next;
    for (int i = 0; i<index; i++)
        elem = elem->next;

    return elem;

}
