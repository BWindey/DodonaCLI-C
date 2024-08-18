#include <assert.h>

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

void testObject1() {
	// Test 1: Simple object with one key-value pair (string)
	wiValue* testObject1 = parseJSONString("{\"key1\":\"value1\"}");
	wiTestEnum(WIPAIR, testObject1->_type, enumToString);
	assert(testObject1->contents.pairVal != NULL);

	wiPair* pair1 = testObject1->contents.pairVal;
	wiTestString("key1", pair1->key);
	wiTestEnum(WISTRING, pair1->value->_type, enumToString);
	wiTestString("value1", pair1->value->contents.stringVal);

	freeEverything(testObject1);
}

void testObject2() {
	// Test 2: Simple object with multiple key-value pairs (int and string)
	wiValue* testObject2 = parseJSONString("{ \"key1\": 42, \"key2\": \"value2\" }");
	wiTestEnum(WIPAIR, testObject2->_type, enumToString);
	assert(testObject2->contents.pairVal != NULL);

	wiPair* pair2_kv1 = testObject2->contents.pairVal;
	wiTestString("key1", pair2_kv1->key);
	wiTestEnum(WIINT, pair2_kv1->value->_type, enumToString);
	wiTestInt(42, pair2_kv1->value->contents.intVal);

	wiPair* pair2_kv2 = pair2_kv1->nextPair;
	assert(pair2_kv2 != NULL);
	wiTestString("key2", pair2_kv2->key);
	wiTestEnum(WISTRING, pair2_kv2->value->_type, enumToString);
	wiTestString("value2", pair2_kv2->value->contents.stringVal);

	freeEverything(testObject2);
}

void testObject3() {
	// Test 3: Simple object with multiple key-value pairs (float, bool, string)
	wiValue* testObject3 = parseJSONString("{ \"key1\": 3.14, \"key2\": true, \"key3\": \"value3\" }");
	wiTestEnum(WIPAIR, testObject3->_type, enumToString);
	assert(testObject3->contents.pairVal != NULL);

	wiPair* pair3_kv1 = testObject3->contents.pairVal;
	wiTestString("key1", pair3_kv1->key);
	wiTestEnum(WIFLOAT, pair3_kv1->value->_type, enumToString);
	wiTestFloat(3.14, pair3_kv1->value->contents.floatVal);

	wiPair* pair3_kv2 = pair3_kv1->nextPair;
	assert(pair3_kv2 != NULL);
	wiTestString("key2", pair3_kv2->key);
	wiTestEnum(WIBOOL, pair3_kv2->value->_type, enumToString);
	wiTestBool(true, pair3_kv2->value->contents.boolVal);

	wiPair* pair3_kv3 = pair3_kv2->nextPair;
	assert(pair3_kv3 != NULL);
	wiTestString("key3", pair3_kv3->key);
	wiTestEnum(WISTRING, pair3_kv3->value->_type, enumToString);
	wiTestString("value3", pair3_kv3->value->contents.stringVal);

	freeEverything(testObject3);
}

void testObject4() {
	// Test 4: Simple object with an array and other values
	wiValue* testObject4 = parseJSONString(
			"{ \"key1\": [1, 2, 3], \"key2\": false, \"key3\": \"value4\" }"
	);
	wiTestEnum(WIPAIR, testObject4->_type, enumToString);
	assert(testObject4->contents.pairVal != NULL);

	wiPair* pair4_kv1 = testObject4->contents.pairVal;
	wiTestString("key1", pair4_kv1->key);
	wiTestEnum(WIARRAY, pair4_kv1->value->_type, enumToString);
	assert(pair4_kv1->value->contents.arrayVal != NULL);

	// Verify array contents [1, 2, 3]
	wiArrayEl* array1 = pair4_kv1->value->contents.arrayVal;
	wiTestEnum(WIINT, array1->elementVal->_type, enumToString);
	wiTestInt(1, array1->elementVal->contents.intVal);
	assert(array1->nextElement != NULL);

	wiArrayEl* array2 = array1->nextElement;
	wiTestEnum(WIINT, array2->elementVal->_type, enumToString);
	wiTestInt(2, array2->elementVal->contents.intVal);
	assert(array2->nextElement != NULL);

	wiArrayEl* array3 = array2->nextElement;
	wiTestEnum(WIINT, array3->elementVal->_type, enumToString);
	wiTestInt(3, array3->elementVal->contents.intVal);

	wiPair* pair4_kv2 = pair4_kv1->nextPair;
	assert(pair4_kv2 != NULL);
	wiTestString("key2", pair4_kv2->key);
	wiTestEnum(WIBOOL, pair4_kv2->value->_type, enumToString);
	wiTestBool(false, pair4_kv2->value->contents.boolVal);

	wiPair* pair4_kv3 = pair4_kv2->nextPair;
	assert(pair4_kv3 != NULL);
	wiTestString("key3", pair4_kv3->key);
	wiTestEnum(WISTRING, pair4_kv3->value->_type, enumToString);
	wiTestString("value4", pair4_kv3->value->contents.stringVal);

	freeEverything(testObject4);
}

int main() {
	printf("Testing simple objects...\n");

	testObject1();
	testObject2();
	testObject3();
	testObject4();

	wiPrintResult("simple objects");

	return 0;
}


