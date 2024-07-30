#include <stdio.h>
#include <stdlib.h>
#include "wiJSON.h"

wiValue* root;

wiValue* parseJSON(char *jsonString) {
	root = (wiValue*)malloc(sizeof(wiValue));
	root->_type = WIPAIR;

	wiPair* pair = (wiPair*)malloc(sizeof(wiPair));
	pair->key = "Hello, World!\n";

	root->contents.pairVal = pair;

	return root;
}


void freeEverything() {
	free(root->contents.pairVal);
	free(root);
}


int main() {
	parseJSON("{\"testKey\": \"testVal\"}");

	printf("%s", root->contents.pairVal->key);
	freeEverything();
}
