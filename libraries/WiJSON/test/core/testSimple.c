#include <assert.h>
#include <string.h>

#include "../../include/wiJSON.h"
#include "../../../WiEnrich/include/enrich.h"

int main() {
	printf("Testing simple values...\n");

	wiValue* testInt = parseJSONString("993214241");
	wiValue* testBool = parseJSONString("false");
	wiValue* testString = parseJSONString("\"BEGINhiweonfioewfamcl;mkclm oqfnieowq;mfkl;w mfkfo;wqnmcEND\"");
	wiValue* testFloat = parseJSONString("-3.21e5");

	assert(testInt->_type == WIINT);
	assert(testInt->contents.intVal == 993214241);
	freeEverything(testInt);

	assert(testBool->_type == WIBOOL);
	assert(testBool->contents.boolVal == false);
	freeEverything(testBool);

	assert(testString->_type == WISTRING);
	assert(strcmp(testString->contents.stringVal, "BEGINhiweonfioewfamcl;mkclm oqfnieowq;mfkl;w mfkfo;wqnmcEND") == 0);
	freeEverything(testString);

	assert(testFloat->_type == WIFLOAT);
	assert(testFloat->contents.floatVal == -3.21e5);
	freeEverything(testFloat);

	char message[] = "Simple tests [BRIGHT-GREEN]succeeded[/].\n\n";
	wiEnrich(message);
	printf("%s", message);

	return 0;
}


