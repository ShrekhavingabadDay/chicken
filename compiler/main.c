#include "lib/stack.h"
#include "lib/tokenizer.h"
#include <stdio.h>

void read_user_input(stackelem *input_segment){
	char user_input;

	do {
		user_input = getchar();
		push(input_segment, user_input);
	} while(user_input != '\n');
}

/*
	returns 1 if compilation was successful
	returns 0 if some error occured.
*/
int compile(stackelem *program_segment){
	stackelem *memory_segment = new_stack();
	stackelem *input_segment  = new_stack();	
}

int main(int argc, char *argv[]){
    if (argc < 2) {
        printf("Live environment is not yet a feature, please provide source code to compile!\n");
        return 0;
    } else if (argc > 2) {
        printf("Too many arguments given!\n");
        return 1;
    }
	stackelem *program_segment = tokenize(argv[1]);
	print_stack(program_segment);
	free_stack(program_segment);
	return 0;
}
