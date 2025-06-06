# Makefile for lab-4-1-calc-lib
# Builds the math executable from math.c and links with LAPACK and BLAS

CC = gcc
CFLAGS = -O2 -Wall
LDFLAGS = -llapacke -llapack -lblas
TARGET = math
SRC = math.c
OBJ = math.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $<

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean run
