#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "wiJSON.h"
#include "wiUtility.h"

int getIntVal(wiPair* pair) {
	assert(pair->value->_type == WIINT);
	return pair->value->contents.intVal;
}

bool getBoolVal(wiPair* pair) {
	assert(pair->value->_type == WIBOOL);
	return pair->value->contents.boolVal;
}

char* getStringVal(wiPair* pair) {
	assert(pair->value->_type == WISTRING);
	return pair->value->contents.stringVal;
}

double getFloatVal(wiPair* pair) {
	assert(pair->value->_type == WIFLOAT);
	return pair->value->contents.floatVal;
}

wiPair* getPairVal(wiPair* pair) {
	assert(pair->value->_type == WIPAIR);
	return pair->value->contents.pairVal;
}

wiArrayEl* getArrayVal(wiPair* pair) {
	assert(pair->value->_type == WIARRAY);
	return pair->value->contents.arrayVal;
}

/*
 * Finds a pair with a matching key, and returns the corresponding value.
 * 
 * Returns NULL if none was present.
 */
wiValue* get(wiPair* pair, const char* key) {
	while (pair != NULL && strcmp(pair->key, key) != 0) {
		pair = pair->nextPair;
	}
	if (pair == NULL) {
		return NULL;
	}
	return pair->value;
}
