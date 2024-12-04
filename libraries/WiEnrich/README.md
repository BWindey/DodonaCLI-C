# wiEnrich library
> [!WARNING]
> For technical reasons, [RED] is not a valid keyword, use `[FRED]`!

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
> Passing a string literal to the function will cause a seg-fault!
> This is due to the nature of string literals, they can't be modified.
> Use strdup("...") to circumvent this.


## NO\_COLOR
When the "$NO\_COLOR" system variable is set,
no colour will be substituted. Only bold, italic,


## How to use
Run `make` inside the 'WiEnrich/' directory.
This will produce `lib/libwienrich.a`, which you can link to your own program,
as a normal object file.

Alternatively, place `wiEnrich.c` alongside your other '.c' files.

This library uses another library of mine: `wiAssert.h`.
You will need to include it when you don't use the first method to get your
library file.
Don't forget to also `#include "wiEnrich.h"` in some way.


## Limitations
There are only a limited set of colours available to use.
When you have some experience with these escape-characters,
you will know why.
This doesn't support extra colours,
you'll need to find the escape-characters for those yourself.

The library also does not keep track of which effects have been applied when.
Practically, this means that you cannot escape/stop one effect at a time.
When inserting a `[/]`, all text will be back to normal.


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
