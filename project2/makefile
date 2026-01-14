# Makefile for grades program
# Running 'make' will compile all source files into executable named 'grades'

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

# Target executable name
TARGET = grades

# Source files (all .c files)
SRCS = grades.c list.c validation.c database.c commands.c

# Object files (replace .c with .o)
OBJS = $(SRCS:.c=.o)

# Default target: build the executable
# This runs when you type 'make' with no arguments
all: $(TARGET)

# Link object files into final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile each .c file into a .o object file
# $< means the source file (.c)
# $@ means the target file (.o)
%.o: %.c grades.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up compiled files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets (not actual files)
.PHONY: all clean