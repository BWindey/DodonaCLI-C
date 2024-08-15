# Compiler and flags
CC = gcc
CFLAGS = -I../include -I../../WiEnrich -Wall -Wextra -pedantic
LDFLAGS = -L../lib -lwiJSON -L../../WiEnrich/lib -lwienrich

# Directories, add new ones here
CORE_DIR = core
UTILITY_DIR = utility

OUT_CORE_DIR = $(CORE_DIR)/out
OUT_UTILITY_DIR = $(UTILITY_DIR)/out

# Sources ad executables
CORE_TESTS = $(wildcard $(CORE_DIR)/*.c)
UTILITY_TESTS = $(wildcard $(UTILITY_DIR)/*.c)

CORE_EXECUTABLES = $(patsubst $(CORE_DIR)/%.c,$(OUT_CORE_DIR)/%.out,$(CORE_TESTS))
UTILITY_EXECUTABLES = $(patsubst $(UTILITY_DIR)/%.c,$(OUT_UTILITY_DIR)/%.out,$(UTILITY_TESTS))


# Default target
all: $(CORE_EXECUTABLES) $(UTILITY_EXECUTABLES)
	@echo "\033[93;4mCore tests...\033[0m"
	@for test_exec in $(CORE_EXECUTABLES); do ./$$test_exec; done
	@echo "\033[93;4mUtility tests...\033[0m" 
	@for test_exec in $(UTILITY_EXECUTABLES); do ./$$test_exec; done

# Compile core tests
$(OUT_CORE_DIR)/%.out: $(CORE_DIR)/%.c | $(OUT_CORE_DIR)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

# Compile utility tests
$(OUT_UTILITY_DIR)/%.out: $(UTILITY_DIR)/%.c | $(OUT_UTILITY_DIR)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)


# Ensure output directories exist
$(OUT_CORE_DIR):
	mkdir -p $(OUT_CORE_DIR)

$(OUT_UTILITY_DIR):
	mkdir -p $(OUT_UTILITY_DIR)


clean: 
	rm -rf $(OUT_CORE_DIR) $(OUT_UTILITY_DIR)


.PHONY: all clean
