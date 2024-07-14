#include <stdio.h>
#include <string.h>

#include "arguments.h"
#include "subcommands/info.h"

int main(int argc, const char* argv[]) {
    handleArgs(argc, argv);

    return 0;
}


void handleArgs(int argc, const char* argv[]) {
    printf("Found %d arguments.\n\n", argc - 1);

    const char* first_arg = argv[1];

    // Check for --help flag and then handle subcommands
    if (argc <= 1 || strcmp(first_arg, "--help") == 0 || strcmp(first_arg, "-h") == 0) {
        printf(HELP);

        return;

    } else if (strcmp(first_arg, "info") == 0) {
        info(argc - 1, &argv[1]);

    }
}
