#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wiJSON.h"

// Forward declarations
bool isBlank(const char);
int jumpBlankChars(const char*, unsigned int);

int parseJSONArray	(const char*, unsigned int, wiValue*);
int parseJSONBool	(const char*, unsigned int, wiValue*);
int parseJSONNull	(const char*, unsigned int, wiValue*);
int parseJSONNumber (const char*, unsigned int, wiValue*);
int parseJSONPair	(const char*, unsigned int, wiValue*);
int parseJSONString	(const char*, unsigned int, wiValue*);
int parseJSONValue	(const char*, unsigned int, wiValue*);


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
 * Returns the index of the last parsed character in the jsonString
 */
int parseJSONValue(const char *jsonString, unsigned int index, wiValue* parent) {
	// Enforce we can start parsing
	assert(!isBlank(jsonString[index]));

	switch (jsonString[index]) {
		case '{':	// Object
			index = parseJSONPair(jsonString, index, parent);
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
			break;

		default:	// Try for a number
			index = parseJSONNumber(jsonString, index, parent);
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
	strncpy(stringVal, jsonString + index, closingIndex - index);
	stringVal[closingIndex - 1] = '\0';

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
int parseJSONNumber(const char* jsonString, unsigned int index, wiValue* parent) {
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

int parseJSONPair(const char* jsonString, unsigned int index, wiValue* parent) {
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

		// Parse key
		// Doing this with the existing parseJSONString to avoid duplication
		index = jumpBlankChars(jsonString, index + 1);
		index = parseJSONString(jsonString, index, dummyForKey);
		currentPair->key = dummyForKey->contents.stringVal;

		// Check correctness
		assert(jsonString[index] == ':');

		// Parse value
		value = (wiValue*) malloc(sizeof(wiValue));
		index = jumpBlankChars(jsonString, index + 1);
		index = parseJSONValue(jsonString, index, value);
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
int parseJSONArray(const char* jsonString, unsigned int index, wiValue* parent) {
	assert(jsonString[index] == '[');

	// Already move index 1 forward, as we don't need the opening quote
	index++;

	// Find closing ']'
	unsigned int closingIndex = index + 1;

	while (jsonString[closingIndex] != '\0' && jsonString[closingIndex] != ']') {
		closingIndex++;
	}
	assert(jsonString[closingIndex] == ']');

	index = jumpBlankChars(jsonString, index);

	parent->_type = WIARRAY;
	wiArrayEl* currentElement = NULL;
	wiValue* currentElementValue;

	wiArrayEl* previousElement;

	// Parse every json-value in the array
	while (index < closingIndex) {
		previousElement = currentElement;

		currentElement = (wiArrayEl*) malloc(sizeof(wiArrayEl));
		currentElementValue = (wiValue*) malloc(sizeof(wiValue));

		// Parse the actual elementValue
		index = parseJSONValue(jsonString, index, currentElementValue);
		currentElement->elementVal = currentElementValue;
		
		index = jumpBlankChars(jsonString, index);
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

	return jumpBlankChars(jsonString, closingIndex);
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

	return jumpBlankChars(jsonString, index + 5);
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
int jumpBlankChars(const char* string, unsigned int index) {
	while (isBlank(string[index]) && string[index] != '\0') {
		index++;
	}

	return index;
}

bool isBlank(const char c) {
	return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}
