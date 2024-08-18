#include <assert.h>
#include <stdio.h>

#include "../../../include/wiJSON.h"
#include "../../../../WiTesting/wiTest.h"

const char* enumToString(wiType type) {
	switch (type) {
        case WIARRAY: return "WIARRAY";
        case WIBOOL: return "WIBOOL";
        case WIFLOAT: return "WIFLOAT";
        case WIINT: return "WIINT";
        case WINULL: return "WINULL";
        case WIPAIR: return "WIPAIR";
        case WISTRING: return "WISTRING";
		default: return "WI_UNKNOWN";
    }
}

void testDodonaCourses() {
	// Path so the executables link correctly
	FILE* jsonFile = fopen("core/file/files/testDodonaCourses.json", "r");
	assert(jsonFile != NULL);
	wiValue* testObjectCourses = parseJSONFile(jsonFile);
	fclose(jsonFile);

	assert(testObjectCourses != NULL);
	wiTestEnum(WIARRAY, testObjectCourses->_type, enumToString);

	wiArrayEl* currentElement = testObjectCourses->contents.arrayVal;
	assert(currentElement != NULL);

	wiTestEnum(WIPAIR, currentElement->elementVal->_type, enumToString);
	wiPair* pair = currentElement->elementVal->contents.pairVal;

	assert(pair != NULL);
	wiTestString("id", pair->key);
	wiTestEnum(WIINT, pair->value->_type, enumToString);
	wiTestInt(3230, pair->value->contents.intVal);

	pair = pair->nextPair;
	assert(pair != NULL);
	wiTestString("name", pair->key);
	wiTestEnum(WISTRING, pair->value->_type, enumToString);
	wiTestString("Algoritmen en Datastructuren 1", pair->value->contents.stringVal);

	pair = pair->nextPair;
	assert(pair != NULL);
	wiTestString("teacher", pair->key);
	wiTestEnum(WISTRING, pair->value->_type, enumToString);
	wiTestString("Veerle Fack", pair->value->contents.stringVal);

	pair = pair->nextPair;
	assert(pair != NULL);
	wiTestString("color", pair->key);
	wiTestEnum(WISTRING, pair->value->_type, enumToString);
	wiTestString("brown", pair->value->contents.stringVal);

	pair = pair->nextPair;
	assert(pair != NULL);
	wiTestString("year", pair->key);
	wiTestEnum(WISTRING, pair->value->_type, enumToString);
	wiTestString("2023-2024", pair->value->contents.stringVal);

	pair = pair->nextPair; 	// Visibility
	assert(pair != NULL);
	pair = pair->nextPair; 	// Registration
	assert(pair != NULL);
	pair = pair->nextPair; 	// Created_at
	assert(pair != NULL);
	pair = pair->nextPair; 	// Updated_at
	assert(pair != NULL);

	pair = pair->nextPair;
	assert(pair != NULL);
	wiTestString("url", pair->key);
	wiTestEnum(WISTRING, pair->value->_type, enumToString);
	wiTestString("https://dodona.be/nl/courses/3230.json", pair->value->contents.stringVal);

	pair = pair->nextPair;
	assert(pair != NULL);
	wiTestString("series", pair->key);
	wiTestEnum(WISTRING, pair->value->_type, enumToString);
	wiTestString("https://dodona.be/nl/courses/3230/series.json", pair->value->contents.stringVal);

	assert(pair->nextPair == NULL);

	currentElement = currentElement->nextElement; 	// Computergebruik
	assert(currentElement != NULL);
	wiTestEnum(WIPAIR, currentElement->elementVal->_type, enumToString);
	currentElement = currentElement->nextElement; 	// Programmeren
	assert(currentElement != NULL);
	wiTestEnum(WIPAIR, currentElement->elementVal->_type, enumToString);
	currentElement = currentElement->nextElement; 	// Scriptingtalen
	assert(currentElement != NULL);
	wiTestEnum(WIPAIR, currentElement->elementVal->_type, enumToString);
	currentElement = currentElement->nextElement; 	// Computergebruik
	assert(currentElement != NULL);
	wiTestEnum(WIPAIR, currentElement->elementVal->_type, enumToString);

	pair = currentElement->elementVal->contents.pairVal;

	assert(pair != NULL);
	wiTestString("id", pair->key);
	wiTestEnum(WIINT, pair->value->_type, enumToString);
	wiTestInt(1662, pair->value->contents.intVal);

	pair = pair->nextPair;
	assert(pair != NULL);
	wiTestString("name", pair->key);
	wiTestEnum(WISTRING, pair->value->_type, enumToString);
	wiTestString("Computergebruik", pair->value->contents.stringVal);

	pair = pair->nextPair;
	assert(pair != NULL);
	wiTestString("teacher", pair->key);
	wiTestEnum(WISTRING, pair->value->_type, enumToString);
	wiTestString("Peter Dawyndt", pair->value->contents.stringVal);

	pair = pair->nextPair;
	assert(pair != NULL);
	wiTestString("color", pair->key);
	wiTestEnum(WISTRING, pair->value->_type, enumToString);
	wiTestString("indigo", pair->value->contents.stringVal);

	pair = pair->nextPair;
	assert(pair != NULL);
	wiTestString("year", pair->key);
	wiTestEnum(WISTRING, pair->value->_type, enumToString);
	wiTestString("2022-2023", pair->value->contents.stringVal);

	pair = pair->nextPair; 	// Visibility
	assert(pair != NULL);
	pair = pair->nextPair; 	// Registration
	assert(pair != NULL);
	pair = pair->nextPair; 	// Created_at
	assert(pair != NULL);
	pair = pair->nextPair; 	// Updated_at
	assert(pair != NULL);

	pair = pair->nextPair;
	assert(pair != NULL);
	wiTestString("url", pair->key);
	wiTestEnum(WISTRING, pair->value->_type, enumToString);
	wiTestString("https://dodona.be/nl/courses/1662.json", pair->value->contents.stringVal);

	pair = pair->nextPair;
	assert(pair != NULL);
	wiTestString("series", pair->key);
	wiTestEnum(WISTRING, pair->value->_type, enumToString);
	wiTestString("https://dodona.be/nl/courses/1662/series.json", pair->value->contents.stringVal);

	assert(pair->nextPair == NULL);

	assert(currentElement->nextElement == NULL);

	freeEverything(testObjectCourses);
}

int main() {
	// Actual Dodona output
	printf("Testing Dodona courses...\n");

	testDodonaCourses();

	wiPrintResult("Dodona courses");

	return 0;
}


