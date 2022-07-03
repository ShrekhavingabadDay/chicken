#include "error.h"
#include <stdio.h>

void show_error(ERROR_TYPE e, char *op_name){
	printf("\033[0;31m");
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

	printf(" at operand %s\033[0m\n", op_name);
}

void show_syntax_error(const char *buf, int row, int col){
	printf("\033[0;31mSyntax error on row %d\nat character %d:\nExpected chicken, got %s...\033[0m\n", row, col, buf);
}
