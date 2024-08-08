# DodonaCLI in C 
As a challenge for myself, and to learn the language, I decided to try rewrite as much as possible from the original DodonaCLI (https://github.com/BWindey/DodonaCLI), 
written by me in Python, in the C programming language.

If you stumbled upon this, and want to actively use this, please go towards the original: https://github.com/BWindey/DodonaCLI

As long as this version isn't somewhat feature-complete, I'll never recommend it.


## Requires:
gcc and Make to compile, and the C99 standard (for `strtol` and `strtod`). 
No idea if this works on Windows/MacOS, haven't tested

## To Do:
- [ ] All the subcommands:
    - [ ] Display (API)
    - [X] Info: --help, version, completion, github, changelog, man-page
    - [ ] Next (API)
    - [ ] Post: selected exerc, link top file (API)
    - [ ] Settings (low priority)
    - [ ] Status
    - [ ] Sub: load, view (API)
    - [ ] Tutorial (API)
    - [ ] Up
- [ ] Implement https requests
- [ ] Implement json 
- [ ] Implement save-file
- [ ] Create some abstraction layer around API (maybe some level of caching)
- [X] Create some library for displaying rich text (colours, bold, italic, ...)

