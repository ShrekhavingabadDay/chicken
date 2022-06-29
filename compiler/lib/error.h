#ifndef ERROR_H
#define ERROR_H

#include<stdlib.h>

typedef enum {
	SYNTAX,
	OUT_OF_BOUNDS,
	TYPE_MISMATCH,
} ERROR_TYPE;

void show_error(ERROR_TYPE e, char *m);

#endif
