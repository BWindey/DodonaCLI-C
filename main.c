#include "main.h"


int main(int argc, const char* argv[]) {
    handleArgs(argc, argv);

    return 0;
}


void handleArgs(int argc, const char* argv[]) {
    const char USAGE_MAIN[] = "Usage: dodona [OPTIONS] COMMAND [ARGS]...\n";
    const char HELP_MAIN[] =
        "\n"
        "  A 3rd party Command Line Interface for Dodona. Finally you have no need to\n"
        "  exit your terminal anymore! Use --help for more info about flags, or read\n"
        "  the README on discord.\n"
        "\n"
        "Options:\n"
        "  --help  Show this message and exit.\n"
        "\n"
        "Commands:\n"
        "  display   Display info based on the current selection.\n"
        "  info      Info about shell-completion, changelog, version,...\n"
        "  next      WARNING: might overwrite 'boilerplate' file! Move to the next...\n"
        "  post      Post a solution-file to Dodona.\n"
        "  select    Select based on id or name.\n"
        "  settings  Interactive settings-menu to change some settings\n"
        "  status    Display your current selection.\n"
        "  sub       Get submission data.\n"
        "  tutorial  Start tutorial\n"
        "  up        Deselect default last selected thing.\n";

    const char* first_arg = argv[1];

    // Check for --help flag and then handle subcommands
    if (argc <= 1 || strcmp(first_arg, "--help") == 0 || strcmp(first_arg, "-h") == 0) {
        printf("%s%s", USAGE_MAIN, HELP_MAIN);
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
        printf("%s\nUse '--help' for more info.\n", USAGE_MAIN);
        exit(1);
    }
}
