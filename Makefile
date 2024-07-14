CC = gcc
CFLAGS = -Wall -Wextra -std=c11

OBJ = arguments.o subcommands/info.o

EXEC = dodc

# Default target
all : $(EXEC)

# Link object files together
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC)

# Compile main
arguments.o: arguments.c arguments.h subcommands/info.h
	$(CC) $(CFLAGS) -c arguments.c -o arguments.o

# Compile subcommands
subcommands/info.o: subcommands/info.c subcommands/info.h
	$(CC) $(CFLAGS) -c subcommands/info.c -o subcommands/info.o


# Clean up
clean:
	rm -f $(OBJ) $(EXEC)

# Avoid confusion with actual files called "all" or "clean"
.PHONY: all clean
