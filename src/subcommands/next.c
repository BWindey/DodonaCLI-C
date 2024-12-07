#include <stdio.h>

#include "next.h"


void next(int argc, const char* argv[]) {
    printf("Hello from next\n\n");
    printf("Found %d arguments for next-subcommand:\n", argc - 1);
    for (int i = 0; i < argc; i++) {
        printf("%s, ", argv[i]);
    }
    printf("\n");
}
