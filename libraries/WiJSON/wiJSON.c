#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "wiJSON.h"

// Forward declarations
bool isBlank(const char);
int jumpBlankChars(const char*, unsigned int);

int parseJSONValue	(const char*, unsigned int, wiValue*);
int parseJSONInt	(const char*, unsigned int, wiValue*);
int parseJSONBool	(const char*, unsigned int, wiValue*);
int parseJSONString	(const char*, unsigned int, wiValue*);
int parseJSONFloat	(const char*, unsigned int, wiValue*);
int parseJSONPair	(const char*, unsigned int, wiValue*);
int parseJSONObject	(const char*, unsigned int, wiValue*);
int parseJSONArray	(const char*, unsigned int, wiValue*);
int parseJSONNull	(const char*, unsigned int, wiValue*);


// For testing purposes, a main function.
// TODO: Remove this when done testing
int main() {
	wiValue* root = parseJSON("\"Hello, World!\"");

	printf("RootValue type: %d\n", root->_type);
	printf("WISTRING: %d\n", WISTRING);
	printf("Value: %s\n", root->contents.stringVal);
	freeEverything(root);
}


/*
 * Entrypoint of the library.
 * Takes in the jsonString and returns a wiValue pointer
 */
wiValue* parseJSON(const char *jsonString) {
	wiValue* root = (wiValue*)malloc(sizeof(wiValue));

	// Start index at first non-blank char
	unsigned int index = jumpBlankChars(jsonString, 0);

	parseJSONValue(jsonString, index, root);


	return root;
}


/*
 * Parses a JSON-value, whether it is an int, string, ... or array 
 * (will delegate)
 *
 * WARNING: requires index to point to the right place to start parsing,
 * 		can't be blank!
 *
 * Returns the index of the last parsed character in the jsonString
 */
int parseJSONValue(const char *jsonString, unsigned int index, wiValue* parent) {
	// Enforce we can start parsing
	assert(!isBlank(jsonString[index]));

	switch (jsonString[index]) {
		case '{':	// Object
			break;

		case '[':	// Array
			index = parseJSONArray(jsonString, index, parent);
			break;

		case '"':	// String
			index = parseJSONString(jsonString, index, parent);
			break;

		case 't':	// Boolean (fall-through)
		case 'f':
			index = parseJSONBool(jsonString, index, parent);
			break;

		case 'n': 	// null
			index = parseJSONNull(jsonString, index, parent);
			if (strncmp(jsonString + index, "null", 4) == 0) {

			}
			break;

		default:	// Try for a number
			break;
	}

	return index;
}

int parseJSONInt(const char* jsonString, unsigned int index, wiValue* parent) {
	return index;
}

/*
 * Tries to parse a boolean value and assign it to wiValue* parent contents.
 *
 * Returns the index of the next non-blank character after the boolean value.
 *
 * Asserts if it doesn't find "true" or "false" from the cursor,
 * or when the boolean value is followed by an illegal character (for json).
 */
int parseJSONBool(const char* jsonString, unsigned int index, wiValue* parent) {
	if (strncmp(jsonString + index, "true", 4) == 0) {
		parent->contents.boolVal = true;
		index += 4;		// Length of "true"

	} else {
		assert(strncmp(jsonString + index, "false", 4) == 0);
		parent->contents.boolVal = false;
		index += 5;		// Length of "false"

	}
	parent->_type = WIBOOL;

	char nextCharAfterBool = jsonString[index];
	assert(
			isBlank(nextCharAfterBool) 
			|| nextCharAfterBool == ',' 
			|| nextCharAfterBool == '}' 
			|| nextCharAfterBool == ']'
			|| nextCharAfterBool == '\0'
	);

	return jumpBlankChars(jsonString, index);
}

/*
 * Tries to parse a string and assign it to the wiValue* parent contents.
 * 
 * Returns the index of the next non-blank character after the boolean value.
 *
 * Asserts if the index isn't standing on the opening '"',
 * and when it can't find a closing quote.
 */
int parseJSONString(const char* jsonString, unsigned int index, wiValue* parent) {
	// TODO: still an off-by-one error
	assert(jsonString[index] == '"');

	// Already move index 1 forward, as we don't need the opening quote
	index++;

	// Find closing '"'
	unsigned int closingIndex = index + 1;
	while (jsonString[closingIndex] != '\0' && jsonString[closingIndex] != '"') {
		closingIndex++;
	}

	assert(jsonString[closingIndex] == '"');

	char* stringVal = (char*)malloc(sizeof(char) * (closingIndex - index));
	strncpy(stringVal, jsonString + index, closingIndex - 1 - index);
	stringVal[closingIndex - 1] = '\0';

	parent->_type = WISTRING;
	parent->contents.stringVal = stringVal;

	// To check behind string, we move 1 further from the closing quote
	closingIndex++;

	char nextCharAfterString = jsonString[closingIndex];
	assert(
			isBlank(nextCharAfterString) 
			|| nextCharAfterString == ',' 
			|| nextCharAfterString == '}' 
			|| nextCharAfterString == ']'
			|| nextCharAfterString == '\0'
	);

	return jumpBlankChars(jsonString, closingIndex);
}

int parseJSONFloat(const char* jsonString, unsigned int index, wiValue* parent) {
	return index;
}

int parseJSONPair(const char* jsonString, unsigned int index, wiValue* parent) {
	return index;
}

int parseJSONObject(const char* jsonString, unsigned int index, wiValue* parent) {
	return index;
}

int parseJSONArray(const char* jsonString, unsigned int index, wiValue* parent) {
	return index;
}

/*
 * Tries to parse 'null' (NULL) and asserts if it isn't the case.
 * This can only assert on a non-valid json-string
 *
 * It sets the value and type of the wiValue* parent, 
 * and returns the index of the next non-blank character after "null"
 */
int parseJSONNull(const char* jsonString, unsigned int index, wiValue* parent) {
	assert(strncmp(jsonString + index, "null", 4) == 0);

	parent->_type = WINULL;
	index += 4;

	char nextCharAfterNULL = jsonString[index];
	assert(
			isBlank(nextCharAfterNULL) 
			|| nextCharAfterNULL == ',' 
			|| nextCharAfterNULL == '}' 
			|| nextCharAfterNULL == ']'
			|| nextCharAfterNULL == '\0'
	);

	return jumpBlankChars(jsonString, index);
}

void freeEverything(wiValue* root) {
	// TODO: 
	free(root);
}


/*
 * Returns the index of the first non-blank character
 * starting from 'index'
 */
int jumpBlankChars(const char* string, unsigned int index) {
	while (isBlank(string[index]) && string[index] != '\0') {
		index++;
	}

	return index;
}

bool isBlank(const char c) {
	return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}
