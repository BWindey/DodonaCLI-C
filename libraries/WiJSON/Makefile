cc = GCC
CFLAGS = -Iinclude -Wall -Wextra -std=c99

SRC_DIR = src
OBJ_DIR = obj
TEST_DIR = test

SRC = $(wildcard $(SRC_DIR)/*.c) 
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

TEST_SRC = $(wildcard $(TEST_DIR)/*.c) 
TEST_OBJ = $(patsubst $(TEST_DIR)/%.c,$(OBJ_DIR)/%.o,$(TEST_SRC))
TEST_EXE = $(TEST_SRC:.c=.out)

LIBRARY = libWiJSON.a

# Default target: compile source into single object archive
all: $(LIBRARY)

# How to create library: bundle the object-files together
$(LIBRARY): $(OBJ)
	ar rcs $@ $^

# How to create the object files: with gcc -c <.c-files>
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c 
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@


# Test executables
$(TEST_EXE): $(TEST_OBJ) $(LIBRARY)
	$(CC) $(CFLAGS) $^ -o $@

# Target to run the tests
test: $(TEST_EXE)
	@echo "Running tests..."
	@for exe in $(TEST_EXE); do ./$$exe || exit 1; done
	@echo "All tests passed."


# Target to clean everything but the source-files
clean:
	rm -f $(OBJ) $(LIBRARY) $(TEST_OBJ) $(TEST_EXE)
	rm -rf $(OBJ_DIR)


.PHONY: all clean test

