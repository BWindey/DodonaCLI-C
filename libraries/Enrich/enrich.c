#include <assert.h>
#include <stdio.h>
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

int main() {
    char msg[] =
        "NUMBER: [NUMBER]Hello, World![/]\n"
        "BOLD: [BOLD]Hello, World![/]\n"
        "DIM: [DIM]Hello, World![/]\n"
        "ITALIC: [ITALIC]Hello, World![/]\n"
        "UNDER: [UNDER]Hello, World![/]\n"
        "STRIKE: [STRIKE]Hello, World![/]\n"
        "BLACK: [BLACK]Hello, World![/]\n"
        "BRIGHT-BLACK: [BRIGHT-BLACK]Hello, World![/]\n"
        "ON BLACK: [ON BLACK]Hello, World![/]\n"
        "ON BRIGHT-BLACK: [ON BRIGHT-BLACK]Hello, World![/]\n"
        "RED: [RED]Hello, World![/]\n"
        "BRIGHT-RED: [BRIGHT-RED]Hello, World![/]\n"
        "ON RED: [ON RED]Hello, World![/]\n"
        "ON BRIGHT-RED: [ON BRIGHT-RED]Hello, World![/]\n"
        "GREEN: [GREEN]Hello, World![/]\n"
        "BRIGHT-GREEN: [BRIGHT-GREEN]Hello, World![/]\n"
        "ON GREEN: [ON GREEN]Hello, World![/]\n"
        "ON BRIGHT-GREEN: [ON BRIGHT-GREEN]Hello, World![/]\n"
        "YELLOW: [YELLOW]Hello, World![/]\n"
        "BRIGHT-YELLOW: [BRIGHT-YELLOW]Hello, World![/]\n"
        "ON YELLOW: [ON YELLOW]Hello, World![/]\n"
        "ON BRIGHT-YELLOW: [ON BRIGHT-YELLOW]Hello, World![/]\n"
        "BLUE: [BLUE]Hello, World![/]\n"
        "BRIGHT-BLUE: [BRIGHT-BLUE]Hello, World![/]\n"
        "ON BLUE: [ON BLUE]Hello, World![/]\n"
        "ON BRIGHT-BLUE: [ON BRIGHT-BLUE]Hello, World![/]\n"
        "MAGENTA: [MAGENTA]Hello, World![/]\n"
        "BRIGHT-MAGENTA: [BRIGHT-MAGENTA]Hello, World![/]\n"
        "ON MAGENTA: [ON MAGENTA]Hello, World![/]\n"
        "ON BRIGHT-MAGENTA: [ON BRIGHT-MAGENTA]Hello, World![/]\n"
        "CYAN: [CYAN]Hello, World![/]\n"
        "BRIGHT-CYAN: [BRIGHT-CYAN]Hello, World![/]\n"
        "ON CYAN: [ON CYAN]Hello, World![/]\n"
        "ON BRIGHT-CYAN: [ON BRIGHT-CYAN]Hello, World![/]\n"
        "WHITE: [WHITE]Hello, World![/]\n"
        "BRIGHT-WHITE: [BRIGHT-WHITE]Hello, World![/]\n"
        "ON WHITE: [ON WHITE]Hello, World![/]\n"
        "ON BRIGHT-WHITE: [ON BRIGHT-WHITE]Hello, World![/]\n";


    printf("Original msg:\n%s\n", msg);
    enrich(msg);
    printf("Enriched msg:\n%s\n", msg);

    return 0;
}

void enrich(char* msg) {
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
                while (strcmp(substring, patterns[keyword].pattern) != 0 && keyword < SUPPORTED_KEYWORDS * 4) {
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
                strncpy(msg + pNew, patterns[keyword].replacement, patterns[keyword].replacementLength);
                pOri += patterns[keyword].patternLength - 1;
                pNew += patterns[keyword].replacementLength - 1;
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