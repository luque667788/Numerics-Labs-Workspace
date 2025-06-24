# Makefile for building the fit.cpp ellipse fitting demo
# This builds a C++ program that demonstrates ellipse fitting using LAPACK and FLTK

CXX       = g++
# Compiler flags:
# -Wall -Wextra: Enable comprehensive warnings
# -O2: Optimization level 2 for better performance
# `fltk-config --cxxflags`: Get proper FLTK compilation flags
# -std=c++11: Use C++11 standard for compatibility
CXXFLAGS  = -Wall -Wextra -O2 -std=c++11 `fltk-config --cxxflags`

# Linker flags:
# `fltk-config --ldflags`: Get proper FLTK linking flags  
# -llapacke: Link against LAPACKE library for linear algebra
# -llapack: Link against LAPACK library (backend for LAPACKE)
# -lblas: Link against BLAS library (basic linear algebra subprograms)
# -lgfortran: Link against Fortran runtime (LAPACK dependency)
LDFLAGS   = `fltk-config --ldflags` -llapacke -llapack -lblas -lgfortran

TARGET    = fit
SRCS      = fit.cpp
OBJS      = $(SRCS:.cpp=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(TARGET) $(OBJS)

run: $(TARGET)
	./$(TARGET)

# Additional targets for convenience
debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)

install-deps:
	@echo "Installing required dependencies..."
	@echo "On Ubuntu/Debian: sudo apt-get install libfltk1.3-dev liblapacke-dev"
	@echo "On Fedora/RHEL: sudo dnf install fltk-devel lapack-devel"
	@echo "On Arch: sudo pacman -S fltk lapack"

help:
	@echo "Available targets:"
	@echo "  all      - Build the program (default)"
	@echo "  run      - Build and run the program"
	@echo "  clean    - Remove built files"
	@echo "  debug    - Build with debug symbols"
	@echo "  install-deps - Show dependency installation commands"
	@echo "  help     - Show this help message"
