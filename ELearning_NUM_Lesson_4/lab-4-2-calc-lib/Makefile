# Makefile for lab-4-2-calc-lib

CC = gcc
CFLAGS = -Wall -O2 -llapacke -llapack -lblas -lm
TARGET = math
SRC = math.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o
