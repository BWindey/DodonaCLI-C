#include <stdio.h>

#include "status.h"


void status(int argc, const char* argv[]) {
    printf("Hello from status\n\n");
    printf("Found %d arguments for status-subcommand:\n", argc - 1);
    for (int i = 0; i < argc; i++) {
        printf("%s, ", argv[i]);
    }
    printf("\n");
}
