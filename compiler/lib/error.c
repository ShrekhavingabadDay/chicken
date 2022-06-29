#include "error.h"
#include <stdio.h>
#include <stdlib.h>

// TODO: colors in terminal

void show_error(ERROR_TYPE e, char *message){
	switch (e) {
		case SYNTAX:
			printf("Syntax error:\n");
			break;
		case OUT_OF_BOUNDS:
			printf("Index out of bounds error:\n");
			break;
		case TYPE_MISMATCH:
			printf("Invalid type error:\n");
			break;
		default:
			break;
	}

	printf("%s\n", message);
	exit(1);
}
