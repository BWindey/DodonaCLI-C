CC = gcc
CFLAGS = -Wall -Wextra -std=c99
SRCDIR = .
SUBCMDIR = $(SRCDIR)/subcommands
LIBDIR = $(SRCDIR)/libraries
OBJDIR = $(SRCDIR)/obj

SRC = $(SRCDIR)/main.c \
      $(SUBCMDIR)/display.c \
      $(SUBCMDIR)/info.c \
      $(SUBCMDIR)/next.c \
      $(SUBCMDIR)/post.c \
      $(SUBCMDIR)/select.c \
      $(SUBCMDIR)/settings.c \
      $(SUBCMDIR)/status.c \
      $(SUBCMDIR)/sub.c \
      $(SUBCMDIR)/tutorial.c \
      $(SUBCMDIR)/up.c \
      $(LIBDIR)/cJSON/cJSON.c \
      $(LIBDIR)/Enrich/enrich.c

OBJ = $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

EXEC = dodc

# Default target
all: $(EXEC)

# Link object files together
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC)

# Pattern rule for object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJ) $(EXEC)
	rm -rf $(OBJDIR)

# Avoid confusion with actual files called "all" or "clean"
.PHONY: all clean
