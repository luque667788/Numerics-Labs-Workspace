# Makefile for building the int.cpp FLTK demo

CXX       = g++
CXXFLAGS  = -Wall -Wextra -O2 `fltk-config --cxxflags`
LDFLAGS   = `fltk-config --ldflags`

TARGET    = lab-1-3-sin-simple-graph
SRCS      = lab-1-3-sin-simple-graph.cpp
OBJS      = $(SRCS:.cpp=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(TARGET) $(OBJS)
run: $(TARGET)
	./$(TARGET)
