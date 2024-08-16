#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../../include/wiJSON.h"
#include "../../include/wiUtility.h"
#include "../../../WiEnrich/include/enrich.h"

void testObject1() {
	// Test 1: Simple object with one key-value pair (string)
	wiValue* testObject1 = parseJSONString("{\"key1\":\"value1\"}");
	assert(testObject1->contents.pairVal != NULL);

	wiPair* pair1 = testObject1->contents.pairVal;
	assert(strcmp(pair1->key, "key1") == 0);
	assert(strcmp(getStringVal(pair1), "value1") == 0);

	assert(strcmp(getStringVal(get(pair1, "key1")), "value1") == 0);

	freeEverything(testObject1);
}


void testObject2() {
	// Test 2: Simple object with multiple key-value pairs (int and string)
	wiValue* testObject2 = parseJSONString(
		"{ \"key1\": 42, \"key2\": \"value2\" }"
	);
	assert(testObject2->_type == WIPAIR);
	assert(testObject2->contents.pairVal != NULL);

	wiPair* pair2_kv1 = testObject2->contents.pairVal;
	assert(strcmp(pair2_kv1->key, "key1") == 0);
	assert(getIntVal(pair2_kv1) == 42);

	wiPair* pair2_kv2 = pair2_kv1->nextPair;
	assert(pair2_kv2 != NULL);
	assert(strcmp(pair2_kv2->key, "key2") == 0);
	assert(strcmp(getStringVal(pair2_kv2), "value2") == 0);

	assert(strcmp(getStringVal(get(testObject2->contents.pairVal, "key2")), "value2") == 0);
	assert(getIntVal(get(testObject2->contents.pairVal, "key1")) == 42);

	freeEverything(testObject2);
}


void testObject3() {
	// Test 3: Simple object with multiple key-value pairs (float, bool, string)
	wiValue* testObject3 = parseJSONString("{ \"key1\": 3.14, \"key2\": true, \"key3\": \"value3\" }");
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

	wiPair* pair = testObject3->contents.pairVal;
	assert(getBoolVal(get(pair, "key2")) == true);
	assert(getFloatVal(get(pair, "key1")) == 3.14);

	freeEverything(testObject3);
}


void testObject4() {
	// Test 4: Simple object with an array and other values
	wiValue* testObject4 = parseJSONString("{ \"key1\": [1, 2, 3], \"key2\": false, \"key3\": \"value4\" }");
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

	freeEverything(testObject4);
}


int main() {
	printf("Testing simple objects...\n");

	testObject1();
	testObject2();
	testObject3();
	testObject4();

	char message[] = "Simple object tests [BRIGHT-GREEN]succeeded[/].\n\n";
	wiEnrich(message);
	printf("%s", message);

	return 0;
}


