#pragma once

#include <stdbool.h>

#include "wiJSON.h"

int getIntVal(wiPair*);
bool getBoolVal(wiPair*);
char* getStringVal(wiPair*);
double getFloatVal(wiPair*);
wiPair* getPairVal(wiPair*);
wiArrayEl* getArrayVal(wiPair*);

wiValue* get(wiPair*, const char*);
