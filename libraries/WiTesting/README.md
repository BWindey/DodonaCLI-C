# WiTesting
This is a small "library" to help writing tests.
It consists of a single header-file that contains everything you need.

It works on primitive types:
- int 
- float
- bool 
- string
- char

And it also works on enums, given that you provide it with a function that
converts your enum to a string.
For example:
```c
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
```

## Usage
There is a function for every type called `wiTest<Type>` (f.e. `wiTestInt`).
Each function expects 2 arguments: EXPECTED and GOT.

`wiTestEnum` however also expects a third argument `enumToString`, a function.

The functions will evaulate whether EXPECTED == GOT, 
and print out a message to stderr when this evaluation fails:
[!example_output.png]

> [!IMPORTANT]
> These testing-functions will never abort the program.

The wiTest.h headerfile also provides an `wiFailedTests` variable.
This variable will count up for every failed test, 
and can thus be used to check if any test failed.


## Example code 
```c 
#include "wiTest.h"

const char* enumToString(wiType type) {
	switch (type) {
        case WIBOOL: return "WIBOOL";
        case WIFLOAT: return "WIFLOAT";
        case WIINT: return "WIINT";
        case WISTRING: return "WISTRING";
		default: return "WI_UNKNOWN";
    }
}

int main() {
    // Parse JSON
	wiValue* testInt1 = parseJSONString("993214241");

    // Test if correctly parsed
	wiTestEnum(WIINT, testInt1->_type, enumToString);
	wiTestInt(993214240, testInt1->contents.intVal);

    // Free allocated memory
	freeEverything(testInt1);

	if (wiFailedTests > 1) {
		printf("%d failed tests", wiFailedTests);
	} else if (wiFailedTests == 1) {
		printf("1 failed test", wiFailedTests);
	} else {
		printf("All tests succeeded.");
	}

    return wiFailedTests;
}
```
