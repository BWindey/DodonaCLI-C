#pragma once

#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int wiFailedTests = 0;

#define _wiTest(EXPECTED_STR, GOT_STR, GOT_VALUE_FORMAT, GOT_VALUE) \
{ \
	wiFailedTests += 1; \
    char* oldLocale = strdup(setlocale(LC_ALL, NULL)); \
    setlocale(LC_ALL, ""); \
    fprintf(stderr, "\033[91;1mAssertion\033[0m: '%s == %s'\n", EXPECTED_STR, GOT_STR); \
    fprintf(stderr, "\u2502 Actual:   " GOT_VALUE_FORMAT "\n", GOT_VALUE); \
    fprintf(stderr, \
            "\u2570 At function '%s', file '%s', line %d\n\n", \
            __func__, __FILE__, __LINE__ \
    ); \
    setlocale(LC_ALL, oldLocale); \
    free(oldLocale); \
}


#define wiTestInt(EXPECTED, GOT) \
{ \
	if ((EXPECTED) != (GOT)) { \
		_wiTest(#EXPECTED, #GOT, "%ld", (long)(GOT)); \
	} \
}
 
#define FLOAT_TOLERANCE 0.0001
#define wiTestFloat(EXPECTED, GOT) \
{ \
	if (fabs((EXPECTED) - (GOT)) > FLOAT_TOLERANCE) { \
		_wiTest(#EXPECTED, #GOT, "%f", (double)GOT); \
	} \
}


#define wiTestBool(EXPECTED, GOT) \
{ \
	if ((EXPECTED) != (GOT)) { \
		const char* expectedStr = (EXPECTED) ? "true" : "false"; \
		const char* gotStr = (GOT) ? "true" : "false"; \
		_wiTest(expectedStr, #GOT, "%s", gotStr); \
	} \
}
 
#define wiTestEnum(EXPECTED, GOT, enumToString) \
{ \
	if ((EXPECTED) != (GOT)) {\
		_wiTest(enumToString(EXPECTED), #GOT, "%s", enumToString(GOT)); \
	} \
}

#define wiTestString(EXPECTED, GOT) \
{ \
	if (strcmp((EXPECTED), (GOT)) != 0) { \
		_wiTest(#EXPECTED, #GOT, "\"%s\"", GOT); \
	} \
}
