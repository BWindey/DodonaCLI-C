#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "wiJSON.h"
#include "../Enrich/enrich.h"


void testSimpleValues();
void testSimpleArray();
void testSimpleObject();

void testObject1();
void testObject2();
/* void testObject3();
void testObject4(); */

int main() {
	char msg[] = "[BRIGHT-YELLOW][UNDER]Core tests...[/]\n";
	enrich(msg);
	printf("%s", msg);

	testSimpleValues();
	testSimpleArray();
	testSimpleObject();
	
	testObject1();
	testObject2();
	/* testObject3();
	testObject4(); */

	char message[] = "[BLACK][ON BRIGHT-GREEN]All tests passed.[/]\n\n";
	enrich(message);
	printf("%s", message);

	return 0;
}

void testSimpleValues() {
	printf("Testing simple values...\n");
	wiValue* testInt = parseJSON("993214241");
	wiValue* testBool = parseJSON("false");
	wiValue* testString = parseJSON("\"BEGINhiweonfioewfamcl;mkclm oqfnieowq;mfkl;w mfkfo;wqnmcEND\"");
	wiValue* testFloat = parseJSON("-3.21e5");

	assert(testInt->_type == WIINT);
	assert(testInt->contents.intVal == 993214241);

	assert(testBool->_type == WIBOOL);
	assert(testBool->contents.boolVal == false);

	assert(testString->_type == WISTRING);
	assert(strcmp(testString->contents.stringVal, "BEGINhiweonfioewfamcl;mkclm oqfnieowq;mfkl;w mfkfo;wqnmcEND") == 0);

	assert(testFloat->_type == WIFLOAT);
	assert(testFloat->contents.floatVal == -3.21e5);

	char message[] = "Simple tests [BRIGHT-GREEN]succeeded[/].\n\n";
	enrich(message);
	printf("%s", message);
}

void testSimpleArray() {
	printf("Testing simple array...\n");
	wiValue* testArray = parseJSON(
		"[ 1, -32.4, \"Hello, World!\", true, false, -32e4, null]"
	);

	assert(testArray->_type == WIARRAY);
	assert(testArray->contents.arrayVal != NULL);
	assert(testArray->contents.arrayVal->elementVal != NULL);

	wiArrayEl* currentElement = testArray->contents.arrayVal;

	assert(currentElement != NULL);
	assert(currentElement->elementVal->_type == WIINT);
	assert(currentElement->elementVal->contents.intVal == 1);

	currentElement = currentElement->nextElement;

	assert(currentElement != NULL);
	assert(currentElement->elementVal->_type == WIFLOAT);
	assert(currentElement->elementVal->contents.floatVal == -32.4);

	currentElement = currentElement->nextElement;

	assert(currentElement != NULL);
	assert(currentElement->elementVal->_type == WISTRING);
	assert(strcmp(currentElement->elementVal->contents.stringVal, "Hello, World!") == 0);

	currentElement = currentElement->nextElement;

	assert(currentElement != NULL);
	assert(currentElement->elementVal->_type == WIBOOL);
	assert(currentElement->elementVal->contents.boolVal == true);

	currentElement = currentElement->nextElement;

	assert(currentElement != NULL);
	assert(currentElement->elementVal->_type == WIBOOL);
	assert(currentElement->elementVal->contents.boolVal == false);

	currentElement = currentElement->nextElement;

	assert(currentElement != NULL);
	assert(currentElement->elementVal->_type == WIFLOAT);
	assert(currentElement->elementVal->contents.floatVal == -32e4);

	currentElement = currentElement->nextElement;

	assert(currentElement != NULL);
	assert(currentElement->elementVal->_type == WINULL);

	char message[] = "Simple array tests [BRIGHT-GREEN]succeeded[/].\n\n";
	enrich(message);
	printf("%s", message);
}

void testSimpleObject() {
	printf("Testing simple objects...\n");

	// Test 1: Simple object with one key-value pair (string)
	wiValue* testObject1 = parseJSON("{\"key1\":\"value1\"}");
	assert(testObject1->_type == WIPAIR);
	assert(testObject1->contents.pairVal != NULL);

	wiPair* pair1 = testObject1->contents.pairVal;
	assert(strcmp(pair1->key, "key1") == 0);
	assert(pair1->value->_type == WISTRING);
	assert(strcmp(pair1->value->contents.stringVal, "value1") == 0);

	// Test 2: Simple object with multiple key-value pairs (int and string)
	wiValue* testObject2 = parseJSON("{ \"key1\": 42, \"key2\": \"value2\" }");
	assert(testObject2->_type == WIPAIR);
	assert(testObject2->contents.pairVal != NULL);

	wiPair* pair2_kv1 = testObject2->contents.pairVal;
	assert(strcmp(pair2_kv1->key, "key1") == 0);
	assert(pair2_kv1->value->_type == WIINT);
	assert(pair2_kv1->value->contents.intVal == 42);

	wiPair* pair2_kv2 = pair2_kv1->nextPair;
	assert(pair2_kv2 != NULL);
	assert(strcmp(pair2_kv2->key, "key2") == 0);
	assert(pair2_kv2->value->_type == WISTRING);
	assert(strcmp(pair2_kv2->value->contents.stringVal, "value2") == 0);

	// Test 3: Simple object with multiple key-value pairs (float, bool, string)
	wiValue* testObject3 = parseJSON("{ \"key1\": 3.14, \"key2\": true, \"key3\": \"value3\" }");
	assert(testObject3->_type == WIPAIR);
	assert(testObject3->contents.pairVal != NULL);

	wiPair* pair3_kv1 = testObject3->contents.pairVal;
	assert(strcmp(pair3_kv1->key, "key1") == 0);
	assert(pair3_kv1->value->_type == WIFLOAT);
	assert(pair3_kv1->value->contents.floatVal == 3.14);

	wiPair* pair3_kv2 = pair3_kv1->nextPair;
	assert(pair3_kv2 != NULL);
	assert(strcmp(pair3_kv2->key, "key2") == 0);
	assert(pair3_kv2->value->_type == WIBOOL);
	assert(pair3_kv2->value->contents.boolVal == true);

	wiPair* pair3_kv3 = pair3_kv2->nextPair;
	assert(pair3_kv3 != NULL);
	assert(strcmp(pair3_kv3->key, "key3") == 0);
	assert(pair3_kv3->value->_type == WISTRING);
	assert(strcmp(pair3_kv3->value->contents.stringVal, "value3") == 0);

	// Test 4: Simple object with an array and other values
	wiValue* testObject4 = parseJSON("{ \"key1\": [1, 2, 3], \"key2\": false, \"key3\": \"value4\" }");
	assert(testObject4->_type == WIPAIR);
	assert(testObject4->contents.pairVal != NULL);

	wiPair* pair4_kv1 = testObject4->contents.pairVal;
	assert(strcmp(pair4_kv1->key, "key1") == 0);
	assert(pair4_kv1->value->_type == WIARRAY);
	assert(pair4_kv1->value->contents.arrayVal != NULL);

	// Verify array contents [1, 2, 3]
	wiArrayEl* array1 = pair4_kv1->value->contents.arrayVal;
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
	assert(pair4_kv2->value->_type == WIBOOL);
	assert(pair4_kv2->value->contents.boolVal == false);

	wiPair* pair4_kv3 = pair4_kv2->nextPair;
	assert(pair4_kv3 != NULL);
	assert(strcmp(pair4_kv3->key, "key3") == 0);
	assert(pair4_kv3->value->_type == WISTRING);
	assert(strcmp(pair4_kv3->value->contents.stringVal, "value4") == 0);

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
	assert(pair->value->_type == WIINT);
	assert(pair->value->contents.intVal == 3230);
	
	pair = pair->nextPair;
	assert(pair != NULL);
	assert(strcmp(pair->key, "name") == 0);
	assert(pair->value->_type == WISTRING);
	assert(strcmp(pair->value->contents.stringVal, "Algoritmen en Datastructuren 1") == 0);
	
	pair = pair->nextPair;
	assert(pair != NULL);
	assert(strcmp(pair->key, "teacher") == 0);
	assert(pair->value->_type == WISTRING);
	assert(strcmp(pair->value->contents.stringVal, "Veerle Fack") == 0);
	
	pair = pair->nextPair;
	assert(pair != NULL);
	assert(strcmp(pair->key, "color") == 0);
	assert(pair->value->_type == WISTRING);
	assert(strcmp(pair->value->contents.stringVal, "brown") == 0);
	
	pair = pair->nextPair;
	assert(pair != NULL);
	assert(strcmp(pair->key, "year") == 0);
	assert(pair->value->_type == WISTRING);
	assert(strcmp(pair->value->contents.stringVal, "2023-2024") == 0);
	
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
	assert(pair->value->_type == WISTRING);
	assert(strcmp(pair->value->contents.stringVal, "https://dodona.be/nl/courses/3230.json") == 0);
	
	pair = pair->nextPair;
	assert(pair != NULL);
	assert(strcmp(pair->key, "series") == 0);
	assert(pair->value->_type == WISTRING);
	assert(strcmp(pair->value->contents.stringVal, "https://dodona.be/nl/courses/3230/series.json") == 0);
	
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
	assert(pair->value->_type == WIINT);
	assert(pair->value->contents.intVal == 1662);
	
	pair = pair->nextPair;
	assert(pair != NULL);
	assert(strcmp(pair->key, "name") == 0);
	assert(pair->value->_type == WISTRING);
	assert(strcmp(pair->value->contents.stringVal, "Computergebruik") == 0);
	
	pair = pair->nextPair;
	assert(pair != NULL);
	assert(strcmp(pair->key, "teacher") == 0);
	assert(pair->value->_type == WISTRING);
	assert(strcmp(pair->value->contents.stringVal, "Peter Dawyndt") == 0);
	
	pair = pair->nextPair;
	assert(pair != NULL);
	assert(strcmp(pair->key, "color") == 0);
	assert(pair->value->_type == WISTRING);
	assert(strcmp(pair->value->contents.stringVal, "indigo") == 0);
	
	pair = pair->nextPair;
	assert(pair != NULL);
	assert(strcmp(pair->key, "year") == 0);
	assert(pair->value->_type == WISTRING);
	assert(strcmp(pair->value->contents.stringVal, "2022-2023") == 0);
	
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
	assert(pair->value->_type == WISTRING);
	assert(strcmp(pair->value->contents.stringVal, "https://dodona.be/nl/courses/1662.json") == 0);
	
	pair = pair->nextPair;
	assert(pair != NULL);
	assert(strcmp(pair->key, "series") == 0);
	assert(pair->value->_type == WISTRING);
	assert(strcmp(pair->value->contents.stringVal, "https://dodona.be/nl/courses/1662/series.json") == 0);
	
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
			"{\"id\":36018,\"name\":\"week 12: tweede evaluatie (JavaScript)\",\"description\":\"test-description101\",\"visibility\":\"open\",\"visibility_start\":null,\"order\":11,\"created_at\":\"2024-02-08T14:51:07.000+01:00\",\"updated_at\":\"2024-05-25T21:00:13.000+02:00\",\"deadline\":\"2024-05-14T17:10:00.000+02:00\",\"url\":\"https://dodona.be/nl/series/36018.json\",\"course\":\"https://dodona.be/nl/courses/3355.json\",\"exercises\":\"https://dodona.be/nl/series/36018/activities.json\"}"
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
	char message[] = "Dodona series test [BRIGHT-GREEN]passed[/].\n\n";
	enrich(message);
	printf("%s", message);
}

