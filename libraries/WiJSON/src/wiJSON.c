#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/wiJSON.h"

// Forward declarations for internal functions
bool isBlank(const char);
void jumpBlankChars(FILE*);

void parseArray	(FILE*, wiValue*);
void parseBool	(FILE*, wiValue*);
void parseNull	(FILE*, wiValue*);
void parseNumber(FILE*, wiValue*);
void parsePair	(FILE*, wiValue*);
void parseString(FILE*, wiValue*);
void parseValue	(FILE*, wiValue*);

/*
 * Entrypoint for parsing strings.
 * Returns a wiValue* object that holds all the JSON data.
 *
 * Will assert when it finds invalid JSON.
 *
 * Under the hood, this will convert the string to a FILE* and call the
 * parser for FILE's.
 */
wiValue* parseJSONString(const char* jsonString) {
	FILE* stringAsFile = fmemopen((void*) jsonString, strlen(jsonString), "r");
	assert(stringAsFile != NULL);

	wiValue* root = parseJSONFile(stringAsFile);

	fclose(stringAsFile);

	return root;
}

/*
 * Entrypoint for parsing files.
 * Returns a wiValue* object that holds all the JSON data.
 *
 * Will assert when it finds invalid JSON.
 */
wiValue* parseJSONFile(FILE* jsonFile) {
	wiValue* root = (wiValue*) malloc(sizeof(wiValue));

	parseValue(jsonFile, root);

	return root;
}

/*
 *	Determine which type of value it is, 
 *	and call the right function to parse that value.
 *
 * 	Asserts when the file-pointer is on a blank space.
 */
void parseValue(FILE* jsonFile, wiValue* parent) {
	// Peek the current character, 
	// and put it back because some functions will need it.
	// Mainly the Bool, Null and Number functions.
	char c = fgetc(jsonFile);
	ungetc(c, jsonFile);
	assert(!isBlank(c));

	switch (c) {
		case '{': 	// Object
			parsePair(jsonFile, parent);
			break;

		case '[': 	// Array
			parseArray(jsonFile, parent);
			break;

		case '"':
			parseString(jsonFile, parent);
			break;

		case 't':	// Boolean
		case 'f':
			parseBool(jsonFile, parent);
			break;

		case 'n': 	// Null	
			parseNull(jsonFile, parent);
			break;

		default: 	// Try parsing number
			parseNumber(jsonFile, parent);
			break;
	}
}

/*
 * Tries to parse a boolean value and assign it to the wiValue* parent contents.
 *
 * Asserts when it couldn't find "true" or "false",
 * including when it encounters an EOF before finishing parsing.
 */
void parseBool(FILE* jsonFile, wiValue* parent) {
	char buffer[6];
	char c = 0;

	int i = 0;

	// Read the first four letters
	while (i < 4) {
		c = fgetc(jsonFile);
		assert(c != EOF);
		buffer[i] = c;
		i++;
	}

	if (c == 'e') {
		buffer[i] = '\0';
		assert(strcmp(buffer, "true") == 0);
		parent->contents.boolVal = true;

	} else {
		c = fgetc(jsonFile);
		assert(c != EOF);
		buffer[i] = c;
		buffer[i + 1] = '\0';
		assert(strcmp(buffer, "false") == 0);

		parent->contents.boolVal = false;
	}
	parent->_type = WIBOOL;
	jumpBlankChars(jsonFile);
}

/*
 * Tries to parse a string value and assign it to the wiValue* parent contents.
 *
 * Asserts when the FILE* doesn't point to a '"' 
 * and when it couldn't find a closing '"' before EOF.
 */
void parseString(FILE* jsonFile, wiValue* parent) {
	char c = fgetc(jsonFile);
	assert(c == '"');

	unsigned int buffersize = 16;
	char* stringVal = (char*) malloc(sizeof(char) * buffersize);
	assert(stringVal != NULL);

	unsigned int index = 0;
	bool escaped = false;

	// "Cursor" still stands on the opening quote
	c = fgetc(jsonFile);
	stringVal[index] = c;
	index++;

	while (c != EOF && (c != '"' || escaped)) {
		c = fgetc(jsonFile);

		// TODO: move to own function for readability
		if (escaped) {
			switch (c) {
				case '"':
				case '\\':
				case '/':
					stringVal[index] = c;
					index++;
					break;

				case 'b':
					stringVal[index] = '\b';
					index++;
					break;

				case 'f':
					stringVal[index] = '\f';
					index++;
					break;

				case 'n':
					stringVal[index] = '\n';
					index++;
					break;

				case 'r':
					stringVal[index] = '\r';
					index++;
					break;

				case 't':
					stringVal[index] = '\t';
					index++;
					break;

				case 'u':
					// TODO: parse hex-escape code
					break;
			}
			escaped = false;

		} else if (c == '\\') {
			escaped = true;
		} else {
			stringVal[index] = c;
			index++;
		}

		// Re-allocate to larger string if needed
		if (index >= buffersize - 1) {
			buffersize *= 2;
			stringVal = (char*) realloc((void*) stringVal, sizeof(char) * buffersize);
			assert(stringVal != NULL);
		}
	}
	assert(c == '"');
	stringVal[index - 1] = '\0';

	parent->_type = WISTRING;
	parent->contents.stringVal = stringVal;

	c = fgetc(jsonFile);
	if (isBlank(c)) {
		jumpBlankChars(jsonFile);
	} else {
		ungetc(c, jsonFile);
	}
}

/*
 * Small helper function for parseNumber
 */
bool isValidForNumber(const char c) {
	return isdigit(c) || c == 'e' || c == 'E' 
		|| c == '+' || c == '-' || c == '.';
}

/*
 * Tries to parse a string value and assign it to the wiValue* parent contents.
 *
 * Asserts when the FILE* doesn't point to a '"' 
 * and when it couldn't find a closing '"' before EOF.
 */
void parseNumber(FILE* jsonFile, wiValue* parent) {
	char c = fgetc(jsonFile);
	assert(c != EOF);
	assert(c == '+' || c == '-' || (c >= '0' && c <= '9'));

	bool isInteger = true;
	// Largest bool is 22 long, + 1 '\0' at the end
	char buffer[23];

	unsigned int index = 0;

	while (c != EOF && index < 22 && isValidForNumber(c)) {
		buffer[index] = c;
		c = fgetc(jsonFile);
		index++;

		if (c == '.' || c == 'e' || c == 'E') {
			isInteger = false;
		}
	}

	// Check next character, shouldn't be numerical cause it won't get parsed
	assert(c == EOF || isBlank(c) || c == ',' || c == ']' || c == '}');
	ungetc(c, jsonFile);
	jumpBlankChars(jsonFile);

	buffer[index] = '\0';

	// Parse number
	char* end;
	if (isInteger) {
		long intval = strtol(buffer, &end, 10);
		parent->_type = WIINT;
		parent->contents.intVal = intval;
	} else {
		double floatVal = strtod(buffer, &end);
		parent->_type = WIFLOAT;
		parent->contents.floatVal = floatVal;
	}
	assert(*end == '\0');
}

/*
 * Tries to parse key-value pairs and assign it to the wiValue* parent contents.
 *
 * Asserts when the FILE* doesn't point to a '{',
 * when it can't find a ':' after a key,
 * when it can't find a ',' after a value,
 * and when it couldn't find a closing '}' before EOF.
 */
void parsePair(FILE* jsonFile, wiValue* parent) {
	char c = fgetc(jsonFile);
	assert(c == '{');

	parent->_type = WIPAIR;
	wiPair* currentPair = NULL;
	wiPair* previousPair = NULL;

	wiValue* value = NULL;
	wiValue* dummyForKey = (wiValue*) malloc(sizeof(wiValue));

	while (c != EOF && c != '}') {
		previousPair = currentPair;

		currentPair = (wiPair*) malloc(sizeof(wiPair));
		currentPair->nextPair = NULL;
		currentPair->value = NULL;
		currentPair->key = NULL;

		// Get key with dummy-value
		jumpBlankChars(jsonFile);
		parseString(jsonFile, dummyForKey);
		currentPair->key = dummyForKey->contents.stringVal;

		// Jump the ':'
		c = fgetc(jsonFile);
		assert(c == ':');
		jumpBlankChars(jsonFile);

		// Parse value
		value = (wiValue*) malloc(sizeof(wiValue));
		parseValue(jsonFile, value);
		currentPair->value = value;

		c = fgetc(jsonFile);
		assert(c == ',' || c == '}');

		// Link the pair to parent/previous pair
		if (previousPair == NULL) {
			parent->contents.pairVal = currentPair;
		} else {
			previousPair->nextPair = currentPair;
		}
	}
	free(dummyForKey);

	assert(c == '}');

	jumpBlankChars(jsonFile);
}

/*
 * Tries to parse array elements and assign it to the wiValue* parent contents.
 *
 * Asserts when the FILE* doesn't point to a '[',
 * when it couldn't find comma's between array-elements,
 * and when it couldn't find a closing ']' before EOF.
 */
void parseArray(FILE* jsonFile, wiValue* parent) {
	char c = fgetc(jsonFile);
	assert(c == '[');

	parent->_type = WIARRAY;

	wiArrayEl* currentElement = NULL;
	wiArrayEl* previousElement = NULL;

	wiValue* currentValue = NULL;

	while (c != EOF && c != ']') {
		previousElement = currentElement;

		currentElement = (wiArrayEl*) malloc(sizeof(wiArrayEl));
		currentElement->elementVal = NULL;
		currentElement->nextElement = NULL;

		currentValue = (wiValue*) malloc(sizeof(wiValue));

		jumpBlankChars(jsonFile);
		parseValue(jsonFile, currentValue);

		currentElement->elementVal = currentValue;
		
		c = fgetc(jsonFile);
		assert(c == ',' || c == ']');

		// Link to parent/previous element
		if (previousElement == NULL) {
			parent->contents.arrayVal = currentElement;
		} else {
			previousElement->nextElement = currentElement;
		}
	}
	assert(c == ']');
	jumpBlankChars(jsonFile);
}

/*
 * Tries to parse "null" and assigns it to the wiValue* parent contents.
 *
 * Asserts when it doesn't read "null".
 */
void parseNull(FILE* jsonFile, wiValue* parent) {
	assert(fgetc(jsonFile) == 'n');
	assert(fgetc(jsonFile) == 'u');
	assert(fgetc(jsonFile) == 'l');
	assert(fgetc(jsonFile) == 'l');
	parent->_type = WINULL;
	jumpBlankChars(jsonFile);
}

/*
 * Free all the allocated memory for the passed wiValue*.
 * It's advised to call this when you don't need your JSON anymore, 
 * because it can take up a bit of space. 
 * */
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
 * Advances the FILE* untill it finds a non-blank char (see isBlank()).
 */
void jumpBlankChars(FILE* file) {
	char c = fgetc(file);

	while (c != EOF && isBlank(c)) {
		c = fgetc(file);
	}
	// Place the last one back, as it is not blank.
	ungetc(c, file);
}

/*
 * Peeks at the fileptr and returns whether it points to a blank character.
 * Blank characters are defined as ' ', '\t', '\n' and '\r'.
 *
 * This does NOT check if the pointer points to EOF, 
 * you should check this seperatly.
 */
bool isBlank(const char c) {
	return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}
