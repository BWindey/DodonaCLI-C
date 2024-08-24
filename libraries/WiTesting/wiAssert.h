#pragma once

#include <ctype.h>
#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _wiAssert(EXPECTED_STR, GOT_STR, GOT_VALUE_FORMAT, GOT_VALUE, ...) \
{ \
    char* oldLocale = strdup(setlocale(LC_ALL, NULL)); \
    setlocale(LC_ALL, ""); \
    fprintf(stderr, "\033[91;1mAssertion\033[0m: '%s == %s'\n", EXPECTED_STR, GOT_STR); \
    fprintf(stderr, "\u2502 Actual:   " GOT_VALUE_FORMAT "\n", GOT_VALUE); \
	\
	char* varArgs[] = { __VA_ARGS__ "", "" }, **n; \
	n = varArgs; \
	while (strcmp(*n, "") != 0) { \
		fprintf(stderr, "\u2502 Message: \"%s\"\n", *n); \
		n++; \
	} \
	\
    fprintf(stderr, \
            "\u2570 At function '%s', file '%s', line %d\n\n", \
            __func__, __FILE__, __LINE__ \
    ); \
    setlocale(LC_ALL, oldLocale); \
    free(oldLocale); \
	abort(); \
}


#define wiAssertInt(EXPECTED, GOT, ...) \
{ \
	if ((EXPECTED) != (GOT)) { \
		_wiAssert(#EXPECTED, #GOT, "%ld", (long)(GOT), __VA_ARGS__); \
	} \
}
 
#ifndef FLOAT_TOLERANCE
#define FLOAT_TOLERANCE 1e-5
#endif // !FLOAT_TOLERANCE

#define wiAssertFloat(EXPECTED, GOT, ...) \
{ \
	if (fabs((EXPECTED) - (GOT)) > FLOAT_TOLERANCE) { \
		_wiAssert(#EXPECTED, #GOT, "%f", (double)(GOT), __VA_ARGS__); \
	} \
}


#define wiAssertBool(EXPECTED, GOT, ...) \
{ \
	if ((EXPECTED) != (GOT)) { \
		const char* expectedStr = (EXPECTED) ? "true" : "false"; \
		const char* gotStr = (GOT) ? "true" : "false"; \
		_wiAssert(expectedStr, #GOT, "%s", gotStr, __VA_ARGS__); \
	} \
}
 
#define wiAssertEnum(EXPECTED, GOT, enumToString, ...) \
{ \
	if ((EXPECTED) != (GOT)) {\
		_wiAssert(enumToString(EXPECTED), #GOT, "%s", enumToString(GOT), __VA_ARGS__); \
	} \
}

#define wiAssertString(EXPECTED, GOT, ...) \
{ \
	if (strcmp((EXPECTED), (GOT)) != 0) { \
		_wiAssert(#EXPECTED, #GOT, "\"%s\"", (GOT), __VA_ARGS__); \
	} \
}

#define wiAssertChar(EXPECTED, GOT, ...) \
{ \
	if ((EXPECTED) != (GOT)) { \
		_wiAssert(#EXPECTED, #GOT, "'%c'", (char)(GOT), __VA_ARGS__); \
	} \
}
