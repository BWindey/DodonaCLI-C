#include <assert.h>
#include <stdio.h>
#include <string.h>

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

void testDodonaSeries() {
	// Path so the executables link correctly
	FILE* jsonFile = fopen("core/file/files/testDodonaSeries.json", "r");
	assert(jsonFile != NULL);
	wiValue* testObjectSeries = parseJSONFile(jsonFile);
	fclose(jsonFile);

	assert(testObjectSeries != NULL);
	wiTestEnum(WIPAIR, testObjectSeries->_type, enumToString);

	wiPair* pair = testObjectSeries->contents.pairVal;
	assert(pair != NULL);

	// Test "id"
	wiTestString("id", pair->key);
	wiTestEnum(WIINT, pair->value->_type, enumToString);
	wiTestInt(36018, pair->value->contents.intVal);

	pair = pair->nextPair;
	assert(pair != NULL);

	// Test "name"
	wiTestString("name", pair->key);
	wiTestEnum(WISTRING, pair->value->_type, enumToString);
	wiTestString("week 12: tweede evaluatie (JavaScript)", pair->value->contents.stringVal);

	pair = pair->nextPair;
	assert(pair != NULL);

	// Test "description"
	wiTestString("description", pair->key);
	wiTestEnum(WISTRING, pair->value->_type, enumToString);
	wiTestString("test-description101", pair->value->contents.stringVal);

	pair = pair->nextPair;
	assert(pair != NULL);

	// Test "visibility"
	wiTestString("visibility", pair->key);
	wiTestEnum(WISTRING, pair->value->_type, enumToString);
	wiTestString("open", pair->value->contents.stringVal);

	pair = pair->nextPair;
	assert(pair != NULL);

	// Test "visibility_start"
	wiTestString("visibility_start", pair->key);
	assert(pair->value->_type == WINULL);

	pair = pair->nextPair;
	assert(pair != NULL);

	// Test "order"
	wiTestString("order", pair->key);
	wiTestEnum(WIINT, pair->value->_type, enumToString);
	wiTestInt(11, pair->value->contents.intVal);

	pair = pair->nextPair;
	assert(pair != NULL);

	// Test "created_at"
	wiTestString("created_at", pair->key);
	wiTestEnum(WISTRING, pair->value->_type, enumToString);
	wiTestString("2024-02-08T14:51:07.000+01:00", pair->value->contents.stringVal);

	pair = pair->nextPair;
	assert(pair != NULL);

	// Test "updated_at"
	wiTestString("updated_at", pair->key);
	wiTestEnum(WISTRING, pair->value->_type, enumToString);
	wiTestString("2024-05-25T21:00:13.000+02:00", pair->value->contents.stringVal);

	pair = pair->nextPair;
	assert(pair != NULL);

	// Test "deadline"
	wiTestString("deadline", pair->key);
	wiTestEnum(WISTRING, pair->value->_type, enumToString);
	wiTestString("2024-05-14T17:10:00.000+02:00", pair->value->contents.stringVal);

	pair = pair->nextPair;
	assert(pair != NULL);

	// Test "url"
	wiTestString("url", pair->key);
	wiTestEnum(WISTRING, pair->value->_type, enumToString);
	wiTestString("https://dodona.be/nl/series/36018.json", pair->value->contents.stringVal);

	pair = pair->nextPair;
	assert(pair != NULL);

	// Test "course"
	wiTestString("course", pair->key);
	wiTestEnum(WISTRING, pair->value->_type, enumToString);
	wiTestString("https://dodona.be/nl/courses/3355.json", pair->value->contents.stringVal);

	pair = pair->nextPair;
	assert(pair != NULL);

	// Test "exercises"
	wiTestString("exercises", pair->key);
	wiTestEnum(WISTRING, pair->value->_type, enumToString);
	wiTestString("https://dodona.be/nl/series/36018/activities.json", pair->value->contents.stringVal);

	pair = pair->nextPair;
	assert(pair == NULL);  // No more pairs, ensure we have checked all expected values

	freeEverything(testObjectSeries);
}


int main() {
	// Actual Dodona output
	printf("Testing Dodona series...\n");

	testDodonaSeries();

	wiPrintResult("Dodona series");

	return 0;
}


