# Compiler and flags
CC = gcc
CFLAGS = -Iinclude -I../WiEnrich -Wall -Wextra -pedantic
LDFLAGS = -L../WiEnrich/lib -lwienrich

# Directories
SRCDIR = src
INCDIR = include
OBJDIR = obj
TESTDIR = test
WIENRICH_DIR = ../WiEnrich

# Library names
LIBRARY = libwiJSON.a
WIENRICH_LIB = $(WIENRICH_DIR)/lib/libwienrich.a

# Source and object files
SRC_FILES = $(wildcard $(SRCDIR)/*.c)
OBJ_FILES = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC_FILES))

# Test files and their executables
TEST_SRC_FILES = $(wildcard $(TESTDIR)/*.c)
TEST_EXECUTABLES = $(patsubst $(TESTDIR)/%.c,$(TESTDIR)/%.out,$(TEST_SRC_FILES))

# Default target
all: $(LIBRARY)

# Rule to create the static library for your project
$(LIBRARY): $(OBJ_FILES)
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
test: $(WIENRICH_LIB) $(TEST_EXECUTABLES)
	for test_exec in $(TEST_EXECUTABLES); do ./$$test_exec; done

# Rule to compile test executables
$(TESTDIR)/%.out: $(TESTDIR)/%.c $(LIBRARY) $(WIENRICH_LIB)
	$(CC) $(CFLAGS) $< -o $@ -L. -lwiJSON $(LDFLAGS)

# Clean up
clean:
	rm -rf $(OBJDIR) $(LIBRARY) $(TEST_EXECUTABLES)
	$(MAKE) -C $(WIENRICH_DIR) clean

.PHONY: test clean all
