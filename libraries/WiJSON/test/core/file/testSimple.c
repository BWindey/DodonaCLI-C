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

wiValue* openFileAndParse(const char* fileName) {
	FILE* jsonFile = fopen(fileName, "r");
	assert(jsonFile != NULL);
	wiValue* root = parseJSONFile(jsonFile);
	fclose(jsonFile);

	return root;
}

void testInt() {
	wiValue* testInt1 = openFileAndParse("core/file/files/testSimpleInt1.json");
	wiTestEnum(WIINT, testInt1->_type, enumToString);
	wiTestInt(993214241, testInt1->contents.intVal);
	freeEverything(testInt1);

	wiValue* testInt2 = openFileAndParse("core/file/files/testSimpleInt2.json");
	wiTestEnum(WIINT, testInt2->_type, enumToString);
	wiTestInt(-13, testInt2->contents.intVal);
	freeEverything(testInt2);

	wiValue* testInt3 = openFileAndParse("core/file/files/testSimpleInt3.json");
	wiTestEnum(WIINT, testInt3->_type, enumToString);
	wiTestInt(1234567890, testInt3->contents.intVal);
	freeEverything(testInt3);
}

void testBool() {
	wiValue* testTrue = openFileAndParse("core/file/files/testBoolTrue.json");
	wiTestEnum(WIBOOL, testTrue->_type, enumToString);
	wiTestBool(true, testTrue->contents.boolVal);
	freeEverything(testTrue);

	wiValue* testFalse = openFileAndParse("core/file/files/testBoolFalse.json");
	wiTestEnum(WIBOOL, testFalse->_type, enumToString);
	wiTestBool(false, testFalse->contents.boolVal);
	freeEverything(testFalse);
}

void testString() {
	wiValue* testString1 = openFileAndParse("core/file/files/testSimpleString1.json");
	wiTestEnum(WISTRING, testString1->_type, enumToString);
	wiTestString(
		"BEGINhiweonfioewfamcl;mkclm oqfnieowq;mfkl;w mfkfo;wqnmcEND",
		testString1->contents.stringVal
	);
	freeEverything(testString1);

	wiValue* testString2 = openFileAndParse("core/file/files/testSimpleString2.json");
	wiTestEnum(WISTRING, testString2->_type, enumToString);
	wiTestString(
		"Mooi, het leven is \\033[1mmooi!\\033[0m",
		testString2->contents.stringVal
	);
	freeEverything(testString2);
}

void testFloat() {
	wiValue* testLargeNegative = openFileAndParse("core/file/files/testSimpleFloat1.json");
	wiTestEnum(WIFLOAT, testLargeNegative->_type, enumToString);
	wiTestFloat(-3.21e5, testLargeNegative->contents.floatVal);
	freeEverything(testLargeNegative);

	wiValue* testLargePositive = openFileAndParse("core/file/files/testSimpleFloat2.json");
	wiTestEnum(WIFLOAT, testLargePositive->_type, enumToString);
	wiTestFloat(4.128e32, testLargePositive->contents.floatVal);
	freeEverything(testLargePositive);

	wiValue* testPi = openFileAndParse("core/file/files/testSimpleFloat3.json");
	wiTestEnum(WIFLOAT, testPi->_type, enumToString);
	wiTestFloat(3.141592, testPi->contents.floatVal);
	freeEverything(testPi);
}

int main() {
	printf("Testing simple values...\n");

	testInt();
	testBool();
	testString();
	testFloat();

	wiPrintResult("simple values");

	return 0;
}
