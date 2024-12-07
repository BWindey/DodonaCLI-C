CC = gcc
CFLAGS = -Iinclude -Iinclude/subcommands -Wall -Wextra -std=c99

EXEC := dodc

# Directory structure
INCLUDEDIR := include
SRCDIR := src
LIBDIR := libraries
SUBCMDIR := $(SRCDIR)/subcommands

# All the libraries with lib/ folder and include/ folder
LIBRARIES_DIRS := $(LIBDIR)/WiEnrich \
				  $(LIBDIR)/WiJSON \
				  $(LIBDIR)/WiTUI

# Library with only 2 header files
CFLAGS += -I$(LIBDIR)/WiTesting


# -- All the Make magic -- #

# Get all source files
SRC = $(SRCDIR)/main.c \
	  $(wildcard $(SUBCMDIR)/*.c)

# Where the temporary object files will go, and how they look
OBJDIR = obj
OBJ = $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Expected folders in a library:
# - lib (with libxxx.a file in)
# - include (with all header files in)
# Also need a Makefile that can generate the libxxx.a file

# Add -I include paths for each library
CFLAGS += $(foreach dir,$(LIBRARIES_DIRS),-I$(dir)/include)

# Get all libxxx.a files
LIBRARIES = $(foreach dir,$(LIBRARIES_DIRS),-L$(dir)/lib -l$(shell basename $(dir) | tr '[:upper:]' '[:lower:]'))


# Default target
all: libraries $(EXEC)

# Link object files together
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC) $(LIBRARIES)

# Pattern rule for object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBRARIES)


# Target for the libraries
libraries: $(LIBRARIES_DIRS)
	for dir in $(LIBRARIES_DIRS); do \
		$(MAKE) -C $$dir; \
	done

$(LIBRARIES_DIRS):
	$(MAKE) -c $@

# Clean up
clean:
	rm -f $(OBJ) $(EXEC)
	rm -rf $(OBJDIR)
	for dir in $(LIBRARIES_DIRS); do \
		$(MAKE) -C $$dir clean || true; \
	done

# Avoid confusion with actual files called "all" or "clean"
.PHONY: all clean libraries
