#pragma once

// Possible JSON types
#define wiJSON_OBJECT 1
#define wiJSON_ARRAY  2
#define wiJSON_STRING 3
#define wiJSON_NUMBER 4


typedef struct {
    int type;
} wiJSON;
