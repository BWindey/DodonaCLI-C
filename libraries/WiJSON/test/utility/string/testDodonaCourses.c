#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../../../include/wiJSON.h"
#include "../../../include/wiUtility.h"
#include "../../../../WiEnrich/include/enrich.h"

int main() {
	// Actual Dodona output
	printf("Testing Dodona courses...\n");
	wiValue* testObjectCourses = parseJSONString(
		"["
		"	{"
	 		"	  \"id\": 3230,"
	 		"	  \"name\": \"Algoritmen en Datastructuren 1\","
	 		"	  \"teacher\": \"Veerle Fack\","
	 		"	  \"color\": \"brown\","
	 		"	  \"year\": \"2023-2024\","
	 		"	  \"visibility\": \"visible_for_institution\","
	 		"	  \"registration\": \"open_for_institution\","
	 		"	  \"created_at\": \"2024-01-10T09:50:53.000+01:00\","
	 		"	  \"updated_at\": \"2024-02-09T11:08:07.000+01:00\","
	 		"	  \"url\": \"https://dodona.be/nl/courses/3230.json\","
	 		"	  \"series\": \"https://dodona.be/nl/courses/3230/series.json\""
	 		"	},"
	 		"	{"
	 		"	  \"id\": 2876,"
	 		"	  \"name\": \"Computergebruik\","
	 		"	  \"teacher\": \"Peter Dawyndt\","
	 		"	  \"color\": \"brown\","
	 		"	  \"year\": \"2023-2024\","
	 		"	  \"visibility\": \"visible_for_institution\","
	 		"	  \"registration\": \"open_for_institution\","
	 		"	  \"created_at\": \"2023-09-16T15:12:14.000+02:00\","
	 		"	  \"updated_at\": \"2023-11-30T15:20:31.000+01:00\","
	 		"	  \"url\": \"https://dodona.be/nl/courses/2876.json\","
	 		"	  \"series\": \"https://dodona.be/nl/courses/2876/series.json\""
	 		"	},"
	 		"	{"
	 		"	  \"id\": 2955,"
	 		"	  \"name\": \"Programmeren\","
	 		"	  \"teacher\": \"Kris Coolsaet\","
	 		"	  \"color\": \"brown\","
	 		"	  \"year\": \"2023-2024\","
	 		"	  \"visibility\": \"visible_for_institution\","
	 		"	  \"registration\": \"open_for_institution\","
	 		"	  \"created_at\": \"2023-09-27T10:04:55.000+02:00\","
	 		"	  \"updated_at\": \"2023-09-27T10:04:55.000+02:00\","
	 		"	  \"url\": \"https://dodona.be/nl/courses/2955.json\","
	 		"	  \"series\": \"https://dodona.be/nl/courses/2955/series.json\""
	 		"	},"
	 		"	{"
	 		"	  \"id\": 3355,"
	 		"	  \"name\": \"Scriptingtalen\","
	 		"	  \"teacher\": \"Peter Dawyndt\","
	 		"	  \"color\": \"brown\","
	 		"	  \"year\": \"2023-2024\","
	 		"	  \"visibility\": \"visible_for_institution\","
	 		"	  \"registration\": \"open_for_institution\","
	 		"	  \"created_at\": \"2024-02-08T14:51:07.000+01:00\","
	 		"	  \"updated_at\": \"2024-02-10T13:43:05.000+01:00\","
	 		"	  \"url\": \"https://dodona.be/nl/courses/3355.json\","
	 		"	  \"series\": \"https://dodona.be/nl/courses/3355/series.json\""
	 		"	},"
	 		"	{"
	 		"	  \"id\": 1662,"
	 		"	  \"name\": \"Computergebruik\","
	 		"	  \"teacher\": \"Peter Dawyndt\","
	 		"	  \"color\": \"indigo\","
	 		"	  \"year\": \"2022-2023\","
	 		"	  \"visibility\": \"visible_for_all\","
	 		"	  \"registration\": \"open_for_all\","
	 		"	  \"created_at\": \"2022-09-12T12:30:37.000+02:00\","
	 		"	  \"updated_at\": \"2023-11-30T15:21:34.000+01:00\","
	 		"	  \"url\": \"https://dodona.be/nl/courses/1662.json\","
	 		"	  \"series\": \"https://dodona.be/nl/courses/1662/series.json\""
	 		"	}"
		"]"
	);

	assert(testObjectCourses != NULL);
	assert(testObjectCourses->_type == WIARRAY);

	wiArrayEl* currentElement = testObjectCourses->contents.arrayVal;
	assert(currentElement != NULL);

	assert(currentElement->elementVal->_type == WIPAIR);
	wiPair* pair = currentElement->elementVal->contents.pairVal;

	assert(pair != NULL);
	assert(strcmp(pair->key, "id") == 0);
	assert(getIntVal(pair) == 3230);

	pair = pair->nextPair;
	assert(pair != NULL);
	assert(strcmp(pair->key, "name") == 0);
	assert(strcmp(getStringVal(pair), "Algoritmen en Datastructuren 1") == 0);

	pair = pair->nextPair;
	assert(pair != NULL);
	assert(strcmp(pair->key, "teacher") == 0);
	assert(strcmp(getStringVal(pair), "Veerle Fack") == 0);

	pair = pair->nextPair;
	assert(pair != NULL);
	assert(strcmp(pair->key, "color") == 0);
	assert(strcmp(getStringVal(pair), "brown") == 0);

	pair = pair->nextPair;
	assert(pair != NULL);
	assert(strcmp(pair->key, "year") == 0);
	assert(strcmp(getStringVal(pair), "2023-2024") == 0);

	pair = pair->nextPair; 	// Visibility
	assert(pair != NULL);
	pair = pair->nextPair; 	// Registration
	assert(pair != NULL);
	pair = pair->nextPair; 	// Created_at
	assert(pair != NULL);
	pair = pair->nextPair; 	// Updated_at
	assert(pair != NULL);

	pair = pair->nextPair;
	assert(pair != NULL);
	assert(strcmp(pair->key, "url") == 0);
	assert(strcmp(getStringVal(pair), "https://dodona.be/nl/courses/3230.json") == 0);

	pair = pair->nextPair;
	assert(pair != NULL);
	assert(strcmp(pair->key, "series") == 0);
	assert(strcmp(getStringVal(pair), "https://dodona.be/nl/courses/3230/series.json") == 0);

	assert(pair->nextPair == NULL);

	currentElement = currentElement->nextElement; 	// Computergebruik
	assert(currentElement != NULL);
	assert(currentElement->elementVal->_type == WIPAIR);
	currentElement = currentElement->nextElement; 	// Programmeren
	assert(currentElement != NULL);
	assert(currentElement->elementVal->_type == WIPAIR);
	currentElement = currentElement->nextElement; 	// Scriptingtalen
	assert(currentElement != NULL);
	assert(currentElement->elementVal->_type == WIPAIR);
	currentElement = currentElement->nextElement; 	// Computergebruik
	assert(currentElement != NULL);
	assert(currentElement->elementVal->_type == WIPAIR);

	pair = currentElement->elementVal->contents.pairVal;

	assert(pair != NULL);
	assert(strcmp(pair->key, "id") == 0);
	assert(getIntVal(pair) == 1662);

	pair = pair->nextPair;
	assert(pair != NULL);
	assert(strcmp(pair->key, "name") == 0);
	assert(strcmp(getStringVal(pair), "Computergebruik") == 0);

	pair = pair->nextPair;
	assert(pair != NULL);
	assert(strcmp(pair->key, "teacher") == 0);
	assert(strcmp(getStringVal(pair), "Peter Dawyndt") == 0);

	pair = pair->nextPair;
	assert(pair != NULL);
	assert(strcmp(pair->key, "color") == 0);
	assert(strcmp(getStringVal(pair), "indigo") == 0);

	pair = pair->nextPair;
	assert(pair != NULL);
	assert(strcmp(pair->key, "year") == 0);
	assert(strcmp(getStringVal(pair), "2022-2023") == 0);

	pair = pair->nextPair; 	// Visibility
	assert(pair != NULL);
	pair = pair->nextPair; 	// Registration
	assert(pair != NULL);
	pair = pair->nextPair; 	// Created_at
	assert(pair != NULL);
	pair = pair->nextPair; 	// Updated_at
	assert(pair != NULL);

	pair = pair->nextPair;
	assert(pair != NULL);
	assert(strcmp(pair->key, "url") == 0);
	assert(strcmp(getStringVal(pair), "https://dodona.be/nl/courses/1662.json") == 0);

	pair = pair->nextPair;
	assert(pair != NULL);
	assert(strcmp(pair->key, "series") == 0);
	assert(strcmp(getStringVal(pair), "https://dodona.be/nl/courses/1662/series.json") == 0);

	assert(pair->nextPair == NULL);

	assert(currentElement->nextElement == NULL);

	freeEverything(testObjectCourses);
	char message[] = "Dodona courses test [BRIGHT-GREEN]passed[/].\n\n";
	wiEnrich(message);
	printf("%s", message);
}
