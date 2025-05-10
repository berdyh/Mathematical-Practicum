# Compiler and flags
CXX       := g++
CXXFLAGS  := -std=c++17 -Wall -Wextra -Wpedantic -O2 -Iinclude

# Source files
SRCDIR    := src
TESTDIR   := test

SRC_CPP   := $(wildcard $(SRCDIR)/*.cpp)
TEST_CPP  := $(wildcard $(TESTDIR)/main.cpp)
SRCS      := $(SRC_CPP) $(TEST_CPP)

# Object files: .cpp → .o
OBJS      := $(SRCS:.cpp=.o)

# Precompiled unit tests object
UNIT_OBJ  := include/unit.o

# Final executable
TARGET    := test/sort

# Default target
all: $(TARGET)

# Link everything
$(TARGET): $(OBJS) $(UNIT_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile .cpp → .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
