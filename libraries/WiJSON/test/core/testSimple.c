#include <assert.h>
#include <string.h>

#include "../../include/wiJSON.h"
#include "../../../WiEnrich/include/enrich.h"

void testInt() {
	wiValue* testInt1 = parseJSONString("993214241");
	assert(testInt1->_type == WIINT);
	assert(testInt1->contents.intVal == 993214241);
	freeEverything(testInt1);

	wiValue* testInt2 = parseJSONString("-13");
	assert(testInt2->_type == WIINT);
	assert(testInt2->contents.intVal == -13);
	freeEverything(testInt2);

	wiValue* testInt3 = parseJSONString("1234567890");
	assert(testInt3->_type == WIINT);
	assert(testInt3->contents.intVal == 1234567890);
	freeEverything(testInt3);
}

void testBool() {
	wiValue* testTrue = parseJSONString("true");
	assert(testTrue->_type == WIBOOL);
	assert(testTrue->contents.boolVal == true);
	freeEverything(testTrue);

	wiValue* testFalse = parseJSONString("false");
	assert(testFalse->_type == WIBOOL);
	assert(testFalse->contents.boolVal == false);
	freeEverything(testFalse);
}

void testString() {
	wiValue* testString = parseJSONString("\"BEGINhiweonfioewfamcl;mkclm oqfnieowq;mfkl;w mfkfo;wqnmcEND\"");
	assert(testString->_type == WISTRING);
	assert(strcmp(testString->contents.stringVal, "BEGINhiweonfioewfamcl;mkclm oqfnieowq;mfkl;w mfkfo;wqnmcEND") == 0);
	freeEverything(testString);
}

void testFloat() {
	wiValue* testLargeNegative = parseJSONString("-3.21e5");
	assert(testLargeNegative->_type == WIFLOAT);
	assert(testLargeNegative->contents.floatVal == -3.21e5);
	freeEverything(testLargeNegative);

	wiValue* testLargePositive = parseJSONString("4.128e32");
	assert(testLargePositive->_type == WIFLOAT);
	assert(testLargePositive->contents.floatVal == 4.128e32);
	freeEverything(testLargePositive);

	wiValue* testPi = parseJSONString("3.141592");
	assert(testPi->_type == WIFLOAT);
	assert(testPi->contents.floatVal == 3.141592);
	freeEverything(testPi);
}

int main() {
	printf("Testing simple values...\n");

	testInt();
	testBool();
	testString();
	testFloat();

	char message[] = "Simple tests [BRIGHT-GREEN]succeeded[/].\n\n";
	wiEnrich(message);
	printf("%s", message);

	return 0;
}
