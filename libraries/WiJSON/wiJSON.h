#pragma once
#include <stdbool.h>	// Required for the boolean value

// Forward declaration needed for cyclic uses
typedef struct wiArrayEl wiArrayEl;
typedef struct wiValue 	 wiValue;
typedef struct wiPair 	 wiPair;

typedef union wiValueContents {
	long intVal;
	bool boolVal;
	char* stringVal;
	double floatVal;
	wiPair* pairVal;
	wiValue* objVal;
	wiArrayEl* arrayVal;
} wiValueContents;

typedef enum wiType {
	WISTRING, WIINT, WIFLOAT, WIARRAY, WIOBJECT, WIBOOL, WIPAIR, WINULL
} wiType;

// Represent value's from key-value-pairs
typedef struct wiValue {
	wiType _type;
	wiValueContents contents;
} wiValue;

// Represent arrays by elements pointing to eachother
typedef struct wiArrayEl {
	wiValue* elementVal;
	wiArrayEl* nextElement;
} wiArrayEl;

// Represent key-value pairs pointing to the next for iteration
typedef struct wiPair {
	char* key;
	wiValue* value;
	wiPair* nextPair;
} wiPair;


wiValue* parseJSON(const char*);
void freeEverything(wiValue* root);
