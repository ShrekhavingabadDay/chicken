#include "stack.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

// string functions
stringelem *create_stringelem(char value) {
    stringelem *new = (stringelem*) malloc(sizeof(stringelem));
    new->value = value;
    new->next = NULL;
    return new;
}

void string_push(stringelem *first, char value){

    stringelem *iter = first;
    while ( iter->next != NULL ){
        iter = iter->next;
    }

    stringelem *new = create_stringelem(value);
    iter->next = new;
    new->next = NULL;
}

stringelem *create_chicken_string(char *str){

    int string_length = strlen(str);

    if (string_length < 1) return NULL;

    if (string_length == 1) return create_stringelem((int)str[0]);

    stringelem *first = create_stringelem((int)str[0]);
    for (int i = 1; i<string_length; i++){
        string_push(first, (int)str[i]);
    }
    return first;

}

void print_chicken_string(stringelem *s){
    stringelem *iter = s;
    while (iter != NULL) {
        printf("%c", iter->value);
        iter = iter->next;
    }
}

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

stackelem *create_stackelem(int value){
    stackelem *new = (stackelem*) malloc(sizeof(stackelem));
    new->value = value;
    new->string = NULL;
    new->next = NULL;
    return new;
}

stackelem *create_stackelem_for_string(stringelem *strelem){
    stackelem *new = (stackelem*) malloc(sizeof(stackelem));
    new->string = strelem;
    new->next = NULL;
    return new;
}

void stack_push(stackelem *first, int value){

    stackelem *iter = first;
    while ( iter->next != NULL ){
        iter = iter->next;
    }

    stackelem *new = create_stackelem(value);
    iter->next = new;
    new->next = NULL;

}

void stack_push_string(stackelem *first, char *str){

    stringelem *string = create_chicken_string(str);

    stackelem *iter = first;
    while ( iter->next != NULL ){
        iter = iter->next;
    }

    stackelem *new = create_stackelem_for_string(string);
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