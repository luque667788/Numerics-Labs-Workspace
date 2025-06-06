# Makefile for lab-4-1-fit (FLTK)
# Builds the fit executable from fit.cpp and links with FLTK libraries

# Compiler and flags
CXX = g++
# Use fltk-config to get the correct FLTK compiler and linker flags
CXXFLAGS = $(shell fltk-config --cxxflags) -O2 -Wall
LDFLAGS = $(shell fltk-config --ldflags)

# Target and source files
TARGET = fit
SRC = fit.cpp
OBJ = $(SRC:.cpp=.o)

# Default target
all: $(TARGET)

# Rule to link the executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Rule to compile C++ source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Target to run the executable
run: $(TARGET)
	./$(TARGET)

# Target to clean up build files
clean:
	rm -f $(OBJ) $(TARGET)

# Phony targets (targets that don't represent files)
.PHONY: all clean run
