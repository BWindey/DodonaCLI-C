#include <assert.h>
#include <string.h>

#include "../../include/wiJSON.h"
#include "../../../WiEnrich/include/enrich.h"

void testDodonaSeries() {
	wiValue* testObjectSeries = parseJSONString(
		"{"
			"\"id\":36018,"
			"\"name\":\"week 12: tweede evaluatie (JavaScript)\","
			"\"description\":\"test-description101\","
			"\"visibility\":\"open\","
			"\"visibility_start\":null,"
			"\"order\":11,"
			"\"created_at\":\"2024-02-08T14:51:07.000+01:00\","
			"\"updated_at\":\"2024-05-25T21:00:13.000+02:00\","
			"\"deadline\":\"2024-05-14T17:10:00.000+02:00\","
			"\"url\":\"https://dodona.be/nl/series/36018.json\","
			"\"course\":\"https://dodona.be/nl/courses/3355.json\","
			"\"exercises\":\"https://dodona.be/nl/series/36018/activities.json\""
		"}"
	);

	assert(testObjectSeries != NULL);
	assert(testObjectSeries->_type == WIPAIR);

	wiPair* pair = testObjectSeries->contents.pairVal;
	assert(pair != NULL);

	// Test "id"
	assert(strcmp(pair->key, "id") == 0);
	assert(pair->value->_type == WIINT);
	assert(pair->value->contents.intVal == 36018);

	pair = pair->nextPair;
	assert(pair != NULL);

	// Test "name"
	assert(strcmp(pair->key, "name") == 0);
	assert(pair->value->_type == WISTRING);
	assert(strcmp(pair->value->contents.stringVal, "week 12: tweede evaluatie (JavaScript)") == 0);

	pair = pair->nextPair;
	assert(pair != NULL);

	// Test "description"
	assert(strcmp(pair->key, "description") == 0);
	assert(pair->value->_type == WISTRING);
	assert(strcmp(pair->value->contents.stringVal, "test-description101") == 0);

	pair = pair->nextPair;
	assert(pair != NULL);

	// Test "visibility"
	assert(strcmp(pair->key, "visibility") == 0);
	assert(pair->value->_type == WISTRING);
	assert(strcmp(pair->value->contents.stringVal, "open") == 0);

	pair = pair->nextPair;
	assert(pair != NULL);

	// Test "visibility_start"
	assert(strcmp(pair->key, "visibility_start") == 0);
	assert(pair->value->_type == WINULL);

	pair = pair->nextPair;
	assert(pair != NULL);

	// Test "order"
	assert(strcmp(pair->key, "order") == 0);
	assert(pair->value->_type == WIINT);
	assert(pair->value->contents.intVal == 11);

	pair = pair->nextPair;
	assert(pair != NULL);

	// Test "created_at"
	assert(strcmp(pair->key, "created_at") == 0);
	assert(pair->value->_type == WISTRING);
	assert(strcmp(pair->value->contents.stringVal, "2024-02-08T14:51:07.000+01:00") == 0);

	pair = pair->nextPair;
	assert(pair != NULL);

	// Test "updated_at"
	assert(strcmp(pair->key, "updated_at") == 0);
	assert(pair->value->_type == WISTRING);
	assert(strcmp(pair->value->contents.stringVal, "2024-05-25T21:00:13.000+02:00") == 0);

	pair = pair->nextPair;
	assert(pair != NULL);

	// Test "deadline"
	assert(strcmp(pair->key, "deadline") == 0);
	assert(pair->value->_type == WISTRING);
	assert(strcmp(pair->value->contents.stringVal, "2024-05-14T17:10:00.000+02:00") == 0);

	pair = pair->nextPair;
	assert(pair != NULL);

	// Test "url"
	assert(strcmp(pair->key, "url") == 0);
	assert(pair->value->_type == WISTRING);
	assert(strcmp(pair->value->contents.stringVal, "https://dodona.be/nl/series/36018.json") == 0);

	pair = pair->nextPair;
	assert(pair != NULL);

	// Test "course"
	assert(strcmp(pair->key, "course") == 0);
	assert(pair->value->_type == WISTRING);
	assert(strcmp(pair->value->contents.stringVal, "https://dodona.be/nl/courses/3355.json") == 0);

	pair = pair->nextPair;
	assert(pair != NULL);

	// Test "exercises"
	assert(strcmp(pair->key, "exercises") == 0);
	assert(pair->value->_type == WISTRING);
	assert(strcmp(pair->value->contents.stringVal, "https://dodona.be/nl/series/36018/activities.json") == 0);

	pair = pair->nextPair;
	assert(pair == NULL);  // No more pairs, ensure we have checked all expected values

	freeEverything(testObjectSeries);
}


int main() {
	// Actual Dodona output
	printf("Testing Dodona series...\n");

	testDodonaSeries();

	char message[] = "Dodona series test [BRIGHT-GREEN]passed[/].\n\n";
	wiEnrich(message);
	printf("%s", message);

	return 0;
}


