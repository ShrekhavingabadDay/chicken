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
    stackelem *main_stack; // pointer to the main program stack
    stackelem *input_register; // pointer to user input
} First_Segment;

typedef struct {
    First_Segment first_segment;
    stackelem *second_segment; // second segment soters code
    int third_segment_start;
} main_stack;

stackelem *tokenize(char *filename){

	FILE *input = fopen(filename, "r");

    if (input == NULL)
        return NULL;
	
	char c, prev = 0;
	int chickens = 0;

    stackelem *stack = NULL;
	do{
        c = getc(input);
		if (c == ' ') chickens++;
		else if (c == '\n'){

			if (prev != '\n') chickens++;

            if (stack == NULL){
                stack = create_stackelem(chickens);
            }else{
			    stack_push(stack, chickens);
            }

			chickens = 0;
		}
        prev = c;
    }while(!feof(input));

    stack_push(stack, AXE);

	fclose(input);
	return stack;
}

int compile(stackelem *code_segment, char *user_input){

        main_stack stack_main;

        stack_main.first_segment.input_register = string_to_stack(user_input);
        stack_main.second_segment = code_segment;
        //storing the index where the program stack starts for referencing it later
        stack_main.third_segment_start = stack_length(stack_main.second_segment);

        /*
        print_stack(stack_main.second_segment);
	    print_stack(stack_main.first_segment.input_register);
        printf("program stack starts at: %d\n", stack_main.third_segment_start);
        */

        stackelem *current_opcode = stack_main.second_segment;
        OPCODE opcode = current_opcode->value;

        while (opcode != AXE){

            switch (opcode) {

                case CHICKEN:
                    stack_push_string(stack_main.second_segment, string_to_stack("chicken"));

                default:
                    break;

            }
            current_opcode = current_opcode->next;
            opcode = current_opcode->value;
            
        }

        printf("OUTPUT: %c\n", stack_peek(stack_main.second_segment)->value);

	    free_stack(stack_main.second_segment);
        free_stack(stack_main.first_segment.input_register);

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
