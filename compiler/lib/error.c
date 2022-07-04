#include "error.h"
#include <stdio.h>
#include <stdlib.h>

// ansi escape code for red
const char *RED   = "\033[0;31m";
const char *RESET = "\033[0m";

void show_error(ERROR_TYPE e, char *op_name){
	printf("%s", RED);
	switch (e) {
		case SYNTAX:
			printf("Syntax error");
			break;
		case OUT_OF_BOUNDS:
			printf("Index out of bounds error");
			break;
		case TYPE_MISMATCH:
			printf("Invalid type error");
			break;
		default:
			break;
	}

	printf(" at operand %s%s\n", op_name, RESET);
}

void show_syntax_error(const char *buf, int row, int col){
	printf("%sSyntax error on row %d\nat character %d:\nExpected chicken, got %s...%s\n", RED, row, col, buf, RESET);
}

void panic(){
	printf("%sbruh%s", RED, RESET);
	exit(1);
}
