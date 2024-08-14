#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/wiJSON.h"

// Forward declarations for internal functions
bool isBlank(const char);

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
	unsigned long stringLength = strlen(jsonString);

	FILE* stringAsFile = fmemopen((void*) jsonString, stringLength, "r");

	return parseJSONFile(stringAsFile);
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
void parseJSONValue(FILE* jsonFile, wiValue* parent) {
	assert(!isBlank(jsonFile));

	// Peek the current character, 
	// and put it back because some functions will need it.
	// Mainly the Bool, Null and Number functions.
	char c = fgetc(jsonFile);
	ungetc(c, jsonFile);

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
