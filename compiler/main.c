#include "lib/stack.h"
#include "lib/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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

char* opcode_to_string(OPCODE o){
	switch (o) {
		case AXE:
			return "AXE";
		case CHICKEN:
			return "CHICKEN";
		case ADD:
			return "ADD";
		case FOX:
			return "FOX";
		case ROOSTER:
			return "ROOSTER";
		case COMPARE:
			return "COMPARE";
		case PICK:
			return "PICK";
		case PECK:
			return "PECK";
		case FR:
			return "FR";
		case BBQ:
			return "BBQ";
		default:
			/*
			chicken lore:

			10+ operand *pushes* number onto stack
					^
			      chickens push eggs out of their cloaca
					      ^
				  this was my thought process

			thanks for coming to my TED talk!
			 */
			return "EGG";
	}
}

// https://stackoverflow.com/questions/27097915/read-all-data-from-stdin-c
char* read_stdin()
{
    int c;
    size_t p4kB = 4096, i = 0;
    void *newPtr = NULL;
    char *ptrString = malloc(p4kB * sizeof (char));

    while (ptrString != NULL && (c = getchar()) != EOF)
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

void parser_error(const char *buf, int row, int col){
	show_syntax_error(buf, row, col);
	exit(1);
}

void clear_buffer(char *buf, int *fill){
	while(*fill)
		buf[(*fill)--] = 0;
}

stackelem *tokenize(char *filename){

	char buf[CHN_LEN+1];
	int buf_index = 0;

	FILE *input = fopen(filename, "r");
        int row = 1, col = 1;

        if (input == NULL)
                panic();
	
	char c, prev = 0;
	int chickens = 0;

        stackelem *stack = NULL;
	do{
                c = getc(input);
		col++;
		buf[buf_index++] = c;
		if (buf_index == CHN_LEN+2){
			buf[CHN_LEN] = 0;
			parser_error(buf, row, col);
		}

		switch (c) {
		case ' ':
			chickens++;
			buf[--buf_index] = '\0';
			if (strcmp("chicken", buf))
				parser_error(buf, row, col);
			clear_buffer(buf, &buf_index);
			break;
		case '\n':
			row++;
			col = 0;

			if (prev != '\n') {
				chickens++;
				buf[--buf_index]=0;
				if (strcmp("chicken", buf))
						parser_error(buf, row, col);
			}

			stack = stack_push_int(stack, chickens);
			chickens = 0;
			clear_buffer(buf, &buf_index);
			break;
		case '#':
			do{
				c = getc(input);
				clear_buffer(buf, &buf_index);
			} while (c != '\n');
			break;
		default:
			break;

		}
                prev = c;
        }while(!feof(input));

        stack_push_int(stack, AXE);

	fclose(input);
	return stack;
}

void error_handler(ERROR_TYPE error_type, OPCODE instruction, stackelem *stack){
	if (error_type == NONE) return;
	show_error(error_type, opcode_to_string(instruction));
	free_stack(stack);
	exit(1);
}

/* ----- functions used at compilation ----- */

OPCODE next_opcode(stackelem **current_opcode){
        *current_opcode = (*current_opcode)->next;
        return (*current_opcode)->value.integer;
}

int of_same_type(stackelem *a, stackelem *b, VALUE_TYPE t){
	return (
		a->value_type == b->value_type &&
		a->value_type == t
	);
}

int mult(int a, int b){
        return a*b;
}

int sub(int a, int b){
        return b-a;
}

int divide(int a, int b){
        return b/a;
}

ERROR_TYPE add_strings(stackelem *v, stackelem *s_a, stackelem *s_b){

	if (!of_same_type(s_a, s_b, STRING)) return TYPE_MISMATCH;

	string a = s_a->value.str;
	string b = s_b->value.str;

	free(s_a);
	free(s_b);

	string_add(&b, a);
	stack_push_string(v, b);
	string_free(a);

	return NONE;

}

ERROR_TYPE add_integers(stackelem *v, stackelem *s_a, stackelem *s_b){

	if (!of_same_type(s_a, s_b, INTEGER)) return TYPE_MISMATCH;

	int a = s_a->value.integer;
	int b = s_b->value.integer;

	free(s_a);
	free(s_b);

	stack_push_int(v, a+b);

	return NONE;
}

ERROR_TYPE add_arithmetic(stackelem *v){
	
	stackelem *s_a = stack_pop(v);
	stackelem *s_b = stack_pop(v);

	if (
	s_a->value_type == INTEGER &&
	s_b->value_type == INTEGER
	)
		return add_integers(v, s_a, s_b);
	else if (
	s_a->value_type == STRING &&
	s_b->value_type == STRING
	)
		return add_strings(v, s_a, s_b);

	return NONE;
}

ERROR_TYPE arithmetic_op(stackelem *v, int (*op)(int, int) ){
        
        stackelem *s_a = stack_pop(v);
        stackelem *s_b = stack_pop(v);

	if (!of_same_type(s_a, s_b, INTEGER)) return TYPE_MISMATCH;

        int a = s_a->value.integer;
        int b = s_b->value.integer;

        free(s_a);
        free(s_b);

        stack_push_int(v, op(a,b) );

	return NONE;

}

void chicken_op(stackelem *v){
        stack_push_string(v, "chicken");
}

void ten_or_more_op(stackelem *v, int amount){
        stack_push_int(v, amount-10);
}

ERROR_TYPE load_op(stackelem *stack, stackelem **current_opcode){

        int load_from = next_opcode(current_opcode);


        stackelem *popped = stack_pop(stack);
	if (popped->value_type != INTEGER) return TYPE_MISMATCH;
        int index = popped->value.integer;
        free(popped);


        if (!load_from){ // loading from stack in this case
		stackelem *pointer_to_self = stack_get(stack, 0)->value.pointer;
		stackelem *to_add = stack_get( pointer_to_self, index );
		if (to_add == NULL) return OUT_OF_BOUNDS;
                stack_add_elem(stack, to_add );
        }
	else             // and from user input in this one
                stack_push_string(stack, &(( stack_get(stack, 1) )->value.str[index]) );
	return NONE;
}

ERROR_TYPE store_op(stackelem *stack){

        stackelem *s_index = stack_pop(stack);
	if (s_index->value_type != INTEGER) return TYPE_MISMATCH;
        int index = s_index->value.integer;
        free(s_index);

        stackelem *to_store = stack_pop(stack);

        return stack_store_elem(stack, to_store, index);

}

ERROR_TYPE jump_op(stackelem *stack, stackelem **current_opcode) {

        stackelem *s_offset = stack_pop(stack);
        stackelem *s_condition = stack_pop(stack);

	if (s_offset->value_type != INTEGER ||
	    s_condition->value_type != INTEGER) return TYPE_MISMATCH;

        int offset = s_offset->value.integer;
        free(s_offset);

        if (s_condition->value.integer) {

		if (offset<0){
			offset = offset*(-1);
			for (int i = 0; i<offset; i++){
				if (current_opcode == NULL)
					return OUT_OF_BOUNDS;
				*current_opcode = (*current_opcode)->prev;
			}
		}else
                	for (int i = 0; i<offset; i++){
				if (current_opcode == NULL)
					return OUT_OF_BOUNDS;
                        	*current_opcode = (*current_opcode) -> next;
			}
        }
        free(s_condition);
	return NONE;
}

ERROR_TYPE char_op(stackelem *stack){

        stackelem *s_token = stack_pop(stack);
	if (s_token->value_type != INTEGER) return TYPE_MISMATCH;

        int token = s_token->value.integer;

        free(s_token);

	char char_token = token;

        stack_push_char( stack, char_token );

	return NONE;
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

        stack_get(main_stack, 0)->value.pointer = main_stack;

        stackelem *current_opcode = stack_get(main_stack, 2);
        OPCODE opcode = current_opcode->value.integer;

        while (opcode != AXE){

		ERROR_TYPE err = NONE;

                switch (opcode) {

                        case FOX:
                                err = arithmetic_op(main_stack, sub);
                                break;

                        case ADD:
                                err = add_arithmetic(main_stack);
                                break;
                        
                        case ROOSTER:
                                err = arithmetic_op(main_stack, mult);
                                break;

	        	case CHICKEN:
                                chicken_op(main_stack);
                                break;

                        case PICK:
                                err = load_op(main_stack, &current_opcode);
                                break;

                        case PECK:
                                err = store_op(main_stack);
                                break;

                        case FR:
                                err = jump_op(main_stack, &current_opcode);
                                break;

                        case BBQ:
                                err = char_op(main_stack);
                                break;

                        default:
                                ten_or_more_op(main_stack, opcode);
                                break;

                }
		error_handler(err, opcode, main_stack);
		err = NONE;
                opcode = next_opcode(&current_opcode);
		// print_stack(data_segment->next);

		/*
		char c;
		scanf("%c", &c);*/
       
        }

        stackelem *top_of_stack = stack_peek(main_stack);

	print_stackelem(top_of_stack, 0);

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

	if (!isatty(STDIN_FILENO))
        	compile(program_segment, read_stdin());
	else
		compile(program_segment, NULL);


	return 0;
}

