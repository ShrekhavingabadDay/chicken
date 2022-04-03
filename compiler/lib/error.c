#include "error.h"
#include <stdio.h>

// TODO: colors in terminal

void show_error(error *e){
    printf("%s: %s\n", e->origin, e->message);
    free(e);
}