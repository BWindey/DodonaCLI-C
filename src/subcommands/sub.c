#include <stdio.h>

#include "sub.h"


void sub(int argc, const char* argv[]) {
    printf("Hello from sub\n\n");
    printf("Found %d arguments for sub-subcommand:\n", argc - 1);
    for (int i = 0; i < argc; i++) {
        printf("%s, ", argv[i]);
    }
    printf("\n");
}
