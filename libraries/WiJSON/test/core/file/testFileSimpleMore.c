#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../../../include/wiJSON.h"
#include "../../../../WiEnrich/include/enrich.h"

void testSimple() {
	FILE* jsonFile = fopen(
		"/home/bram/Programming/CProjects/DodonaCLI/libraries/WiJSON/test/core/file/files/testSimpleMore.json", 
		"r"
	);

	wiValue* root = parseJSONFile(jsonFile);
	fclose(jsonFile);

	assert(root != NULL);
	assert(root->_type == WIPAIR);

	wiPair* pair = root->contents.pairVal;

	assert(strcmp(pair->key, "key1") == 0);
	assert(pair->value->_type == WIINT);
	assert(pair->value->contents.intVal == 321);

	pair = pair->nextPair;
	assert(pair != NULL);

	assert(strcmp(pair->key, "key2") == 0);
	assert(pair->value->_type == WIFLOAT);
	assert(pair->value->contents.floatVal == -321.25);

	pair = pair->nextPair;
	assert(pair != NULL);

	assert(strcmp(pair->key, "key3") == 0);
	assert(pair->value->_type == WISTRING);
	assert(strcmp(pair->value->contents.stringVal, "Hello, World!") == 0);

	pair = pair->nextPair;
	assert(pair != NULL);

	assert(strcmp(pair->key, "key4") == 0);
	assert(pair->value->_type == WIBOOL);
	assert(pair->value->contents.boolVal == false);

	pair = pair->nextPair;
	assert(pair != NULL);

	assert(strcmp(pair->key, "key5") == 0);
	assert(pair->value->_type == WINULL);

	pair = pair->nextPair;
	assert(pair != NULL);

	assert(strcmp(pair->key, "key6") == 0);
	assert(pair->value->_type == WIBOOL);
	assert(pair->value->contents.boolVal == true);
}

int main() {
	printf("File: testing larger simple object...\n");

	testSimple();

	char message[] = "Larger simple file-test [BRIGHT-GREEN]succeeded[/].\n\n";
	wiEnrich(message);
	printf("%s", message);

	return 0;
}
