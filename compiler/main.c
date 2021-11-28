#include "stack.h"
#include "tokenizer.h"

char* helloworld_path = "/data/data/com.termux/files/home/coding/c/chicken/chicken_code/quine.chicken";

int main(void){
	stackelem *token_stack = tokenize(helloworld_path);
	print_stack(token_stack);
	free_stack(token_stack);
	return 0;
}
