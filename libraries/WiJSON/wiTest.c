#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "wiJSON.h"

void testSimpleValues();
void testSimpleArray();
/* void testSimpleObject();

void testObject1();
void testObject2();
void testObject3();
void testObject4(); */

int main() {
	testSimpleValues();
	testSimpleArray();
	/* testSimpleObject();
	
	testObject1();
	testObject2();
	testObject3();
	testObject4(); */

	printf("All tests passed.\n");

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
	printf("Simple tests succeeded.\n");
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
	printf("Simple array test succeeded.\n");
}





