#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "arguments.h"
#include "subcommands/display.h"
#include "subcommands/info.h"
#include "subcommands/next.h"
#include "subcommands/post.h"
#include "subcommands/select.h"
#include "subcommands/settings.h"
#include "subcommands/status.h"
#include "subcommands/sub.h"
#include "subcommands/tutorial.h"
#include "subcommands/up.h"


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

    } else if (strcmp(first_arg, "display") == 0) {
        display(argc - 1, &argv[1]);

    } else if (strcmp(first_arg, "info") == 0) {
        info(argc - 1, &argv[1]);

    } else if (strcmp(first_arg, "next") == 0) {
        next(argc - 1, &argv[1]);

    } else if (strcmp(first_arg, "post") == 0) {
        post(argc - 1, &argv[1]);

    } else if (strcmp(first_arg, "select") == 0) {
        select(argc - 1, &argv[1]);

    } else if (strcmp(first_arg, "settings") == 0) {
        settings(argc - 1, &argv[1]);

    } else if (strcmp(first_arg, "status") == 0) {
        status(argc - 1, &argv[1]);

    } else if (strcmp(first_arg, "sub") == 0) {
        sub(argc - 1, &argv[1]);

    } else if (strcmp(first_arg, "tutorial") == 0) {
        tutorial(argc - 1, &argv[1]);

    } else if (strcmp(first_arg, "up") == 0) {
        up(argc - 1, &argv[1]);

    } else {
        printf("Usage: dodona [OPTIONS] COMMAND [ARGS]...\nUse '--help' for more info.\n");
        exit(1);
    }
}
