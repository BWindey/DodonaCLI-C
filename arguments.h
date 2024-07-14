#pragma once

const char HELP[] =
    "Usage: dodona [OPTIONS] COMMAND [ARGS]...\n"
    "\n"
    "  A 3rd party Command Line Interface for Dodona. Finally you have no need to\n"
    "  exit your terminal anymore! Use --help for more info about flags, or read\n"
    "  the README on discord.\n"
    "\n"
    "Options:\n"
    "  --help  Show this message and exit.\n"
    "\n"
    "Commands:\n"
    "  display   Display info based on the current selection.\n"
    "  info      Info about shell-completion, changelog, version,...\n"
    "  next      WARNING: might overwrite 'boilerplate' file! Move to the next...\n"
    "  post      Post a solution-file to Dodona.\n"
    "  select    Select based on id or name.\n"
    "  settings  Interactive settings-menu to change some settings\n"
    "  status    Display your current selection.\n"
    "  sub       Get submission data.\n"
    "  tutorial  Start tutorial\n"
    "  up        Deselect default last selected thing.\n";

void handleArgs(int, const char**);
