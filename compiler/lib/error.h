#ifndef ERROR_H
#define ERROR_H

typedef enum {
	NONE,
	SYNTAX,
	OUT_OF_BOUNDS,
	TYPE_MISMATCH,
} ERROR_TYPE;

void show_error(ERROR_TYPE e, char *op_name);
void show_syntax_error(const char *buf, int row, int col);
void panic();

#endif
