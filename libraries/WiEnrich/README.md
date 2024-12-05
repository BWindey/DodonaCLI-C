# wiEnrich library
> [!WARNING]
> For technical reasons, [RED] is not a valid keyword, use [FRED]!

This is a C-library that allows you to write text like:
```
"This is [GREEN]text [ON BRIGHT-BLUE]on a bright-blue[/] background."
```

The single function `char* enrich(char* message)` will take the string,
and substitute all the keywords in proper escape-characters.

This substitution will happen in-place,
and it will happen in a single pass through the string.

> [!WARNING]
> This library WILL throw a runtime error when it detected a problem.
> Potential problems:
>   - No closing `]` for a non-escaped opening `[`
>   - Invalid keyword between `[]`
>   - Using `[/]` without having used another pattern first
>
> To avoid that your program will crash, test it first ;-)

> [!CAUTION]
> Because this library modifies the string in-place, you need to own the
> string in your program, otherwise a segmentation-fault will happen.

To pass a valid string to the `wiEnrich()` function, you should first assign
it to a `char[]` (NOT a `char*`), or use `strdup()`, which will create a
duplicate on the heap (and create a memory leak if you don't `free` it).


## NO\_COLOR
When the "$NO\_COLOR" system variable is set,
no colour will be substituted. Only bold, italic, ...


## How to get the library
First you need to initialise the submodules:
`git submodule update --init --recursive`.

Next you need 2 things: the header file `include/wiEnrich.h` to include in your
program files, and the code.
There are 2 ways to include the code. When you're using this current DodonaCLIC
repository, and have it locally on your pc, navigate to this folder,
and run `make`.
This will produce `lib/libwienrich.a`, which you can then use for your own
program (compiling would be then something like `gcc main.c libwienrich.a`).

You can also use the `wiEnrich.c` directly, but then you also need to include
`submodules/wiTesting/wiAssert.h` in you project under that exact file-path.
I'm sure you can figure that out, and modify file-paths where needed, but
it is more work then using the make-file.


## Limitations
There are only a limited set of colours available to use.
When you have some experience with these escape-characters,
you will know why.
This doesn't support extra colours,
you'll need to find the escape-characters for those yourself.

The library also does not keep track of which effects have been applied when.
Practically, this means that you cannot escape/stop one effect at a time.
When inserting a `[/]`, all text will be back to normal.


## Example usage
```c
#include <stdio.h>
#include "wiEnrich.h"

int main(void) {
	char msg[] = "[BOLD]This is bold.[/]\nAnd this is [BRIGHT-BLUE]blue[/]!";
	wiEnrich(msg);

	printf("%s\n", msg);

	char second[] = "I found [NUMBER]3.14[/] pies today!";
	printf("%s\n", wiEnrich(second));

	return 0;
}
```

## List of recognised keywords
You'll notice that for every available colour,
there is the colour itself as foreground and background,
and both are also available in their bright versions.

- "[/]"             <- Resets everything back
- "[NUMBER]"        <- BOLD + CYAN
- "[BOLD]"
- "[DIM]"
- "[ITALIC]"
- "[UNDER]"
- "[STRIKE]"

- "[BLACK]"
- "[BRIGHT-BLACK]"
- "[ON BLACK]"
- "[ON BRIGHT-BLACK]"

- "[FRED]"          <- WATCH OUT, has weird name
- "[BRIGHT-RED]"
- "[ON RED]"
- "[ON BRIGHT-RED]"

- "[GREEN]"
- "[BRIGHT-GREEN]"
- "[ON GREEN]"
- "[ON BRIGHT-GREEN]"

- "[YELLOW]"
- "[BRIGHT-YELLOW]"
- "[ON YELLOW]"
- "[ON BRIGHT-YELLOW]"

- "[BLUE]"
- "[BRIGHT-BLUE]"
- "[ON BLUE]"
- "[ON BRIGHT-BLUE]"

- "[MAGENTA]"
- "[BRIGHT-MAGENTA]"
- "[ON MAGENTA]"
- "[ON BRIGHT-MAGENTA]"

- "[CYAN]"
- "[BRIGHT-CYAN]"
- "[ON CYAN]"
- "[ON BRIGHT-CYAN]"

- "[WHITE]"
- "[BRIGHT-WHITE]"
- "[ON WHITE]"
- "[ON BRIGHT-WHITE]"
