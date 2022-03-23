#ifndef ERROR_H
#define ERROR_H

#include<stdlib.h>

typedef struct {
    char origin[10];
    char message[50];
} error;

void show_error(error *e);

#endif
