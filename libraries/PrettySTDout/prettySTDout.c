#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "prettySTDout.h"

// [/] is smallest pattern to search for, next smallest is [RED]
#define MIN_PATTERN_LENGTH 3
#define SUPPORTED_KEYWORDS 25


int main() {
    char msg[] = "This is a [[test]] to see if everything works.";

    printf("Original msg:\n | %s\n", msg);
    enrich(msg);
    printf("Enriched msg:\n | %s\n", msg);

    return 0;
}

void enrich(char* msg) {
    unsigned int msgLength = strlen(msg);

    // pOri will be used for the iteration,
    // pNew will be where I put the new characters
    // (replacements will always be smaller then the pattern it relaces)
    unsigned int pOri = 0;
    unsigned int pNew = 0;

    const char* actions[] = {
        "/",
        "NUMBER",
        "BOLD",
        "ITALIC",
        "UNDER",
        "STRIKE",
        "DIM",
        "black",
        "BLACK",
        "blue",
        "BLUE",
        "cyan",
        "CYAN",
        "default",
        "DEFAULT",
        "green",
        "GREEN",
        "magenta",
        "MAGENTA",
        "red",
        "RED",
        "white",
        "WHITE",
        "yellow",
        "YELLOW"
    };

    // Iterate over msg-string
    while (msg[pOri] != '\0') {

        // If '[' encountered, see if it is a pattern we need to replace
        if (pOri + MIN_PATTERN_LENGTH < msgLength && msg[pOri] == '[') {

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

            int keyword = 0;
            while (strcmp(substring, actions[keyword]) != 0 && keyword < SUPPORTED_KEYWORDS) {
                keyword++;
            }
            // Enforce valid keyword
            assert(strcmp(substring, actions[keyword]) == 0);

            switch (keyword) {
                case 0:
                    // [/]: 0
                    strncpy(msg + pNew, "\e[0m", 4);
                    pOri += 2;
                    pNew += 3;
                    // Check if new pointer still is behind,
                    // aka there has been a replacement already
                    assert(pNew <= pOri);
                    break;

                case 16:
                    // [GREEN]: 32
                    strncpy(msg + pNew, "\e[32m", 5);
                    pOri += 6;
                    pNew += 4;
                    break;

                default:
                    break;
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
}
