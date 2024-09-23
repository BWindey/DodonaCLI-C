#pragma once

#include <ctype.h>
#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int wiFailedTests = 0;

#define _wiTest(EXPECTED_STR, GOT_STR, GOT_VALUE_FORMAT, GOT_VALUE, ...) \
{ \
	wiFailedTests += 1; \
    char* oldLocale = strdup(setlocale(LC_ALL, NULL)); \
    setlocale(LC_ALL, ""); \
    fprintf(stderr, "\033[91;1mFailed test\033[0m: '%s == %s'\n", EXPECTED_STR, GOT_STR); \
    fprintf(stderr, "\u2502 Actual:     " GOT_VALUE_FORMAT "\n", GOT_VALUE); \
	\
	char* varArgs[] = { __VA_ARGS__ "", "" }, **n; \
	n = varArgs; \
	if (strcmp(*n, "") != 0) { \
		fprintf(stderr, "\u2502 Message:\n"); \
	} \
	while (strcmp(*n, "") != 0) { \
		fprintf(stderr, "\u2502 \t%s\n", *n); \
		n++; \
	} \
	\
    fprintf(stderr, \
            "\u2570 At function '%s', file '%s', line %d\n\n", \
            __func__, __FILE__, __LINE__ \
    ); \
    setlocale(LC_ALL, oldLocale); \
    free(oldLocale); \
}


#define wiTestInt(EXPECTED, GOT, ...) \
{ \
	long exp = (long)(EXPECTED); \
	long got = (long)(GOT); \
	if (exp != got) { \
		_wiTest(#EXPECTED, #GOT, "%ld", got, __VA_ARGS__); \
	} \
}
 
#ifndef FLOAT_TOLERANCE
#define FLOAT_TOLERANCE 1e-5
#endif // !FLOAT_TOLERANCE

#define wiTestFloat(EXPECTED, GOT, ...) \
{ \
	double expected = (double)(EXPECTED); \
	double got = (double)(GOT); \
	if (fabs(expected - got) > FLOAT_TOLERANCE) { \
		_wiTest(#EXPECTED, #GOT, "%f", got, __VA_ARGS__); \
	} \
}


#define wiTestBool(EXPECTED, GOT, ...) \
{ \
	bool expected = (bool)(EXPECTED); \
	bool got = (bool)(GOT); \
	if (expected != got) { \
		const char* expectedStr = expected ? "true" : "false"; \
		const char* gotStr = got ? "true" : "false"; \
		_wiTest(expectedStr, #GOT, "%s", gotStr, __VA_ARGS__); \
	} \
}
 
#define wiTestEnum(EXPECTED, GOT, enumToString, ...) \
{ \
	if ((EXPECTED) != (GOT)) {\
		_wiTest(enumToString(EXPECTED), #GOT, "%s", enumToString(GOT), __VA_ARGS__); \
	} \
}

#define wiTestString(EXPECTED, GOT, ...) \
{ \
	if (strcmp((EXPECTED), (GOT)) != 0) { \
		_wiTest(#EXPECTED, #GOT, "\"%s\"", (GOT), __VA_ARGS__); \
	} \
}

#define wiTestChar(EXPECTED, GOT, ...) \
{ \
	char expected = (char)(EXPECTED); \
	char got = (char)(GOT); \
	if (expected != got) { \
		_wiTest(#EXPECTED, #GOT, "'%c'", got, __VA_ARGS__); \
	} \
}

/*
 * A standard way to print the results.
 * 
 * char* testName: name of test used to print out the messages.
 */
static inline void wiPrintResult(char* testName) {
	if (wiFailedTests > 1) {
		printf("%d %s tests \033[91mfailed\033[0m.\n\n", wiFailedTests, testName);

	} else if (wiFailedTests == 1) {
		printf("1 %s test \033[91mfailed\033[0m.\n\n", testName);

	} else {
		printf(
			"%c%s tests \033[92mpassed\033[0m.\n\n", 
			toupper(testName[0]),
			&testName[1]
		);
	}
}
