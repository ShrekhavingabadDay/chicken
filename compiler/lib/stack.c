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
        free(tmp);
	}
}

void print_stack(stackelem *first){
	stackelem *iter = first;
	while (iter != NULL){
        if (iter->string != NULL) printf("%s ", iter->string);
        else printf("%d ", iter->value);
		iter = iter->next;
	}
    printf("\n");
}

stackelem *create_stackelem(int value, char *string){
    stackelem *new = (stackelem*) malloc(sizeof(stackelem));
    new->value = value;
    new->string = string;
    new->next = NULL;
    return new;
}

void stack_push(stackelem *first, int value){

    if (first==NULL) return;

    stackelem *iter = first;
    while ( iter->next != NULL )
        iter = iter->next;

    stackelem *new = create_stackelem(value, NULL);
    iter->next = new;
    new->next = NULL;

}

stackelem *stack_push_back(stackelem *first, int value){

    if (first==NULL) return NULL;

    stackelem *new = create_stackelem(value, NULL);
    new->next = first;
    return new;
}

void stack_push_string(stackelem *first, char *str){

    if (first==NULL) return;

    stackelem *iter = first;
    while ( iter->next != NULL )
        iter = iter->next;
    
    stackelem *new = create_stackelem(0, str);
    iter->next = new;
    new->next = NULL;

}

stackelem *stack_push_back_string(stackelem *first, char *str){

    if (first==NULL) return NULL;

    stackelem *new = create_stackelem(0, str);
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
    stackelem *to_add = create_stackelem(element->value, element->string);
    iter->next = to_add;
}