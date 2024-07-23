#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "enrich.h"

// [/] is smallest pattern to search for, next smallest is [RED]
#define MIN_PATTERN_LENGTH 3
#define SUPPORTED_KEYWORDS 38

// #define DEBUG


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
        {"/",               "\e[0m",    3,  4},
        {"NUMBER",          "\e[1;36m", 8,  7},
        {"BOLD",            "\e[1m",    6,  4},
        {"DIM",             "\e[2m",    5,  4},
        {"ITALIC",          "\e[3m",    8,  4},
        {"UNDER",           "\e[4m",    7,  4},
        {"STRIKE",          "\e[9m",    8,  4},
        {"BLACK",           "\e[30m",   7,  5},
        {"BRIGHT-BLACK",    "\e[90m",   14, 5},
        {"ON BLACK",        "\e[40m",   10, 5},
        {"ON BRIGHT-BLACK", "\e[100m",  17, 6},
        {"RED",             "\e[31m",   5,  5},
        {"BRIGHT-RED",      "\e[91m",   12, 5},
        {"ON RED",          "\e[41m",   8,  5},
        {"ON BRIGHT-RED",   "\e[101m",  15, 6},
        {"GREEN",           "\e[32m",   7,  5},
        {"BRIGHT-GREEN",    "\e[92m",   14, 5},
        {"ON GREEN",        "\e[42m",   10, 5},
        {"ON BRIGHT-GREEN", "\e[102m",  17, 6},
        {"YELLOW",          "\e[33m",   8,  5},
        {"BRIGHT-YELLOW",   "\e[93m",   15, 5},
        {"ON YELLOW",       "\e[43m",   11, 5},
        {"ON BRIGHT-YELLOW","\e[103m",  18, 6},
        {"BLUE",            "\e[34m",   6,  5},
        {"BRIGHT-BLUE",     "\e[94m",   13, 5},
        {"ON BLUE",         "\e[44m",   9,  5},
        {"ON BRIGHT-BLUE",  "\e[104m",  16, 6},
        {"MAGENTA",         "\e[35m",   9,  5},
        {"BRIGHT-MAGENTA",  "\e[95m",   16, 5},
        {"ON MAGENTA",      "\e[45m",   12, 5},
        {"ON BRIGHT-MAGENTA","\e[105m", 19, 6},
        {"CYAN",            "\e[36m",   6,  5},
        {"BRIGHT-CYAN",     "\e[96m",   13, 5},
        {"ON CYAN",         "\e[46m",   9,  5},
        {"ON BRIGHT-CYAN",  "\e[106m",  16, 6},
        {"WHITE",           "\e[37m",   7,  5},
        {"BRIGHT-WHITE",    "\e[97m",   14, 5},
        {"ON WHITE",        "\e[47m",   10, 5},
        {"ON BRIGHT-WHITE", "\e[107m",  17, 6}
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

                #ifdef DEBUG
                printf("\nFound keyword: %s\n", substring);
                #endif

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

        #ifdef DEBUG
        printf("After %i/%i: '%s'\e[0m\n", pOri, msgLength, msg);
        #endif
    }

    if (pOri > pNew) {
        msg[pNew] = '\0';
    }

#ifdef DEBUG
    printf("After loop: %s\e[0m\n\n", msg);
#endif

    // Enforce closed
    assert(currentlyOpen == 'F');
}
