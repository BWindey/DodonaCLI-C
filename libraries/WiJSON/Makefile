# Compiler and flags
CC = gcc
CFLAGS = -Iinclude -I../WiEnrich -Wall -Wextra -pedantic
LDFLAGS = -L../WiEnrich/lib -lwienrich

# Directories
SRCDIR = src
INCDIR = include
OBJDIR = obj
LIBDIR = lib
WIENRICH_DIR = ../WiEnrich


# Library names
LIBRARY = $(LIBDIR)/libwiJSON.a
WIENRICH_LIB = $(WIENRICH_DIR)/lib/libwienrich.a

# Source and object files
SRC_FILES = $(wildcard $(SRCDIR)/*.c)
OBJ_FILES = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC_FILES))


# Default target
all: $(LIBRARY)

# Rule to create the static library for your project
$(LIBRARY): $(OBJ_FILES)
	@mkdir -p $(LIBDIR)
	ar rcs $@ $^

# Rule to build the WiEnrich library
$(WIENRICH_LIB):
	$(MAKE) -C $(WIENRICH_DIR)

# Rule to compile object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure obj directory exists
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Rule to build and run tests
test: $(LIBRARY) $(WIENRICH_LIB)
	$(MAKE) -C test


# Clean up
clean:
	rm -rf $(OBJDIR) $(LIBRARY) $(TEST_EXECUTABLES) $(LIBDIR)
	$(MAKE) -C $(WIENRICH_DIR) clean
	$(MAKE) -C test clean

.PHONY: test clean all
