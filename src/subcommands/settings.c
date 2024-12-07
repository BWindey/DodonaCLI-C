#include <stdio.h>

#include "settings.h"


void settings(int argc, const char* argv[]) {
    printf("Hello from settings\n\n");
    printf("Found %d arguments for settings-subcommand:\n", argc - 1);
    for (int i = 0; i < argc; i++) {
        printf("%s, ", argv[i]);
    }
    printf("\n");
}
