#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "info.h"
#include "constants.h"
#include "wiEnrich.h"


const char USAGE_INFO[] = "Usage: dodona info [OPTIONS] COMMAND [ARGS]...\n";
const char HELP_INFO[] =
    "  Info about shell-completion, changelog, version, update-availability and\n"
    "  GitHub page.\n"
    "\n"
    "Options:\n"
    "  --help  Show this message and exit.\n"
    "\n"
    "Commands:\n"
    "  changelog     Changelog for the latest version.\n"
    "  completion    Tab completion, very handy for fast use\n"
    "  github        Link to the GitHub page of DodonaCLI.\n"
    "  man-page      Man-pages for DodonaCLI, very professional\n"
    "  version       Display the current version of DodonaCLI.\n";


void info(int argc, const char* argv[]) {
    // Check if any subcommand is used
    if (argc <= 1 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        printf("%s\n%s\n", USAGE_INFO, HELP_INFO);

    } else if (strcmp(argv[1], "version") == 0) {
        char version[] = VERSION_FORMATTED;
        wiEnrich(version);
        printf("DodonaCLI %s\n", version);

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
    } else {
        printf("%s", USAGE_INFO);
        exit(1);
    }
}
