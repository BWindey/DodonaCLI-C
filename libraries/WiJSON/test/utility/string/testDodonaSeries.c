#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../../../include/wiJSON.h"
#include "../../../include/wiUtility.h"
#include "../../../../WiEnrich/include/enrich.h"

int main() {
	// Actual Dodona output
	printf("Testing Dodona series...\n");

	wiValue* testObjectSeries = parseJSONString(
		"{"
		"	\"id\":36018,"
		"	\"name\":\"week 12: tweede evaluatie (JavaScript)\","
		"	\"description\":\"test-description101\","
		"	\"visibility\":\"open\","
		"	\"visibility_start\":null,"
		"	\"order\":11,"
		"	\"created_at\":\"2024-02-08T14:51:07.000+01:00\","
		"	\"updated_at\":\"2024-05-25T21:00:13.000+02:00\","
		"	\"deadline\":\"2024-05-14T17:10:00.000+02:00\","
		"	\"url\":\"https://dodona.be/nl/series/36018.json\","
		"	\"course\":\"https://dodona.be/nl/courses/3355.json\","
		"	\"exercises\":\"https://dodona.be/nl/series/36018/activities.json\""
		"}"
	);

	wiPair* pair = testObjectSeries->contents.pairVal;

	assert(getIntVal(get(pair, "id")) == 36018);
	assert(getIntVal(get(pair, "order")) == 11);
	assert(isNull(get(pair, "visibility_start")));
	assert(strcmp(getStringVal(get(pair, "url")), "https://dodona.be/nl/series/36018.json") == 0);
	assert(strcmp(getStringVal(get(pair, "name")), "week 12: tweede evaluaie (JavaScript)"));

	freeEverything(testObjectSeries);
	char message[] = "Dodona series test [BRIGHT-GREEN]passed[/].\n\n";
	wiEnrich(message);
	printf("%s", message);
}
