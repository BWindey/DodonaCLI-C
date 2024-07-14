#include <stdio.h>

#include "display.h"


void display(int argc, const char* argv[]) {
    printf("Hello from display\n\n");
    printf("Found %d arguments for info-subcommand:\n", argc - 1);
    for (int i = 0; i < argc; i++) {
        printf("%s, ", argv[i]);
    }
    printf("\n");
}
