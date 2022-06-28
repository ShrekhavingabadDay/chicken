#include "lib/stack.h"
#include <stdio.h>
#include <unistd.h>

// length of the string "chicken"
#define CHN_LEN 7

typedef enum {
        AXE,     // exit
        CHICKEN, // prints chicken
        ADD,     // adds up two top stack values
        FOX,     // subtracts two top stack values
        ROOSTER, // multiplies two top stack values
        COMPARE, // this one's not that hard to guess
        PICK,    // loads from a specified source
        PECK,    // pops and stores given value to stack
        FR,      // jumps to specified position in specified stack
        BBQ      // pushes character as ascii
} OPCODE;

// https://stackoverflow.com/questions/27097915/read-all-data-from-stdin-c
char* read_stdin()
{
    int c;
    size_t p4kB = 4096, i = 0;
    void *newPtr = NULL;
    char *ptrString = malloc(p4kB * sizeof (char));

    while (ptrString != NULL && (c = getchar()) != '\n' && c != EOF)
    {
        if (i == p4kB * sizeof (char))
        {
            p4kB += 4096;
            if ((newPtr = realloc(ptrString, p4kB * sizeof (char))) != NULL)
                ptrString = (char*) newPtr;
            else
            {
                free(ptrString);
                return NULL;
            }
        }
        ptrString[i++] = c;
    }

    if (ptrString != NULL)
    {
        ptrString[i] = '\0';
        ptrString = realloc(ptrString, strlen(ptrString) + 1);
    } 
    else return NULL;

    return ptrString;
}

// TODO: make tokenizer ( a lot ) more sophisticated
stackelem *tokenize(char *filename){

	FILE *input = fopen(filename, "r");
        int row, col = 1;

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

                        if (stack == NULL)
                                stack = create_int_stackelem(chickens);
                        else
			        stack_push_int(stack, chickens);
			chickens = 0;
		}
                prev = c;
        }while(!feof(input));
        // stack_push_int(stack, chickens);

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

/* we need to be able to add two strings together,
   so add won't be called by the "arithmetic_op" wrapper function
*/
void add_strings(stackelem *v, stackelem *s_a, stackelem *s_b){

	string a = s_a->value.str;
	string b = s_b->value.str;

	free(s_a);
	free(s_b);

	string_add(&b, a);
	stack_push_string(v, b);
	string_free(a);

}

void add_integers(stackelem *v, stackelem *s_a, stackelem *s_b){

	int a = s_a->value.integer;
	int b = s_b->value.integer;

	free(s_a);
	free(s_b);

	stack_push_int(v, a+b);

}

void add_arithmetic(stackelem *v){
	
	stackelem *s_a = stack_pop(v);
	stackelem *s_b = stack_pop(v);

	if (
	s_a->value_type == INTEGER &&
	s_b->value_type == INTEGER
	)
		add_integers(v, s_a, s_b);
	else if (
	s_a->value_type == STRING &&
	s_b->value_type == STRING
	)
		add_strings(v, s_a, s_b);

}

int sub(int a, int b){
        return b-a;
}

int divide(int a, int b){
        return b/a;
}

/* TODO: handle possible errors:
	- type mismatches
	- indexing going out of bounds
	- and the ones that i'll only find after it's too late
*/

/*
        "godfather" function of all arithmetic operations
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

/* 
   epic bugfix: load_op takes **opcode instead of *opcode
   for ( by now ) obvious reasons
*/
void load_op(stackelem *stack, stackelem **current_opcode){

        int load_from = next_opcode(current_opcode);

        /* DEBUG:
        printf("loading from: %s\n", (load_from ? "user input" : "stack") );
	*/

        stackelem *popped = stack_pop(stack);
        int index = popped->value.integer;
        free(popped);

	/* DEBUG
        printf("loading element with index %d\n", index);
	*/

        if (!load_from){ // loading from stack in this case
		stackelem *pointer_to_self = stack_get(stack, 0)->value.pointer;
		stackelem *to_add = stack_get( pointer_to_self, index );
                stack_add_elem(stack, to_add );
        }
	else{            // and from user input in this one
                stack_push_string(stack, &(( stack_get(stack, 1) )->value.str[index]) );
        }
}

void store_op(stackelem *stack){

        stackelem *s_index = stack_pop(stack);
        int index = s_index->value.integer;
        free(s_index);

        stackelem *to_store = stack_pop(stack);

        int result = stack_store_elem(stack, to_store, index);

}

/* TODO: jump in negative direction
   - doubly linked list?
   - store head and tail in a struct?
   - ^that's still a surprisingly huge amount of coding
   wasn't anticipating it when i started :O
*/
void jump_op(stackelem *stack, stackelem **current_opcode) {

        stackelem *s_offset = stack_pop(stack);
        stackelem *s_condition = stack_pop(stack);

        int offset = s_offset->value.integer;
        free(s_offset);

        if (s_condition->value.integer) {

		if (offset<0){
			offset = offset*(-1);
			for (int i = 0; i<offset; i++)
				*current_opcode = (*current_opcode)->prev;
		}else

                	for (int i = 0; i<offset; i++)
                        	*current_opcode = (*current_opcode) -> next;
        }
        free(s_condition);
}

void char_op(stackelem *stack){

        stackelem *s_token = stack_pop(stack);

        int token = s_token->value.integer;

        free(s_token);

	char char_token = token;
	// printf("Pushing char %c\n", char_token);

        stack_push_char( stack, char_token );
}

int compile(stackelem *code_segment, char *user_input){

        stackelem* main_stack;
	stackelem *data_segment = stack_peek(code_segment);

        /* [ ] */
        main_stack = code_segment;
        /* 	|
                |
                V
           [ code ] */
        main_stack = stack_push_back_string(main_stack, user_input);
        /*      |
                |
                V
           [ 'user input', code ] */
        main_stack = stack_push_back_pointer(main_stack, main_stack);
        /*      |
                |
	        V
         * [ [...], 'user input', code ] */

        /* 
	   changing the first elements pointer to point to itself
	   this way we'll actually have a pointer to self as the first element
	   (just like in python)
	*/
        stack_get(main_stack, 0)->value.pointer = main_stack;

        stackelem *current_opcode = stack_get(main_stack, 2);
        OPCODE opcode = current_opcode->value.integer;

        while (opcode != AXE){

                switch (opcode) {

                        case FOX:
				/* DEBUG */
				// printf("subtracting\n");
				/**/
                                arithmetic_op(main_stack, sub);
                                break;

                        case ADD:
				/* DEBUG */
				// printf("adding\n");
				/**/
                                add_arithmetic(main_stack);
                                break;
                        
                        case ROOSTER:
				/* DEBUG */
				// printf("multiplying\n");
				/**/
                                arithmetic_op(main_stack, mult);
                                break;

	        	case CHICKEN:
				/* DEBUG */
				// printf("chicken\n");
				/**/
                                chicken_op(main_stack);
                                break;

                        case PICK:
				/* DEBUG */
				// printf("loading\n");
				/**/
                                load_op(main_stack, &current_opcode);
                                break;

                        case PECK:
				/* DEBUG */
				// printf("storing\n");
				/**/
                                store_op(main_stack);
                                break;

                        case FR:
				/* DEBUG */
				// printf("jumping\n");
				/**/
                                jump_op(main_stack, &current_opcode);
                                break;

                        case BBQ:
				/* DEBUG */
				// printf("pushing character\n");
				/**/
                                char_op(main_stack);
                                break;

                        default:
				/* DEBUG */
				// printf("pushing integer\n");
				/**/
                                ten_or_more_op(main_stack, opcode);
                                break;

                }
                opcode = next_opcode(&current_opcode);
       
                //print_stack(data_segment->next);
		//char car;
		//scanf("%c", &car); // wow this stops the programm
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
                compile(program_segment, read_stdin());


	return 0;
}

