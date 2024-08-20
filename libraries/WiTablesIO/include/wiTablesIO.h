#pragma once

typedef struct wiBox {
	char* title;
	int width;
} wiBox;

typedef struct wiRow {
	char** elements;
	wiBox* extraInfo;
} wiRow;

typedef struct wiShortcuts {
	char quit;
	char up;
	char down;
	char left;
	char right;
} wiShortcuts;

typedef struct wiTable {
	char* title;
	wiRow** rows;
	wiShortcuts* shortcuts;
} wiTable;


wiTable* wiCreateTable(char* title);
void wiFreeTable(wiTable* table);
