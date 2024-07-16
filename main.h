#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "subcommands/display.h"
#include "subcommands/info.h"
#include "subcommands/next.h"
#include "subcommands/post.h"
#include "subcommands/select.h"
#include "subcommands/settings.h"
#include "subcommands/status.h"
#include "subcommands/sub.h"
#include "subcommands/tutorial.h"
#include "subcommands/up.h"

void handleArgs(int, const char**);
