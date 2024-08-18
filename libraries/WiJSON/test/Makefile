# Compiler and flags
CC = gcc
CFLAGS = -I../include -I../../WiEnrich -Wall -Wextra -pedantic
LDFLAGS = -L../lib -lwiJSON -L../../WiEnrich/lib -lwienrich

TESTING_LIB = ../../WiTesting/wiTest.h

# Directories, add new ones here
CORE_DIR = core
UTILITY_DIR = utility

CORE_STRING_DIR = $(CORE_DIR)/string
CORE_FILE_DIR = $(CORE_DIR)/file
UTILITY_STRING_DIR = $(UTILITY_DIR)/string
UTILITY_FILE_DIR = $(UTILITY_DIR)/file

OUT_CORE_STRING_DIR = $(CORE_STRING_DIR)/out
OUT_CORE_FILE_DIR = $(CORE_FILE_DIR)/out
OUT_UTILITY_STRING_DIR = $(UTILITY_STRING_DIR)/out
OUT_UTILITY_FILE_DIR = $(UTILITY_FILE_DIR)/out

# Sources and executables
CORE_STRING_TESTS = $(wildcard $(CORE_STRING_DIR)/*.c)
CORE_FILE_TESTS = $(wildcard $(CORE_FILE_DIR)/*.c)
UTILITY_STRING_TESTS = $(wildcard $(UTILITY_STRING_DIR)/*.c)
UTILITY_FILE_TESTS = $(wildcard $(UTILITY_FILE_DIR)/*.c)

CORE_STRING_EXECUTABLES = $(patsubst $(CORE_STRING_DIR)/%.c,$(OUT_CORE_STRING_DIR)/%.out,$(CORE_STRING_TESTS))
CORE_FILE_EXECUTABLES = $(patsubst $(CORE_FILE_DIR)/%.c,$(OUT_CORE_FILE_DIR)/%.out,$(CORE_FILE_TESTS))
UTILITY_STRING_EXECUTABLES = $(patsubst $(UTILITY_STRING_DIR)/%.c,$(OUT_UTILITY_STRING_DIR)/%.out,$(UTILITY_STRING_TESTS))
UTILITY_FILE_EXECUTABLES = $(patsubst $(UTILITY_FILE_DIR)/%.c,$(OUT_UTILITY_FILE_DIR)/%.out,$(UTILITY_FILE_TESTS))


# Default target
all: core utility


cleantest: clean all


# Only test core-tests
core: corestring corefile

corestring: $(CORE_STRING_EXECUTABLES)
	@echo ""
	@echo "\033[93;1mCore string tests...\033[0m"
	@for test_exec in $(CORE_STRING_EXECUTABLES); do \
		./$$test_exec; \
	done

corefile: $(CORE_FILE_EXECUTABLES)
	@echo ""
	@echo "\033[93;1mCore file tests...\033[0m"
	@for test_exec in $(CORE_FILE_EXECUTABLES); do \
		./$$test_exec; \
	done


# Only test utility-tests
utility: utilitystring utilityfile

utilitystring: $(UTILITY_STRING_EXECUTABLES)
	@echo ""
	@echo "\033[93;1mUtility string tests...\033[0m"
	@for test_exec in $(UTILITY_STRING_EXECUTABLES); do \
		./$$test_exec; \
	done

utilityfile: $(UTILITY_FILE_EXECUTABLES)
	@echo ""
	@echo "\033[93;1mUtility file tests...\033[0m"
	@for test_exec in $(UTILITY_FILE_EXECUTABLES); do \
		./$$test_exec; \
	done


# Compile core tests
$(OUT_CORE_STRING_DIR)/%.out: $(CORE_STRING_DIR)/%.c | $(OUT_CORE_STRING_DIR) $(TESTING_LIB)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

$(OUT_CORE_FILE_DIR)/%.out: $(CORE_FILE_DIR)/%.c | $(OUT_CORE_FILE_DIR) $(TESTING_LIB)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

# Compile utility tests
$(OUT_UTILITY_STRING_DIR)/%.out: $(UTILITY_STRING_DIR)/%.c | $(OUT_UTILITY_STRING_DIR) $(TESTING_LIB)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

$(OUT_UTILITY_FILE_DIR)/%.out: $(UTILITY_FILE_DIR)/%.c | $(OUT_UTILITY_FILE_DIR) $(TESTING_LIB)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)


# Ensure output directories exist
$(OUT_CORE_STRING_DIR):
	mkdir -p $(OUT_CORE_STRING_DIR)

$(OUT_CORE_FILE_DIR):
	mkdir -p $(OUT_CORE_FILE_DIR)

$(OUT_UTILITY_STRING_DIR):
	mkdir -p $(OUT_UTILITY_STRING_DIR)

$(OUT_UTILITY_FILE_DIR):
	mkdir -p $(OUT_UTILITY_FILE_DIR)


clean: 
	rm -rf $(OUT_CORE_STRING_DIR) $(OUT_CORE_FILE_DIR)
	rm -rf $(OUT_UTILITY_STRING_DIR) $(OUT_UTILITY_FILE_DIR)


.PHONY: all clean core utility corestring corefile utilitystring utilityfile cleantest 
