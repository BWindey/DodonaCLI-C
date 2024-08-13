#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/enrich.h"

// [/] is smallest pattern to search for, next smallest is [RED]
#define MIN_PATTERN_LENGTH 3
#define SUPPORTED_KEYWORDS 38


struct patternStruct {
    char pattern[20];
    char replacement[7];
    int patternLength;
    int replacementLength;
};


void enrich(char* msg) {
    char* noColorTemp = getenv("NO_COLOR");
    char noColor = (noColorTemp == NULL || strlen(noColorTemp) == 0) ? 'F' : 'T';

    char currentlyOpen = 'F';
    unsigned int msgLength = strlen(msg);

    // pOri will be used for the iteration,
    // pNew will be where I put the new characters
    // (replacements will always be smaller then the pattern it relaces)
    unsigned int pOri = 0;
    unsigned int pNew = 0;

    const struct patternStruct patterns[] = {
        {"/",               "\033[0m",    3,  4},
        {"NUMBER",          "\033[1;36m", 8,  7},
        {"BOLD",            "\033[1m",    6,  4},
        {"DIM",             "\033[2m",    5,  4},
        {"ITALIC",          "\033[3m",    8,  4},
        {"UNDER",           "\033[4m",    7,  4},
        {"STRIKE",          "\033[9m",    8,  4},
        {"BLACK",           "\033[30m",   7,  5},
        {"BRIGHT-BLACK",    "\033[90m",   14, 5},
        {"ON BLACK",        "\033[40m",   10, 5},
        {"ON BRIGHT-BLACK", "\033[100m",  17, 6},
        {"RED",             "\033[31m",   5,  5},
        {"BRIGHT-RED",      "\033[91m",   12, 5},
        {"ON RED",          "\033[41m",   8,  5},
        {"ON BRIGHT-RED",   "\033[101m",  15, 6},
        {"GREEN",           "\033[32m",   7,  5},
        {"BRIGHT-GREEN",    "\033[92m",   14, 5},
        {"ON GREEN",        "\033[42m",   10, 5},
        {"ON BRIGHT-GREEN", "\033[102m",  17, 6},
        {"YELLOW",          "\033[33m",   8,  5},
        {"BRIGHT-YELLOW",   "\033[93m",   15, 5},
        {"ON YELLOW",       "\033[43m",   11, 5},
        {"ON BRIGHT-YELLOW","\033[103m",  18, 6},
        {"BLUE",            "\033[34m",   6,  5},
        {"BRIGHT-BLUE",     "\033[94m",   13, 5},
        {"ON BLUE",         "\033[44m",   9,  5},
        {"ON BRIGHT-BLUE",  "\033[104m",  16, 6},
        {"MAGENTA",         "\033[35m",   9,  5},
        {"BRIGHT-MAGENTA",  "\033[95m",   16, 5},
        {"ON MAGENTA",      "\033[45m",   12, 5},
        {"ON BRIGHT-MAGENTA","\033[105m", 19, 6},
        {"CYAN",            "\033[36m",   6,  5},
        {"BRIGHT-CYAN",     "\033[96m",   13, 5},
        {"ON CYAN",         "\033[46m",   9,  5},
        {"ON BRIGHT-CYAN",  "\033[106m",  16, 6},
        {"WHITE",           "\033[37m",   7,  5},
        {"BRIGHT-WHITE",    "\033[97m",   14, 5},
        {"ON WHITE",        "\033[47m",   10, 5},
        {"ON BRIGHT-WHITE", "\033[107m",  17, 6}
    };

    // Iterate over msg-string
    while (msg[pOri] != '\0') {

        // If '[' encountered, see if it is a pattern we need to replace
        if (pOri + MIN_PATTERN_LENGTH <= msgLength && msg[pOri] == '[') {

            int keyword;
            if (msg[pOri + 1] == '[') {
                // Escaped '['
                pOri++;
                msg[pNew] = msg[pOri];

            } else {
                // Find closing ']'
                unsigned int j = pOri + 1;
                while (msg[j] != '\0' && msg[j] != ']') {
                    j++;
                }
                // Enforce closing the brackets
                assert(j != msgLength);

                // Extract substring
                char substring[j - pOri];
                strncpy(substring, msg + pOri + 1, j - pOri - 1);
                substring[j - pOri - 1] = '\0';

                // Check if valid, and which, keyword
                keyword = 0;
                while (strcmp(substring, patterns[keyword].pattern) != 0 && keyword < SUPPORTED_KEYWORDS) {
                    keyword++;
                }
                // Enforce valid keyword
                assert(strcmp(substring, patterns[keyword].pattern) == 0);

                // Don't allow closing nothing (due to in-place nature of this operation)
                if (keyword == 0) {
                    assert(currentlyOpen == 'T');
                    currentlyOpen = 'F';
                } else {
                    currentlyOpen = 'T';
                }

                // Actually replace
                if (noColor == 'F') {
                    strncpy(msg + pNew, patterns[keyword].replacement, patterns[keyword].replacementLength);
                    pOri += patterns[keyword].patternLength - 1;
                    pNew += patterns[keyword].replacementLength - 1;
                } else {
                    pOri += patterns[keyword].patternLength;
                    msg[pNew] = msg[pOri];
                }
            }
        } else if (pOri > pNew) {
            msg[pNew] = msg[pOri];
        }


        pNew++;
        pOri++;
    }

    if (pOri > pNew) {
        msg[pNew] = '\0';
    }

    // Enforce closed
    assert(currentlyOpen == 'F');
}
