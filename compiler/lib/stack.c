#include "stack.h"
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
        printf("%d ", iter->value);
		iter = iter->next;
	}
}

stackelem *create_stackelem(int value, char *string){
    stackelem *new = (stackelem*) malloc(sizeof(stackelem));
    new->value = value;
    new->string = string;
    new->next = NULL;
    return new;
}

void stack_push(stackelem *first, int value){

    stackelem *iter = first;
    while ( iter->next != NULL ){
        iter = iter->next;
    }

    stackelem *new = create_stackelem(value, NULL);
    iter->next = new;
    new->next = NULL;

}

void stack_push_string(stackelem *first, char *str){

    stackelem *iter = first;
    while ( iter->next != NULL ){
        iter = iter->next;
    }
    
    stackelem *new = create_stackelem(0, str);
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