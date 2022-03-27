#include "stack.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

/* head element just complicates things... */

void free_stack(stackelem *first){
	stackelem *tmp;
	while (first != NULL){
		tmp = first;
		first = first->next;
		free(tmp);
	}
}

void print_stack(stackelem *first){
	stackelem *iter = first;
	while (iter != NULL){
        printf("%d ", iter->value);
		iter = iter->next;
	}
}

stackelem *create_stackelem(int value){
    stackelem *new = (stackelem*) malloc(sizeof(stackelem));
    new->value = value;
    new->next = NULL;
    return new;
}

stackelem *create_string_stackelem(int value) {
    stackelem *new = (stackelem*) malloc(sizeof(stackelem));
    new->value = value;
    new->is_string = true;
    new->next = NULL;
    return new;
}
/*
stackelem *stack_push(stackelem *first, int value){
	stackelem *new = create_stackelem(value);
	first->next = new;
    new->next = NULL;
    return new;
}

stackelem *stack_push_string(stackelem *first, int value){
	stackelem *new = create_string_stackelem(value);
	first->next = new;
    new->next = NULL;
    return new;
}
*/
void stack_push(stackelem *first, int value){

    stackelem *iter = first;
    while ( iter->next != NULL ){
        iter = iter->next;
    }

    stackelem *new = create_stackelem(value);
    iter->next = new;
    new->next = NULL;

}

void string_stack_push(stackelem *first, int value){

    stackelem *iter = first;
    while ( iter->next != NULL ){
        iter = iter->next;
    }

    stackelem *new = create_string_stackelem(value);
    iter->next = new;
    new->next = NULL;
}

stackelem *stack_pop(stackelem *first){
    stackelem *iter = first;
    if (iter == NULL) return NULL;
    while (iter->next != NULL) iter = iter->next;
    return iter;
}

stackelem *stack_get(stackelem* first, int index){

    // cannot get anything below one:
    if (index < 1) return NULL;

    /* the stack enumeration in chicken starts from index 1 */
    int i = 1;
    stackelem *iter = first;
    while (iter->next != NULL && i < index){
        iter = iter->next;
        i++;
    }
    // couldn't get to that index -> out of bounds
    if (i < index) return NULL;

    return iter;
}

stackelem *string_to_stack(char *str){

    int string_length = strlen(str);

    if (string_length < 1) return NULL;

    if (string_length == 1) return create_string_stackelem((int)str[0]);

    stackelem *first = create_string_stackelem((int)str[0]);
    for (int i = 1; i<string_length; i++){
        string_stack_push(first, (int)str[i]);
    }
    return first;

}

int stack_length(stackelem *first){
    stackelem *iter = first;
    if (iter == NULL) return 0;
    if (iter->next == NULL) return 1;
    int length = 1;
    while (iter->next != NULL){
        length++;
        iter = iter->next;
    }
    return length;
}

stackelem *stack_peek(stackelem *first){
    if (first == NULL) return NULL;
    stackelem *iter = first;
    while (iter->next != NULL)
        iter = iter->next;
    return iter;
}

void stack_push_string(stackelem *stack, stackelem *string){
    stackelem *iter = string;
    stackelem *stack_last = stack_peek(stack);
    do{
        stack_last->next = iter;
        stack_last = stack_last->next;
        iter = iter->next;
    }while(iter != NULL);
}