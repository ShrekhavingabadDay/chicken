#include "stack.h"
#include "error.h"
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

void print_stackelem(stackelem *e, char sep){
              
	if (e == NULL) {printf("NULL%c", sep); return;}

	switch (e->value_type){
		case STRING:
			printf("%s%c", e->value.str, sep);
			break;
		case INTEGER:
			printf("%d%c", e->value.integer, sep);
			break;
		case POINTER:
			printf("%p%c", e->value.pointer, sep);
			break;
		default:
			printf("<unknown type> ");

	}
	// printf("(%s) ", value_name(e->value_type) );
}

void print_stack(stackelem *first){
	if (first == NULL) {printf("[]\n"); return;}
	printf("[ ");
	stackelem *iter = first;
	while (iter->next != NULL){
		print_stackelem(iter, ' ');
		iter = iter->next;
	}
	print_stackelem(iter, 0);
    	printf(" ]\n");
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

stackelem *stack_push_int(stackelem *first, int value){

    if (first==NULL)
	    return create_int_stackelem(value);

    stackelem *iter = first;
    while ( iter->next != NULL )
        iter = iter->next;

    stackelem *new = create_int_stackelem(value);
    iter->next = new;
    new->prev  = iter;
    new->next = NULL;

    return first;

}

stackelem *stack_push_back_int(stackelem *first, int value){

    if (first==NULL) return create_int_stackelem(value);

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

stackelem *stack_copy_elem(stackelem *element){

	stackelem *copy;

	switch (element->value_type){
		case INTEGER:
			copy = create_int_stackelem(element->value.integer);
			break;
		case STRING:
			copy = create_string_stackelem(element->value.str);
			break;
		case POINTER:
			copy = create_pointer_stackelem(element->value.pointer);
			break;
		default:
			copy = NULL;
	}

	copy->next = NULL;
	copy->prev = NULL;

	return copy;

}

ERROR_TYPE stack_store_elem(stackelem *stack, stackelem *element, int store_to){
    
    if (store_to < 0 || store_to-1 > stack_length(stack) ) return OUT_OF_BOUNDS;

    stackelem *left_n = stack;

    while (--store_to)
        left_n = left_n->next;

    stackelem *copy = stack_copy_elem(element);

    stackelem *middle = left_n->next;
    stackelem *right_n = middle->next;

    left_n->next = NULL;
    right_n->prev = NULL;

    free(middle);

    left_n->next = copy;
    copy->prev = left_n;

    copy->next = right_n;
    right_n->prev = copy;

    return NONE;
}
