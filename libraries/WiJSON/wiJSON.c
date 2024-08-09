#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "wiJSON.h"

// Forward declarations
bool isBlank(const char);
int jumpBlankChars(const char*, unsigned int);

int parseJSONArray	(const char*, unsigned int, wiValue*);
int parseJSONBool	(const char*, unsigned int, wiValue*);
int parseJSONNull	(const char*, unsigned int, wiValue*);
int parseJSONNumber (const char*, unsigned int, wiValue*);
int parseJSONObject	(const char*, unsigned int, wiValue*);
int parseJSONPair	(const char*, unsigned int, wiValue*);
int parseJSONString	(const char*, unsigned int, wiValue*);
int parseJSONValue	(const char*, unsigned int, wiValue*);


// For testing purposes, a main function.
// TODO: Remove this when done testing
int main() {
	wiValue* root = parseJSON(
			"{\"key\": \"value\"}"
	);

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
 * Returns the index of the last parsed character in the jsonString
 */
int parseJSONValue(const char *jsonString, unsigned int index, wiValue* parent) {
	// Enforce we can start parsing
	assert(!isBlank(jsonString[index]));

	switch (jsonString[index]) {
		case '{':	// Object
			index = parseJSONObject(jsonString, index, parent);
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
	return index;
}

int parseJSONObject(const char* jsonString, unsigned int index, wiValue* parent) {
	// TODO: Do I really need this parseObject thing?
	// 		Can't I just immediatly parse the key-value pairs???
	assert(jsonString[index] == '{');

	index++;
	index = jumpBlankChars(jsonString, index);

	while (jsonString[index] != '\0' && jsonString[index] != '}') {
		// Only a key-value pair is possible after a '{'
		assert(jsonString[index] == '"');

		index = parseJSONObject(jsonString, index, parent);
		assert(jsonString[index] == ',' || jsonString[index] == '}');

		if (jsonString[index] == ',') {
			index++;
			index = jumpBlankChars(jsonString, index);
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
	wiArrayEl* currentElement;
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

		if (previousElement != NULL) {
			// Happens when we're at least at the 2nd element in the array
			previousElement->nextElement = currentElement;
		} else {
			// Only happens when it's the first element
			parent->contents.arrayVal = currentElement;
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

		case WIOBJECT:
			// TODO:
			break;

		case WIPAIR:
			// TODO:
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
