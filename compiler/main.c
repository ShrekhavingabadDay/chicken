#include "lib/stack.h"
#include <stdio.h>

// length of the string "chicken"
#define CHN_LEN 7

// subtract this to get value that needs to get pushed onto the stack
#define OFFSET 10

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
    stackelem *self; // pointer to the main program stack
    stackelem *stack; // pointer to rest of stack
    char *user_input; // pointer to user input
} main_stack;

// TODO: make tokenizer sensitive to syntax errors, give hints on where the error is located
stackelem *tokenize(char *filename){

	FILE *input = fopen(filename, "r");
    int row, col = 1;
    // int wb_index = 0;
    // char word_buffer[CHN_LEN];

    if (input == NULL)
        return NULL;
	
	char c, prev = 0;
	int chickens = 0;

    stackelem *stack = NULL;
	do{
        c = getc(input);

        // word_buffer[wb_index] = c;
        

		if (c == ' ') chickens++;
		else if (c == '\n'){

			if (prev != '\n') chickens++;

            if (stack == NULL)
                stack = create_stackelem(chickens, NULL);
            else
			    stack_push(stack, chickens);
			chickens = 0;
		}
        prev = c;
    }while(!feof(input));

    stack_push(stack, AXE);

	fclose(input);
	return stack;
}

/* ----- functions used at compilation ----- */

OPCODE next_opcode(stackelem **current_opcode){
    *current_opcode = (*current_opcode)->next;
    return (*current_opcode)->value;
}

void chicken_op(stackelem *v){
    stack_push_string(v, "chicken");
}

void add_op(stackelem *v){
    stackelem *s_a = stack_pop(v);
    stackelem *s_b = stack_pop(v);

    // TODO #1: do typechecking instead of naive approach
    int a = s_a->value;
    int b = s_b->value;

    free(s_a);
    free(s_b);

    stack_push(v, a+b);
}

void ten_or_more_op(stackelem *v, int amount){
    stack_push(v, amount-OFFSET);
}

void subtract_op(stackelem *v){
    stackelem *s_a = stack_pop(v);
    stackelem *s_b = stack_pop(v);

    // TODO #1: do typechecking instead of naive approach
    int a = s_a->value;
    int b = s_b->value;

    free(s_a);
    free(s_b);
    
    stack_push(v, b-a);
}

// double-wide instructions get current_opcode pointer as argument
void load_op(main_stack *ms, stackelem *current_opcode){

    int load_from = next_opcode(&current_opcode);
    // DEBUG: printf("loading from: %d\n", load_from);

    stackelem *popped = stack_pop(ms->stack);
    int index = popped->value;
    free(popped);

    // index-1 because the "first element of the stack" is a pointer to itself
    if (!load_from)
        stack_add_elem(ms->stack, stack_get(ms->stack, index-1));

    else
        stack_push(ms->stack, ms->user_input[index]);
}

int compile(stackelem *code_segment, char *user_input){

    main_stack ms;
    
    ms.user_input = user_input;
    ms.stack = code_segment;
    ms.stack = stack_push_back_string(code_segment, user_input);
    ms.self = ms.stack;

    stackelem *current_opcode = stack_get(ms.stack, 1);
    OPCODE opcode = current_opcode->value;

    while (opcode != AXE){

        switch (opcode) {

            case ADD:
                // printf("ADD operation\n");
                add_op(ms.stack);
                break;

            case CHICKEN:
                // printf("CHICKEN operation\n");
                chicken_op(ms.stack);
                break;

            case PICK:
                // printf("PICK operation\n");
                load_op(&ms, current_opcode);
                break;

            default:
                // printf("TENORMORE operation\n");
                ten_or_more_op(ms.stack, opcode);
                break;

        }
        opcode = next_opcode(&current_opcode);

        // print_stack(ms.stack);
        /* debugging: breaking on every instruction, because cat enters infinite loop
        char debug;
        scanf("%c", &debug);*/      
    }

    stackelem *top_of_stack = stack_peek(ms.stack);

    if (top_of_stack->string != NULL){
        printf("%s\n", top_of_stack->string);
    } else {
        printf("%d\n", top_of_stack->value);
    }

    free_stack(ms.self);

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

    if (argc >= 3)
        compile(program_segment, argv[2]);
    else
        compile(program_segment, NULL);

	return 0;
}

