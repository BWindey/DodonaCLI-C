#pragma once
#include <stdbool.h>	// Required for the boolean value

// Forward declaration needed for cyclic uses
typedef struct wiArrayEl wiArrayEl;
typedef struct wiValue wiValue;
typedef struct wiPair wiPair;

typedef union wiValueContents {
	char* stringVal;
	int intVal;
	double floatVal;
	wiArrayEl* arrayVal;
	wiValue* objVal;
	bool boolVal;
	wiPair* pairVal;
} wiValueContents;

// Represent arrays by elements pointing to eachother
typedef struct wiArrayEl {
	wiValue* elementVal;
	wiArrayEl* nextElement;
} wiArrayEl;

// Represent value's from key-value-pairs
typedef struct wiValue {
	enum wiType {
		WISTRING, WIINT, WIFLOAT, WIARRAY, WIOBECT, WIBOOL, WIPAIR
	} _type;
	wiValueContents contents;

} wiValue;

// Represent key-value pairs pointing to the next for iteration
typedef struct wiPair {
	char* key;
	wiValue* value;
	wiPair* nextPair;
} wiPair;


wiValue* parseJSON(char*);
