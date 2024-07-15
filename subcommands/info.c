#include <stdio.h>
#include <string.h>

#include "../constants.h"
#include "info.h"

const char HELP_USAGE[] = "Usage: dodona info [OPTIONS] COMMAND [ARGS]...\n";
const char HELP_INFO[] =
    "  Info about shell-completion, changelog, version, update-availability and\n"
    "  GitHub page.\n"
    "\n"
    "Options:\n"
    "  --help  Show this message and exit.\n"
    "\n"
    "Commands:\n"
    "  changelog     Changelog for the latest version.\n"
    "  check-update  Checks if there is a new update available for DodonaCLI.\n"
    "  completion    Tab completion, very handy for fast use\n"
    "  github        Link to the GitHub page of DodonaCLI.\n"
    "  man-page      Man-pages for DodonaCLI, very professional\n"
    "  version       Display the current version of DodonaCLI.\n";


void info(int argc, const char* argv[]) {
    // Check if any subcommand is used
    if (argc <= 1 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        printf("%s\n%s\n", HELP_USAGE, HELP_INFO);

    } else if (strcmp(argv[1], "version") == 0) {
        printf("DodonaCLI v%s\n", VERSION);

    } else if (strcmp(argv[1], "github") == 0) {
        printf("https://www.github.com/BWindey/DodonaCLI-C\n");

    } else if (strcmp(argv[1], "changelog") == 0) {
        printf("No changelog for now, cause there is no previous version :P\n");

    } else if (strcmp(argv[1], "man-page") == 0) {
        printf(
            "Man-page is the same as for the orignal, look on Github under the man-page folder.\n"
        );

    } else if (strcmp(argv[1], "completion") == 0) {
        printf(
            "You can download a completion-script for bash at the original DodonaCLI Github "
            "(https://github.com/BWindey/DodonaCLI)\n"
        );
    }
}
