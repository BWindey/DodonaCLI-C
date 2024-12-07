#include <stdio.h>

#include "up.h"


void up(int argc, const char* argv[]) {
    printf("Hello from up\n\n");
    printf("Found %d arguments for up-subcommand:\n", argc - 1);
    for (int i = 0; i < argc; i++) {
        printf("%s, ", argv[i]);
    }
    printf("\n");
}
