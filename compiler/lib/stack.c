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

void print_stack(stackelem *head){
	stackelem *iter = head->next;
	while (iter != NULL){
        printf("%d ", iter->value);
		iter = iter->next;
	}
}

stackelem *create_stackelem(int value){
    stackelem *new = (stackelem*) malloc(sizeof(stackelem));
    new->value = value;
    return new;
}

stackelem *create_string_stackelem(int value) {
    stackelem *new = (stackelem*) malloc(sizeof(stackelem));
    new->value = value;
    new->is_string = true;
    return new;
}

stackelem *new_stack(){
	stackelem *head = create_stackelem(0);
	head->next = NULL;
	return head;
}

stackelem *new_string_stack(){
    stackelem *head = create_string_stackelem(0);
    head->next = NULL;
    return head;
}

void stack_push(stackelem *head, int value){
    head->value++;
	stackelem *new = create_stackelem(value);
	stackelem *first = head->next;
	new->next = first;
	head->next = new;
}

void stack_push_string(stackelem *head, int value){
    head->value++;
	stackelem *new = create_string_stackelem(value);
	stackelem *first = head->next;
	new->next = first;
	head->next = new;
}

void stack_push_back(stackelem *head, int value){

    // if list is empty:
    if (head->value == 0){
        stack_push(head, value); 
        return;
    }

    // finding last elem
    error *e;
    stackelem *last = stack_get(head, head->value, e);

    if (e == NULL) {
        show_error(e);
        return;
    }

    stackelem *new = create_stackelem(value);
    last->next = new;
    new->next = NULL;
    head->value++;
}

stackelem *stack_pop(stackelem *head, error *e){

    error *err = NULL;

    if (head->value == 0) {
      err = (error*)malloc(sizeof(error));
      strcpy(err->origin, "[STACK]");
      strcpy(err->message, "Cannot pop: no value in stack!");
      e = err;
      return 0;
    }

    head->value--;
	stackelem *first = head->next;
	stackelem *second = first->next;
	head->next = second;
    free(first);
    e = err;
    return first;
}

stackelem *stack_get(stackelem* head, int index, error *e){

    error *err = NULL;

    index--;

    if (index <= -1 || index >= head->value){
        strcpy(err->origin, "[STACK]");

        char str[50];
        sprintf(str, "Cannot get %d from stack: out of bounds!", index);

        strcpy(err->message, str);
        e = err;
        return NULL;
    }

    stackelem* elem = head->next;
    for (int i = 0; i<index; i++)
        elem = elem->next;

    e = err;
    return elem;

}

stackelem *string_to_stack(char *str){

    stackelem *head = new_string_stack();

    int i = 0;
    while (str[i] != '\0'){
        stack_push_back(head, (int)str[i]);
        i++;
    }

    return head;
}
