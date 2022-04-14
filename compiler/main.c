#include "lib/stack.h"
#include <stdio.h>

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
    stackelem *self; // pointer to the main program stack
    stackelem *stack; // pointer to rest of stack
    stackelem *used; // pointer to stack the program uses
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
    stack_push(stack, chickens);

    stack_push(stack, AXE);

	fclose(input);
	return stack;
}

/* ----- functions used at compilation ----- */

OPCODE next_opcode(stackelem **current_opcode){
    *current_opcode = (*current_opcode)->next;
    return (*current_opcode)->value;
}

int mult(int a, int b){
    return a*b;
}

int add(int a, int b){
    return a+b;
}

int sub(int a, int b){
    return b-a;
}

int divide(int a, int b){
    return b/a;
}

/*
    a "mother" function to all arithmetic operations on stack,
    beacause all follow the same pattern when getting values from the stack
*/
void arithmetic_op(stackelem *v, int (*op)(int, int) ){
    
    stackelem *s_a = stack_pop(v);
    stackelem *s_b = stack_pop(v);

    int a = s_a->value;
    int b = s_b->value;

    free(s_a);
    free(s_b);

    stack_push(v, op(a,b) );

}

void chicken_op(stackelem *v){
    stack_push_string(v, "chicken");
}

void ten_or_more_op(stackelem *v, int amount){
    stack_push(v, amount-10);
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

void store_op(stackelem *stack){

    stackelem *s_index = stack_pop(stack);
    int index = s_index->value;
    free(s_index);

    stackelem *to_store = stack_pop(stack);

    // TODO: handle possible errors
    int result = stack_store_elem(stack, to_store, index);

}

void jump_op(stackelem *stack, stackelem **current_opcode) {

    stackelem *s_offset = stack_pop(stack);
    stackelem *s_condition = stack_pop(stack);

    int offset = s_offset->value;
    free(s_condition);

    if (s_condition->value) {
        for (int i = 0; i<offset; i++)
            *current_opcode = (*current_opcode) -> next;
    }
    free(s_offset);
}

void char_op(stackelem *stack){
    stackelem *s_token = stack_pop(stack);

    int token = s_token->value;

    free(s_token);

    stack_push(stack, token);
}

int compile(stackelem *code_segment, char *user_input){

    main_stack ms;
    
    ms.user_input = user_input;
    ms.stack = code_segment;
    ms.stack = stack_push_back_string(code_segment, user_input);
    ms.used = stack_peek(ms.stack); // going to contain trailing EXIT instruction
    ms.self = ms.stack;

    stackelem *current_opcode = stack_get(ms.stack, 1);
    OPCODE opcode = current_opcode->value;

    while (opcode != AXE){

        switch (opcode) {

            case FOX:
		// DEBUG
		printf("subtracting\n");
                arithmetic_op(ms.stack, sub);
                break;

            case ADD:
		// DEBUG
		printf("adding\n");
                arithmetic_op(ms.stack, add);
                break;
            
            case ROOSTER:
		// DEBUG
		printf("multiplying\n");
                arithmetic_op(ms.stack, mult);
                break;

	    case CHICKEN:
		// DEBUG
		printf("©hicken\n");
                chicken_op(ms.stack);
                break;

            case PICK:
		// DEBUG
		printf("loading\n");
                load_op(&ms, current_opcode);
                break;

            case PECK:
		// DEBUG
		printf("storing\n");
                store_op(ms.stack);
                break;

            case FR:
		// DEBUG
		printf("jumping\n");
                jump_op(ms.stack, &current_opcode);
                break;

            case BBQ:
		// DEBUG
		printf("pushing character\n");
                char_op(ms.stack);
                break;

            default:
		// DEBUG
		printf("pushing integer\n");
                ten_or_more_op(ms.stack, opcode);
                break;

        }
        opcode = next_opcode(&current_opcode);
    
        print_stack(ms.used->next); //don't print the zero, we know it's there bruh
	char car;
	scanf("%c", &car); // wow this stops the programm
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

