#ifndef TOKENIZER
#define TOKENIZER

#include<stdlib.h>
#include<stdio.h>

#include "stack.h"

stackelem *tokenize(char *filename){

	FILE *input = fopen(filename, "r");

	stackelem *stack = new_stack();
	
	unsigned char c, prev = 0;
	char chickens = 0;
	c = getc(input);
	while (!feof(input)){
		if (c == ' ')
			chickens++;
		else if (c == '\n'){
			if (prev != '\n') chickens ++;
			push(stack, chickens);
			chickens = 0;
		}
		c = getc(input);
	}
	fclose(input);
	return stack;

}


#endif
