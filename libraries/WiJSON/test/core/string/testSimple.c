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

void testInt() {
	wiValue* testInt1 = parseJSONString("993214241");
	wiTestEnum(WIINT, testInt1->_type, enumToString);
	wiTestInt(993214241, testInt1->contents.intVal);
	freeEverything(testInt1);

	wiValue* testInt2 = parseJSONString("-13");
	wiTestEnum(WIINT, testInt2->_type, enumToString);
	wiTestInt(-13, testInt2->contents.intVal);
	freeEverything(testInt2);

	wiValue* testInt3 = parseJSONString("1234567890");
	wiTestEnum(WIINT, testInt3->_type, enumToString);
	wiTestInt(1234567890, testInt3->contents.intVal);
	freeEverything(testInt3);
}

void testBool() {
	wiValue* testTrue = parseJSONString("true");
	wiTestEnum(WIBOOL, testTrue->_type, enumToString);
	wiTestBool(true, testTrue->contents.boolVal);
	freeEverything(testTrue);

	wiValue* testFalse = parseJSONString("false");
	wiTestEnum(WIBOOL, testFalse->_type, enumToString);
	wiTestBool(false, testFalse->contents.boolVal);
	freeEverything(testFalse);
}

void testString() {
	wiValue* testString = parseJSONString(
		"\"BEGINhiweonfioewfamcl;mkclm oqfnieowq;mfkl;w mfkfo;wqnmcEND\""
	);
	wiTestEnum(WISTRING, testString->_type, enumToString);
	wiTestString(
		"BEGINhiweonfioewfamcl;mkclm oqfnieowq;mfkl;w mfkfo;wqnmcEND",
		testString->contents.stringVal
	);
	freeEverything(testString);
}

void testFloat() {
	wiValue* testLargeNegative = parseJSONString("-3.21e5");
	wiTestEnum(WIFLOAT, testLargeNegative->_type, enumToString);
	wiTestFloat(-3.21e5, testLargeNegative->contents.floatVal);
	freeEverything(testLargeNegative);

	wiValue* testLargePositive = parseJSONString("4.128e32");
	wiTestEnum(WIFLOAT, testLargePositive->_type, enumToString);
	wiTestFloat(4.128e32, testLargePositive->contents.floatVal);
	freeEverything(testLargePositive);

	wiValue* testPi = parseJSONString("3.141592");
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
