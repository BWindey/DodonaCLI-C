#include <stdio.h>

#include "post.h"


void post(int argc, const char* argv[]) {
    printf("Hello from post\n\n");
    printf("Found %d arguments for post-subcommand:\n", argc - 1);
    for (int i = 0; i < argc; i++) {
        printf("%s, ", argv[i]);
    }
    printf("\n");
}
