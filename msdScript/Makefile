# Define the compiler to use
CXX = g++

# Compiler flags
# -std=c++11 : Compile for C++11 standard
# -Wall : Turn on most compiler warnings
CXXFLAGS = -std=c++11 -Wall

# Define any directories containing header files
INCLUDES =

# Define source (.cpp) and header (.hpp) files
SRCS = main.cpp cmdline.cpp Expr.cpp catch.cpp
HEADERS = cmdline.h Expr.h

# Define the target executable
TARGET = msdscript

# Define the part of the makefile that compiles the executable
all: $(TARGET)

$(TARGET): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TARGET) $(SRCS)

# Define clean command
.PHONY: clean
clean:
	rm -f $(TARGET) *.o

# Define a phony target to avoid problems with files named "all" or "clean"
.PHONY: all clean

