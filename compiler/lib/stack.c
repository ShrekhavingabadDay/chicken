#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* stack functions */

void free_stack(stackelem *first){
	stackelem *tmp;
	while (first != NULL){
		tmp = first;
		first = first->next;

		if (tmp->value_type == STRING)
			string_free(tmp->value.str);

        	free(tmp);
	}
}

void print_stack(stackelem *first){
	stackelem *iter = first;
	while (iter != NULL){
		if (iter->value_type == STRING)
			printf("%s ", iter->value.str);
		else 
			printf("%d ", iter->value.integer);
		iter = iter->next;
	}
    	printf("\n");
}

stackelem *create_int_stackelem(int value){

    stackelem *new = (stackelem*) malloc(sizeof(stackelem));

    new->value_type = INTEGER;
    new->value.integer = value;
    new->next = NULL;

    return new;
}

stackelem *create_string_stackelem(const char *value){

    stackelem *new = (stackelem*) malloc(sizeof(stackelem));

    new->value_type = STRING;
    new->value.str = string_create(value);
    new->next = NULL;

    return new;
}

void stack_push_int(stackelem *first, int value){

    if (first==NULL) return;

    stackelem *iter = first;
    while ( iter->next != NULL )
        iter = iter->next;

    stackelem *new = create_int_stackelem(value);
    iter->next = new;
    new->next = NULL;

}

stackelem *stack_push_back_int(stackelem *first, int value){

    if (first==NULL) return NULL;

    stackelem *new = create_int_stackelem(value);
    new->next = first;
    return new;
}

void stack_push_string(stackelem *first, const char *value){

    if (first==NULL) return;

    stackelem *iter = first;

    while ( iter->next != NULL )
        iter = iter->next;
    
    stackelem *new = create_string_stackelem(value);
    iter->next = new;
    new->next = NULL;

}

stackelem *stack_push_back_string(stackelem *first, const char *value){

    if (first==NULL) return NULL;

    stackelem *new = create_string_stackelem(value);
    new->next = first;
    return new;
}

stackelem *stack_pop(stackelem *first){
    stackelem *iter = first;
    stackelem *prev = NULL;
    if (iter == NULL) return NULL;
    while (iter->next != NULL){
        prev = iter;
        iter = iter->next;
    }
    if (prev != NULL) prev->next = NULL;
    return iter;
}

stackelem *stack_get(stackelem* first, int index){

    // cannot get anything below one:
    if (index < 0) return NULL;

    /* the stack enumeration in chicken starts from index 1 */
    int i = 0;
    stackelem *iter = first;
    while (iter->next != NULL && i < index){
        iter = iter->next;
        i++;
    }
    // couldn't get to that index -> out of bounds
    if (i < index) return NULL;

    return iter;
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

void stack_add_elem(stackelem *stack, stackelem *element){
    stackelem *iter = stack;
    while ( iter->next != NULL )
        iter = iter->next;

    // iter->next = element;
    // element->next = NULL;
	stackelem *to_add = (element->value_type == STRING) ? 
				create_string_stackelem(element->value.str) : 
				create_int_stackelem(element->value.integer);

}

// int, so that we can handle errors more elegantly
int stack_store_elem(stackelem *stack, stackelem *element, int store_to){
    stackelem *prev;
    stackelem *current;
    stackelem *next;
    
    // cannot get anything below zero:
    if (store_to < 0) return 1;

    /* the stack enumeration in chicken starts from index 1 */
    int i = 0;
    stackelem *iter = stack;
    while (iter->next != NULL && i < store_to-1){ // store_to-1 because we want the element before the given index
        iter = iter->next;
        i++;
    }

    // couldn't get to that index -> out of bounds
    if (i < store_to-1) return 1;

    prev = iter;

    if (prev->next == NULL){
        prev->next = element;
        return 0;
    }

    current = prev->next;
    next = current->next;

    prev->next = element;
    element->next = next;

    free(current);

    return 0;
}
