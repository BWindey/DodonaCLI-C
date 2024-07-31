#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "wiJSON.h"

// Forward declarations
bool isBlank(const char c);
int parseJSONValue(const char*, unsigned int);


// For testing purposes, a main function.
// TODO: Remove this when done testing
int main() {
	wiValue* root = parseJSON("\"test\"");

	printf("%s", root->contents.pairVal->key);
	freeEverything(root);
}


wiValue* parseJSON(const char *jsonString) {
	wiValue* root = (wiValue*)malloc(sizeof(wiValue));

	unsigned int index = 0;

	// Remove whitespace
	while (isBlank(jsonString[index]) && jsonString[index] != '\0') {
		index++;
	}

	parseJSONValue(jsonString, index);


	return root;
}


void freeEverything(wiValue* root) {
	free(root->contents.pairVal);
	free(root);
}


bool isBlank(const char c) {
	return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}
