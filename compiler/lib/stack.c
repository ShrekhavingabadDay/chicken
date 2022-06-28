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

char *value_name(VALUE_TYPE t){
	switch (t) {
		case INTEGER:
			return "int";
			break;
		case STRING:
			return "str";
			break;
		case POINTER:
			return "ptr";
			break;
		default:
			return "<unknown>";
	}
}

void print_stackelem(stackelem *e){
	switch (e->value_type){
		case STRING:
			printf("%s ", e->value.str);
			break;
		case INTEGER:
			printf("%d ", e->value.integer);
			break;
		case POINTER:
			printf("%p ", e->value.pointer);
			break;
		default:
			printf("<unknown type> ");

	}
	// printf("(%s) ", value_name(e->value_type) );
}

void print_stack(stackelem *first){
	printf("[ ");
	stackelem *iter = first;
	while (iter != NULL){
		print_stackelem(iter);
		iter = iter->next;
	}
    	printf("]\n");
}

stackelem *create_int_stackelem(int value){

    stackelem *new = (stackelem*) malloc(sizeof(stackelem));

    new->value_type = INTEGER;
    new->value.integer = value;
    new->next = NULL;
    new->prev = NULL;

    return new;
}

stackelem *create_string_stackelem(const char *value){

    stackelem *new = (stackelem*) malloc(sizeof(stackelem));

    new->value_type = STRING;
    new->value.str = string_create(value);
    new->next = NULL;
    new->prev = NULL;

    return new;
}

stackelem *create_char_stackelem(const char value){

	stackelem *new = (stackelem*) malloc(sizeof(stackelem));

	new->value_type = STRING;

	string s = string_create(NULL);
	string_add_char(&s, value);

	new->value.str = s;
	new->next = NULL;
    	new->prev = NULL;

	return new;
}

stackelem *create_pointer_stackelem(stackelem *value){

    stackelem *new = (stackelem*) malloc(sizeof(stackelem));

    new->value_type = POINTER;
    new->value.pointer = value;
    new->next = NULL;
    new->prev = NULL;

    return new;
}

void stack_push_int(stackelem *first, int value){

    if (first==NULL) return;

    stackelem *iter = first;
    while ( iter->next != NULL )
        iter = iter->next;

    stackelem *new = create_int_stackelem(value);
    iter->next = new;
    new->prev  = iter;
    new->next = NULL;

}

stackelem *stack_push_back_int(stackelem *first, int value){

    if (first==NULL) return NULL;

    stackelem *new = create_int_stackelem(value);
    new->next = first;
    new->prev  = NULL;
    return new;
}

// TODO: There's a lot of code repetition :(
// aaand i'm pretty sure that stack_peek does what I do in these functions
void stack_push_string(stackelem *first, const char *value){

    if (first==NULL) return;

    stackelem *iter = first;

    while ( iter->next != NULL )
        iter = iter->next;
    
    stackelem *new = create_string_stackelem(value);
    iter->next = new;
    new->prev  = iter;
    new->next = NULL;

}

void stack_push_char(stackelem *first, const char value){
	if (first == NULL) return;

	stackelem *iter = first;

	while ( iter->next != NULL )
		iter = iter->next;

	stackelem *new = create_char_stackelem(value);
	iter->next = new;
    	new->prev  = iter;
	new->next = NULL;
}

stackelem *stack_push_back_string(stackelem *first, const char *value){

	// why was this here?
    // if (first==NULL) return NULL;

    stackelem *new = create_string_stackelem(value);
    new->next = first;
    new->prev = NULL; 
    return new;
}

/* void stack_push_pointer(stackelem *first, stackelem *value){

    if (first==NULL) return;

    stackelem *iter = first;

    while ( iter->next != NULL )
        iter = iter->next;
    
    stackelem *new = create_pointer_stackelem(value);
    iter->next = new;
    new->next = NULL;

} */

stackelem *stack_push_back_pointer(stackelem *first, stackelem *value){

    if (first==NULL) return NULL;

    stackelem *new = create_pointer_stackelem(value);
    new->next = first;
    new->prev= NULL;
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

    if (index < 0) return NULL;

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
	stackelem *to_add;

	switch (element->value_type){
		case INTEGER:
			to_add = create_int_stackelem(element->value.integer);
			break;
		case STRING:
			to_add = create_string_stackelem(element->value.str);
			break;
		case POINTER:
			to_add = create_pointer_stackelem(element->value.pointer);
			break;
		default:
			to_add = NULL;
	}

	iter->next = to_add;

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
