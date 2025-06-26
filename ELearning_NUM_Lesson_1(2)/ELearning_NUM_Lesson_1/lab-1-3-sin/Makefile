CC      = gcc
CFLAGS  = -Wall -Wextra -O2
LDFLAGS = -lm

TARGET  = sin
SRCS    = sin.c
OBJS    = $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(TARGET) $(OBJS)
run: $(TARGET)
	./$(TARGET)