#include <stdio.h>

#include "select.h"


void select(int argc, const char* argv[]) {
    printf("Hello from select\n\n");
    printf("Found %d arguments for select-subcommand:\n", argc - 1);
    for (int i = 0; i < argc; i++) {
        printf("%s, ", argv[i]);
    }
    printf("\n");
}
