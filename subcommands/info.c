#include <stdio.h>

#include "info.h"


void info(int argc, const char* argv[]) {
    printf("Hello from info\n\n");
    printf("Found %d arguments for info-subcommand.\n\n", argc - 1);
    for (int i = 0; i < argc; i++) {
        printf("%s", argv[i]);
    }
}
