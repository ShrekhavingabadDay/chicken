#include "lib/stack.h"

// length of the string "chicken"
#define CHN_LEN 7

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
    char *input_register; // pointer to user input
} First_Segment;

typedef struct {
    First_Segment first_segment;
    stackelem *second_segment; // second segment soters code
    int third_segment_start;
} main_stack;

stackelem *tokenize(char *filename){

	FILE *input = fopen(filename, "r");
    int row, col = 1;
    int wb_index = 0;
    char word_buffer[CHN_LEN];

    if (input == NULL)
        return NULL;
	
	char c, prev = 0;
	int chickens = 0;

    stackelem *stack = NULL;
	do{
        c = getc(input);

        word_buffer[wb_index] = c;
        

		if (c == ' ') chickens++;
		else if (c == '\n'){

			if (prev != '\n') chickens++;

            if (stack == NULL){
                stack = create_stackelem(chickens, NULL);
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

        stack_main.first_segment.input_register = user_input;
        stack_main.second_segment = code_segment;
        stack_main.third_segment_start = stack_length(stack_main.second_segment);

        stackelem *current_opcode = stack_main.second_segment;
        OPCODE opcode = current_opcode->value;

        while (opcode != AXE){

            switch (opcode) {

                case CHICKEN:

                    stack_push_string(stack_main.second_segment, "chicken");

                default:
                    break;

            }
            current_opcode = current_opcode->next;
            opcode = current_opcode->value;
            
        }

        stackelem *top_of_stack = stack_peek(stack_main.second_segment);
        if (top_of_stack->string != NULL){
            printf("%s", top_of_stack->string);
            printf("\n");
        }

	    free_stack(stack_main.second_segment);

        return 0;
}

int main(int argc, char *argv[]){

    if (argc < 2) {
        printf("Live environment is not yet a feature, please provide source code to compile!\n");
        return 0;
    }

    char *file = argv[1]; 

	stackelem *program_segment = tokenize(file);
    if (program_segment == NULL) {
        printf("%s does not exist!\n", file);
        return 1;
    }

    compile(program_segment, argv[2]);

	return 0;
}

