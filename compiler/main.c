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

/*typedef struct {
    stackelem *self; // pointer to the main program stack
    stackelem *stack; // pointer to rest of stack
    stackelem *used; // pointer to stack the program uses
    char *user_input; // pointer to user input
} main_stack;*/

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
                stack = create_int_stackelem(chickens);
            else
			    stack_push_int(stack, chickens);
			chickens = 0;
		}
        prev = c;
    }while(!feof(input));
    stack_push_int(stack, chickens);

    stack_push_int(stack, AXE);

	fclose(input);
	return stack;
}

/* ----- functions used at compilation ----- */

OPCODE next_opcode(stackelem **current_opcode){
    *current_opcode = (*current_opcode)->next;
    return (*current_opcode)->value.integer;
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

    int a = s_a->value.integer;
    int b = s_b->value.integer;

    free(s_a);
    free(s_b);

    stack_push_int(v, op(a,b) );

}

void chicken_op(stackelem *v){
    stack_push_string(v, "chicken");
}

void ten_or_more_op(stackelem *v, int amount){
    stack_push_int(v, amount-10);
}

// double-wide instructions get current_opcode pointer as argument
void load_op(stackelem *stack, stackelem *current_opcode){

    int load_from = next_opcode(&current_opcode);

    // DEBUG:
    printf("loading from: %s\n", (load_from ? "user input" : "stack") );

    stackelem *popped = stack_pop(stack);
    int index = popped->value.integer;
    free(popped);

    printf("loading element with index %d\n", index);

    if (!load_from){
	stackelem *pointer_to_self = stack_get(stack, 0)->value.pointer;
	stackelem *to_add = stack_get( pointer_to_self, index );
        stack_add_elem(stack, to_add );
    }else{
        stack_push_string(stack, &(( stack_get(stack, 1) )->value.str[index]) );
    }
}

void store_op(stackelem *stack){

    stackelem *s_index = stack_pop(stack);
    int index = s_index->value.integer;
    free(s_index);

    stackelem *to_store = stack_pop(stack);

    // TODO: handle possible errors
    int result = stack_store_elem(stack, to_store, index);

}

void jump_op(stackelem *stack, stackelem **current_opcode) {

    stackelem *s_offset = stack_pop(stack);
    stackelem *s_condition = stack_pop(stack);

    int offset = s_offset->value.integer;
    free(s_condition);

    if (s_condition->value.integer) {
        for (int i = 0; i<offset; i++)
            *current_opcode = (*current_opcode) -> next;
    }
    free(s_offset);
}

void char_op(stackelem *stack){
    stackelem *s_token = stack_pop(stack);

    int token = s_token->value.integer;

    free(s_token);

    stack_push_int(stack, token);
}

int compile(stackelem *code_segment, char *user_input){

    stackelem* main_stack;
   
    /* [ ] */
    main_stack = code_segment;
    /* 	|
        |
        V
       [ code ] */
    main_stack = stack_push_back_string(main_stack, user_input);
    /*  |
        |
        V
       [ 'user input', code ] */
    main_stack = stack_push_back_pointer(main_stack, main_stack);
    /*  |
        |
	V
     * [ [...], 'user input', code ] */

    stackelem *current_opcode = stack_get(main_stack, 2);
    OPCODE opcode = current_opcode->value.integer;

    while (opcode != AXE){

        switch (opcode) {

            case FOX:
		// DEBUG
		printf("subtracting\n");
                arithmetic_op(main_stack, sub);
                break;

            case ADD:
		// DEBUG
		printf("adding\n");
                arithmetic_op(main_stack, add);
                break;
            
            case ROOSTER:
		// DEBUG
		printf("multiplying\n");
                arithmetic_op(main_stack, mult);
                break;

	    case CHICKEN:
		// DEBUG
		printf("chicken\n");
                chicken_op(main_stack);
                break;

            case PICK:
		// DEBUG
		printf("loading\n");
                load_op(main_stack, current_opcode);
                break;

            case PECK:
		// DEBUG
		printf("storing\n");
                store_op(main_stack);
                break;

            case FR:
		// DEBUG
		printf("jumping\n");
                jump_op(main_stack, &current_opcode);
                break;

            case BBQ:
		// DEBUG
		printf("pushing character\n");
                char_op(main_stack);
                break;

            default:
		// DEBUG
		printf("pushing integer\n");
                ten_or_more_op(main_stack, opcode);
                break;

        }
        opcode = next_opcode(&current_opcode);
    
        print_stack(main_stack);
	char car;
	scanf("%c", &car); // wow this stops the programm
    }

    stackelem *top_of_stack = stack_peek(main_stack);


	print_stackelem(top_of_stack);

    free_stack(main_stack);

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

