#include <stdio.h>
#include <string.h>

#include "prettySTDout.h"


int main() {
    char msg[] = "Hello, [[World]!";

    printf("Original msg:\n%s\n\n", msg);

    enrich(msg);

    printf("Enriched msg:\n%s\n", msg);

    return 0;
}

void enrich(char* msg) {
    unsigned int msgLength = strlen(msg);

    // pOri will be used for the iteration,
    // pNew will be where I put the new characters
    // (replacements will always be smaller then the pattern it relaces)
    unsigned int pOri = 0;
    unsigned int pNew = 0;

    // Iterate over msg-string
    while (msg[pOri] != '\0') {

        // If '[' encountered, see if it is a pattern we need to replace
        if (pOri + MIN_PATTERN_LENGTH < msgLength && msg[pOri] == '[') {

            // If it's escaped using another '[', ignore this
            if (msg[pOri + 1] == '[') {
                pOri++;
            }
        }

        if (pOri > pNew) {
            msg[pNew] = msg[pOri];
        }

        pNew++;
        pOri++;
    }

    msg[pNew] = '\0';
}
