#include<stdlib.h>
#include<stdio.h>

#include "stack.h"
#include "tokenizer.h"

stackelem *tokenize(char *filename){

	FILE *input = fopen(filename, "r");

	stackelem *stack = new_stack();
	
	char c, prev = 0;
	int chickens = 0;

	do{
        c = getc(input);
		if (c == ' ') chickens++;
		else if (c == '\n'){
			if (prev != '\n') chickens++;
			push_back(stack, chickens);
            // print_stack(stack);
			chickens = 0;
		}
        prev = c;
    }while(!feof(input));

    push_back(stack, 0); // appending the EXIT opcode

	fclose(input);
	return stack;
}

