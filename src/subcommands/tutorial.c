#include <stdio.h>

#include "tutorial.h"


void tutorial(int argc, const char* argv[]) {
    printf("Hello from tutorial\n\n");
    printf("Found %d arguments for tutorial-subcommand:\n", argc - 1);
    for (int i = 0; i < argc; i++) {
        printf("%s, ", argv[i]);
    }
    printf("\n");
}
