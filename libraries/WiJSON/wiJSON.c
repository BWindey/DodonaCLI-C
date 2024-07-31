#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "wiJSON.h"

bool isBlank(const char c) {
	return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

wiValue* parseJSON(char *jsonString) {
	wiValue* root = (wiValue*)malloc(sizeof(wiValue));

	unsigned int index = 0;

	// Remove whitespace
	while (isBlank(jsonString[index]) && jsonString[index] != '\0') {
		index++;
	}


	return root;
}


void freeEverything(wiValue* root) {
	free(root->contents.pairVal);
	free(root);
}


int main() {
	wiValue* root = parseJSON("{\"testKey\": \"testVal\"}");

	printf("%s", root->contents.pairVal->key);
	freeEverything(root);
}
