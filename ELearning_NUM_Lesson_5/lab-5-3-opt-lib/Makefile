# Makefile for lab-5-3-opt-lib
# Builds the opt executable from opt.c and links with NLopt library

CC = gcc
CFLAGS = -O2 -Wall
LDFLAGS = -lnlopt
TARGET = opt
SRC = opt.c
OBJ = opt.o

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
