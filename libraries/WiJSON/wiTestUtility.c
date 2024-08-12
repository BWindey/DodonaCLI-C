#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "wiJSON.h"
#include "wiUtility.h"
#include "../Enrich/enrich.h"


void testSimpleObject();

void testObject1();
void testObject2();
/* void testObject3();
void testObject4(); */

int main() {
	char msg[] = "[BRIGHT-YELLOW][UNDER]Utility tests...[/]\n";
	enrich(msg);
	printf("%s", msg);

	testSimpleObject();
	
	testObject1();
	testObject2();
	/* testObject3();
	testObject4(); */

	char message[] = "[BLACK][ON BRIGHT-GREEN]All utility tests passed.[/]\n\n";
	enrich(message);
	printf("%s", message);

	return 0;
}

void testSimpleObject() {
	printf("Testing simple objects...\n");

	// Test 1: Simple object with one key-value pair (string)
	wiValue* testObject1 = parseJSON("{\"key1\":\"value1\"}");
	assert(testObject1->contents.pairVal != NULL);

	wiPair* pair1 = testObject1->contents.pairVal;
	assert(strcmp(pair1->key, "key1") == 0);
	assert(strcmp(getStringVal(pair1), "value1") == 0);


	// Test 2: Simple object with multiple key-value pairs (int and string)
	wiValue* testObject2 = parseJSON("{ \"key1\": 42, \"key2\": \"value2\" }");
	assert(testObject2->_type == WIPAIR);
	assert(testObject2->contents.pairVal != NULL);

	wiPair* pair2_kv1 = testObject2->contents.pairVal;
	assert(strcmp(pair2_kv1->key, "key1") == 0);
	assert(getIntVal(pair2_kv1) == 42);

	wiPair* pair2_kv2 = pair2_kv1->nextPair;
	assert(pair2_kv2 != NULL);
	assert(strcmp(pair2_kv2->key, "key2") == 0);
	assert(strcmp(getStringVal(pair2_kv2), "value2") == 0);


	// Test 3: Simple object with multiple key-value pairs (float, bool, string)
	wiValue* testObject3 = parseJSON("{ \"key1\": 3.14, \"key2\": true, \"key3\": \"value3\" }");
	assert(testObject3->_type == WIPAIR);
	assert(testObject3->contents.pairVal != NULL);

	wiPair* pair3_kv1 = testObject3->contents.pairVal;
	assert(strcmp(pair3_kv1->key, "key1") == 0);
	assert(getFloatVal(pair3_kv1) == 3.14);

	wiPair* pair3_kv2 = pair3_kv1->nextPair;
	assert(pair3_kv2 != NULL);
	assert(strcmp(pair3_kv2->key, "key2") == 0);
	assert(getBoolVal(pair3_kv2) == true);

	wiPair* pair3_kv3 = pair3_kv2->nextPair;
	assert(pair3_kv3 != NULL);
	assert(strcmp(pair3_kv3->key, "key3") == 0);
	assert(strcmp(getStringVal(pair3_kv3), "value3") == 0);


	// Test 4: Simple object with an array and other values
	wiValue* testObject4 = parseJSON("{ \"key1\": [1, 2, 3], \"key2\": false, \"key3\": \"value4\" }");
	assert(testObject4->_type == WIPAIR);
	assert(testObject4->contents.pairVal != NULL);

	wiPair* pair4_kv1 = testObject4->contents.pairVal;
	assert(strcmp(pair4_kv1->key, "key1") == 0);
	assert(pair4_kv1->value->_type == WIARRAY);
	assert(pair4_kv1->value->contents.arrayVal != NULL);

	// Verify array contents [1, 2, 3]
	wiArrayEl* array1 = getArrayVal(pair4_kv1);
	assert(array1->elementVal->_type == WIINT);
	assert(array1->elementVal->contents.intVal == 1);
	assert(array1->nextElement != NULL);

	wiArrayEl* array2 = array1->nextElement;
	assert(array2->elementVal->_type == WIINT);
	assert(array2->elementVal->contents.intVal == 2);
	assert(array2->nextElement != NULL);

	wiArrayEl* array3 = array2->nextElement;
	assert(array3->elementVal->_type == WIINT);
	assert(array3->elementVal->contents.intVal == 3);

	wiPair* pair4_kv2 = pair4_kv1->nextPair;
	assert(pair4_kv2 != NULL);
	assert(strcmp(pair4_kv2->key, "key2") == 0);
	assert(getBoolVal(pair4_kv2) == false);

	wiPair* pair4_kv3 = pair4_kv2->nextPair;
	assert(pair4_kv3 != NULL);
	assert(strcmp(pair4_kv3->key, "key3") == 0);
	assert(strcmp(getStringVal(pair4_kv3), "value4") == 0);

	char message[] = "Simple object tests [BRIGHT-GREEN]succeeded[/].\n\n";
	enrich(message);
	printf("%s", message);
}

void testObject1() {
	// Actual Dodona output
	printf("Testing Dodona courses...\n");
	wiValue* testObjectCourses = parseJSON(
		"["
		"	{"
  		"	  \"id\": 3230,"
  		"	  \"name\": \"Algoritmen en Datastructuren 1\","
  		"	  \"teacher\": \"Veerle Fack\","
  		"	  \"color\": \"brown\","
  		"	  \"year\": \"2023-2024\","
  		"	  \"visibility\": \"visible_for_institution\","
  		"	  \"registration\": \"open_for_institution\","
  		"	  \"created_at\": \"2024-01-10T09:50:53.000+01:00\","
  		"	  \"updated_at\": \"2024-02-09T11:08:07.000+01:00\","
  		"	  \"url\": \"https://dodona.be/nl/courses/3230.json\","
  		"	  \"series\": \"https://dodona.be/nl/courses/3230/series.json\""
  		"	},"
  		"	{"
  		"	  \"id\": 2876,"
  		"	  \"name\": \"Computergebruik\","
  		"	  \"teacher\": \"Peter Dawyndt\","
  		"	  \"color\": \"brown\","
  		"	  \"year\": \"2023-2024\","
  		"	  \"visibility\": \"visible_for_institution\","
  		"	  \"registration\": \"open_for_institution\","
  		"	  \"created_at\": \"2023-09-16T15:12:14.000+02:00\","
  		"	  \"updated_at\": \"2023-11-30T15:20:31.000+01:00\","
  		"	  \"url\": \"https://dodona.be/nl/courses/2876.json\","
  		"	  \"series\": \"https://dodona.be/nl/courses/2876/series.json\""
  		"	},"
  		"	{"
  		"	  \"id\": 2955,"
  		"	  \"name\": \"Programmeren\","
  		"	  \"teacher\": \"Kris Coolsaet\","
  		"	  \"color\": \"brown\","
  		"	  \"year\": \"2023-2024\","
  		"	  \"visibility\": \"visible_for_institution\","
  		"	  \"registration\": \"open_for_institution\","
  		"	  \"created_at\": \"2023-09-27T10:04:55.000+02:00\","
  		"	  \"updated_at\": \"2023-09-27T10:04:55.000+02:00\","
  		"	  \"url\": \"https://dodona.be/nl/courses/2955.json\","
  		"	  \"series\": \"https://dodona.be/nl/courses/2955/series.json\""
  		"	},"
  		"	{"
  		"	  \"id\": 3355,"
  		"	  \"name\": \"Scriptingtalen\","
  		"	  \"teacher\": \"Peter Dawyndt\","
  		"	  \"color\": \"brown\","
  		"	  \"year\": \"2023-2024\","
  		"	  \"visibility\": \"visible_for_institution\","
  		"	  \"registration\": \"open_for_institution\","
  		"	  \"created_at\": \"2024-02-08T14:51:07.000+01:00\","
  		"	  \"updated_at\": \"2024-02-10T13:43:05.000+01:00\","
  		"	  \"url\": \"https://dodona.be/nl/courses/3355.json\","
  		"	  \"series\": \"https://dodona.be/nl/courses/3355/series.json\""
  		"	},"
  		"	{"
  		"	  \"id\": 1662,"
  		"	  \"name\": \"Computergebruik\","
  		"	  \"teacher\": \"Peter Dawyndt\","
  		"	  \"color\": \"indigo\","
  		"	  \"year\": \"2022-2023\","
  		"	  \"visibility\": \"visible_for_all\","
  		"	  \"registration\": \"open_for_all\","
  		"	  \"created_at\": \"2022-09-12T12:30:37.000+02:00\","
  		"	  \"updated_at\": \"2023-11-30T15:21:34.000+01:00\","
  		"	  \"url\": \"https://dodona.be/nl/courses/1662.json\","
  		"	  \"series\": \"https://dodona.be/nl/courses/1662/series.json\""
  		"	}"
		"]"
	);

	assert(testObjectCourses != NULL);
	assert(testObjectCourses->_type == WIARRAY);

	wiArrayEl* currentElement = testObjectCourses->contents.arrayVal;
	assert(currentElement != NULL);

	assert(currentElement->elementVal->_type == WIPAIR);
	wiPair* pair = currentElement->elementVal->contents.pairVal;

	assert(pair != NULL);
	assert(strcmp(pair->key, "id") == 0);
	assert(getIntVal(pair) == 3230);
	
	pair = pair->nextPair;
	assert(pair != NULL);
	assert(strcmp(pair->key, "name") == 0);
	assert(strcmp(getStringVal(pair), "Algoritmen en Datastructuren 1") == 0);
	
	pair = pair->nextPair;
	assert(pair != NULL);
	assert(strcmp(pair->key, "teacher") == 0);
	assert(strcmp(getStringVal(pair), "Veerle Fack") == 0);
	
	pair = pair->nextPair;
	assert(pair != NULL);
	assert(strcmp(pair->key, "color") == 0);
	assert(strcmp(getStringVal(pair), "brown") == 0);
	
	pair = pair->nextPair;
	assert(pair != NULL);
	assert(strcmp(pair->key, "year") == 0);
	assert(strcmp(getStringVal(pair), "2023-2024") == 0);
	
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
	assert(strcmp(pair->key, "url") == 0);
	assert(strcmp(getStringVal(pair), "https://dodona.be/nl/courses/3230.json") == 0);
	
	pair = pair->nextPair;
	assert(pair != NULL);
	assert(strcmp(pair->key, "series") == 0);
	assert(strcmp(getStringVal(pair), "https://dodona.be/nl/courses/3230/series.json") == 0);
	
	assert(pair->nextPair == NULL);

	currentElement = currentElement->nextElement; 	// Computergebruik
	assert(currentElement != NULL);
	assert(currentElement->elementVal->_type == WIPAIR);
	currentElement = currentElement->nextElement; 	// Programmeren
	assert(currentElement != NULL);
	assert(currentElement->elementVal->_type == WIPAIR);
	currentElement = currentElement->nextElement; 	// Scriptingtalen
	assert(currentElement != NULL);
	assert(currentElement->elementVal->_type == WIPAIR);
	currentElement = currentElement->nextElement; 	// Computergebruik
	assert(currentElement != NULL);
	assert(currentElement->elementVal->_type == WIPAIR);

	pair = currentElement->elementVal->contents.pairVal;

	assert(pair != NULL);
	assert(strcmp(pair->key, "id") == 0);
	assert(getIntVal(pair) == 1662);
	
	pair = pair->nextPair;
	assert(pair != NULL);
	assert(strcmp(pair->key, "name") == 0);
	assert(strcmp(getStringVal(pair), "Computergebruik") == 0);
	
	pair = pair->nextPair;
	assert(pair != NULL);
	assert(strcmp(pair->key, "teacher") == 0);
	assert(strcmp(getStringVal(pair), "Peter Dawyndt") == 0);
	
	pair = pair->nextPair;
	assert(pair != NULL);
	assert(strcmp(pair->key, "color") == 0);
	assert(strcmp(getStringVal(pair), "indigo") == 0);
	
	pair = pair->nextPair;
	assert(pair != NULL);
	assert(strcmp(pair->key, "year") == 0);
	assert(strcmp(getStringVal(pair), "2022-2023") == 0);
	
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
	assert(strcmp(pair->key, "url") == 0);
	assert(strcmp(getStringVal(pair), "https://dodona.be/nl/courses/1662.json") == 0);
	
	pair = pair->nextPair;
	assert(pair != NULL);
	assert(strcmp(pair->key, "series") == 0);
	assert(strcmp(getStringVal(pair), "https://dodona.be/nl/courses/1662/series.json") == 0);
	
	assert(pair->nextPair == NULL);

	assert(currentElement->nextElement == NULL);

	freeEverything(testObjectCourses);
	char message[] = "Dodona courses test [BRIGHT-GREEN]passed[/].\n\n";
	enrich(message);
	printf("%s", message);
}

void testObject2() {
	// Actual Dodona output
	printf("Testing Dodona series...\n");

	wiValue* testObjectSeries = parseJSON(
		"{"
		"	\"id\":36018,"
		"	\"name\":\"week 12: tweede evaluatie (JavaScript)\","
		"	\"description\":\"test-description101\","
		"	\"visibility\":\"open\","
		"	\"visibility_start\":null,"
		"	\"order\":11,"
		"	\"created_at\":\"2024-02-08T14:51:07.000+01:00\","
		"	\"updated_at\":\"2024-05-25T21:00:13.000+02:00\","
		"	\"deadline\":\"2024-05-14T17:10:00.000+02:00\","
		"	\"url\":\"https://dodona.be/nl/series/36018.json\","
		"	\"course\":\"https://dodona.be/nl/courses/3355.json\","
		"	\"exercises\":\"https://dodona.be/nl/series/36018/activities.json\""
		"}"
	);

	wiPair* pair = testObjectSeries->contents.pairVal;

	assert(get(pair, "id")->contents.intVal == 36018);
	assert(strcmp(get(pair, "url")->contents.stringVal, "https://dodona.be/nl/series/36018.json") == 0);
	assert(get(pair, "order")->contents.intVal == 11);
	assert(get(pair, "visibility_start")->_type == WINULL);
	assert(strcmp(get(pair, "name")->contents.stringVal, "week 12: tweede evaluaie (JavaScript)"));

	freeEverything(testObjectSeries);
	char message[] = "Dodona series test [BRIGHT-GREEN]passed[/].\n\n";
	enrich(message);
	printf("%s", message);
}

