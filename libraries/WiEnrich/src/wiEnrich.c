#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/wiEnrich.h"
#include "../../WiTesting/wiAssert.h"

/* [/] is smallest pattern to search for, next smallest is [FRED] */
#define MIN_PATTERN_LENGTH 3
#define SUPPORTED_KEYWORDS 38


struct patternStruct {
    char pattern[18]; 		/* Longest pattern + '\0' */
    char replacement[7];	/* Longest replacement + '\0' */
    int pattern_length;		/* Pattern length, including [] */
    int replacement_length;
};


/*
 * Find all [PATTERNS] and replace them with their ansi escape values.
 * Lots of assertions possible, look at the README for more info,
 * and a list of possible PATTERNS.
 * This function returns a pointer to the same in-place-modified string for
 * ease of use, but remember to not pass a string literal to the function!
 * String literarls are read-only, so the function will seg-fault while
 * trying to modify the string in-place.
 * You can use strdup() to get around this issue.
 */
char* wiEnrich(char* msg) {
    char* no_color_char = getenv("NO_COLOR");
    bool NO_COLOR = !(no_color_char == NULL || strlen(no_color_char) == 0);

    bool currently_open = false;
    unsigned int msg_length = strlen(msg);

    // pOri will be used for the iteration,
    // pNew will be where I put the new characters
    // (replacements will always be smaller then the pattern it relaces)
    unsigned int p_ori = 0;
    unsigned int p_new = 0;

    struct patternStruct patterns[] = {
        {"/",               "\033[0m",    3,  4},
        {"BOLD",            "\033[1m",    6,  4},
        {"DIM",             "\033[2m",    5,  4},
        {"ITALIC",          "\033[3m",    8,  4},
        {"UNDER",           "\033[4m",    7,  4},
        {"STRIKE",          "\033[9m",    8,  4},
        {"NUMBER",          "\033[1;36m", 8,  7},
        {"BLACK",           "\033[30m",   7,  5},
        {"BRIGHT-BLACK",    "\033[90m",   14, 5},
        {"ON BLACK",        "\033[40m",   10, 5},
        {"ON BRIGHT-BLACK", "\033[100m",  17, 6},
        {"FRED",            "\033[31m",   6,  5},
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
    while (msg[p_ori] != '\0') {
        // If '[' encountered, see if it is a pattern we need to replace
        if (p_ori + MIN_PATTERN_LENGTH <= msg_length && msg[p_ori] == '[') {

            int keyword_index;
            if (msg[p_ori + 1] == '[') {
                // Escaped '['
                p_ori++;
                msg[p_new] = msg[p_ori];

            } else {
                // Find closing ']'
                unsigned int j = p_ori + 1;
                while (msg[j] != '\0' && msg[j] != ']') {
                    j++;
                }
                wiAssert(j != msg_length, "Couldn't find closing bracket");

                // Extract substring
                char substring[j - p_ori];
                strncpy(substring, msg + p_ori + 1, j - p_ori - 1);
                substring[j - p_ori - 1] = '\0';

                // Check if valid, and which, keyword
                keyword_index = 0;
                while (
					strcmp(substring, patterns[keyword_index].pattern) != 0
					&& keyword_index < SUPPORTED_KEYWORDS
				) {
                    keyword_index++;
                }
                wiAssert(
					strcmp(substring, patterns[keyword_index].pattern) == 0,
					"Not a valid keyword:",
					substring,
				);

                // Don't allow closing nothing (due to in-place nature of this operation)
                if (keyword_index == 0) {
                    wiAssert(
						currently_open == true,
						"Not allowed to use [/] without previous pattern"
					);
                    currently_open = false;
                } else {
                    currently_open = true;
                }

                // Actually replace, when $NO_COLOR, only do bold, italic, ...
                if (NO_COLOR == false || keyword_index < 6) {
                    strncpy(
						msg + p_new,
						patterns[keyword_index].replacement,
						patterns[keyword_index].replacement_length
					);
                    p_ori += patterns[keyword_index].pattern_length - 1;
                    p_new += patterns[keyword_index].replacement_length - 1;
                } else {
                    p_ori += patterns[keyword_index].pattern_length;
                    msg[p_new] = msg[p_ori];
                }
            }
        } else if (p_ori > p_new) {
            msg[p_new] = msg[p_ori];
        }

        p_new++;
        p_ori++;
    }

	msg[p_new] = '\0';

	return msg;
}
