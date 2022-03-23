#include <stdio.h>
#include <stdlib.h>
#include "lib/stack.h"

typedef enum {
    AXE, // exit
    CHICKEN, // prints chicken
    ADD, // adds up two top stack values
    FOX, // subtracts two top stack values
    ROOSTER, // multiplies two top stack values
    COMPARE, // this one's not that hard to guess
    PICK, // loads from a specified source
    PECK, // pops and stores given value to stack
    FR, // jumps to specified position in specified stack
    BBQ // pushes character as ascii
} OPCODE;

typedef struct {
    stackelem *third_segment; // stores the pointer to the head of the segment usable by the program as memory
    stackelem *second_segment; // stores the pointer to the head of the segment that stores the instructions
    stackelem *input_register; // stores the pointer to the head of the input stack
} program_stack;

stackelem *tokenize(char *filename){

	FILE *input = fopen(filename, "r");

    if (input == NULL)
        return NULL;

	stackelem *stack = new_stack();
	
	char c, prev = 0;
	int chickens = 0;

	do{
        c = getc(input);
		if (c == ' ') chickens++;
		else if (c == '\n'){
			if (prev != '\n') chickens++;
			stack_push_back(stack, chickens);
			chickens = 0;
		}
        prev = c;
    }while(!feof(input));

    stack_push_back(stack, 0); // appending the EXIT opcode

	fclose(input);
	return stack;
}

int compile(stackelem *code_segment, char *user_input){

        program_stack ps;
        stackelem *input_string = string_to_stack(user_input);

        ps.second_segment = code_segment;
        ps.third_segment = new_stack();
        ps.input_register = input_string;

        print_stack(ps.second_segment);
	    print_stack(ps.input_register);

	    free_stack(ps.second_segment);
        free_stack(ps.input_register);
        free_stack(ps.third_segment);

        return 0;
}

int main(int argc, char *argv[]){

    if (argc < 2) {
        printf("Live environment is not yet a feature, please provide source code to compile!\n");
        return 0;
    } else if (argc > 2) {
        printf("Too many arguments given!\n");
        return 1;
    }

    char *file = argv[1]; 

	stackelem *program_segment = tokenize(file);
    if (program_segment == NULL) {
        printf("%s does not exist!\n", file);
        return 1;
    }

    compile(program_segment, "soap");

	return 0;
}
