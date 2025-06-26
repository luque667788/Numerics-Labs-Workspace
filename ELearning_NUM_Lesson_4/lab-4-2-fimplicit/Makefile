# Makefile for lab-4-2-fimplicit (Convex Hull)
# Builds the math executable from math.c

# Compiler and flags
CC = gcc
CFLAGS = -O2 -Wall -std=c99
LDFLAGS = 

# Target and source files
TARGET = math
SRC = math.c
OBJ = $(SRC:.c=.o)

# Default target
all: $(TARGET)

# Rule to link the executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Rule to compile C source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Target to run the executable
run: $(TARGET)
	./$(TARGET)

# Target to clean up build files
clean:
	rm -f $(OBJ) $(TARGET)

# Phony targets (targets that don't represent files)
.PHONY: all clean run
