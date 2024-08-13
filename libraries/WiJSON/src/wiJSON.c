#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/wiJSON.h"

// Forward declarations
bool isBlank(const char);
unsigned int jumpBlankChars(const char*, unsigned int);

unsigned int parseArray	(const char*, unsigned int, wiValue*);
unsigned int parseBool	(const char*, unsigned int, wiValue*);
unsigned int parseNull	(const char*, unsigned int, wiValue*);
unsigned int parseNumber(const char*, unsigned int, wiValue*);
unsigned int parsePair	(const char*, unsigned int, wiValue*);
unsigned int parseString(const char*, unsigned int, wiValue*);
unsigned int parseValue	(const char*, unsigned int, wiValue*);


/*
 * Entrypoint of the library.
 * Takes in the jsonString and returns a wiValue pointer
 */
wiValue* parseJSONString(const char *jsonString) {
	wiValue* root = (wiValue*)malloc(sizeof(wiValue));

	// Start index at first non-blank char
	unsigned int index = jumpBlankChars(jsonString, 0);

	parseValue(jsonString, index, root);

	return root;
}


/*
 * Parses a JSON-value, whether it is an int, string, ... or array 
 * (will delegate)
 *
 * Returns the index of the last parsed character in the jsonString
 */
unsigned int parseValue(const char *jsonString, unsigned int index, wiValue* parent) {
	// Enforce we can start parsing
	assert(!isBlank(jsonString[index]));

	switch (jsonString[index]) {
		case '{':	// Object
			index = parsePair(jsonString, index, parent);
			break;

		case '[':	// Array
			index = parseArray(jsonString, index, parent);
			break;

		case '"':	// String
			index = parseString(jsonString, index, parent);
			break;

		case 't':	// Boolean (fall-through)
		case 'f':
			index = parseBool(jsonString, index, parent);
			break;

		case 'n': 	// null
			index = parseNull(jsonString, index, parent);
			break;

		default:	// Try for a number
			index = parseNumber(jsonString, index, parent);
			break;
	}

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
unsigned int parseBool(const char* jsonString, unsigned int index, wiValue* parent) {
	if (strncmp(jsonString + index, "true", 4) == 0) {
		parent->contents.boolVal = true;
		index += 4;		// Length of "true"

	} else {
		assert(strncmp(jsonString + index, "false", 4) == 0);
		parent->contents.boolVal = false;
		index += 5;		// Length of "false"

	}
	parent->_type = WIBOOL;

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
unsigned int parseString(const char* jsonString, unsigned int index, wiValue* parent) {
	assert(jsonString[index] == '"');

	// Already move index 1 forward, as we don't need the opening quote
	index++;

	// Find closing '"'
	unsigned int closingIndex = index + 1;
	while (jsonString[closingIndex] != '\0' && jsonString[closingIndex] != '"') {
		closingIndex++;
	}

	assert(jsonString[closingIndex] == '"');

	unsigned int substringLength = closingIndex - index;

	// length+1 for null-byte
	char* stringVal = (char*)malloc(sizeof(char) * (substringLength + 1));
	strncpy(stringVal, jsonString + index, substringLength);
	stringVal[substringLength] = '\0';

	parent->_type = WISTRING;
	parent->contents.stringVal = stringVal;

	// To check behind string, we move 1 further from the closing quote
	closingIndex++;

	return jumpBlankChars(jsonString, closingIndex);
}

/*
 * Tries to parse a json-number (float or int).
 *
 * Returns the index of the next non-blank char after the number
 *
 * Asserts when index isn't at a valid json-number-character,
 * or when it fails to parse the number (non-valid number).
 */
unsigned int parseNumber(const char* jsonString, unsigned int index, wiValue* parent) {
	assert(
			jsonString[index] == '+' 
			|| jsonString[index] == '-'
			|| (jsonString[index] >= '0' && jsonString[index] <= '9')
	);

	bool isInteger = true;
	unsigned int closingIndex = index + 1;
	while (
		jsonString[closingIndex] != '\0' 
		&& !isBlank(jsonString[closingIndex])
		&& jsonString[closingIndex] != ','
		&& jsonString[closingIndex] != ']'
		&& jsonString[closingIndex] != '}'
	) {
		if (
			jsonString[closingIndex] == '.' 
			|| jsonString[closingIndex] == 'e'
			|| jsonString[closingIndex] == 'E'
		) {
			isInteger = false;
		}

		closingIndex++;
	}

	char number[closingIndex - index];
	strncpy(number, jsonString + index, closingIndex - index);
	number[closingIndex - index] = '\0';

	char* end;

	if (isInteger) {
		long intVal = strtol(number, &end, 10);

		parent->_type = WIINT;
		parent->contents.intVal = intVal;
	} else {
		double floatVal = strtod(number, &end);

		parent->_type = WIFLOAT;
		parent->contents.floatVal = floatVal;
	}

	// Check if it was a real parseable number
	assert(*end == '\0');

	return jumpBlankChars(jsonString, closingIndex);
}

unsigned int parsePair(const char* jsonString, unsigned int index, wiValue* parent) {
	assert(jsonString[index] == '{');

	parent->_type = WIPAIR;
	wiPair* currentPair = NULL;
	wiPair* previousPair;

	wiValue* value;
	// Can reuse this dummy wiValue, so only initialise once
	wiValue* dummyForKey = (wiValue*) malloc(sizeof(wiValue));

	while (jsonString[index] != '\0' && jsonString[index] != '}') {
		previousPair = currentPair;
		currentPair = (wiPair*) malloc(sizeof(wiPair));
		currentPair->nextPair = NULL;

		// Parse key
		// Doing this with the existing parseJSONString to avoid duplication
		index = jumpBlankChars(jsonString, index + 1);
		index = parseString(jsonString, index, dummyForKey);
		currentPair->key = dummyForKey->contents.stringVal;

		// Check correctness
		assert(jsonString[index] == ':');

		// Parse value
		value = (wiValue*) malloc(sizeof(wiValue));
		index = jumpBlankChars(jsonString, index + 1);
		index = parseValue(jsonString, index, value);
		currentPair->value = value;

		// Check correctness and prepare for parsing next if necessary
		assert(jsonString[index] == ',' || jsonString[index] == '}');

		// Link pair to parent/previous pair
		if (previousPair == NULL) {
			// Only happens with first pair
			parent->contents.pairVal = currentPair;
		} else {
			previousPair->nextPair = currentPair;
		}
	}

	free(dummyForKey);

	return jumpBlankChars(jsonString, index + 1);
}

/*
 * Tries to parse a json-array starting from 'index'.
 *
 * Returns the index of the next non-blank character after the end of the array
 *
 * Asserts when initial jsonString at index != '[', 
 * and when it can't find a closing bracket before a null-byte,
 * and when there is no comma or closing bracket after each array-element.
 * (All disregarding white-space)
 */
unsigned int parseArray(const char* jsonString, unsigned int index, wiValue* parent) {
	assert(jsonString[index] == '[');

	// Already move index 1 forward, as we don't need the opening bracket
	index++;

	index = jumpBlankChars(jsonString, index);

	parent->_type = WIARRAY;
	wiArrayEl* currentElement = NULL;
	wiValue* currentElementValue;

	wiArrayEl* previousElement = NULL;

	// Parse every json-value in the array
	while (jsonString[index] != '\0' && jsonString[index] != ']') {
		previousElement = currentElement;

		currentElement = (wiArrayEl*) malloc(sizeof(wiArrayEl));
		currentElementValue = (wiValue*) malloc(sizeof(wiValue));

		currentElement->nextElement = NULL;

		// Parse the actual elementValue
		index = parseValue(jsonString, index, currentElementValue);
		currentElement->elementVal = currentElementValue;
		
		assert(jsonString[index] == ',' || jsonString[index] == ']');

		if (jsonString[index] == ',') {
			index++;
			index = jumpBlankChars(jsonString, index);
		}

		if (previousElement == NULL) {
			// Only happens with the first element
			parent->contents.arrayVal = currentElement;
		} else {
			previousElement->nextElement = currentElement;
		}
	}

	return jumpBlankChars(jsonString, index + 1);
}

/*
 * Tries to parse 'null' (NULL) and asserts if it isn't the case.
 * This can only assert on a non-valid json-string
 *
 * It sets the value and type of the wiValue* parent, 
 * and returns the index of the next non-blank character after "null"
 */
unsigned int parseNull(const char* jsonString, unsigned int index, wiValue* parent) {
	assert(strncmp(jsonString + index, "null", 4) == 0);

	parent->_type = WINULL;

	return jumpBlankChars(jsonString, index + 4);
}

void freeEverything(wiValue* root) {
	switch (root->_type) {
		case WISTRING:
			free(root->contents.stringVal);
			break;

		case WIARRAY: 
			{
				wiArrayEl* currentElement = root->contents.arrayVal;
				wiArrayEl* nextElement;

				while(currentElement != NULL) {
					nextElement = currentElement->nextElement;

					freeEverything(currentElement->elementVal);
					free(currentElement);

					currentElement = nextElement;
				}
			}
			break;

		case WIPAIR: 
			{
				wiPair* currentPair = root->contents.pairVal;
				wiPair* nextPair;

				while(currentPair != NULL) {
					nextPair = currentPair->nextPair;

					free(currentPair->key);
					freeEverything(currentPair->value);
					free(currentPair);

					currentPair = nextPair;
				}
			}
			break;

		default:
				break;
	}

	free(root);
}


/*
 * Returns the index of the first non-blank character
 * starting from 'index'
 */
unsigned int jumpBlankChars(const char* string, unsigned int index) {
	while (isBlank(string[index]) && string[index] != '\0') {
		index++;
	}

	return index;
}

bool isBlank(const char c) {
	return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}
