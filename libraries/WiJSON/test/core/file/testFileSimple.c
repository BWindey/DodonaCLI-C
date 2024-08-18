#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../../../include/wiJSON.h"
#include "../../../../WiEnrich/include/enrich.h"

void testSimple() {
	FILE* jsonFile = fopen(
		"/home/bram/Programming/CProjects/DodonaCLI/libraries/WiJSON/test/core/file/files/testSimple.json", 
		"r"
	);

	wiValue* root = parseJSONFile(jsonFile);
	fclose(jsonFile);

	assert(root != NULL);
	assert(root->_type == WIPAIR);

	wiPair* pair = root->contents.pairVal;

	assert(pair != NULL);
	assert(strcmp(pair->key, "key1") == 0);
	assert(pair->value != NULL);
	assert(pair->value->_type == WISTRING);
	assert(strcmp(pair->value->contents.stringVal, "value1") == 0);

	freeEverything(root);
}

int main() {
	printf("File: testing simple object...\n");

	testSimple();

	char message[] = "Simple file-test [BRIGHT-GREEN]succeeded[/].\n\n";
	wiEnrich(message);
	printf("%s", message);

	return 0;
}
